#pragma once
#include "global.h"
#include <stdint.h>
#include "i_window.h"
#include "i_text_screen.h"

class MainWindow final : public IWindow {
public:
  MainWindow();
  ~MainWindow();
  void onButton(char button) override;
  void update() override;
  void onPause() override;
  void onResume() override;
#ifdef NAMED_WINDOWS
  const char* const title() const override;
#endif
private:
#ifdef USE_1HZ_INTERRUPT
  static inline volatile bool time_ticked = false;
  static void timeISR();
#else
  uint32_t prev_update_ = 0;
#endif
};