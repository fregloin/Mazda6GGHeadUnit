#pragma once
#include "global.h"
#include <LiquidCrystal_I2C.h>

class LCDScreen final : public ITextScreen {
public:
  void println(const char* text) override {
    lcd_.println(text);
  }

  void print(const char* text) override {
    lcd_.print(text);
  }

  void setCursor(int col, int row) override {
    lcd_.setCursor(col, row);
  }

  void update() override {}

  void clear() override {
    lcd_.clear();
  }

  void init() override {
    lcd_.init();
    lcd_.backlight();
  }

  inline LiquidCrystal_I2C& screen() {
    return lcd_;
  }

protected:
  LiquidCrystal_I2C lcd_ {0x27, 16, 2};
};
