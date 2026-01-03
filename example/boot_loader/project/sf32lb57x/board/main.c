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
#include "stdlib.h"
#include "register.h"
#include "../dfu/dfu.h"
#include "boot_flash.h"
#include "secboot.h"
#include "../dfu/dfu_protocol.h"

#define CMD_BUF_SIZE 32
#define CMD_ARG_MAX 3

typedef struct
{
    uint8_t pos;
    char buf[CMD_BUF_SIZE];
} cmd_buf_t;

uint32_t rx_data_buf_len(void);
uint32_t rx_data_buf_get(uint8_t *buf, uint32_t len);

static cmd_buf_t cmd_buf;
static uint8_t dfu_data[DFU_MAX_BLK_SIZE];

void HAL_MspInit(void);

void cdc_acm_data_send_with_dtr_test(uint8_t *buf, uint32_t len);
extern void cdc_acm_init(uint8_t busid, uintptr_t base);

extern flash_read_func g_flash_read;
extern flash_write_func g_flash_write;
extern flash_erase_func g_flash_erase;
struct sec_configuration g_temp_sec_config;
struct sec_configuration *temp_sec_config;

ALIGN(4) struct sec_configuration sec_config_cache;

typedef void (*ram_hook_handler)(void);
void boot_ram(void)
{
    volatile ram_hook_handler hook = (volatile ram_hook_handler)hwp_hpsys_aon->RESERVE0;
    if (hook)
        hook();
}


//#define BOOT_TEST
#ifdef BOOT_TEST
void boot_test(void)
{
    uint32_t delay = (100 << BOOT_PU_Delay_Pos) | (200 << BOOT_PD_Delay_Pos);
    if (HAL_Get_backup(RTC_BACKUP_BOOTOPT + 1) == 0)
    {
        HAL_Set_backup(RTC_BACKUP_BOOTOPT, delay);
        HAL_Set_backup(RTC_BACKUP_BOOTOPT + 1, 1);
        HAL_PMU_Reboot();
    }
    else
    {
        boot_uart_tx(hwp_usart1, (uint8_t *)"E", 1);
        __asm("B .");
    }
}
#else
#define boot_test()
#endif

/**************************Efuse**************************************************/
#define boot_efuse_init_stage1(void) \
{ \
    hwp_efusec->TCR = 0x1DF; \
    /* Read bank0 */ \
    sifli_hw_efuse_read_bank(0); \
}

//TODO:
#define boot_efuse_init_stage2(void) \
{ \
    /* Read bank3 */ \
    sifli_hw_efuse_read_bank(3); \
}


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
    struct sec_configuration *sec_config = &sec_config_cache;

    if (img_hdr->flags & DFU_FLAG_ENC)
    {
        uint32_t is_flash = 1;

        /* verify public sig_key hash */
        if (sifli_sigkey_pub_verify(sec_config->sig_pub_key, DFU_SIG_KEY_SIZE))
            sifli_secboot_exception(SECBOOT_SIGKEY_PUB_ERR);

        if (coreid < 2 * CORE_MAX)
        {
            coreid %= CORE_MAX;
            // Read Root key
            boot_efuse_init_stage2();
            if (coreid == CORE_HCPU || coreid == CORE_BL || coreid == CORE_LCPU)
            {
                ALIGN(4)
                static uint8_t dfu_key[DFU_KEY_SIZE];
                /** key in plaintext */
                ALIGN(4)
                static uint8_t dfu_key1[DFU_KEY_SIZE];
                if (is_addr_in_nor(dest))
                {
                    memcpy(dfu_key, img_hdr->key, sizeof(dfu_key));
                    sifli_hw_init_xip_key(dfu_key);

                    // Setup XIP for decoding and running
                    HAL_FLASH_NONCE_CFG(boot_handle, dest, dest + img_hdr->length, dfu_get_counter(0));
                    if (is_flash)
                        HAL_FLASH_ALIAS_CFG(boot_handle, dest, img_hdr->length, src - dest);
                    HAL_FLASH_AES_CFG(boot_handle, 1);          /* enable on-the-fly decoder */
                }
                else
                {
                    /* copy encrypted key to ram as AES_ACC cannot access flash */
                    memcpy(dfu_key, img_hdr->key, sizeof(dfu_key));
                    sifli_hw_dec_key(dfu_key, dfu_key1, sizeof(dfu_key1));
                    g_flash_read(src, (const int8_t *)dest, img_hdr->length);
                    sifli_hw_dec(dfu_key1, (uint8_t *)dest, (uint8_t *)dest, img_hdr->length, 0);
                }
#ifdef PKG_SIFLI_MBEDTLS_BOOT
                /* verify image hash signature */
                if (sifli_img_sig_hash_verify(img_hdr->sig, sec_config->sig_pub_key, (uint8_t *)dest, img_hdr->length))
                    sifli_secboot_exception(SECBOOT_IMG_HASH_SIG_ERR);
#endif
                run_img(dest);
            }
        }
    }

    if (img_hdr->length && (src != dest)
            && (is_addr_in_nor((uint32_t)dest) == 0))
    {
        g_flash_read(src, (const int8_t *)dest, img_hdr->length);
    }

    if (coreid < 2 * CORE_MAX)
    {
        coreid %= CORE_MAX;
        if (coreid == CORE_HCPU || coreid == CORE_BL || coreid == CORE_LCPU)
        {
            if (is_addr_in_nor(dest))
            {
                HAL_FLASH_ALIAS_CFG(boot_handle, dest, img_hdr->length, src - dest);
            }

            run_img(dest);
        }
    }
}

#ifndef CFG_BOOTROM
static void update_sec_flash(struct sec_configuration *sec_config)
{
    uint32_t flash_addr = g_config_addr;
    //uint32_t size = (sizeof(struct sec_configuration) + SPI_NAND_PAGE_SIZE * 2) & (~(SPI_NAND_PAGE_SIZE * 2 - 1));
    uint32_t size = sizeof(struct sec_configuration);
    if (size % 4096 != 0)
    {
        size = (size + 4096) / 4096 * 4096;
    }
    g_flash_erase(flash_addr, size);
    g_flash_read(flash_addr, (const int8_t *)sec_config, sizeof(struct sec_configuration));
}

static void select_boot()
{
    uint8_t hcpu_des = HAL_Get_backup(RTC_BACKUP_NAND_OTA_DES);
    uint32_t hcpu_len = HAL_Get_backup(RTC_BAKCUP_OTA_FORCE_MODE);
    temp_sec_config = &g_temp_sec_config;

    struct sec_configuration *flash_config = (struct sec_configuration *)g_config_addr;
    //memcpy((const int8_t *)temp_sec_config, (uint8_t *)MPI5_MEM_BASE, sizeof(struct sec_configuration));
    g_flash_read(g_config_addr, (const int8_t *)temp_sec_config, sizeof(struct sec_configuration));

    int hcpu1_img_idx = DFU_FLASH_IMG_IDX(DFU_FLASH_IMG_HCPU);
    int hcpu2_img_idx = DFU_FLASH_IMG_IDX(DFU_FLASH_IMG_HCPU2);
    // HAL_sw_breakpoint();

    if (sec_config_cache.running_imgs[CORE_HCPU] == &(flash_config->imgs[hcpu1_img_idx]))
    {
        // now we are running on hcpu1
        switch (hcpu_des)
        {
        case DFU_DES_RUNNING_ON_HCPU1:
        {
            // normal power on hcpu1
            break;
        }
        case DFU_DES_SWITCH_TO_HCPU2:
        {
            //HAL_sw_breakpoint();
            // switch to hcpu2

            sec_config_cache.running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu2_img_idx]);

            hcpu_des = DFU_DES_UPDATE_HCPU2;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);

            temp_sec_config->running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu2_img_idx]);
            if (hcpu_len != 0)
            {
                temp_sec_config->imgs[DFU_FLASH_IMG_IDX(DFU_FLASH_IMG_HCPU2)].length = hcpu_len;
            }

            update_sec_flash(temp_sec_config);
            break;
        }
        case DFU_DES_UPDATE_HCPU2:
        {
            // switch to hcpu2 fail, back to hcpu1
            sec_config_cache.running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu1_img_idx]);
            hcpu_des = DFU_DES_RUNNING_ON_HCPU1;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);

            temp_sec_config->running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu1_img_idx]);
            update_sec_flash(temp_sec_config);
            break;
        }
        case DFU_DES_NONE:
        {
            // first power on, use current
            hcpu_des = DFU_DES_RUNNING_ON_HCPU1;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);
            break;
        }
        default:
            hcpu_des = DFU_DES_RUNNING_ON_HCPU1;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);
        }

    }
    else if (sec_config_cache.running_imgs[CORE_HCPU] == &(flash_config->imgs[hcpu2_img_idx]))
    {
        // now we are running on hcpu2
        switch (hcpu_des)
        {
        case DFU_DES_RUNNING_ON_HCPU2:
        {
            // normal power on hcpu2
            break;
        }
        case DFU_DES_SWITCH_TO_HCPU1:
        {
            // switch to hcpu1
            sec_config_cache.running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu1_img_idx]);
            hcpu_des = DFU_DES_UPDATE_HCPU1;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);

            temp_sec_config->running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu1_img_idx]);
            if (hcpu_len != 0)
            {
                temp_sec_config->imgs[DFU_FLASH_IMG_IDX(DFU_FLASH_IMG_HCPU)].length = hcpu_len;
            }

            update_sec_flash(temp_sec_config);
            break;
        }
        case DFU_DES_UPDATE_HCPU1:
        {
            // switch to hcpu1 fail, back to hcpu2
            sec_config_cache.running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu2_img_idx]);
            hcpu_des = DFU_DES_RUNNING_ON_HCPU2;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);

            temp_sec_config->running_imgs[CORE_HCPU] = &(flash_config->imgs[hcpu2_img_idx]);
            update_sec_flash(temp_sec_config);
            break;
        }
        case DFU_DES_NONE:
        {
            // first power on, use current
            hcpu_des = DFU_DES_RUNNING_ON_HCPU2;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);
            break;
        }
        default:
            hcpu_des = DFU_DES_RUNNING_ON_HCPU2;
            HAL_Set_backup(RTC_BACKUP_NAND_OTA_DES, hcpu_des);
        }
    }
}
#endif /* CFG_BOOTROM */

void boot_images_help()
{
    if (sec_config_cache.magic == SEC_CONFIG_MAGIC)
    {
#ifdef CFG_BOOTROM
        if (sec_config_cache.running_imgs[CORE_BL] != (struct image_header_enc *)FLASH_UNINIT_32)
        {
            int flash_id = ((uint32_t)sec_config_cache.running_imgs[CORE_BL] - g_config_addr - 0x1000) / sizeof(struct image_header_enc)
                           + DFU_FLASH_IMG_LCPU;
            dfu_boot_img_in_flash(flash_id);
        }
#else

        dfu_install_info info = {0};
        dfu_install_info info_ext = {0};

        if (DFU_DOWNLOAD_REGION_START_ADDR != FLASH_UNINIT_32)
        {
            g_flash_read(DFU_DOWNLOAD_REGION_START_ADDR, (const int8_t *)&info, sizeof(dfu_install_info));
        }
        if (DFU_INFO_REGION_START_ADDR != FLASH_UNINIT_32)
        {
            g_flash_read(DFU_INFO_REGION_START_ADDR, (const int8_t *)&info_ext, sizeof(dfu_install_info));
        }
        if (info.magic == SEC_CONFIG_MAGIC && info_ext.magic == SEC_CONFIG_MAGIC)
        {
            info = info_ext;
        }

        select_boot();

        if (DFU_DOWNLOAD_REGION_START_ADDR != FLASH_UNINIT_32)
        {
            if ((HAL_Get_backup(RTC_BAKCUP_OTA_FORCE_MODE) == DFU_FORCE_MODE_REBOOT_TO_PACKAGE_OTA_MANAGER) ||
                    (info.magic == SEC_CONFIG_MAGIC) && (info.install_state == DFU_PACKAGE_INSTALL))
            {
                sec_config_cache.running_imgs[CORE_HCPU] = (struct image_header_enc *) & (((struct sec_configuration *)FLASH_TABLE_START_ADDR)->imgs[DFU_FLASH_IMG_IDX(DFU_FLASH_IMG_LCPU)]);
            }
        }

        board_init_psram();
        /* load extended img if present, such as ACPU img */
        if (sec_config_cache.imgs[DFU_FLASH_IMG_IDX(DFU_FLASH_HCPU_EXT2)].length != FLASH_UNINIT_32)
        {
            dfu_boot_img_in_flash(DFU_FLASH_HCPU_EXT2);
        }

        if (sec_config_cache.running_imgs[CORE_HCPU] != (struct image_header_enc *)FLASH_UNINIT_32)
        {
            int flash_id = ((uint32_t)sec_config_cache.running_imgs[CORE_HCPU] - g_config_addr - 0x1000) / sizeof(struct image_header_enc) + DFU_FLASH_IMG_LCPU;

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

#ifdef __CC_ARM                                                 /*ARMCC*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) /*ARMCLANG*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#elif __ICCARM__                                                /*IAR**/
#error Not support yet
#else                                                           /*GCC*/
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
int _write(int fd, char *ptr, int len)
{
    if (fd > 2)
    {
        return -1;
    }
    boot_uart_tx(hwp_usart1, (uint8_t *)ptr, len);
    return len;
}
#endif

#ifdef PKG_USING_CHERRYUSB

PUTCHAR_PROTOTYPE
{
    boot_uart_tx(hwp_usart1, (uint8_t *)&ch, 1);
    return ch;
}

static uint32_t cmd_split(char *cmd, uint32_t length, char *argv[CMD_ARG_MAX])
{
    char *ptr;
    uint32_t position;
    uint32_t argc;
    uint32_t i;

    ptr = cmd;
    position = 0;
    argc = 0;

    while (position < length)
    {
        /* strip bank and tab */
        while ((*ptr == ' ' || *ptr == '\t') && position < length)
        {
            *ptr = '\0';
            ptr ++;
            position ++;
        }

        if (argc >= CMD_ARG_MAX)
        {
            printf("Too many args ! We only Use:\n");
            for (i = 0; i < argc; i++)
            {
                printf("%s ", argv[i]);
            }
            printf("\n");
            break;
        }

        if (position >= length) break;

        /* handle string */
        if (*ptr == '"')
        {
            ptr ++;
            position ++;
            argv[argc] = ptr;
            argc ++;

            /* skip this string */
            while (*ptr != '"' && position < length)
            {
                if (*ptr == '\\')
                {
                    if (*(ptr + 1) == '"')
                    {
                        ptr ++;
                        position ++;
                    }
                }
                ptr ++;
                position ++;
            }
            if (position >= length) break;

            /* skip '"' */
            *ptr = '\0';
            ptr ++;
            position ++;
        }
        else
        {
            argv[argc] = ptr;
            argc ++;
            while ((*ptr != ' ' && *ptr != '\t') && position < length)
            {
                ptr ++;
                position ++;
            }
            if (position >= length) break;
        }
    }

    return argc;
}

int32_t handle_cmd_dfu_recv(uint32_t argc, char **argv)
{
    int r = DFU_FAIL;
    int32_t offset = 0;
    int32_t delta;

    if (argc == 2)
    {
        int len = atoi(argv[1]);
        if (len < DFU_MAX_BLK_SIZE)
        {
            while (offset < len)
            {
                delta = rx_data_buf_get(&dfu_data[offset], len - offset);
                offset += delta;
            }
            r = dfu_receive_pkt(len, dfu_data);
        }
    }
    if (r == DFU_SUCCESS)
    {
        cdc_acm_data_send_with_dtr_test("OK\n", 3);
    }
    else
    {
        cdc_acm_data_send_with_dtr_test("Fail\n", 5);
    }

    return r;

}


void handle_cmd(char *cmd, uint32_t len)
{
    char *argv[CMD_ARG_MAX];
    uint32_t argc;

    if (len == 0)
    {
        return;
    }

    /* strim the beginning of command */
    while ((*cmd  == ' ') || (*cmd == '\t'))
    {
        cmd++;
        len--;
    }

    if (len == 0)
    {
        return;
    }

    memset(argv, 0x0, sizeof(argv));
    argc = cmd_split(cmd, len, argv);
    if (argc == 0)
    {
        return;
    }

    if (strncmp((const char *)argv[0], "dfu_recv", 8) == 0)
    {
        handle_cmd_dfu_recv(argc, argv);
    }
}

void handle_rx_data(void)
{
    char ch;
    uint32_t len;

    while (1)
    {
        len = rx_data_buf_get((uint8_t *)&ch, 1);
        if (len == 0)
        {
            break;
        }
        if ((ch == '\r') || (ch == '\n'))
        {
            handle_cmd(cmd_buf.buf, cmd_buf.pos);
            memset(cmd_buf.buf, 0, CMD_BUF_SIZE);
            cmd_buf.pos = 0;
        }
        if (cmd_buf.pos >= CMD_BUF_SIZE)
        {
            cmd_buf.pos = 0;
        }
        if (((ch >= 'a') && (ch <= 'z'))
                || ((ch >= 'A') && (ch <= 'Z'))
                || ((ch >= '0') && (ch <= '9'))
                || (ch == ' ') || (ch == '\t') || (ch == '_'))
        {
            cmd_buf.buf[cmd_buf.pos] = ch;
            cmd_buf.pos++;
            /* leave one byte for null terminator */
            if (cmd_buf.pos >= CMD_BUF_SIZE)
            {
                cmd_buf.pos = 0;
            }
        }
    }
}
#endif /* PKG_USING_CHERRYUSB */

#ifdef CFG_BOOTROM
static bool boot_is_bootmode(void)
{
    bool is_bootmode;
    GPIO_PinState pin_state;

    pin_state = HAL_GPIO_ReadPin(hwp_gpio1, BOARD_BOOT_MODE_PIN);

    if ((pin_state == GPIO_PIN_SET) || (hwp_hpsys_cfg->BMR))
    {
        is_bootmode = true;
    }
    else
    {
        is_bootmode = false;
    }

    return is_bootmode;
}
#endif /* CFG_BOOTROM */


#if defined(__CC_ARM) || defined(__CLANG_ARM)
    int main(void)
#elif defined(__ICCARM__)
    int __low_level_init(void)
#elif defined(__GNUC__)
    int entry(void)
#endif
{
    HAL_Delay_us(0);

    //TODO: need to be removed
    // board_pinmux_uart();

    // 3. Power on flash.
    board_flash_power_on();

    // 4. Check boot mode.
    HAL_MspInit();

    // 5. Boot images
#ifdef CFG_BOOTROM
    if (!boot_is_bootmode())
#endif
    {
        // 6. Read boot options
        board_boot_device = board_boot_from();

        /* init AES_ACC as normal mode */
        __HAL_SYSCFG_CLEAR_SECURITY();
        boot_device_init();
        boot_images_help();
    }

    //TODO:
    if (RCC_SYSCLK_HRC48 == HAL_RCC_HCPU_GetClockSrc(RCC_CLK_MOD_SYS))
    {
        HAL_HPAON_EnableXT48();
        HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_SYS, RCC_SYSCLK_HXT48);
    }

    dfu_init();

#ifdef PKG_USING_CHERRYUSB
    cdc_acm_init(0, (uintptr_t)USBC_BASE);
#endif /* PKG_USING_CHERRYUSB */
    while (1)
    {
#ifdef PKG_USING_CHERRYUSB
        if (rx_data_buf_len() > 0)
        {
            handle_rx_data();
        }
#endif /* PKG_USING_CHERRYUSB */
    }

    return HAL_OK;
}


