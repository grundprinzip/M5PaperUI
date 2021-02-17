#include "widget_paint.hpp"

void Paint::Init() {
  log_d("New Canvas %d x %d", width_, height_);
  canvas_.createCanvas(width_, height_);
  Widget::Init();
}

bool Paint::EventInside(int16_t x, int16_t y) const {
  return x_offset_ <= x && x < (x_offset_ + width_) && y_offset_ <= y &&
         y < (y_offset_ + height_);
}

void Paint::InternalEventHandler(TouchEvent evt) {
  last_ = last_ == 0 ? millis() : last_;
  int32_t now = millis();
  // if (now - last_ > 2000 || evt.type == EventType::TOUCH_UP) {
  if (evt.type == EventType::TOUCH_UP) {
    parent_->UpdateMode(UPDATE_MODE_GLD16);
    parent_->RequireRedraw();
    last_ = now;
  }
  // log_d("Drawing: %d %s", last_, evt.str().c_str());
  for (int16_t i = 0; i < 10; ++i) {
    for (int16_t j = 0; j < 10; ++j) {
      canvas_.drawPixel(evt.x2 + i, evt.y2 + j, Grayscale::G15);
    }
  }
  // canvas_.drawString("X", evt.x2, evt.y2);
}