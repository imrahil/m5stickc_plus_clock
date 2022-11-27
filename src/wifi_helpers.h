#ifndef _APP_WIFI_
#define _APP_WIFI_

#include <Arduino.h>
#include <M5StickCPlus.h>
#include <WiFi.h>

#include "utility.h"
#include <app_secrets.h>

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  LCD_Clear();
  textCenter center;

  M5.Lcd.setTextColor(TFT_GREEN);
  drawCenteredText("Please, wait", 20);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_ORANGE);

  drawCenteredText("Searching Wifi networks...", 50);

  int n = WiFi.scanNetworks();
  int mobileApFound = -1;

  if (n == 0)
  {
    LCD_Clear();
    drawCenteredText("No networks found...");

    return;
  }
  else
  {
    for (int i = 0; i < n; ++i)
    {
      String ssid = WiFi.SSID(i);

      if (ssid == MOBILE_WLAN_SSID)
      {
        mobileApFound = i;
      }
    }
  }

  if (mobileApFound > -1)
  {
    WiFi.begin(MOBILE_WLAN_SSID, MOBILE_WLAN_PASS);
  }
  else
  {
    WiFi.begin(WLAN_SSID, WLAN_PASS);
  }

  drawCenteredText("Connecting to WiFi...", 70);

  int count_try = 0;

  while (WiFi.status() != WL_CONNECTED && count_try < WLAN_MAX_TRY)
  {
    delay(1000);

    count_try++;
  }

  if (WiFi.status() == WL_NO_SSID_AVAIL)
  {
    LCD_Clear();

    M5.Lcd.setTextColor(TFT_RED);
    drawCenteredText("Impossible to", 30);
    drawCenteredText("establish", 55);
    drawCenteredText("WiFi connection!", 80);

    WiFi.disconnect();
  } else {
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_GREEN);

    drawCenteredText("Connected!", 90);

    M5.Lcd.setTextSize(1);
    drawCenteredText(mobileApFound > -1 ? "Mobile hotspot" : "WLAN", 110);

    delay(1000);
  }
}

#endif
