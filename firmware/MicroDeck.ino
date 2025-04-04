#include <SPI.h>
#include <TFT_eSPI.h>
#include <esp_heap_caps.h>

// Конфигурация дисплея
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SEGMENTS 32
#define STATUS_BAR_H 20

// Конфигурация игры
#define MAP_SIZE 32
#define PLAYER_START_X 4.0f
#define PLAYER_START_Y 4.0f
#define FOV (PI / 2)
#define MAX_DEPTH 5
#define MIN_DISTANCE 0.15f
#define STEP_SIZE 0.1f
#define MOVE_SPEED 0.05f
#define ROTATE_SPEED 0.05f
#define JOYSTICK_DEADZONE 0.3f

// Конфигурация аппаратной части
#define PIN_X 26
#define PIN_Y 25
#define PIN_SW 33

TFT_eSPI tft = TFT_eSPI();

struct Point2D {
  float x = PLAYER_START_X;
  float y = PLAYER_START_Y;
};

struct JoyStick {
  bool button() {
    static uint32_t t_last = 0;
    if (!digitalRead(PIN_SW) && (millis() - t_last) > 1000) {
      t_last = millis();
      return true;
    }
    return false;
  }

  float readAxis(int pin) {
    const int STEPS = 3968;
    int value = analogRead(pin);
    float normalized = -2.0f * (STEPS / 2 - value) / STEPS;
    return (fabs(normalized) > JOYSTICK_DEADZONE) ? normalized : 0.0f;
  }
};

JoyStick joystick;

struct DisplayState {
  struct Column {
    uint16_t top;
    uint16_t bottom;
    bool changed;
  } columns[SEGMENTS];
} display;

const uint8_t mapGrid[MAP_SIZE][MAP_SIZE] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
  {1,0,1,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,1,0,1},
  {1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1},
  {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
  {1,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,0,1},
  {1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1},
  {1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
  {1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
  {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
  {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
  {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void setup() {
  pinMode(PIN_X, INPUT);
  pinMode(PIN_Y, INPUT);
  pinMode(PIN_SW, INPUT_PULLUP);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);

  memset(&display, 0, sizeof(display));
  for (auto &col : display.columns) {
    col.top = SCREEN_HEIGHT;
    col.bottom = 0;
  }

  fps();
}

void loop() {

}

void fps() {
  Point2D position = { PLAYER_START_X, PLAYER_START_Y };
  float viewAngle = 0.0f;

  while (true) {

    float move = joystick.readAxis(PIN_X);
    float rotate = joystick.readAxis(PIN_Y);

    float newX = position.x + cos(viewAngle) * move * MOVE_SPEED;
    float newY = position.y + sin(viewAngle) * move * MOVE_SPEED;

    if (mapGrid[(int)newY][(int)newX] == 0) {
      position.x = newX;
      position.y = newY;
    }

    viewAngle += rotate * ROTATE_SPEED;
    viewAngle = fmod(viewAngle, 2 * PI);
    if (viewAngle < 0) viewAngle += 2 * PI;

    for (uint8_t i = 0; i < SEGMENTS; i++) {
      float rayAngle = viewAngle + FOV / 2 - (i * FOV / SEGMENTS);
      float distance = 0.0f;
      bool hit = false;

      while (distance < MAX_DEPTH) {
        float testX = position.x + cos(rayAngle) * distance;
        float testY = position.y + sin(rayAngle) * distance;

        if (testX < 0 || testX >= MAP_SIZE || testY < 0 || testY >= MAP_SIZE) break;

        if (mapGrid[(int)testY][(int)testX] == 1) {
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
      if (col.changed) {
        col.top = top;
        col.bottom = bottom;
      }
    }

    for (int i = 0; i < SEGMENTS; i++) {
      if (display.columns[i].changed) {
        tft.fillRect(i * (SCREEN_WIDTH / SEGMENTS), 0,
                     SCREEN_WIDTH / SEGMENTS, SCREEN_HEIGHT, TFT_BLACK);

        if (display.columns[i].bottom > 0) {
          tft.fillRect(i * (SCREEN_WIDTH / SEGMENTS), display.columns[i].top,
                       SCREEN_WIDTH / SEGMENTS,
                       display.columns[i].bottom - display.columns[i].top,
                       ILI9341_CYAN);
        }
        display.columns[i].changed = false;
      }
    }

    if (joystick.button()) {
      break;
    }
  }

  memset(&display, 0, sizeof(display));
  for (auto &col : display.columns) {
    col.top = SCREEN_HEIGHT;
    col.bottom = 0;
  }

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Game Exited");
}