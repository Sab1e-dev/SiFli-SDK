/*
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * \file     startup_gd32vf103.S
 * \brief    NMSIS Nuclei N/NX Class Core based Core Device Startup File for
 *  Device gd32vf103
 * \version  V1.00
 * \date     21 Nov 2019
 *
 *
 ******************************************************************************/

#include "riscv_encoding.h"

/*Avoid name conflict with C lib*/
#define _start __start 

.macro DECLARE_INT_HANDLER  INT_HDL_NAME
#if defined(__riscv_xlen) && (__riscv_xlen == 32)
    .word \INT_HDL_NAME
#else
    .dword \INT_HDL_NAME
#endif
.endm

    .section .vtable

    .weak  AON_IRQHandler        
    .weak  ble_isr               
    .weak  DMAC2_CH1_IRQHandler  
    .weak  DMAC2_CH2_IRQHandler  
    .weak  DMAC2_CH3_IRQHandler  
    .weak  DMAC2_CH4_IRQHandler  
    .weak  DMAC2_CH5_IRQHandler  
    .weak  DMAC2_CH6_IRQHandler  
    .weak  DMAC2_CH7_IRQHandler  
    .weak  DMAC2_CH8_IRQHandler  
    .weak  PATCH_IRQHandler      
    .weak  dm_isr                
    .weak  USART4_IRQHandler     
    .weak  USART5_IRQHandler     
    .weak  SECU2_IRQHandler      
    .weak  bt_isr                
    .weak  BTIM3_IRQHandler      
    .weak  BTIM4_IRQHandler      
    .weak  PTC2_IRQHandler       
    .weak  LPTIM3_IRQHandler     
    .weak  GPIO2_IRQHandler      
    .weak  HPSYS0_IRQHandler     
    .weak  HPSYS1_IRQHandler     
    .weak  HCPU2LCPU_IRQHandler  
    .weak  Interrupt24_IRQHandler
    .weak  Interrupt25_IRQHandler
    .weak  Interrupt26_IRQHandler
    .weak  Interrupt27_IRQHandler
    .weak  Interrupt28_IRQHandler
    .weak  Interrupt29_IRQHandler
    .weak  Interrupt30_IRQHandler
    .weak  Interrupt31_IRQHandler

    .globl vector_base
    .type vector_base, @object
vector_base:
    j _start                                                /* 0: Reserved, Jump to _start when reset for vector table not remapped cases.*/
    .align LOG_REGBYTES                                     /*    Need to align 4 byte for RV32, 8 Byte for RV64 */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 1: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 2: Reserved */
    DECLARE_INT_HANDLER     eclic_msip_handler              /* 3: Machine software interrupt */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 4: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 5: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 6: Reserved */
    DECLARE_INT_HANDLER     SysTick_Handler                 /* 7: Machine timer interrupt */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 8: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 9: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 10: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 11: Reserved */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 12: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 13: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 14: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 15: Reserved */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 16: Reserved */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 17: Bus Error interrupt */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 18: Performance Monitor */

    DECLARE_INT_HANDLER    AON_IRQHandler        
    DECLARE_INT_HANDLER    ble_isr               
    DECLARE_INT_HANDLER    DMAC2_CH1_IRQHandler  
    DECLARE_INT_HANDLER    DMAC2_CH2_IRQHandler  
    DECLARE_INT_HANDLER    DMAC2_CH3_IRQHandler  
    DECLARE_INT_HANDLER    DMAC2_CH4_IRQHandler  
    DECLARE_INT_HANDLER    DMAC2_CH5_IRQHandler  
    DECLARE_INT_HANDLER    DMAC2_CH6_IRQHandler  
    DECLARE_INT_HANDLER    DMAC2_CH7_IRQHandler  
    DECLARE_INT_HANDLER    DMAC2_CH8_IRQHandler  
    DECLARE_INT_HANDLER    PATCH_IRQHandler      
    DECLARE_INT_HANDLER    dm_isr                
    DECLARE_INT_HANDLER    USART4_IRQHandler     
    DECLARE_INT_HANDLER    USART5_IRQHandler     
    DECLARE_INT_HANDLER    SECU2_IRQHandler      
    DECLARE_INT_HANDLER    bt_isr                
    DECLARE_INT_HANDLER    BTIM3_IRQHandler      
    DECLARE_INT_HANDLER    BTIM4_IRQHandler      
    DECLARE_INT_HANDLER    PTC2_IRQHandler       
    DECLARE_INT_HANDLER    LPTIM3_IRQHandler     
    DECLARE_INT_HANDLER    GPIO2_IRQHandler      
    DECLARE_INT_HANDLER    HPSYS0_IRQHandler     
    DECLARE_INT_HANDLER    HPSYS1_IRQHandler     
    DECLARE_INT_HANDLER    HCPU2LCPU_IRQHandler  
    DECLARE_INT_HANDLER    Interrupt24_IRQHandler
    DECLARE_INT_HANDLER    Interrupt25_IRQHandler
    DECLARE_INT_HANDLER    Interrupt26_IRQHandler
    DECLARE_INT_HANDLER    Interrupt27_IRQHandler
    DECLARE_INT_HANDLER    Interrupt28_IRQHandler
    DECLARE_INT_HANDLER    Interrupt29_IRQHandler
    DECLARE_INT_HANDLER    Interrupt30_IRQHandler
    DECLARE_INT_HANDLER    Interrupt31_IRQHandler


    .section .init

    .globl _start
    .type _start, @function

/**
 * Reset Handler called on controller reset
 */
_start:
    /* ===== Startup Stage 1 ===== */
    /* Disable Global Interrupt */
    csrc CSR_MSTATUS, MSTATUS_MIE
    /* Jump to logical address first to ensure correct operation of RAM region  */
//    j .
    la      a0, _start
    li      a1, 0
    slli    a1, a1, 29
    bleu    a1, a0, _start0800
    srli    a1, a1, 2
    bleu    a1, a0, _start0800
    la      a0, _start0800
    add     a0, a0, a1
    jr      a0

_start0800:
    /* Initialize GP and Stack Pointer SP */
    .option push
    .option norelax
    la gp, __global_pointer$
    la tp, __tls_base
    .option pop
    la sp, _sp

    /*
     * Set the the NMI base mnvec to share
     * with mtvec by setting CSR_MMISC_CTL
     * bit 9 NMI_CAUSE_FFF to 1
     */
    li t0, MMISC_CTL_NMI_CAUSE_FFF
    csrs CSR_MMISC_CTL, t0

    /*
     * Intialize ECLIC vector interrupt
     * base address mtvt to vector_base
     */
    la t0, vector_base
    csrw CSR_MTVT, t0

    /*
     * Set ECLIC non-vector entry to be controlled
     * by mtvt2 CSR register.
     * Intialize ECLIC non-vector interrupt
     * base address mtvt2 to irq_entry.
     */
    la t0, irq_entry
    csrw CSR_MTVT2, t0
    csrs CSR_MTVT2, 0x1

    /*
     * Set Exception Entry MTVEC to early_exc_entry
     * Due to settings above, Exception and NMI
     * will share common entry.
     * This early_exc_entry is only used during early
     * boot stage before main
     */
    la t0, early_exc_entry
    csrw CSR_MTVEC, t0

    /* Set the interrupt processing mode to ECLIC mode */
    li t0, 0x3f
    csrc CSR_MTVEC, t0
    csrs CSR_MTVEC, 0x3

    /* ===== Startup Stage 2 ===== */

#if defined(__riscv_flen) && __riscv_flen > 0
    /* Enable FPU, and set state to initial */
    li t0, MSTATUS_FS
    csrc mstatus, t0
    li t0, MSTATUS_FS_INITIAL
    csrs mstatus, t0
#endif

    /* Enable mcycle and minstret counter */
    csrci CSR_MCOUNTINHIBIT, 0x5

    /* ===== Startup Stage 3 ===== */
    /*
     * Load text section from CODE ROM to CODE RAM
     * when text LMA is different with VMA
     */
    la a0, _text_lma
    la a1, _text
    /* If text LMA and VMA are equal
     * then no need to copy text section */
    beq a0, a1, 2f
    la a2, _etext
    bgeu a1, a2, 2f

1:
    /* Load code section if necessary */
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Load data section */
    la a0, _data_lma
    la a1, _data
    la a2, _edata
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Clear bss section */
    la a0, __bss_start
    la a1, _end
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:

    /*
     * Call vendor defined SystemInit to
     * initialize the micro-controller system
     */
    call SystemInit

    /* Call global constructors */
    la a0, __libc_fini_array
    call atexit
    /* Call C/C++ constructor start up code */
    call __libc_init_array

    /* do pre-init steps before main */
    call _premain_init

    /*
     * When all initialization steps done
     * set exception entry to correct exception
     * entry and jump to main.
     * And set the interrupt processing mode to
     * ECLIC mode
     */
    la t0, exc_entry
    csrw CSR_MTVEC, t0
    li t0, 0x3f
    csrc CSR_MTVEC, t0
    csrs CSR_MTVEC, 0x3

    /* ===== Call Main Function  ===== */
    /* argc = argv = 0 */
    li a0, 0
    li a1, 0

/* #ifdef RTOS_RTTHREAD */
#if 1
    // Call pre_main function for patch install
    call pre_main
#else
    call main
#endif
    /* do post-main steps after main */
    call _postmain_fini

1:
    j 1b

/* Early boot exception entry before main */
.align 6
.global early_exc_entry
early_exc_entry:
    wfi
    j early_exc_entry
