#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "esp_err.h"

#define HTTP_CLIENT_MAX_BUFFER_SIZE 256

typedef void (*http_response_callback_t)(const char *response, int response_len, esp_err_t err);

esp_err_t http_client_get(const char *url, char *response_buffer, size_t buffer_size, http_response_callback_t callback);
void http_client_fetch_task(void* pvParameters);

#endif // HTTP_CLIENT_H

