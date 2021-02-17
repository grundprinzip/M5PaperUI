#include <M5EPD.h>

#include "widget.hpp"
#include "widget_frame.hpp"

void Frame::AddWidget(const Widget::ptr_t &w) {
  w->ParentFrame(this);

  // If the widget uses it's own canvas, the relative position of the frame has
  // to be propagated to the widget so that the widget can use the absolute
  // position to push the canvas.
  if (w->has_own_canvas()) {
    w->UpdateRelativePosition(x_, y_);
  } else {
    w->SetCanvas(&canvas_);
  }

  widgets_.push_back(w);
}

void Frame::Init() {
  canvas_.createCanvas(width_, height_);
  for (const auto &w : widgets_) {
    w->Init();
  }
}

void Frame::Draw() {
  bool is_view_dirty = NeedsRedraw();

  if (is_view_dirty) {
    log_d("Push Frame Canvas");
    canvas_.pushCanvas(x_, y_, UPDATE_MODE_NONE);
  }

  for (const auto &w : widgets_) {
    is_view_dirty |= w->Draw();
  }

  if (is_view_dirty) {
    log_d("Updating EPD");
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
    const auto &position = w->position();
    int16_t adj_x = x - x_ - position.x;
    int16_t adj_y = y - y_ - position.y;
    if (w->EventInside(adj_x, adj_y)) {
      return true;
    }
  }
  return false;
}

void Frame::HandleEvent(TouchEvent evt) {
  for (const auto &w : widgets_) {
    const auto &position = w->position();
    int16_t adj_x = evt.x1 - x_ - position.x;
    int16_t adj_y = evt.y1 - y_ - position.y;
    if (w->EventInside(adj_x, adj_y)) {
      w->HandleEvent(evt);
    }
  }
}