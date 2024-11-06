#pragma once
#include "global.h"
#include <stdint.h>
#include "i_window.h"
#include "i_window_stack.h"

class WindowStack : public IWindowStack {
public:
  ~WindowStack();
  void sendButton(char button) override;
  void pushWindow(IWindow* window) override;
  void back() override;
  void update();

  IWindow* currentWindow() const override;

private:
  struct WindowNode {
    WindowNode* prev = nullptr;
    IWindow* window = nullptr;
  };

  WindowNode root_;
  WindowNode* current_ = &root_;
};