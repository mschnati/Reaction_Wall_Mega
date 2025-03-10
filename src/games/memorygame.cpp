#include "globals.h"
#include "memorygame.h"
#include <stdlib.h>
#include <string.h>
#include <EEPROM.h>

#define EEPROM_TOP5_ADDR 0 // Address in EEPROM to store top 5 scores

#define BOARD_ROWS BUTTON_ROWS
#define BOARD_COLS BUTTON_COLS

int top5Scores[5] = {0};

// Flashes the square at (row, col) with the given color for 'duration' milliseconds.
static void flashSquare(int row, int col, CRGB color, int duration) {
    setButtonColor(row, col, color);
    FastLED.show();
    delay(duration);
    setButtonColor(row, col, CRGB::Black);
    FastLED.show();
    delay(100);   // Short pause between flashes.
}

// Waits until a button is pressed and returns the corresponding row and column via the pointers.
// This function polls the entire matrix.
static void waitForButtonPress(int* pressedRow, int* pressedCol) {
    bool pressed = false;
    while (!pressed) {
        for (int r = 0; r < BOARD_ROWS; r++) {
            for (int c = 0; c < BOARD_COLS; c++) {
                int btnID = ButtonMatrix[r][c]; 
                if (checkButton(btnID + 1)) {    // Adjust index if needed.
                    *pressedRow = r;
                    *pressedCol = c;
                    pressed = true;
                    delay(100); // Debounce delay.
                    break;
                }
            }
            if (pressed) break;
        }
    }
}

// Runs a complete round of the memory game.
void memorygame_run(MemoryGameState* state) {
    // Load the top 5 scores from EEPROM.
    loadTop5Scores(top5Scores);

    FastLED.clear();
    FastLED.show();
    u8g2.clearBuffer();

    // Initialize game state.
    state->sequenceLength = 0;
    state->isGameActive = true;

    randomSeed(analogRead(A0));
    
    // Start by adding the first random square.
    state->sequence[state->sequenceLength][0] = random(BOARD_ROWS);
    state->sequence[state->sequenceLength][1] = random(BOARD_COLS);
    // avoid broken button
    if (state->sequence[state->sequenceLength][0] == 1 && state->sequence[state->sequenceLength][1] == 3) {
        state->sequence[state->sequenceLength][0] = 2;
    }
    state->sequenceLength++;

    delay(1000);

    while (state->isGameActive) {
        // Display the current sequence.
        updateDisplay("Memory Game", 0, 10);
        updateDisplay("Repeat the sequence", 2, 8);
        char scoreMsg[16];
        sprintf(scoreMsg, "Score: %d", state->sequenceLength - 1);
        updateDisplay(scoreMsg, 3, 8);
        for (int i = 0; i < state->sequenceLength; i++) {
            int r = state->sequence[i][0];
            int c = state->sequence[i][1];
            flashSquare(r, c, CRGB::White, 500);
            delay(100);
        }
        
        // Let the player repeat the sequence.
        for (int i = 0; i < state->sequenceLength; i++) {
            int inputRow = -1, inputCol = -1;
            waitForButtonPress(&inputRow, &inputCol);
            
            // If the player pressed the wrong button, show error and end the game.
            if (inputRow != state->sequence[i][0] || inputCol != state->sequence[i][1]) {
                u8g2.clearBuffer();
                updateDisplay("Game Over", 3, 8);
                updateDisplay(scoreMsg, 5, 8);
                // Flash error indication.
                for (int j = 0; j < 3; j++) {
                    FastLED.clear();
                    FastLED.show();
                    delay(200);
                    setButtonColor(inputRow, inputCol, CRGB::Red);
                    FastLED.show();
                    delay(200);
                }
                state->isGameActive = false;
                delay(1000);

                for (int i = 0; i < 5; i++) {
                    if ((state->sequenceLength - 1) > top5Scores[i]) {
                        for (int j = 4; j > i; j--) {
                            top5Scores[j] = top5Scores[j - 1];
                        }
                        top5Scores[i] = state->sequenceLength - 1;
                        break;
                    }
                }
                // Display the top 5 scores.
                u8g2.clearBuffer();
                updateDisplay("Top 5 Scores", 0, 8);
                bool newScore = false;
                for (int i = 0; i < 5; i++) {
                    char scoreMsg[16];
                    if ((top5Scores[i] == (state->sequenceLength - 1)) && !newScore) {
                        sprintf(scoreMsg, "NEW!    %d", top5Scores[i]);
                        newScore = true;
                    } else {
                        sprintf(scoreMsg, "%d.  %d", i + 1, top5Scores[i]);
                    }
                    updateDisplay(scoreMsg, i + 1, 8);
                }
                delay(4000);

                // save the top 5 scores to EEPROM.
                saveTop5Scores(top5Scores);

                return;
            } else {
                // Correct press. Indicate success briefly.
                flashSquare(inputRow, inputCol, CRGB::Green, 200);
            }
        }
        
        // Player completed the sequence correctly. Add a new random move.
        state->sequence[state->sequenceLength][0] = random(BOARD_ROWS);
        state->sequence[state->sequenceLength][1] = random(BOARD_COLS);
        // avoid broken button 1, 3
        if (state->sequence[state->sequenceLength][0] == 1 && state->sequence[state->sequenceLength][1] == 3) {
            state->sequence[state->sequenceLength][0] = 2;
        }
        // avoid broken button 5, 0
        if (state->sequence[state->sequenceLength][0] == 5 && state->sequence[state->sequenceLength][1] == 0) {
            state->sequence[state->sequenceLength][1] = 1;
        }
        state->sequenceLength++;

        delay(500);
    }
}

// Load the top 5 scores from EEPROM.
void loadTop5Scores(int top5Scores[5]) {
    for (int i = 0; i < 5; i++) {
        int val = EEPROM.read(EEPROM_TOP5_ADDR + i);
        // If EEPROM isn't initialized, it will return 255. Set default score to 0.
        if (val == 255) {
            top5Scores[i] = 0;
        } else {
            top5Scores[i] = val;
        }
    }
}

// Save the top 5 scores to EEPROM.
void saveTop5Scores(int top5Scores[5]) {
    for (int i = 0; i < 5; i++) {
        EEPROM.write(EEPROM_TOP5_ADDR + i, top5Scores[i]);
    }
}