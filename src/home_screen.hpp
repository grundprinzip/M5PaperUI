#pragma once
#include <memory>

#include "widgetlib.hpp"

class HomeScreen : public Frame {
public:
  using ptr_t = std::shared_ptr<HomeScreen>;

  HomeScreen(int16_t x, int16_t y, int16_t w, int16_t h);

  static ptr_t Create(int16_t x, int16_t y, int16_t w, int16_t h) {
    return std::make_shared<HomeScreen>(x, y, w, h);
  }

  /// Initializes all views that have been added to the frame.
  void Init(WidgetContext *) override;

private:
  void Prepare(WidgetContext *ctx);
  void CreateAppButton(int16_t x, int16_t y, const std::string &name,
                       const uint8_t *icon, WButton::handler_fun_t fun);

  WIconButton::ptr_t paint_button_;
  WIconButton::ptr_t settings_button_;
  bool hs_initialized_ = false;
};