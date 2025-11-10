#include "http_client.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <inttypes.h>

#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#include "esp_crt_bundle.h"
#endif

static const char *TAG = "HTTP_CLIENT";

typedef struct {
    char *buffer;
    size_t buffer_size;
    size_t buffer_len;
    http_response_callback_t callback;
} http_client_context_t;

static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    http_client_context_t *ctx = (http_client_context_t *)evt->user_data;
    
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            // Reset buffer length at the start of a new connection
            if (ctx && ctx->buffer) {
                ctx->buffer_len = 0;
                memset(ctx->buffer, 0, ctx->buffer_size);
            }
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            // Copy data regardless of chunked or not
            if (ctx && ctx->buffer && evt->data) {
                // The last byte in buffer is kept for the NULL character in case of out-of-bound access.
                size_t copy_len = MIN(evt->data_len, (ctx->buffer_size - 1 - ctx->buffer_len));
                if (copy_len > 0) {
                    memcpy(ctx->buffer + ctx->buffer_len, evt->data, copy_len);
                    ctx->buffer_len += copy_len;
                    // Ensure null-termination
                    ctx->buffer[ctx->buffer_len] = '\0';
                } else {
                    ESP_LOGW(TAG, "Buffer full, cannot copy more data");
                }
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            // Ensure null-termination at the end
            if (ctx && ctx->buffer) {
                ctx->buffer[ctx->buffer_len] = '\0';
            }
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGD(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGD(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            break;
    }
    return ESP_OK;
}

esp_err_t http_client_get(const char *url, char *response_buffer, size_t buffer_size, http_response_callback_t callback) {
    if (!url || !response_buffer || buffer_size == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    http_client_context_t ctx = {
        .buffer = response_buffer,
        .buffer_size = buffer_size,
        .buffer_len = 0,
        .callback = callback
    };

    esp_http_client_config_t config = {
        .url = url,
        .timeout_ms = 5000,
        .event_handler = _http_event_handler,
        .user_data = &ctx,
#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
        .crt_bundle_attach = esp_crt_bundle_attach,
#else
        .cert_pem = NULL,  // Will use default certificate bundle if available
#endif
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = esp_http_client_perform(client);
    
    if (err == ESP_OK) {
        int status_code = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRId64,
                status_code,
                esp_http_client_get_content_length(client));
        
        // Ensure null-termination
        response_buffer[ctx.buffer_len] = '\0';
        
        // Call callback if provided
        if (callback) {
            callback(response_buffer, ctx.buffer_len, ESP_OK);
        }
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        if (callback) {
            callback(NULL, 0, err);
        }
    }

    esp_http_client_cleanup(client);
    return err;
}

void http_client_fetch_task(void* pvParameters) {
    // This is a placeholder - specific task implementation should be in main.c
    // This function is kept for backward compatibility but should not be used
    ESP_LOGW(TAG, "http_client_fetch_task called - this should be implemented in main.c");
    vTaskDelete(NULL);
}

