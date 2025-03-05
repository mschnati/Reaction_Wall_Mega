#include "globals.h"
#include "memorygame.h"
#include <stdlib.h>
#include <string.h>

#define BOARD_ROWS BUTTON_ROWS
#define BOARD_COLS BUTTON_COLS

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
                    flashSquare(r, c, CRGB::White, 50);
                    delay(200); // Debounce delay.
                    break;
                }
            }
            if (pressed) break;
        }
    }
}

// Runs a complete round of the memory game.
void memorygame_run(MemoryGameState* state) {
    int score = 0;
    // Initialize game state.
    state->sequenceLength = 0;
    state->isGameActive = true;
    
    // Start by adding the first random square.
    state->sequence[state->sequenceLength][0] = random(BOARD_ROWS);
    state->sequence[state->sequenceLength][1] = random(BOARD_COLS);
    state->sequenceLength++;

    while (state->isGameActive) {
        // Display the current sequence.
        updateDisplay("Memory Game", 0, 10);
        updateDisplay("Repeat the sequence", 2, 8);
        char scoreMsg[16];
        sprintf(scoreMsg, "Score: %d", score);
        updateDisplay(scoreMsg, 4, 8);
        for (int i = 0; i < state->sequenceLength; i++) {
            int r = state->sequence[i][0];
            int c = state->sequence[i][1];
            flashSquare(r, c, CRGB::White, 500);
            delay(200);
        }
        
        // Let the player repeat the sequence.
        for (int i = 0; i < state->sequenceLength; i++) {
            int inputRow = -1, inputCol = -1;
            waitForButtonPress(&inputRow, &inputCol);
            
            // If the player pressed the wrong button, show error and end the game.
            if (inputRow != state->sequence[i][0] || inputCol != state->sequence[i][1]) {
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

                return;
            } else {
                // Correct press. Indicate success briefly.
                flashSquare(inputRow, inputCol, CRGB::Green, 200);
            }
        }
        
        // Player completed the sequence correctly. Add a new random move.
        state->sequence[state->sequenceLength][0] = random(BOARD_ROWS);
        state->sequence[state->sequenceLength][1] = random(BOARD_COLS);
        state->sequenceLength++;
        
        updateDisplay("Good!", 3, 8);
        delay(500);
    }
}