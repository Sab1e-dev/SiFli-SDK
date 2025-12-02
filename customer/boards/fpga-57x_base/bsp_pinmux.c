/*
 * SPDX-FileCopyrightText: 2019-2022 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bsp_board.h"

#ifdef BSP_USING_PSRAM1
/* APS 128p*/
static void board_pinmux_psram_func0()
{
    HAL_PIN_Set(PAD_SA01, MPI1_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA02, MPI1_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA03, MPI1_DIO2, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA04, MPI1_DIO3, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA05, MPI1_DIO4, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA06, MPI1_DIO5, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA07, MPI1_DIO6, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA08, MPI1_DIO7, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA09, MPI1_DQSDM, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA10, MPI1_CLK,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA11, MPI1_CS,   PIN_NOPULL, 1);

    HAL_PIN_Set_Analog(PAD_SA00, 1);
    HAL_PIN_Set_Analog(PAD_SA12, 1);
}

/* APS 1:64p 2:32P, 4:Winbond 32/64/128p*/
static void board_pinmux_psram_func1_2_4(int func)
{
    HAL_PIN_Set(PAD_SA01, MPI1_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA02, MPI1_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA03, MPI1_DIO2, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA04, MPI1_DIO3, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA08, MPI1_DIO4, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA09, MPI1_DIO5, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA10, MPI1_DIO6, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA11, MPI1_DIO7, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA07, MPI1_CLK,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA05, MPI1_CS,   PIN_NOPULL, 1);

#ifdef FPGA
    HAL_PIN_Set(PAD_SA00, MPI1_DM, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA06, MPI1_CLKB, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA12, MPI1_DQSDM, PIN_PULLDOWN, 1);
#else
    switch (func)
    {
    case 1:             // APS 64P XCELLA
        HAL_PIN_Set(PAD_SA12, MPI1_DQSDM, PIN_PULLDOWN, 1);
        HAL_PIN_Set_Analog(PAD_SA00, 1);
        HAL_PIN_Set_Analog(PAD_SA06, 1);
        break;
    case 2:             // APS 32P LEGACY
        HAL_PIN_Set(PAD_SA00, MPI1_DM, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA12, MPI1_DQS, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA06, MPI1_CLKB, PIN_NOPULL, 1);
        break;
    case 4:             // Winbond 32/64/128p
        //HAL_PIN_Set(PAD_SA06, MPI1_CLKB, PIN_NOPULL, 1);
        HAL_PIN_Set(PAD_SA12, MPI1_DQSDM, PIN_NOPULL, 1);
        HAL_PIN_Set_Analog(PAD_SA00, 1);
        HAL_PIN_Set_Analog(PAD_SA06, 1);
        break;
    }
#endif
}

/* APS 16p*/
static void board_pinmux_psram_func3()
{
    HAL_PIN_Set(PAD_SA09, MPI1_CLK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA08, MPI1_CS,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_SA05, MPI1_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA07, MPI1_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_SA06, MPI1_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_SA10, MPI1_DIO3, PIN_PULLUP, 1);

    HAL_PIN_Set_Analog(PAD_SA00, 1);
    HAL_PIN_Set_Analog(PAD_SA01, 1);
    HAL_PIN_Set_Analog(PAD_SA02, 1);
    HAL_PIN_Set_Analog(PAD_SA03, 1);
    HAL_PIN_Set_Analog(PAD_SA04, 1);
    HAL_PIN_Set_Analog(PAD_SA11, 1);
    HAL_PIN_Set_Analog(PAD_SA12, 1);
}

static void board_pinmux_mpi1_none(void)
{
    uint32_t i;

    for (i = 0; i <= 12; i++)
    {
        HAL_PIN_Set_Analog(PAD_SA00 + i, 1);
    }
}
#endif
void BSP_PIN_Init(void)
{
#ifdef SOC_BF0_HCPU
    // HCPU pins

    //touch
    HAL_PIN_Set(PAD_PA46, I2C1_SCL, PIN_PULLUP, 1); //scl
    HAL_PIN_Set(PAD_PA45, I2C1_SDA, PIN_PULLUP, 1); //sda
    //HAL_PIN_Set(PAD_PA01, GPIO_A1, PIN_NOPULL, 1); //reset

    uint32_t pid = (hwp_hpsys_cfg->IDR & HPSYS_CFG_IDR_PID_Msk) >> HPSYS_CFG_IDR_PID_Pos;

    pid &= 7;
    pid = 3;
    if (pid == 0)   // Puya flash
    {
        HAL_PIN_Set(PAD_SA01, MPI1_CS,   PIN_NOPULL, 1);
        HAL_PIN_Set(PAD_SA09, MPI1_CLK,  PIN_NOPULL, 1);
        HAL_PIN_Set(PAD_SA07, MPI1_DIO0, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA02, MPI1_DIO1, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA10, MPI1_DIO3, PIN_NOPULL, 1);

        if (PM_STANDBY_BOOT == SystemPowerOnModeGet())
        {
            /* not support yet, has DIO2 is decided by flash size */
            HAL_ASSERT(0);
        }

        //Should set in bootloader
        //HAL_PIN_Set(PAD_SA03, MPI1_DIO2, PIN_PULLUP, 1);   // 64Mbits only
        //HAL_PIN_Set(PAD_SA00, MPI1_DIO2, PIN_PULLUP, 1);   // 16/32Mbits
    }
    else if (pid == 1)  // GD Flash
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
    else    // psram
    {
#ifdef BSP_USING_PSRAM1
        switch (pid)
        {
        case 5: //BOOT_PSRAM_APS_16P:
            board_pinmux_psram_func3();         // 16Mb APM QSPI PSRAM
            break;
        case 4: //BOOT_PSRAM_APS_32P:
            board_pinmux_psram_func1_2_4(2);    // 32Mb APM LEGACY PSRAM
            break;
        case 6: //BOOT_PSRAM_WINBOND:                // Winbond HYPERBUS PSRAM
            board_pinmux_psram_func1_2_4(4);
            break;
        case 3: // BOOT_PSRAM_APS_64P:
            board_pinmux_psram_func1_2_4(1);    // 64Mb APM XCELLA PSRAM
            break;
        case 2: //BOOT_PSRAM_APS_128P:
            board_pinmux_psram_func0();         // 128Mb APM XCELLA PSRAM
            break;
        default:
            board_pinmux_mpi1_none();
            break;
        }
#endif /* BSP_USING_PSRAM1 */
    }


#ifndef USE_V2_MEM
    // MPI3
    HAL_PIN_Set(PAD_PA16, MPI3_CLK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA12, MPI3_CS,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA15, MPI3_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA13, MPI3_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA14, MPI3_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA17, MPI3_DIO3, PIN_PULLUP, 1);

    // SDIO
#ifdef BSP_USING_SDIO
    //TODO
    HAL_PIN_Set(PAD_PA14, SD1_CLK, PIN_NOPULL, 1); // SDIO1
    HAL_PIN_Set(PAD_PA15, SD1_CMD, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA16, SD1_DIO0, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA17, SD1_DIO1, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA12, SD1_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA13, SD1_DIO3, PIN_PULLUP, 1);
#endif
#else
    // MPI2
    HAL_PIN_Set(PAD_PA22, MPI2_CLK,  PIN_NOPULL,   1);
    HAL_PIN_Set(PAD_PA12, MPI2_CS,   PIN_NOPULL,   1);
    HAL_PIN_Set(PAD_PA27, MPI2_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA20, MPI2_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA26, MPI2_DIO2, PIN_PULLUP,   1);
    HAL_PIN_Set(PAD_PA15, MPI2_DIO3, PIN_PULLUP, 1);

    //SDIO
#ifdef BSP_USING_SDIO
    HAL_PIN_Set(PAD_PA08, SD2_CLK, PIN_NOPULL, 1);  // SD2
    HAL_PIN_Set(PAD_PA09, SD2_CMD, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA10, SD2_DIO0, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA11, SD2_DIO1, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA06, SD2_DIO2,  PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA07, SD2_DIO3, PIN_PULLUP, 1);
#endif
#endif
    // UART1
    HAL_PIN_Set(PAD_PA19, USART1_TXD, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA18, USART1_RXD, PIN_PULLUP, 1);

#if 0
    // I2S2
    HAL_PIN_Set(PAD_PA82, I2S2_SDO, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA91, I2S2_BCK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA84, I2S2_LRCK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA86, I2S2_SDI, PIN_PULLDOWN, 1);
#endif


    // PDM1
    HAL_PIN_Set(PAD_PA11, GPIO_A11, PIN_PULLUP, 1); //???
    HAL_PIN_Set(PAD_PA22, PDM1_CLK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA23, PDM1_DATA, PIN_PULLDOWN, 1);
    //I2S1
    HAL_PIN_Set(PAD_PA30, I2S1_LRCK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA29, I2S1_BCK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA28, I2S1_SDI, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA25, I2S1_SDO, PIN_NOPULL, 1);


    // UART2
    HAL_PIN_Set(PAD_PA27, USART2_TXD, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA26, USART2_RXD, PIN_PULLUP, 1);

    HAL_PIN_Set(PAD_PA04, LCDC1_SPI_CLK, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA03, LCDC1_SPI_CS, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA05, LCDC1_SPI_DIO0, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA06, LCDC1_SPI_DIO1, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA07, LCDC1_SPI_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA08, LCDC1_SPI_DIO3, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA02, LCDC1_SPI_TE, PIN_PULLUP, 1);
    //HAL_PIN_Set(PAD_PA00, LCDC1_SPI_RSTB, PIN_PULLUP, 1);

    // Key1
    HAL_PIN_Set(PAD_PA35, GPIO_A35, PIN_NOPULL, 0);

    // I2C2
    HAL_PIN_Set(PAD_PA47, I2C2_SDA, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA48, I2C2_SCL, PIN_PULLUP, 1);

    // I2C1
    HAL_PIN_Set(PAD_PA45, I2C1_SDA, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA46, I2C1_SCL, PIN_PULLUP, 1);

#ifdef BSP_USING_DCMI
    // Digital Camera Interface
    HAL_PIN_Set(PAD_PA30, GPTIM2_CH1, PIN_PULLUP, 1); // mclk output

    HAL_PIN_Set(PAD_PA49, DCMI_CLK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA31, DCMI_VSYNC, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA32, DCMI_HSYNC, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA50, DCMI_DI0, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA51, DCMI_DI1, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA52, DCMI_DI2, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA53, DCMI_DI3, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA54, DCMI_DI4, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA55, DCMI_DI5, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA56, DCMI_DI6, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA57, DCMI_DI7, PIN_NOPULL, 1);
#endif

#endif

    // LCPU pins

    // UART4
    HAL_PIN_Set(PAD_PB01, USART4_TXD, PIN_PULLUP, 0);
    HAL_PIN_Set(PAD_PB00, USART4_RXD, PIN_PULLUP, 0);
    //HAL_PIN_Set(PAD_PB18, USART5_TXD, PIN_PULLUP, 0);
    //HAL_PIN_Set(PAD_PB17, USART5_RXD, PIN_PULLUP, 0);

    // UART5
    HAL_PIN_Set(PAD_PB03, USART5_TXD, PIN_PULLUP, 0);
    HAL_PIN_Set(PAD_PB02, USART5_RXD, PIN_PULLUP, 0);

}

