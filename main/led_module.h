#ifndef LED_MODULE_H
#define LED_MODULE_H

#include "esp_err.h"

// Built-in blue LED on ESP32 boards is typically on GPIO 2
#define BLUE_LED_GPIO 2

esp_err_t led_module_init(void);
void led_module_blink(void);

#endif // LED_MODULE_H

