/*
 * SPDX-FileCopyrightText: 2019-2025 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bf0_hal.h"

/** @addtogroup BF0_HAL_Driver
  * @{
  */

/** @defgroup AON AON
  * @brief AON HAL module driver
  * @{
  */

#ifdef HAL_AON_MODULE_ENABLED

__HAL_ROM_USED int8_t HAL_HPAON_QueryWakeupPin(GPIO_TypeDef *gpio, uint16_t gpio_pin)
{
    return -1;
}

__HAL_ROM_USED GPIO_TypeDef *HAL_HPAON_QueryWakeupGpioPin(uint8_t wakeup_pin, uint16_t *gpio_pin)
{
    return NULL;
}

HAL_StatusTypeDef HAL_HPAON_GetWakeupPinMode(uint8_t wakeup_pin, AON_PinModeTypeDef *mode)
{
    return HAL_ERROR;
}


#endif /* HAL_AON_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */