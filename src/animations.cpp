#include "globals.h"
#include "animations.h"
#define RANDOM_BLOCK_INTERVAL 1000  // 1 second between updates
#define NUM_BLOCKS 3

static unsigned long lastRandomBlockUpdate = 0;

/**
 * Lights up, up to NUM_BLOCKS random blocks with random colors.
 */
void randomBlock() {
    unsigned long currentTime = millis();
    if (currentTime - lastRandomBlockUpdate >= RANDOM_BLOCK_INTERVAL) {
        FastLED.clear();
        // Light up random blocks
        for (int i = 0; i < NUM_BLOCKS; i++) {
            int x = random(0, BUTTON_ROWS);
            int y = random(0, BUTTON_COLS);
            CRGB color = CHSV(random(255), 255, 255);
            setButtonColor(x, y, color);
        }
        FastLED.show();
        lastRandomBlockUpdate = currentTime;
    }
}

/**
 * Lights up a spinning rainbow border around the Button Matrix.
 * @param numButtons Specific buttons inside the border to light up.
 * @param buttons The buttons to light up. Array of arrays with row and column coordinates.
 */
void rainbowBorder(int numButtons, int buttons[][2]) {
    static uint8_t offset = 0; // Offset for spinning effect
    FastLED.clear();

    int totalLength = 2 * (BUTTON_ROWS + BUTTON_COLS - 2); // Total length of the border
    int index = 0;

    // Light up the border with a rainbow effect
    for (int i = 0; i < BUTTON_ROWS; i++) {
        setButtonColor(i, 0, CHSV((index * 255 / totalLength + offset) % 255, 255, 255));
        index++;
    }
    for (int j = 1; j < BUTTON_COLS; j++) {
        setButtonColor(BUTTON_ROWS - 1, j, CHSV((index * 255 / totalLength + offset) % 255, 255, 255));
        index++;
    }
    for (int i = BUTTON_ROWS - 2; i >= 0; i--) {
        setButtonColor(i, BUTTON_COLS - 1, CHSV((index * 255 / totalLength + offset) % 255, 255, 255));
        index++;
    }
    for (int j = BUTTON_COLS - 2; j > 0; j--) {
        setButtonColor(0, j, CHSV((index * 255 / totalLength + offset) % 255, 255, 255));
        index++;
    }

    // Light up the specific buttons inside the border
    for (int k = 0; k < numButtons; k++) {
        int row = buttons[k][0];
        int col = buttons[k][1];
        rainbowButton(row, col); // Change color as needed
        // setButtonColor(row, col, CRGB::White); // Change color as needed
    }

    FastLED.show();
    offset += 1; // Increment offset for spinning effect
    delay(20); // Adjust delay for desired speed
}

// Idle Animation 1: Rainbow Fill Animation
// Fills the entire matrix with a smoothly cycling rainbow.
void rainbowFillAnimation() {
    static uint8_t offset = 0;
    for (int row = 0; row < BUTTON_ROWS; row++) {
        for (int col = 0; col < BUTTON_COLS; col++) {
            // Calculate a hue that varies with row, col, and time (offset)
            uint8_t hue = (offset + row * 30 + col * 15) % 255;
            setButtonColor(row, col, CHSV(hue, 255, 255));
        }
    }
    FastLED.show();
    offset++;
    delay(30);
}

// Idle Animation 2: Wave Animation
// Creates a horizontal wave effect across the matrix.
void waveAnimation() {
    static uint8_t offset = 0;
    FastLED.clear();
    for (int row = 0; row < BUTTON_ROWS; row++) {
        for (int col = 0; col < BUTTON_COLS; col++) {
            // Shift hue based on the column and a time-dependent offset
            uint8_t hue = (col * 40 + offset) % 255;
            setButtonColor(row, col, CHSV(hue, 255, 200));
        }
    }
    FastLED.show();
    offset++;
    delay(30);
}

// Idle Animation 3: Checkerboard Fade Animation
// Displays a checkerboard pattern that gradually fades between different hues.
void checkerboardFadeAnimation() {
    static uint8_t offset = 0;
    FastLED.clear();
    for (int row = 0; row < BUTTON_ROWS; row++) {
        for (int col = 0; col < BUTTON_COLS; col++) {
            uint8_t hue;
            // Alternate colors in a checkerboard pattern.
            if ((row + col) % 2 == 0) {
                hue = (offset + 50) % 255;
            } else {
                hue = (offset + 150) % 255;
            }
            setButtonColor(row, col, CHSV(hue, 255, 200));
        }
    }
    FastLED.show();
    offset++;
    delay(50);
}