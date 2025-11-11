#include "max7219.h"
#include "spi_module.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/spi_master.h"
#include "freertos/task.h"

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
    max7219_send(0x0A, 0x01); // Low brightness (0x00-0x0F, where 0x0F is max)
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

void max7219_loader_animation(void) {
    const int cycles = 2; // Number of back-and-forth cycles
    const int delay_ms = 100; // Delay between frames
    
    ESP_LOGI(TAG, "Starting loader animation");
    
    // Clear all digits first
    for (int digit = 1; digit <= 8; digit++) {
        max7219_send(digit, 0x0F); // blank
    }
    vTaskDelay(pdMS_TO_TICKS(50));
    
    for (int cycle = 0; cycle < cycles; cycle++) {
        // Right to left (digit 8 to 1)
        for (int pos = 8; pos >= 1; pos--) {
            // Clear previous position
            if (pos < 8) {
                max7219_send(pos + 1, 0x0F); // blank previous
            }
            // Show dot at current position (0x0F blank + 0x80 decimal point = 0x8F)
            max7219_send(pos, 0x8F); // blank with decimal point
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }
        
        // Left to right (digit 1 to 8)
        for (int pos = 1; pos <= 8; pos++) {
            // Clear previous position
            if (pos > 1) {
                max7219_send(pos - 1, 0x0F); // blank previous
            }
            // Show dot at current position
            max7219_send(pos, 0x8F); // blank with decimal point
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }
    }
    
    // Clear display after animation
    max7219_send(1, 0x0F);
    
    ESP_LOGI(TAG, "Loader animation complete");
}

void max7219_new_block_animation(void) {
    const int delay_ms = 80; // Faster animation for new block
    
    ESP_LOGI(TAG, "New block detected - showing animation");
    
    // Simple one-way animation: right to left, then left to right
    // Right to left (digit 8 to 1)
    for (int pos = 8; pos >= 1; pos--) {
        // Clear previous position
        if (pos < 8) {
            max7219_send(pos + 1, 0x0F); // blank previous
        }
        // Show dot at current position
        max7219_send(pos, 0x8F); // blank with decimal point
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
    
    // Left to right (digit 1 to 8)
    for (int pos = 1; pos <= 8; pos++) {
        // Clear previous position
        if (pos > 1) {
            max7219_send(pos - 1, 0x0F); // blank previous
        }
        // Show dot at current position
        max7219_send(pos, 0x8F); // blank with decimal point
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
    
    // Clear last position
    max7219_send(8, 0x0F);
}

