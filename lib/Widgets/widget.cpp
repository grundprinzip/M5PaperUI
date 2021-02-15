#include "widget.hpp"

void Widget::Init() {
  auto canvas = parent_->canvas();
  if (widget_style_.ShouldDraw(WidgetStyle::BORDER)) {
    log_d("Drawing outline");
    canvas->drawFastHLine(0, 0, width_, border_color_.toInt());
    canvas->drawFastHLine(0, height_ - 1, width_, border_color_.toInt());
    canvas->drawFastVLine(0, 0, height_, border_color_.toInt());
    canvas->drawFastVLine(width_ - 1, 0, height_, border_color_.toInt());
  } else if (widget_style_.ShouldDraw(WidgetStyle::FILL_W_BORDER)) {
    log_d("Drawing fill and border");
    canvas->drawFastHLine(0, 0, width_, border_color_.toInt());
    canvas->drawFastHLine(0, height_ - 1, width_, border_color_.toInt());
    canvas->drawFastVLine(0, 0, height_, border_color_.toInt());
    canvas->drawFastVLine(width_ - 1, 0, height_, border_color_.toInt());
    canvas->fillRect(1, 1, width_ - 2, height_ - 2, background_color_.toInt());
  } else if (widget_style_.ShouldDraw(WidgetStyle::FILL)) {
    log_d("Drawing fill");
    canvas->fillRect(0, 0, width_, height_, background_color_.toInt());
  }
}

void Widget::Draw() {
  // Only some update modes support grayscale display.
  auto update_mode = parent_->update_mode();
  bool supportsGrayscale =
      update_mode == UPDATE_MODE_GC16 || update_mode == UPDATE_MODE_INIT ||
      update_mode == UPDATE_MODE_GL16 || update_mode == UPDATE_MODE_GLD16 ||
      update_mode == UPDATE_MODE_GLR16;
  if (!supportsGrayscale && !background_color_.monochrome()) {
    log_d("Update mode of the EPD might not support grayscale display");
  }
}

void Widget::BackgroundColor(Grayscale c) { background_color_ = c; }