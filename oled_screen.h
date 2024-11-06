#pragma once
#include "global.h"
#include <GyverOLED.h>

class OLEDScreen final : public ITextScreen {
public:
  void println(const char* text) override {
    oled_.println(text);
  }

  void print(const char* text) override {
    oled_.print(text);
  }

  void setCursor(int col, int row) override {
    oled_.setCursor(col * 6, row);
  }

  void update() override {
    oled_.update();
  }

  void clear() override {
    oled_.clear();
  }

  void init() override {
    oled_.init();
    oled_.clear();
  }

  inline GyverOLED<SSD1306_128x32, OLED_NO_BUFFER>& screen() {
    return oled_;
  }

protected:
  GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled_;
};
