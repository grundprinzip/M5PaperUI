#include "main_window.hpp"

void MainWindow::Init(WidgetContext *ctx) {

  top_bar_ = Frame::Create(0, 0, 540, 40);
  top_bar_->UpdateMode(UPDATE_MODE_DU);

  // Border
  auto border = Widget::Create(0, 0, 540, 40, [](Widget* w){
    w->Style(WidgetStyle::BORDER);
    w->BorderColor(Grayscale::GS_BLACK);
    w->BorderWidth(3);
  });
  top_bar_->AddWidget(border);

  // Time
  auto w_time = Label::Create(220, 10, 100, 25, "09:32", 2);
  w_time->HAlign(Label::MIDDLE);
  top_bar_->AddWidget(w_time);

  top_bar_->Init(ctx);

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
  bottom_bar_->AddWidget(back);

  auto home = WButton::Create(350, 10, 100, 60, "Home");
  home->TextSize(3);
  home->Style(WidgetStyle::BORDER);
  home->BorderColor(Grayscale::G15);
  bottom_bar_->AddWidget(home);

  bottom_bar_->Init(ctx);
}

void MainWindow::Draw() {
  top_bar_->Draw();
  bottom_bar_->Draw();
}

bool MainWindow::EventInside(int16_t x, int16_t y) const {
  if (top_bar_->EventInside(x, y))
    return true;
  return bottom_bar_->EventInside(x, y);
}

void MainWindow::HandleEvent(TouchEvent evt) {
  if (top_bar_->EventInside(evt.x1, evt.y1))
    top_bar_->HandleEvent(evt);
  if (bottom_bar_->EventInside(evt.x1, evt.y1))
    bottom_bar_->HandleEvent(evt);
}