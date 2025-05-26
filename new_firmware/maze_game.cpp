#include "maze_game.h"
#include "maze_map.h"
#include <math.h>
#include <string.h>

float readAxis(uint8_t axis_pin) {
    int value = analogRead(axis_pin);
    float normalized = -2.0f * (4096 / 2 - value) / 4096;
    return (fabs(normalized) > 0.4) ? normalized : 0.0f;
}

MazeGame::MazeGame(TFT_eSPI &display)
    : m_tft(display),
      m_position{PLAYER_START_X, PLAYER_START_Y},
      m_viewAngle(0.0f)
{
    resetDisplay();
}

MazeGame::~MazeGame()
{
    resetDisplay();
}

void MazeGame::loop()
{
    
    // processInput();
    castRays();
    renderWalls();
    
}

void MazeGame::processInput(uint8_t x, uint8_t y)
{
    float move = readAxis(26);
    float rotate = readAxis(25);

    float newX = m_position.x + cos(m_viewAngle) * move * MOVE_SPEED;
    float newY = m_position.y + sin(m_viewAngle) * move * MOVE_SPEED;

    if (mapGrid[(int)newY][(int)newX] == 0)
    {
        m_position.x = newX;
        m_position.y = newY;
    }

    m_viewAngle += rotate * ROTATE_SPEED;
    m_viewAngle = fmod(m_viewAngle, 2 * PI);
    if (m_viewAngle < 0)
        m_viewAngle += 2 * PI;
}

void MazeGame::castRays()
{
    for (uint8_t i = 0; i < SEGMENTS; i++)
    {
        float rayAngle = m_viewAngle + FOV / 2 - (i * FOV / SEGMENTS);
        float distance = 0.0f;
        bool hit = false;

        while (distance < MAX_DEPTH)
        {
            float testX = m_position.x + cos(rayAngle) * distance;
            float testY = m_position.y + sin(rayAngle) * distance;

            if (testX < 0 || testX >= MAP_SIZE || testY < 0 || testY >= MAP_SIZE)
                break;

            if (mapGrid[(int)testY][(int)testX] == 1)
            {
                distance = fmax(distance, MIN_DISTANCE);
                distance *= cos(m_viewAngle - rayAngle);
                hit = true;
                break;
            }
            distance += STEP_SIZE;
        }

        uint16_t height = SCREEN_HEIGHT / (distance + 0.0001f);
        height = constrain(height, 0, SCREEN_HEIGHT);

        auto &col = m_displayState.columns[i];
        uint16_t top = (SCREEN_HEIGHT - height) >> 1;
        uint16_t bottom = top + height;

        col.changed = (col.top != top) || (col.bottom != bottom);
        if (col.changed)
        {
            col.top = top;
            col.bottom = bottom;
        }
    }
}

void MazeGame::renderWalls()
{
    for (int i = 0; i < SEGMENTS; i++)
    {
        if (m_displayState.columns[i].changed)
        {
            m_tft.fillRect(i * (SCREEN_WIDTH / SEGMENTS), 0,
                           SCREEN_WIDTH / SEGMENTS, SCREEN_HEIGHT, TFT_BLACK);

            if (m_displayState.columns[i].bottom > 0)
            {
                m_tft.fillRect(i * (SCREEN_WIDTH / SEGMENTS),
                               m_displayState.columns[i].top,
                               SCREEN_WIDTH / SEGMENTS,
                               m_displayState.columns[i].bottom - m_displayState.columns[i].top,
                               ILI9341_CYAN);
            }
            m_displayState.columns[i].changed = false;
        }
    }
}

void MazeGame::resetDisplay()
{
    memset(&m_displayState, 0, sizeof(m_displayState));
    for (auto &col : m_displayState.columns)
    {
        col.top = SCREEN_HEIGHT;
        col.bottom = 0;
    }
    m_tft.fillScreen(TFT_BLACK);
}

void MazeGame::exit()
{
    m_tft.fillScreen(TFT_BLACK);
    m_tft.setCursor(0, 0);
    m_tft.println("Game Exited");
    delay(300);
    resetDisplay();
}