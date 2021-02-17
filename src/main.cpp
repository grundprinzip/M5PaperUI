#include <Arduino.h>
#include <M5EPD.h>

#include "widgetlib.hpp"

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
  auto frame = Frame::Create(0, 0, 540, 960);
  frame->UpdateMode(UPDATE_MODE_NONE);

  auto p = Paint::Create(10, 10, 520, 900);
  frame->AddWidget(p);

  /* auto l = Label::Create(10, 300, 200, 100, "Hello World there is", 4);
  frame->AddWidget(l);

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

  ctx->AddFrame(frame);
}

void loop() {
  // put your main code here, to run repeatedly:
  ctx->Draw();
}