#include "uart.h"

#if USE_UART1
UART_HandleTypeDef huart1 = {0};
void USART1_IRQHandler()
{
    HAL_UART_IRQHandler(&huart1);
}

#if USE_DMA_UART1_TX
static DMA_HandleTypeDef hdma_uart1_tx = {0};
void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_uart1_tx);
}
#endif // USE_DMA_UART1_TX

#if UART1_RXBUF_SIZE
static uint8_t UART1_RxBuf[UART1_RXBUF_SIZE];
static uint8_t UART1_RxStatus = 0;
static void *UART1_pRxData = NULL;
static uint16_t UART1_RxSize = 0;

#if USE_DMA_UART1_RX
DMA_HandleTypeDef hdma_uart1_rx = {0};
void DMA1_Channel5_IRQHandler()
{
    HAL_DMA_IRQHandler(&hdma_uart1_rx);
}
#endif // USE_DMA_UART1_RX

#endif // UART1_RXBUF_SIZE

#endif // USE_UART1

void uart_init()
{
#if USE_UART1
    __USART1_CLK_ENABLE();
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        // error handler
    }
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

#if USE_DMA_UART1_TX
    __DMA1_CLK_ENABLE();
    hdma_uart1_tx.Instance = DMA1_Channel4;
    hdma_uart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart1_tx.Init.Mode = DMA_NORMAL;
    hdma_uart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_uart1_tx) != HAL_OK)
    {
        // error handler
    }
    __HAL_LINKDMA(&huart1, hdmatx, hdma_uart1_tx);
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
#endif // USE_DMA_UART1_TX

#if UART1_RXBUF_SIZE && USE_DMA_UART1_RX
    __DMA1_CLK_ENABLE();
    hdma_uart1_rx.Instance = DMA1_Channel5;
    hdma_uart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart1_rx.Init.Mode = DMA_NORMAL;
    hdma_uart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    if (HAL_DMA_Init(&hdma_uart1_rx) != HAL_OK)
    {
        // error handler
    }
    __HAL_LINKDMA(&huart1, hdmarx, hdma_uart1_rx);
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
#endif // USE_DMA_UART1_RX

#endif
}

HAL_StatusTypeDef uart_tx_byte(UART_HandleTypeDef *huart, uint8_t TxData)
{
    return HAL_UART_Transmit(huart, &TxData, 1, UART_TIMEOUT);
}

HAL_StatusTypeDef uart_tx(UART_HandleTypeDef *huart, const void *p, uint16_t size)
{
#if USE_UART1
    if (huart->Instance == USART1)
    {
#if USE_DMA_UART1_TX
        return HAL_UART_Transmit_DMA(huart, (uint8_t *)p, size);
#else
        return HAL_UART_Transmit(huart, (uint8_t *)p, size, UART_TIMEOUT);
#endif
    }
#endif
    return HAL_ERROR;
}

HAL_StatusTypeDef uart_tx_str(UART_HandleTypeDef *huart, const char *str)
{
    uint16_t size = 0;
    while (str[size] != '\0')
        ++size;
    ++size;
    return uart_tx(huart, str, size);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if USE_UART1 && UART1_RXBUF_SIZE
    if (huart->Instance == USART1)
    {
        for (uint16_t i = 0; i < UART1_RxSize; ++i)
            ((uint8_t *)UART1_pRxData)[i] = UART1_RxBuf[i];
        UART1_RxStatus = 1;
#if USE_DMA_UART1_RX
        HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RxBuf, UART1_RxSize);
#else
        HAL_UART_Receive_IT(&huart1, (uint8_t *)UART1_RxBuf, UART1_RxSize);
#endif
    }
#endif
}

uint8_t uart_get_rx_st(UART_HandleTypeDef *huart)
{
#if USE_UART1 && UART1_RXBUF_SIZE
    if (huart->Instance == USART1)
    {
        if (UART1_RxStatus)
        {
            UART1_RxStatus = 0;
            return 1;
        }
    }
#endif
    return 0;
}

HAL_StatusTypeDef uart_rx_byte(UART_HandleTypeDef *huart, void *p)
{
#if USE_UART1 && UART1_RXBUF_SIZE
    if (huart->Instance == USART1)
    {
        UART1_pRxData = p;
        UART1_RxSize = 1;
        return HAL_UART_Receive_IT(&huart1, (uint8_t *)UART1_RxBuf, 1);
    }
#endif
    return HAL_ERROR;
}

HAL_StatusTypeDef uart_rx(UART_HandleTypeDef *huart, void *p, uint16_t size)
{
#if USE_UART1 && UART1_RXBUF_SIZE
    if (huart->Instance == USART1)
    {
        UART1_pRxData = p;
        UART1_RxSize = size;
#if USE_DMA_UART1_RX
        return HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RxBuf, size);
#else
        return HAL_UART_Receive_IT(&huart1, (uint8_t *)UART1_RxBuf, size);
#endif
    }
#endif
    return HAL_ERROR;
}

#if USE_UART_STDIO
int fputc(int c, FILE *f)
{
#if USE_UART_STDIO == 1
    uart_tx_byte(&huart1, c);
    return c;
#endif
    return -1;
}
#endif
