#include <WiFi.h>
#include "config.h"
#include "bmps.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

struct Alarms
{
  const char *Region;
  bool isArarmOn;
};

Alarms regions[] = {
    {"Закарпатська область", false},
    {"Хмельницька область", false},
    {"Черкаська область", false},
    {"Чернігівська область", false},
    {"Чернівецька область", false},
    {"Донецька область", false},
    {"Дніпропетровська область", false},
    {"Івано-Франківська область", false},
    {"Харківська область", false},
    {"Херсонська область", false},
    {"Кіровоградська область", false},
    {"м. Київ", false},
    {"Київська область", false},
    {"Волинська область", false},
    {"Львівська область", false},
    {"Луганська область", false},
    {"Миколаївська область", false},
    {"Одеська область", false},
    {"Полтавська область", false},
    {"Рівненська область", false},
    {"Сумська область", false},
    {"Тернопільська область", false},
    {"Вінницька область", false},
    {"Запорізька область", false},
    {"Житомирська область", false},
};

void wifi_conn()
{
  display_some("Connecting WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
  }
  display_some("WiFi connected");
  delay(110);
}

void parseAlerts(String result)
{
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, result);
  if (error)
  {
    display_some("Server isn't working");
    delay(7000);
    display.setPowerSave(1);
    return;
  }
  for (JsonPair state : doc["states"].as<JsonObject>())
  {
    const char *state_key = state.key().c_str();
    bool state_value_alertnow = state.value()["alertnow"];
    for (int i = 0; i < 25; i++)
    {
      if (strcmp(state_key, regions[i].Region) == 0)
      {
        regions[i].isArarmOn = state_value_alertnow;
        break;
      }
    }
  }
}

void get_arlm()
{
  HTTPClient http;
  http.begin(ALARM_API);
  int httpCode = http.GET();
  if (httpCode == 200)
  {
    String result = http.getString();
    parseAlerts(result);
    update_alert();
  }
  http.end();
}

void display_some(String text)
{
  display.clearBuffer();
  display.setFont(u8g2_font_samim_14_t_all);
  display.drawStr(12, 38, text.c_str());
  display.sendBuffer();
  delay(1000);
}

void update_alert()
{
  display.clearBuffer();
  display.setBitmapMode(1);
  display.drawXBMP(0, 0, 128, 64, ua_map);
  display.drawXBMP(0, 0, 128, 64, krum);
  for (int i = 0; i < 25; i++)
  {
    if (regions[i].isArarmOn == true)
    {
      display.drawXBMP(0, 0, 128, 64, Regions_Alert_Bitmaps[i]);
    }
  }
  display.sendBuffer();
}

void setup()
{
  Serial.begin(115200);
  pinMode(5, INPUT_PULLUP);
  display.begin();
  display.setColorIndex(1);
  display.clearBuffer();
  display.setBitmapMode(1);
  display.drawXBMP(0, 0, 128, 64, ua_map);
  display.drawXBMP(0, 0, 128, 64, krum);
  display.sendBuffer();
  wifi_conn();
  start_timer = millis();
}

void loop()
{
  buttonState = digitalRead(5);
  if ((millis() - http_started >= http_timer && IsScreenOff == false) || (CheckOnSetup == true && IsScreenOff == false))
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      get_arlm();
      http_started = millis();
      CheckOnSetup = false;
    }
    if (WiFi.status() != WL_CONNECTED)
    {
      wifi_conn();
    }
  }
  if ((millis() - start_timer >= sleep_timer) && (IsScreenOff == false)) {
    display.clearBuffer();
    display.sendBuffer();
    display.setPowerSave(1);
    sleep_on_timer = millis();
    IsScreenOff = true;
  }
  if ((IsScreenOff == true && millis() - sleep_on_timer >= sleep_on_screen_off_timer) || (IsScreenOff && IsButtonPressed == true)) {
    display.setPowerSave(0);
    start_timer = millis();
    get_arlm();
    IsScreenOff = false;
    IsButtonPressed = false;
  }
  if ((buttonState == LOW) && (millis() - btn_deadzone_started >= btn_deadzone)) {
    btn_deadzone_started = millis();
    IsButtonPressed = true;
  }
}
