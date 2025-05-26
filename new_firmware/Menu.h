#include <cstdint>
#include <TFT_eSPI.h>

class Menu
{
private:
    TFT_eSPI &tft;
    const char *items[3] = {"Maze", "Info", "Shut down"};
    

public:

    Menu(TFT_eSPI &display) : tft(display)
    {
    }

    uint8_t selected = 0;

    void draw();
    void onUp();
    void onDown();
    // void onSelect();
};