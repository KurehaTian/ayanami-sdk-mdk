/*
 * aya_driver_uart.h
 *
 *  Created on: 2023Äê4ÔÂ2ÈÕ
 *      Author: KurehaTian
 */

#ifndef __AYA_DRIVER_UART_H
#define __AYA_DRIVER_UART_H
#include "aya_driver_head.h"

typedef enum
{
    UART_0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7
} uart_index_t;



void uart_init(uart_index_t uart_n, uint32_t baudrate);
void uart_write_byte(uart_index_t uart_n, uint8_t dat);
void uart_write_buffer(uart_index_t uart_n, uint8_t *buff, uint32_t len);
void uart_write_string(uart_index_t uart_n, const char *str);
uint8_t uart_read_byte(uart_index_t uart_n);
uint8_t uart_quary_byte(uart_index_t uart_n, uint8_t *dat);

void ConfigureUART(void);



#endif 
