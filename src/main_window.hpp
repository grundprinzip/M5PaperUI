#pragma once

#include "widgetlib.hpp"

class MainWindow {

public:
  void Init(const WidgetContext::ptr_t &ctx);

private:
  Frame::ptr_t top_bar_;
  Frame::ptr_t bottom_bar_;
};