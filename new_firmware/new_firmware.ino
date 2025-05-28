#include <TFT_eSPI.h>
#include "Menu.h"
#include "JoyStick.h"
#include "Buttons.h"
#include "maze_game.h"
#include "snake.h"

#define JOY_X_PIN 25
#define JOY_Y_PIN 26

TFT_eSPI tft = TFT_eSPI();
Menu mainMenu = Menu(tft);
JoystickInput joystick(JOY_X_PIN, JOY_Y_PIN);
ButtonsInput buttons = ButtonsInput();

unsigned long lastMoveTime = 0;
const unsigned long debounceDelay = 200;

void menu() {
  mainMenu.draw();

  while (1) {
    unsigned long now = millis();

    if (now - lastMoveTime > debounceDelay) {
      int move = joystick.readY();
      if (move == 1) {
        mainMenu.up();
        lastMoveTime = now;
      } else if (move == -1) {
        mainMenu.down();
        lastMoveTime = now;
      }
    }

    if (buttons.readX()) {
      switch (mainMenu.selected) {
        case 0: {
          MazeGame maze(tft);

          while(1) 
          {   
              if (buttons.readA()) {
                  maze.exit();
                  mainMenu.draw();
                  break;
              }
              maze.processInput(joystick.readX(), joystick.readY());
              maze.loop();
          }
          break;
        }
        case 1: {
          SnakeGame snake(tft);

          snake.start();
          while(1)
          {  
            if (buttons.readB()) {
              tft.fillScreen(TFT_BLACK);
              mainMenu.draw();
              break;
            }
            snake.loop();
          }
          
          break;
        }
        case 2: {
          tft.fillScreen(TFT_BLACK);
          tft.drawString("Info", tft.width() / 2, 14);

          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          tft.drawString("Firmware: 0.1", 82, 60);

          delay(8000);
          tft.fillScreen(TFT_BLACK);
          tft.setTextColor(TFT_WHITE, TFT_CYAN);
          mainMenu.draw();
          break;
        }  

        case 3: {
          tft.fillScreen(TFT_BLACK);
          tft.setTextColor(TFT_WHITE);
          tft.drawString("Shut down...", tft.width() / 2, tft.height() / 2);
          delay(1500);
          esp_deep_sleep_start();
          break;
        }
      }
    }

    delay(40);
  }
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  menu();
}

void loop() {
}
