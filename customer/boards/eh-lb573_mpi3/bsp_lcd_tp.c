#include "bsp_board.h"

/*
    LCD's SPI_EN is HW connect to backlight by default,
    so power on both of them for convenience if LCD not need backlight
*/
// #define LCD_LCD_BACKLIGHT        (1)       // GPIO_A01
//#define LCD_VIO_EN                        // NO pin

#define LCD_RESET_PIN           (0)         // GPIO_A00

//#define TP_VCC_EN                     // NO pin
//#define TP_VIO_EN                     // NO pin

#define TP_RESET            (1)            // GPIO_A01

// #define LCD_TP_COM_POWER    (9)   // GPIO_A09

extern void BSP_GPIO_Set(int pin, int val, int is_porta);

void BSP_LCD_Reset(uint8_t high1_low0)
{
    BSP_GPIO_Set(LCD_RESET_PIN, high1_low0, 1);
}


void BSP_LCD_PowerDown(void)
{
    BSP_LCD_Reset(0);
}

void BSP_LCD_PowerUp(void)
{


}

void BSP_TP_PowerUp(void)
{
    BSP_GPIO_Set(TP_RESET, 1, 1);
}

void BSP_TP_PowerDown(void)
{
    BSP_GPIO_Set(TP_RESET, 0, 1);
}

void BSP_TP_Reset(uint8_t high1_low0)
{
    BSP_GPIO_Set(TP_RESET, high1_low0, 1);
}

