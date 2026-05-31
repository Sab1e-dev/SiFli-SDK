/*
 * SPDX-FileCopyrightText: 2026 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "board.h"
#include "boot_flash.h"
#include "string.h"

void bootloader_switch_clock(int mpi)
{
    // HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_FLASH1, RCC_CLK_FLASH_DLL2);
}

void board_init_psram()
{
#ifndef CFG_BOOTROM
    bootloader_switch_clock(1);
#endif
    BSP_SetFlash1DIV(2);    // for QSPI PSRAM need set divider to avoid to high, OPI PSRAM do not care.

    bsp_psramc_init();
}


