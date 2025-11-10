#include "sntp_module.h"
#include "esp_log.h"
#include "esp_sntp.h"

static const char *TAG = "SNTP_MODULE";

void initialize_sntp(void)
{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();
    ESP_LOGI(TAG, "SNTP initialized");
}

