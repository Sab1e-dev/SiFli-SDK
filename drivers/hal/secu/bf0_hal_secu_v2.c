/**
  ******************************************************************************
  * @file   bf0_hal_secu_v2.c
  * @author Sifli software development team
  * @brief   Security HAL module driver.
  *          This file provides firmware functions to manage the following
  ******************************************************************************
*/
/**
 *
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

#include "bf0_hal.h"

/** @addtogroup BF0_HAL_Driver
  * @{
  */

/** @defgroup SECU Security
  * @brief Security HAL module driver
  * @{
  */

#if defined(HAL_SECU_MODULE_ENABLED)||defined(_SIFLI_DOXYGEN_)

volatile uint32_t *HAL_SECU_Master_Reg(SECU_MODULE_TYPE module, uint32_t *bit_n)
{
    volatile uint32_t *p_reg = NULL;
    HAL_ASSERT(bit_n != NULL);

    switch (module)
    {
    /*SECU1*/
    case SECU_MOD_PTC1:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_PTC1_SEC_Pos;
        break;
    case SECU_MOD_DMAC1:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_DMAC1_SEC_Pos;
        break;
    case SECU_MOD_USBC:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_USBC_SEC_Pos;
        break;
    case SECU_MOD_AES:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_AES_SEC_Pos;
        break;
    case SECU_MOD_LCDC1:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_LCDC_SEC_Pos;
        break;
    case SECU_MOD_EPIC:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_EPIC_SEC_Pos;
        break;
    case SECU_MOD_EZIP:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_EZIP_SEC_Pos;
        break;
    case SECU_MOD_HCPU:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG0);
        *bit_n = SECU1_HPMST_ATTR_CFG0_HCPU_SEC_Pos;
        break;
    case SECU_MOD_LCPU:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG1);
        *bit_n = SECU1_HPMST_ATTR_CFG1_LCPU_SEC_Pos;
        break;
    case SECU_MOD_JPEG:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG1);
        *bit_n = SECU1_HPMST_ATTR_CFG1_JPEG_SEC_Pos;
        break;
    case SECU_MOD_NPU:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG1);
        *bit_n = SECU1_HPMST_ATTR_CFG1_NPU_SEC_Pos;
        break;
    case SECU_MOD_V2DGPU:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG1);
        *bit_n = SECU1_HPMST_ATTR_CFG1_V2DGPU_SEC_Pos;
        break;
    case SECU_MOD_ACPU:
        p_reg = &(hwp_secu1->HPMST_ATTR_CFG1);
        *bit_n = SECU1_HPMST_ATTR_CFG1_ACPU_SEC_Pos;
        break;

    /*SECU2*/
    case SECU_MOD_PTC2:
        p_reg = &(hwp_secu2->LPMST_ATTR_CFG);
        *bit_n = SECU2_LPMST_ATTR_CFG_PTC2_SEC_Pos;
        break;
    case SECU_MOD_DMAC2:
        p_reg = &(hwp_secu2->LPMST_ATTR_CFG);
        *bit_n = SECU2_LPMST_ATTR_CFG_DMAC2_SEC_Pos;
        break;

    default:
        HAL_ASSERT(0);
    }

    return p_reg;
}

volatile uint32_t *HAL_SECU_Slave_Reg(SECU_MODULE_TYPE module, uint32_t *bit_n)
{
    volatile uint32_t *p_reg = NULL;
    HAL_ASSERT(bit_n != NULL);

    switch (module)
    {
    /*SECU1*/
    case SECU_MOD_GPIO1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_GPIO1_SEC_Pos;
        break;
    case SECU_MOD_MPI1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_MPI1_REG_SEC_Pos;
        break;
    case SECU_MOD_MPI2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_MPI2_REG_SEC_Pos;
        break;
    case SECU_MOD_MPI3:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_MPI3_REG_SEC_Pos;
        break;
    case SECU_MOD_SDMMC1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_SDMMC_SEC_Pos;
        break;
    case SECU_MOD_CRC1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_CRC1_SEC_Pos;
        break;
    case SECU_MOD_USBC:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_USBC_SEC_Pos;
        break;
    case SECU_MOD_DCMI:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_DCMI_SEC_Pos;
        break;
    case SECU_MOD_V2DGPU:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_V2DGPU_SEC_Pos;
        break;
    case SECU_MOD_EPIC:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_EPIC_SEC_Pos;
        break;
    case SECU_MOD_ACPU:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_ACPU_SEC_Pos;
        break;
    case SECU_MOD_RCC1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_RCC1_SEC_Pos;
        break;
    case SECU_MOD_PINMUX1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_PINMUX1_SEC_Pos;
        break;
    case SECU_MOD_ATIM1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_ATIM1_SEC_Pos;
        break;
    case SECU_MOD_AUDPRC:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_AUDPRC_SEC_Pos;
        break;
    case SECU_MOD_I2S1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG0);
        *bit_n = SECU1_HPSLV_ATTR_CFG0_I2S1_SEC_Pos;
        break;
    case SECU_MOD_SYSCFG1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_SYSCFG1_SEC_Pos;
        break;
    case SECU_MOD_EFUSE:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_EFUSE_SEC_Pos;
        break;
    case SECU_MOD_AES:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_AES_SEC_Pos;
        break;
    case SECU_MOD_TRNG:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_TRNG_SEC_Pos;
        break;
    case SECU_MOD_PTC1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_PTC1_SEC_Pos;
        break;
    case SECU_MOD_DMAC1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_DMAC1_SEC_Pos;
        break;
    case SECU_MOD_MABOX1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_MAILBOX1_SEC_Pos;
        break;
    case SECU_MOD_UART1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_USART1_SEC_Pos;
        break;
    case SECU_MOD_UART2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_USART2_SEC_Pos;
        break;
    case SECU_MOD_UART3:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_USART3_SEC_Pos;
        break;
    case SECU_MOD_AUDCODEC:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_AUDCODEC_SEC_Pos;
        break;
    case SECU_MOD_TSEN:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_TSEN_SEC_Pos;
        break;
    case SECU_MOD_GPTIM1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_GPTIM1_SEC_Pos;
        break;
    case SECU_MOD_GPTIM2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_GPTIM2_SEC_Pos;
        break;
    case SECU_MOD_BTIM1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_BTIM1_SEC_Pos;
        break;
    case SECU_MOD_BTIM2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG1);
        *bit_n = SECU1_HPSLV_ATTR_CFG1_BTIM2_SEC_Pos;
        break;
    case SECU_MOD_UART4:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_USART4_SEC_Pos;
        break;
    case SECU_MOD_CAN:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_CAN_SEC_Pos;
        break;
    case SECU_MOD_SDMMC2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_SDMMC2_SEC_Pos;
        break;
    case SECU_MOD_WDT1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_WDT1_SEC_Pos;
        break;
    case SECU_MOD_SPI1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_SPI1_SEC_Pos;
        break;
    case SECU_MOD_SPI2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_SPI2_SEC_Pos;
        break;
    case SECU_MOD_PDM1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_PDM1_SEC_Pos;
        break;
    case SECU_MOD_I2C1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_I2C1_SEC_Pos;
        break;
    case SECU_MOD_I2C2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_I2C2_SEC_Pos;
        break;
    case SECU_MOD_I2C3:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_I2C3_SEC_Pos;
        break;
    case SECU_MOD_I2C4:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_I2C4_SEC_Pos;
        break;
    case SECU_MOD_GPADC:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_GPADC_SEC_Pos;
        break;
    case SECU_MOD_EZIP:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_EZIP_SEC_Pos;
        break;
    case SECU_MOD_LCDC1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_LCDC_SEC_Pos;
        break;
    case SECU_MOD_JPEG:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_JPEG_SEC_Pos;
        break;
    case SECU_MOD_NPU:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG2);
        *bit_n = SECU1_HPSLV_ATTR_CFG2_NPU_SEC_Pos;
        break;
    case SECU_MOD_HPSYS_AON:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG3);
        *bit_n = SECU1_HPSLV_ATTR_CFG3_HPSYS_AON_SEC_Pos;
        break;
    case SECU_MOD_LPTIM1:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG3);
        *bit_n = SECU1_HPSLV_ATTR_CFG3_LPTIM1_SEC_Pos;
        break;
    case SECU_MOD_LPTIM2:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG3);
        *bit_n = SECU1_HPSLV_ATTR_CFG3_LPTIM2_SEC_Pos;
        break;
    case SECU_MOD_PMUC:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG3);
        *bit_n = SECU1_HPSLV_ATTR_CFG3_PMUC_SEC_Pos;
        break;
    case SECU_MOD_RTC:
        p_reg = &(hwp_secu1->HPSLV_ATTR_CFG3);
        *bit_n = SECU1_HPSLV_ATTR_CFG3_RTC_SEC_Pos;
        break;

    /*SECU2*/
    case SECU_MOD_AON:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG0);
        *bit_n = SECU2_LPSLV_ATTR_CFG0_AON_SEC_Pos;
        break;
    case SECU_MOD_GPIO2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG0);
        *bit_n = SECU2_LPSLV_ATTR_CFG0_GPIO2_SEC_Pos;
        break;
    case SECU_MOD_RFC:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG0);
        *bit_n = SECU2_LPSLV_ATTR_CFG0_RFC_SEC_Pos;
        break;
    case SECU_MOD_BT_PHY:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG0);
        *bit_n = SECU2_LPSLV_ATTR_CFG0_BT_PHY_SEC_Pos;
        break;
    case SECU_MOD_BT_MAC:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG0);
        *bit_n = SECU2_LPSLV_ATTR_CFG0_BT_MAC_SEC_Pos;
        break;
    case SECU_MOD_CRC2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG0);
        *bit_n = SECU2_LPSLV_ATTR_CFG0_CRC2_SEC_Pos;
        break;
    case SECU_MOD_RCC2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_RCC2_SEC_Pos;
        break;
    case SECU_MOD_DMAC2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_DMAC2_SEC_Pos;
        break;
    case SECU_MOD_MABOX2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_MAILBOX2_SEC_Pos;
        break;
    case SECU_MOD_PINMUX2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_PINMUX2_SEC_Pos;
        break;
    case SECU_MOD_PATCH:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_PATCH_SEC_Pos;
        break;
    case SECU_MOD_UART5:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_USART5_SEC_Pos;
        break;
    case SECU_MOD_BTIM3:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_BTIM3_SEC_Pos;
        break;
    case SECU_MOD_BTIM4:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_BTIM4_SEC_Pos;
        break;
    case SECU_MOD_WDT2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_WDT2_SEC_Pos;
        break;
    case SECU_MOD_PTC2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_PTC2_SEC_Pos;
        break;
    case SECU_MOD_SYSCFG2:
        p_reg = &(hwp_secu2->LPSLV_ATTR_CFG1);
        *bit_n = SECU2_LPSLV_ATTR_CFG1_SYSCFG2_SEC_Pos;
        break;
    default:
        HAL_ASSERT(0);
    }

    return p_reg;
}

HAL_StatusTypeDef HAL_SECU_SetAttr(SECU_MODULE_TYPE module, uint32_t role, uint32_t flag)
{
    volatile uint32_t *p_reg;
    uint32_t bit_n, tmp;

    if (role & SECU_ROLE_MASTER)
    {
        p_reg = HAL_SECU_Master_Reg(module, &bit_n);
        tmp = *p_reg;
        if ((module != SECU_MOD_HCPU) && (module != SECU_MOD_LCPU) && (module != SECU_MOD_ACPU)) //No privilege bit for HCPU&LCPU&ACPU
        {
            tmp |= 1 << (bit_n + 3); //set xxx_priv_use
            if (flag & SECU_FLAG_PRIV)
            {
                tmp |= 1 << (bit_n + 1); //set xxx_priv
            }
            else
            {
                tmp &= ~(1 << (bit_n + 1)); //clear xxx_priv
            }
        }

        tmp |= 1 << (bit_n + 2); //set xxx_sec_use
        if (flag & SECU_FLAG_SECU)
        {
            tmp |= 1 << (bit_n + 0); //set xxx_sec
        }
        else
        {
            tmp &= ~(1 << (bit_n + 0)); //clear xxx_sec
        }

        *p_reg = tmp;
    }

    if (role & SECU_ROLE_SLAVE)
    {
        p_reg = HAL_SECU_Slave_Reg(module, &bit_n);
        tmp = *p_reg;
        if (flag & SECU_FLAG_PRIV)
        {
            tmp |= 1 << (bit_n + 1); //set xxx_priv
        }
        else
        {
            tmp &= ~(1 << (bit_n + 1)); //clear xxx_priv
        }

        if (flag & SECU_FLAG_SECU)
        {
            tmp |= 1 << (bit_n + 0); //set xxx_sec
        }
        else
        {
            tmp &= ~(1 << (bit_n + 0)); //clear xxx_sec
        }

        *p_reg = tmp;
    }

    return HAL_OK;
}

/*Treat S-bus C-bus access as same address*/
#define SECU_MEM_ADDR(addr) ((addr) & 0x0FFFFFFF)
#define SECU_HPMEM_ADDR(addr) ((addr) & 0x00FFFFFF)

HAL_StatusTypeDef HAL_SECU_MemReg_CFG(SECU_MEM_TYPE memory_type, SECU_MemAttr_Reg *reg, uint32_t start, uint32_t end, uint32_t flag)
{
    uint32_t rw_cfg;

    if ((memory_type == SECU_MEM_MPI1)
            || (memory_type == SECU_MEM_MPI2)
            || (memory_type == SECU_MEM_MPI3)) //mpi memory
    {
        *reg->st_reg = SECU_MEM_ADDR(start);
        *reg->end_reg   = SECU_MEM_ADDR(end - SECU_MEM_MIN_BLOCK);
    }
    else //sram memory
    {
        if ((memory_type == SECU_MEM_LPSYS_RAM0)
                || (memory_type == SECU_MEM_LPSYS_RAM1))
        {
            *reg->st_reg = SECU_MEM_ADDR(start);
            *reg->end_reg   = SECU_MEM_ADDR(end - SECU_SRAM_MIN_BLOCK);
        }
        else
        {
            *reg->st_reg = SECU_HPMEM_ADDR(start);
            *reg->end_reg   = SECU_HPMEM_ADDR(end - SECU_SRAM_MIN_BLOCK);
        }
    }
    rw_cfg = *reg->rw_reg;
    rw_cfg &= ~(0xFUL << (reg->rg_of * 6 + 2));
    rw_cfg |= ((flag & 0xF) << (reg->rg_of * 6 + 2));
    *reg->rw_reg = rw_cfg;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_MemAddr_Check(SECU_MEM_TYPE memory_type, uint32_t start, uint32_t end)
{
    if (start >= end)
    {
        return HAL_ERROR;//end address must larger than start addr.
    }

    if ((memory_type == SECU_MEM_MPI1)
            || (memory_type == SECU_MEM_MPI2)
            || (memory_type == SECU_MEM_MPI3)) //mpi
    {
        if ((SECU_MEMRange_ALIGN(start) != start)
                || (SECU_MEMRange_ALIGN(end) != end))
        {
            return HAL_ERROR; //Address not aligned.
        }
    }
    else //sram
    {
        if ((SECU_SRAMRange_ALIGN(start) != start)
                || (SECU_SRAMRange_ALIGN(end) != end))
        {
            return HAL_ERROR; //Address not aligned.
        }
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_MemReg_GET(SECU_MEM_TYPE memory_type, SECU_MemAttr_Reg *MemReg, uint32_t *rg_num)
{
    switch (memory_type)
    {
    case SECU_MEM_MPI1:
        MemReg->st_reg = &(hwp_secu1->MPI1_R0_CFG0);
        MemReg->end_reg = &(hwp_secu1->MPI1_R0_CFG1);
        MemReg->rw_reg = &(hwp_secu1->MPI1_RW_CFG);
        *rg_num = SECU_MEM_RG_NUM;
        break;
    case SECU_MEM_MPI2:
        MemReg->st_reg = &(hwp_secu1->MPI2_R0_CFG0);
        MemReg->end_reg = &(hwp_secu1->MPI2_R0_CFG1);
        MemReg->rw_reg = &(hwp_secu1->MPI2_RW_CFG);
        *rg_num = SECU_MEM_RG_NUM;
        break;
    case SECU_MEM_MPI3:
        MemReg->st_reg = &(hwp_secu1->MPI3_R0_CFG0);
        MemReg->end_reg = &(hwp_secu1->MPI3_R0_CFG1);
        MemReg->rw_reg = &(hwp_secu1->MPI3_RW_CFG);
        *rg_num = SECU_MEM_RG_NUM;
        break;
    case SECU_MEM_HPSYS_RAM1:
        MemReg->st_reg = &(hwp_secu1->RAM1_R0_CFG0);
        MemReg->end_reg = &(hwp_secu1->RAM1_R0_CFG1);
        MemReg->rw_reg = &(hwp_secu1->RAM1_RW_CFG);
        *rg_num = SECU_MEM_RG_NUM - 1;
        break;
    case SECU_MEM_HPSYS_RAM2:
        MemReg->st_reg = &(hwp_secu1->RAM2_R0_CFG0);
        MemReg->end_reg = &(hwp_secu1->RAM2_R0_CFG1);
        MemReg->rw_reg = &(hwp_secu1->RAM2_RW_CFG);
        *rg_num = SECU_MEM_RG_NUM - 1;
        break;
    case SECU_MEM_LPSYS_RAM0:
        MemReg->st_reg = &(hwp_secu2->RAM0_R0_CFG0);
        MemReg->end_reg = &(hwp_secu2->RAM0_R0_CFG1);
        MemReg->rw_reg = &(hwp_secu2->RAM0_RW_CFG);
        *rg_num = SECU_MEM_RG_NUM - 1;
        break;
    case SECU_MEM_LPSYS_RAM1:
        MemReg->st_reg = &(hwp_secu2->RAM1_R0_CFG0);
        MemReg->end_reg = &(hwp_secu2->RAM1_R0_CFG1);
        MemReg->rw_reg = &(hwp_secu2->RAM1_RW_CFG);
        *rg_num = SECU_MEM_RG_NUM - 1;
        break;
    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_SetMemoryAttr(SECU_MEM_TYPE memory_type, const SECU_MemAttr_Type *attrs, uint32_t attrs_cnt)
{
    uint32_t i, j, rg_num = 0, flag = 0;
    SECU_MemAttr_Reg MemReg = {0};
    HAL_StatusTypeDef ret = HAL_OK;

    if (HAL_SECU_MemReg_GET(memory_type, &MemReg, &rg_num) != HAL_OK)
    {
        return HAL_ERROR;
    }

    for (i = 0; i < rg_num; i++) //find free mem region
    {
        if (*MemReg.st_reg > *MemReg.end_reg)
            break;
        MemReg.st_reg += 2;
        MemReg.end_reg += 2;
        MemReg.rg_of++;
    }

    if ((i == rg_num) || (attrs_cnt > (rg_num - MemReg.rg_of))) //unfind free mem region
    {
        return HAL_ERROR;
    }

    for (j = 0; j < attrs_cnt; j++)
    {
        if (HAL_SECU_MemAddr_Check(memory_type, attrs[j].start, attrs[j].end) != HAL_OK)
        {
            return HAL_ERROR;
        }
        flag = SECU_FLAG_NONE;
        if (attrs[j].flag & SECU_FLAG_PRIV)
            flag |= (SECU_PRIV_RD_ATTR | SECU_PRIV_WR_ATTR);
        if (attrs[j].flag & SECU_FLAG_SECU)
            flag |= (SECU_SEC_RD_ATTR | SECU_SEC_WR_ATTR);
        if (attrs[j].flag & SECU_FLAG_SEC_WR)
            flag = SECU_SEC_WR_ATTR;

        if (HAL_SECU_MemReg_CFG(memory_type, &MemReg, attrs[j].start, attrs[j].end, flag) != HAL_OK)
        {
            return HAL_ERROR;
        }

        MemReg.st_reg += 2;
        MemReg.end_reg += 2;
        MemReg.rg_of++;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_SetMemoryAttr_RW(SECU_MEM_TYPE memory_type, const SECU_MemAttr_Type *attrs, uint32_t attrs_cnt)
{
    uint32_t i, j, k, rg_num;
    SECU_MemAttr_Reg MemReg = {0};
    HAL_StatusTypeDef ret = HAL_OK;

    if (HAL_SECU_MemReg_GET(memory_type, &MemReg, &rg_num) != HAL_OK)
    {
        return HAL_ERROR;
    }

    for (i = 0; i < rg_num; i++) //find free mem region
    {
        if (*MemReg.st_reg > *MemReg.end_reg)
            break;
        MemReg.st_reg += 2;
        MemReg.end_reg += 2;
        MemReg.rg_of++;
    }

    if ((i == rg_num) || (attrs_cnt > (rg_num - MemReg.rg_of))) //unfind free mem region
    {
        return HAL_ERROR;
    }

    for (j = 0; j < attrs_cnt; j++)
    {
        if (HAL_SECU_MemAddr_Check(memory_type, attrs[j].start, attrs[j].end) != HAL_OK)
        {
            return HAL_ERROR;
        }

        if (HAL_SECU_MemReg_CFG(memory_type, &MemReg, attrs[j].start, attrs[j].end, attrs[j].flag) != HAL_OK)
        {
            return HAL_ERROR;
        }

        MemReg.st_reg += 2;
        MemReg.end_reg += 2;
        MemReg.rg_of++;
    }

    return HAL_OK;
}


HAL_StatusTypeDef HAL_SECU_Apply(SECU_GROUP_TYPE group)
{
    switch (group)
    {
    case SECU_GROUP_HPMST:
        hwp_secu1->SECU_CTRL = SECU1_SECU_CTRL_HPMST_ATTR_UPDATE;
        break;

    case SECU_GROUP_LPMST:
        hwp_secu2->SECU_CTRL = SECU2_SECU_CTRL_LPMST_ATTR_UPDATE;
        break;
    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_ApplyAndLock(SECU_GROUP_TYPE group)
{
    switch (group)
    {
    case SECU_GROUP_HPMST:
        hwp_secu1->SECU_CTRL = SECU1_SECU_CTRL_HPMST_ATTR_UPDATE | SECU1_SECU_CTRL_HPMST_LOCK;
        break;
    case SECU_GROUP_HPSLV:
        hwp_secu1->SECU_CTRL = SECU1_SECU_CTRL_HPSLV_LOCK;
        break;
    case SECU_GROUP_HPMPI1:
        hwp_secu1->SECU_CTRL = SECU1_SECU_CTRL_MPI1_LOCK;
        break;
    case SECU_GROUP_HPMPI2:
        hwp_secu1->SECU_CTRL = SECU1_SECU_CTRL_MPI2_LOCK;
        break;
    case SECU_GROUP_HPMPI3:
        hwp_secu1->SECU_CTRL = SECU1_SECU_CTRL_MPI3_LOCK;
        break;
    case SECU_GROUP_HPRAM:
        hwp_secu1->SECU_CTRL = SECU1_SECU_CTRL_RAM_LOCK;
        break;

    case SECU_GROUP_LPMST:
        hwp_secu2->SECU_CTRL = SECU2_SECU_CTRL_LPMST_ATTR_UPDATE | SECU2_SECU_CTRL_LPMST_LOCK;
        break;
    case SECU_GROUP_LPSLV:
        hwp_secu2->SECU_CTRL = SECU2_SECU_CTRL_LPSLV_LOCK;
        break;
    case SECU_GROUP_LPRAM:
        hwp_secu2->SECU_CTRL = SECU2_SECU_CTRL_RAM_LOCK;
        break;
    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_KeySet(SECU_KEY_TYPE key_type, uint32_t *key)
{
    volatile uint32_t *key_reg = NULL;

    HAL_ASSERT(key != NULL);

    if (key_type == SECU_KEY0)
    {
        key_reg = &(hwp_secu1->KEY0_W0);
    }
    else if (key_type == SECU_KEY1)
    {
        key_reg = &(hwp_secu1->KEY1_W0);
    }
    else
    {
        return HAL_ERROR;
    }

    for (int i = 0; i < 8; i++) //key = 32byte
    {
        *key_reg = *key;
        key_reg++;
        key++;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_MpiKey_Select(SECU_MPIKEY_TYPE mpi_key, SECU_KEY_TYPE key_type)
{
    volatile uint32_t *key_reg = &(hwp_secu1->KEY_CFG);
    uint32_t key_cfg = 0, bit = 0;

    if ((key_type != SECU_KEY0)
            && (key_type != SECU_KEY1)
            && (key_type != SECU_KEY_NULL))
    {
        return HAL_ERROR;
    }

    switch (mpi_key)
    {
    case SECU_MPI2_KEY0:
        bit = SECU1_KEY_CFG_MPI2_KEY0_SEL_Pos;
        break;
    case SECU_MPI2_KEY1:
        bit = SECU1_KEY_CFG_MPI2_KEY1_SEL_Pos;
        break;
    case SECU_MPI3_KEY0:
        bit = SECU1_KEY_CFG_MPI3_KEY0_SEL_Pos;
        break;
    case SECU_MPI3_KEY1:
        bit = SECU1_KEY_CFG_MPI3_KEY1_SEL_Pos;
        break;
    default:
        return HAL_ERROR;
    }
    key_cfg = *key_reg;
    key_cfg &= ~(0x3UL << bit);
    key_cfg |= (key_type << bit);
    *key_reg = key_cfg;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_RootKey_AesLock(SECU_ROOTKEY_AES_TYPE lock)
{
    volatile uint32_t *key_reg = &(hwp_secu1->KEY_CFG);
    uint32_t key_cfg = 0;

    key_cfg = *key_reg;

    if (lock == SECU_ROOTKEY_UNLK)
    {
        key_cfg &= ~SECU1_KEY_CFG_AES_ROOT_KEY_LOCK;
    }
    else if (lock == SECU_ROOTKEY_LK)
    {
        key_cfg |= SECU1_KEY_CFG_AES_ROOT_KEY_LOCK;
    }
    else
    {
        return HAL_ERROR;
    }

    *key_reg = key_cfg;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_SECU_NMI_ClearDelay(uint32_t delay)
{
    volatile uint32_t *nmi_reg = &(hwp_secu1->NMI_INT);
    uint32_t nmi_int = 0;

    if (delay > 0x7UL)
    {
        return HAL_ERROR;
    }

    nmi_int |= (delay << SECU1_NMI_INT_CLR_DLY_Pos);
    *nmi_reg = nmi_int;

    return HAL_OK;
}

void HAL_SECU_NMI_TriggerEN(void)
{
    hwp_secu1->NMI_INT |= SECU1_NMI_INT_ENABLE;
}

void HAL_SECU_NMI_TriggerDIS(void)
{
    hwp_secu1->NMI_INT &= ~SECU1_NMI_INT_ENABLE;
}

HAL_StatusTypeDef HAL_SECU_ACPU_VTOR(uint32_t offset)
{
    volatile uint32_t *acpu_reg = &(hwp_secu1->ACPU);
    uint32_t acpu = 0;

    if ((offset & 0x1FFFFFFUL) != offset)
    {
        return HAL_ERROR;
    }

    acpu = *acpu_reg;
    acpu &= ~SECU1_ACPU_VTOR;
    acpu |= (offset << SECU1_ACPU_VTOR_Pos);
    *acpu_reg = acpu;

    return HAL_OK;
}

/*acpu clock enable*/
void HAL_SECU_ACPU_CLKEN(void)
{
    hwp_secu1->ACPU |= SECU1_ACPU_ENR;
}

void HAL_SECU_ACPU_CLKDIS(void)
{
    hwp_secu1->ACPU &= ~SECU1_ACPU_ENR;
}

void HAL_SECU_ACPU_RSTR(void)
{
    hwp_secu1->ACPU |= SECU1_ACPU_RSTR;
}

void HAL_SECU_ACPU_VTOR_LOCK(void)
{
    hwp_secu1->ACPU |= SECU1_ACPU_LOCKNSVTOR;
}

void HAL_SECU_ACPU_LAUNCH(void)
{
    hwp_secu1->ACPU &= ~SECU1_ACPU_WAIT;
}

HAL_StatusTypeDef HAL_SECU_HCPU_LOCK(SECU_HCPU_LOCK_TYPE lock_type)
{
    switch (lock_type)
    {
    case SECU_LOCK_TCM:
        hwp_secu1->ACPU |= SECU1_HCPU_LOCKTCM;
        break;
    case SECU_LOCK_SAU:
        hwp_secu1->ACPU |= SECU1_HCPU_LOCKSAU;
        break;
    case SECU_LOCK_NSMPU:
        hwp_secu1->ACPU |= SECU1_HCPU_LOCKNSMPU;
        break;
    case SECU_LOCK_SMPU:
        hwp_secu1->ACPU |= SECU1_HCPU_LOCKSMPU;
        break;
    case SECU_LOCK_NSVTOR:
        hwp_secu1->ACPU |= SECU1_HCPU_LOCKNVSVTOR;
        break;
    case SECU_LOCK_SVTOR:
        hwp_secu1->ACPU |= SECU1_HCPU_LOCKSVTAIRCR;
        break;
    default:
        return HAL_ERROR;
    }

    return HAL_OK;
}

#endif /* HAL_SECU_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sifli Technology *******END OF FILE****/
