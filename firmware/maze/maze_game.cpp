#include <TFT_eSPI.h>
#include <cstdint>

#include "maze_config.h"
#include "maze_map.cpp"

#include "../sdk/input.cpp"


struct Point2D
{
    float x = PLAYER_START_X;
    float y = PLAYER_START_Y;
};

struct DisplayState {
    struct Column {
      uint16_t top;
      uint16_t bottom;
      bool changed;
    } columns[SEGMENTS];
};

DisplayState display;
JoyStick joystick;

void start(TFT_eSPI tft)
{
    Point2D position = {PLAYER_START_X, PLAYER_START_Y};
    float viewAngle = 0.0f;

    memset(&display, 0, sizeof(display));

    for (auto &col : display.columns)
    {
        col.top = SCREEN_HEIGHT;
        col.bottom = 0;
    }

    while (true)
    {

        float move = joystick.readAxis(PIN_X);
        float rotate = joystick.readAxis(PIN_Y);

        float newX = position.x + cos(viewAngle) * move * MOVE_SPEED;
        float newY = position.y + sin(viewAngle) * move * MOVE_SPEED;

        if (mapGrid[(int)newY][(int)newX] == 0)
        {
            position.x = newX;
            position.y = newY;
        }

        viewAngle += rotate * ROTATE_SPEED;
        viewAngle = fmod(viewAngle, 2 * PI);
        if (viewAngle < 0)
            viewAngle += 2 * PI;

        for (uint8_t i = 0; i < SEGMENTS; i++)
        {
            float rayAngle = viewAngle + FOV / 2 - (i * FOV / SEGMENTS);
            float distance = 0.0f;
            bool hit = false;

            while (distance < MAX_DEPTH)
            {
                float testX = position.x + cos(rayAngle) * distance;
                float testY = position.y + sin(rayAngle) * distance;

                if (testX < 0 || testX >= MAP_SIZE || testY < 0 || testY >= MAP_SIZE)
                    break;

                if (mapGrid[(int)testY][(int)testX] == 1)
                {
                    distance = fmax(distance, MIN_DISTANCE);
                    distance *= cos(viewAngle - rayAngle);
                    hit = true;
                    break;
                }
                distance += STEP_SIZE;
            }

            uint16_t height = SCREEN_HEIGHT / (distance + 0.0001f);
            height = constrain(height, 0, SCREEN_HEIGHT);

            uint16_t top = (SCREEN_HEIGHT - height) >> 1;
            uint16_t bottom = top + height;

            auto &col = display.columns[i];
            col.changed = (col.top != top) || (col.bottom != bottom);
            if (col.changed)
            {
                col.top = top;
                col.bottom = bottom;
            }
        }

        for (int i = 0; i < SEGMENTS; i++)
        {
            if (display.columns[i].changed)
            {
                tft.fillRect(i * (SCREEN_WIDTH / SEGMENTS), 0,
                             SCREEN_WIDTH / SEGMENTS, SCREEN_HEIGHT, TFT_BLACK);

                if (display.columns[i].bottom > 0)
                {
                    tft.fillRect(i * (SCREEN_WIDTH / SEGMENTS), display.columns[i].top,
                                 SCREEN_WIDTH / SEGMENTS,
                                 display.columns[i].bottom - display.columns[i].top,
                                 ILI9341_CYAN);
                }
                display.columns[i].changed = false;
            }
        }

        if (joystick.button())
        {
            break;
        }
    }

    memset(&display, 0, sizeof(display));
    for (auto &col : display.columns)
    {
        col.top = SCREEN_HEIGHT;
        col.bottom = 0;
    }

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.println("Game Exited");
}