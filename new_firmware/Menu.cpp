#include "Menu.h"
#include "maze_game.h"
#include "JoyStick.h"
#include "Buttons.h"

void Menu::draw()
{
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    int8_t arr_size = std::size(items);

    int16_t x = tft.width() / 2;
    int16_t yStart = tft.height() / 2 - (arr_size * 24) / 2;
    int16_t fontOffset = (tft.fontHeight() / 2);

    tft.drawString("MicroDeck 0.1", x, 16);

    for (uint8_t i = 0; i < arr_size; i++)
    {
        int16_t y = yStart + i * 30;
        if (i == selected)
        {
            tft.fillRoundRect(x - 60, y - 20, 120, 28, 5, TFT_WHITE);
            tft.setTextColor(TFT_BLACK, TFT_WHITE);
            tft.drawString(items[i], x, y - fontOffset);
        }
        else
        {
            tft.fillRoundRect(x - 60, y - 20, 120, 28, 5, TFT_CYAN);
            tft.setTextColor(TFT_WHITE, TFT_CYAN);
            tft.drawString(items[i], x, y - fontOffset);
        }
    }
}

void Menu::up()
{
    if (selected == 0)
        selected = 2;
    else
        selected--;
    draw();
}

void Menu::down()
{
    if (selected == 2)
        selected = 0;
    else
        selected++;
    draw();
}