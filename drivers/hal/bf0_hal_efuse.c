/*
 * SPDX-FileCopyrightText: 2019-2025 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bf0_hal.h"
#include "pmu_ldo_update.h"
/** @addtogroup BF0_HAL_Driver
  * @{
  */

/** @defgroup EFUSE EFUSE
  * @brief EFUSE HAL module driver
  * @{
  */

#if (defined(HAL_EFUSE_MODULE_ENABLED))||defined(_SIFLI_DOXYGEN_)

#define EFUSE_PGM_THPCK_NS (20)
#define EFUSE_PGM_TCKHP_US  (10)
#define EFUSE_RD_TIM_NS    (500)
#define EFUSE_PCLK_LIMIT   (120000000)


HAL_StatusTypeDef HAL_EFUSE_Init(void)
{
#ifdef EFUSEC_TIMR_THRCK
    uint32_t pclk;
    uint32_t pgm_tckhp;
    uint32_t pgm_thpck;
    uint32_t rd_thrck;
    uint32_t pgm_tckhp_ns;

    pclk = HAL_RCC_GetPCLKFreq(CORE_ID_HCPU, true);
    if (pclk > EFUSE_PCLK_LIMIT)
    {
        return HAL_ERROR;
    }

    pgm_thpck = (uint64_t)EFUSE_PGM_THPCK_NS * pclk / (1000 * 1000000) + 1;
    pgm_thpck = MAKE_REG_VAL(pgm_thpck, EFUSEC_TIMR_THPCK_Msk, EFUSEC_TIMR_THPCK_Pos);
    if (pgm_thpck > EFUSEC_TIMR_THPCK)
    {
        return HAL_ERROR;
    }

    pgm_tckhp = ((uint64_t)EFUSE_PGM_TCKHP_US * pclk + 500000) / 1000000;
    pgm_tckhp_ns = (uint64_t)pgm_tckhp * 1000000 * 1000 / pclk;
    if (pgm_tckhp_ns > 11000)
    {
        pgm_tckhp -= 1;
    }
    else if (pgm_tckhp_ns < 9000)
    {
        pgm_tckhp += 1;
    }
    pgm_tckhp = MAKE_REG_VAL(pgm_tckhp, EFUSEC_TIMR_TCKHP_Msk, EFUSEC_TIMR_TCKHP_Pos);
    if (pgm_tckhp > EFUSEC_TIMR_TCKHP)
    {
        return HAL_ERROR;
    }

    rd_thrck = (uint64_t)EFUSE_RD_TIM_NS * pclk / (1000 * 1000000) + 1;
    rd_thrck = MAKE_REG_VAL(rd_thrck, EFUSEC_TIMR_THRCK_Msk, EFUSEC_TIMR_THRCK_Pos);
    if (rd_thrck > EFUSEC_TIMR_THRCK)
    {
        return HAL_ERROR;
    }

    hwp_efusec->TIMR = pgm_thpck | pgm_tckhp | rd_thrck;
#endif
    return HAL_OK;
}

void HAL_EFUSE_ConfigBypass(bool enabled)
{
    HAL_ASSERT(0);
}


int32_t HAL_EFUSE_Write(uint16_t bit_offset, uint8_t *data, int32_t size)
{
    uint32_t ready = 0;
    int byte_off = (bit_offset >> 3) % HAL_EFUSE_BANK_SIZE;
    int bank = (bit_offset >> 8);
    volatile uint32_t *pg_reg = (volatile uint32_t *) & (hwp_efusec->PGM_DATA0);
    uint32_t timeout;
    uint32_t i;
    uint32_t word_size;

    // Should be less than a bank, and do not accross bank
    if ((size > HAL_EFUSE_BANK_SIZE)
            || ((byte_off + size) > HAL_EFUSE_BANK_SIZE)
            || (size & 3)  //multiple of 4
            || (bit_offset & 31)) //offset is 32bits aligned
    {
        return 0;
    }

    /* clear PGM_DATA to avoid programming unexpected bit */
    for (i = 0; i < 8; i++)
    {
        pg_reg[i] = 0;
    }

    /* Change HPSYS LDO VERF */
    uint32_t org = pmu_ldo_inc(3);

#ifdef HPSYS_CFG_ANAU_CR_EFUSE_VDD_EN
    MODIFY_REG(hwp_hpsys_cfg->ANAU_CR, HPSYS_CFG_ANAU_CR_EFUSE_VDD_PD, HPSYS_CFG_ANAU_CR_EFUSE_VDD_EN);
#else
    /* Enable BGR if it has not been enabled,
       it may have been enabled by other module,
       don't disable it as other module may need it */
    hwp_tsen->BGR |= TSEN_BGR_EN;
    /* delay 50us, then enable LDO */
    HAL_Delay_us(50);
#endif
#ifdef EFUSEC_ANACR_LDO_EN
    hwp_efusec->ANACR |= EFUSEC_ANACR_LDO_EN;
#endif
    /* delay 50us, then starting programming  */
    HAL_Delay_us(50);

    /* select bank and enable PGM mode */
    hwp_efusec->CR = (bank << EFUSEC_CR_BANKSEL_Pos) | (1 << EFUSEC_CR_MODE_Pos);

    pg_reg += (byte_off >> 2);
    word_size = size >> 2;
    for (i = 0; i < word_size; i++)
    {
        pg_reg[i] = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        data += 4;
    }
    /* start program */
    hwp_efusec->CR |= EFUSEC_CR_EN;
    /* minimum: 10us one bit
     * here: 10ms one bit for 48MHz clock
     */
    timeout = (uint32_t)size * 8 * 480000;
    while (((hwp_efusec->SR & EFUSEC_SR_DONE) == 0) && (ready < timeout))
        ready++;
    hwp_efusec->SR |= EFUSEC_SR_DONE;

    if (ready >= timeout)
    {
        size = 0;
    }

#ifdef EFUSEC_ANACR_LDO_EN
    hwp_efusec->ANACR &= ~EFUSEC_ANACR_LDO_EN;
#endif

    pmu_ldo_recover(org);

#ifdef HPSYS_CFG_ANAU_CR_EFUSE_VDD_EN
    MODIFY_REG(hwp_hpsys_cfg->ANAU_CR, HPSYS_CFG_ANAU_CR_EFUSE_VDD_EN, HPSYS_CFG_ANAU_CR_EFUSE_VDD_PD);
#endif
    return size;
}

int32_t HAL_EFUSE_Read(uint16_t bit_offset, uint8_t *data, int size)
{
    uint32_t ready = 0;
    int byte_off = (bit_offset >> 3) % HAL_EFUSE_BANK_SIZE;
    int bank = (bit_offset >> 8);
    uint32_t timeout;
    volatile uint32_t *rd_reg = (volatile uint32_t *) & (hwp_efusec->BANK0_DATA0);
    uint32_t i;
    uint32_t word_size;
    uint32_t val;

    if ((size > HAL_EFUSE_BANK_SIZE)
            || ((byte_off + size) > HAL_EFUSE_BANK_SIZE) // Should be less than a bank, and do not accross bank
            || (size & 3)  //multiple of 4
            || (bit_offset & 31)) //offset is 32bits aligned
    {
        return 0;
    }

    uint32_t org = pmu_ldo_inc(3);

    //HAL_Delay_us(20);
    /* select bank and enable READ mode */
    hwp_efusec->CR = (bank << EFUSEC_CR_BANKSEL_Pos);
    /* start read */
    hwp_efusec->CR |= EFUSEC_CR_EN;
    /* mininum: two cycle one bit
     * here: 1ms one bit for 48MHz clock
     */
    timeout = (uint32_t)size * 8 * 48000;
    while (((hwp_efusec->SR & EFUSEC_SR_DONE) == 0) && (ready < timeout))
        ready++;

    hwp_efusec->SR |= EFUSEC_SR_DONE;

    if (ready >= timeout)
    {
        pmu_ldo_recover(org);
        return 0;
    }
    rd_reg += (bank << 3);  // Each bank has 8 registers.
    rd_reg += (byte_off >> 2);
    word_size = size >> 2;
    for (i = 0; i < word_size; i++)
    {
        val = rd_reg[i];
        data[0] = val & 0xFF;
        data[1] = (val >> 8) & 0xFF;
        data[2] = (val >> 16) & 0xFF;
        data[3] = (val >> 24) & 0xFF;
        data += 4;
    }

    pmu_ldo_recover(org);

    return size;
}

#endif /* HAL_EFUSE_MODULE_ENABLED */


/**
  * @}
  */

/**
  * @}
  */