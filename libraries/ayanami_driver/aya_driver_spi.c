#include "aya_driver_spi.h"

void spi_init(spi_index_t spi_n, spi_mode_t mode, int baudrate)
{
    switch (spi_n)
    {
    case SPI_0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

        GPIOPinConfigure(GPIO_PA2_SSI0CLK);
        GPIOPinConfigure(GPIO_PA5_SSI0TX);
        GPIOPinConfigure(GPIO_PA4_SSI0RX);
        GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);
        SSIConfigSetExpClk(SSI0_BASE, 22000000, mode,
                           SSI_MODE_MASTER, baudrate, 8);
        SSIEnable(SSI0_BASE);
        break;
    case SPI_1:
        break;
    case SPI_2:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

        GPIOPinConfigure(GPIO_PB4_SSI2CLK);
        GPIOPinConfigure(GPIO_PB7_SSI2TX);
        // MISO、CS功能因LCD实际需要去掉，需要则参考其他SPI进行补充
        // 默认全部使用软件CS，即总线收发前先将CS以GPIO操作
        // SysCtlClockGet()
        GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_7);
        SSIConfigSetExpClk(SSI2_BASE, 22000000, mode,
                           SSI_MODE_MASTER, baudrate, 8);
        SSIEnable(SSI2_BASE);

        break;
    case SPI_3:
        break;
    }
}

uint8_t spi_readwritebyte(spi_index_t spi_n, uint8_t data)
{

    uint32_t tx_buff = data, rx_buff;
    SSIDataPut(SSI0_BASE + spi_n, tx_buff);
    while (SSIBusy(SSI0_BASE + spi_n))
        ;
    SSIDataGet(SSI0_BASE + spi_n, &rx_buff);
    while (SSIBusy(SSI0_BASE + spi_n))
        ;
    return (uint8_t)(rx_buff & 0xff);
}

void spi_readwrite(spi_index_t spi_n, const uint8_t *txdata, uint8_t *rxdata, uint32_t length)
{
    uint8_t tx_buff, rx_buff;
    for (int pt = 0; pt < length; pt++)
    {
        rx_buff = spi_readwritebyte(spi_n, *(txdata + pt));
        *(rxdata + pt) = rx_buff;
    }
}

void spi_read(spi_index_t spi_n, uint8_t *rxdata, uint32_t length)
{
    uint8_t rx_buff;
    for (int pt = 0; pt < length; pt++)
    {
        rx_buff = spi_readwritebyte(spi_n, 0x00);
        *(rxdata + pt) = rx_buff;
    }
}

void spi_write(spi_index_t spi_n, const uint8_t *txdata, uint32_t length)
{
    for (int pt = 0; pt < length; pt++)
    {
        spi_readwritebyte(spi_n, *(txdata + pt));
    }
}