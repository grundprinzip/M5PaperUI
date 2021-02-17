#include <Arduino.h>
#include <M5EPD.h>


#include "widgetlib.hpp"
#include "main_window.hpp"

WidgetContext::ptr_t ctx;

void setup() {

  M5.begin();
  M5.EPD.SetRotation(90);
  M5.TP.SetRotation(90);
  M5.EPD.Clear(true);
  M5.RTC.begin();

  // put your setup code here, to run once:
  ctx = std::make_shared<WidgetContext>();
  // 540 x 960
  /* auto frame = Frame::Create(50, 50, 300, 600);
  frame->UpdateMode(UPDATE_MODE_NONE);

  // auto p = Paint::Create(10, 10, 520, 900);
  // frame->AddWidget(p);

  auto l = Label::Create(10, 10, 200, 100, "Hello 1", 4);
  frame->AddWidget(l);

  auto l2 = Label::Create(10, 200, 200, 20, "Hello 2", 3);
  l2->NeedsOwnCanvas();

  frame->AddWidget(l2);

  auto w = Widget::Create(10, 200, 50, 50);
  w->BackgroundColor(Grayscale::G15);
  w->Style(WidgetStyle::FILL);

  frame->AddWidget(w);

  auto b = WButton::Create(50, 500, 100, 100, "Button");
  b->Style(WidgetStyle::BORDER);
  b->BorderColor(Grayscale::G15);
  frame->AddWidget(b);

  auto w2 = Widget::Create(0, 0, 300, 600);
  w2->Style(WidgetStyle::BORDER);
  w2->BorderColor(Grayscale::GS_BLACK);
  frame->AddWidget(w2); */

  /*
    // Should be a small box
    auto w = Widget::Create(50, 50, 50, 50);
    w->BackgroundColor(Grayscale::G15);
    w->Style(WidgetStyle::BORDER);
    w->BorderColor(Grayscale::G15);
    frame->AddWidget(w);



    auto b = WButton::Create(50, 500, 200, 200, "Button");
    b->Style(WidgetStyle::BORDER);
    b->BorderColor(Grayscale::G15);
    frame->AddWidget(b);

    auto w = Widget::Create(50, 50, 50, 50);
    w->BackgroundColor(Grayscale::G15);
    w->Style(WidgetStyle::BORDER);
    w->BorderColor(Grayscale::G15);
    frame->AddWidget(w); */

  //ctx->AddFrame(frame);
  auto win = std::make_shared<MainWindow>();
  ctx->AddFrame(win);
}

void loop() {
  // put your main code here, to run repeatedly:
  ctx->Draw();
}