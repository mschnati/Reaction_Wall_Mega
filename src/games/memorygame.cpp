#include "globals.h"
#include "memorygame.h"
#include <stdlib.h>
#include <string.h>

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
    if (state->sequence[state->sequenceLength][0] == 1 && state->sequence[state->sequenceLength][1] == 4) {
        state->sequence[state->sequenceLength][0] = 2;
    }
    state->sequenceLength++;

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
                delay(2000);

                for (int i = 0; i < 5; i++) {
                    if (state->sequenceLength > top5Scores[i]) {
                        for (int j = 4; j > i; j--) {
                            top5Scores[j] = top5Scores[j - 1];
                        }
                        top5Scores[i] = state->sequenceLength;
                        u8g2.clearBuffer();
                        updateDisplay("New High Score!", 0, 8);
                        // Display the top 5 scores.
                        for (int j = 0; j < 5; j++) {
                            if (top5Scores[j] == state->sequenceLength) {
                                char scoreMsg[16];
                                sprintf(scoreMsg, "NEW!: %d", top5Scores[j]);
                                updateDisplay(scoreMsg, j + 1, 8);
                            } else {
                                char scoreMsg[16];
                                sprintf(scoreMsg, "%d: %d", j + 1, top5Scores[j]);
                                updateDisplay(scoreMsg, j + 1, 8);
                            }
                        }
                        delay(5000);
                        break;
                    }
                }

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