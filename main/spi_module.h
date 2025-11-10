#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include "driver/spi_master.h"

// SPI Pin Configuration
#define PIN_NUM_MOSI   23
#define PIN_NUM_CLK    18
#define PIN_NUM_CS     15

esp_err_t spi_module_init(void);
spi_device_handle_t spi_module_get_handle(void);

#endif // SPI_MODULE_H

