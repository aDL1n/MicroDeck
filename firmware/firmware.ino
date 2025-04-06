#include <SPI.h>
#include <TFT_eSPI.h>
#include <esp_heap_caps.h>

#include "maze/maze_game.cpp"
#include "config.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
  pinMode(PIN_X, INPUT);
  pinMode(PIN_Y, INPUT);
  pinMode(PIN_SW, INPUT_PULLUP);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);

  start(tft);
}

void loop() {
}