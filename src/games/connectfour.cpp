#include "globals.h"
#include <string.h>

#define PLAYER_1_COLOR CRGB::Red
#define PLAYER_2_COLOR CRGB::Blue
#define WIN_COLOR CRGB::Green
#define WIN_DISPLAY_TIME 3000

// Helper: Set the color of a cell at (row, col) on the 6x6 matrix.
void setCellColor(int row, int col, CRGB color) {
    // Use the existing function to light the button at that coordinate.
    setButtonColor(row, col, color);
}

// Initialize the Connect Four board.
void connectfour_init(ConnectFourState* state) {
    memset(state->board, 0, sizeof(state->board));
    state->currentPlayer = 1;
    state->isActive = false;
    state->lastUpdate = 0;
    state->hasWinner = false;
    // Optionally clear the display:
    FastLED.clear();
    FastLED.show();
}

// Check if there are four in a row starting from (row, col) in direction (dRow, dCol)
static bool checkDirection(ConnectFourState* state, int row, int col, int dRow, int dCol) {
    int player = state->board[row][col];
    for (int i = 1; i < 4; i++) {
        int newRow = row + i * dRow;
        int newCol = col + i * dCol;
        if (newRow < 0 || newRow >= 6 || newCol < 0 || newCol >= 6 ||
           state->board[newRow][newCol] != player) {
            return false;
        }
    }
    return true;
}

// Check if the current board has a win.
static bool check_win(ConnectFourState* state) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            if (state->board[row][col] != 0) {
                // Check horizontally.
                if (col <= 6 - 4 && checkDirection(state, row, col, 0, 1)) {
                    state->winningCells[0][0] = row;
                    state->winningCells[0][1] = col;
                    state->winningCells[1][0] = row;
                    state->winningCells[1][1] = col + 1;
                    state->winningCells[2][0] = row;
                    state->winningCells[2][1] = col + 2;
                    state->winningCells[3][0] = row;
                    state->winningCells[3][1] = col + 3;
                    return true;
                }
                // Check vertically.
                if (row <= 6 - 4 && checkDirection(state, row, col, 1, 0)) {
                    state->winningCells[0][0] = row;
                    state->winningCells[0][1] = col;
                    state->winningCells[1][0] = row + 1;
                    state->winningCells[1][1] = col;
                    state->winningCells[2][0] = row + 2;
                    state->winningCells[2][1] = col;
                    state->winningCells[3][0] = row + 3;
                    state->winningCells[3][1] = col;
                    return true;
                }
                // Check diagonal (down-right).
                if (row <= 6 - 4 && col <= 6 - 4 && checkDirection(state, row, col, 1, 1)) {
                    state->winningCells[0][0] = row;
                    state->winningCells[0][1] = col;
                    state->winningCells[1][0] = row + 1;
                    state->winningCells[1][1] = col + 1;
                    state->winningCells[2][0] = row + 2;
                    state->winningCells[2][1] = col + 2;
                    state->winningCells[3][0] = row + 3;
                    state->winningCells[3][1] = col + 3;
                    return true;
                }
                // Check diagonal (up-right).
                if (row >= 3 && col <= 6 - 4 && checkDirection(state, row, col, -1, 1)) {
                    state->winningCells[0][0] = row;
                    state->winningCells[0][1] = col;
                    state->winningCells[1][0] = row - 1;
                    state->winningCells[1][1] = col + 1;
                    state->winningCells[2][0] = row - 2;
                    state->winningCells[2][1] = col + 2;
                    state->winningCells[3][0] = row - 3;
                    state->winningCells[3][1] = col + 3;
                    return true;
                }
            }
        }
    }
    return false;
}

// Check if the board is full
static bool check_draw(ConnectFourState* state) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            if (state->board[row][col] == 0) return false;
        }
    }
    return true;
}

// Helper: Check if any button in the given column is pressed.
// We check all rows in that column. Use your existing checkButton function.
static bool columnPressed(int col) {
    for (int row = 0; row < 6; row++) {
        int btnID = ButtonMatrix[row][col];
        if (checkButton(btnID + 1)) {  // Adjust for index if needed.
            return true;
        }
    }
    return false;
}

/**
 * Update the Connect Four game state.
 * When a button in a column is pressed and the column is not full, a piece is dropped.
 */
void connectfour_update(ConnectFourState* state) {
    if (!state->isActive) return;

    FastLED.clear();

    // Draw the board pieces.
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            int cell = state->board[row][col];
            if (cell == 1) {
                setCellColor(row, col, PLAYER_1_COLOR);
            }
            else if (cell == 2) {
                setCellColor(row, col, PLAYER_2_COLOR);
            }
        }
    }

    // Check input for each column.
    for (int col = 0; col < 6; col++) {
        if (columnPressed(col)) {
            // Only process one column at a time (debounce).
            // Find the lowest empty cell in this column.
            int dropRow = -1;
            for (int row = 0; row < 6; row++) {
                if (state->board[row][col] == 0) {
                    dropRow = row;
                    break;
                }
            }
            if (dropRow != -1) {
                CRGB moveColor = (state->currentPlayer == 1) ? PLAYER_1_COLOR : PLAYER_2_COLOR;
            
                // Animate the falling piece from the top (row 5) down to the dropRow.
                for (int r = 5; r >= dropRow; r--) {
                    FastLED.clear();
                    // Redraw the static board pieces.
                    for (int row = 0; row < 6; row++) {
                        for (int c = 0; c < 6; c++) {
                            if (state->board[row][c] != 0) {
                                CRGB color = (state->board[row][c] == 1) ? PLAYER_1_COLOR : PLAYER_2_COLOR;
                                setCellColor(row, c, color);
                            }
                        }
                    }
                    // Draw the falling piece at the current animation row.
                    setCellColor(r, col, moveColor);
                    FastLED.show();
                    delay(50);
                }
                // After animation, commit the move to the board.
                state->board[dropRow][col] = state->currentPlayer;
                
                // Check for win.
                if (check_win(state)) {
                    state->hasWinner = true;
                    // Highlight only the winning pieces in green.
                    for (int i = 0; i < 4; i++) {
                        int r = state->winningCells[i][0];
                        int c = state->winningCells[i][1];
                        setCellColor(r, c, WIN_COLOR);
                    }
                    FastLED.show();
                    delay(WIN_DISPLAY_TIME);
                    state->isActive = false;
                    return;
                }
                else if (check_draw(state)) {
                    // In case of a draw, just clear after a short delay.
                    FastLED.show();
                    delay(1000);
                    state->isActive = false;
                    FastLED.clear();
                    FastLED.show();
                    return;
                }
                
                // Switch players.
                state->currentPlayer = (state->currentPlayer == 1) ? 2 : 1;
                // Pause briefly to debounce input.
                delay(250);
                break; // Process one move per update.
            }
        }
    }

    FastLED.show();
}

/**
 * Start the Connect Four game.
 */
void connectfour_start(ConnectFourState* state) {
    connectfour_init(state);
    // Optional extra delay for debouncing before starting.
    delay(500);
    state->isActive = true;
}

/**
 * Check if the Connect Four game is running.
 */
bool connectfour_is_running(ConnectFourState* state) {
    return state->isActive;
}