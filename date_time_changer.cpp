#include "window_stack.h"
#include "date_time_changer.h"

extern WindowStack window_stack;
extern RTC_DS3231 rtc;
extern ITextScreen& screen;

DateTimeChanger::DateTimeChanger() {
  screen.clear();

  auto dt = rtc.now();
  yearSpin_.setValue(dt.year());
  monthSpin_.setValue(dt.month());
  daySpin_.setValue(dt.day());
  hourSpin_.setValue(dt.hour());
  minuteSpin_.setValue(dt.minute());
  normalizeDate();

  for (auto& node : nodes_) {
    node.spinBox->setSpinListener(this);
  }

  updateDateTime();
}

DateTime DateTimeChanger::dt() const { 
  return DateTime(yearSpin_.value(),
    monthSpin_.value(),
    daySpin_.value(),
    hourSpin_.value(),
    minuteSpin_.value());
}

void DateTimeChanger::onSpin(const SpinBox<int16_t>& sender, int16_t value) {
  switch (sender.tag()) {
    case kYear: {
      updateItem(kYearCol, 0, NUM_4, value);
      normalizeDate();
      screen.update();
    }
      break;
    case kMonth: {
      updateItem(kMonthCol, 0, NUM_2, value);
      normalizeDate();
      screen.update();
    }
      break;
    case kDay: {
      updateItem(kDayCol, 0, NUM_2, value);
      normalizeDate();
      screen.update();
    }
      break;
    case kHour: {
      updateItem(kHourCol, 0, NUM_2, value);
      screen.update();
    }
      break;
    case kMinute: {
      updateItem(kMinuteCol, 0, NUM_2, value);
      screen.update();
    }
      break;
    default:
      break;
  }
}

void DateTimeChanger::next() {
  screen.setCursor(nodes_[index_].column, 1);
  screen.print(" ");
  index_ = ++index_;
  if (index_ > 4)
    index_ = 0;
  screen.setCursor(nodes_[index_].column, 1);
  screen.print("^");
  screen.update();
}

void DateTimeChanger::prev() {
  screen.setCursor(nodes_[index_].column, 1);
  screen.print(" ");
  index_ = --index_;
  if (index_ < 0)
    index_ = 4;
  screen.setCursor(nodes_[index_].column, 1);
  screen.print("^");
  screen.update();
}

void DateTimeChanger::increment() {
  nodes_[index_].spinBox->increment();
}

void DateTimeChanger::decrement() {
  nodes_[index_].spinBox->decrement();
}

void DateTimeChanger::normalizeDate() {
  static const uint8_t dayInMonth[12] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  bool leap_year = ((yearSpin_.value() % 4) == 0);
  uint8_t day_in_month = dayInMonth[monthSpin_.value() - 1];
  uint8_t day_in_month_adj = day_in_month;
  if (monthSpin_.value() == 2 && leap_year) {
    ++day_in_month_adj;
  }
  daySpin_.setMax(day_in_month_adj);
}

void DateTimeChanger::updateItem(int col, int row, const char* format, int16_t value) {
  screen.setCursor(col, row);
  char buffer[32] = {0};
  sprintf(buffer, format, value);
  screen.print(buffer);
}

void DateTimeChanger::updateDateTime() {
  updateItem(kYearCol, 0, NUM_4, yearSpin_.value());
  screen.setCursor(kSep1Col, 0);
  screen.print(".");
  updateItem(kMonthCol, 0, NUM_2, monthSpin_.value());
  screen.setCursor(kSep2Col, 0);
  screen.print(".");
  updateItem(kDayCol, 0, NUM_2, daySpin_.value());

  updateItem(kHourCol, 0, NUM_2, hourSpin_.value());
  screen.setCursor(kSep3Col, 0);
  screen.print(":");
  updateItem(kMinuteCol, 0, NUM_2, minuteSpin_.value());

  screen.update();
}

void DateTimeChanger::onButton(char button) {
  switch (button) {
    case 'a':
      prev();
      break;
    case 'd':
      next();
      break;
    case 'o':
      rtc.adjust(dt());
      window_stack.back();
      return;
    case 'p':
      window_stack.back();
      return;
    case 'w':
      increment();
      break;
    case 's':
      decrement();
      break;
  }
}

void DateTimeChanger::onPause() {}

void DateTimeChanger::onResume() {
  screen.clear();
  screen.update();
}

void DateTimeChanger::update() {
  if (millis() - prev_update_ < 750)
    return;
  screen.setCursor(nodes_[index_].column, 1);
  screen.print(show_cursor ? "^" : " ");
  show_cursor = !show_cursor;
  prev_update_ = millis();
}

#ifdef NAMED_WINDOWS
const char* const DateTimeChanger::title() const {
  static const char* const title = "DateTime";
  return title;
}
#endif