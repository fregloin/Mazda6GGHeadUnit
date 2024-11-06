#include <Arduino.h>
#include <RTClib.h>
#include "settings_window.h"
#include "window_stack.h"
#include "main_window.h"

extern WindowStack window_stack;
extern RTC_DS3231 rtc;
extern ITextScreen& screen;

#ifndef USE_1HZ_INTERRUPT
#warning Using hardware 1HZ iterrupts
#endif

static PGM_P dayInWords[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
static PGM_P monthInWords[12] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
  "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
static PGM_P date_format = "%02d/%3s/%04d %3s";
static PGM_P time_format = "%02d:%02d:%02d %3d\xDF";

MainWindow::MainWindow() {
  screen.clear();
  screen.update();
#ifdef USE_1HZ_INTERRUPT
  attachInterrupt(INT0, timeISR, CHANGE);
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
#endif
}

MainWindow::~MainWindow() {
#ifdef USE_1HZ_INTERRUPT
  rtc.writeSqwPinMode(DS3231_OFF);
  detachInterrupt(INT0);
#endif
}

void MainWindow::onButton(char button) {
  switch (button) {
    case 'o': {
      window_stack.pushWindow(new SettingsWindow);
    }
      break;
    default:
      break;
  }
}

void MainWindow::update() {
#ifdef USE_1HZ_INTERRUPT
  if (!time_ticked)
    return;
  time_ticked = false;
#else
  if (millis() - prev_update_ < 500)
    return;
#endif
  auto rtcTime = rtc.now();

  auto ss = rtcTime.second();
  auto mm = rtcTime.minute();
  auto hh = rtcTime.hour();
  auto DD = rtcTime.dayOfTheWeek();
  auto dd = rtcTime.day();
  auto MM = rtcTime.month();
  auto yyyy = rtcTime.year();

  char buffer[32];
  sprintf(buffer, date_format, dd, monthInWords[MM - 1], yyyy, dayInWords[DD]);
  screen.setCursor(0, 0);
  screen.print(buffer);
  screen.update();

  sprintf(buffer, time_format, hh, mm, ss, static_cast<int8_t>(rtc.getTemperature()));

  screen.setCursor(0, 1);
  screen.print(buffer);
  screen.update();
#ifndef USE_1HZ_INTERRUPT
  prev_update_ = millis();
#endif
}

void MainWindow::onPause() {
#ifdef USE_1HZ_INTERRUPT
  rtc.writeSqwPinMode(DS3231_OFF);
#endif
}

void MainWindow::onResume() {
  screen.clear();
  screen.update();
#ifdef USE_1HZ_INTERRUPT
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
#endif
}

#ifdef NAMED_WINDOWS
const char* const MainWindow::title() const {
  static const char* const title = "Main";
  return title;
}
#endif

#ifdef USE_1HZ_INTERRUPT
void MainWindow::timeISR() {
  MainWindow::time_ticked = true;
}
#endif