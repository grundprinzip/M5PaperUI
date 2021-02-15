#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <M5EPD.h>

enum class WidgetState { PRE, UDPATE, POST };

// Forward declaration.
class Widget;

class Frame {
public:
  using ptr_t = std::shared_ptr<Frame>;

  Frame(): canvas_(&M5.EPD) {}

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

  inline void UpdateMode(m5epd_update_mode_t u) { update_mode_ = u; }

  void AddWidget(const std::shared_ptr<Widget>& w);

  M5EPD_Canvas* canvas() {
    return &canvas_;
  }

  m5epd_update_mode_t update_mode() const {
    return update_mode_;
  }

protected:
  int16_t x_;
  int16_t y_;
  int16_t width_;
  int16_t height_;

  // Update mode for this widget.
  m5epd_update_mode_t update_mode_;

  std::vector<std::shared_ptr<Widget>> widgets_;

  WidgetState state_ = WidgetState::PRE;

public:
  M5EPD_Canvas canvas_;
};