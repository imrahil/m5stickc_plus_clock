#include <Arduino.h>
#include <M5StickCPlus.h>
#include <AXP192.h>

#include "dseg_font.h"
#include "app_secrets.h"

#define LCD_WIDTH 240
#define LCD_HEIGHT 135

#define LCD_ROTATION 3

#define BRIGHTNESS_MAX 4

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

int bright[4] = {8, 9, 10, 12};

void setup()
{
  M5.begin();

  M5.Lcd.setRotation(LCD_ROTATION);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setSwapBytes(true);
  M5.Lcd.setTextSize(1);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  M5.Axp.EnableCoulombcounter();
  M5.Axp.ScreenBreath(bright[0]);

  RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours = 0;
  TimeStruct.Minutes = 0;
  TimeStruct.Seconds = 0;

  RTC_DateTypeDef DateStruct;
  DateStruct.WeekDay = 1;
  DateStruct.Month = 1;
  DateStruct.Date = 1;
  DateStruct.Year = 2022;
}

int H = 0;
int M = 0;

String days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

int brtLvl = 0;
bool inv = 0;

String formatValue(String value)
{
  if (value.length() < 2)
  {
    value = "0" + value;
  }

  return value;
}

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

    M5.Axp.ScreenBreath(bright[brtLvl]);

    // clear brightness level area
    M5.Lcd.fillRect(LCD_WIDTH - 14, 0, 8, 30, TFT_BLACK);
  }

  M5.Rtc.GetTime(&RTC_TimeStruct);
  M5.Rtc.GetData(&RTC_DateStruct);

  // day of the week
  String weekDay = days[RTC_DateStruct.WeekDay - 1];
  M5.Lcd.drawString(weekDay, 4, 2, 2);

  // battery voltage
  String batt = String(M5.Axp.GetBatVoltage()) + " V";
  M5.Lcd.drawString(batt, LCD_WIDTH - (batt.length() * 8) - 16, 10, 2);

  // color change to grey
  M5.Lcd.setTextColor(TFT_LIGHTGREY, TFT_BLACK);

  // day and month
  String dayMonth = String(RTC_DateStruct.Date) + "/" + String(RTC_DateStruct.Month);
  M5.Lcd.drawString(dayMonth, 4, 20, 4);

  // year
  String year = String(RTC_DateStruct.Year);
  M5.Lcd.drawString(year, 70, 28, 2);

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
