__author__ = 'yankai'

import re
import sys


#########################################################
class Field:
    def __init__(self, name, start, stop, rw, value):
        self.name = name
        self.start = start
        self.stop = stop
        self.bw = start - stop + 1
        self.value = value
        self.rw = rw
        self.rprop = ''
        self.cprop = ''
        self.seq = 0
        self.comb = 0
        self.description = ''

    def set_description(self, description):
        self.description = description

    def display(self):
        print '[%d:%d] %s %s %s' % (self.start, self.stop, self.name.upper(), self.rw, self.value)

    def set_rprop(self, rprop):
        self.rprop = rprop

    def set_cprop(self, cprop):
        self.cprop = cprop

    def validate(self):
        # ====================== check field name ======================
        self.name = self.name.lower()
        m = re.match(r'^\w+$', self.name)
        if not m:
            print 'Error! Invalid field name:', self.name
            return 0

        # ============ check r/w attributes ============
        self.rw = self.rw.lower()
        if self.name == 'rsvd':
            if self.rw != '':       # rsvd field has no rw attribute
                print 'Warning! %s attribute ignored for RSVD field' % self.rw
            self.rw = 'rsvd'        # always set to rsvd attribute for easy handling
        elif self.rw in ['rw']:
            if self.rprop == 'extreg':
                self.rw = 'rw_ext'  # alias to rw_ext for following processing
                self.comb = 1       # ext rw needs combinational logic
            else:
                self.seq = 1        # int rw needs sequential DFF
        elif self.rw in ['w','rw0c','rw1c','rwc','rw1s','rc','rs','rwo','w1t','w1c','w1s','rc_w0','rt_w1']:
            self.comb = 1           # fields that need combinational logic
        elif self.rw in ['r']:
            pass
        else:
            print 'Error! Invalid read/write attribute:', self.rw
            return 0

        # ======================= check bit range ======================
        if self.start > 31 or self.stop < 0 or self.start < self.stop:
            print 'Error! Field bit range error: [%d:%d]' % (self.start, self.stop)
            return 0

        if self.rw in ['rw0c','rw1c','rwc','rw1s','rc','rs','w1t','w1c','w1s']:  # only support 1 bit
            if self.bw > 1:
                print 'Error! %s fields only support single bit' % self.rw
                return 0

        #  ============ check default value ===========
        m = re.match(r"^(\d+)'([bdh])(\w+)$", self.value)
        if not m:
            print 'Error! Invalid default value:', self.value
            return 0

        bw = int(m.group(1))
        radix = m.group(2)
        value = m.group(3)

        # check bit width
        if self.bw != int(bw):
            print 'Error! Bit width mismatch:', self.value
            return 0

        # binary value
        if radix == 'b':
            if value == '0':                        # ignore case like 2'b0
                pass
            elif len(value) != self.bw:             # 3'b101 len(101) must equal to 3
                print 'Error! Bit width mismatch:', self.value
                return 0
            elif re.search(r'[^01]', value):        # contains only 0 and 1
                print 'Error! Invalid binary value:', self.value
                return 0

        # hex value
        elif radix == 'h':
            if int(value, 16) >= 2 ** bw:
                print 'Error! Hex value out of range:', self.value
                return 0

        # decimal value
        else:
            if int(value) >= 2 ** bw:
                print 'Error! Decimal value out of range:', self.value
                return 0

        #  ============ check property ===========
        if self.rprop in ['','extreg']:
            pass
        else:
            print 'Error! Invalid %s RTL property' % self.rprop
            return 0

        if self.cprop in ['','internal','bitmask']:
            pass
        else:
            print 'Error! Invalid %s C property' % self.cprop
            return 0

        return 1


#########################################################
class Register32:
    def __init__(self, name, offset, value):
        self.name = name
        self.offset = offset
        self.value = value
        self.fields = []
        self.seq = 0            # 1 means there is internal sequential logic
        self.comb = 0           # 1 means there is internal combinational logic
        self.max_seq = 0        # max length of sequential field names
        self.max_comb = 0       # max length of combinational field names
        self.rprop = ''
        self.cprop = ''
        self.description = ''

    def set_description(self, description):
        self.description = description

    def add_field(self, field):
        if field.validate():
            self.fields.append(field)

            if field.seq == 1:
                self.seq = 1
                if len(field.name) > self.max_seq:
                    self.max_seq = len(field.name)
            elif field.comb == 1:
                self.comb = 1
                if len(field.name) > self.max_comb:
                    self.max_comb = len(field.name)
        else:
            print "Error! Invalid field %s in %s" % (field.name, self.name)
            sys.exit()

    def display(self):
        print 'Register: %s @ 0x%s' % (self.name.upper(), self.offset)
        for i in range(len(self.fields)):
            self.fields[i].display()

    def set_rprop(self, rprop):
        self.rprop = rprop

    def set_cprop(self, cprop):
        self.cprop = cprop

    def validate(self):
        # check name
        self.name = self.name.lower()
        m = re.match(r'^\w+$', self.name)
        if not m:
            print 'Invalid register name:', self.name
            return 0

        # check offset
        self.offset = self.offset.lower()
        m = re.match(r'^[0123456789abcdef]+$', self.offset)
        if not m:
            print 'Error! Invalid offset:', self.offset
            return 0
        if int(self.offset, 16) % 4 != 0:
            print 'Error! Register offset must be multiples of 4'
            return 0

        # check if empty fields
        if len(self.fields) == 0:
            print 'Error! %s has no fields' % self.name
            return 0

        # check register value and field value consistency
        # TODO

        # check register RTL/C property
        if self.rprop in ['']:
            pass
        elif self.rprop in ['decr', 'decw', 'decrw']:
            self.comb = 1   # need combinational logic
        else:
            print 'Error! Invalid %s RTL property' % self.rprop
            return 0

        if self.cprop in ['','internal']:
            pass
        else:
            print 'Error! Invalid %s C property' % self.cprop
            return 0

        # sort based on start bit position
        self.fields.sort(key=lambda x: x.start, reverse=True)

        # check overlap between neighboring fields and fill gap
        for i in range(len(self.fields)-1, 0, -1):
            gap = self.fields[i-1].stop - self.fields[i].start - 1
            if gap < 0:
                print ('Error! Field position overlap in %s: between %s(%d) and %s(%d)' % (self.name, self.fields[i].name, self.fields[i].start,self.fields[i-1].name, self.fields[i-1].stop))
                return 0
            elif gap > 0:
                start = self.fields[i-1].stop - 1
                stop = self.fields[i].start + 1
                value = '%d\'h0' % (start - stop + 1)
                field = Field('rsvd', start, stop, 'rsvd', value)
                self.fields.insert(i, field)
                print 'Warning! Field position gap in %s' % self.name

        # insert the first field if not starting with 31
        if self.fields[0].start < 31:
            start = 31
            stop = self.fields[0].start + 1
            value = '%d\'h0' % (start - stop + 1)
            field = Field('rsvd', start, stop, 'rsvd', value)
            self.fields.insert(0, field)
            print 'Warning! Field position gap in %s' % self.name
        elif self.fields[0].start > 31:
            print 'Error! Field position exceeds in %s' % self.name
            return 0

        # insert the last field if not ending with 0
        if self.fields[-1].stop > 0:
            start = self.fields[-1].stop - 1
            stop = 0
            value = '%d\'h0' % (start - stop + 1)
            field = Field('rsvd', start, stop, 'rsvd', value)
            self.fields.append(field)
            print 'Warning! Field position gap in %s' % self.name

        return 1


#########################################################
if __name__ == '__main__':
    reg32 = Register32('pwr_ctrl', '0C', '00000000')
    field0 = Field('aaa', 1, 0, 'rw', "2'b0")
    field1 = Field('bbb', 2, 2, 'rw', "1'h0")
    field2 = Field('ccc', 7, 3, 'rw', "5'h0")

    reg32.add_field(field0)
    reg32.add_field(field1)
    reg32.add_field(field2)

    reg32.display()

