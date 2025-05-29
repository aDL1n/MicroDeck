#include <TFT_eSPI.h>
#include <cstdint>
#include "maze_config.h"

class MazeGame {
public:
    explicit MazeGame(TFT_eSPI& display);
    void loop();
    void processInput(uint8_t x, uint8_t y);
    void exit();
    ~MazeGame();

private:
    struct Point2D {
        float x;
        float y;
    };

    struct DisplayColumn {
        uint16_t top;
        uint16_t bottom;
        bool changed;
    };

    struct DisplayState {
        DisplayColumn columns[SEGMENTS];
    };

    // Private methods
    void castRays();
    void renderWalls();
    void resetDisplay();

    // Game state
    Point2D m_position;
    float m_viewAngle;
    DisplayState m_displayState;
    
    // Hardware dependencies
    TFT_eSPI& m_tft;
};