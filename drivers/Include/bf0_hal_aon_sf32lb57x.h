/*
 * SPDX-FileCopyrightText: 2019-2025 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BF0_HAL_AON_SF32LB57_H
#define __BF0_HAL_AON_SF32LB57_H

#ifdef __cplusplus
extern "C" {
#endif


/** @brief hpsys wakeup source */
typedef enum
{
    /* First part, keep enum value same as macro definition to simplify implementation */
    HPAON_WAKEUP_SRC_RTC       = HPSYS_AON_WER_RTC_Pos,   /**< RTC wakeup source */
    HPAON_WAKEUP_SRC_LPTIM1    = HPSYS_AON_WER_LPTIM1_Pos,    /**< LPTIM1 wakeup source */
    HPAON_WAKEUP_SRC_LP2HP_REQ = HPSYS_AON_WER_LP2HP_REQ_Pos,  /**< LP2HP manual wakeup source */
    HPAON_WAKEUP_SRC_LP2HP_IRQ = HPSYS_AON_WER_LP2HP_IRQ_Pos,  /**< LP2HP mailbox interrupt wakeup source */
    HPAON_WAKEUP_SRC_GPIO1     = HPSYS_AON_WER_GPIO1_Pos,    /**< GPIO1 wakeup source */
    HPAON_WAKEUP_SRC_PMUC      = HPSYS_AON_WER_PMUC_Pos,     /**< PMUC wakeup source */
    HPAON_WAKEUP_SRC_PIN       = HPSYS_AON_WER_PIN_Pos,     /**< PMUC wakeup source */
} HPAON_WakeupSrcTypeDef;

/** @brief lpsys wakeup source */
typedef enum
{
    /* First part, keep enum value same as macro definition to simplify implementation */
    LPAON_WAKEUP_SRC_HP2LP_REQ = LPSYS_AON_WER_HP2LP_REQ_Pos, /**< HP2LP manual wakeup source */
    LPAON_WAKEUP_SRC_HP2LP_IRQ = LPSYS_AON_WER_HP2LP_IRQ_Pos, /**< HP2LP mailbox interrupt wakeup source */
    LPAON_WAKEUP_SRC_LPTIM3    = LPSYS_AON_WER_LPTIM3_Pos,   /**< LPTIM2 wakeup source */
    LPAON_WAKEUP_SRC_BT        = LPSYS_AON_WER_BT_Pos,       /**< BT wakeup source */
} LPAON_WakeupSrcTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* __BF0_HAL_AON_SF32LB57_H */