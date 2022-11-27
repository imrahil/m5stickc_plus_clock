#include <Arduino.h>
#include <M5StickCPlus.h>
#include "AXP192.h"
#include "7seg20.h"

#define grey 0x65DB
#define BUTTON_A 37
#define BUTTON_B 39

#define LCD_WIDTH 240
#define LCD_HEIGHT 135

#define LCD_ROTATION 3

RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

int bright[4] = {8, 9, 10, 12};

void setup()
{
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setSwapBytes(true);
  M5.Lcd.setTextSize(1);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  M5.Axp.EnableCoulombcounter();
  M5.Axp.ScreenBreath(bright[0]);

  RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours = 0;
  TimeStruct.Minutes = 0;
  TimeStruct.Seconds = 00;

  RTC_DateTypeDef DateStruct;
  DateStruct.WeekDay = 1;
  DateStruct.Month = 1;
  DateStruct.Date = 1;
  DateStruct.Year = 2022;
}

int H = 0;
int M = 0;

String ho = "";
String mi = "";
String se = "";

String days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

int c = 0;
int pres = 0;
bool inv = 0;

void loop()
{
  // M5.update();

  if (digitalRead(BUTTON_A) == 0)
  {
    if (pres == 0)
    {
      pres = 1;
      c++;

      if (c > 3)
      {
        c = 0;
      }

      M5.Axp.ScreenBreath(bright[c]);
      M5.Lcd.fillRect(146, 55, 8, 30, TFT_BLACK);
    }
  }
  else
  {
    pres = 0;
  }

  M5.Rtc.GetTime(&RTC_TimeStruct);
  M5.Rtc.GetData(&RTC_DateStruct);

  M5.Lcd.setCursor(0, 15);
  M5.Lcd.setTextFont(0);

  M5.Lcd.drawString(String(M5.Axp.GetBatVoltage()) + " V  ", 114, 3);
  M5.Lcd.setFreeFont(&DSEG7_Classic_Bold_30);

  if (H != int(RTC_TimeStruct.Hours) || M != int(RTC_TimeStruct.Minutes))
  {
    ho = String(RTC_TimeStruct.Hours);
    mi = String(RTC_TimeStruct.Minutes);

    if (ho.length() < 2)
    {
      ho = "0" + ho;
    }

    if (mi.length() < 2)
    {
      mi = "0" + mi;
    }

    M5.Lcd.drawString(ho + ":" + mi, 2, 46);

    H = int(RTC_TimeStruct.Hours);
    M = int(RTC_TimeStruct.Minutes);
  }

  se = String(RTC_TimeStruct.Seconds);

  if (se.length() < 2)
  {
    se = "0" + se;
  }

  M5.Lcd.drawString(se, 112, 57, 4);

  M5.Lcd.drawString(days[RTC_DateStruct.WeekDay - 1] + "    ", 4, 0, 2);
  M5.Lcd.setTextColor(grey, TFT_BLACK);

  M5.Lcd.drawString(String(RTC_DateStruct.Date) + "/" + String(RTC_DateStruct.Month), 4, 20, 4);
  M5.Lcd.drawString(String(RTC_DateStruct.Year), 70, 28, 2);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  for (int i = 0; i < c + 1; i++)
  {
    M5.Lcd.fillRect(146, 73 - (i * 5), 8, 3, grey);
  }

  if (digitalRead(BUTTON_B) == LOW)
  {
    while (digitalRead(BUTTON_B) == LOW)
    {
    }

    M5.Lcd.invertDisplay(inv);
    inv = !inv;
  }

  delay(12);
}
