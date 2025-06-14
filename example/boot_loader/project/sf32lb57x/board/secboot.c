#include <rtconfig.h>
#include <board.h>
#include <string.h>
#include <stdlib.h>
#include "register.h"
#include "../dfu/dfu.h"
#ifdef PKG_SIFLI_MBEDTLS_BOOT
    #include "mbedtls/cipher.h"
    #include "mbedtls/pk.h"
#endif
#include "secboot.h"
#include "boot_flash.h"

/* out buf size must more than 32 byte */
int sifli_hash_calculate(uint8_t *in, uint32_t in_size, uint8_t *out, uint8_t algo)
{
    if (!in || !in_size || !out || algo > 3)
        return -1;

    HAL_HASH_reset();
    HAL_HASH_init(NULL, algo, 0);
    HAL_HASH_run(in, in_size, 1);
    HAL_HASH_result(out);

    return 0;
}

int sifli_hash_verify(uint8_t *data, uint32_t data_size, uint8_t *hash, uint32_t hash_size)
{
    uint8_t hash_out[32] = {0};

    if (!data || !hash)
        return -1;

    if (sifli_hash_calculate(data, data_size, hash_out, HASH_ALGO_SHA256))
        return -1;

    if (memcmp(hash_out, hash, hash_size))
        return -1;

    return 0;
}

int sifli_sigkey_pub_verify(uint8_t *sigkey, uint32_t key_size)
{
    uint32_t size = 0;

    uint8_t sigkey_hash[DFU_SIG_HASH_SIZE] = {0};
    size = sifli_hw_efuse_read(EFUSE_ID_SIG_HASH, sigkey_hash, DFU_SIG_HASH_SIZE);
    if (size == DFU_SIG_HASH_SIZE)
        return sifli_hash_verify(sigkey, key_size, sigkey_hash, DFU_SIG_HASH_SIZE);
    else
        return -1;
}

int sifli_security_enabled(void)
{
    uint8_t sec_en = 0, size = 0;

    size = sifli_hw_efuse_read(EFUSE_ID_SECURE_ENABLED, &sec_en, DFU_SECURE_SIZE);
    if (size == DFU_SECURE_SIZE)
    {
        sec_en = sec_en & 0xF;
        if ((sec_en == 0x1) || (sec_en == 0xF))
            return 1;
    }

    return 0;
}

static void sifli_create_ftab_key(uint8_t *key_out)
{
    ALIGN(4)
    static uint8_t key_in[DFU_KEY_SIZE];

    /*use uid+uid --> rootkey+uid(iv) --> ftab_key*/
    sifli_hw_efuse_read(EFUSE_UID, key_in, DFU_UID_SIZE);
    memcpy(&key_in[DFU_UID_SIZE], key_in, DFU_UID_SIZE);
    HAL_AES_init(NULL, DFU_KEY_SIZE, (uint32_t *)key_in, AES_MODE_CBC);
    HAL_AES_run(AES_ENC, key_in, key_out, DFU_KEY_SIZE);
}

void sifli_decrypt_ftab(uint8_t *ftab, uint32_t size)
{
    ALIGN(4)
    static uint8_t ftab_key[DFU_KEY_SIZE];

    sifli_create_ftab_key(ftab_key);
    sifli_hw_dec_once(ftab_key, ftab, ftab, size);
}

int sifli_ftab_hash_verify(struct sec_configuration *ftab)
{
    uint8_t ftab_hash[DFU_FTAB_HASH_SIZE] = {0};

    memcpy(ftab_hash, ftab->ftab_hash, DFU_FTAB_HASH_SIZE);
    memset(ftab->ftab_hash, 0, DFU_FTAB_HASH_SIZE);
    return sifli_hash_verify((uint8_t *)ftab, sizeof(struct sec_configuration), ftab_hash, DFU_FTAB_HASH_SIZE);
}

int sifli_bootloader_hash_verify(uint8_t *bl_img, uint32_t bl_size, uint8_t *bl_hash)
{
    if (!bl_img || !bl_size || !bl_hash)
        return -1;

    return sifli_hash_verify(bl_img, bl_size, bl_hash, DFU_IMG_HASH_SIZE);
}

#ifdef PKG_SIFLI_MBEDTLS_BOOT
int sifli_img_sig_hash_verify(uint8_t *img_hash_sig, uint8_t *sig_pub_key, uint8_t *image, uint32_t img_size)
{
    uint8_t img_hash[32] = {0};
    mbedtls_pk_context pk;

    /*1.calculate image hash*/
    if (sifli_hash_calculate(image, img_size, img_hash, HASH_ALGO_SHA256))
        return -1;

    /*2.verify image hash digital signature*/
    mbedtls_pk_init(&pk);
    if (mbedtls_pk_parse_public_key(&pk, sig_pub_key, DFU_SIG_KEY_SIZE))
        return -1;
    mbedtls_rsa_set_padding((mbedtls_rsa_context *)pk.pk_ctx, MBEDTLS_RSA_PKCS_V15, MBEDTLS_MD_SHA256);
    if (mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, img_hash, DFU_IMG_HASH_SIZE, img_hash_sig, DFU_SIG_SIZE))
        return -1;

    return 0;
}
#endif

static void sifli_mpi_xip_dec_cfg(uint32_t start, uint32_t len, uint8_t *key)
{
    HAL_SECU_KeySet(SECU_KEY0, (uint32_t *)key);

    if (boot_handle->Instance == hwp_mpi2)
    {
        HAL_SECU_MpiKey_Select(SECU_MPI2_KEY0, SECU_KEY0);
    }
    else if (boot_handle->Instance == hwp_mpi3)
    {
        HAL_SECU_MpiKey_Select(SECU_MPI3_KEY0, SECU_KEY0);
    }
    HAL_FLASH_NONCE_CFG(boot_handle, start, start + len, dfu_get_counter(0));
    HAL_FLASH_AES_CFG(boot_handle, 1); /* enable on-the-fly decoder */
}

static void sifli_mpi_xip_dec2_cfg(uint32_t start, uint32_t len, uint8_t *key)
{
    HAL_SECU_KeySet(SECU_KEY1, (uint32_t *)key);

    if (boot_handle->Instance == hwp_mpi2)
    {
        HAL_SECU_MpiKey_Select(SECU_MPI2_KEY1, SECU_KEY1);
    }
    else if (boot_handle->Instance == hwp_mpi3)
    {
        HAL_SECU_MpiKey_Select(SECU_MPI3_KEY1, SECU_KEY1);
    }
    HAL_FLASH_NONCE_CFG2(boot_handle, start, start + len, dfu_get_counter(0));
    HAL_FLASH_AES_CFG(boot_handle, 1); /* enable on-the-fly decoder */
}

void sifli_mpi_xip_dec(uint8_t coreid, uint32_t start, uint32_t len, uint8_t *key)
{
    if (coreid == CORE_HCPU)
    {
        sifli_mpi_xip_dec_cfg(start, len, key);
    }
    else //CORE_BL
    {
        sifli_mpi_xip_dec2_cfg(start, len, key);
    }
}

void sifli_secboot_exception(uint8_t excpt)
{
    char *err = NULL;

    switch (excpt)
    {
    case SECBOOT_SIGKEY_PUB_ERR:
        err = "K";
        break;
    case SECBOOT_IMG_HASH_SIG_ERR:
        err = "I";
        break;
    case SECBOOT_FTAB_HASH_VF_ERR:
        err = "F";
        break;
    case SECBOOT_BL_HASH_VF_ERR:
        err = "B";
        break;
    default:
        err = "N";
        break;
    }
    boot_uart_tx(hwp_usart1, (uint8_t *)err, strlen(err));

    HAL_sw_breakpoint();
}
