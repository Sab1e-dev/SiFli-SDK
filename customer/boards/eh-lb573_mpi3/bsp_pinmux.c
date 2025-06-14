/**
  ******************************************************************************
  * @file   bsp_pinmux.c
  * @author Sifli software development team
  ******************************************************************************
*/
/**
 * @attention
 * Copyright (c) 2019 - 2022,  Sifli Technology
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
#include "bsp_board.h"
#include <string.h>


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
        break;
    case 2:             // APS 32P LEGACY
        HAL_PIN_Set(PAD_SA00, MPI1_DM, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA12, MPI1_DQS, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA06, MPI1_CLKB, PIN_NOPULL, 1);
        break;
    case 4:             // Winbond 32/64/128p
        //HAL_PIN_Set(PAD_SA06, MPI1_CLKB, PIN_NOPULL, 1);
        HAL_PIN_Set(PAD_SA12, MPI1_DQSDM, PIN_NOPULL, 1);
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
    HAL_PIN_Set(PAD_PA10, MPI1_DIO3, PIN_PULLUP, 1);
}

/* i2s pinmux set */
void i2s_pinmux_set(int idx)
{
    HAL_PIN_Set(PAD_PA06, I2S1_LRCK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA05, I2S1_BCK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA04, I2S1_SDI, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA03, I2S1_SDO, PIN_NOPULL, 1);
}

void board_pwm_pinmux_set(int pad, char *pwm, int channel)
{
    if (!strcmp(pwm, "pwm2")) // GPTIM1
    {
        if (channel == 1)
            HAL_PIN_Set(pad, GPTIM1_CH1, PIN_PULLUP, 1);
        else if (channel == 2)
            HAL_PIN_Set(pad, GPTIM1_CH2, PIN_PULLUP, 1);
        else if (channel == 3)
            HAL_PIN_Set(pad, GPTIM1_CH3, PIN_PULLUP, 1);
        else if (channel == 4)
            HAL_PIN_Set(pad, GPTIM1_CH4, PIN_PULLUP, 1);
    }
    else if (!strcmp(pwm, "pwm3")) // GPTIM2
    {
        if (channel == 1)
            HAL_PIN_Set(pad, GPTIM2_CH1, PIN_PULLUP, 1);
        else if (channel == 2)
            HAL_PIN_Set(pad, GPTIM2_CH2, PIN_PULLUP, 1);
        else if (channel == 3)
            HAL_PIN_Set(pad, GPTIM2_CH3, PIN_PULLUP, 1);
        else if (channel == 4)
            HAL_PIN_Set(pad, GPTIM2_CH4, PIN_PULLUP, 1);
    }
    else if (!strcmp(pwm, "pwma1")) // ATIM1
    {
        if (channel == 1)
            HAL_PIN_Set(pad, ATIM1_CH1, PIN_PULLUP, 1);
        else if (channel == 2)
            HAL_PIN_Set(pad, ATIM1_CH1, PIN_PULLUP, 1);
        else if (channel == 3)
            HAL_PIN_Set(pad, ATIM1_CH1, PIN_PULLUP, 1);
        else if (channel == 4)
            HAL_PIN_Set(pad, ATIM1_CH1, PIN_PULLUP, 1);
    }
    else if (!strcmp(pwm, "pwmlp1")) // LPTIM1
    {
        HAL_PIN_Set(pad, LPTIM1_OUT, PIN_PULLUP, 1);
    }
    else if (!strcmp(pwm, "pwmlp2")) // LPTIM2
    {
        HAL_PIN_Set(pad, LPTIM2_OUT, PIN_PULLUP, 1);
    }
}

void BSP_PIN_Init(void)
{
#ifdef SOC_BF0_HCPU
    // HCPU pins

    uint32_t pid = (hwp_hpsys_cfg->IDR & HPSYS_CFG_IDR_PID_Msk) >> HPSYS_CFG_IDR_PID_Pos;

    pid &= 7;
    if (pid == 0)   // Puya flash
    {
        HAL_PIN_Set(PAD_SA01, MPI1_CS,   PIN_NOPULL, 1);
        HAL_PIN_Set(PAD_SA09, MPI1_CLK,  PIN_NOPULL, 1);
        HAL_PIN_Set(PAD_SA07, MPI1_DIO0, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA02, MPI1_DIO1, PIN_PULLDOWN, 1);
        HAL_PIN_Set(PAD_SA10, MPI1_DIO3, PIN_NOPULL, 1);

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
        }
#endif /* BSP_USING_PSRAM1 */
    }

#ifdef BSP_USING_SDIO
    HAL_PIN_Set(PAD_PA15, SD1_CMD, PIN_PULLUP, 1);
    HAL_Delay_us(20);   // add a delay before clock setting to avoid wrong cmd happen

    HAL_PIN_Set(PAD_PA14, SD1_CLK,  PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA16, SD1_DIO0, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA17, SD1_DIO1, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA12, SD1_DIO2, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA13, SD1_DIO3, PIN_PULLUP, 1);
#else   // NOR/NAND FLASH
    // MPI2
    //TODO: pin_const not changed yet
    HAL_PIN_Set(PAD_PA16, MPI3_CLK,  PIN_NOPULL,   1);
    HAL_PIN_Set(PAD_PA12, MPI3_CS,   PIN_NOPULL,   1);
    HAL_PIN_Set(PAD_PA15, MPI3_DIO0, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA13, MPI3_DIO1, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA14, MPI3_DIO2, PIN_PULLUP,   1);
    HAL_PIN_Set(PAD_PA17, MPI3_DIO3, PIN_PULLUP, 1);
#endif

#if 1
    // UART1
    HAL_PIN_Set(PAD_PA19, USART1_TXD, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA18, USART1_RXD, PIN_PULLUP, 1);
#else
    //SWD
    HAL_PIN_Set(PAD_PA18, SWDIO, PIN_PULLDOWN, 1);
    HAL_PIN_Set(PAD_PA19, SWCLK, PIN_PULLDOWN, 1);
    HAL_PIN_SetMode(PAD_PA18, 1, PIN_DIGITAL_IO_PULLDOWN);
    HAL_PIN_SetMode(PAD_PA19, 1, PIN_DIGITAL_IO_PULLDOWN);
#endif /* 1 */


    HAL_PIN_Set(PAD_PA04, LCDC_SPI_CLK, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA03, LCDC_SPI_CS, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA05, LCDC_SPI_DIO0, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA06, LCDC_SPI_DIO1, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA07, LCDC_SPI_DIO2, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA08, LCDC_SPI_DIO3, PIN_NOPULL, 1);

#ifdef BSP_LCDC_USING_DDR_QADSPI
    HAL_PIN_Set_DS0(PAD_PA04, 1, 1);
    HAL_PIN_Set_DS0(PAD_PA05, 1, 0);
    HAL_PIN_Set_DS0(PAD_PA06, 1, 0);
    HAL_PIN_Set_DS0(PAD_PA07, 1, 0);
    HAL_PIN_Set_DS0(PAD_PA08, 1, 0);

    HAL_PIN_Set_DS1(PAD_PA04, 1, 1);
    HAL_PIN_Set_DS1(PAD_PA05, 1, 0);
    HAL_PIN_Set_DS1(PAD_PA06, 1, 0);
    HAL_PIN_Set_DS1(PAD_PA07, 1, 0);
    HAL_PIN_Set_DS1(PAD_PA08, 1, 0);
#endif /* DDR_LCD */


    HAL_PIN_Set(PAD_PA02, LCDC_SPI_TE, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA00, GPIO_A0, PIN_NOPULL, 1);

    //HAL_PIN_Set(PAD_PA00, LCDC1_SPI_RSTB, PIN_PULLUP, 1);
#if defined(LCD_USING_PWM_AS_BACKLIGHT)
    HAL_PIN_Set(PAD_PA01, GPTIM2_CH4, PIN_NOPULL, 1);   // LCDC1_BL_PWM_CTRL, LCD backlight PWM
#endif

    // Key1
    HAL_PIN_Set(PAD_PA34, GPIO_A34, PIN_NOPULL, 1);

    //HAL_PIN_Set(PAD_PB00, USART4_TXD, PIN_PULLUP, 0);
    //HAL_PIN_Set(PAD_PB01, USART4_RXD, PIN_PULLUP, 0);
#ifdef  BSP_USING_USBD
    HAL_PIN_Set(PAD_PA35, GPIO_A35, PIN_NOPULL, 1);
    HAL_PIN_Set(PAD_PA36, GPIO_A36, PIN_NOPULL, 1);
    HAL_PIN_Set_Analog(PAD_PA35, 1);
    HAL_PIN_Set_Analog(PAD_PA36, 1);
#else

#endif

#endif

    HAL_PIN_Set(PAD_PA42, I2C2_SCL, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA41, I2C2_SDA, PIN_PULLUP, 1);

    /* TP WKUP INT */
    HAL_PIN_Set(PAD_PA33, GPIO_A33, PIN_NOPULL, 1);
    /* TP RESET */
    HAL_PIN_Set(PAD_PA01, GPIO_A1, PIN_NOPULL, 1);

    HAL_PIN_Set(PAD_PA44, I2C1_SCL, PIN_PULLUP, 1);
    HAL_PIN_Set(PAD_PA43, I2C1_SDA, PIN_PULLUP, 1);

    // UART4
#ifdef BSP_USING_UART4
    HAL_PIN_Set(PAD_PA63, USART4_TXD, PIN_PULLUP, 0);
    HAL_PIN_Set(PAD_PA62, USART4_RXD, PIN_PULLUP, 0);
#endif
}


/************************ (C) COPYRIGHT Sifli Technology *******END OF FILE****/
