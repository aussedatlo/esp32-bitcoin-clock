# ESP32 Bitcoin Block Height Display

ESP32 project that fetches Bitcoin block height from blockchain.info API and displays it on a MAX7219 7-segment display.

## Features

- WiFi connectivity
- HTTPS API requests with certificate validation
- SNTP time synchronization
- MAX7219 7-segment display driver
- Modular code structure

## Software Requirements

Before building and flashing this project, you must install the Espressif IoT Development Framework (ESP-IDF).  
Follow the official installation guide for your operating system:  
[ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation)

Ensure you have set up the required toolchain, Python environment, and added ESP-IDF tools to your PATH before proceeding.

## Hardware Required

- ESP32 development board
- MAX7219 7-segment display module
- SPI connections (MOSI, CLK, CS)

## Configuration

### WiFi Setup

Configure WiFi credentials using menuconfig:

```bash
idf.py menuconfig
```

Navigate to: `Example Configuration` → `WiFi Configuration`

- Set `WiFi SSID` to your network name
- Set `WiFi Password` to your network password

Alternatively, set via environment variables:

```bash
WIFI_SSID="your_ssid" WIFI_PASSWORD="your_password" idf.py build
```

## Build and Flash

```bash
idf.py set-target esp32
idf.py build
idf.py -p PORT flash monitor
```

## Project Structure

```
main/
├── main.c              # Application entry point
├── wifi_module.c/h     # WiFi connection management
├── http_client.c/h     # Generic HTTP client
├── max7219.c/h         # MAX7219 display driver
├── spi_module.c/h      # SPI bus initialization
└── sntp_module.c/h     # SNTP time synchronization
```

## License

See LICENSE file for details.
