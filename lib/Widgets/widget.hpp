#pragma once
#include <M5EPD.h>
#include <cstdint>
#include <memory>

#include "widget_constants.hpp"
#include "widget_frame.hpp"



/// A widget is a displayable element on the screen. It has a the required
/// visual properties and an update mode.
class Widget {

public:
  using ptr_t = std::shared_ptr<Widget>;

  static ptr_t Create(int16_t x_offset, int16_t y_offset, int16_t width,
                      int16_t height) {
    return std::make_shared<Widget>(width, height, x_offset, y_offset);
  }

  Widget(int16_t x_offset, int16_t y_offset, int16_t width, int16_t height)
      : width_(width), height_(height), x_offset_(x_offset),
        y_offset_(y_offset) {
        }

  virtual void Init();

  virtual void Draw();

  inline void Style(WidgetStyle style) { widget_style_ = style; }

  inline void BorderColor(Grayscale c) { border_color_ = c; }

  void BackgroundColor(Grayscale c);

  inline void ParentFrame(Frame* f) {
    parent_ = f;
  }

protected:
  // Width of the widgets.
  int16_t width_;
  // Height of the widget.
  int16_t height_;
  // x offset of the origin of the screen.
  int16_t x_offset_;
  // y offset from the origin of the screen.
  int16_t y_offset_;
  // Background color
  Grayscale background_color_ = Grayscale::G0;
  // Border color
  Grayscale border_color_ = Grayscale::G0;
  // Width of the outline.
  int16_t border_width_ = 1;

  WidgetStyle widget_style_ = WidgetStyle::NONE;

  // This is the actual canvas this widget is drawn on. In principle, this is
  // the display library.
  Frame* parent_;


};