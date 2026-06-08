#ifndef CONFIG_H
#define CONFIG_H

const unsigned long http_timer = 2UL * 60UL * 1000UL;
const unsigned long sleep_timer = 3UL * 60UL * 1000UL;
const unsigned long sleep_on_screen_off_timer = 16UL * 60UL * 1000UL;
const unsigned long btn_deadzone = 70UL;
unsigned long http_started = 0;
unsigned long start_timer = 0;
unsigned long sleep_on_timer = 0;
unsigned long btn_deadzone_started = 0;
bool CheckOnSetup = true;
bool IsScreenOff = false;
bool IsButtonPressed = false;
int buttonState = 0;

#define WIFI_SSID   "Your-Wifi"
#define WIFI_PASS   "Your-Password"

#define ALARM_API "http://ubilling.net.ua/aerialalerts/"

#endif