#include "fractal.h"
#include <Arduino.h>

Fractal::Fractal(TFT_eSPI &tft_spi, JoyStick &joystick)
    : tft(tft_spi), joyStick(joystick)
{
}

Fractal::~Fractal()
{
}

void Fractal::start()
{
    generatePalette();
    while (1)
    {
        uint32_t lastUpdate = 0;

        if (millis() - lastUpdate > 100)
        {
            if(joyStick.button(B_btn)) {
                zoom *= 1.2f;
                drawFractal();
                lastUpdate = millis();
            }
            else if(joyStick.button(A_btn)) {
                zoom /= 1.2f;
                drawFractal();
                lastUpdate = millis();
            }

            float xVal = joyStick.readAxis(PIN_X);
            float yVal = joyStick.readAxis(PIN_Y);
            bool moved = false;

            if(fabs(xVal) > 0) {
                offsetX += xVal / zoom;
                moved = true;
            }

            if(fabs(yVal) > 0.0f) {
                offsetY -= yVal / zoom;
                moved = true;
            }

            if(moved) {
                drawFractal();
                lastUpdate = millis();
            }
        }
    }
}

void Fractal::generatePalette()
{
    for (int i = 0; i <= MAX_ITER; i++)
    {
        float t = (float)i / MAX_ITER;
        uint8_t r = 9 * (1 - t) * t * t * t * 255;
        uint8_t g = 15 * (1 - t) * (1 - t) * t * t * 255;
        uint8_t b = 8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255;
        palette[i] = tft.color565(r, g, b);
    }
    palette[MAX_ITER] = TFT_BLACK;
}

void Fractal::drawFractal()
{
    if (!buffer)
        return;

    const float zoomFactor = 1.0 / (zoom * SCREEN_WIDTH * 0.5);

    for (int y = 0; y < SCREEN_HEIGHT; y += BUF_H)
    {
        int blockH = min(BUF_H, SCREEN_HEIGHT - y);

        for (int x = 0; x < SCREEN_WIDTH; x += BUF_W)
        {
            int blockW = min(BUF_W, SCREEN_WIDTH - x);

            for (int by = 0; by < blockH; by++)
            {
                float cy = (y + by - SCREEN_HEIGHT / 2) * zoomFactor + offsetY;

                for (int bx = 0; bx < blockW; bx++)
                {
                    float cx = (x + bx - SCREEN_WIDTH / 2) * zoomFactor + offsetX;

                    float zx = cx;
                    float zy = cy;
                    int iter = MAX_ITER;

                    for (int i = 0; i < MAX_ITER; i++)
                    {
                        float zx2 = zx * zx;
                        float zy2 = zy * zy;
                        if (zx2 + zy2 > 4.0)
                        {
                            iter = i;
                            break;
                        }
                        zy = 2 * zx * zy + cy;
                        zx = zx2 - zy2 + cx;
                    }

                    buffer[by * BUF_W + bx] = palette[iter];
                }
            }

            tft.pushImage(x, y, blockW, blockH, buffer);
        }
    }
}