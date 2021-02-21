#pragma once
#include <M5EPD.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "widget_constants.hpp"
#include "widget_frame.hpp"

/// A widget is a displayable element on the screen. It has a the required
/// visual properties and an update mode.
class Widget {

public:
  using ptr_t = std::shared_ptr<Widget>;

  // Function pointer to the handler
  using handler_fun_t = std::function<void(TouchEvent)>;

  struct Point {
    int16_t x;
    int16_t y;
  };

  static ptr_t Create(int16_t x_offset, int16_t y_offset, int16_t width,
                      int16_t height) {
    return std::make_shared<Widget>(x_offset, y_offset, width, height);
  }

  static ptr_t Create(int16_t x_offset, int16_t y_offset, int16_t width,
                      int16_t height, std::function<void(Widget*)> mod) {
    auto w = std::make_shared<Widget>(x_offset, y_offset, width, height);
    // Call the handler to make modifications easier
    mod(w.get());
    return w;
  }



  Widget(int16_t x_offset, int16_t y_offset, int16_t width, int16_t height)
      : width_(width), height_(height), x_(x_offset), y_(y_offset),
        canvas_(new M5EPD_Canvas(&M5.EPD)) {
    // Create a new canvas for this on screen component.
    canvas_->createCanvas(width_, height_);
  }

  virtual ~Widget() {
    if (has_own_canvas_)
      delete canvas_;
  }

  virtual void Init();

  virtual bool Draw();

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

  // Replace the old canvas with a new one.
  void SetCanvas(M5EPD_Canvas *c) {
    delete canvas_;
    canvas_ = c;
    has_own_canvas_ = false;
  }

  /// When the widget uses it's onw canvas, we need to convert the relative
  /// positioning of the elements that are drawn to absolute positions. This
  /// means that the x/y positions are reset and the relative positions adjusted
  /// by its old values and the values of the parameters.
  void UpdateRelativePosition(int16_t relative_x, int16_t relative_y) {
    has_own_canvas_ = true;
    x_offset_ = relative_x + x_;
    y_offset_ = relative_y + y_;
  }

  inline void NeedsOwnCanvas() { has_own_canvas_ = true; }

  inline bool has_own_canvas() const { return has_own_canvas_; }

  inline bool dirty() const { return view_dirty_; }

  inline Point position() const { return {x_, y_}; }

  inline void BorderWidth(int16_t w) { border_width_ = w;}

  inline int16_t padding() const { return padding_; }

  inline void Padding(int16_t p) { padding_ = p; }

  inline virtual void Reset() { view_dirty_ = true; }

  inline void Name(const std::string &n) { name_ = n; }

  inline std::string name() const { return name_; }

protected:
  // Helper method that allows a widget to response to any event.
  virtual void InternalEventHandler(TouchEvent evt) {}

  // Width of the widgets.
  int16_t width_;
  // Height of the widget.
  int16_t height_;
  // X position
  int16_t x_;
  // Y position
  int16_t y_;
  // x offset of the origin of the screen.
  int16_t x_offset_ = 0;
  // y offset from the origin of the screen.
  int16_t y_offset_ = 0;
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

  M5EPD_Canvas *canvas_ = nullptr;

  // By default a widget re-uses the canvas of the parent frame.
  bool has_own_canvas_ = false;

  bool view_dirty_ = true;

  /// Padding between the border and the content.
  int16_t padding_ = 0;

  std::string name_;
};