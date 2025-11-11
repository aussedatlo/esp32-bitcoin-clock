#include "led_module.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "LED_MODULE";

esp_err_t led_module_init(void) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << BLUE_LED_GPIO),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    
    esp_err_t ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure GPIO %d: %s", BLUE_LED_GPIO, esp_err_to_name(ret));
        return ret;
    }
    
    // Set LED to off initially
    gpio_set_level(BLUE_LED_GPIO, 0);
    
    ESP_LOGI(TAG, "LED module initialized on GPIO %d", BLUE_LED_GPIO);
    return ESP_OK;
}

void led_module_blink(void) {
    // Turn LED on
    gpio_set_level(BLUE_LED_GPIO, 1);
    // Small delay for visibility
    vTaskDelay(pdMS_TO_TICKS(200));
    // Turn LED off
    gpio_set_level(BLUE_LED_GPIO, 0);
}

