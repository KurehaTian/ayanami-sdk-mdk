#ifndef AYA_DRIVER_SPI_H_
#define AYA_DRIVER_SPI_H_
#include <aya_driver_head.h>
#include <aya_device_head.h>
typedef enum
{
    SPI_0 = 0,
    SPI_1 = 0x1000,
    SPI_2 = 0x2000,
    SPI_3 = 0x3000
} spi_index_t;

typedef enum
{
    SPI_MODE_0 = SSI_FRF_MOTO_MODE_0,
    SPI_MODE_1 = SSI_FRF_MOTO_MODE_1,
    SPI_MODE_2 = SSI_FRF_MOTO_MODE_2,
    SPI_MODE_3 = SSI_FRF_MOTO_MODE_3
} spi_mode_t;

void spi_init(spi_index_t spi_n, spi_mode_t mode, int baudrate);
uint8_t spi_readwritebyte(spi_index_t spi_n, uint8_t data);
void spi_readwrite(spi_index_t spi_n, const uint8_t *txdata, uint8_t *rxdata, uint32_t length);
void spi_read(spi_index_t spi_n, uint8_t *rxdata, uint32_t length);
void spi_write(spi_index_t spi_n, const uint8_t *txdata, uint32_t length);

#endif
