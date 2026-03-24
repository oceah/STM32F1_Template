#include "gpio.h"

void gpio_init()
{
    GPIO_InitTypeDef GPIO_Init = {0};
    GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;

    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();

    // B6: I2C-SCl, B7-I2C-SDA
    GPIO_Init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_Init);

    // C13: LED
    GPIO_Init.Pin = GPIO_PIN_13;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_Init);
}
