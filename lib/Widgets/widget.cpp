#include "widget.hpp"

void Widget::Init() {
  if (needs_new_canvas_) {
    canvas_.createCanvas(width_, height_);
  }
  auto canvas = canvas_;
  if (widget_style_.ShouldDraw(WidgetStyle::BORDER)) {
    log_d("Drawing outline");
    canvas_.drawFastHLine(0, 0, width_, border_color_.toInt());
    canvas_.drawFastHLine(0, height_ - 1, width_, border_color_.toInt());
    canvas_.drawFastVLine(0, 0, height_, border_color_.toInt());
    canvas_.drawFastVLine(width_ - 1, 0, height_, border_color_.toInt());
  } else if (widget_style_.ShouldDraw(WidgetStyle::FILL_W_BORDER)) {
    log_d("Drawing fill and border");
    canvas_.drawFastHLine(0, 0, width_, border_color_.toInt());
    canvas_.drawFastHLine(0, height_ - 1, width_, border_color_.toInt());
    canvas_.drawFastVLine(0, 0, height_, border_color_.toInt());
    canvas_.drawFastVLine(width_ - 1, 0, height_, border_color_.toInt());
    canvas_.fillRect(1, 1, width_ - 2, height_ - 2, background_color_.toInt());
  } else if (widget_style_.ShouldDraw(WidgetStyle::FILL)) {
    log_d("Drawing fill");
    canvas_.fillRect(0, 0, width_, height_, background_color_.toInt());
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
  log_d("Drawing widget at %d %d", x_offset_, y_offset_);
  canvas_.pushCanvas(x_offset_, y_offset_, parent_->update_mode());
}

void Widget::BackgroundColor(Grayscale c) { background_color_ = c; }

void Widget::RegisterHandler(handler_fun_t f) { handlers_.push_back(f); }

void Widget::HandleEvent(TouchEvent evt) {
  InternalEventHandler(evt);
  for (const auto &h : handlers_) {
    h(evt);
  }
}