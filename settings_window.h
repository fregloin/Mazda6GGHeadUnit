#pragma once
#include "global.h"
#include "i_text_screen.h"
#include "i_window.h"

class SettingsWindow final : public IWindow {
public:
  SettingsWindow();

  void update() override;
  void onPause() override;
  void onResume() override;
  void onButton(char button) override;
#ifdef NAMED_WINDOWS
  const char* const title() const override;
#endif
private:
  int8_t index_ = 0;

  void left();
  void right();
  void runSettings();
  void updateTitle();
  void reinit();
};