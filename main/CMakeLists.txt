idf_component_register(SRCS "led_module.c" "main.c" "wifi_module.c" "http_client.c" "max7219.c" "spi_module.c" "sntp_module.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver freertos
                    PRIV_REQUIRES esp_event esp_http_client esp-tls esp_wifi esp_netif nvs_flash)
