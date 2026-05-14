/*
 * SPDX-FileCopyrightText: 2019-2026 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __DRV_DMA_H__
#define __DRV_DMA_H__

#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

void *dma_malloc_in_1m(uint32_t size);
void *dma_free_in_1m(void *p);

#ifdef __cplusplus
}
#endif

#endif
