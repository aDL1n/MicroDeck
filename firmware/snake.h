#include <Arduino.h>
#include <TFT_eSPI.h>

struct Point2D_int8_t {
  int8_t x;
  int8_t y;
};

class SnakeGame {
public:
    SnakeGame(TFT_eSPI& display);
    void start();
    void loop();

private:
    TFT_eSPI& tft;
    
    uint32_t tp;
    uint32_t timer;
    int8_t w;
    int8_t h;
    int cellSize;
    int8_t snailSize;
    int8_t dir;
    Point2D_int8_t* vec_Snake;
    Point2D_int8_t P_food;
    bool won;
    
    void handleInput();
    void updateGame();
    void drawGame();
    void checkCollisions();
    void wrapCoordinates(Point2D_int8_t& point);
};
