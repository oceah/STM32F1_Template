#ifndef _SYS_H
#define _SYS_H

#include "stm32f1xx_hal.h"

#define SYSCLK_VALUE 72000000U

typedef volatile uint32_t GPIO_PinType;

#define GPIO_Pin(r, n) \
    *(GPIO_PinType *)(((uintptr_t)(r) & 0xf0000000) + 0x2000000 + (((uintptr_t)(r) & 0xfffff) << 5) + ((uint32_t)(n) << 2))

#define PAin(n)  GPIO_Pin(&GPIOA->IDR, n)
#define PBin(n)  GPIO_Pin(&GPIOB->IDR, n)
#define PCin(n)  GPIO_Pin(&GPIOC->IDR, n)
#define PDin(n)  GPIO_Pin(&GPIOD->IDR, n)
#define PEin(n)  GPIO_Pin(&GPIOE->IDR, n)
#define PFin(n)  GPIO_Pin(&GPIOF->IDR, n)
#define PGin(n)  GPIO_Pin(&GPIOG->IDR, n)

#define PAout(n) GPIO_Pin(&GPIOA->ODR, n)
#define PBout(n) GPIO_Pin(&GPIOB->ODR, n)
#define PCout(n) GPIO_Pin(&GPIOC->ODR, n)
#define PDout(n) GPIO_Pin(&GPIOD->ODR, n)
#define PEout(n) GPIO_Pin(&GPIOE->ODR, n)
#define PFout(n) GPIO_Pin(&GPIOF->ODR, n)
#define PGout(n) GPIO_Pin(&GPIOG->ODR, n)

#ifdef __cplusplus
extern "C" {
#endif

void sys_init();

void delay_us(uint32_t us);
#define delay_ms(ms) HAL_Delay(ms)
void delay_s(uint32_t s);

#ifdef __cplusplus
}
#endif

#endif
