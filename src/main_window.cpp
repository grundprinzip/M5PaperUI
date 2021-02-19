#include "main_window.hpp"

void MainWindow::InitializeTopBar(const WidgetContext::ptr_t &ctx) {
  top_bar_ = Frame::Create(0, 0, 540, 40);
  top_bar_->UpdateMode(UPDATE_MODE_DU);

  // Border
  auto border = Widget::Create(0, 0, 540, 40, [](Widget *w) {
    w->Style(WidgetStyle::BORDER);
    w->BorderColor(Grayscale::GS_BLACK);
    w->BorderWidth(3);
  });
  top_bar_->AddWidget(border);

  // Time
  auto w_time = Label::Create(270, 10, 100, 25, "09:32", 2);
  w_time->HAlign(Label::MIDDLE);
  top_bar_->AddWidget(w_time);

  ctx->AddFrame(top_bar_);
}

void MainWindow::InitializeBottomBar(const WidgetContext::ptr_t &ctx) {
  bottom_bar_ = Frame::Create(0, 880, 540, 80);
  bottom_bar_->UpdateMode(UPDATE_MODE_DU);
  // Bottom bar
  auto bot_border = Widget::Create(0, 0, 540, 80, [](Widget *w) {
    w->Style(WidgetStyle::BORDER);
    w->BorderColor(Grayscale::GS_BLACK);
    w->BorderWidth(3);
  });
  bottom_bar_->AddWidget(bot_border);

  // Add Two buttons back and home
  auto back = WButton::Create(100, 10, 100, 60, "Back");
  back->TextSize(3);
  back->Style(WidgetStyle::BORDER);
  back->BorderColor(Grayscale::G15);
  back->Padding(20);
  auto self = shared_from_this();
  back->RegisterHandler([this, self, ctx](TouchEvent evt) {
    if (evt.type == EventType::TOUCH_UP && !this->home_screen_->NeedsRedraw()) {
      log_d("Dropping frame");
      ctx->PopFrame();
      ctx->AddFrame(this->home_screen_);
    }
  });
  bottom_bar_->AddWidget(back);

  auto home = WButton::Create(350, 10, 100, 60, "Home");
  home->TextSize(3);
  home->Style(WidgetStyle::BORDER);
  home->BorderColor(Grayscale::G15);
  home->Padding(20);
  bottom_bar_->AddWidget(home);

  ctx->AddFrame(bottom_bar_);
}

void MainWindow::InitializeHomeScreen(const WidgetContext::ptr_t &ctx) {
  auto top_dim = top_bar_->dimension();
  auto bot_dim = bottom_bar_->dimension();
  // Create the frame
  home_screen_ =
      Frame::Create(2, top_dim.h + 2, 536, 960 - 8 - top_dim.h - bot_dim.h);
  home_screen_->UpdateMode(UPDATE_MODE_GC16);
  auto home_dim = home_screen_->dimension();
  log_d("Home Dim %s", home_dim.str().c_str());

  // Add the border around the frame
  home_screen_->AddWidget(
      Widget::Create(0, 0, home_dim.w - 1, home_dim.h - 1, [](Widget *w) {
        w->Style(WidgetStyle::BORDER);
        w->BorderColor(Grayscale::GS_BLACK);
        w->BorderWidth(3);
      }));

  // Add the buttons for the Applications
  auto paint = WButton::Create(10, 50, 150, 150, "Paint");
  paint->TextSize(3);
  paint->Style(WidgetStyle::BORDER);
  paint->BorderColor(Grayscale::G15);
  paint->Padding(20);
  paint->RegisterHandler([home_dim, ctx](TouchEvent e) {
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
  home_screen_->AddWidget(paint);

  ctx->AddFrame(home_screen_);
}

void MainWindow::Start(const WidgetContext::ptr_t &ctx) {
  InitializeTopBar(ctx);
  InitializeBottomBar(ctx);
  InitializeHomeScreen(ctx);
}