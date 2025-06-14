#!/usr/local/bin/python
__author__ = 'yankai'

import re
import sys
import xlrd
import argparse
import register
import os
import peripherals

class Prometheus:
    def __init__(self):
        self.name = ''
        self.regs = []        

    # ==========================================================
    # Name: parse_sheet(self, regmap)
    # Para: regmap - register map file in excel spreadsheet
    # Func: parse spreadsheet to load all reg/field info
    # ==========================================================
    def parse_sheet(self, regmap):
        periph_name = re.search(r'[/\\](\w*)_regmap\.xlsx', regmap)
        if periph_name is None:
            return 
        print(regmap)
        self.name = re.sub('.xlsx|_regmap', '', periph_name.group(1))
        try:
            book = xlrd.open_workbook(regmap)
            sheet = book.sheet_by_index(0)
        finally:
            pass

        # process excel spreadsheet
        for i in range(sheet.nrows):
            line = sheet.row_values(i)
            # in case of unexpected space
            for k in range(len(line)):
                line[k].strip()
            # create registers
            m = re.match(r'0x\w+', line[0])
            if m is not None:
                offset = line[0].replace('0x', '')  # remove '0x'
                value = line[2].replace('0x', '')   # remove '0x'
                value.replace('_', '')              # remove '_' in 0x0000_0000
                name = line[3]
                description = line[4]

                reg = register.Register32(name, offset, value)
                reg.set_description(description)
                reg.set_rprop(line[5])
                reg.set_cprop(line[6])
                self.regs.append(reg)
                continue

            # process the following fields
            m1 = re.match(r'\[(\d+):(\d+)\]', line[0])
            m2 = re.match(r'\[(\d+)\]', line[0])
            if m1 is not None:
                start = int(m1.group(1))
                stop = int(m1.group(2))
                rw = line[1]
                value = line[2]
                name = line[3]
                description = line[4]
                rprop = line[5]
                cprop = line[6]
            elif m2 is not None:
                start = int(m2.group(1))
                stop = start
                rw = line[1]
                value = line[2]
                name = line[3]
                description = line[4]
                rprop = line[5]
                cprop = line[6]
            else:
                print 'Warning: unrecognized row: %d' % (i+1)   # excel starts with row 1
                continue
            
            
            field = register.Field(name, start, stop, rw, value)
            field.set_rprop(rprop)
            field.set_cprop(cprop)
            field.set_description(description)
            self.regs[-1].add_field(field)

        # validate all registers
        rsvd_reg = 1
        for i in range(len(self.regs)-1, -1, -1):
            # check reg itself
            if not self.regs[i].validate():
                print 'Error! Invalid register:', self.regs[i].name
                sys.exit()

            # check overlap/gap between regs
            if i == 0:
                gap = (int(self.regs[i].offset, 16))/4
            else:
                gap = (int(self.regs[i].offset, 16) - int(self.regs[i-1].offset, 16))/4 - 1

            if gap < 0:
                print 'Error! Register offset overlap %s @ 0x%s' % (self.regs[i].name, self.regs[i].offset)
                sys.exit()
            elif gap > 0:
                print 'Warning! Register offset gap %s @ 0x%s' % (self.regs[i].name, self.regs[i].offset)
                if args.type == 'header':
                    reg = register.Register32('RSVD{}'.format(rsvd_reg), 'null', gap)
                else:    
                    reg = register.Register32('RSVD{}'.format(rsvd_reg), '{:X}'.format(int(self.regs[i].offset, 16) - gap*4), '0')
                self.regs.insert(i, reg)
                rsvd_reg += 1

    # ==========================================================
    # Name: generate_apb(self)
    # Func: generate the APB registers RTL
    # ==========================================================
    def generate_apb(self):
        # calculate the paddr bit width
        addr_bw = 0
        max_offset = int(self.regs[-1].offset, 16)
        while max_offset > 0:
            addr_bw += 1
            max_offset /= 2

        # write out RTL file
        with open('./{}_registers.v'.format(self.name), 'w') as f:
            space = ' '

            # region: ----------------------- Port List -----------------------
            f.write('module %s_registers (\n' % self.name)
            # APB bus signals
            f.write('  // APB bus signals\n')
            f.write('  input       %8s  pclk,\n' % space)
            f.write('  input       %8s  presetn,\n' % space)
            f.write('  input       %8s  psel,\n' % space)
            f.write('  input       %8s  penable,\n' % space)
            f.write('  input       %8s  pwrite,\n' % space)
            f.write('  input       %8s  paddr,\n' % '[{}:0]'.format(addr_bw-1))
            f.write('  input       %8s  pwdata,\n' % '[31:0]')
            f.write('  output  reg %8s  prdata,\n' % '[31:0]')
            f.write('  output      %8s  pready,\n' % space)
            f.write('  output      %8s  pslverr,\n' % space)
            f.write('  output      %8s  pclk_gated,\n' % space)
            # User defined registers
            f.write('  // User defined registers\n')
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('  // 0x%s %s\n' % (reg.offset, reg.name.upper()))

                # deal with special RTL property for register
                if reg.rprop == 'decr':
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_read'))
                elif reg.rprop == 'decw':
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_write'))
                elif reg.rprop == 'decrw':
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_read'))
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_write'))

                # deal with each field
                for field in reg.fields:
                    port = reg.name + '_' + field.name
                    if field.bw == 1:
                        bw = space
                    else:
                        bw = '[{}:0]'.format(field.bw-1)

                    if field.rw == 'r':
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw':
                        f.write('  output  reg %8s  %s,\n' % (bw, port))
                    elif field.rw == 'rw_ext':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w'))
                        f.write('  output      %8s  %s,\n' % (bw, port+'_wdata'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'w':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w'))
                        f.write('  output      %8s  %s,\n' % (bw, port+'_wdata'))
                    elif field.rw == 'rwo':
                        f.write('  output      %8s  %s,\n' % (space, port+'_wo'))
                        f.write('  output      %8s  %s,\n' % (bw, port+'_wdata'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw0c':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w0c'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw1c':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1c'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rwc':
                        f.write('  output      %8s  %s,\n' % (space, port+'_wc'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rc':
                        f.write('  output      %8s  %s,\n' % (space, port+'_rc'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rs':
                        f.write('  output      %8s  %s,\n' % (space, port+'_rs'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw1s':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1s'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'w1t':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1t'))
                    elif field.rw == 'w1c':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1c'))
                    elif field.rw == 'w1s':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1s'))

            # Scan signals
            f.write('  // Scan signals\n')
            f.write('  input       %8s  scan_en\n' % space)
            f.write(');\n\n')
            # endregion

            # region: ------------------ Signal Declaration -------------------
            f.write('  // Wire/Reg Declaration\n')
            f.write('  reg %8s  state;\n' % space)
            f.write('  reg %8s  state_next;\n' % space)
            f.write('  reg %8s  paddr_msb;\n' % ('[{}:0]'.format(addr_bw-3)))
            f.write('  reg %8s  paddr_next;\n' % ('[{}:0]'.format(addr_bw-3)))
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('  reg %8s  %s_decode;\n' % (space, reg.name))
            f.write('  wire %7s  pclk_en;\n' % space)
            f.write('  wire %7s  is_access;\n' % space)
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('  wire %7s  %s_rdata;\n' % ('[31:0]', reg.name))
            # endregion

            # region: --------------------- Clock Gating ----------------------
            f.write('\n')
            f.write('  // ================== Clock Gating ==================\n')
            f.write('  assign pclk_en = psel;\n')
            f.write('\n')
            f.write('  macro_clkgate_te #(.FPGA_BP_CG(0))\n')
            f.write('  u_cg_pclk (\n')
            f.write('    .en         ( pclk_en    ),\n')
            f.write('    .test_en    ( scan_en    ),\n')
            f.write('    .clk_in     ( pclk       ),\n')
            f.write('    .clk_out    ( pclk_gated )\n')
            f.write('  );\n\n')
            # endregion

            # region: ------------------ FSM Implementation -------------------
            f.write('  // =============== FSM Implementation ===============\n')
            f.write("  localparam ST_IDLE   = 1'b0;\n")
            f.write("  localparam ST_ACCESS = 1'b1;\n\n")
            # sequential logic
            f.write('  // sequential\n')
            f.write('  always @(posedge pclk_gated or negedge presetn) begin\n')
            f.write('    if (!presetn) begin\n')
            f.write('      state     <= `CK2Q ST_IDLE;\n')
            f.write("      paddr_msb <= `CK2Q %d'b0;\n" % (addr_bw-2))
            f.write('    end\n')
            f.write('    else begin\n')
            f.write('      state     <= `CK2Q state_next;\n')
            f.write('      paddr_msb <= `CK2Q paddr_next;\n')
            f.write('    end\n')
            f.write('  end\n\n')
            # combinational logic
            f.write('  // combinational\n')
            f.write('  always @(*) begin\n')
            f.write('    case (state)\n')
            f.write('      ST_IDLE: begin\n')
            f.write('        state_next = (psel && !penable) ? ST_ACCESS : state;\n')
            f.write('        paddr_next = (psel && !penable) ? paddr[%d:2] : paddr_msb;\n' % (addr_bw-1))
            f.write('      end\n')
            f.write('      ST_ACCESS: begin\n')
            f.write('        state_next = ST_IDLE;\n')
            f.write('        paddr_next = paddr_msb;\n')
            f.write('      end\n')
            f.write('    endcase\n')
            f.write('  end\n\n')
            f.write("  assign pready = 1'b1;\n")
            f.write("  assign pslverr = 1'b0;\n")
            f.write('  assign is_access = (state == ST_ACCESS);\n\n')
            # endregion

            # region: ---------------- Registers Implementation ---------------
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('  // {:=^50}\n'.format(' {} @ 0x{} '.format(reg.name.upper(), reg.offset)))

                # region: --------- concatenate all the fields to form a 32-bit prdata ---------
                # if single field, then assign xxx_rdata = xxx_xxx without {};
                #   A - r/rw0c/rw1c/rwc/rc/rs/rw1s/rwo/rw_ext fields use external rdata
                #   B - rw fields use internal DFF data
                #   C - w/w1t/w1c/w1s/rsvd fields use default reset value
                # if multiple fields, then assign xxx_rdata = {xxx_xxx, xxx_xxx ...} with {};
                # ------------------------------------------------------------------------------
                alist = ['r','rw0c','rw1c','rwc','rc','rs','rw1s','rwo','rw_ext']
                blist = ['rw']
                clist = ['w','w1t','w1c','w1s','rsvd']

                if len(reg.fields) == 1:
                    if reg.fields[0].rw in alist:
                        f.write('  assign %s_rdata = %s_%s_rdata;\n\n' % (reg.name, reg.name, reg.fields[0].name))
                    elif reg.fields[0].rw in blist:
                        f.write('  assign %s_rdata = %s_%s;\n\n' % (reg.name, reg.name, reg.fields[0].name))
                    elif reg.fields[0].rw in clist:
                        f.write('  assign %s_rdata = %s;\n\n' % (reg.name, reg.fields[0].value))
                    else:   # in case a/b/c list not full set
                        print 'Error! Unrecognized %s attribute of %s' % (reg.fields[0].rw, reg.fields[0].name)
                        sys.exit()
                else:
                    f.write('  assign %s_rdata = { ' % reg.name)
                    for i in range(len(reg.fields)):
                        if reg.fields[i].rw in alist:
                            s = '%s_%s_rdata' % (reg.name, reg.fields[i].name)
                        elif reg.fields[i].rw in blist:
                            s = '%s_%s' % (reg.name, reg.fields[i].name)
                        elif reg.fields[i].rw in clist:
                            s = reg.fields[i].value
                        else:   # in case a/b/c list not full set
                            print 'Error! Unrecognized %s attribute of %s' % (reg.fields[i].rw, reg.fields[i].name)
                            sys.exit()

                        if i == len(reg.fields) - 1:
                            f.write('{}\n{:>{width}}}};\n\n'.format(s, ' ', width=len(reg.name)+18))
                        else:
                            f.write('{},\n{:>{width}}'.format(s, ' ', width=len(reg.name)+20))
                # endregion

                # region: ----------------------- Sequential logic -----------------------------
                # Only fields with rw properties are implemented internally
                # ------------------------------------------------------------------------------
                width = reg.max_seq + len(reg.name) + 1
                if reg.seq == 1:
                    f.write('  always @(posedge pclk_gated or negedge presetn) begin\n')
                    f.write('    if (!presetn) begin\n')
                    # generate reset values
                    for field in reg.fields:
                        if field.seq == 1:
                            dff = reg.name + '_' + field.name
                            f.write('      {:<{width}} <= `CK2Q {value};\n'.format(dff, width=width, value=field.value))
                    f.write('    end\n')
                    f.write('    else if (is_access && pwrite && %s_decode) begin\n' % reg.name)
                    # generate write logic
                    for field in reg.fields:
                        if field.seq == 1:
                            dff = reg.name + '_' + field.name
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('      {:<{width}} <= `CK2Q pwdata[{}];\n'.format(dff, index, width=width))
                    f.write('    end\n')
                    f.write('  end\n\n')

                # endregion

                # region: --------------------- Combinational logic ----------------------------------------------
                # 1. If register has decr/decw property
                # 2. Deal with fields in ['w','rw0c','rw1c','rwc','rc','rs','rw1s','rwo','w1t','w1c','w1s','rw_ext']
                # ------------------------------------------------------------------------------------------------
                width = reg.max_comb + len(reg.name) + 7

                if reg.comb == 1:
                    if reg.rprop == 'decr':
                        sig = reg.name + '_read'
                        f.write('  assign {:<{width}} = is_access & {}_decode & ~pwrite;\n'
                                .format(sig, reg.name, width=width))
                    elif reg.rprop == 'decw':
                        sig = reg.name + '_write'
                        f.write('  assign {:<{width}} = is_access & {}_decode & pwrite;\n'
                                .format(sig, reg.name, width=width))
                    elif reg.rprop == 'decrw':
                        sig = reg.name + '_read'
                        f.write('  assign {:<{width}} = is_access & {}_decode & ~pwrite;\n'
                                .format(sig, reg.name, width=width))
                        sig = reg.name + '_write'
                        f.write('  assign {:<{width}} = is_access & {}_decode & pwrite;\n'
                                .format(sig, reg.name, width=width))

                    for field in reg.fields:
                        if field.rw == 'rw0c':
                            sig = reg.name + '_' + field.name + '_w0c'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite & ~pwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'rw1c':
                            sig = reg.name + '_' + field.name + '_w1c'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite & pwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'rwc':
                            sig = reg.name + '_' + field.name + '_wc'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite;\n'
                                    .format(sig, reg.name, width=width))
                        elif field.rw == 'rw1s':
                            sig = reg.name + '_' + field.name + '_w1s'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite & pwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'rc':
                            sig = reg.name + '_' + field.name + '_rc'
                            f.write('  assign {:<{width}} = is_access & {}_decode & ~pwrite;\n'
                                    .format(sig, reg.name, width=width))
                        elif field.rw == 'rs':
                            sig = reg.name + '_' + field.name + '_rs'
                            f.write('  assign {:<{width}} = is_access & {}_decode & ~pwrite;\n'
                                    .format(sig, reg.name, width=width))
                        elif field.rw == 'w1t':
                            sig = reg.name + '_' + field.name + '_w1t'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite & pwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'w1c':
                            sig = reg.name + '_' + field.name + '_w1c'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite & pwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'w1s':
                            sig = reg.name + '_' + field.name + '_w1s'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite & pwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'w':
                            sig = reg.name + '_' + field.name + '_w'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite;\n'
                                    .format(sig, reg.name, width=width))
                            sig = reg.name + '_' + field.name + '_wdata'
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('  assign {:<{width}} = pwdata[{}];\n'.format(sig, index, width=width))
                        elif field.rw == 'rwo':
                            sig = reg.name + '_' + field.name + '_wo'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite;\n'
                                    .format(sig, reg.name, width=width))
                            sig = reg.name + '_' + field.name + '_wdata'
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('  assign {:<{width}} = pwdata[{}];\n'.format(sig, index, width=width))
                        elif field.rw == 'rw_ext':
                            sig = reg.name + '_' + field.name + '_w'
                            f.write('  assign {:<{width}} = is_access & {}_decode & pwrite;\n'
                                    .format(sig, reg.name, width=width))
                            sig = reg.name + '_' + field.name + '_wdata'
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('  assign {:<{width}} = pwdata[{}];\n'.format(sig, index, width=width))
                    f.write('\n')
                # endregion
            # endregion

            # region: -------------------- Address Decoder --------------------
            f.write('  // ================ Address Decoder =================\n')
            f.write('  always @(*) begin\n')
            f.write("    prdata = 32'h0;\n")
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write("    {}_decode = 1'b0;\n".format(reg.name))
            f.write('\n')
            f.write('    case (paddr_msb)\n')
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('      // {} @ 0x{}\n'.format(reg.name.upper(), reg.offset))
                f.write("      {width}'b{:0{width}b}: begin\n".format(int(reg.offset, 16)/4, width=addr_bw-2))
                f.write("        {}_decode = 1'b1;\n".format(reg.name))
                f.write('        prdata = {}_rdata;\n'.format(reg.name))
                f.write('      end\n')
            f.write('      default:;\n')
            f.write('    endcase\n')
            f.write('  end\n\n')
            f.write('endmodule\n')
            # endregion

        f.close()
        print 'Congratulations! RTL generated successfully'

    # ==========================================================
    # Name: generate_ahb(self)
    # Func: generate the AHB registers RTL
    # ==========================================================
    def generate_ahb(self):
        # calculate the haddr bit width
        addr_bw = 0
        max_offset = int(self.regs[-1].offset, 16)
        while max_offset > 0:
            addr_bw += 1
            max_offset /= 2

        # write out RTL file
        with open('./{}_registers.v'.format(self.name), 'w') as f:
            space = ' '

            # region: ----------------------- Port List -----------------------
            f.write('module %s_registers (\n' % self.name)
            # APB bus signals
            f.write('  // AHB bus signals\n')
            f.write('  input       %8s  hclk,\n' % space)
            f.write('  input       %8s  hresetn,\n' % space)
            f.write('  input       %8s  hsel,\n' % space)
            f.write('  input       %8s  htrans,\n' % '[1:0]')
            f.write('  input       %8s  hready,\n' % space)
            f.write('  input       %8s  haddr,\n' % '[{}:0]'.format(addr_bw-1))
            f.write('  input       %8s  hwrite,\n' % space)
            f.write('  input       %8s  hwdata,\n' % '[31:0]')
            f.write('  output  reg %8s  hrdata,\n' % '[31:0]')
            f.write('  output      %8s  hreadyout,\n' % space)
            f.write('  output      %8s  hresp,\n' % space)
            f.write('  output      %8s  hclk_gated,\n' % space)
            # User defined registers
            f.write('  // User defined registers\n')
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('  // 0x%s %s\n' % (reg.offset, reg.name.upper()))

                # deal with special RTL property for register
                if reg.rprop == 'decr':
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_read'))
                elif reg.rprop == 'decw':
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_write'))
                elif reg.rprop == 'decrw':
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_read'))
                    f.write('  output      %8s  %s,\n' % (space, reg.name+'_write'))

                # deal with each field
                for field in reg.fields:
                    port = reg.name + '_' + field.name
                    if field.bw == 1:
                        bw = space
                    else:
                        bw = '[{}:0]'.format(field.bw-1)

                    if field.rw == 'r':
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw':
                        f.write('  output  reg %8s  %s,\n' % (bw, port))
                    elif field.rw == 'rw_ext':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w'))
                        f.write('  output      %8s  %s,\n' % (bw, port+'_wdata'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'w':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w'))
                        f.write('  output      %8s  %s,\n' % (bw, port+'_wdata'))
                    elif field.rw == 'rwo':
                        f.write('  output      %8s  %s,\n' % (space, port+'_wo'))
                        f.write('  output      %8s  %s,\n' % (bw, port+'_wdata'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw0c':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w0c'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw1c':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1c'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rwc':
                        f.write('  output      %8s  %s,\n' % (space, port+'_wc'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rc':
                        f.write('  output      %8s  %s,\n' % (space, port+'_rc'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rs':
                        f.write('  output      %8s  %s,\n' % (space, port+'_rs'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'rw1s':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1s'))
                        f.write('  input       %8s  %s,\n' % (bw, port+'_rdata'))
                    elif field.rw == 'w1t':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1t'))
                    elif field.rw == 'w1c':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1c'))
                    elif field.rw == 'w1s':
                        f.write('  output      %8s  %s,\n' % (space, port+'_w1s'))

            # Scan signals
            f.write('  // Scan signals\n')
            f.write('  input       %8s  scan_en\n' % space)
            f.write(');\n\n')
            # endregion

            # region: ------------------ Signal Declaration -------------------
            f.write('  // Wire/Reg Declaration\n')
            f.write('  wire %7s  ahb_acc;\n' % space)
            f.write('  wire %7s  ahb_wr_acc;\n' % space)
            f.write('  wire %7s  ahb_rd_acc;\n' % space)
            f.write('  wire %7s  hclk_en;\n' % space)
            f.write('  reg  %7s  read;\n' % space)
            f.write('  reg  %7s  write;\n' % space)
            f.write('  reg  %7s  haddr_msb;\n' % ('[{}:0]'.format(addr_bw-3)))
            for reg in self.regs:
                if reg.offset != 'null':
                    f.write('  reg  %7s  %s_decode;\n' % (space, reg.name))
            for reg in self.regs:
                if reg.offset != 'null':
                    f.write('  wire %7s  %s_rdata;\n' % ('[31:0]', reg.name))
            # endregion

            # region: --------------------- Clock Gating ----------------------
            f.write('\n')
            f.write('  // ================== Clock Gating ==================\n')
            f.write('  assign hclk_en = ahb_acc | read | write;\n')
            f.write('\n')
            f.write('  macro_clkgate_te #(.FPGA_BP_CG(0))\n')
            f.write('  u_cg_hclk (\n')
            f.write('    .en         ( hclk_en    ),\n')
            f.write('    .test_en    ( scan_en    ),\n')
            f.write('    .clk_in     ( hclk       ),\n')
            f.write('    .clk_out    ( hclk_gated )\n')
            f.write('  );\n\n')
            # endregion

            # region: ----------------- ahb2reg conversion --------------------
            f.write('  // =============== ahb2reg conversion ===============\n')
            f.write('  // access decode\n')
            f.write('  assign ahb_acc = hsel & htrans[1] & hready;\n')
            f.write('  assign ahb_wr_acc = ahb_acc & hwrite;\n')
            f.write('  assign ahb_rd_acc = ahb_acc & ~hwrite;\n')
            f.write('\n')
            f.write('  // buffered enable\n')
            f.write('  always @(posedge hclk_gated or negedge hresetn) begin\n')
            f.write('    if (!hresetn) begin\n')
            f.write('      read  <= `CK2Q 1\'b0;\n')
            f.write('      write <= `CK2Q 1\'b0;\n')
            f.write('    end\n')
            f.write('    else begin\n')
            f.write('      read  <= `CK2Q ahb_rd_acc;\n')
            f.write('      write <= `CK2Q ahb_wr_acc;\n')
            f.write('    end\n')
            f.write('  end\n')
            f.write('\n')
            f.write('  // buffered address\n')
            f.write('  always @(posedge hclk_gated or negedge hresetn) begin\n')
            f.write('    if (!hresetn)\n')
            f.write('      haddr_msb <= `CK2Q 32\'b0;\n')
            f.write('    else if (ahb_acc)\n')
            f.write('      haddr_msb <= `CK2Q haddr[%d:2];\n' % (addr_bw-1))
            f.write('  end\n')
            f.write('\n')
            f.write('  assign hreadyout = 1\'b1;\n')
            f.write('  assign hresp     = 1\'b0;\n')
            f.write('\n\n')
            # endregion

            # region: ---------------- Registers Implementation ---------------
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('  // {:=^50}\n'.format(' {} @ 0x{} '.format(reg.name.upper(), reg.offset)))

                # region: --------- concatenate all the fields to form a 32-bit prdata ---------
                # if single field, then assign xxx_rdata = xxx_xxx without {};
                #   A - r/rw0c/rw1c/rwc/rc/rs/rw1s/rwo/rw_ext fields use external rdata
                #   B - rw fields use internal DFF data
                #   C - w/w1t/w1c/w1s/rsvd fields use default reset value
                # if multiple fields, then assign xxx_rdata = {xxx_xxx, xxx_xxx ...} with {};
                # ------------------------------------------------------------------------------
                alist = ['r','rw0c','rw1c','rwc','rc','rs','rw1s','rwo','rw_ext']
                blist = ['rw']
                clist = ['w','w1t','w1c','w1s','rsvd']

                if len(reg.fields) == 1:
                    if reg.fields[0].rw in alist:
                        f.write('  assign %s_rdata = %s_%s_rdata;\n\n' % (reg.name, reg.name, reg.fields[0].name))
                    elif reg.fields[0].rw in blist:
                        f.write('  assign %s_rdata = %s_%s;\n\n' % (reg.name, reg.name, reg.fields[0].name))
                    elif reg.fields[0].rw in clist:
                        f.write('  assign %s_rdata = %s;\n\n' % (reg.name, reg.fields[0].value))
                    else:   # in case a/b/c list not full set
                        print 'Error! Unrecognized %s attribute of %s' % (reg.fields[0].rw, reg.fields[0].name)
                        sys.exit()
                else:
                    f.write('  assign %s_rdata = { ' % reg.name)
                    for i in range(len(reg.fields)):
                        if reg.fields[i].rw in alist:
                            s = '%s_%s_rdata' % (reg.name, reg.fields[i].name)
                        elif reg.fields[i].rw in blist:
                            s = '%s_%s' % (reg.name, reg.fields[i].name)
                        elif reg.fields[i].rw in clist:
                            s = reg.fields[i].value
                        else:   # in case a/b/c list not full set
                            print 'Error! Unrecognized %s attribute of %s' % (reg.fields[i].rw, reg.fields[i].name)
                            sys.exit()

                        if i == len(reg.fields) - 1:
                            f.write('{}\n{:>{width}}}};\n\n'.format(s, ' ', width=len(reg.name)+18))
                        else:
                            f.write('{},\n{:>{width}}'.format(s, ' ', width=len(reg.name)+20))
                # endregion

                # region: ----------------------- Sequential logic -----------------------------
                # Only fields with rw properties are implemented internally
                # ------------------------------------------------------------------------------
                width = reg.max_seq + len(reg.name) + 1
                if reg.seq == 1:
                    f.write('  always @(posedge hclk_gated or negedge hresetn) begin\n')
                    f.write('    if (!hresetn) begin\n')
                    # generate reset values
                    for field in reg.fields:
                        if field.seq == 1:
                            dff = reg.name + '_' + field.name
                            f.write('      {:<{width}} <= `CK2Q {value};\n'.format(dff, width=width, value=field.value))
                    f.write('    end\n')
                    f.write('    else if (write && %s_decode) begin\n' % reg.name)
                    # generate write logic
                    for field in reg.fields:
                        if field.seq == 1:
                            dff = reg.name + '_' + field.name
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('      {:<{width}} <= `CK2Q hwdata[{}];\n'.format(dff, index, width=width))
                    f.write('    end\n')
                    f.write('  end\n\n')

                # endregion

                # region: --------------------- Combinational logic ----------------------------------------------
                # 1. If register has decr/decw property
                # 2. Deal with fields in ['w','rw0c','rw1c','rwc','rc','rs','rw1s','rwo','w1t','w1c','w1s','rw_ext']
                # ------------------------------------------------------------------------------------------------
                width = reg.max_comb + len(reg.name) + 7

                if reg.comb == 1:
                    if reg.rprop == 'decr':
                        sig = reg.name + '_read'
                        f.write('  assign {:<{width}} = read & {}_decode;\n' .format(sig, reg.name, width=width))
                    elif reg.rprop == 'decw':
                        sig = reg.name + '_write'
                        f.write('  assign {:<{width}} = write & {}_decode;\n' .format(sig, reg.name, width=width))
                    elif reg.rprop == 'decrw':
                        sig = reg.name + '_read'
                        f.write('  assign {:<{width}} = read & {}_decode;\n' .format(sig, reg.name, width=width))
                        sig = reg.name + '_write'
                        f.write('  assign {:<{width}} = write & {}_decode;\n' .format(sig, reg.name, width=width))

                    for field in reg.fields:
                        if field.rw == 'rw0c':
                            sig = reg.name + '_' + field.name + '_w0c'
                            f.write('  assign {:<{width}} = write & {}_decode & ~hwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'rw1c':
                            sig = reg.name + '_' + field.name + '_w1c'
                            f.write('  assign {:<{width}} = write & {}_decode & hwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'rwc':
                            sig = reg.name + '_' + field.name + '_wc'
                            f.write('  assign {:<{width}} = write & {}_decode;\n'
                                    .format(sig, reg.name, width=width))
                        elif field.rw == 'rw1s':
                            sig = reg.name + '_' + field.name + '_w1s'
                            f.write('  assign {:<{width}} = write & {}_decode & hwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'rc':
                            sig = reg.name + '_' + field.name + '_rc'
                            f.write('  assign {:<{width}} = read & {}_decode;\n'
                                    .format(sig, reg.name, width=width))
                        elif field.rw == 'rs':
                            sig = reg.name + '_' + field.name + '_rs'
                            f.write('  assign {:<{width}} = read & {}_decode;\n'
                                    .format(sig, reg.name, width=width))
                        elif field.rw == 'w1t':
                            sig = reg.name + '_' + field.name + '_w1t'
                            f.write('  assign {:<{width}} = write & {}_decode & hwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'w1c':
                            sig = reg.name + '_' + field.name + '_w1c'
                            f.write('  assign {:<{width}} = write & {}_decode & hwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'w1s':
                            sig = reg.name + '_' + field.name + '_w1s'
                            f.write('  assign {:<{width}} = write & {}_decode & hwdata[{}];\n'
                                    .format(sig, reg.name, field.start, width=width))
                        elif field.rw == 'w':
                            sig = reg.name + '_' + field.name + '_w'
                            f.write('  assign {:<{width}} = write & {}_decode;\n'
                                    .format(sig, reg.name, width=width))
                            sig = reg.name + '_' + field.name + '_wdata'
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('  assign {:<{width}} = hwdata[{}];\n'.format(sig, index, width=width))
                        elif field.rw == 'rwo':
                            sig = reg.name + '_' + field.name + '_wo'
                            f.write('  assign {:<{width}} = write & {}_decode;\n'
                                    .format(sig, reg.name, width=width))
                            sig = reg.name + '_' + field.name + '_wdata'
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('  assign {:<{width}} = hwdata[{}];\n'.format(sig, index, width=width))
                        elif field.rw == 'rw_ext':
                            sig = reg.name + '_' + field.name + '_w'
                            f.write('  assign {:<{width}} = write & {}_decode;\n'
                                    .format(sig, reg.name, width=width))
                            sig = reg.name + '_' + field.name + '_wdata'
                            if field.bw == 1:
                                index = field.start
                            else:
                                index = '%d:%d' % (field.start, field.stop)
                            f.write('  assign {:<{width}} = hwdata[{}];\n'.format(sig, index, width=width))
                    f.write('\n')
                # endregion
            # endregion

            # region: -------------------- Address Decoder --------------------
            f.write('  // ================ Address Decoder =================\n')
            f.write('  always @(*) begin\n')
            f.write("    hrdata = 32'h0;\n")
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write("    {}_decode = 1'b0;\n".format(reg.name))
            f.write('\n')
            f.write('    case (haddr_msb)\n')
            for reg in self.regs:
                if reg.offset == 'null':
                    continue
                f.write('      // {} @ 0x{}\n'.format(reg.name.upper(), reg.offset))
                f.write("      {width}'b{:0{width}b}: begin\n".format(int(reg.offset, 16)/4, width=addr_bw-2))
                f.write("        {}_decode = 1'b1;\n".format(reg.name))
                f.write('        hrdata = {}_rdata;\n'.format(reg.name))
                f.write('      end\n')
            f.write('      default:;\n')
            f.write('    endcase\n')
            f.write('  end\n\n')
            f.write('endmodule\n')
            # endregion

        f.close()
        print 'Congratulations! RTL generated successfully'

    # ==========================================================
    # Name: generate_cheader(self)
    # Func: generate C header structure for software use
    # ==========================================================
    def generate_cheader(self):
        try:
            f = open(self.name + '.h', 'w')
        except IOError:
            print 'Error: cannot write to %s.h' % self.name
            sys.exit(1)

        # ---------- write structure ----------
        f.write('#ifndef __%s_H\n' % self.name.upper())
        f.write('#define __%s_H\n' % self.name.upper())
        f.write('\n')
        f.write('typedef struct\n')
        f.write('{\n')
        for reg in self.regs:
            # reserved space
            if reg.offset == 'null':
                if reg.value == 1:
                    f.write('__IO uint32_t %s;\n' % reg.name.upper())
                else:
                    f.write('__IO uint32_t %s[%d];\n' % (reg.name.upper(), reg.value))
            else:
                f.write('__IO uint32_t %s;\n' % reg.name.upper())   # TODO: add register description
        f.write('} %s_TypeDef;\n' % self.name.upper())
        f.write('\n')

        # ---------- write bit definition ----------
        for reg in self.regs:                                       # TODO: add bit description
            if reg.offset == 'null':
                continue
            s = ' Bit definition for %s_%s register ' % (self.name.upper(), reg.name.upper())
            f.write('\n/*{:*^{width}}*/\n'.format(s, width=76))

            for field in reversed(reg.fields):
                if field.name == 'rsvd' or field.cprop == 'internal':
                    continue
                s = '%s_%s_%s' % (self.name.upper(), reg.name.upper(), field.name.upper())
                msk = hex(pow(2, field.bw) - 1).upper()
                msk = re.sub('0X', '0x', msk)
                msk = msk.replace('L', '')
                f.write('#define {:<{width}}  ({}U)\n'.format(s+'_Pos', field.stop, width=30))
                f.write('#define {:<{width}}  ({}UL << {})\n'.format(s+'_Msk', msk, s+'_Pos', width=30))
                f.write('#define {:<{width}}  {}\n'.format(s, s+'_Msk', width=30))

                if field.cprop == 'bitmask':
                    msk_width = len(msk) - 2
                    for i in range(field.bw):
                        bit_msk = hex(pow(2, i))
                        bit_msk = re.sub('0x', '', bit_msk)
                        bit_msk = '0x{:0>{msk_width}}'.format(bit_msk, msk_width=msk_width)
                        f.write('#define {:<{width}}  ({}UL << {})\n'.format(s+'_'+str(i), bit_msk, s+'_Pos', width=30))

        f.write('\n#endif\n')

        f.close()
        print 'Congratulations! C header generated successfully'

    # ==========================================================
    # Name: generate_peripherals(self, f)
    # Para: file writer f
    # Para: p peripheral dict, {'base': '0x41040000', 'group_name': 'BLE_RF', 'name': 'BLE_RF'}
    # Func: generate peripheral info
    # ==========================================================
    def generate_peripherals(self, f, p):
        make_line(f, '<peripheral>')
        inc_indentation()
        make_line(f, '<name>%s</name>' % p['name'].upper())
        make_line(f, '<description></description>')
        make_line(f, '<groupName>%s</groupName>' % self.name.upper())
        make_line(f, '<baseAddress>%s</baseAddress>' % p['base'])
        make_line(f, '<addressBlock>')
        inc_indentation()                                                           
        make_line(f, '<offset>0x0</offset>')
        make_line(f, '<size>0x1000</size>')
        make_line(f, '<usage>registers</usage>')
        dec_indentation()
        make_line(f, '</addressBlock>')
        make_line(f, '<registers>')
        inc_indentation()

        self.generate_registers(f)

        dec_indentation()
        make_line(f, '</registers>')
        dec_indentation()
        make_line(f, '</peripheral>')

        
    # ==========================================================
    # Name: generate_registers(self, f)
    # Para: file writer f
    # Func: helper for generate_peripherals, generate registers' info
    # ==========================================================
    def generate_registers(self, f):
        for reg in self.regs:
            make_line(f, '<register>')
            inc_indentation()
            make_line(f, '<name>%s</name>' % reg.name.upper())
            make_line(f, '<displayName>%s</displayName>' % reg.name.upper())
            make_line(f, '<description>%s</description>' % reg.description.replace('<', '&lt;'))
            make_line(f, '<addressOffset>0x%s</addressOffset>' % reg.offset)
            make_line(f, '<size>0x20</size>')                         #register size 32 bits
            make_line(f, '<access>read-write</access>')                                                
            make_line(f, '<resetValue>0x%s</resetValue>' % reg.value.replace('_', ''))                                               
            
            make_line(f, '<fields>')
            inc_indentation()

            self.generate_field(f, reg.fields)

            dec_indentation()
            make_line(f, '</fields>')
            dec_indentation()
            make_line(f, '</register>')
    
    # ==========================================================
    # Name: generate_field(self, f, fields):
    # Para: file writer f
    # Func: helper for generate_registers, generate fields' info
    # ==========================================================
    def generate_field(self, f, fields):
        rsvd = 0;
        for field in fields:
            make_line(f, '<field>')
            inc_indentation()

            name = field.name.upper()
            if name == 'RSVD':
                rsvd += 1
                if rsvd > 1:
                    name = name + str(rsvd)
            make_line(f, '<name>%s</name>' % name)

            description = field.description.split('\n')
            make_line(f, '<description>')
            for i in  range(0, len(description)):
                if description[i] != '':
                    make_line(f, description[i].replace('<', '&lt;'))
            make_line(f, '</description>')

            make_line(f, '<bitOffset>%d</bitOffset>' % field.stop)
        
            m = re.match(r'(\d+)\'[hbd]([0-9a-fA-F]+)', field.value)
            if m is None:
                print(field.value)
            make_line(f, '<bitWidth>%d</bitWidth>' % int(m.group(1)))

            dec_indentation()
            make_line(f, '</field>')


# ==========================================================
# indentation helper for generating svd files
# ==========================================================

def init_indentation():
    global level
    level = 0

def inc_indentation():
    global level
    level += 1

def dec_indentation():
    global level
    level -= 1
    if level < 0:
        level = 0

def get_indentation_space():
    return ' ' * level * 2

def make_line(f,s):
    f.write(get_indentation_space() + s.encode('utf-8') + '\n')


# ==========================================================
# Name: generate_svd(proms)
# Para: list of prom
# Func: generate svd file for all xlsx files
# ==========================================================
def generate_svd(proms, periph_list, soc_name):
    soc_name = soc_name.upper()
    try:
        f = open('{}.svd'.format(soc_name), 'w') 
    except IOError:
        print 'Error: cannot generate file name.svd'
        sys.exit(1)
    make_line(f,'<?xml version="1.0" encoding="utf-8" standalone="no"?>')
    make_line(f,'<device schemaVersion="1.1"')
    make_line(f,'xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"')
    make_line(f,'xs:noNamespaceSchemaLocation="CMSIS-SVD_Schema_1_1.xsd">')

    #Chip info
    inc_indentation()
    make_line(f,'<name>{}</name>'.format(soc_name))
    make_line(f,'<version>1.0</version>')
    make_line(f,'<description>{}</description>'.format(soc_name))

    #CPU detials
    make_line(f,'<!-- details about the cpu embedded in the device -->')
    make_line(f,'<cpu>')
    inc_indentation()
    make_line(f,'<name>CM33</name>')
    make_line(f,'<revision>r0p1</revision>')
    make_line(f,'<endian>little</endian>')
    make_line(f,'<mpuPresent>true</mpuPresent>')
    make_line(f,'<fpuPresent>true</fpuPresent>')
    make_line(f,'<nvicPrioBits>3</nvicPrioBits>')
    make_line(f,'<vendorSystickConfig>false</vendorSystickConfig>')
    dec_indentation()
    make_line(f,'</cpu>')

    #Bus properties
    make_line(f,'<!--Bus Interface Properties-->')
    make_line(f,'<addressUnitBits>8</addressUnitBits>')

    #Max bit width
    make_line(f,'<!--the maximum data bit width accessible within a single transfer-->')
    make_line(f,'<width>32</width>')

    #Register default properties
    make_line(f,'<!--Register Default Properties-->')
    make_line(f,'<size>0x20</size>')
    make_line(f,'<resetValue>0x0</resetValue>')
    make_line(f,'<resetMask>0xFFFFFFFF</resetMask>')

    make_line(f,'<peripherals>')
    inc_indentation()

    # generate register info for all xlsx spreadsheets
    for p in periph_list:
        group_name = p['group_name']
        if group_name not in proms:
            group_name = group_name.replace('BLE_', '')
            if group_name not in proms:
                group_name = None

        if group_name is not None:
            prom = proms[group_name]
            prom.generate_peripherals(f, p)
        else:    
            print('{} not found'.format(p['group_name']))

    dec_indentation()
    make_line(f, '</peripherals>')
    dec_indentation()
    make_line(f, '</device>')

    f.close()
    
# ==========================================================
# Name: parse_args()
# Para: None
# Func: parse arguments
# ==========================================================
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('regmap', help='regmap spreadsheet folder')
    parser.add_argument('-t', '--type', choices=['apb', 'ahb', 'svd', 'header'], default='apb')
    return parser.parse_args()


# ==========================================================
# main function starts here
# ==========================================================
if __name__ == '__main__':
    args = parse_args()
    proms = {}
    for root, dirs, files in os.walk(args.regmap):
        for name in files:
            #if 'spi' not in name:
            #    continue
            if name.endswith('.xlsx'):
                prom = Prometheus()
                path = os.path.join(root, name)#args.regmap + '/' + name
                prom.parse_sheet(path) 
                if len(prom.regs) > 0:
                    proms[prom.name.upper()] = prom
                #proms.append(prom)

    periph_list = peripherals.get_peripheral_list(args.regmap)

    if args.type == 'apb':
        for k, v in proms.items():
            v.generate_apb()
    elif args.type == 'ahb':
        for prom in proms:
            prom.generate_ahb()
    elif args.type == 'svd':
        init_indentation()
        generate_svd(proms, periph_list, args.regmap)
    elif args.type == 'header':
        for k, v in proms.items():
            v.generate_cheader()

    #if args.type != 'svd':    
    #    prom.generate_cheader()
    
