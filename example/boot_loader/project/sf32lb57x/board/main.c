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
#include <board.h>
#include <string.h>
#include "register.h"
#include "../dfu/dfu.h"
#include "boot_flash.h"
#include "secboot.h"

int board_boot_src;
ALIGN(4) struct sec_configuration sec_config_cache;

typedef void (*ram_hook_handler)(void);
void boot_ram(void)
{
    volatile ram_hook_handler hook = (volatile ram_hook_handler)hwp_hpsys_aon->RESERVE0;
    if (hook)
        hook();
}


/**************************Efuse**************************************************/
#define boot_efuse_init_stage1(void) \
{ \
    hwp_efusec->TCR = 0x1DF; \
    /* Read bank0 */ \
    sifli_hw_efuse_read_bank(0); \
}

#define boot_efuse_init_stage2(void)


/************************Boot *****************************************/

void run_img(uint32_t dest)
{
    __asm
    (
        "LDR SP, [%0]                     \n"
        "LDR PC, [%0, #4]                 \n"
        :           /* Outputs */
        : "r"(dest) /* Inputs */
        :           /* Clobbers */
    );
}

uint8_t is_addr_in_nor(uint32_t addr)
{
    if (boot_handle && boot_handle->isNand == 0 &&
            addr >= boot_handle->base && addr < boot_handle->base + boot_handle->size)
        return 1;
    else
        return 0;
}

void dfu_boot_img_in_flash(int flashid)
{
    uint32_t src = sec_config_cache.ftab[flashid].base;
    uint32_t dest = sec_config_cache.ftab[flashid].xip_base;
    int coreid = DFU_FLASH_IMG_IDX(flashid);
    struct image_header_enc *img_hdr = &(sec_config_cache.imgs[coreid]);
#if !defined(CFG_BOOTROM) || defined(PKG_SIFLI_MBEDTLS_BOOT)
    struct sec_configuration *sec_config = &sec_config_cache;
#endif

    if (img_hdr->flags & DFU_FLAG_ENC)
    {
#ifndef CFG_BOOTROM
        /* verify public sig_key hash */
        if (sifli_sigkey_pub_verify(sec_config->sig_pub_key, DFU_SIG_KEY_SIZE))
            sifli_secboot_exception(SECBOOT_SIGKEY_PUB_ERR);
#endif /* CFG_BOOTROM */

        if (coreid < 2 * CORE_MAX)
        {
            coreid %= CORE_MAX;
            if (coreid == CORE_HCPU || coreid == CORE_BL)
            {
                ALIGN(4)
                static uint8_t dfu_key[DFU_KEY_SIZE];
                /** key in plaintext */
                ALIGN(4)
                static uint8_t dfu_key1[DFU_KEY_SIZE];
                if (is_addr_in_nor(dest))
                {
                    memcpy(dfu_key, img_hdr->key, sizeof(dfu_key));
                    sifli_hw_dec_key(dfu_key, dfu_key1, sizeof(dfu_key1));
                    // Setup XIP for decoding and running
                    sifli_mpi_xip_dec(coreid, dest, img_hdr->length, dfu_key1);
                    HAL_FLASH_ALIAS_CFG(boot_handle, dest, img_hdr->length, src - dest);
                }
                else
                {
                    /* copy encrypted key to ram as AES_ACC cannot access flash */
                    memcpy(dfu_key, img_hdr->key, sizeof(dfu_key));
                    sifli_hw_dec_key(dfu_key, dfu_key1, sizeof(dfu_key1));
                    g_flash_read(src, (const int8_t *)dest, img_hdr->length);
                    sifli_hw_dec_once(dfu_key1, (uint8_t *)dest, (uint8_t *)dest, img_hdr->length);
                }
#ifdef CFG_BOOTROM
                /*verify bootloader hash*/
                if (sifli_bootloader_hash_verify((uint8_t *)dest, img_hdr->length, img_hdr->sig))
                    sifli_secboot_exception(SECBOOT_BL_HASH_VF_ERR);
#else
#ifdef PKG_SIFLI_MBEDTLS_BOOT
                /* verify image hash signature */
                if (sifli_img_sig_hash_verify(img_hdr->sig, sec_config->sig_pub_key, (uint8_t *)dest, img_hdr->length))
                    sifli_secboot_exception(SECBOOT_IMG_HASH_SIG_ERR);
#endif
#endif
                run_img(dest);
            }
        }
    }
    else /*plain text image*/
    {
        if (coreid < 2 * CORE_MAX)
        {
            coreid %= CORE_MAX;
            if (coreid == CORE_HCPU || coreid == CORE_BL)
            {
                if (is_addr_in_nor(dest))
                    HAL_FLASH_ALIAS_CFG(boot_handle, dest, img_hdr->length, src - dest);
                else if (src != dest)
                    g_flash_read(src, (const int8_t *)dest, img_hdr->length);

                run_img(dest);
            }
        }
    }
}

void boot_images_help()
{
    if (sec_config_cache.magic == SEC_CONFIG_MAGIC)
    {
#ifdef  CFG_BOOTROM
        if (sec_config_cache.running_imgs[CORE_BL] != (struct image_header_enc *)FLASH_UNINIT_32)
        {
            int flash_id = ((uint32_t)sec_config_cache.running_imgs[CORE_BL] - g_config_addr - 0x1000) / sizeof(struct image_header_enc)
                           + DFU_FLASH_IMG_LCPU;
            dfu_boot_img_in_flash(flash_id);
        }
#else
        if (sec_config_cache.running_imgs[CORE_HCPU] != (struct image_header_enc *)FLASH_UNINIT_32)
        {
            int flash_id = ((uint32_t)sec_config_cache.running_imgs[CORE_HCPU] - g_config_addr - 0x1000) / sizeof(struct image_header_enc) + DFU_FLASH_IMG_LCPU;
            {
                extern void board_init_psram();
                board_init_psram();
            }
            dfu_boot_img_in_flash(flash_id);
        }
#endif
    }
}

void hw_preinit0(void)
{
#if 0
    /* lower power on threshold and set VBAT_LDO output voltage to default 3.3V*/
    MODIFY_REG(hwp_pmuc->AON_LDO, PMUC_AON_LDO_VBAT_POR_TH_Msk | PMUC_AON_LDO_VBAT_LDO_SET_VOUT_Msk,
               MAKE_REG_VAL(0, PMUC_AON_LDO_VBAT_POR_TH_Msk, PMUC_AON_LDO_VBAT_POR_TH_Pos)
               | MAKE_REG_VAL(6, PMUC_AON_LDO_VBAT_LDO_SET_VOUT_Msk, PMUC_AON_LDO_VBAT_LDO_SET_VOUT_Pos));

    /* auto power down if VCC is low */
    hwp_pmuc->WER |= PMUC_WER_LOWBAT;
#endif

    HAL_Delay_us(0);

    // 1. Read efuse bank0 first to take efuse effect.
    boot_efuse_init_stage1();

    // 2. If ram hook existed, just jump to ram.
    boot_ram();
}

/**************************main**************************************/

#if defined(__CC_ARM) || defined(__CLANG_ARM)
    int main(void)
#elif defined(__ICCARM__)
    int __low_level_init(void)
#elif defined(__GNUC__)
    int entry(void)
#endif
{
    HAL_Delay_us(0);

    // 3. Read boot options
    board_boot_src = board_boot_from();

    // 4. Power on flash.
    board_flash_power_on();

    // 5. Check boot mode.
    HAL_MspInit();

    // 6. Boot images
#ifdef CFG_BOOTROM
    if (hwp_hpsys_cfg->BMR == 0)
#endif
    {
        /* init AES_ACC as normal mode */
        //TODO:
        // __HAL_SYSCFG_CLEAR_SECURITY();
        dfu_flash_init();
        boot_images_help();
    }


    while (1)
        ;

    return HAL_OK;
}


