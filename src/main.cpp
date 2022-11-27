#include <Arduino.h>
#include <M5StickCPlus.h>
#include <AXP192.h>
#include <WiFi.h>
#include <NTPClient.h>   //https://github.com/taranais/NTPClient
#include <Preferences.h>

#include "utility.h"
#include "wifi_helpers.h"
#include "dseg_font.h"

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Preferences preferences;

void setup()
{
  M5.begin();

  preferences.begin("ntp-clock", false);

  // read stored date from preferences
  String clockUpdateDate = preferences.getString("clockUpdateDate", "");

  // build current date from RTC
  M5.Rtc.GetData(&RTC_DateStruct);
  String currentDate = String(RTC_DateStruct.Year) + "." + String(RTC_DateStruct.Month) + "." + String(RTC_DateStruct.Date);

  // if dates are different connect to wlan and sync time with NTP server
  if (clockUpdateDate.equals("") || !clockUpdateDate.equals(currentDate)) {
    // search for wifi networks and connects to selected wlan neetwork or mobile hotspot (if available)
    initWiFi();

    if (WiFi.status() == WL_CONNECTED) {
      timeClient.begin();
      // Set offset time in seconds to adjust for your timezone, for example:
      // GMT +1 = 3600
      // GMT +8 = 28800
      // GMT -1 = -3600
      // GMT 0 = 0
      timeClient.setTimeOffset(3600);

      timeClient.update();

      RTC_TimeTypeDef TimeStruct;
      TimeStruct.Hours = timeClient.getHours();
      TimeStruct.Minutes = timeClient.getMinutes();
      TimeStruct.Seconds = timeClient.getSeconds();

      String formattedDate = timeClient.getFormattedDate();
      int year = formattedDate.substring(0, 4).toInt();
      int month = formattedDate.substring(5, 7).toInt();
      int day = formattedDate.substring(8, 10).toInt();

      RTC_DateTypeDef DateStruct;
      DateStruct.WeekDay = calcDayNumFromDate(year, month, day);
      DateStruct.Date = day;
      DateStruct.Month = month;
      DateStruct.Year = year;

      M5.Rtc.SetTime(&TimeStruct);
      M5.Rtc.SetData(&DateStruct);

      // save current date to preferences
      preferences.putString("clockUpdateDate", currentDate);
      preferences.end();

      WiFi.disconnect();
    }
  }

  LCD_Clear(1);

  M5.Axp.EnableCoulombcounter();
  M5.Axp.ScreenBreath(BRIGHTNESS_LEVELS[0]);
}

int H = 0;
int M = 0;

int brtLvl = 0;
bool inv = 0;

void loop()
{
  M5.update();

  if (M5.BtnA.wasReleased())
  {
    brtLvl++;

    if (brtLvl == BRIGHTNESS_MAX)
    {
      brtLvl = 0;
    }

    M5.Axp.ScreenBreath(BRIGHTNESS_LEVELS[brtLvl]);

    // clear brightness level area
    M5.Lcd.fillRect(LCD_WIDTH - 14, 0, 8, 30, TFT_BLACK);
  }

  M5.Rtc.GetTime(&RTC_TimeStruct);
  M5.Rtc.GetData(&RTC_DateStruct);

  // day of the week
  String weekDay = DAYS[RTC_DateStruct.WeekDay - 1];
  M5.Lcd.drawString(weekDay, 4, 2, 2);

  // battery voltage
  String batt = String(M5.Axp.GetBatVoltage()) + " V";
  M5.Lcd.drawString(batt, LCD_WIDTH - (batt.length() * 8) - 16, 10, 2); // TODO: use Lcd.textWidth

  // color change to grey
  M5.Lcd.setTextColor(TFT_LIGHTGREY, TFT_BLACK);

  // day, month and year
  String dayMonthYear = String(RTC_DateStruct.Date) + "." + String(RTC_DateStruct.Month) + "." + String(RTC_DateStruct.Year);
  M5.Lcd.drawString(dayMonthYear, 4, 20, 4);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  // segment font
  M5.Lcd.setFreeFont(&DSEG7_Classic_Regular);

  // updates only if value changed (e.g.: every minute or hour)
  if (H != int(RTC_TimeStruct.Hours) || M != int(RTC_TimeStruct.Minutes))
  {
    String ho = formatValue(String(RTC_TimeStruct.Hours));
    String mi = formatValue(String(RTC_TimeStruct.Minutes));

    M5.Lcd.drawString(ho + ":" + mi, 2, LCD_HEIGHT - 70);

    H = int(RTC_TimeStruct.Hours);
    M = int(RTC_TimeStruct.Minutes);
  }

  String se = formatValue(String(RTC_TimeStruct.Seconds));
  M5.Lcd.drawString(se, LCD_WIDTH - 30, LCD_HEIGHT - 30, 4);

  // brightness level bars
  for (int i = 0; i < brtLvl + 1; i++)
  {
    M5.Lcd.fillRect(LCD_WIDTH - 14, 20 - (i * 5), 8, 3, TFT_LIGHTGREY);
  }

  // invert display when button B (side one) was released
  if (M5.BtnB.wasReleased())
  {
    M5.Lcd.invertDisplay(inv);
    inv = !inv;
  }

  delay(50);
}
