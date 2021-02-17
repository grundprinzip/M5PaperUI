#include "widget_button.hpp"

bool WButton::EventInside(int16_t x, int16_t y) const {
  log_d("%d < %d < %d; %d < %d < %d", 0, x, width_, 0, y,
        height_);
  return x_offset_ <= x && x < (x_offset_ + width_) && y_offset_ <= y &&
         y < (y_offset_ + height_);
}

void WButton::Init() {
  canvas_.createCanvas(width_, height_);
  NeedsNewCanvas(false);
  Widget::Init();
  Label::Init();
}

void WButton::Draw() {
  log_d("Drawing button at %d %d", x_offset_, y_offset_);
  canvas_.pushCanvas(x_offset_, y_offset_, parent_->update_mode());
}

void WButton::InternalEventHandler(TouchEvent evt) {
  /* if (last_event_ != evt) {
    parent_->RequireRedraw();
    if (evt == EventType::TOUCH_DOWN) {
      WidgetStyle(WidgetStyle::FILL);
      BackgroundColor(Grayscale::G15);
      parent_->UpdateMode(UPDATE_MODE_DU4);
    }

    if (evt == EventType::TOUCH_UP) {
      WidgetStyle(WidgetStyle::BORDER);
      BackgroundColor(Grayscale::G0);
    }
    last_event_ = evt;
  } */
}