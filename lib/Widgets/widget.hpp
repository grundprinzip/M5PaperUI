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
///
/// The position of a widget is always relative to the frame.
class Widget {

public:
  using ptr_t = std::shared_ptr<Widget>;

  // Function pointer to the handler
  using handler_fun_t = std::function<void(TouchEvent, Widget *)>;

  struct Point {
    int16_t x;
    int16_t y;
  };

  static ptr_t Create(int16_t x_offset, int16_t y_offset, int16_t width,
                      int16_t height) {
    return std::make_shared<Widget>(x_offset, y_offset, width, height);
  }

  /// Helper function to create a widget, takes an additional functor as a
  /// parameter that allows to modify the widget before returning it to the
  /// caller.
  static ptr_t Create(int16_t x_offset, int16_t y_offset, int16_t width,
                      int16_t height, std::function<void(Widget*)> mod) {
    auto w = std::make_shared<Widget>(x_offset, y_offset, width, height);
    // Call the handler to make modifications easier
    mod(w.get());
    return w;
  }

  /// Default constructor.
  Widget(int16_t x_offset, int16_t y_offset, int16_t width, int16_t height)
      : width_(width), height_(height), x_(x_offset), y_(y_offset),
        canvas_(new M5EPD_Canvas(&M5.EPD)) {

  }

  virtual ~Widget() {
    if (has_own_canvas_)
      delete canvas_;
  }

  /// Usually, Init is only called once when a Widget is added to a frame to
  /// prepare the canvas and place the different elements on it. However, in
  /// certain cases, it is possible to call Init() from the Draw() loop to force
  /// rebuilding the canvas multiple times.
  virtual void Init();

  /// Called to either push the canvas to the screen if the widget uses it's own
  /// canvas otherwise the drawing is issued from the parent frame.
  ///
  /// Returns true if the view was dirty. (TODO)
  virtual bool Draw();

  /// Update the style of the widget (NONE, BORDER, FILLED, FILLED_W_BORDER).
  inline void Style(WidgetStyle style) { widget_style_ = style; }

  /// Sets the border color.
  inline void BorderColor(Grayscale c) { border_color_ = c; }

  /// Sets the background color.
  void BackgroundColor(Grayscale c);

  /// Returns a pointer to the parent frame.
  inline void ParentFrame(Frame *f) { parent_ = f; }

  /// Returns true if the coordinates passed to the function are within the bounds of the widget.
  virtual bool EventInside(int16_t x, int16_t y) const {
    return 0 <= x && x < width_ && 0 <= y && y < height_;
  }

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

  /// Makes the Widget use a separate canvas. Calling this method has only
  /// affect when called before Init().
  inline void NeedsOwnCanvas() { has_own_canvas_ = true; }

  /// Returns true if the widget uses its own canvas.
  inline bool has_own_canvas() const { return has_own_canvas_; }

  /// Returns true if the view is dirty and needs to be redrawn.
  inline bool dirty() const { return view_dirty_; }

  /// Returns a point of the origin of the widget.
  inline Point position() const { return {x_, y_}; }

  /// Sets the border width if WidgetStyle::BORDER / FILLED_W_BORDER.
  inline void BorderWidth(int16_t w) { border_width_ = w;}

  /// Returns the padding of the widget.
  inline int16_t padding() const { return padding_; }

  /// Sets the padding of the widget.
  inline void Padding(int16_t p) { padding_ = p; }

  /// When Reset is called, the view is marked as dirty and refreshed in the
  /// next draw loop.
  inline virtual void Reset() { view_dirty_ = true; }

  /// Sets the name of the widget. The name can be used for debugging or
  /// identification purposes.
  inline void Name(const std::string &n) { name_ = n; }

  /// Returns the name of the widget as a string.
  inline std::string name() const { return name_; }

  /// Sets the border style (ROUND / NORMAL)
  inline void SetBorderStyle(BorderStyle s) { border_style_ = s; }

  /// Returns the border style.
  inline BorderStyle border_style() const { return border_style_; }

  /// Sets the border radius. This value is only used when BorderStyle::ROUND is
  /// applied.
  inline void SetBorderRadius(int16_t r) { border_radius_ = r; }

  /// Update mode for the widget. Setting a values has only an effect if the
  /// widget uses its own canvas and the widget is configured to ignore the
  /// update mode of the parent.
  inline void UpdateMode(m5epd_update_mode_t m) { update_mode_ = m; }

  /// By default, the widget applies the screen update mode of the parent to the
  /// widget child as well. Setting this value to true will let the widget
  /// configure its own screen update mode.
  inline void SetChildUpdateMode(bool v) { use_child_update_mode_ = v; }

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

  /// List of handlers associated with this widget.
  std::vector<handler_fun_t> handlers_;

  /// This is the actual canvas this widget is drawn on. In principle, this is
  /// the display library.
  Frame *parent_;

  /// The primary canvas of the widget. If has_own_canvas_ is true, is an
  /// independent canvas, otherwise points to the canvas of the parent.
  M5EPD_Canvas *canvas_ = nullptr;

  /// By default a widget re-uses the canvas of the parent frame.
  bool has_own_canvas_ = false;

  /// If true, will force redrawing the widget in the next loop iteration.
  bool view_dirty_ = true;

  /// Padding between the border and the content.
  int16_t padding_ = 0;
  /// Name of the widget. The name is mainly used for debugging purposes.
  std::string name_;

  // If the border style is set to ROUND, defines the border radius.
  int16_t border_radius_ = 5;
  /// Sets the border style of the widget (NORMAL / ROUND). The value is only
  /// applied if the widget style is set to draw an outline.
  BorderStyle border_style_ = BorderStyle::NONE;
  /// Update mode for the canvas. This value is only applied if
  /// `has_own_canvas_` is true and `use_child_update_mode_` is true.
  m5epd_update_mode_t update_mode_ = UPDATE_MODE_NONE;
  /// If true, allows to configure the screen update mode for the canvas of this
  /// particular widget independent of the parent frame.
  bool use_child_update_mode_ = false;
};