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

void SystemClock_Config(void)
{
}

#if 0
const unsigned short int pin_array[][3] =
{
    { PAD_PA19, PA19_I2C_UART, 4},
    { PAD_PA18, PA18_I2C_UART, 4},

    { PAD_SA00, MPI1_DIO2,  5},
    { PAD_SA01, MPI1_CS,    5},
    { PAD_SA02, MPI1_DIO1,  5},
    { PAD_SA03, MPI1_DIO2,  5},
    { PAD_SA04, MPI1_CS,    5},
    { PAD_SA07, MPI1_DIO0,  5},
    { PAD_SA08, MPI1_DIO3,  5},
    { PAD_SA09, MPI1_CLK,   5},
    { PAD_SA10, MPI1_DIO3,  5},
    { PAD_SA11, MPI1_DIO0,  5},

    { PAD_PA16, MPI2_CLK, 1},
    { PAD_PA12, MPI2_CS,  1},
    { PAD_PA15, MPI2_DIO0, 1},
    { PAD_PA13, MPI2_DIO1, 1},
    { PAD_PA14, MPI2_DIO2, 1},
    { PAD_PA17, MPI2_DIO3, 1},

    { PAD_PA14, SD1_CLK,  2},
    { PAD_PA15, SD1_CMD,  2},
    { PAD_PA16, SD1_DIO0, 2},
    { PAD_PA17, SD1_DIO1, 2},
    { PAD_PA12, SD1_DIO2, 2},
    { PAD_PA13, SD1_DIO3, 2},
    { PAD_PA23, GPIO_A23, 0},

};

int HAL_PIN_Func2Idx(int pad, pin_function func, int hcpu)
{
    int i;

    for (i = 0; i < sizeof(pin_array) / sizeof(pin_array[0]); i++)
        if (pad == pin_array[i][0] && func == pin_array[i][1])
            return pin_array[i][2];
    return 0;
}
#endif

// Do not use HAL_PIN_Get in bootloader ROM
pin_function HAL_PIN_Idx2Func(int pad, int idx,  int hcpu)
{
    return 0;
}

void board_pinmux_mpi1_puya_base()
{
    HAL_PIN_Set(PAD_SA01, MPI1_CS,   PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA09, MPI1_CLK,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA07, MPI1_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA02, MPI1_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA10, MPI1_DIO3, PIN_NOPULL, 1);
}

void board_pinmux_mpi1_puya_ext(int is64Mb)
{
    HAL_PIN_Set_Analog(PAD_SA04, 1);
    HAL_PIN_Set_Analog(PAD_SA05, 1);
    HAL_PIN_Set_Analog(PAD_SA06, 1);
    HAL_PIN_Set_Analog(PAD_SA08, 1);
    HAL_PIN_Set_Analog(PAD_SA11, 1);
    HAL_PIN_Set_Analog(PAD_SA12, 1);

    if (is64Mb)
    {
        HAL_PIN_Set_Analog(PAD_SA00, 1);
        HAL_PIN_Set(PAD_SA03, MPI1_DIO2, PIN_PULLUP, 1);
    }
    else
    {
        HAL_PIN_Set_Analog(PAD_SA03, 1);
        HAL_PIN_Set(PAD_SA00, MPI1_DIO2, PIN_PULLUP, 1);
    }
}

void board_pinmux_mpi1_gd()
{
    HAL_PIN_Set(PAD_SA04, MPI1_CS,   PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA09, MPI1_CLK,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA11, MPI1_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA02, MPI1_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA00, MPI1_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_SA08, MPI1_DIO3, PIN_PULLUP, 1);

    HAL_PIN_Set_Analog(PAD_SA01, 1);
    HAL_PIN_Set_Analog(PAD_SA03, 1);
    HAL_PIN_Set_Analog(PAD_SA05, 1);
    HAL_PIN_Set_Analog(PAD_SA06, 1);
    HAL_PIN_Set_Analog(PAD_SA07, 1);
    HAL_PIN_Set_Analog(PAD_SA10, 1);
    HAL_PIN_Set_Analog(PAD_SA12, 1);

}

void board_pinmux_mpi3()
{
    HAL_PIN_Set(PAD_PA16, MPI3_CLK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA12, MPI3_CS,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA15, MPI3_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA13, MPI3_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA14, MPI3_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA17, MPI3_DIO3, PIN_PULLUP, 1);
}

void board_pinmux_sd()
{
    HAL_PIN_Set(PAD_PA15, SD1_CMD, PIN_PULLUP, 1);
    HAL_Delay_us(20);   // add a delay before clock setting to avoid wrong cmd happen

    HAL_PIN_Set(PAD_PA14, SD1_CLK,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA16, SD1_DIO0, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA17, SD1_DIO1, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA12, SD1_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA13, SD1_DIO3, PIN_PULLUP, 1);
}

void board_pinmux_uart()
{
    HAL_PIN_Set(PAD_PA19, USART1_TXD, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA18, USART1_RXD, PIN_PULLUP, 1);
}

static void board_pinmux_mpi1_none(void)
{
    uint32_t i;

    for (i = 0; i <= 12; i++)
    {
        HAL_PIN_Set_Analog(PAD_SA00 + i, 1);
    }
}

int board_boot_from(void)
{
    uint32_t pid = (hwp_hpsys_cfg->IDR & HPSYS_CFG_IDR_PID_Msk) >> HPSYS_CFG_IDR_PID_Pos;
    int r;

    pid &= 7;
    if (BOOT_SIP_NONE == pid)
        board_pinmux_mpi1_none();

    if (pid == BOOT_SIP_PUYA)
        r = BOOT_FROM_SIP_PUYA;
    else if (pid == BOOT_SIP_GD)
        r = BOOT_FROM_SIP_GD;
    else
    {
#ifdef CFG_BOOTROM
//TODO:
#ifdef PMUC_CR_PIN_RET
        hwp_pmuc->CR &= ~PMUC_CR_PIN_RET;
        HAL_Delay_us(100);
#endif /* PMUC_CR_PIN_RET */
        // Use external PIN(DIO1,DIO3) to detect NOR/NAND/SD-NAND
        int ext_bond = HAL_GPIO_ReadPin(hwp_gpio1, 17);      // A17,  MPI2_DIO3
        ext_bond |= (HAL_GPIO_ReadPin(hwp_gpio1, 13) << 1);  // A13,  MPI2_DIO1

        if (ext_bond == 0)
        {
            r = BOOT_FROM_NOR;
        }
        else if (ext_bond == 1)
        {
            r = BOOT_FROM_NAND;
        }
        else
        {
            r = (ext_bond == 2) ? BOOT_FROM_SD : BOOT_FROM_EMMC;
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



