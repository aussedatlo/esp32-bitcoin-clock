#ifndef SPI_MODULE_H
#define SPI_MODULE_H

#include "driver/spi_master.h"

// SPI Pin Configuration (ESP32-C3 SuperMini)
#define PIN_NUM_MISO   5   // GPIO5
#define PIN_NUM_MOSI   6   // GPIO6
#define PIN_NUM_CLK    4   // GPIO4
#define PIN_NUM_CS     7   // GPIO7

esp_err_t spi_module_init(void);
spi_device_handle_t spi_module_get_handle(void);

#endif // SPI_MODULE_H

