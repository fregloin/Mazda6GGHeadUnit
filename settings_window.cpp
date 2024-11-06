#include "window_stack.h"
#include "date_time_changer.h"
#include "settings_window.h"

extern WindowStack window_stack;
extern ITextScreen& screen;

static const size_t node_count = 5;
static const char* const nodes[node_count] = {
  "Date and Time",
  "Sound",
  "Display settings",
  "Misc",
  "Help screen"
};

SettingsWindow::SettingsWindow() {
  reinit();
}

void SettingsWindow::update() {}

void SettingsWindow::onPause() {}

void SettingsWindow::onResume() {
  reinit();
}

void SettingsWindow::onButton(char button) {
  switch (button) {
    case 'a':
      left();
      break;
    case 'd':
      right();
      break;
    case 'o':
      runSettings();
      break;
    case 'p':
      window_stack.back();
      break;
    default:
      break;
  }
}

void SettingsWindow::left() {
  if (--index_ < 0)
    index_ = 4;
  updateTitle();
}

void SettingsWindow::right() {
  if (++index_ > node_count - 1)
    index_ = 0;
  updateTitle();
}

void SettingsWindow::runSettings() {
  switch (index_) {
    case 0:
      window_stack.pushWindow(new DateTimeChanger);
      break;
    default:
      break;
  }
}

void SettingsWindow::updateTitle() {
  for (auto i = 0; i < 16; i++) {
    screen.setCursor(i, 1);
    screen.print(" ");
  }
  screen.setCursor(0, 1);
  screen.print(nodes[index_]);
  screen.setCursor(13, 0);
  char buff[16];
  sprintf(buff, "%1d/%1d", index_ + 1, node_count);
  screen.print(buff);
  screen.update();
}

#ifdef NAMED_WINDOWS
const char* const SettingsWindow::title() const {
  static const char* const title = "Settings";
  return title;
}
#endif

void SettingsWindow::reinit() {
  screen.clear();
  screen.setCursor(0, 0);
#ifdef NAMED_WINDOWS
  screen.print(title());
#else
  screen.print("Settings");
#endif
  updateTitle();
}