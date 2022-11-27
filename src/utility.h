#ifndef _APP_UTILITY_
#define _APP_UTILITY_

#include <Arduino.h>
#include <M5StickCPlus.h>

#define LCD_WIDTH 240
#define LCD_HEIGHT 135

#define BRIGHTNESS_MAX 4
const int BRIGHTNESS_LEVELS[4] = {7, 8, 9, 10};

#define LCD_ROTATION 3

const String DAYS[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

struct textCenter
{
  int horizontal;
  int vertical;
};

void LCD_Clear(int fontSize = 2)
{
  M5.Lcd.setRotation(LCD_ROTATION);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setSwapBytes(true);

  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(fontSize);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}

String formatValue(String value)
{
  if (value.length() < 2)
  {
    value = "0" + value;
  }

  return value;
}

struct textCenter centerText(String value)
{
  textCenter center;

  center.horizontal = LCD_WIDTH / 2 - M5.Lcd.textWidth(value) / 2;
  center.vertical = LCD_HEIGHT / 2 - M5.Lcd.fontHeight() / 2;

  return center;
}

void drawCenteredText(String text, int horizontal, int vertical)
{
  M5.Lcd.drawString(text, horizontal, vertical);
}

void drawCenteredText(String text, int vertical)
{
  textCenter center = centerText(text);

  drawCenteredText(text, center.horizontal, vertical);
}

void drawCenteredText(String text)
{
  textCenter center = centerText(text);

  drawCenteredText(text, center.horizontal, center.vertical);
}

// source - https://stackoverflow.com/questions/40517192/c-day-of-week-for-given-date
int calcDayNumFromDate(int y, int m, int d)
{
  m = (m + 9) % 12;
  y -= m / 10;
  int dn = 365 * y + y / 4 - y / 100 + y / 400 + (m * 306 + 5) / 10 + (d - 1);

  return dn;
}

#endif
