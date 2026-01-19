/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-21     zylx         first version
 */

#include "board.h"
#include "boot_flash.h"

// #define BOOT_DEVICE_FORCED

board_boot_device_type_t board_boot_device;

void SystemClock_Config(void)
{
}

// Do not use HAL_PIN_Get in bootloader ROM
pin_function HAL_PIN_Idx2Func(int pad, int idx,  int hcpu)
{
    return 0;
}

void board_pinmux_mpi1_type1(void)
{
    HAL_PIN_CompileTimeSet(PAD_SA10, MPI1_FLASH_CLK,  PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_SA00, MPI1_FLASH_CS,   PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_SA12, MPI1_FLASH_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_SA02, MPI1_FLASH_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_SA04, MPI1_FLASH_DIO2, PIN_PULLUP, 1);
    HAL_PIN_CompileTimeSet(PAD_SA11, MPI1_FLASH_DIO3, PIN_PULLUP, 1);
    //TODO: is it needed to set other to analog?
}

void board_pinmux_mpi1_type2(void)
{
    HAL_PIN_CompileTimeSet(PAD_SA10, MPI1_FLASH_CLK,  PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_SA03, MPI1_FLASH_CS,   PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_SA12, MPI1_FLASH_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_SA02, MPI1_FLASH_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_SA01, MPI1_FLASH_DIO2, PIN_PULLUP, 1);
    HAL_PIN_CompileTimeSet(PAD_SA09, MPI1_FLASH_DIO3, PIN_PULLUP, 1);
}

void board_pinmux_mpi2(void)
{
    HAL_PIN_CompileTimeSet(PAD_SB12, MPI2_CLK, PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_SB06, MPI2_CS,  PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_SB10, MPI2_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_SB05, MPI2_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_SB04, MPI2_DIO2, PIN_PULLUP, 1);
    HAL_PIN_CompileTimeSet(PAD_SB11, MPI2_DIO3, PIN_PULLUP, 1);
}

void board_pinmux_mpi3(void)
{
    HAL_PIN_CompileTimeSet(PAD_PA16, MPI3_CLK, PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_PA12, MPI3_CS,  PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_PA15, MPI3_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_PA13, MPI3_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_CompileTimeSet(PAD_PA14, MPI3_DIO2, PIN_PULLUP, 1);
    HAL_PIN_CompileTimeSet(PAD_PA17, MPI3_DIO3, PIN_PULLUP, 1);
}

void board_pinmux_sd()
{
    HAL_PIN_CompileTimeSet(PAD_PA15, SD1_CMD, PIN_PULLUP, 1);
    HAL_Delay_us(20);   // add a delay before clock setting to avoid wrong cmd happen

    HAL_PIN_CompileTimeSet(PAD_PA14, SD1_CLK,  PIN_NOPULL, 1);
    HAL_PIN_CompileTimeSet(PAD_PA16, SD1_DIO0, PIN_PULLUP, 1);
    HAL_PIN_CompileTimeSet(PAD_PA17, SD1_DIO1, PIN_PULLUP, 1);
    HAL_PIN_CompileTimeSet(PAD_PA12, SD1_DIO2, PIN_PULLUP, 1);
    HAL_PIN_CompileTimeSet(PAD_PA13, SD1_DIO3, PIN_PULLUP, 1);
}

static void board_pinmux_mpi1_none(void)
{
    uint32_t i;

    for (i = 0; i <= 12; i++)
    {
        HAL_PIN_Set_Analog(PAD_SA00 + i, 1);
    }
}

static uint8_t board_read_pkgid(void)
{
    uint8_t pkgid;
    int32_t r;

    r = HAL_EFUSE_Read2(EFUSE_PKGID_OFFSET, &pkgid, EFUSE_PKGID_SIZE);
    HAL_ASSERT(EFUSE_PKGID_SIZE == r);

    return pkgid;
}

static uint8_t board_get_soc_boot_device(void)
{
    uint8_t pkgid;

    pkgid = board_read_pkgid();

    return GET_REG_VAL2(pkgid, PKGID_BOOT_DEVICE);
}

static uint8_t board_get_bootstrap_type(void)
{
    uint8_t bootstrap_type;
    bootstrap_type = (uint8_t)HAL_GPIO_ReadPin(hwp_gpio1, BOARD_BOOTSTRAP_PIN_BIT0);
    bootstrap_type |= ((uint8_t)HAL_GPIO_ReadPin(hwp_gpio1, BOARD_BOOTSTRAP_PIN_BIT1) << 1);

    return bootstrap_type;
}

board_boot_device_type_t board_boot_from(void)
{
    uint8_t boot_device;
    board_boot_device_type_t r;
    uint8_t bootstrap_type;

    boot_device = board_get_soc_boot_device();

#ifdef BOOT_DEVICE_FORCED
    boot_device = PKGID_BOOT_DEVICE_EXT;
#endif /* BOOT_DEVICE_FORCED */
    if (boot_device == PKGID_BOOT_DEVICE_MPI2)
    {
        r = BOARD_BOOT_DEVICE_MPI2;
    }
    else if (boot_device == PKGID_BOOT_DEVICE_MPI1_TYPE1)
    {
        r = BOARD_BOOT_DEVICE_MPI1_TYPE1;
    }
    else if (boot_device == PKGID_BOOT_DEVICE_MPI1_TYPE2)
    {
        r = BOARD_BOOT_DEVICE_MPI1_TYPE2;
    }
    else
    {
#ifdef CFG_BOOTROM
//TODO:
#ifdef PMUC_CR_PIN_RET
        hwp_pmuc->CR &= ~PMUC_CR_PIN_RET;
        HAL_Delay_us(100);
#endif /* PMUC_CR_PIN_RET */
        bootstrap_type = board_get_bootstrap_type();

        if (BOARD_BOOTSTRAP_FROM_EXT_NOR == bootstrap_type)
        {
            r = BOARD_BOOT_DEVICE_MPI3_NOR;
        }
        else if (BOARD_BOOTSTRAP_FROM_EXT_NAND == bootstrap_type)
        {
            r = BOARD_BOOT_DEVICE_MPI3_NAND;
        }
        else if (BOARD_BOOTSTRAP_FROM_EXT_SD == bootstrap_type)
        {
            r = BOARD_BOOT_DEVICE_SD;
        }
        else if (BOARD_BOOTSTRAP_FROM_EXT_EMMC == bootstrap_type)
        {
            r = BOARD_BOOT_DEVICE_EMMC;
        }
        HAL_Set_backup(RTC_BACKUP_BOOTOPT, r);
#else
        r = HAL_Get_backup(RTC_BACKUP_BOOTOPT);
#endif
    }
    return r;
}

// Use internal LDO to power on flash.
//TODO:
void board_flash_power_on()
{
    // hwp_pmuc->PERI_LDO |= PMUC_PERI_LDO_EN_VDD33_LDO2;

    // No longer needed as bootmode delay is 1s.
    // HAL_Delay_us(0);
    // HAL_Delay_us(2000);
}



