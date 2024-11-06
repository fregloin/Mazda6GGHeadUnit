#pragma once
#include "global.h"

class IWindow {
public:
  virtual ~IWindow() = default;
  virtual void onButton(char button) = 0;
  virtual void update() = 0;
  virtual void onPause() = 0;
  virtual void onResume() = 0;
#ifdef NAMED_WINDOWS
  virtual const char* const title() const = 0;
#endif
};