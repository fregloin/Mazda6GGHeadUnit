#pragma once
#include "global.h"

class IWindow;

class IWindowStack {
public:
  virtual ~IWindowStack() = default;
  virtual void sendButton(char button) = 0;
  virtual void pushWindow(IWindow* window) = 0;
  virtual void back() = 0;
  virtual IWindow* currentWindow() const = 0;
};
