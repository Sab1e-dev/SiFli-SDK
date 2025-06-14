/**
  ******************************************************************************
  * @file   bsp_init.c
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
#include "bf0_hal_rtc.h"

#ifndef LXT_LP_CYCLE
    #define LXT_LP_CYCLE 200
#endif

#ifdef SOC_BF0_HCPU
static uint16_t mpi1_div = 1;
static uint16_t mpi2_div = 1;
static uint16_t mpi3_div = 1;


static uint32_t otp_flash_addr = AUTO_FLASH_MAC_ADDRESS;

#define FUNC_BSP_FLASH_DIV_GET(i) \
uint16_t BSP_GetFlash##i##DIV(void) \
{ \
    return mpi##i##_div; \
}\

#define FUNC_BSP_FLASH_DIV_SET(i) \
void BSP_SetFlash##i##DIV(uint16_t div) \
{ \
    mpi##i##_div = div; \
}\

FUNC_BSP_FLASH_DIV_GET(1);
FUNC_BSP_FLASH_DIV_GET(2);
FUNC_BSP_FLASH_DIV_GET(3);

FUNC_BSP_FLASH_DIV_SET(1)
FUNC_BSP_FLASH_DIV_SET(2)
FUNC_BSP_FLASH_DIV_SET(3)


int rt_psram_init(void);
int rt_hw_flash1_init(uint8_t auto_detect);
int rt_hw_flash2_init(uint8_t auto_detect);
int rt_hw_flash_init(void);


uint32_t BSP_GetOtpBase(void)
{
    return otp_flash_addr;
}
#endif

#ifdef SOC_BF0_HCPU
#define HXT_DELAY_EXP_VAL 1000
static void LRC_init(void)
{
    HAL_RC_CAL_update_reference_cycle_on_48M(LXT_LP_CYCLE);
    uint32_t ref_cnt = HAL_RC_CAL_get_reference_cycle_on_48M();
    uint32_t cycle_t = (uint32_t)ref_cnt / (48 * LXT_LP_CYCLE);

    HAL_PMU_SET_HXT3_RDY_DELAY((HXT_DELAY_EXP_VAL / cycle_t + 1));
}
#endif

void HAL_PreInit(void)
{
#ifdef SOC_BF0_HCPU

    // __asm("B .");
    /* not switch back to XT48 if other clock source has been selected already */
    if (RCC_SYSCLK_HRC48 == HAL_RCC_HCPU_GetClockSrc(RCC_CLK_MOD_SYS))
    {
        // To avoid somebody cancel request.
        HAL_HPAON_EnableXT48();
        HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_SYS, RCC_SYSCLK_HXT48);
    }

    HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_HP_PERI, RCC_CLK_PERI_HXT48);

    if (PM_STANDBY_BOOT != SystemPowerOnModeGet())
    {
        // Halt LCPU first to avoid LCPU in running state
        HAL_HPAON_WakeCore(CORE_ID_LCPU);
        HAL_RCC_Reset_and_Halt_LCPU(1);
#ifndef USE_ATE_MODE
        // get system configure from EFUSE
        BSP_System_Config();
#endif
        HAL_HPAON_StartGTimer();
        HAL_PMU_EnableRC32K(1);
        HAL_PMU_LpCLockSelect(PMU_LPCLK_RC32);

#ifndef LXT_DISABLE
        HAL_PMU_EnableXTAL32();
        if (HAL_PMU_LXTReady() != HAL_OK)
            HAL_ASSERT(0);
        // RTC/GTIME/LPTIME Using same low power clock source
        HAL_RTC_ENABLE_LXT();
#endif

#ifndef CFG_BOOTLOADER
        {
#ifdef LXT_DISABLE
            uint8_t is_enable_lxt = 0;
#else
            uint8_t is_enable_lxt = 1;
#endif
            uint32_t wdt_staus = 0xFF;//1
            uint32_t wdt_time = 0;
            uint16_t wdt_clk = 32768;
            HAL_LCPU_CONFIG_set(HAL_LCPU_CONFIG_XTAL_ENABLED, &is_enable_lxt, 1);
            HAL_LCPU_CONFIG_set(HAL_LCPU_CONFIG_WDT_STATUS, &wdt_staus, 4);
            HAL_LCPU_CONFIG_set(HAL_LCPU_CONFIG_WDT_TIME, &wdt_time, 4);
            HAL_LCPU_CONFIG_set(HAL_LCPU_CONFIG_WDT_CLK_FEQ, &wdt_clk, 2);
            HAL_PMU_SetWdt((uint32_t)hwp_wdt2);   // Add reboot cause for watchdog2
        }

#endif/* CFG_BOOTLOADER */
        HAL_RCC_LCPU_ClockSelect(RCC_CLK_MOD_LP_PERI, RCC_CLK_PERI_HXT48);

        HAL_HPAON_CANCEL_LP_ACTIVE_REQUEST();
        if (HAL_LXT_DISABLED())
            LRC_init();
    }


    // Reset sysclk used by HAL_Delay_us
    HAL_Delay_us(0);
    //HAL_sw_breakpoint();

    mpi1_div = 1;
    mpi2_div = 2;
    mpi3_div = 2;

    /* Init the low level hardware */
    HAL_MspInit();
#if defined (BSP_USING_PSRAM)
    rt_psram_init();
#endif

#if defined(BSP_USING_NOR_FLASH1) || defined(BSP_USING_NOR_FLASH2) || defined(BSP_USING_NOR_FLASH3)
    // TODO: move select FLASH1 clock to here if MPI1 used as FLASH.
    if (PM_STANDBY_BOOT == SystemPowerOnModeGet())
    {
        //TODO: pin device is not restored
        HAL_HPAON_ENABLE_PAD();
        /* rt_hw_flash_init cannot be called as data has not been restored at this moment,
           so rt_sem_init cannot be called */
#if defined(BSP_USING_NOR_FLASH1)
        BSP_Flash_hw1_init();
#endif
#if defined(BSP_USING_NOR_FLASH2)
        BSP_Flash_hw2_init();
#endif
    }
    else
    {
        rt_hw_flash_init();
    }
#endif /* BSP_USING_NOR_FLASH3 */


    /*
      LPSYS:
            hclk is fixed 48MHz, pclk1 should <=24MHz
    */
    HAL_RCC_HCPU_SetDiv(1, 2, 5);

#elif defined(SOC_BF0_LCPU)
    HAL_LPAON_EnableXT48();
    HAL_RCC_LCPU_ClockSelect(RCC_CLK_MOD_SYS, RCC_SYSCLK_HXT48);
    HAL_RCC_LCPU_ClockSelect(RCC_CLK_MOD_LP_PERI, RCC_CLK_PERI_HXT48);
    HAL_RCC_LCPU_SetDiv(2, 1, 3);
    HAL_MspInit();
#endif
}

extern void BSP_PIN_Init(void);
// Called in HAL_MspInit
void BSP_IO_Init(void)
{
    BSP_PIN_Init();
    BSP_Power_Up(true);
}


__WEAK void SystemClock_Config(void)
{

}



/************************ (C) COPYRIGHT Sifli Technology *******END OF FILE****/
