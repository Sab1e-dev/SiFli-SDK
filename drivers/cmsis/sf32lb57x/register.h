/**
  ******************************************************************************
  * @file   register.h
  * @author Sifli software development team
  * @brief  SiFli chipset register definition
  *          This file provides register definition for SiFli chipset
  * @{
  ******************************************************************************
*/
/**
 * Copyright (c) 2023 - 2023,  Sifli Technology
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Sifli integrated circuit
 *    in a product or a software update for such product, must reproduce the above
 *    copyright notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Sifli nor the names of its contributors may be used to endorse
 *    or promote products derived from this software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Sifli integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY SIFLI TECHNOLOGY "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SIFLI TECHNOLOGY OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _REGISTER_H_
#define _REGISTER_H_

/*************** individual IP register C header ***************/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef SOC_BF0_HCPU

/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
    /* -------------------  Processor Exceptions Numbers  ----------------------------- */
    NonMaskableInt_IRQn           = -14,     /*  2 Non Maskable Interrupt */
    HardFault_IRQn                = -13,     /*  3 HardFault Interrupt */
    MemoryManagement_IRQn         = -12,     /*  4 Memory Management Interrupt */
    BusFault_IRQn                 = -11,     /*  5 Bus Fault Interrupt */
    UsageFault_IRQn               = -10,     /*  6 Usage Fault Interrupt */
    SecureFault_IRQn              =  -9,     /*  7 Secure Fault Interrupt */
    SVCall_IRQn                   =  -5,     /* 11 SV Call Interrupt */
    DebugMonitor_IRQn             =  -4,     /* 12 Debug Monitor Interrupt */
    PendSV_IRQn                   =  -2,     /* 14 Pend SV Interrupt */
    SysTick_IRQn                  =  -1,     /* 15 System Tick Interrupt */

    /* -------------------  Processor Interrupt Numbers  ------------------------------ */
    AON_IRQn                      =   0,
    BLE_MAC_IRQn                  =   1,
    DMAC2_CH1_IRQn                =   2,
    DMAC2_CH2_IRQn                =   3,
    DMAC2_CH3_IRQn                =   4,
    DMAC2_CH4_IRQn                =   5,
    DMAC2_CH5_IRQn                =   6,
    DMAC2_CH6_IRQn                =   7,
    DMAC2_CH7_IRQn                =   8,
    DMAC2_CH8_IRQn                =   9,
    PATCH_IRQn                    =  10,
    DM_MAC_IRQn                   =  11,
    Interrupt12_IRQn              =  12,
    USART5_IRQn                   =  13,
    SECU2_IRQn                    =  14,
    BT_MAC_IRQn                   =  15,
    BTIM3_IRQn                    =  16,
    BTIM4_IRQn                    =  17,
    PTC2_IRQn                     =  18,
    LPTIM3_IRQn                   =  19,
    GPIO2_IRQn                    =  20,
    LCPU_SYSRESET_IRQn            =  21,
    Interrupt22_IRQn              =  22,
    Interrupt23_IRQn              =  23,
    Interrupt24_IRQn              =  24,
    Interrupt25_IRQn              =  25,
    Interrupt26_IRQn              =  26,
    Interrupt27_IRQn              =  27,
    Interrupt28_IRQn              =  28,
    Interrupt29_IRQn              =  29,
    Interrupt30_IRQn              =  30,
    Interrupt31_IRQn              =  31,
    Interrupt32_IRQn              =  32,
    Interrupt33_IRQn              =  33,
    Interrupt34_IRQn              =  34,
    Interrupt35_IRQn              =  35,
    Interrupt36_IRQn              =  36,
    Interrupt37_IRQn              =  37,
    Interrupt38_IRQn              =  38,
    Interrupt39_IRQn              =  39,
    Interrupt40_IRQn              =  40,
    Interrupt41_IRQn              =  41,
    Interrupt42_IRQn              =  42,
    Interrupt43_IRQn              =  43,
    Interrupt44_IRQn              =  44,
    Interrupt45_IRQn              =  45,
    LPTIM1_IRQn                   =  46,
    LPTIM2_IRQn                   =  47,
    WDT2_IRQn                     =  48,
    RTC_IRQn                      =  49,
    DMAC1_CH1_IRQn                =  50,
    DMAC1_CH2_IRQn                =  51,
    DMAC1_CH3_IRQn                =  52,
    DMAC1_CH4_IRQn                =  53,
    DMAC1_CH5_IRQn                =  54,
    DMAC1_CH6_IRQn                =  55,
    DMAC1_CH7_IRQn                =  56,
    DMAC1_CH8_IRQn                =  57,
    LCPU2HCPU_IRQn                =  58,
    USART1_IRQn                   =  59,
    SPI1_IRQn                     =  60,
    I2C1_IRQn                     =  61,
    EPIC_IRQn                     =  62,
    LCDC1_IRQn                    =  63,
    I2S1_IRQn                     =  64,
    GPADC_IRQn                    =  65,
    EFUSEC_IRQn                   =  66,
    AES_IRQn                      =  67,
    PTC1_IRQn                     =  68,
    TRNG_IRQn                     =  69,
    GPTIM1_IRQn                   =  70,
    GPTIM2_IRQn                   =  71,
    BTIM1_IRQn                    =  72,
    BTIM2_IRQn                    =  73,
    USART2_IRQn                   =  74,
    SPI2_IRQn                     =  75,
    I2C2_IRQn                     =  76,
    V2D_GPU_IRQn                  =  77,
    I2C4_IRQn                     =  78,
    SDMMC1_IRQn                   =  79,
    ACPU2HCPU_IRQn                =  80,
    SDMMC2_IRQn                   =  81,
    PDM1_IRQn                     =  82,
    CAN1_IRQn                     =  83,
    GPIO1_IRQn                    =  84,
    MPI1_IRQn                     =  85,
    MPI2_IRQn                     =  86,
    MPI3_IRQn                     =  87,
    NPU_IRQ                       =  88,
    EZIP_IRQn                     =  89,
    AUDPRC_IRQn                   =  90,
    TSEN_IRQn                     =  91,
    USBC_IRQn                     =  92,
    I2C3_IRQn                     =  93,
    ATIM1_IRQn                    =  94,
    USART3_IRQn                   =  95,
    AUD_HP_IRQn                   =  96,
    DCMI_IRQn                     =  97,
    SECU1_IRQn                    =  98,
    JPEGD_IRQn                    =  99,
    HASH_IRQn                     =  100,
    USART4_IRQn                   =  101,
    DMAC1_CH9_IRQn                =  102,
    DMAC1_CH10_IRQn               =  103,
    HCPU2LCPU_IRQn                =  -1,
    /* Interrupts 100 .. 479 are left out */
} IRQn_Type;

#define GPDMA1_CH1_IRQn    DMAC1_CH1_IRQn
#define GPDMA1_CH2_IRQn    DMAC1_CH2_IRQn
#define GPDMA1_CH3_IRQn    DMAC1_CH3_IRQn
#define GPDMA1_CH4_IRQn    DMAC1_CH4_IRQn
#define GPDMA1_CH5_IRQn    DMAC1_CH5_IRQn
#define GPDMA1_CH6_IRQn    DMAC1_CH6_IRQn
#define GPDMA1_CH7_IRQn    DMAC1_CH7_IRQn
#define GPDMA1_CH8_IRQn    DMAC1_CH8_IRQn
#define GPDMA1_CH9_IRQn     DMAC1_CH9_IRQn
#define GPDMA1_CH10_IRQn    DMAC1_CH10_IRQn
#define GPDMA1_CH1_Handler DMAC1_CH1_Handler
#define GPDMA1_CH2_Handler DMAC1_CH2_Handler
#define GPDMA1_CH3_Handler DMAC1_CH3_Handler
#define GPDMA1_CH4_Handler DMAC1_CH4_Handler
#define GPDMA1_CH5_Handler DMAC1_CH5_Handler
#define GPDMA1_CH6_Handler DMAC1_CH6_Handler
#define GPDMA1_CH7_Handler DMAC1_CH7_Handler
#define GPDMA1_CH8_Handler DMAC1_CH8_Handler
#define GPDMA1_CH9_Handler  DMAC1_CH9_Handler
#define GPDMA1_CH10_Handler DMAC1_CH10_Handler

/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if   defined (__CC_ARM)
#pragma push
#pragma anon_unions
#elif defined (__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning 586
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif


/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __CM33_REV                0x0000U   /* Core revision r0p1 */
#define __SAUREGION_PRESENT       1U        /* SAU regions present */
#define __MPU_PRESENT             1U        /* MPU present */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U        /* no FPU present */
#define __DSP_PRESENT             1U        /* no DSP extension present */
#include "core_cm33.h"                      /* Processor and core peripherals */
#include "system_bf0_ap.h"                 /* System Header */

//#include "system_ARMCM33.h"               /* System Header */

#define __ICACHE_PRESENT          1U
#define __DCACHE_PRESENT          1U

#if defined(SOC_BF0_HCPU)
#define MPU_REGION_NUM       12
#endif /* SOC_BF0_HCPU */

#include "core_mstar.h"                     /* cache related functions */



#elif defined(SOC_BF0_LCPU)



/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn
{
    /* =======================================  Nuclei Core Specific Interrupt Numbers  ======================================== */

    Reserved0_IRQn            =   0,
    Reserved1_IRQn            =   1,
    Reserved2_IRQn            =   2,
    Msip_IRQn                 =   3,
    Reserved3_IRQn            =   4,
    Reserved4_IRQn            =   5,
    Reserved5_IRQn            =   6,
    SysTimer_IRQn             =   7,
    Reserved6_IRQn            =   8,
    Reserved7_IRQn            =   9,
    Reserved8_IRQn            =  10,
    Reserved9_IRQn            =  11,
    Reserved10_IRQn           =  12,
    Reserved11_IRQn           =  13,
    Reserved12_IRQn           =  14,
    Reserved13_IRQn           =  15,
    Reserved14_IRQn           =  16,
    Bwei_IRQn                 =  17,
    Pmovi_IRQn                =  18,

    /* ===========================================  N300 Specific Interrupt Numbers  ========================================= */
    /* interruput numbers */
    AON_IRQn                  =  19,
    BLE_MAC_IRQn              =  20,
    DMAC2_CH1_IRQn            =  21,
    DMAC2_CH2_IRQn            =  22,
    DMAC2_CH3_IRQn            =  23,
    DMAC2_CH4_IRQn            =  24,
    DMAC2_CH5_IRQn            =  25,
    DMAC2_CH6_IRQn            =  26,
    DMAC2_CH7_IRQn            =  27,
    DMAC2_CH8_IRQn            =  28,
    PATCH_IRQn                =  29,
    DM_MAC_IRQn               =  30,
    USART4_IRQn               =  31,
    USART5_IRQn               =  32,
    SECU2_IRQn                =  33,
    BT_MAC_IRQn               =  34,
    BTIM3_IRQn                =  35,
    BTIM4_IRQn                =  36,
    PTC2_IRQn                 =  37,
    LPTIM3_IRQn               =  38,
    GPIO2_IRQn                =  39,
    HPSYS0_IRQn               =  40,
    HPSYS1_IRQn               =  41,
    HCPU2LCPU_IRQn            =  42,
    ACPU2LCPU_IRQn            =  43,
    Interrupt25_IRQn          =  44,
    Interrupt26_IRQn          =  45,
    Interrupt27_IRQn          =  46,
    Interrupt28_IRQn          =  47,
    Interrupt29_IRQn          =  48,
    Interrupt30_IRQn          =  49,
    Interrupt31_IRQn          =  50,
    SOC_INT_MAX,

    // Not work on LCPU
    LCPU2HCPU_IRQn = -1,
} IRQn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ToDo: define the correct core features for the nuclei_soc */
#define __ECLIC_PRESENT           1                     /*!< Set to 1 if ECLIC is present */
//#define __ECLIC_BASEADDR          0x30020000UL          /*!< Set to ECLIC baseaddr of your device */

#define __ECLIC_INTCTLBITS        3                     /*!< Set to 1 - 8, the number of hardware bits are actually implemented in the clicintctl registers. */
#define __ECLIC_INTNUM            32                    /*!< Set to 1 - 1005, the external interrupt number of ECLIC Unit */
#define __SYSTIMER_PRESENT        1                     /*!< Set to 1 if System Timer is present */
//#define __SYSTIMER_BASEADDR       0x30030000UL          /*!< Set to SysTimer baseaddr of your device */

/*!< Set to 0, 1, or 2, 0 not present, 1 single floating point unit present, 2 double floating point unit present */
#define __FPU_PRESENT             1

#define __DSP_PRESENT             1                     /*!< Set to 1 if DSP is present */
#define __PMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __PMP_ENTRY_NUM           8                     /*!< Set to 8 or 16, the number of PMP entries */
#define __ICACHE_PRESENT          0                     /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT          0                     /*!< Set to 1 if D-Cache is present */
#define __INC_INTRINSIC_API       0                     /*!< Set to 1 if intrinsic api header files need to be included */
#define __Vendor_SysTickConfig    0                     /*!< Set to 1 if different SysTick Config is used */
#define __Vendor_EXCEPTION        0                     /*!< Set to 1 if vendor exception hander is present */

/** @} */ /* End of group Configuration_of_NMSIS */

#define NVIC_ClearPendingIRQ  ECLIC_ClearPendingIRQ
#define NVIC_EnableIRQ        ECLIC_EnableIRQ
#define NVIC_DisableIRQ       ECLIC_DisableIRQ


#include "nmsis_config.h"                       /*!< Nuclei N/NX class processor and core peripherals */
#include "system_bf0_ap.h"                 /* System Header */

/* ToDo: include your system_nuclei_soc.h file
         replace 'Device' with your device name */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__GNUC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

/* =========================================  End of section using anonymous unions  ========================================= */
#if defined (__GNUC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */

/* Macros for memory access operations */
//#define _REG8P(p, i)                        ((volatile uint8_t *) ((uintptr_t)((p) + (i))))
//#define _REG16P(p, i)                       ((volatile uint16_t *) ((uintptr_t)((p) + (i))))
//#define _REG32P(p, i)                       ((volatile uint32_t *) ((uintptr_t)((p) + (i))))
//#define _REG64P(p, i)                       ((volatile uint64_t *) ((uintptr_t)((p) + (i))))
//#define _REG8(p, i)                         (*(_REG8P(p, i)))
//#define _REG16(p, i)                        (*(_REG16P(p, i)))
//#define _REG32(p, i)                        (*(_REG32P(p, i)))
//#define _REG64(p, i)                        (*(_REG64P(p, i)))
//#define REG8(addr)                          _REG8((addr), 0)
//#define REG16(addr)                         _REG16((addr), 0)
//#define REG32(addr)                         _REG32((addr), 0)
//#define REG64(addr)                         _REG64((addr), 0)
//
///* Macros for address type convert and access operations */
//#define ADDR16(addr)                        ((uint16_t)(uintptr_t)(addr))
//#define ADDR32(addr)                        ((uint32_t)(uintptr_t)(addr))
//#define ADDR64(addr)                        ((uint64_t)(uintptr_t)(addr))
//#define ADDR8P(addr)                        ((uint8_t *)(uintptr_t)(addr))
//#define ADDR16P(addr)                       ((uint16_t *)(uintptr_t)(addr))
//#define ADDR32P(addr)                       ((uint32_t *)(uintptr_t)(addr))
//#define ADDR64P(addr)                       ((uint64_t *)(uintptr_t)(addr))
/* Macros for Bit Operations */
#if __riscv_xlen == 32
#define BITMASK_MAX                         0xFFFFFFFFUL
#define BITOFS_MAX                          31
#else
#define BITMASK_MAX                         0xFFFFFFFFFFFFFFFFULL
#define BITOFS_MAX                          63
#endif

// BIT/BITS only support bit mask for __riscv_xlen
// For RISC-V 32 bit, it support mask 32 bit wide
// For RISC-V 64 bit, it support mask 64 bit wide
#define BIT(ofs)                            (0x1UL << (ofs))
#define BITS(start, end)                    ((BITMASK_MAX) << (start) & (BITMASK_MAX) >> (BITOFS_MAX - (end)))
#define GET_BIT(regval, bitofs)             (((regval) >> (bitofs)) & 0x1)
#define CLR_BIT(regval, bitofs)             ((regval) &= (~BIT(bitofs)))
#define FLIP_BIT(regval, bitofs)            ((regval) ^= BIT(bitofs))
#define WRITE_BIT(regval, bitofs, val)      CLR_BIT(regval, bitofs); ((regval) |= ((val) << bitofs) & BIT(bitofs))
#define CHECK_BIT(regval, bitofs)           (!!((regval) & (0x1UL<<(bitofs))))
#define GET_BITS(regval, start, end)        (((regval) & BITS((start), (end))) >> (start))
#define SET_BITS(regval, start, end)        ((regval) |= BITS((start), (end)))
#define CLR_BITS(regval, start, end)        ((regval) &= (~BITS((start), (end))))
#define FLIP_BITS(regval, start, end)       ((regval) ^= BITS((start), (end)))
#define WRITE_BITS(regval, start, end, val) CLR_BITS(regval, start, end); ((regval) |= ((val) << start) & BITS((start), (end)))
#define CHECK_BITS_ALL(regval, start, end)  (!((~(regval)) & BITS((start), (end))))
#define CHECK_BITS_ANY(regval, start, end)  ((regval) & BITS((start), (end)))

#define BITMASK_SET(regval, mask)           ((regval) |= (mask))
#define BITMASK_CLR(regval, mask)           ((regval) &= (~(mask)))
#define BITMASK_FLIP(regval, mask)          ((regval) ^= (mask))
#define BITMASK_CHECK_ALL(regval, mask)     (!((~(regval)) & (mask)))
#define BITMASK_CHECK_ANY(regval, mask)     ((regval) & (mask))

/* ToDo: add here your device peripherals pointer definitions
         following is an example for timer */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */

// Interrupt Numbers
//#define SOC_ECLIC_NUM_INTERRUPTS    86

// Interrupt Handler Definitions
//#define SOC_MTIMER_HANDLER          eclic_mtip_handler
//#define SOC_SOFTINT_HANDLER         eclic_msip_handler

typedef struct EXC_Frame
{
    unsigned long ra;                /* ra: x1, return address for jump */
    unsigned long tp;                /* tp: x4, thread pointer */
    unsigned long t0;                /* t0: x5, temporary register 0 */
    unsigned long t1;                /* t1: x6, temporary register 1 */
    unsigned long t2;                /* t2: x7, temporary register 2 */
    unsigned long a0;                /* a0: x10, return value or function argument 0 */
    unsigned long a1;                /* a1: x11, return value or function argument 1 */
    unsigned long a2;                /* a2: x12, function argument 2 */
    unsigned long a3;                /* a3: x13, function argument 3 */
    unsigned long a4;                /* a4: x14, function argument 4 */
    unsigned long a5;                /* a5: x15, function argument 5 */
    unsigned long cause;             /* cause: machine cause csr register */
    unsigned long epc;               /* epc: machine exception program counter csr register */
    unsigned long msubm;             /* msubm: machine sub-mode csr register, nuclei customized */
#ifndef __riscv_32e
    unsigned long a6;                /* a6: x16, function argument 6 */
    unsigned long a7;                /* a7: x17, function argument 7 */
    unsigned long t3;                /* t3: x28, temporary register 3 */
    unsigned long t4;                /* t4: x29, temporary register 4 */
    unsigned long t5;                /* t5: x30, temporary register 5 */
    unsigned long t6;                /* t6: x31, temporary register 6 */
#endif
} EXC_Frame_Type;


/**
 * \brief Dump Exception Frame
 */
void Exception_DumpFrame(unsigned long sp, uint8_t mode);

/**
 * \brief Register an exception handler for exception code EXCn
 */
extern void Exception_Register_EXC(uint32_t EXCn, unsigned long exc_handler);

/**
 * \brief Get current exception handler for exception code EXCn
 */
extern unsigned long Exception_Get_EXC(uint32_t EXCn);

#endif

/* --------  End of section using anonymous unions and disabling warnings  -------- */
#if   defined (__CC_ARM)
#pragma pop
#elif defined (__ICCARM__)
/* leave anonymous unions enabled */
#elif (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
#pragma clang diagnostic pop
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning restore
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

#include <stdint.h>
#include "mem_map.h"


//================== CORE ===================
#include "cache.h"
#define CACHE_BASE          0xE0080000
#define hwp_cache       ((CACHE_TypeDef         *)    CACHE_BASE)

/*************** individual IP register C header ***************/
#include "bt_mac.h"
#include "bt_phy.h"
#include "bt_rfc.h"
#include "hpsys_rcc.h"
#include "lpsys_rcc.h"
#include "dmac.h"
#include "gpdma.h"
#include "usart.h"
#include "epic.h"
#include "spi.h"
#include "gpt.h"
#include "atim.h"
#include "audprc.h"
#include "btim.h"
#include "mailbox1.h"
#include "mailbox2.h"
#include "rtc.h"
#include "mpi.h"
#include "lptim.h"
#include "i2c.h"
#include "hpsys_cfg.h"
#include "lpsys_cfg.h"
#include "efusec.h"
#include "i2s.h"
#include "crc.h"
#include "lcd_if.h"
#include "sd.h"
#include "aes_acc.h"
#include "gpio1.h"
#include "hpsys_pinmux.h"
#include "hpsys_aon.h"
#include "lpsys_aon.h"
#include "pmuc.h"
#include "gpadc.h"
#include "trng.h"
#include "ptc.h"
#include "ezip.h"
#include "patch.h"
#include "wdt.h"
#include "pdm.h"
#include "usbc_x.h"
#include "jpegd.h"
#include "audcodec.h"
#include "tsen.h"
#include "secu1.h"
#include "secu2.h"
#include "v2d_gpu.h"
#include "dcmi.h"
#include "can.h"

/******************* Base Addresss Definition ******************/
//================== MCU_HPSYS ===================
#define HPSYS_RCC_BASE      0x50000000
#define SECU1_BASE          0x50002000
#define PINMUX1_BASE        0x50003000
#define ATIM1_BASE          0x50004000
#define AUDPRC_BASE         0x50005000
#define LCDC1_BASE          0x50008000
#define I2S1_BASE           0x50009000
#define CAN1_BASE           0x5000a000
#define HPSYS_CFG_BASE      0x5000b000
#define EFUSEC_BASE         0x5000c000
#define AES_BASE            0x5000d000
#define I2S2_BASE           0x5000e000
#define TRNG_BASE           0x5000f000
//------------------------------------
#define GPIO1_BASE          0x50040000
#define MPI1_BASE           0x50041000
#define MPI2_BASE           0x50042000
#define MPI3_BASE           0x50043000
#define SDMMC1_BASE         0x50045000
#define SDMMC2_BASE         0x50046000
#define CRC1_BASE           0x50048000
//------------------------------------
#define PTC1_BASE           0x50080000
#define GPDMA1_BASE         0x50081000
#define MAILBOX1_BASE       0x50082000
#define USART1_BASE         0x50084000
#define USART2_BASE         0x50085000
#define USART3_BASE         0x50086000
#define USART4_BASE         0x50087000
#define AUDCODEC_BASE       0x50088000
#define TSEN_BASE           0x50089000
#define GPTIM1_BASE         0x50090000
#define BTIM1_BASE          0x50092000
#define SPI1_BASE           0x50095000
#define SPI2_BASE           0x50096000
#define PDM1_BASE           0x5009a000
#define I2C1_BASE           0x5009c000
#define I2C2_BASE           0x5009d000
#define I2C3_BASE           0x5009e000
#define I2C4_BASE           0x5009f000
//------------------------------------
//------------------------------------
#define GPTIM2_BASE         0x500b0000
#define BTIM2_BASE          0x500b1000
#define GPADC_BASE          0x500b2000
//------------------------------------
#define HPSYS_AON_BASE      0x500c0000
#define LPTIM1_BASE         0x500c1000
#define LPTIM2_BASE         0x500c2000
#define PMUC_BASE           0x500ca000
#define RTC_BASE            0x500cb000
#define IWDT_BASE           0x500cc000
//TODO PMIC
#define EZIP1_BASE          0x50100000
#define EPIC_BASE           0x50101000
#define JPEGD_BASE          0x50104000
#define USBC_BASE           0x50140000
#define DCMI_BASE           0x50141000
#define V2D_GPU_BASE        0x50142000
#define EPIC_RAM_BASE       0x50148000

//================== MCU_LPSYS ===================
#define LPSYS_RCC_BASE      0x40000000
#define DMAC2_BASE          0x40001000
#define MAILBOX2_BASE       0x40002000
#define PINMUX2_BASE        0x40003000
#define PATCH_BASE          0x40004000
#define USART5_BASE         0x40006000
#define SECU2_BASE          0x40007000
#define BTIM3_BASE          0x40009000
#define BTIM4_BASE          0x4000a000
#define PTC2_BASE           0x4000c000
#define LPSYS_CFG_BASE      0x4000f000
//------------------------------------
#define LPSYS_AON_BASE      0x40040000
#define WDT2_BASE           0x40041000
#define LPTIM3_BASE         0x40042000
//------------------------------------
/** no GPIO2 module, placeholder for convenience*/
#define GPIO2_BASE          0x00000000
#define BT_RFC_MEM_BASE     0x40082000
#define BT_RFC_REG_BASE     0x40082800
#define BT_PHY_BASE         0x40084000
#define CRC2_BASE           0x40085000
#define BT_MAC_BASE         0x40090000



/****************** Header Pointers Definition *****************/
#define hwp_hpsys_rcc   ((HPSYS_RCC_TypeDef     *)    HPSYS_RCC_BASE)
#define hwp_lpsys_rcc   ((LPSYS_RCC_TypeDef     *)    LPSYS_RCC_BASE)
#define hwp_gpdma1      ((GPDMA_TypeDef         *)    GPDMA1_BASE)
#define hwp_dmac1       ((GPDMA_TypeDef         *)    GPDMA1_BASE)
#define hwp_dmac2       ((DMAC_TypeDef          *)    DMAC2_BASE)
#define hwp_atim1       ((ATIM_TypeDef          *)    ATIM1_BASE)
#define hwp_audprc      ((AUDPRC_TypeDef        *)    AUDPRC_BASE)
#define hwp_audcodec    ((AUDCODEC_TypeDef      *)    AUDCODEC_BASE)
#define hwp_gptim1      ((GPT_TypeDef           *)    GPTIM1_BASE)
#define hwp_gptim2      ((GPT_TypeDef           *)    GPTIM2_BASE)
#define hwp_btim1       ((BTIM_TypeDef          *)    BTIM1_BASE)
#define hwp_btim2       ((BTIM_TypeDef          *)    BTIM2_BASE)
#define hwp_btim3       ((BTIM_TypeDef          *)    BTIM3_BASE)
#define hwp_btim4       ((BTIM_TypeDef          *)    BTIM4_BASE)
#define hwp_epic        ((EPIC_TypeDef          *)    EPIC_BASE)
#define hwp_v2d_gpu     ((V2D_GPU_TypeDef       *)    V2D_GPU_BASE)
#define hwp_spi1        ((SPI_TypeDef           *)    SPI1_BASE)
#define hwp_spi2        ((SPI_TypeDef           *)    SPI2_BASE)
#define hwp_usart1      ((USART_TypeDef         *)    USART1_BASE)
#define hwp_usart2      ((USART_TypeDef         *)    USART2_BASE)
#define hwp_usart3      ((USART_TypeDef         *)    USART3_BASE)
#define hwp_usart4      ((USART_TypeDef         *)    USART4_BASE)
#define hwp_usart5      ((USART_TypeDef         *)    USART5_BASE)
#define hwp_secu2       ((SECU2_TypeDef         *)    SECU2_BASE)
#define hwp_i2c1        ((I2C_TypeDef           *)    I2C1_BASE)
#define hwp_i2c2        ((I2C_TypeDef           *)    I2C2_BASE)
#define hwp_i2c3        ((I2C_TypeDef           *)    I2C3_BASE)
#define hwp_i2c4        ((I2C_TypeDef           *)    I2C4_BASE)
#define hwp_mailbox1    ((MAILBOX1_TypeDef      *)    MAILBOX1_BASE)
#define hwp_mailbox2    ((MAILBOX2_TypeDef      *)    MAILBOX2_BASE)
#define hwp_ptc1        ((PTC_TypeDef           *)    PTC1_BASE)
#define hwp_ptc2        ((PTC_TypeDef           *)    PTC2_BASE)
#define hwp_ezip1       ((EZIP_TypeDef          *)    EZIP1_BASE)
#define hwp_efusec      ((EFUSEC_TypeDef        *)    EFUSEC_BASE)
#define hwp_rtc         ((RTC_TypeDef           *)    RTC_BASE)
#define hwp_pmuc        ((PMUC_TypeDef          *)    PMUC_BASE)
#define hwp_mpi1        ((MPI_TypeDef           *)    MPI1_BASE)
#define hwp_mpi2        ((MPI_TypeDef           *)    MPI2_BASE)
#define hwp_mpi3        ((MPI_TypeDef           *)    MPI3_BASE)
#define hwp_lptim1      ((LPTIM_TypeDef         *)    LPTIM1_BASE)
#define hwp_lptim2      ((LPTIM_TypeDef         *)    LPTIM2_BASE)
#define hwp_lptim3      ((LPTIM_TypeDef         *)    LPTIM3_BASE)
#define hwp_hpsys_cfg   ((HPSYS_CFG_TypeDef     *)    HPSYS_CFG_BASE)
#define hwp_lpsys_cfg   ((LPSYS_CFG_TypeDef     *)    LPSYS_CFG_BASE)
#define hwp_i2s1        ((I2S_TypeDef           *)    I2S1_BASE)
#define hwp_pdm1        ((PDM_TypeDef           *)    PDM1_BASE)
#define hwp_crc1        ((CRC_TypeDef           *)    CRC1_BASE)
#define hwp_crc2        ((CRC_TypeDef           *)    CRC2_BASE)
#define hwp_trng        ((TRNG_TypeDef          *)    TRNG_BASE)
#define hwp_lcdc1       ((LCD_IF_TypeDef        *)    LCDC1_BASE)
#define hwp_secu1       ((SECU1_TypeDef         *)    SECU1_BASE)
#define hwp_sdmmc1      ((SD_TypeDef            *)    SDMMC1_BASE)
#define hwp_sdmmc2      ((SD_TypeDef            *)    SDMMC2_BASE)
#define hwp_aes_acc     ((AES_ACC_TypeDef       *)    AES_BASE)
#define hwp_i2s2        ((I2S_TypeDef           *)    I2S2_BASE)
#define hwp_gpio1       ((GPIO_TypeDef          *)    GPIO1_BASE)
#define hwp_gpio2       ((GPIO_TypeDef          *)    GPIO2_BASE)
#define hwp_usbc        ((USBC_X_Typedef        *)    USBC_BASE)
#define hwp_dcmi        ((DCMI_TypeDef          *)    DCMI_BASE)
#define hwp_pinmux1     ((HPSYS_PINMUX_TypeDef  *)    PINMUX1_BASE)
#define hwp_hpsys_aon   ((HPSYS_AON_TypeDef     *)    HPSYS_AON_BASE)
#define hwp_lpsys_aon   ((LPSYS_AON_TypeDef     *)    LPSYS_AON_BASE)
#define hwp_gpadc       ((GPADC_TypeDef         *)    GPADC_BASE)
#define hwp_patch       ((PATCH_TypeDef         *)    PATCH_BASE)
#define hwp_bt_rfc      ((BT_RFC_TypeDef        *)    BT_RFC_REG_BASE)
#define hwp_bt_phy      ((BT_PHY_TypeDef        *)    BT_PHY_BASE)
#define hwp_bt_mac      ((BT_MAC_TypeDef        *)    BT_MAC_BASE)
#define hwp_wdt2        ((WDT_TypeDef           *)    WDT2_BASE)
#define hwp_iwdt        ((WDT_TypeDef           *)    IWDT_BASE)
#define hwp_tsen        ((TSEN_TypeDef          *)    TSEN_BASE)
#define hwp_jpegd       ((JPEGD_TypeDef         *)    JPEGD_BASE)
#define hwp_can1        ((CAN_TypeDef           *)    CAN1_BASE)


/**=================================Extra defines by firmware ==========================================*/
/** Get mailbox base type*/
#define hwp_qspi1       hwp_mpi1
#define hwp_qspi2       hwp_mpi2
#define hwp_qspi3       hwp_mpi3

#define hwp_hmailbox    ((MAILBOX1_TypeDef       *)    MAILBOX1_BASE)
#define hwp_lmailbox    ((MAILBOX2_TypeDef       *)    MAILBOX2_BASE)
#define hwp_usbc_x      ((USBC_X_Typedef        *)    USBC_BASE))

#define hwp_gpadc1    hwp_gpadc
#ifdef SOC_BF0_LCPU
#define hwp_crc         hwp_crc2
#else
#define hwp_crc         hwp_crc1
#endif


#define USART1        hwp_usart1
#define USART2        hwp_usart2
#define USART3        hwp_usart3
#define USART4        hwp_usart4
#define USART5        hwp_usart5

#define DMA1          hwp_dmac1
#define DMA2          hwp_dmac2

#define FLASH1        hwp_qspi1
#define FLASH2        hwp_qspi2
#define FLASH3        hwp_qspi3

#define SDIO1          hwp_sdmmc1

#define SPI1          hwp_spi1
#define SPI2          hwp_spi2

#define GPTIM1        hwp_gptim1
#define GPTIM2        hwp_gptim2
#define GPTIM3        hwp_gptim3
#define GPTIM4        hwp_gptim4
#define GPTIM5        hwp_gptim5
#define ATIM1         hwp_atim1
#define BTIM1         hwp_btim1
#define BTIM2         hwp_btim2
#define BTIM3         hwp_btim3
#define BTIM4         hwp_btim4
#define LPTIM1        hwp_lptim1
#define LPTIM2        hwp_lptim2
#define LPTIM3        hwp_lptim3
#define TRNG          hwp_trng


/** Mailbox instances */
#define HMAILBOX_BASE       MAILBOX1_BASE
#define LMAILBOX_BASE       MAILBOX2_BASE

/** HCPU2LCPU mailbox instance */
#define H2L_MAILBOX   ((MAILBOX_CH_TypeDef *)HMAILBOX_BASE)

/** HCPU mutex instance channel1 */
#define HMUTEX_CH1    ((MUTEX_CH_TypeDef *)&hwp_hmailbox->C1IER)
/** HCPU mutex instance channel2 */
#define HMUTEX_CH2    ((MUTEX_CH_TypeDef *)&hwp_hmailbox->C2IER)

/** LCPU2HCPU mailbox instance */
#define L2H_MAILBOX   ((MAILBOX_CH_TypeDef *)LMAILBOX_BASE)
/** LCPU mutex instance channel1 */
#define LMUTEX_CH1    ((MUTEX_CH_TypeDef *)&hwp_lmailbox->C1IER)
/** LCPU mutex instance channel2 */
#define LMUTEX_CH2    ((MUTEX_CH_TypeDef *)&hwp_lmailbox->C2IER)

/** EPIC instance */
#define EPIC            hwp_epic
#define LCDC1           hwp_lcdc1


#define I2C1           hwp_i2c1
#define I2C2           hwp_i2c2
#define I2C3           hwp_i2c3
#define I2C4           hwp_i2c4

#define CRC            hwp_crc
/** EZIP instance */
#define EZIP           hwp_ezip1


#define DMA1_Channel1       ((DMA_Channel_TypeDef *) &DMA1->CCR1)
#define DMA1_Channel2       ((DMA_Channel_TypeDef *) &DMA1->CCR2)
#define DMA1_Channel3       ((DMA_Channel_TypeDef *) &DMA1->CCR3)
#define DMA1_Channel4       ((DMA_Channel_TypeDef *) &DMA1->CCR4)
#define DMA1_Channel5       ((DMA_Channel_TypeDef *) &DMA1->CCR5)
#define DMA1_Channel6       ((DMA_Channel_TypeDef *) &DMA1->CCR6)
#define DMA1_Channel7       ((DMA_Channel_TypeDef *) &DMA1->CCR7)
#define DMA1_Channel8       ((DMA_Channel_TypeDef *) &DMA1->CCR8)
#define DMA1_Channel9       ((DMA_Channel_TypeDef *) &DMA1->CCR9)
#define DMA1_Channel10      ((DMA_Channel_TypeDef *) &DMA1->CCR10)
//TODO: whether suitable for dynamic allocation?
#define DMA1_CHANNEL_NUM    (10)
#define DMA1_CSELR          ((DMA_Request_TypeDef *) &DMA1->CSELR1)
#define DMA2_Channel1       ((DMA_Channel_TypeDef *) &DMA2->CCR1)
#define DMA2_Channel2       ((DMA_Channel_TypeDef *) &DMA2->CCR2)
#define DMA2_Channel3       ((DMA_Channel_TypeDef *) &DMA2->CCR3)
#define DMA2_Channel4       ((DMA_Channel_TypeDef *) &DMA2->CCR4)
#define DMA2_Channel5       ((DMA_Channel_TypeDef *) &DMA2->CCR5)
#define DMA2_Channel6       ((DMA_Channel_TypeDef *) &DMA2->CCR6)
#define DMA2_Channel7       ((DMA_Channel_TypeDef *) &DMA2->CCR7)
#define DMA2_Channel8       ((DMA_Channel_TypeDef *) &DMA2->CCR8)
#define DMA2_CSELR          ((DMA_Request_TypeDef *) &DMA2->CSELR1)
#define DMA2_CHANNEL_NUM    (8)

#define HASH_ACC_BASE       (&hwp_aes_acc->HASH_SETTING)
#ifdef SOC_BF0_HCPU
#define PBR_BASE            ((uint32_t)&hwp_rtc->PBR0R)
/** PBR, placeholder for PBR pin, interface is different from GPIO actually  */
#define hwp_pbr             ((GPIO_TypeDef         *)PBR_BASE)
#endif /* SOC_BF0_HCPU */


//#define PMUC_VER   ()

/**
 *
 * @} Peripheral_memory_map
 */


#define HCPU2LCPU_OFFSET       (0x0A000000)
#define LCPU_CBUS_2_HCPU_OFSET (0x20800000)
#define LCPUROM2HCPU_OFFSET    (LCPU_CBUS_2_HCPU_OFSET)
#define LCPUITCM2HCPU_OFFSET   (LCPU_CBUS_2_HCPU_OFSET)
#define LCPU_SBUS_2_HCPU_OFFSET (0x00000000)
#define LCPUDTCM2HCPU_OFFSET   (LCPU_SBUS_2_HCPU_OFFSET)
#define LCPURAM2HCPU_OFFSET    (LCPU_SBUS_2_HCPU_OFFSET)


typedef enum
{
    RESET = 0,
    SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum
{
    ERROR = 0,
    SUCCESS = !ERROR
} ErrorStatus;


#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define IS_LPUART_INSTANCE(INSTANCE)    (0)

#define HCPU_IS_SRAM_ADDR(addr)  (((uint32_t)(addr) >= HPSYS_RAM0_BASE) && ((uint32_t)(addr) < HPSYS_RAM_END))
/**
  * @brief  Convert HCPU SRAM address which can be used by LCPU
  * @param  addr HCPU SRAM address
  * @retval address which can be accessed by LCPU
*/
#define HCPU_ADDR_2_LCPU_ADDR(addr)     (HCPU_IS_SRAM_ADDR(addr) ? (uint32_t)((addr) + HCPU2LCPU_OFFSET) : (uint32_t)(addr))


#define HCPU_MPI_CBUS_ADDR_2_SBUS_ADDR(addr) ((uint32_t)(addr) + HPSYS_MPI_MEM_CBUS_2_SBUS_OFFSET)
#define HCPU_MPI_SBUS_ADDR_2_CBUS_ADDR(addr) ((uint32_t)(addr) - HPSYS_MPI_MEM_CBUS_2_SBUS_OFFSET)

#define HCPU_IS_MPI_CBUS_ADDR(addr)  (((uint32_t)(addr) >= MPI1_MEM_BASE) && ((uint32_t)(addr) < (MPI3_MEM_BASE+QSPI3_MAX_SIZE)))

#define HCPU_MPI_SBUS_ADDR(addr)       (HCPU_IS_MPI_CBUS_ADDR(addr) ? HCPU_MPI_CBUS_ADDR_2_SBUS_ADDR(addr) : ((uint32_t)(addr)))

#define HCPU_MPI_CBUS_ADDR(addr)       (HCPU_IS_MPI_CBUS_ADDR(addr) ? ((uint32_t)(addr)) : HCPU_MPI_SBUS_ADDR_2_CBUS_ADDR(addr))

/**
  * @brief  Convert LCPU SRAM address which can be used by HCPU
  * @param  addr LCPU SRAM address
  * @retval address which can be accessed by HCPU
*/
#define LCPU_ADDR_2_HCPU_ADDR(addr) ((addr) + LCPURAM2HCPU_OFFSET)

/**
  * @brief  Convert LCPU ROM address which can be used by HCPU
  * @param  addr LCPU ROM address
  * @retval address which can be accessed by HCPU
*/
#define LCPU_ROM_ADDR_2_HCPU_ADDR(addr) ((addr) + LCPUROM2HCPU_OFFSET)
/**
  * @brief  Convert LCPU ITCM address which can be used by HCPU
  * @param  addr LCPU ITCM address
  * @retval address which can be accessed by HCPU
*/
#define LCPU_ITCM_ADDR_2_HCPU_ADDR(addr) ((addr) + LCPUITCM2HCPU_OFFSET)
/**
  * @brief  Convert LCPU DTCM address which can be used by HCPU
  * @param  addr LCPU ITCM address
  * @retval address which can be accessed by HCPU
*/
#define LCPU_DTCM_ADDR_2_HCPU_ADDR(addr) ((addr) + LCPUDTCM2HCPU_OFFSET)

#define GPADC_CALIB_FLOW_VERSION        (4)

#ifndef LCPU_BOOT_ADDR
#define LCPU_BOOT_ADDR          (LCPU_RAM_DATA_START_ADDR+LCPU_RAM_DATA_SIZE-4)
#endif

#define IS_LCPU(id)  ((*id)&1)

/* Peripheral_timer_clk */
#define FIXED_GPTBTIM_SRC_CLK   (48000000)

#if defined (USE_HAL_DRIVER)
#include "bf0_hal.h"
#endif /* USE_HAL_DRIVER */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
