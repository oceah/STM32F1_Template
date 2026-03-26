#ifndef _UART_H
#define _UART_H

#include "sys.h"

/**
 * @param 0 do not use serial port as stdio
 * @param 1 use UART1 as stdio
 */
#define USE_UART_STDIO 1
#if USE_UART_STDIO
#include <stdio.h>
#endif

#define UART_TIMEOUT     ((uint32_t)0x000000ff)

#define USE_UART1        1
#define USE_DMA_UART1_TX 1
#define UART1_RXBUF_SIZE 32 // 0 to disable UART1-RX
#define USE_DMA_UART1_RX 1

#ifdef __cplusplus
extern "C" {
#endif

#if USE_UART1
extern UART_HandleTypeDef huart1;
#endif

void uart_init();

HAL_StatusTypeDef uart_tx_byte(UART_HandleTypeDef *huart, uint8_t TxData);
HAL_StatusTypeDef uart_tx(UART_HandleTypeDef *huart, const void *p, uint16_t size);
HAL_StatusTypeDef uart_tx_str(UART_HandleTypeDef *huart, const char *str);

/**
 * @brief read and reset uart rx status
 * @return 1 if rx success else 0
 */
uint8_t uart_get_rx_st(UART_HandleTypeDef *huart);
HAL_StatusTypeDef uart_rx_byte(UART_HandleTypeDef *huart, void *p);
HAL_StatusTypeDef uart_rx(UART_HandleTypeDef *huart, void *p, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif // _UART_H
