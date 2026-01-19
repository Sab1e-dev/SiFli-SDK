/* Includes ------------------------------------------------------------------*/
#include <rtconfig.h>
#include <string.h>
#include <stdio.h>
#include "bf0_hal.h"
#include "board.h"
#include "../dfu/dfu.h"


void boot_uart_tx(USART_TypeDef *uart, uint8_t *data, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        while ((uart->ISR & UART_FLAG_TXE) == 0);
        uart->TDR = (uint32_t)data[i];
    }
}

/**
* Initializes the Global MSP.
*/
#define BOOT_MODE_DELAY 100000
void HAL_MspInit(void)
{
    // TODO:
    __HAL_WDT_DISABLE();
#ifdef CFG_BOOTROM
    uint8_t uid[EFUSE_UID_BYTE_SIZE];
    int r;
    printf("SFBL\n");
    r = sifli_hw_efuse_read(EFUSE_UID, uid, EFUSE_UID_BYTE_SIZE);
    if (EFUSE_UID_BYTE_SIZE == r)
    {
        r--;
        for (; r >= 0; r--)
        {
            printf("%02X", uid[r]);
        }
        printf("\n");
    }

    HAL_Delay_us(BOOT_MODE_DELAY);      // Wait for boot_mode options.
#endif
}

void mpu_config(void)
{
    // Do nothing
}

void cache_enable(void)
{
    // Do nothing
}

