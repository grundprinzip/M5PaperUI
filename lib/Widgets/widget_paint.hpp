#pragma once

#include "widget.hpp"

/**
 * This is a sample widget that lets the user draw with their fingers on the
 * screen. The main logic is handled inside the internal event handler.
 *
 * For every event that is triggered by the main event loop, this widget draws a
 * rectangle on the screen. The size of the rectangle depends on the size of the
 * recorded touch event.
 *
 * Only when the finger is lifted from the screen (and an Touch UP event is
 * fired). The widget marks the view as dirty and as a result the view is
 * rendered on the screen.
 * */
class Paint : public Widget {
public:
  Paint(int16_t x, int16_t y, int16_t w, int16_t h) : Widget(x, y, w, h) {
    widget_style_ = WidgetStyle::BORDER;
    border_color_ = Grayscale::G15;
  }

  virtual ~Paint() {}

  static ptr_t Create(int16_t x, int16_t y, int16_t w, int16_t h) {
    const auto &ptr = std::make_shared<Paint>(x, y, w, h);
    ptr->NeedsOwnCanvas();
    return ptr;
  }

  virtual void Init() override;

  void Reset() override;

  bool EventInside(int16_t x, int16_t y) const override;

  virtual void InternalEventHandler(TouchEvent evt) override;

private:
  int32_t last_ = 0;
};