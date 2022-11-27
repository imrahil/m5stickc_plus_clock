#ifndef _APP_UTILITY_
#define _APP_UTILITY_

#include <Arduino.h>
#include <M5StickCPlus.h>

#define LCD_WIDTH 240
#define LCD_HEIGHT 135

#define BRIGHTNESS_MAX 4

#define LCD_ROTATION 3

String DAYS[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

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

#endif
