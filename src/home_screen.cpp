#include "home_screen.hpp"
#include "image_resources.h"

HomeScreen::HomeScreen(int16_t x, int16_t y, int16_t w, int16_t h) : Frame() {
  x_ = x;
  y_ = y;
  width_ = w;
  height_ = h;
  update_mode_ = UPDATE_MODE_GC16;
  name_ = "HomeScreen";
}

void HomeScreen::CreateAppButton(int16_t x, int16_t y, const std::string &name,
                                 const uint8_t *icon,
                                 WButton::handler_fun_t fun) {
  auto button = WIconButton::Create(x, y, 130, 130, icon, 128, 128);
  button->Style(WidgetStyle::BORDER);
  button->BorderColor(Grayscale::GS_BLACK);
  button->RegisterHandler(fun);
  button->Name(name);
  AddWidget(button);

  auto paint_label = Label::Create(x, y + 130 + 10, 130, 20, name);
  paint_label->HAlign(Label::MIDDLE);
  paint_label->Name(name);
  AddWidget(paint_label);
}

void HomeScreen::Prepare(WidgetContext *ctx) {
  auto home_dim = dimension();
  AddWidget(Widget::Create(0, 0, home_dim.w - 1, home_dim.h - 1, [](Widget *w) {
    w->Style(WidgetStyle::BORDER);
    w->BorderColor(Grayscale::GS_BLACK);
    w->BorderWidth(3);
    w->Name("MainBorder");
  }));

  CreateAppButton(10, 30, "Paint", PAINTBRUSH, [home_dim, ctx](TouchEvent e) {
    if (e.type != EventType::TOUCH_UP)
      return;
    ctx->PopFrame();
    // Create the paint frame
    auto pf = Frame::Create(home_dim.x, home_dim.y, home_dim.w, home_dim.h);
    pf->UpdateMode(UPDATE_MODE_GC16);
    auto wpaint = Paint::Create(0, 0, home_dim.w, home_dim.h);
    wpaint->Style(WidgetStyle::BORDER);
    wpaint->BorderColor(Grayscale::G15);
    pf->AddWidget(wpaint);
    ctx->AddFrame(pf);
  });
}

void HomeScreen::Init(WidgetContext *ctx) {
  // The buttons need to be added before the frame is intialized.
  if (!hs_initialized_) {
    Prepare(ctx);
    hs_initialized_ = true;
  }
  Frame::Init(ctx);
}