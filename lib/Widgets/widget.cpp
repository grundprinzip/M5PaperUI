#include "widget.hpp"

void Widget::Init() {
  // Initialize the view to be dirty to draw it the first time.
  view_dirty_ = true;
  int16_t x = has_own_canvas_ ? 0 : x_;
  int16_t y = has_own_canvas_ ? 0 : y_;

  if (widget_style_.ShouldDraw(WidgetStyle::BORDER)) {
    log_d("Drawing outline");
    // Top
    canvas_->drawFastHLine(x, y, width_, border_width_, border_color_.toInt());
    // Bottom
    canvas_->drawFastHLine(x, y + height_ - 1, width_, border_width_,
                           border_color_.toInt());
    // Left
    canvas_->drawFastVLine(x, y, height_, border_width_, border_color_.toInt());
    // Right
    canvas_->drawFastVLine(x + width_ - 1, y, height_, border_width_,
                           border_color_.toInt());
  }

  if (widget_style_.ShouldDraw(WidgetStyle::FILL_W_BORDER)) {
    log_d("Drawing fill and border");
    canvas_->fillRect(x + border_width_, y + border_width_,
                      width_ - (2 * border_width_),
                      height_ - (2 * border_width_), background_color_.toInt());
  }

  if (widget_style_.ShouldDraw(WidgetStyle::FILL)) {
    log_d("Drawing fill");
    canvas_->fillRect(x, y, width_, height_, background_color_.toInt());
  }
}

bool Widget::Draw() {
  if (view_dirty_) {
    log_d("Drawing widget at %d %d %d %d %d", x_, x_offset_, y_, y_offset_,
          has_own_canvas_);
    if (has_own_canvas_) {
      // Only some update modes support grayscale display.
      auto update_mode = parent_->update_mode();
      bool supportsGrayscale =
          update_mode == UPDATE_MODE_GC16 || update_mode == UPDATE_MODE_INIT ||
          update_mode == UPDATE_MODE_GL16 || update_mode == UPDATE_MODE_GLD16 ||
          update_mode == UPDATE_MODE_GLR16;
      if (!supportsGrayscale && !background_color_.monochrome()) {
        log_d("Update mode of the EPD might not support grayscale display");
      }

      canvas_->pushCanvas(x_offset_, y_offset_, parent_->update_mode());
    }
    view_dirty_ = false;
  }
  return view_dirty_;
}

void Widget::BackgroundColor(Grayscale c) { background_color_ = c; }

void Widget::RegisterHandler(handler_fun_t f) { handlers_.push_back(f); }

void Widget::HandleEvent(TouchEvent evt) {
  InternalEventHandler(evt);
  for (const auto &h : handlers_) {
    h(evt);
  }
}