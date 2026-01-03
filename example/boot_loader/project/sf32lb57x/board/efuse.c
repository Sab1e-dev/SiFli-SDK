#include "rtconfig.h"
#include "stdint.h"
#include "string.h"
#include "bf0_hal.h"
#include "board.h"
#include "../dfu/dfu.h"

#define AES_BLOCK_SIZE 512

ALIGN(4)
uint8_t g_uid[DFU_UID_SIZE];
uint8_t root_key[DFU_KEY_SIZE];
ALIGN(4)
static uint8_t g_aes_ctr_iv[DFU_IV_LEN];
dfu_efuse_read_hook_t g_dfu_efuse_read_hook;

int sifli_hw_efuse_write(uint8_t id, uint8_t *data, int size)
{
    int r = 0;
    switch (id)
    {
    case EFUSE_UID:
        r = HAL_EFUSE_Write2(EFUSE_UID_OFFSET, data, EFUSE_UID_SIZE);
        break;
    case DFU_CONFIG_PKGID:
        r = HAL_EFUSE_Write2(EFUSE_PKGID_OFFSET, data, EFUSE_PKGID_SIZE);
        break;
    case EFUSE_ID_ROOT:
        r = HAL_EFUSE_Write2(EFUSE_ROOTKEY_OFFSET, data, EFUSE_ROOTKEY_SIZE);
        break;
    case EFUSE_ID_SECURE_ENABLED:
    {
        r = HAL_EFUSE_Write2(EFUSE_SECEN_OFFSET, data, EFUSE_SECEN_SIZE);
    }
    break;
    default:
        r = 0;
    }
    return r;
}


int sifli_hw_efuse_read(uint8_t id, uint8_t *data, int size)
{
    int r;
    if (g_dfu_efuse_read_hook)
    {
        r = g_dfu_efuse_read_hook(id, data, size);
        if (r != 0)
            return r;
    }

    if (id == EFUSE_UID)
    {
        r = HAL_EFUSE_Read(EFUSE_UID_OFFSET, data, DFU_UID_SIZE);
    }
    else if (id == EFUSE_ID_ROOT)
    {
        r = HAL_EFUSE_Read(EFUSE_ROOTKEY_OFFSET, data, DFU_KEY_SIZE);
    }
    else if (id == EFUSE_ID_SECURE_ENABLED)
    {
        r = HAL_EFUSE_Read2(EFUSE_SECEN_OFFSET, data, EFUSE_SECEN_SIZE);
    }
    else
        r = 0;
    return r;

}

int sifli_hw_efuse_read_bank(uint32_t i)
{
    static uint8_t data[HAL_EFUSE_BANK_SIZE];
    int32_t r = 0;
    int size;
    if (i >= HAL_EFUSE_BANK_NUM)
        return -1;

    size = HAL_EFUSE_Read(HAL_EFUSE_BANK_SIZE * i * 8, data, HAL_EFUSE_BANK_SIZE);;
    if (size == 0)
        r = -2;

    return r;
}

int sifli_hw_dec(uint8_t *key, uint8_t *in_data, uint8_t *out_data, int size, uint32_t init_offset)
{
    uint32_t offset = 0;


    if (g_dfu_efuse_read_hook && !key)
    {
        g_dfu_efuse_read_hook(EFUSE_ID_ROOT, root_key, DFU_KEY_SIZE);
        key = &root_key[0];
    }

    static uint8_t temp[AES_BLOCK_SIZE];
    memset(temp, 0, AES_BLOCK_SIZE);
    while (offset < size)
    {
        int len = (size - offset) < AES_BLOCK_SIZE ? (size - offset) : AES_BLOCK_SIZE;
        memcpy(temp, in_data + offset, len);
        HAL_AES_init((uint32_t *)key, DFU_KEY_SIZE, (uint32_t *)dfu_get_counter(init_offset + offset), AES_MODE_CTR);
        HAL_AES_run(AES_DEC, temp, out_data + offset, len);
        offset += len;
    }
    return offset;
}

void sifli_hw_init_xip_key(uint8_t *enc_img_key)
{
    uint8_t *uid;

    /* enable dedicated mode for image key decryption */
    static uint32_t plain_key[DFU_KEY_SIZE >> 2];

    //TODO:
    __HAL_SYSCFG_SET_SECURITY();
    uid = &g_uid[0];
    sifli_hw_efuse_read(EFUSE_UID, uid, DFU_UID_SIZE);
    memset(plain_key, 0, sizeof(plain_key));
    HAL_AES_init(NULL, DFU_KEY_SIZE, (uint32_t *)uid, AES_MODE_CBC);
    HAL_AES_run(AES_DEC, enc_img_key, (uint8_t *)plain_key, DFU_KEY_SIZE);
    /* restore to normal mode */
    //TODO:
    __HAL_SYSCFG_CLEAR_SECURITY();
}

int sifli_hw_dec_key(uint8_t *in_data, uint8_t *out_data, int size)
{
    uint8_t *uid;
    uint8_t *key = NULL;

    uid = &g_uid[0];
    sifli_hw_efuse_read(EFUSE_UID, uid, DFU_UID_SIZE);
    HAL_AES_init((uint32_t *)key, DFU_KEY_SIZE, (uint32_t *)uid, AES_MODE_CBC);
    HAL_AES_run(AES_DEC, in_data, out_data, DFU_KEY_SIZE);

    return 0;
}

uint8_t *dfu_get_counter(uint32_t offset)
{
    int i;
    sifli_hw_efuse_read(EFUSE_ID_SIG_HASH, g_aes_ctr_iv, DFU_SIG_HASH_SIZE);
    memset(&g_aes_ctr_iv[8], 0, 8);
    offset >>= 4;   // Counter is increased every 16 bytes
    for (i = 15; i >= 12 && offset > 0; i--, offset >>= 8)
    {
        g_aes_ctr_iv[i] = offset & 0xff;
    }
    return g_aes_ctr_iv;
}

void sifli_hw_enc(uint8_t *in_data, uint8_t *out_data, int size)
{
    uint8_t *key = NULL;

    if (g_dfu_efuse_read_hook)
    {
        g_dfu_efuse_read_hook(EFUSE_ID_ROOT, root_key, DFU_KEY_SIZE);
        key = &root_key[0];
    }

#ifdef HAL_AES_MODULE_ENABLED
    uint8_t *uid;

    uid = &g_uid[0];
    sifli_hw_efuse_read(EFUSE_UID, uid, DFU_UID_SIZE);
    HAL_AES_init((uint32_t *)key, DFU_KEY_SIZE, (uint32_t *)uid, AES_MODE_CBC);
    HAL_AES_run(AES_ENC, in_data, out_data, DFU_KEY_SIZE);
#else

    uint8_t *uid;

    uid = &g_uid[0];
    sifli_hw_efuse_read(EFUSE_UID, uid, DFU_UID_SIZE);

    static mbedtls_aes_context ctx;
    static uint8_t stream_block[16];
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_enc(&ctx, key, DFU_KEY_SIZE * 8);

    mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, size, &uid[0], in_data, out_data);
#endif
}


