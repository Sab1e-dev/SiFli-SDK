/*
 * SPDX-FileCopyrightText: 2019-2022 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <rtthread.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <drv_log.h>
#include <drv_common.h>
#include <drv_dma.h>


#if defined (SYS_HEAP_IN_PSRAM)
    #undef calloc
    #undef free
    extern void *app_sram_calloc(rt_size_t count, rt_size_t size);
    extern void *app_sram_free(void *ptr);
    #define  calloc(c, s)   app_sram_calloc(c, s)
    #define  free(p)        app_sram_free(p)
#endif

#define MAX_RAM_1M  16
#define IS_ACCROSS_1M_BOUNDARY(addr, size) \
    ((((uint32_t)(addr) & 0xFFFFF) + (size)) > 0x100000)


void *dma_free_in_1m(void *p)
{
    if (p)
        free(p);
}

void *dma_malloc_in_1m(uint32_t size)
{
    int i;
    void *p = NULL;
    void *max_1m[MAX_RAM_1M] = {0};

    if (size > 0x100000 || size == 0)
    {
        return NULL;
    }

    for (i = 0; i < MAX_RAM_1M; i++)
    {
        p = calloc(1, size);
        if (p == NULL)
        {
            break;
        }

        if (!IS_ACCROSS_1M_BOUNDARY(p, size))
        {
            break;
        }
        max_1m[i] = p;
    }

    if (i == MAX_RAM_1M)
    {
        p = NULL;
    }

    for (int j = 0; j < i; j++)
    {
        if (max_1m[j])
        {
            free(max_1m[j]);
            max_1m[j] = NULL;
        }
    }

    return p;
}

#ifdef DMA_SUPPORT_DYN_CHANNEL_ALLOC
void DMAC1_CH1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH1_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC1_CH2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH2_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC1_CH3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH3_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC1_CH4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH4_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC1_CH5_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH5_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC1_CH6_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH6_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC1_CH7_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH7_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC1_CH8_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC1_CH8_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH1_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH2_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH3_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH4_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH5_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH5_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH6_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH6_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH7_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH7_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC2_CH8_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC2_CH8_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

#if defined(DMA3)
/* SF32LB58X LCPU still use ROM implementation */
void DMAC3_CH1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH1_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC3_CH2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH2_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC3_CH3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH3_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC3_CH4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH4_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC3_CH5_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH5_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC3_CH6_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH6_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC3_CH7_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH7_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}

void DMAC3_CH8_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DMAC3_CH8_IRQHandler();

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* DMA3 */
#endif /* DMA_SUPPORT_DYN_CHANNEL_ALLOC */

/// @} file

