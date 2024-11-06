#pragma once
#include "global.h"
#include <stdint.h>
#include <RTClib.h>
#include "i_text_screen.h"
#include "i_window.h"
#include "i_window_stack.h"
#include "spin_box.h"

enum ItemType {kMin = 0, kYear = kMin, kMonth, kDay, kHour, kMinute, kMax = kMinute};

class DateTimeChanger : public IWindow, public ISpinnable<int16_t> {
public:
  DateTimeChanger();
  DateTime dt() const;
  void onSpin(const SpinBox<int16_t>& sender, int16_t value);
  void onButton(char button);
  void next();
  void prev();
  void increment();
  void decrement();
  void update();
  void onPause() override;
  void onResume() override;
#ifdef NAMED_WINDOWS
  const char* const title() const override;
#endif

private:
  void normalizeDate();
  void updateItem(int col, int row, const char* format, int16_t value);
  void updateDateTime();

private:
  enum Column {
    kYearCol = 0,
    kSep1Col = 4,
    kMonthCol = 5,
    kSep2Col = 7,
    kDayCol = 8,
    kHourCol = 11,
    kSep3Col = 13,
    kMinuteCol = 14
  };
// Use signed int16 to avoid underflow on decrement
  SpinBox<int16_t> yearSpin_   {2000, 2030, kYear};
  SpinBox<int16_t> monthSpin_  {1, 12, kMonth};
  SpinBox<int16_t> daySpin_    {1, 31, kDay};
  SpinBox<int16_t> hourSpin_   {0, 23, kHour};
  SpinBox<int16_t> minuteSpin_ {0, 59, kMinute};

  struct Node {
    SpinBox<int16_t>* spinBox;
    uint8_t  column;
  };

  Node nodes_[5] = {
    {&yearSpin_, kYearCol},
    {&monthSpin_, kMonthCol},
    {&daySpin_, kDayCol},
    {&hourSpin_, kHourCol},
    {&minuteSpin_, kMinuteCol}
  };

  static const char* const NUM_4 PROGMEM = "%04d";
  static const char* const NUM_2 PROGMEM = "%02d";

  bool show_cursor = true;
  int8_t index_ = 0;
  uint32_t prev_update_ = millis();
};