#include "main_window.hpp"
#include "home_screen.hpp"
#include "image_resources.h"

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
  auto w_time = Label::Create(270, 10, 100, 25, "09:32", 20);
  w_time->HAlign(Label::MIDDLE);
  top_bar_->AddWidget(w_time);
  top_bar_->Name("TopBarFrame");

  ctx->RegisterFrame(top_bar_);
  ctx->PushFrame(top_bar_->name());
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
  back->TextSize(26);
  back->Style(WidgetStyle::BORDER);
  back->BorderColor(Grayscale::G15);
  back->HAlign(Label::MIDDLE);
  back->VAlign(Label::CENTER);
  auto self = shared_from_this();
  back->RegisterHandler([this, self, ctx](TouchEvent evt, Widget *) {
    if (evt.type == EventType::TOUCH_UP && !this->home_screen_->NeedsRedraw()) {
      log_d("Dropping frame");
      ctx->PopFrame();
      ctx->PushFrame("HomeScreenFrame");
    }
  });
  bottom_bar_->AddWidget(back);

  auto home = WButton::Create(350, 10, 100, 60, "Home");
  home->TextSize(26);
  home->Style(WidgetStyle::BORDER);
  home->BorderColor(Grayscale::G15);
  home->HAlign(Label::MIDDLE);
  home->VAlign(Label::CENTER);
  bottom_bar_->AddWidget(home);
  bottom_bar_->Name("BottomBarFrame");

  ctx->RegisterFrame(bottom_bar_);
  ctx->PushFrame(bottom_bar_->name());
}

void MainWindow::InitializeHomeScreen(const WidgetContext::ptr_t &ctx) {
  auto top_dim = top_bar_->dimension();
  auto bot_dim = bottom_bar_->dimension();
  // Create the frame
  home_screen_ = HomeScreen::Create(2, top_dim.h + 2, 536,
                                    960 - 8 - top_dim.h - bot_dim.h);
  auto home_dim = home_screen_->dimension();
  log_d("Home Dim %s", home_dim.str().c_str());
  home_screen_->Name("HomeScreenFrame");
  ctx->RegisterFrame(home_screen_);
  ctx->PushFrame(home_screen_->name());
}

void MainWindow::Start(const WidgetContext::ptr_t &ctx) {
  InitializeTopBar(ctx);
  InitializeBottomBar(ctx);
  InitializeHomeScreen(ctx);
}