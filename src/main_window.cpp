#include "main_window.hpp"

void MainWindow::Init(const WidgetContext::ptr_t &ctx) {

  top_bar_ = Frame::Create(0, 0, 540, 40);
  top_bar_->UpdateMode(UPDATE_MODE_DU);

  // Border
  auto border = Widget::Create(0, 0, 540, 40, [](Widget* w){
    w->Style(WidgetStyle::BORDER);
    w->BorderColor(Grayscale::GS_BLACK);
    w->BorderWidth(3);
  });
  top_bar_->AddWidget(border);



  ctx->AddFrame(top_bar_);
  //ctx->AddFrame(bottom_bar_);
}