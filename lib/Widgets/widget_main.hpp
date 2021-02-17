#pragma once
#include <cstdint>
#include <memory>
#include <sstream>
#include <stack>

#include <M5EPD.h>

#include "widget_constants.hpp"
#include "widget_frame.hpp"

class WidgetContext {
public:

  using ptr_t = std::shared_ptr<WidgetContext>;

  void AddFrame(const Frame::ptr_t &f) {
    log_d("Adding new frame.");
    // Create new canvas.
    // f->canvas_ = M5EPD_Canvas(driver_);
    view_stack_.push(f);
    // Initialize the view.
    f->Init();
  }

  void Draw();

private:
  /// At least 10px touch distance
  static const int16_t TOUCH_EPSILON = 10;

  enum TouchState { TS_NONE = 0, TS_TOUCH };

  std::stack<Frame::ptr_t> view_stack_;

  TouchState state_ = TS_NONE;
  TouchEvent event_;
};