#include <M5EPD.h>

#include "widget.hpp"
#include "widget_frame.hpp"

void Frame::AddWidget(const Widget::ptr_t &w) {
  w->ParentFrame(this);
  widgets_.push_back(w);
}

void Frame::Init() {
  for (const auto &w : widgets_) {
    w->Init();
  }
}

void Frame::Draw() {
  if (NeedsRedraw()) {
    log_d("Drawing widgets");
    for (const auto &w : widgets_) {
      w->Draw();
    }

    log_d("Pushing canvas");
    state_ = WidgetState::POST;
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);
  }
}

bool Frame::EventInside(int16_t x, int16_t y) const {
  // Check the bounds.
  if (x < x_) {
    return false;
  }
  if (x > (x_ + width_)) {
    return false;
  }
  if (y < y_) {
    return false;
  }
  if (y > y_ + height_) {
    return false;
  }
  for (const auto &w : widgets_) {
    if (w->EventInside(x, y)) {
      return true;
    }
  }
  return false;
}

void Frame::HandleEvent(TouchEvent evt) {
  for (const auto &w : widgets_) {
    if (w->EventInside(evt.x1 - x_, evt.y1 - y_)) {
      w->HandleEvent(evt);
    }
  }
}