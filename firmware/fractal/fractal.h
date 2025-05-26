#include <TFT_eSPI.h>
#include "sdk/input.cpp"
#include "../config.h"

#define BUF_W 60
#define BUF_H 80

class Fractal
{
private:
    static const int MAX_ITER = 48;

    uint16_t buffer[BUF_W * BUF_H];
    uint16_t palette[MAX_ITER + 1];

    JoyStick joyStick;
    TFT_eSPI tft;

    float zoom = 1.0;
    float offsetX = -0.5;
    float offsetY = 0.0;

public:
    Fractal(TFT_eSPI& tft_spi, JoyStick& joystick);
    ~Fractal();

    void start();
    void drawFractal();
    void generatePalette();
};