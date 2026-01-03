/*
 * SPDX-FileCopyrightText: 2019-2022 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "rtconfig.h"
#include "bf0_hal.h"
#include "../dfu/dfu.h"
#include "mbedtls/cipher.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#ifdef BSP_USING_DFU_COMPRESS
    #include "zlib.h"
#endif

#ifdef CFG_BOOTLOADER
    #define DBG_ENABLE
#endif /* CFG_BOOTLOADER */

#define LOG_TAG "dfu"
#define DBG_LVL    DBG_INFO

#ifndef ALIGN
    #define ALIGN(n)                    __attribute__((aligned(n)))
#endif /* ALIGN */


#define IS_MPI_ADDR(addr, i) ((addr >= MPI##i##_MEM_BASE) && (addr < (MPI##i##_MEM_BASE + QSPI##i##_MAX_SIZE)))

#define LOG_I(...)   printf(__VA_ARGS__)
#define LOG_E(...)   printf(__VA_ARGS__)

static uint8_t dfu_temp[DFU_MAX_BLK_SIZE];
/* sha256 */
static uint8_t dfu_hash[32];
static mbedtls_sha256_context ctx2;

#ifdef BSP_USING_DFU_COMPRESS
    struct image_header_compress g_dfu_img_progress;
#endif

volatile struct dfu_compress_configuration *g_dfu_compress_config;

/** encoded key */
ALIGN(4)
static uint8_t dfu_key[DFU_KEY_SIZE];
/** key in plaintext */
ALIGN(4)
static uint8_t dfu_key1[DFU_KEY_SIZE];
struct sec_configuration *g_config_cache;
struct dfu_configuration *g_dfu_config;
static uint8_t g_sig_hash_cache[DFU_SIG_HASH_SIZE];
static struct image_header_enc g_boot_patch_img_head;
static uint8_t g_boot_patch_sig_pub_key[DFU_SIG_KEY_SIZE];
static uint32_t g_boot_patch_addr = BOOTLOADER_PATCH_CODE_ADDR;

//#define DFU_DEV
#ifdef DFU_DEV
    extern int sifli_hw_efuse_write_din(uint8_t id, uint8_t *data, int size);
#endif

static const uint8_t g_fake_dfu_efuse_uid[DFU_UID_SIZE] =
{
    0xB7, 0x76, 0x6B, 0x8A, 0xD7, 0xA5, 0xE7, 0xD0, 0x88, 0x52, 0x36, 0xDE, 0xC3, 0x16, 0x36, 0x4C
};

static const uint8_t g_fake_dfu_efuse_sig_hash[DFU_SIG_HASH_SIZE] =
{
    0x56, 0xCE, 0x4E, 0xC4, 0xC9, 0xDC, 0xF4, 0x11
};

static const uint8_t g_fake_dfu_efuse_root_key[DFU_KEY_SIZE] =
{
    0xE6, 0x92, 0xDF, 0xB5, 0xE8, 0xFA, 0xC2, 0x43, 0x78, 0x13, 0x34, 0x5D, 0x1B, 0x4B, 0xE2, 0xB9,
    0x50, 0x9D, 0xE5, 0xC2, 0xF4, 0x05, 0x62, 0xE6, 0xC8, 0x25, 0x85, 0x81, 0x02, 0x59, 0x17, 0x2B,
};

static void clear_interrupt_setting(void)
{
    uint32_t i;
    for (i = 0; i < 16; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        __DSB();
        __ISB();
    }
}


static void dfu_run_img(uint32_t dest)
{
    HAL_DisableInterrupt();
    clear_interrupt_setting();

    __asm("LDR SP, [%0]" :: "r"(dest));
    __asm("LDR PC, [%0, #4]" :: "r"(dest));
}


static int dfu_get_efuse_hook(uint8_t id, uint8_t *data, int size)
{
    int ret = 0;

    if (id == EFUSE_UID)
    {
        memcpy(data, (uint8_t *)g_fake_dfu_efuse_uid, DFU_UID_SIZE);
        ret = DFU_UID_SIZE;
    }
    else if (id == EFUSE_ID_SIG_HASH)
    {
        memcpy(data, (uint8_t *)g_fake_dfu_efuse_sig_hash, DFU_SIG_HASH_SIZE);
        ret = DFU_SIG_HASH_SIZE;
    }
    else if (id == EFUSE_ID_ROOT)
    {
        memcpy(data, (uint8_t *)g_fake_dfu_efuse_root_key, DFU_KEY_SIZE);
        ret = DFU_KEY_SIZE;
    }

    return ret;

}

uint8_t *sifli_dec_verify(uint8_t *key, uint32_t offset,
                          uint8_t *in_data, uint8_t *out_data, int size, uint8_t *hash)
{
    uint8_t *r = NULL;

    sifli_hw_dec(key, in_data, out_data, size, offset);
    mbedtls_sha256_init(&ctx2);
    mbedtls_sha256_starts(&ctx2, 0); /* SHA-256, not 224 */
    mbedtls_sha256_update(&ctx2, out_data, size);
    mbedtls_sha256_finish(&ctx2, dfu_hash);
    if (memcmp(dfu_hash, hash, DFU_SIG_HASH_SIZE) == 0)
        r = out_data;
    return r;
}

static int8_t sifli_sig_key_verify(uint8_t *sig_pub_key)
{
    int r = DFU_FAIL;
    uint8_t sig_hash[DFU_SIG_HASH_SIZE];
    if (sifli_hw_efuse_read(EFUSE_ID_SIG_HASH, (uint8_t *)&sig_hash, DFU_SIG_HASH_SIZE) == 0)
        return DFU_FAIL;

    mbedtls_sha256_init(&ctx2);
    mbedtls_sha256_starts(&ctx2, 0); /* SHA-256, not 224 */
    mbedtls_sha256_update(&ctx2, sig_pub_key, DFU_SIG_KEY_SIZE);
    mbedtls_sha256_finish(&ctx2, dfu_hash);
    if (memcmp(dfu_hash, sig_hash, DFU_SIG_HASH_SIZE) == 0)
        r = DFU_SUCCESS;
    return r;
}

#ifdef BSP_USING_DFU_COMPRESS
static uint32_t dfu_compress_check_dup_image(uint8_t flashid)
{
    uint32_t index = 0xFF;
    if (g_dfu_compress_config->img_count)
    {
        uint32_t i = g_dfu_compress_config->img_count;
        while (i--)
        {
            if (flashid == g_dfu_compress_config->imgs[i].compress_img_id)
            {
                index = i;
                break;
            }
        }
    }
    return index;
}
#endif


static int dfu_process_hdr_sec(uint8_t flashid, uint8_t *data, int size)
{
    int r = DFU_FAIL;
    uint8_t *d;
    struct image_cfg_hdr *hdr = (struct image_cfg_hdr *)data;
    int img_idx = DFU_FLASH_IMG_IDX(flashid);

#ifdef BSP_USING_DFU_COMPRESS
    uint8_t compress_img_idx = DFU_FLASH_IMG_COMPRESS_IDX(flashid);
    if (DFU_FLASH_IMG_COMPRESS_FLASH(flashid) == DFU_FLASH_COMPRESS)
    {
        flashid = DFU_FLASH_IMG_COMPRESS_FLASH(flashid);
    }
#endif

    if (
#ifdef BSP_USING_DFU_COMPRESS
        (flashid > DFU_FLASH_COMPRESS)
#else
        (flashid >= DFU_FLASH_PARTITION)
#endif
        || (flashid < DFU_FLASH_IMG_LCPU)
    )
    {
        return r;
    }

    data += sizeof(struct image_cfg_hdr);
    size -= sizeof(struct image_cfg_hdr);

    d = sifli_dec_verify(NULL, 0, data, dfu_temp, size, hdr->hash);
    if (d)          // Use root key to decode image header
    {
        struct image_header_enc *hdr = (struct image_header_enc *)d;

        // encrypt image key with root key+UID
        sifli_hw_enc(hdr->key, dfu_key, DFU_KEY_SIZE);
        /* save image_key in plaintext */
        memcpy(dfu_key1, hdr->key, DFU_KEY_SIZE);
        memcpy(hdr->key, dfu_key, DFU_KEY_SIZE);

        if (DFU_FLASH_SINGLE == flashid)
        {
            img_idx = DFU_FLASH_IMG_IDX(DFU_FLASH_IMG_HCPU);
            hdr->flags |= DFU_FLAG_SINGLE;
        }
        if (DFU_FLASH_IMG_BOOT2 == flashid)
        {
            memcpy(&g_boot_patch_img_head, d, sizeof(g_boot_patch_img_head));
        }
        else
        {
            sec_flash_update(DFU_FLASH_SEC_CONFIG, SECFG_IMG_OFFSET + img_idx * sizeof(struct image_header_enc), d, size);
        }

        // Save to flash and erase the image flash section.
        sec_flash_erase(flashid, 0, hdr->length);
        r = DFU_SUCCESS;
    }
    else
    {
        LOG_E("image header verify fail\n");
    }
    return r;
}

static int dfu_process_body_sec(uint8_t flashid, uint8_t *data, int size)
{
    int r = DFU_FAIL;
    struct image_body_hdr *hdr = (struct image_body_hdr *)data;
    uint8_t *key;
    uint8_t *d;
    struct image_header_enc *img_hdr;

#ifdef BSP_USING_DFU_COMPRESS
    if (DFU_FLASH_IMG_COMPRESS_FLASH(flashid) == DFU_FLASH_COMPRESS)
    {
        flashid = DFU_FLASH_IMG_COMPRESS_FLASH(flashid);
    }
#endif

    if (
#ifdef BSP_USING_DFU_COMPRESS
        (flashid > DFU_FLASH_COMPRESS)
#else
        (flashid >= DFU_FLASH_PARTITION)
#endif
        || (flashid < DFU_FLASH_IMG_LCPU)
    )
    {
        return r;
    }

    // Read image key
    key = dfu_key1;
    size -= sizeof(struct image_body_hdr);
    data += sizeof(struct image_body_hdr);
    d = sifli_dec_verify(key, hdr->offset, data, dfu_temp, size, hdr->hash);
    if (d)
    {
        if (DFU_FLASH_IMG_BOOT2 == flashid)
        {
            img_hdr = &g_boot_patch_img_head;
        }
        else
        {
            goto __EXIT;
        }
        if (img_hdr->flags & DFU_FLAG_ENC)
            sec_flash_write(flashid, hdr->offset, data, size);
        else
            sec_flash_write(flashid, hdr->offset, d, size);
        r = DFU_SUCCESS;
    }
    else
    {
        LOG_E("body verify fail\n");
    }

__EXIT:
    return r;
}

static int dfu_process_config_sec(uint8_t keyid, uint8_t *data, int len)
{
    int r = DFU_FAIL;

    if (keyid < DFU_CONFIG_FLASH_TABLE)
    {
        sifli_hw_efuse_write(keyid, data, len);
        r = DFU_SUCCESS;
    }
    else if ((keyid == DFU_CONFIG_SIG)
             || (keyid == DFU_CONFIG_BOOT_PATCH_SIG))
    {
        int i;
        mbedtls_sha256_context ctx2;
        mbedtls_sha256_init(&ctx2);
        mbedtls_sha256_starts(&ctx2, 0); /* SHA-256, not 224 */
        mbedtls_sha256_update(&ctx2, data, len);
        mbedtls_sha256_finish(&ctx2, dfu_temp);
        sifli_hw_efuse_read(EFUSE_ID_SIG_HASH, g_sig_hash_cache, DFU_SIG_HASH_SIZE);
        for (i = 0; i < DFU_SIG_HASH_SIZE; i++)
            if (g_sig_hash_cache[i] != dfu_temp[i])
                break;
        if (i == DFU_SIG_HASH_SIZE)
        {
            r = DFU_SUCCESS;
            if (keyid == DFU_CONFIG_SIG)
            {
                sec_flash_write(DFU_FLASH_SEC_CONFIG, SECFG_SIGKEY_OFFSET, data, len);
            }
            else
            {
                memcpy(&g_boot_patch_sig_pub_key, data, sizeof(g_boot_patch_sig_pub_key));
            }
        }
    }
    else if (keyid == DFU_CONFIG_FLASH_TABLE)
    {
        struct image_cfg_hdr *hdr = (struct image_cfg_hdr *)data;
        uint8_t *d;

        data += sizeof(struct image_cfg_hdr);
        len -= sizeof(struct image_cfg_hdr);
        d = sifli_dec_verify(NULL, 0, data, dfu_temp, len, hdr->hash);
        if (d)
        {
            sec_flash_update(DFU_FLASH_SEC_CONFIG, SECFG_FTAB_OFFSET, d, len);
            r = DFU_SUCCESS;
        }
    }
    else if (keyid == DFU_CONFIG_BOOT_PATCH_ADDR)
    {
        g_boot_patch_addr = (uint32_t)data[0] | ((uint32_t)data[1] << 8)
                            | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
        printf("config boot patch addr:0x%x", g_boot_patch_addr);
    }
    return r;
}

int sec_flash_read(int flashid, uint32_t offset, uint8_t *data, uint32_t size)
{
    if (flashid == DFU_FLASH_IMG_BOOT2)
    {
        memcpy(data, (uint8_t *)(g_boot_patch_addr + offset), size);
    }
    else
    {
        HAL_ASSERT(0);
    }
    return size;
}

void sec_flash_write(int flashid, uint32_t offset, uint8_t *data, uint32_t size)
{
    if (flashid == DFU_FLASH_IMG_BOOT2)
    {
        memcpy((uint8_t *)(g_boot_patch_addr + offset), data, size);
    }
    else
    {
        HAL_ASSERT(0);
    }
}

void sec_flash_erase(int flashid, uint32_t offset, uint32_t size)
{
    if (flashid == DFU_FLASH_IMG_BOOT2)
    {
        memset((uint8_t *)g_boot_patch_addr, 0, size);
    }
    else
    {
        HAL_ASSERT(0);
    }
}


void sec_flash_update(int flashid, uint32_t offset, uint8_t *data, uint32_t size)
{
    HAL_ASSERT(0);
}

void sec_flash_init()
{
}


static int dfu_end(uint8_t flashid)
{
    int offset = 0;
    int size = 0;
    int r = DFU_FAIL;
    int img_idx;
    struct image_header_enc *img_hdr;
    uint8_t *sig_pub_key;

    if (
#ifdef BSP_USING_DFU_COMPRESS
        (flashid > DFU_FLASH_COMPRESS)
#else
        (flashid >= DFU_FLASH_PARTITION)
#endif
        || (flashid < DFU_FLASH_IMG_LCPU)
    )
    {
        return r;
    }

    if (DFU_FLASH_SINGLE == flashid)
    {
        img_idx = DFU_FLASH_IMG_IDX(DFU_FLASH_IMG_HCPU);
    }
    else
    {
        img_idx = DFU_FLASH_IMG_IDX(flashid);
    }

    if (DFU_FLASH_IMG_BOOT2 == flashid)
    {
        img_hdr = &g_boot_patch_img_head;
        sig_pub_key = &g_boot_patch_sig_pub_key[0];
    }
    else
    {
        printf("invalid flashid:%d\n", flashid);
        return r;
    }

    {
        // Verify sig key.
        if (sifli_sig_key_verify(sig_pub_key) != DFU_SUCCESS)
        {
            printf("sig key verify fail\n");
            return r;
        }

    }

    {
        // Calculate HASH
        mbedtls_sha256_context ctx2;
        mbedtls_sha256_init(&ctx2);
        mbedtls_sha256_starts(&ctx2, 0); /* SHA-256, not 224 */
        do
        {
            if (offset + sizeof(dfu_temp) > img_hdr->length)
                size = img_hdr->length - offset;
            else
                size = sizeof(dfu_temp);
            if (size)
            {
                sec_flash_read(flashid, offset, dfu_temp, size);
                mbedtls_sha256_update(&ctx2, dfu_temp, size);
            }
            if (size < sizeof(dfu_temp))
                break;
            offset += size;
        }
        while (1);
        mbedtls_sha256_finish(&ctx2, dfu_temp);
    }
    {
        // Verify signature
        mbedtls_pk_context pk;
        mbedtls_pk_init(&pk);
        if (mbedtls_pk_parse_public_key(&pk, sig_pub_key, DFU_SIG_KEY_SIZE) == 0)
        {
            mbedtls_rsa_set_padding((mbedtls_rsa_context *)pk.pk_ctx, MBEDTLS_RSA_PKCS_V15, MBEDTLS_MD_SHA256);
            /*Calculate the RSA encryption of the data. */
            if (mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, dfu_temp, DFU_IMG_HASH_SIZE,
                                  img_hdr->sig, DFU_SIG_SIZE) == 0)
            {
                if (flashid == DFU_FLASH_IMG_BOOT2) //patch in RAM, install directly
                {
                    printf("jump to %x\n", *(uint32_t *)(g_boot_patch_addr + 4));
                    dfu_run_img(g_boot_patch_addr);
                }
                else
                {
                    HAL_ASSERT(0);
                }
                r = DFU_SUCCESS;
            }
            else
            {
                printf("img verify fail\n");
            }
        }
        else
        {
            printf("sig_pub_key verify fail\n");
        }
    }

#ifdef BSP_USING_DFU_COMPRESS
    {
        struct dfu_compress_configuration *config = rt_malloc(sizeof(struct dfu_compress_configuration));
        // All other app should stop in this scenario, heap should be enough
        RT_ASSERT(config);
        rt_memcpy(config, (const void *)g_dfu_compress_config, sizeof(struct dfu_compress_configuration));
        g_dfu_img_progress.state = r == DFU_SUCCESS ? DFU_STATE_BIN_DOWNLOADED : DFU_STATE_BIN_READY;
        rt_memcpy(&config->imgs[config->img_count - 1], &g_dfu_img_progress, sizeof(struct image_header_compress));
        sec_flash_erase(DFU_FLASH_COMPRESS_CONFIG, 0, SPI_NOR_SECT_SIZE);
        sec_flash_write(DFU_FLASH_COMPRESS_CONFIG, 0, (uint8_t *)config, sizeof(struct dfu_compress_configuration));
        rt_free(config);
    }
#endif
    return r;
}

#ifdef BSP_USING_DFU_COMPRESS
static void dfu_get_resource_information(uint8_t flashid, uint32_t *addr, uint32_t *size)
{
    uint32_t addr_l;
    uint32_t size_l;
    switch (flashid)
    {
    case DFU_FLASH_IMAGE:
    {
        addr_l = HCPU_FLASH2_IMG_START_ADDR;
        size_l = HCPU_FLASH2_IMG_SIZE;
        break;
    }
    case DFU_FLASH_FONT:
    {
        addr_l = HCPU_FLASH2_FONT_START_ADDR;
        size_l = HCPU_FLASH2_FONT_SIZE;
        break;
    }
    case DFU_FLASH_IMAGE_COMPRESS:
    {
        addr_l = HCPU_FLASH2_IMG_UPGRADE_START_ADDR;
        size_l = HCPU_FLASH2_IMG_UPGRADE_SIZE;
        break;
    }
    case DFU_FLASH_FONT_COMPRESS:
    {
        addr_l = HCPU_FLASH2_FONT_UPGRADE_START_ADDR;
        size_l = HCPU_FLASH2_FONT_UPGRADE_SIZE;
        break;
    }
    default:
    {
        addr_l = NULL;
        size_l = 0;
        break;
    }
    }
    if (addr)
        *addr = addr_l;
    if (size)
        *size = size_l;

}

static int dfu_process_hdr(uint8_t flashid, uint8_t *data, int size)
{
    int r = DFU_FAIL;
    uint8_t *d;

#if 0
#ifdef BSP_USING_DFU_COMPRESS
    uint8_t compress_img_idx;
    if (flashid == DFU_FLASH_IMAGE_COMPRESS)
        compress_img_idx = DFU_FLASH_IMAGE;
    else if (flashid == DFU_FLASH_FONT_COMPRESS)
        compress_img_idx = DFU_FLASH_FONT;
#endif
#endif

    if (flashid < DFU_FLASH_IMAGE ||
#ifndef BSP_USING_DFU_COMPRESS
            flashid > DFU_FLASH_FONT)
#else
            flashid > DFU_FLASH_FONT_COMPRESS)
#endif
    {
        return r;
    }

    //int img_idx = DFU_FLASH_IMG_IDX(flashid);

#if 0
    if (flashid == DFU_FLASH_IMAGE ||
            flashid == DFU_FLASH_FONT)
    {
        // Image and font can overwrite directly.
        return DFU_SUCCESS;
    }
#endif

    struct image_header *hdr = (struct image_header *)data;

    // Save to flash and erase the image flash section.

//#ifdef BSP_USING_DFU_COMPRESS
//    if (DFU_FLASH_IMAGE_COMPRESS == flashid ||
//             DFU_FLASH_COMPRESS_FONT == flashid)
    {

        uint32_t index = dfu_compress_check_dup_image(flashid);

        struct dfu_compress_configuration *config = rt_malloc(sizeof(struct dfu_compress_configuration));
        RT_ASSERT(config);
        rt_memcpy(config, (const void *)g_dfu_compress_config, sizeof(struct dfu_compress_configuration));

        dfu_get_resource_information(flashid, &config->ctab[config->img_count].base,
                                     &config->ctab[config->img_count].size);

        rt_memcpy(&g_dfu_img_progress.img.none_enc_img, hdr, sizeof(struct image_header));
        g_dfu_img_progress.state = DFU_STATE_BIN_DOWNLOADING;
        g_dfu_img_progress.compress_img_id = flashid;
        g_dfu_img_progress.current_img_len = 0;
        g_dfu_img_progress.current_packet_count = 0;

        if (index != 0xFF)
        {
            rt_memcpy((void *)&config->imgs[index], (const void *)&g_dfu_img_progress, sizeof(struct image_header_compress));

        }
        else
        {
            dfu_get_resource_information(flashid, &config->ctab[config->img_count].base,
                                         &config->ctab[config->img_count].size);
            if (flashid == DFU_FLASH_IMAGE_COMPRESS)
                dfu_get_resource_information(DFU_FLASH_IMAGE, &g_dfu_img_progress.img.none_enc_img.target_base, NULL);
            else if (flashid == DFU_FLASH_FONT_COMPRESS)
                dfu_get_resource_information(DFU_FLASH_FONT, &g_dfu_img_progress.img.none_enc_img.target_base, NULL);
            else
                g_dfu_img_progress.img.none_enc_img.target_base = 0;
            rt_memcpy((void *)&config->imgs[config->img_count++], (const void *)&g_dfu_img_progress, sizeof(struct image_header_compress));
        }

        sec_flash_erase(DFU_FLASH_COMPRESS_CONFIG, 0, SPI_NOR_SECT_SIZE);
        sec_flash_write(DFU_FLASH_COMPRESS_CONFIG, 0, (uint8_t *)config, sizeof(struct dfu_compress_configuration));
        rt_free(config);
    }
//#endif
    sec_flash_erase(flashid, 0, hdr->length);

    r = DFU_SUCCESS;
    return r;
}

static int dfu_process_body(uint8_t flashid, uint8_t *data, uint32_t offset, int size)
{
    int r = DFU_FAIL;
    uint8_t *key;
    uint8_t *d;
    int flash_img_idx = DFU_FLASH_IMG_IDX(flashid);
    struct image_header *img_hdr;

    if (flashid < DFU_FLASH_IMAGE ||
#ifndef BSP_USING_DFU_COMPRESS
            flashid > DFU_FLASH_FONT)
#else
            flashid > DFU_FLASH_FONT_COMPRESS)
#endif
    {
        return r;
    }

#ifdef BSP_USING_DFU_COMPRESS
    {
        img_hdr = &g_dfu_img_progress.img.none_enc_img;
        g_dfu_img_progress.current_img_len += size;
        g_dfu_img_progress.current_packet_count++;
        if ((g_dfu_img_progress.current_packet_count % 4) == 0)
        {
            struct dfu_compress_configuration *config = rt_malloc(sizeof(struct dfu_compress_configuration));
            // All other app should stop in this scenario, heap should be enough
            RT_ASSERT(config);
            rt_memcpy(config, (const void *)g_dfu_compress_config, sizeof(struct dfu_compress_configuration));
            rt_memcpy(&config->imgs[config->img_count - 1], &g_dfu_img_progress, sizeof(struct image_header_compress));
            sec_flash_erase(DFU_FLASH_COMPRESS_CONFIG, 0, SPI_NOR_SECT_SIZE);
            sec_flash_write(DFU_FLASH_COMPRESS_CONFIG, 0, (uint8_t *)config, sizeof(struct dfu_compress_configuration));
            rt_free(config);
        }

    }
#endif

    sec_flash_write(flashid, offset, data, size);
    r = DFU_SUCCESS;

    return r;
}

static int dfu_end_wo_enc(uint8_t flashid)
{
    int offset = 0;
    int size = 0;
    int r = DFU_FAIL;
    int img_idx;
    struct image_header *img_hdr;
    uint8_t *sig_pub_key;

    if (flashid < DFU_FLASH_IMAGE ||
#ifndef BSP_USING_DFU_COMPRESS
            flashid > DFU_FLASH_FONT)
#else
            flashid > DFU_FLASH_FONT_COMPRESS)
#endif
    {
        return r;
    }

    if (g_dfu_img_progress.compress_img_id != flashid)
        return r;


    r = DFU_SUCCESS;
#ifdef BSP_USING_DFU_COMPRESS
    {
        struct dfu_compress_configuration *config = rt_malloc(sizeof(struct dfu_compress_configuration));
        // All other app should stop in this scenario, heap should be enough
        RT_ASSERT(config);
        rt_memcpy(config, (const void *)g_dfu_compress_config, sizeof(struct dfu_compress_configuration));
        if (flashid == DFU_FLASH_IMAGE || flashid == DFU_FLASH_FONT)
        {
            g_dfu_img_progress.state  = r == DFU_SUCCESS ? DFU_STATE_BIN_INSTALLED : DFU_STATE_BIN_NOT_EXISTED;
        }
        else
            g_dfu_img_progress.state = r == DFU_SUCCESS ? DFU_STATE_BIN_DOWNLOADED : DFU_STATE_BIN_READY;
        rt_memcpy(&config->imgs[config->img_count - 1], &g_dfu_img_progress, sizeof(struct image_header_compress));
        sec_flash_erase(DFU_FLASH_COMPRESS_CONFIG, 0, SPI_NOR_SECT_SIZE);
        sec_flash_write(DFU_FLASH_COMPRESS_CONFIG, 0, (uint8_t *)config, sizeof(struct dfu_compress_configuration));
        rt_free(config);
    }
#endif
    return r;
}
#endif


static uint8_t dfu_secure_boot_check()
{
    uint8_t pattern;
    uint32_t ret = 0;
    int len = sifli_hw_efuse_read(EFUSE_ID_SECURE_ENABLED, &pattern, DFU_SECURE_SIZE);
    if ((len > 0) && (pattern != 0))
    {
        ret = 1;
    }

    return ret;
}

void dfu_init(void)
{
    if (dfu_secure_boot_check() == 0)
    {
        g_dfu_efuse_read_hook = dfu_get_efuse_hook;
    }
}


int dfu_verify_hdr_sec(uint8_t flashid, uint8_t *data, int size)
{
    int r = DFU_FAIL;
    struct image_cfg_hdr *hdr = (struct image_cfg_hdr *)data;
    uint8_t *d;

    data += sizeof(struct image_cfg_hdr);
    size -= sizeof(struct image_cfg_hdr);
    d = sifli_dec_verify(NULL, 0, data, dfu_temp, size, hdr->hash);
    if (d)
    {
        r = DFU_SUCCESS;
    }
    return r;

}

int dfu_verify_body_sec(uint8_t flashid, uint8_t *data, int size)
{
    int r = DFU_FAIL;
    struct image_body_hdr *hdr = (struct image_body_hdr *)data;
    uint8_t *key;
    uint8_t *d;

    if ((flashid >= DFU_FLASH_PARTITION)
            || (flashid < DFU_FLASH_IMG_LCPU))
    {
        return r;
    }

    // Read image key
    key = dfu_key1;
    size -= sizeof(struct image_body_hdr);
    data += sizeof(struct image_body_hdr);
    d = sifli_dec_verify(key, hdr->offset, data, dfu_temp, size, hdr->hash);
    if (d)
    {
        r = DFU_SUCCESS;
    }
    return r;
}

int dfu_receive_pkt(int len, uint8_t *data)
{
    int r;
    struct dfu_hdr *hdr = (struct dfu_hdr *)data;

    switch (hdr->command)
    {
    case DFU_IMG_HDR_ENC:
        r = dfu_process_hdr_sec(hdr->id, data + sizeof(struct dfu_hdr), len - sizeof(struct dfu_hdr));
        break;
    case DFU_IMG_BODY_ENC:
        r = dfu_process_body_sec(hdr->id, data + sizeof(struct dfu_hdr), len - sizeof(struct dfu_hdr));
        break;
    case DFU_CONFIG_ENC:
        r = dfu_process_config_sec(hdr->id, data + sizeof(struct dfu_hdr), len - sizeof(struct dfu_hdr));
        break;
    case DFU_END:
        r = dfu_end(hdr->id);
        break;
#ifdef BSP_USING_DFU_COMPRESS
    case DFU_IMG_HDR:
    {
        r = dfu_process_hdr(hdr->id, data + sizeof(struct dfu_hdr), len - sizeof(struct dfu_hdr));
        break;
    }
    case DFU_IMG_BODY:
    {
        struct dfu_hdr_body *hdr1 = (struct dfu_hdr_body *)data;
        r = dfu_process_body(hdr1->id, data + sizeof(struct dfu_hdr_body), hdr1->offset, len - sizeof(struct
                             dfu_hdr_body));
        break;

    }
    case DFU_END_NO_ENC:
    {
        r = dfu_end_wo_enc(hdr->id);
        break;
    }
#endif
    default:
        r = DFU_FAIL;
        break;
    }
    return r;
}

