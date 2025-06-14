/**
  ******************************************************************************
  * @file   bf0_hal_riscv.c
  * @author Sifli software development team
  * @brief   RISCV HAL module driver.
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

/** @addtogroup CORTEX
  * @{
  */


//#if defined(HAL_RISCV_MODULE_ENABLED)||defined(_SIFLI_DOXYGEN_)

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup CORTEX_Exported_Functions
  * @{
  */


/** @addtogroup CORTEX_Exported_Functions_Group1
 *  @brief    Initialization and Configuration functions
 *
@verbatim
  ==============================================================================
              ##### Initialization and Configuration functions #####
  ==============================================================================
    [..]
      This section provides the CORTEX HAL driver functions allowing to configure Interrupts
      SysTick functionalities

@endverbatim
  * @{
  */


/**
  * @brief  Set the priority grouping field (pre-emption priority and subpriority)
  *         using the required unlock sequence.
  * @param  PriorityGroup: The priority grouping bits length.
  *         This parameter can be one of the following values:
  *         @arg NVIC_PRIORITYGROUP_0: 0 bit  for pre-emption priority,
  *                                    4 bits for subpriority
  *         @arg NVIC_PRIORITYGROUP_1: 1 bit  for pre-emption priority,
  *                                    3 bits for subpriority
  *         @arg NVIC_PRIORITYGROUP_2: 2 bits for pre-emption priority,
  *                                    2 bits for subpriority
  *         @arg NVIC_PRIORITYGROUP_3: 3 bits for pre-emption priority,
  *                                    1 bit  for subpriority
  *         @arg NVIC_PRIORITYGROUP_4: 4 bits for pre-emption priority,
  *                                    0 bit  for subpriority
  * @note   When the NVIC_PriorityGroup_0 is selected, IRQ pre-emption is no more possible.
  *         The pending IRQ priority will be managed only by the subpriority.
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
#if 0
    /* Check the parameters */
    HAL_ASSERT(IS_NVIC_PRIORITY_GROUP(PriorityGroup));

    /* Set the PRIGROUP[10:8] bits according to the PriorityGroup parameter value */
    NVIC_SetPriorityGrouping(PriorityGroup);
#endif
}

/**
  * @brief  Set the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @param  PreemptPriority: The pre-emption priority for the IRQn channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority
  * @param  SubPriority: the subpriority level for the IRQ channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority.
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t Priority, uint32_t level)
{
    uint32_t prioritygroup = 0x00;

    /* Check the parameters */
    //HAL_ASSERT(IS_NVIC_SUB_PRIORITY(SubPriority));
    //HAL_ASSERT(IS_NVIC_PREEMPTION_PRIORITY(PreemptPriority));

    ECLIC_SetPriorityIRQ(IRQn, Priority);

    ECLIC_SetLevelIRQ(IRQn, level);
}

/**
  * @brief  Enable a device specific interrupt in the NVIC interrupt controller.
  * @note   To configure interrupts priority correctly, the NVIC_PriorityGroupConfig()
  *         function should be called before.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    //HAL_ASSERT(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Enable interrupt */
    ECLIC_EnableIRQ(IRQn);

}

/**
  * @brief  Disable a device specific interrupt in the NVIC interrupt controller.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_DisableIRQ(IRQn_Type IRQn)
{
    /* Check the parameters */
    //HAL_ASSERT(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Disable interrupt */
    ECLIC_DisableIRQ(IRQn);
}

/**
  * @brief  Initiate a system reset request to reset the MCU.
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_SystemReset(void)
{
#if 0
    /* System Reset */
    NVIC_SystemReset();
#endif
}

/**
  * @brief  Initialize the System Timer with interrupt enabled and start the System Tick Timer (SysTick):
  *         Counter is in free running mode to generate periodic interrupts.
  * @param  TicksNumb: Specifies the ticks Number of ticks between two interrupts.
  * @retval status:  - 0  Function succeeded.
  *                  - 1  Function failed.
  */
__HAL_ROM_USED uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb)
{
    SysTimer_Set_MTIMECTRL(0, 0, 0);
    //SysTimer_AutoStart();
    return SysTick_Config(TicksNumb);
}
/**
  * @}
  */

/** @addtogroup CORTEX_Exported_Functions_Group2
 *  @brief   Cortex control functions
 *
@verbatim
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================
    [..]
      This subsection provides a set of functions allowing to control the CORTEX
      (NVIC, SYSTICK, MPU) functionalities.


@endverbatim
  * @{
  */

/**
  * @brief  Get the priority grouping field from the NVIC Interrupt Controller.
  * @retval Priority grouping field (SCB->AIRCR [10:8] PRIGROUP field)
  */
__HAL_ROM_USED uint32_t HAL_NVIC_GetPriorityGrouping(void)
{
#if 0
    /* Get the PRIGROUP[10:8] field value */
    return NVIC_GetPriorityGrouping();
#else
    return 0;
#endif
}

/**
  * @brief  Get the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @param   PriorityGroup: the priority grouping bits length.
  *         This parameter can be one of the following values:
  *           @arg NVIC_PRIORITYGROUP_0: 0 bit for pre-emption priority,
  *                                      4 bits for subpriority
  *           @arg NVIC_PRIORITYGROUP_1: 1 bit for pre-emption priority,
  *                                      3 bits for subpriority
  *           @arg NVIC_PRIORITYGROUP_2: 2 bits for pre-emption priority,
  *                                      2 bits for subpriority
  *           @arg NVIC_PRIORITYGROUP_3: 3 bits for pre-emption priority,
  *                                      1 bit for subpriority
  *           @arg NVIC_PRIORITYGROUP_4: 4 bits for pre-emption priority,
  *                                      0 bit for subpriority
  * @param  pPreemptPriority: Pointer on the Preemptive priority value (starting from 0).
  * @param  pSubPriority: Pointer on the Subpriority value (starting from 0).
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_GetPriority(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t *pPreemptPriority, uint32_t *pSubPriority)
{
    /* Check the parameters */
    HAL_ASSERT(IS_NVIC_PRIORITY_GROUP(PriorityGroup));
#if 0
    /* Get priority for Cortex-M system or device specific interrupts */
    NVIC_DecodePriority(NVIC_GetPriority(IRQn), PriorityGroup, pPreemptPriority, pSubPriority);
#endif
}

/**
  * @brief  Set Pending bit of an external interrupt.
  * @param  IRQn External interrupt number
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
#if 0

    /* Check the parameters */
    HAL_ASSERT(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Set interrupt pending */
    NVIC_SetPendingIRQ(IRQn);
#endif
}

/**
  * @brief  Get Pending Interrupt (read the pending register in the NVIC
  *         and return the pending bit for the specified interrupt).
  * @param  IRQn External interrupt number.
  *          This parameter can be an enumerator of IRQn_Type enumeration
  * @retval status: - 0  Interrupt status is not pending.
  *                 - 1  Interrupt status is pending.
  */
__HAL_ROM_USED uint32_t HAL_NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
#if 0

    /* Check the parameters */
    HAL_ASSERT(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Return 1 if pending else 0 */
    return NVIC_GetPendingIRQ(IRQn);
#else
    return 0;
#endif
}

/**
  * @brief  Clear the pending bit of an external interrupt.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval None
  */
__HAL_ROM_USED void HAL_NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
#if 0

    /* Check the parameters */
    HAL_ASSERT(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Clear pending interrupt */
    NVIC_ClearPendingIRQ(IRQn);
#endif
}

/**
  * @brief Get active interrupt (read the active register in NVIC and return the active bit).
  * @param IRQn External interrupt number
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval status: - 0  Interrupt status is not actived.
  *                 - 1  Interrupt status is actived.
  */
__HAL_ROM_USED uint32_t HAL_NVIC_GetActive(IRQn_Type IRQn)
{
#if 0

    /* Check the parameters */
    HAL_ASSERT(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Return 1 if active else 0 */
    return NVIC_GetActive(IRQn);
#endif
}

/**
  * @brief Get enable interrupt (read the enable register in NVIC and return the enable bit).
  * @param IRQn External interrupt number
  *         This parameter can be an enumerator of IRQn_Type enumeration
  * @retval status: - 0  Interrupt is not enabled.
  *                 - 1  Interrupt is enabled.
  */
__HAL_ROM_USED uint32_t HAL_NVIC_GetEnableIRQ(IRQn_Type IRQn)
{
#if 0

    /* Check the parameters */
    HAL_ASSERT(IS_NVIC_DEVICE_IRQ(IRQn));

    /* Return 1 if enable else 0 */
    return NVIC_GetEnableIRQ(IRQn);
#endif
}

/**
  * @brief  Configure the SysTick clock source.
  * @param  CLKSource: specifies the SysTick clock source.
  *          This parameter can be one of the following values:
  *             @arg SYSTICK_CLKSOURCE_HCLK_DIV8: AHB clock divided by 8 selected as SysTick clock source.
  *             @arg SYSTICK_CLKSOURCE_HCLK: AHB clock selected as SysTick clock source.
  * @retval None
  */
__HAL_ROM_USED void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource)
{
#if 0

    /* Check the parameters */
    HAL_ASSERT(IS_SYSTICK_CLK_SOURCE(CLKSource));
    if (CLKSource == SYSTICK_CLKSOURCE_HCLK)
    {
        SysTick->CTRL |= SYSTICK_CLKSOURCE_HCLK;
    }
    else
    {
        SysTick->CTRL &= ~SYSTICK_CLKSOURCE_HCLK;
    }
#endif
}

/**
  * @brief  Handle SYSTICK interrupt request.
  * @retval None
  */
__HAL_ROM_USED void HAL_SYSTICK_IRQHandler(void)
{
#if 0
    HAL_SYSTICK_Callback();
#endif
}

/**
  * @brief  SYSTICK callback.
  * @retval None
  */
__weak void HAL_SYSTICK_Callback(void)
{
    /* NOTE : This function should not be modified, when the callback is needed,
              the HAL_SYSTICK_Callback could be implemented in the user file
     */
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sifli Technology *******END OF FILE****/
