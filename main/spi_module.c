#include "spi_module.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "SPI_MODULE";
static spi_device_handle_t spi_handle = NULL;

esp_err_t spi_module_init(void) {
    esp_err_t ret;

    spi_bus_config_t buscfg = {
        .miso_io_num = -1,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
    };

    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI bus initialization failed");
        return ret;
    }

    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI device add failed");
        return ret;
    }

    ESP_LOGI(TAG, "SPI module initialized");
    return ESP_OK;
}

spi_device_handle_t spi_module_get_handle(void) {
    return spi_handle;
}

