#include <cstdint>
#include <TFT_eSPI.h>

class Menu
{
private:
    TFT_eSPI &tft;
    const char *items[4] = {"Maze", "Snake", "Info", "Shut down"};
    

public:

    Menu(TFT_eSPI &display) : tft(display)
    {
    }

    uint8_t selected = 0;

    void draw();
    void up();
    void down();
};