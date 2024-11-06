#include "global.h"
//#define USE_GYVER_OLED
#define USE_LIQUID_I2C

#include <RTClib.h>
#include "spin_box.h"
#include "i_text_screen.h"
#include "window_stack.h"
#include "main_window.h"

#if defined(USE_GYVER_OLED)
#include "oled_screen.h"
static OLEDScreen screen_impl;
ITextScreen& screen = screen_impl;
#elif defined(USE_LIQUID_I2C)
#include "lcd_screen.h"
static LCDScreen screen_impl;
ITextScreen& screen = screen_impl;
#else
#error No Screen defined
#endif

WindowStack window_stack;
RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  screen.init();
  rtc.begin();
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);

  Serial.println(F("OK"));

  window_stack.pushWindow(new MainWindow);
}

void loop() {
  //Todo: process HW buttons here from slave device via interrupt and i2c/spi
  if (Serial.available()) {
    char input = Serial.read();
    window_stack.sendButton(input);
  }

  window_stack.update();
}
