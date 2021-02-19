#pragma once
#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

#include <M5EPD.h>

#include "widget_constants.hpp"
#include "widget_frame.hpp"

class WidgetContext {
public:
  using ptr_t = std::shared_ptr<WidgetContext>;

  /// Adds a new frame to the list of changes that are pushed at the end of the
  /// drawing cycle. New views are assumed to require drawing.
  void AddFrame(const View::ptr_t &f) {
    log_d("Adding new frame.");
    view_changes_.push_back(f);
    f->Init(this);
  }

  void PopFrame() { remove_last_ = true; }

  void Draw();

private:
  /// At least 10px touch distance
  static const int16_t TOUCH_EPSILON = 10;

  enum TouchState { TS_NONE = 0, TS_TOUCH };

  struct RawEvent {
    tp_finger_t evt;
    bool touch;
    uint8_t fingers;

    // Only compare the main properties.
    bool operator==(const RawEvent &o) const {
      return fingers == o.fingers && touch == o.touch && evt.x == o.evt.x &&
             evt.y == o.evt.y;
    }

    bool operator!=(const RawEvent &o) const { return !(*this == o); }
  };

  void HandleTouchEvents();

  std::vector<View::ptr_t> view_stack_;

  std::vector<View::ptr_t> view_changes_;

  TouchState state_ = TS_NONE;

  TouchEvent event_;

  RawEvent last_raw_event_;

  bool remove_last_ = false;
};
