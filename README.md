# UAlarms - ESP32 Air Alert Monitor
![Espressif](https://img.shields.io/badge/espressif-E7352C.svg?style=for-the-badge&logo=espressif&logoColor=white) ![Adafruit](https://img.shields.io/badge/Adafruit-%23000000.svg?style=for-the-badge&logo=adafruit&logoColor=white) <br>
UAlarms is a compact ESP32-based device that displays real-time air raid alerts across all regions of Ukraine on a 128×64 OLED screen.

<img width="256" height="128" alt="Ukraine_Map" src="https://github.com/user-attachments/assets/eeed61d2-9fd0-4a9b-850b-95266cedef1f" />
<img width="256" height="128" alt="Ukraine_Regions_alarm" src="https://github.com/user-attachments/assets/d6e8d741-86de-4996-986e-d31301503bbe" />
<img width="256" height="128" alt="Ukraine_Regions_alarm2" src="https://github.com/user-attachments/assets/66ad1014-0e1c-4937-8248-ca8d23d548fc" />

## Hardware
 
| Component | Role |
|---|---|
| ESP32 | Main microcontroller |
| SH1106 OLED (128×64) | Display |
| Push Button | Manual screen wake / refresh |
## How It Works
 
The device periodically polls an alarm API and overlays active alert regions onto a bitmap map of Ukraine. Each region is represented as a pre-rendered XBM bitmap layer — when an alert is active, that region's layer is drawn on top of the base map, giving an instant visual overview of the current situation across the country.
 
All 25 regions are tracked, including Kyiv city separately from Kyiv oblast.

## Features
 
- Real-time air raid alert status for all 25 Ukrainian regions
- Visual map rendered entirely on a 128×64 OLED with layered XBM bitmaps
- Automatic periodic polling via HTTP
- Automatic screen sleep to prevent OLED burn-in
- Button wake — press to turn the screen back on and trigger an immediate update
- WiFi reconnect logic if connection drops

## Power & Display Management
 
To protect the OLED panel, the device implements automatic sleep after a configurable idle timeout. The screen can be woken at any time by pressing the button, which also triggers an immediate data refresh.
## Software Stack
 
| Library | Purpose |
|---|---|
| WiFi | Network connectivity |
| HTTPClient | Alarm API polling |
| ArduinoJson | JSON response parsing |
| U8g2 | OLED display control |

## Quick Start
 
### 1. Clone the repository
 
```bash
[git clone https://github.com/your-username/ukraine-alarm-map.git](https://github.com/RomanMirka/UAlarms-ESP32_Air_Alert_Monitor.git)
cd UAlarms-ESP32_Air_Alert_Monitor
```
 
### 2. Install Arduino dependencies
 
- ArduinoJson
- U8g2
### 3. Edit `config.h`
 
Fill in your WiFi credentials and alarm API endpoint:
 
```cpp
#define WIFI_SSID    "your-network"
#define WIFI_PASS    "your-password"
```
 
You can also configure timers here:
 
```cpp
#define http_timer                  // Polling interval (ms)
#define sleep_timer                 // Idle time before screen sleeps (ms)
#define sleep_on_screen_off_timer   // Auto-wake interval while screen is off (ms)
#define btn_deadzone                // Button debounce time (ms)
```
 
### 4. Wiring (ESP32)
 
| Component | ESP32 Pin |
|---|---|
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Button | GPIO 5 |
| VCC / GND | 3.3V / GND |
 
### 5. Flash firmware
 
Open the project in Arduino IDE, select **ESP32 Dev Module**, and upload.
 
The device will connect to WiFi, fetch the current alert status, and display the map immediately.

