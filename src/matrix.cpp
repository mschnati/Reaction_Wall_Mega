#include "globals.h"

CRGB leds[NUM_LEDS];
int ButtonMatrix[BUTTON_ROWS][BUTTON_COLS];

/** convert leds array to 16x16 matrix. [0][0] is bottom left corner
 *  [5][0] - [5][5]
 *     -         -
 *   [0][0] - [0][5]
 */
void buttonsToMatrix() {
    for (int i = 0; i < BUTTON_ROWS; i++) {
        for (int j = 0; j < BUTTON_COLS; j++) {
            if (i % 2 == 0) {
                ButtonMatrix[i][j] = i * BUTTON_COLS + j;
            } else {
                ButtonMatrix[i][j] = i * BUTTON_ROWS + (BUTTON_ROWS - 1) - j;
            }
        }
    }
}

/** 
 * Set the color of the button at the given coordinates. Each button is 6 LEDs long
 * @param x The x coordinate of the button (0-5).
 * @param y The y coordinate of the button (0-5).
 */
void setButtonColor(int x, int y, CRGB color) {
    int button = ButtonMatrix[x][y];
    for (int i = 0; i < 6; i++) {
        leds[button * 6 + i] = color;
    }
}

void setButtonColorMixed(int x, int y, CRGB color1, CRGB color2) {
    int button = ButtonMatrix[x][y];
    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0) {
            leds[button * 6 + i] = color1;
        } else {
            leds[button * 6 + i] = color2;
        }
    }
}

void rainbowButton(int x, int y) {
    static uint8_t hue_offset = 0;
    int button = ButtonMatrix[x][y];
    for (int i = 0; i < 6; i++) {
        leds[button * 6 + i] = CHSV((i * 255 / 6 + hue_offset) % 255, 255, 255);
    }
    hue_offset += 1;
}