/*
 * SPDX-FileCopyrightText: 2019-2025 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rtconfig.h"
#include "string.h"
#include "bf0_hal.h"
#include "math.h"


/** @addtogroup BF0_HAL_Driver
  * @{
  */

/** @defgroup PMU PMU
  * @brief PMU HAL module driver
  * @{
  */

#ifdef HAL_PMU_MODULE_ENABLED

//TODO:
HAL_StatusTypeDef HAL_PMU_EnablePinWakeup2(pin_pad pad, uint8_t mode)
{
    return HAL_ERROR;

}

HAL_StatusTypeDef HAL_PMU_DisablePinWakeup2(pin_pad pin)
{
    return HAL_ERROR;
}

HAL_RAM_RET_CODE_SECT(HAL_PMU_ConfigPeriLdo, HAL_StatusTypeDef HAL_PMU_ConfigPeriLdo(PMU_PeriLdoTypeDef ldo, bool en, bool wait))
{
    uint32_t mask;
    uint32_t val;

    if ((PMU_PERI_LDO_1V8 != ldo)
            && (PMU_PERI_LDO2_3V3 != ldo)
            && (PMU_PERI_LDO3_3V3 != ldo))
    {
        return HAL_ERROR;
    }

    /* in assumption that they have same relative offset */
    mask = ((PMUC_PERI_LDO_EN_LDO18_Msk << (ldo - PMU_PERI_LDO_1V8))
            | (PMUC_PERI_LDO_LDO18_PD_VOUT_Msk << (ldo - PMU_PERI_LDO_1V8)));

    if (en)
    {
        val = PMUC_PERI_LDO_EN_LDO18_Msk;
    }
    else
    {
        val = PMUC_PERI_LDO_LDO18_PD_VOUT_Msk;
    }

    val = val << (ldo - PMU_PERI_LDO_1V8);

    MODIFY_REG(hwp_pmuc->PERI_LDO, mask, val);

    if (wait)
    {
        HAL_Delay_us(5000);
    }

    return HAL_OK;
}

#endif /* HAL_PMU_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */
