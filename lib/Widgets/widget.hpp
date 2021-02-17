#pragma once
#include <M5EPD.h>
#include <cstdint>
#include <memory>
#include <vector>
#include <functional>

#include "widget_constants.hpp"
#include "widget_frame.hpp"

/// A widget is a displayable element on the screen. It has a the required
/// visual properties and an update mode.
class Widget {

public:
  using ptr_t = std::shared_ptr<Widget>;

  // Function pointer to the handler
  using handler_fun_t = std::function<void(TouchEvent)>;

  static ptr_t Create(int16_t x_offset, int16_t y_offset, int16_t width,
                      int16_t height) {
    return std::make_shared<Widget>(x_offset, y_offset, width, height);
  }

  Widget(int16_t x_offset, int16_t y_offset, int16_t width, int16_t height)
      : width_(width), height_(height), x_offset_(x_offset),
        y_offset_(y_offset), canvas_(&M5.EPD) {}

  virtual ~Widget() {}

  virtual void Init();

  virtual void Draw();

  inline void Style(WidgetStyle style) { widget_style_ = style; }

  inline void BorderColor(Grayscale c) { border_color_ = c; }

  void BackgroundColor(Grayscale c);

  inline void ParentFrame(Frame *f) { parent_ = f; }

  virtual bool EventInside(int16_t x, int16_t y) const { return false; }

  // Register an external event handler for this widget. The external event
  // handler is called as soon as internal event handling is done.
  void RegisterHandler(handler_fun_t f);

  // This function handles the event procedure for the widget. Before calling
  // any registered external event handler, it will call InternalEventHandler
  // once for the widget.
  void HandleEvent(TouchEvent evt);

protected:

  // Helper method that allows a widget to response to any event.
  virtual void InternalEventHandler(TouchEvent evt) {}

  void NeedsNewCanvas(bool flag) {
    needs_new_canvas_ = flag;
  }

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

  // List of handlers associated with this widget.
  std::vector<handler_fun_t> handlers_;

  // This is the actual canvas this widget is drawn on. In principle, this is
  // the display library.
  Frame *parent_;

  M5EPD_Canvas canvas_;

  bool needs_new_canvas_ = true;
};