#include <M5EPD.h>

#include "widget.hpp"
#include "widget_frame.hpp"

void Frame::AddWidget(const Widget::ptr_t &w) {
  log_d("AddWidget - %s", w->name().c_str());
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

void Frame::Init(WidgetContext *) {
  // If the view was already initialized and is added again to the context,
  // Init() will be called again. However, in this case, we need to dirty all
  // views and simply call Draw().
  if (initialized_) {
    log_d("Frame %s is already initialized.", name_.c_str());
    RequireRedraw();
    for (const auto &w : widgets_) {
      w->Reset();
    }
    return;
  }
  log_d("Frame %s initializing.", name_.c_str());
  canvas_.createCanvas(width_, height_);
  for (const auto &w : widgets_) {
    w->Init();
  }
  initialized_ = true;
}

ScreenUpdateMode Frame::Draw() {
  bool full_refresh = NeedsRedraw();
  if (full_refresh) {
    log_d("Drawing frame %s with %d widgets", name_.c_str(), widgets_.size());
    log_d("Push Frame Canvas");
    canvas_.pushCanvas(x_, y_, update_mode_);
  }

  ScreenUpdateMode mode = ScreenUpdateMode::NONE;
  for (const auto &w : widgets_) {
    if (w->Draw())
      mode = ScreenUpdateMode::PARTIAL;
  }

  if (full_refresh) {
    log_d("Updating EPD");
    state_ = WidgetState::POST;
    return ScreenUpdateMode::FULL;
  }
  return mode;
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