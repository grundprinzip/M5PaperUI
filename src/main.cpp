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
  auto win = std::make_shared<MainWindow>();
  win->Start(ctx);
}

void loop() {
  // put your main code here, to run repeatedly:
  ctx->Draw();
}