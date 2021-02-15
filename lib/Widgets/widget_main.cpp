#include "widget_main.hpp"

void WidgetContext::Draw() { view_stack_.top()->Draw(); }

void WidgetMainLoop(const WidgetContext::ptr_t &ctx) {
  while (true) {
    ctx->Draw();
    delay(10);
  }
}