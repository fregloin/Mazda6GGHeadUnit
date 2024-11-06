#include <stdio.h>
#include <Arduino.h>
#include "window_stack.h"

WindowStack::~WindowStack() {
  while (auto prev = current_->prev) {
    delete(current_);
  }
}

void WindowStack::sendButton(char button) {
  if (current_->window) {
    current_->window->onButton(button);
  }
}

void WindowStack::pushWindow(IWindow* window) {
  if (!window) {
    return;
  }
  if (!root_.window) {
    root_.window = window;
  } else {
    WindowNode* next = new WindowNode;
    next->window = window;
    next->prev = current_;
    current_->window->onPause();
    current_ = next;
  }
}

void WindowStack::back() {
  if (!current_->prev) {
    return; // root node
  }
  auto old = current_;
  current_ = old->prev;
  delete(old);
  current_->window->onResume();
}

IWindow* WindowStack::currentWindow() const {
 return current_->window;
}

void WindowStack::update() {
  if (current_->window) {
    current_->window->update();
  }
}
