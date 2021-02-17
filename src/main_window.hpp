#pragma once

#include "widgetlib.hpp"

class MainWindow : public View{

public:
  void Init(WidgetContext* ctx);

  void Draw() override;

  bool EventInside(int16_t x, int16_t y) const override;

  void HandleEvent(TouchEvent evt) override;

private:
  Frame::ptr_t top_bar_;
  Frame::ptr_t bottom_bar_;
};