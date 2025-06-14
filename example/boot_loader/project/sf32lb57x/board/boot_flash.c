/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     zylx         first version
 */

#include <rtconfig.h>
#include <string.h>
#include <register.h>
#include "../dfu/dfu.h"
#include "bf0_hal.h"
#include "board.h"
#include "boot_flash.h"
#include "sifli_bbm.h"
#include "secboot.h"

#define BOOT_FROM_MPI3

QSPI_FLASH_CTX_T spi_flash_handle[FLASH_MAX_INSTANCE];
DMA_HandleTypeDef spi_flash_dma_handle[FLASH_MAX_INSTANCE];
flash_read_func g_flash_read;
FLASH_HandleTypeDef *boot_handle;
uint32_t g_config_addr;
static uint8_t sd_cache[512];
#ifdef BSP_NO_BOARD_USED
    static uint16_t mpi1_div = 1;
    static uint16_t mpi2_div = 2;
    static uint16_t mpi3_div = 2;
#endif /* BSP_NO_BOARD_USED */

#ifdef BSP_NO_BOARD_USED
uint16_t BSP_GetFlash1DIV(void)
{
    return mpi1_div;
}

uint16_t BSP_GetFlash2DIV(void)
{
    return mpi2_div;
}

uint16_t BSP_GetFlash3DIV(void)
{
    return mpi3_div;
}

void BSP_SetFlash1DIV(uint16_t div)
{
    mpi1_div = div;
}

void BSP_SetFlash2DIV(uint16_t div)
{
    mpi2_div = div;
}

void BSP_SetFlash3DIV(uint16_t div)
{
    mpi3_div = div;
}

#endif /* BSP_NO_BOARD_USED */

int port_read_page(int blk, int page, int offset, uint8_t *buff, uint32_t size, uint8_t *spare, uint32_t spare_len)
{
    int res;
    uint32_t addr = (blk << 17) + (page << 11) + offset;

    FLASH_HandleTypeDef *hflash = (FLASH_HandleTypeDef *)&spi_flash_handle[1].handle;
    if ((offset + size) > SPI_NAND_PAGE_SIZE)
    {
        HAL_ASSERT(0);
    }

    SCB_InvalidateDCache_by_Addr((void *)hflash->base, SPI_NAND_PAGE_SIZE + SPI_NAND_MAXOOB_SIZE);
    res = HAL_NAND_READ_WITHOOB(hflash, addr, buff, size, spare, spare_len);

    return res; //RET_NOERROR;
}

int bbm_get_bb(int blk)
{
    FLASH_HandleTypeDef *hflash = (FLASH_HandleTypeDef *)&spi_flash_handle[1].handle;

    SCB_InvalidateDCache_by_Addr((void *)hflash->base, SPI_NAND_PAGE_SIZE + SPI_NAND_MAXOOB_SIZE);
    int bad = HAL_NAND_GET_BADBLK(hflash, blk);

    return bad;
}

static int read_nand(uint32_t addr, const int8_t *buf, uint32_t size)
{
    int cnt;
    uint32_t fill, offset;

    cnt = 0;
    offset = addr >= boot_handle->base ? addr - boot_handle->base : addr;
    while (size > 0)
    {
        //SCB_InvalidateDCache_by_Addr((void *)boot_handle->base, SPI_NAND_PAGE_SIZE);
        fill = size > SPI_NAND_PAGE_SIZE ? SPI_NAND_PAGE_SIZE : size;
        //if (HAL_NAND_READ_WITHOOB(boot_handle, addr + cnt, (uint8_t *)(buf + cnt), fill, NULL, 0) != fill)
        //    break;
        if (bbm_read_page(offset >> 17, (offset >> 11) & 0x3f, offset & 0x7ff, (uint8_t *)(buf + cnt), fill, NULL, 0) != fill)
            break;
        cnt += fill;
        size -= fill;
        offset += fill;
    }
#if (NAND_BUF_CPY_MODE == 1)
    SCB_InvalidateDCache_by_Addr((void *)buf, cnt);
#endif
    return cnt;
}

static int read_nor(uint32_t addr, const int8_t *buf, uint32_t size)
{
    memcpy((void *)buf, (uint8_t *)addr, size);
    return size;
}

static uint32_t init_mpi1()
{
    // Initialize MPI1
    qspi_configure_t flash_cfg = FLASH1_CONFIG;
    struct dma_config flash_dma = FLASH1_DMA_CONFIG;

    spi_flash_handle[0].dual_mode = 1;
    flash_cfg.line = HAL_FLASH_NOR_MODE;
    if (board_boot_src == BOOT_FROM_SIP_PUYA)
    {
        board_pinmux_mpi1_puya_base();
    }
    else
    {
        board_pinmux_mpi1_gd();
#ifndef CFG_BOOTROM
        flash_cfg.line = HAL_FLASH_QMODE;
#endif
    }
    HAL_FLASH_Init(&(spi_flash_handle[0]), &flash_cfg, &spi_flash_dma_handle[0],     &flash_dma, BSP_GetFlash1DIV());
#ifndef CFG_BOOTROM
    if (board_boot_src == BOOT_FROM_SIP_PUYA)
    {
        // check spi_flash_handle[0].dev_id == 0x176085 // 0x856017
        board_pinmux_mpi1_puya_ext(spi_flash_handle[0].dev_id == 0x176085);
        HAL_FLASH_SET_QUAL_SPI((FLASH_HandleTypeDef *) & (spi_flash_handle[0].handle), true);
        spi_flash_handle[0].handle.Mode = HAL_FLASH_QMODE;
    }
#endif
    g_flash_read = read_nor;
    return (spi_flash_handle[0].base_addr);
}

static uint32_t nand_cache[(2048 + 128) / 4];
static uint32_t bbm_cache_buf[(2048 + 128) / 4];
static uint32_t init_mpi2(int nand)
{
    // Initialize MPI2
    qspi_configure_t flash_cfg = FLASH2_CONFIG;
    struct dma_config flash_dma = FLASH2_DMA_CONFIG;

    board_pinmux_mpi2();
    // for bootrom, force to 1 line spi, for bootloader use 4 line
#ifdef  CFG_BOOTROM
    flash_cfg.line = HAL_FLASH_NOR_MODE;
#else
    flash_cfg.line = HAL_FLASH_QMODE ;
#endif
    g_flash_read = nand ? read_nand : read_nor;
    if (nand)
    {
        flash_cfg.base += HPSYS_MPI_MEM_CBUS_2_SBUS_OFFSET;
        flash_cfg.SpiMode = SPI_MODE_NAND;
        spi_flash_handle[1].handle.data_buf = (uint8_t *)nand_cache;
    }
    HAL_Delay_us(0);
    spi_flash_handle[1].dual_mode = 1;
    spi_flash_handle[1].flash_mode = nand;
    HAL_StatusTypeDef res = HAL_FLASH_Init(&(spi_flash_handle[1]), &flash_cfg, &spi_flash_dma_handle[1], &flash_dma, BSP_GetFlash2DIV());
    if ((res == HAL_OK) && (nand != 0))
    {
        spi_flash_handle[1].handle.buf_mode = 1;    // default set to buffer mode for nand
        HAL_NAND_CONF_ECC(&spi_flash_handle[1].handle, 1); // default enable ECC if support !
        sif_bbm_init(spi_flash_handle[1].total_size, (uint8_t *)bbm_cache_buf);
    }
    return (spi_flash_handle[1].base_addr);
}

static uint32_t init_mpi3(int nand)
{
    // Initialize MPI2
    qspi_configure_t flash_cfg = FLASH3_CONFIG;
    struct dma_config flash_dma = FLASH3_DMA_CONFIG;

    board_pinmux_mpi3();
    // for bootrom, force to 1 line spi, for bootloader use 4 line
#ifdef  CFG_BOOTROM
    flash_cfg.line = HAL_FLASH_NOR_MODE;
#else
    flash_cfg.line = HAL_FLASH_QMODE ;
#endif
    g_flash_read = nand ? read_nand : read_nor;
    if (nand)
    {
        flash_cfg.base += HPSYS_MPI_MEM_CBUS_2_SBUS_OFFSET;
        flash_cfg.SpiMode = SPI_MODE_NAND;
        spi_flash_handle[2].handle.data_buf = (uint8_t *)nand_cache;
    }
    HAL_Delay_us(0);
    spi_flash_handle[2].dual_mode = 1;
    spi_flash_handle[2].flash_mode = nand;
    HAL_StatusTypeDef res = HAL_FLASH_Init(&(spi_flash_handle[2]), &flash_cfg, &spi_flash_dma_handle[2], &flash_dma, BSP_GetFlash3DIV());
    if ((res == HAL_OK) && (nand != 0))
    {
        spi_flash_handle[2].handle.buf_mode = 1;    // default set to buffer mode for nand
        HAL_NAND_CONF_ECC(&spi_flash_handle[2].handle, 1); // default enable ECC if support !
        sif_bbm_init(spi_flash_handle[2].total_size, (uint8_t *)bbm_cache_buf);
    }
    return (spi_flash_handle[2].base_addr);
}

/*****************************SD functions*************************************/
#include "sd_nand_drv.h"
static int read_sdnand(uint32_t addr, const int8_t *buf, uint32_t size)
{
    // TODO: Add SD read.
    uint32_t offset = addr - SDNAND_MEM_ADDR;
    uint32_t remain = size;
    uint8_t *data = (uint8_t *)buf;

    while (remain >= 512)
    {
        sd_read_data(offset, data, 512);
        remain -= 512;
        offset += 512;
        data += 512;
    }
    if (remain > 0)
    {
        sd_read_data(offset, sd_cache, 512);
        memcpy(data, sd_cache, remain);
    }
    return size;
}

static uint32_t init_sdnand()
{
    // TODO: Add SD initial sequence
    board_pinmux_sd();
    uint8_t res = sdmmc1_sdnand();
    if (res != 1)
    {
        // TODO: UART output error message.
        boot_error(res);
    }
    g_flash_read = read_sdnand;
    return SDNAND_MEM_ADDR + SDNAND_START_OFFSET;
}

/******************************************************************************/

#define MAX_RETRY 5
void dfu_flash_init()
{
#if !defined(CFG_BOOTROM) && !defined(FPGA)
    // set clock to high speed for bootloader
    HAL_HPAON_EnableXT48();
    HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_SYS, RCC_SYSCLK_HXT48);
    HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_HP_PERI, RCC_CLK_PERI_HXT48);


    // HAL_PMU_EnableDLL(1);

    // HAL_RCC_HCPU_ConfigHCLK(144);
    //HAL_RCC_HCPU_EnableDLL1(240000000);
    //HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_SYS, RCC_SYSCLK_DLL1);
    // Reset sysclk used by HAL_Delay_us
    HAL_Delay_us(0);
#endif

    switch (board_boot_src)
    {
    case BOOT_FROM_SIP_PUYA:
    case BOOT_FROM_SIP_GD:
#ifdef CFG_BOOTROM
        BSP_SetFlash1DIV(2);    // rom use default rc48 / 2, 48/2 = 24
#else
        //TODO: increase speed for SIP NOR flash
        HAL_PMU_EnableDLL(1);
        HAL_RCC_HCPU_EnableDLL1(144000000);
        HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_SYS, RCC_SYSCLK_DLL1);
        BSP_SetFlash1DIV(4);    // bootloader use sys clock / 4, 144/4 = 36
#endif
        g_config_addr = init_mpi1();
        boot_handle = (FLASH_HandleTypeDef *)&spi_flash_handle[0].handle;
        break;
    case BOOT_FROM_NOR:
    case BOOT_FROM_NAND:
#ifdef CFG_BOOTROM
        BSP_SetFlash2DIV(2);    // rom use default rc48 / 2, 48/2 = 24
        BSP_SetFlash3DIV(2);    // rom use default rc48 / 2, 48/2 = 24
#else
#ifdef FPGA
        BSP_SetFlash2DIV(2);
        BSP_SetFlash3DIV(2);
#else
        HAL_RCC_HCPU_EnableDLL2(288000000);
        BSP_SetFlash2DIV(6);    // bootloader use DLL2 / 6, 288/6 = 48
        HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_FLASH2, RCC_CLK_FLASH_DLL2);
#endif
#endif
#ifndef BOOT_FROM_MPI3
        g_config_addr = init_mpi2(board_boot_src == BOOT_FROM_NOR ? 0 : 1);
        boot_handle = (FLASH_HandleTypeDef *)&spi_flash_handle[1].handle;
#else
        g_config_addr = init_mpi3(board_boot_src == BOOT_FROM_NOR ? 0 : 1);
        boot_handle = (FLASH_HandleTypeDef *)&spi_flash_handle[2].handle;
#endif
        break;
    case BOOT_FROM_SD:
        HAL_RCC_HCPU_EnableDLL2(288000000);
        g_config_addr = init_sdnand();  // TODO, update sd clock divider after clock changed
        boot_handle = NULL;
        break;
    default:
        boot_error('S');
    }

    if (g_flash_read)
    {
        int count = MAX_RETRY;

        while (count)
        {
            g_flash_read(g_config_addr, (const int8_t *)&sec_config_cache, sizeof(struct sec_configuration));

            if (sifli_security_enabled())
            {
                /* Read bank2 root key*/
                sifli_hw_efuse_read_bank(2);
                /*get ftab key, decrypt ftab*/
                sifli_decrypt_ftab((uint8_t *)&sec_config_cache, sizeof(struct sec_configuration));
                /*ftab hash verify*/
                if (sifli_ftab_hash_verify(&sec_config_cache))
                    sifli_secboot_exception(SECBOOT_FTAB_HASH_VF_ERR);
            }

            if (sec_config_cache.magic == SEC_CONFIG_MAGIC)
                break;
#ifdef TARMAC
            HAL_Delay_us(10);
#else
            HAL_Delay_us(1000000);
#endif
            count--;
        }
        if (count == 0)
            boot_error('C');
    }
}


void sec_flash_func_init()
{
    g_flash_read = NULL;
}

#ifdef  CFG_BOOTROM
int HAL_GET_FLASH_DEFAUT_INX(void)
{
    return 4;
}
#endif

