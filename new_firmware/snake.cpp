#include "snake.h"
#include <TFT_eSPI.h>

struct JoyStick {
  uint32_t t_last_pressed = millis();
  int STEPS = 3968;

  float x() {
    int X = analogRead(26);

    if(abs(STEPS/2 - X) > STEPS/3)
      return( -2.0f * (STEPS/2 - X) / STEPS );
    return 0;
  }

  float y() {
    int Y = analogRead(25);
    
    if(abs(STEPS/2 - Y) > STEPS/3)
      return( -2.0f * (STEPS/2 - Y) / STEPS );
    return 0;
  }
};

JoyStick joyStick;

SnakeGame::SnakeGame(TFT_eSPI& display) 
    : tft(display) {
    w = 20;
    h = 15;
    cellSize = tft.width() / w;
    vec_Snake = new Point2D_int8_t[w*h];
}


void SnakeGame::start() {
    tp = millis();
    timer = 300;
    snailSize = 1;
    dir = 1;
    won = false;
    
    vec_Snake[0] = {(int8_t)(w/2), (int8_t)(h/2)};
    P_food = {(int8_t)(w/2 + 2), (int8_t)(h/2 + 2)};
    
    tft.fillScreen(TFT_BLACK);
}

void SnakeGame::loop() {
    handleInput();
    
    if(millis() - tp >= timer && !won) {
        updateGame();
        drawGame();
        tp = millis();
    }
}

void SnakeGame::handleInput() {
    float dx = joyStick.x();
    float dy = joyStick.y();
    
    if(won) return;
    
    if(dx*dx + dy*dy > 0) {
        if(dx*dx > 0.25) {
            if(dx > 0 && dir != 3 && dir != 1) dir = 1;
            else if(dx < 0 && dir != 1 && dir != 3) dir = 3;
        } 
        else if(dy*dy > 0.25) {
            if(dy > 0 && dir != 4 && dir != 2) dir = 2;
            else if(dy < 0 && dir != 2 && dir != 4) dir = 4;
        }
    }
}

void SnakeGame::updateGame() {
    // Проверка съедения еды
    if(vec_Snake[0].x == P_food.x && vec_Snake[0].y == P_food.y) {
        snailSize++;
        P_food.x = (int8_t)(random(1, w-1));
        P_food.y = (int8_t)(random(1, h-1));
    }
    
    // Проверка коллизий с телом
    for(int8_t i = snailSize-1; i > 0; i--) {
        if(vec_Snake[i].x == vec_Snake[0].x && 
           vec_Snake[i].y == vec_Snake[0].y) {
            won = true;
            return;
        }
        vec_Snake[i] = vec_Snake[i-1];
    }
    
    // Обновление позиции головы
    switch(dir) {
        case 1: vec_Snake[0].y++; break;
        case 2: vec_Snake[0].x--; break;
        case 3: vec_Snake[0].y--; break;
        case 4: vec_Snake[0].x++; break;
    }
    
    // Обработка границ экрана
    wrapCoordinates(vec_Snake[0]);
}

void SnakeGame::drawGame() {
    tft.fillScreen(TFT_BLACK);
    
    // Отрисовка змейки
    for(int i = 0; i < snailSize; i++) {
        tft.fillRect(
            vec_Snake[i].x * cellSize,
            tft.height() - (vec_Snake[i].y + 1) * cellSize,
            cellSize - 1,
            cellSize - 1,
            TFT_WHITE
        );
    }
    
    // Отрисовка еды
    tft.fillRect(
        P_food.x * cellSize,
        tft.height() - (P_food.y + 1) * cellSize,
        cellSize - 1,
        cellSize - 1,
        TFT_GREEN
    );
}

void SnakeGame::wrapCoordinates(Point2D_int8_t& point) {
    if(point.y < 0) point.y = h-1;
    if(point.y >= h) point.y = 0;
    if(point.x < 0) point.x = w-1;
    if(point.x >= w) point.x = 0;
}
