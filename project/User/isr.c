/*
 * isr.c
 *
 *  Created on: 2023年6月1日
 *      Author: 13164
 */
#include "isr.h"
int fps = 0;

void Timer0IntHandler(void)
{
    MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    fps = 0;
}

void Timer1IntHandler(void)
{
    MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
   
}

void Timer2IntHandler(void)
{
    MAP_TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

}

void Timer3IntHandler(void)
{
    MAP_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    key_handler();
    //  UARTprintf("KEY:\tUP\tLF\tRT\tDN\tCT\tS1\tS2\n");
    //  UARTprintf("Prs:\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", key_read(0), key_read(1), key_read(2), key_read(3), key_read(4), key_read(5), key_read(6));
    //  UARTprintf("Tim:\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", key_cnt[0], key_cnt[1], key_cnt[2], key_cnt[3], key_cnt[4], key_cnt[5], key_cnt[6]);
}

void Timer4IntHandler(void)
{
    MAP_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
}

void Timer5IntHandler(void)
{
    MAP_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
}

void Uart0IntHandler(void)
{
    int32_t recvData32;
    uint8_t recvData8;
    uint32_t ui32Status = UARTIntStatus(UART0_BASE, true);

    UARTIntClear(UART0_BASE, ui32Status);
    // 判断是否接收到数据
    if (ui32Status & UART_INT_RX)
    {
        // 读取接收的字符
        do
        {
            recvData32 = UARTCharGetNonBlocking(UART0_BASE);
            if (recvData32 != -1)
            {
                recvData8 = (uint8_t)recvData32;
                uart_write_byte(UART_0, recvData8);
            }
        } while (recvData32 != -1);
    }
}

void Uart1IntHandler(void)
{
    int32_t recvData32;
    uint8_t recvData8;
    uint32_t ui32Status = UARTIntStatus(UART1_BASE, true);

    UARTIntClear(UART1_BASE, ui32Status);
    // 判断是否接收到数据
    if (ui32Status & UART_INT_RX)
    {
        // 读取接收的字符
        do
        {
            recvData32 = UARTCharGetNonBlocking(UART1_BASE);
            if (recvData32 != -1)
            {
                recvData8 = (uint8_t)recvData32;
                uart_write_byte(UART_1, recvData8);
            }
        } while (recvData32 != -1);
    }
}

void Uart6IntHandler(void)
{
    int32_t recvData32;
    uint8_t recvData8;
    uint32_t ui32Status = UARTIntStatus(UART6_BASE, true);

    UARTIntClear(UART6_BASE, ui32Status);

    // 判断是否接收到数据
    if (ui32Status & UART_INT_RX)
    {
        // 读取接收的字符
        do
        {
            recvData32 = UARTCharGetNonBlocking(UART6_BASE);
            if (recvData32 != -1)
            {
                recvData8 = (uint8_t)recvData32;
                uart_write_byte(UART_6, recvData8);
            }
        } while (recvData32 != -1);
    }
}
