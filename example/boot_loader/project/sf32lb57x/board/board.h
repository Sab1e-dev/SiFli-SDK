/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "rtconfig.h"
#include <register.h>
#ifndef CFG_BOOTROM
    #include "drv_io.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ALIGN(n)                    __attribute__((aligned(n)))

/** boot mode pin: PA20 */
//TODO: need to be changed to PA00 by new fpga version
#define BOARD_BOOT_MODE_PIN  (20)
/* bootstrap pin that indicates external boot device
 * b2'00: NOR
 * b2'01: NAND
 * b2'10: SD
 * b2'11: eMMC
 */
/** board bootstrap pin bit0: PA17 */
#define BOARD_BOOTSTRAP_PIN_BIT0  (17)
/** board bootstrap pin bit1: PA13 */
#define BOARD_BOOTSTRAP_PIN_BIT1  (13)
/** boot from external NOR connected to MPI3 */
#define BOARD_BOOTSTRAP_FROM_EXT_NOR   (0)
/** boot from external NAND connected to MPI3 */
#define BOARD_BOOTSTRAP_FROM_EXT_NAND  (1)
/** boot from external SD-NAND or SD card connected to SDMMC1 */
#define BOARD_BOOTSTRAP_FROM_EXT_SD    (2)
/** boot from external eMMC connected to SDMMC1 */
#define BOARD_BOOTSTRAP_FROM_EXT_EMMC  (3)

typedef enum
{
    BOARD_BOOT_DEVICE_MPI2       = 0,   /**< boot from mpi2 */
    BOARD_BOOT_DEVICE_MPI1_TYPE1 = 1,   /**< boot from mpi1 type1 */
    BOARD_BOOT_DEVICE_MPI1_TYPE2 = 2,   /**< boot from mpi1 type2 */
    BOARD_BOOT_DEVICE_MPI3_NOR   = 3,   /**< boot from mpi3 nor */
    BOARD_BOOT_DEVICE_MPI3_NAND  = 4,   /**< boot from mpi3 nand */
    BOARD_BOOT_DEVICE_SD         = 5,   /**< boot from sd-nand or sd card */
    BOARD_BOOT_DEVICE_EMMC       = 6,   /**< boot from eMMC */
} board_boot_device_type_t;

extern board_boot_device_type_t board_boot_device;
board_boot_device_type_t board_boot_from(void);
void board_flash_power_on(void);
void board_init_psram(void);

extern void board_pinmux_uart(void);
extern void board_pinmux_mpi1_type1(void);
extern void board_pinmux_mpi1_type2(void);
extern void board_pinmux_mpi2(void);
extern void board_pinmux_mpi3(void);
extern void board_pinmux_sd(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H__ */
