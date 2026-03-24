#include "sys.h"

#define SYSTICK_PER_US (SYSCLK_VALUE / 8000000)
#define SYSCLK_PERIOD_US (0xffffff / SYSTICK_PER_US)

static void sysclk_init()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        // error handler
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        // error handler
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        // error handler
    }
}

void sys_init()
{
    HAL_Init();
    sysclk_init();
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
}

#pragma region delay

static void _delay_us(uint32_t us)
{
    SysTick->LOAD = us * SYSTICK_PER_US;
    SysTick->VAL = 0x00;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    do
    {
        us = SysTick->CTRL;
    } while ((us & 0x01) && !(us & 0x10000));
    SysTick->CTRL = 0x00;
}

void delay_us(uint32_t us)
{
    uint32_t i = us / SYSCLK_PERIOD_US;
    us %= SYSCLK_PERIOD_US;
    while (i--)
        _delay_us(SYSCLK_PERIOD_US);
    if (us)
        _delay_us(us);
}

void HAL_Delay(uint32_t ms)
{
    uint32_t i = ms / 4294967;
    ms %= 4294967;
    while (i--)
        delay_us(4294967000U);
    if (ms)
        delay_us(1000 * ms);
}

void delay_s(uint32_t s)
{
    uint32_t i = s / 4294967;
    s %= 4294967;
    while (i--)
        delay_ms(4294967000U);
    if (s)
        delay_ms(1000 * s);
}

#pragma endregion
