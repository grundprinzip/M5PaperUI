#include <M5EPD.h>

#include "widget_frame.hpp"
#include "widget.hpp"

void Frame::AddWidget(const Widget::ptr_t &w) {
  w->ParentFrame(this);
  widgets_.push_back(w);
}

void Frame::Init() {
  canvas_.createCanvas(width_, height_);
  for (const auto &w : widgets_) {
    w->Init();
  }
}

void Frame::Draw() {
  if (state_ == WidgetState::PRE) {
    log_d("Drawing widgets");
    for (const auto &w : widgets_) {
      w->Draw();
    }

    log_d("Pushing canvas");
    canvas_.pushCanvas(0, 0, update_mode_);
    state_ = WidgetState::POST;
  }
}