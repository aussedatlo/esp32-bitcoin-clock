#include "max7219.h"
#include "spi_module.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/spi_master.h"

static const char *TAG = "MAX7219";

static void max7219_send(uint8_t addr, uint8_t data) {
    spi_device_handle_t spi = spi_module_get_handle();
    if (spi == NULL) {
        ESP_LOGE(TAG, "SPI handle is NULL");
        return;
    }

    uint8_t tx_data[2] = {addr, data};
    spi_transaction_t t = {
        .length = 16,
        .tx_buffer = tx_data,
    };
    esp_err_t ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI transmit failed: %d", ret);
    }
}

void max7219_init(void) {
    max7219_send(0x0F, 0x00); // Display test off
    max7219_send(0x0C, 0x01); // Normal operation
    max7219_send(0x0B, 0x07); // Display all 8 digits
    max7219_send(0x0A, 0x08); // Medium brightness
    max7219_send(0x09, 0xFF); // Decode mode BCD

    ESP_LOGI(TAG, "MAX7219 initialized");
}

void max7219_display_number(int value) {
    for (int digit = 1; digit <= 8; digit++) {
        uint8_t num = value % 10;
        max7219_send(digit, num);
        value /= 10;
        if (value == 0) {
            for (int j = digit + 1; j <= 8; j++) {
                max7219_send(j, 0x0F); // blank
            }
            break;
        }
    }
}

