/* Includes ------------------------------------------------------------------*/
#include <rtconfig.h>
#include "board.h"
#include "string.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void boot_uart_tx(USART_TypeDef *uart, uint8_t *data, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        while ((uart->ISR & UART_FLAG_TXE) == 0);
        uart->TDR = (uint32_t)data[i];
    }
}

#define MAX_RETRY 3
void boot_error(unsigned char code)
{
    int retry;

    boot_uart_tx(hwp_usart1, &code, 1);

    retry = hwp_pmuc->CAU_RSVD & MAX_RETRY;
    retry++;
    if (retry <= MAX_RETRY)
    {
        hwp_pmuc->CAU_RSVD &= ~MAX_RETRY;
        hwp_pmuc->CAU_RSVD |= retry;
        HAL_PMU_Reboot();
    }
    else
    {
        while (1);
    }
}

/**
* Initializes the Global MSP.
*/
#ifdef TARMAC
    #define BOOT_MODE_DELAY 1000
#else
    #define BOOT_MODE_DELAY 1000000
#endif
void HAL_MspInit(void)
{
    // TODO:
    // __HAL_WDT_DISABLE();
    __HAL_IWDT_DISABLE();
#ifdef CFG_BOOTROM
    char *boot_tag = "SFBL\n";
    boot_uart_tx(hwp_usart1, (uint8_t *)boot_tag, strlen(boot_tag));
    HAL_RCC_HCPU_ClockSelect(RCC_CLK_MOD_SYS, RCC_SYSCLK_HXT48);
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


/* USER CODE BEGIN 1 */


/* USER CODE END 1 */


