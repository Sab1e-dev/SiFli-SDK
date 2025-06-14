#ifndef __SECBOOT_H__
#define __SECBOOT_H__

#define SPLIT_THRESHOLD     (256)

#define SECBOOT_SIGKEY_PUB_ERR      1
#define SECBOOT_IMG_HASH_SIG_ERR    2
#define SECBOOT_FTAB_HASH_VF_ERR    3
#define SECBOOT_BL_HASH_VF_ERR      4

extern void boot_uart_tx(USART_TypeDef *uart, uint8_t *data, int len);
void sifli_secboot_exception(uint8_t excpt);
int sifli_ftab_hash_verify(struct sec_configuration *ftab);
int sifli_bootloader_hash_verify(uint8_t *bl_img, uint32_t bl_size, uint8_t *bl_hash);
int sifli_sigkey_pub_verify(uint8_t *sigkey, uint32_t key_size);
int sifli_img_sig_hash_verify(uint8_t *img_hash_sig, uint8_t *sig_pub_key, uint8_t *image, uint32_t img_size);
int sifli_security_enabled(void);
void sifli_decrypt_ftab(uint8_t *ftab, uint32_t size);
void sifli_mpi_xip_dec(uint8_t coreid, uint32_t start, uint32_t len, uint8_t *key);
#endif /* __SECBOOT_H__ */

