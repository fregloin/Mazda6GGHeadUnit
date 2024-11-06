#pragma once
#include "global.h"

class ITextScreen {
public:
  virtual ~ITextScreen() = default;
  virtual void println(const char* text) = 0;
  virtual void print(const char* text) = 0;
  virtual void setCursor(int col, int row) = 0;
  virtual void update() = 0;
  virtual void clear() = 0;
  virtual void init() = 0;
};
