#include <SPI.h>
#include <TFT_eSPI.h>
#include <esp_heap_caps.h>

#include "maze/maze_game.cpp"
// #include "fractal/fractal.cpp"
#include "config.h"

TFT_eSPI tft = TFT_eSPI();
JoyStick joystick;

// Fractal fractal = Fractal(tft, joystick);
MazeGame mazeGame = MazeGame(tft, joystick);

void setup() {
  pinMode(PIN_X, INPUT);
  pinMode(PIN_Y, INPUT);
  pinMode(PIN_SW, INPUT_PULLUP);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);

  // fractal.start();
  mazeGame.start();
}

void loop() {
}