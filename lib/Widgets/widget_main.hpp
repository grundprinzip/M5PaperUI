#pragma once
#include <memory>
#include <stack>

#include <M5EPD.h>

#include "widget_frame.hpp"

class WidgetContext {
public:
  using ptr_t = std::shared_ptr<WidgetContext>;

  WidgetContext(M5EPD_Driver* driver) : driver_(driver) {}

  void AddFrame(const Frame::ptr_t& f) {
    log_d("Adding new frame.");
    // Create new canvas.
    //f->canvas_ = M5EPD_Canvas(driver_);
    view_stack_.push(f);
    // Initialize the view.
    f->Init();
  }

  void Draw();

private:
  std::stack<Frame::ptr_t> view_stack_;
  M5EPD_Driver* driver_;
};

void WidgetMainLoop(const WidgetContext::ptr_t &ctx);