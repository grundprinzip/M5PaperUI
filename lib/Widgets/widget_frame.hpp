#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <M5EPD.h>

enum class WidgetState { PRE, UDPATE, POST };

// Forward declaration.
class Widget;

/// The frame is the center piece when displaying content on the screen. A frame
/// represents the canvas that is drawn on the screen. Different frames can be
/// stacked to display content.
///
/// Each frame contains a list of Widgets (@see Widget) that is displayed. Each
/// Widget is initialized and drawn when needed.
class Frame {
public:
  using ptr_t = std::shared_ptr<Frame>;

  /// Initialize the Frame.
  Frame() {}

  static ptr_t Create(int16_t x, int16_t y, int16_t w, int16_t h) {
    const auto &ptr = std::make_shared<Frame>();
    ptr->x_ = x;
    ptr->y_ = y;
    ptr->width_ = w;
    ptr->height_ = h;
    return ptr;
  }

  void Init();

  void Draw();

  /// Allows setting the update mode for the display. Different update modes
  /// have different properties with regards to refresh time and ghosting on the
  /// display.
  inline void UpdateMode(m5epd_update_mode_t u) { update_mode_ = u; }

  /// Adds a new widget to the list.
  void AddWidget(const std::shared_ptr<Widget> &w);

  /// Return the update mode for the frame.
  m5epd_update_mode_t update_mode() const { return update_mode_; }

  inline bool NeedsRedraw() { return state_ == WidgetState::PRE; }

  inline void RequireRedraw() { state_ = WidgetState::PRE; }

  bool EventInside(int16_t x, int16_t y) const;

  void HandleEvent(TouchEvent evt);

protected:
  int16_t x_;
  int16_t y_;
  int16_t width_;
  int16_t height_;

  /// Update mode for this widget.
  m5epd_update_mode_t update_mode_;

  /// The list of all widgets that belong to this frame.
  std::vector<std::shared_ptr<Widget>> widgets_;

  WidgetState state_ = WidgetState::PRE;
};