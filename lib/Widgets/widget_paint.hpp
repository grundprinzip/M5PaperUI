#pragma once

#include "widget.hpp"

class Paint : public Widget {
public:
  Paint(int16_t x, int16_t y, int16_t w, int16_t h) : Widget(x, y, w, h) {
    widget_style_ = WidgetStyle::BORDER;
    border_color_ = Grayscale::G15;
  }

  virtual ~Paint() {}

  static ptr_t Create(int16_t x, int16_t y, int16_t w, int16_t h) {
    const auto &ptr = std::make_shared<Paint>(x, y, w, h);
    return ptr;
  }

  virtual void Init() override;

  bool EventInside(int16_t x, int16_t y) const override;

  virtual void InternalEventHandler(TouchEvent evt) override;

private:
  int32_t last_ = 0;
};