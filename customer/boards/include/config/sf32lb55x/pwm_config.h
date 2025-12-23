/*
 * SPDX-FileCopyrightText: 2019-2022 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __PWM_CONFIG_H__
#define __PWM_CONFIG_H__

#include <rtconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PWMT1_CORE CORE_ID_HCPU
#define PWMT2_CORE CORE_ID_HCPU
#define PWMT3_CORE CORE_ID_LCPU
#define PWMT4_CORE CORE_ID_LCPU
#define PWMT5_CORE CORE_ID_LCPU

#ifdef BSP_USING_PWMT1
#define PWMT1_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = GPTIM1,       \
       .tim_handle.core         = PWMT1_CORE,    \
       .name                    = "pwmt1",       \
       .channel                 = 0             \
    }
#endif /* BSP_USING_PWMT1 */

#ifdef BSP_USING_PWMT2
#define PWMT2_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = GPTIM2,         \
       .tim_handle.core         = PWMT2_CORE,    \
       .name                    = "pwmt2",       \
       .channel                 = 0             \
    }
#endif /* BSP_USING_PWMT2 */

#ifdef BSP_USING_PWMT3
#define PWMT3_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = GPTIM3,         \
       .tim_handle.core         = PWMT3_CORE,    \
       .name                    = "pwmt3",       \
       .channel                 = 0             \
    }
#endif /* BSP_USING_PWMT3 */

#ifdef BSP_USING_PWMT4
#define PWMT4_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = GPTIM4,         \
       .tim_handle.core         = PWMT4_CORE,    \
       .name                    = "pwmt4",       \
       .channel                 = 0             \
    }
#endif /* BSP_USING_PWMT4 */

#ifdef BSP_USING_PWMT5
#define PWMT5_CONFIG                             \
    {                                           \
       .tim_handle.Instance     = GPTIM5,         \
       .tim_handle.core         = PWMT5_CORE,    \
       .name                    = "pwmt5",       \
       .channel                 = 0             \
    }
#endif /* BSP_USING_PWMT4 */

#ifdef BSP_USING_PWMA1
#define PWMA1_CONFIG                             \
        {                                           \
           .tim_handle.Instance     = (GPT_TypeDef *)ATIM1,         \
           .tim_handle.core         = ATIM1_CORE,    \
           .name                    = "pwma1",       \
           .channel                 = 0             \
        }
#endif /* BSP_USING_PWM_A1 */

#ifdef BSP_USING_PWMA2
#define PWMA2_CONFIG                             \
        {                                           \
           .tim_handle.Instance     = (GPT_TypeDef *)ATIM2,         \
           .tim_handle.core         = ATIM2_CORE,    \
           .name                    = "pwma2",       \
           .channel                 = 0             \
        }
#endif /* BSP_USING_PWM_A2 */

#ifdef BSP_USING_PWM_LPTIM1
#define PWM_LPTIM1_CONFIG                       \
    {                                           \
       .tim_handle.Instance     = hwp_lptim1,   \
       .name                    = "pwmlp1",       \
    }
#endif /* BSP_USING_PWM_LPTIM1 */

#ifdef BSP_USING_PWM_LPTIM2
#define PWM_LPTIM2_CONFIG                       \
    {                                           \
       .tim_handle.Instance     = hwp_lptim2,   \
       .name                    = "pwmlp2",       \
    }
#endif /* BSP_USING_PWM_LPTIM2 */

#ifdef BSP_USING_PWM_LPTIM3
#define PWM_LPTIM3_CONFIG                       \
    {                                           \
       .tim_handle.Instance     = hwp_lptim3,   \
       .name                    = "pwmlp3",       \
    }
#endif /* BSP_USING_PWM_LPTIM3 */

#ifdef __cplusplus
}
#endif

#endif /* __PWM_CONFIG_H__ */

