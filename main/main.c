#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_module.h"
#include "sntp_module.h"
#include "spi_module.h"
#include "max7219.h"
#include "http_client.h"
#include "led_module.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "MAIN";

#define BITCOIN_API_URL "https://blockchain.info/q/getblockcount"
#define HTTP_RESPONSE_BUFFER_SIZE 32

static int previous_block_height = -1; // Track previous block height

static void bitcoin_response_callback(const char *response, int response_len, esp_err_t err) {
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
        return;
    }

    if (response && response_len > 0) {
        int block_height = atoi(response);
        ESP_LOGI(TAG, "Bitcoin block height: %d", block_height);
        
        // Check if this is a new block
        if (previous_block_height != -1 && block_height > previous_block_height) {
            ESP_LOGI(TAG, "New block detected! Previous: %d, New: %d", previous_block_height, block_height);
            max7219_new_block_animation();
        }
        
        max7219_display_number(block_height);
        previous_block_height = block_height;
    } else {
        ESP_LOGW(TAG, "Empty response received");
    }
}

static void bitcoin_fetch_task(void* pvParameters) {
    char response_buffer[HTTP_RESPONSE_BUFFER_SIZE + 1] = {0};

    while (1) {
        ESP_LOGI(TAG, "Fetching Bitcoin block height...");

        led_module_blink();
        
        esp_err_t err = http_client_get(BITCOIN_API_URL, response_buffer, 
                                        sizeof(response_buffer) - 1, 
                                        bitcoin_response_callback);
        
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to fetch Bitcoin block height: %s", esp_err_to_name(err));
        }

        memset(response_buffer, 0, sizeof(response_buffer));
        vTaskDelay(pdMS_TO_TICKS(60000)); // fetch every 60s
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting");
    ESP_ERROR_CHECK(led_module_init());
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init_sta();
    initialize_sntp();
    ESP_ERROR_CHECK(spi_module_init());
    max7219_init();
    max7219_loader_animation();
    xTaskCreate(&bitcoin_fetch_task, "bitcoin_fetch_task", 4096, NULL, 5, NULL);
}

