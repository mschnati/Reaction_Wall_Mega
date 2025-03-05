#include "tictactoe.h"

#define PLAYER_1_COLOR CRGB::Red
#define PLAYER_2_COLOR CRGB::Blue
#define WIN_COLOR CRGB::Green
#define WIN_DISPLAY_TIME 3000

// Helper: set the color of a game square (2x2 buttons) 
// Square positions are 1-9, arranged in a 3x3 grid. 
void setSquareColor(int pos, CRGB color) {
    int boardRow = (pos - 1) / 3;
    int boardCol = (pos - 1) % 3;
    // Map the 3x3 board to the 6x6 button matrix (each square covers a 2x2 region)
    int startRow = boardRow * 2;
    int startCol = boardCol * 2;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            setButtonColor(startRow + i, startCol + j, color);
        }
    }
}

// Helper: check if any button in the square (2x2 block) was pressed.
bool squarePressed(int pos) {
    int boardRow = (pos - 1) / 3;
    int boardCol = (pos - 1) % 3;
    int startRow = boardRow * 2;
    int startCol = boardCol * 2;
    // Check all 4 buttons that constitute this square.
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int btnID = ButtonMatrix[startRow + i][startCol + j];
            if (checkButton(btnID + 1)) { // Button IDs are 0-indexed
                return true;
            }
        }
    }
    return false;
}

/**
 * Check if the current player has won the game
 * @param state: TicTacToeState
 */
static bool check_win(TicTacToeState* state) {
    // Check rows
    for(int i = 0; i < 3; i++) {
        if(state->board[i][0] != 0 && 
            state->board[i][0] == state->board[i][1] && 
            state->board[i][0] == state->board[i][2]) {
            state->winner.positions[0] = i * 3 + 1;
            state->winner.positions[1] = i * 3 + 2;
            state->winner.positions[2] = i * 3 + 3;
            return true;
        }
    }
    
    // Check columns
    for(int i = 0; i < 3; i++) {
        if(state->board[0][i] != 0 && 
            state->board[0][i] == state->board[1][i] && 
            state->board[0][i] == state->board[2][i]) {
            state->winner.positions[0] = i + 1;
            state->winner.positions[1] = i + 4;
            state->winner.positions[2] = i + 7;
            return true;
        }
    }
    
    // Check diagonals
    if(state->board[0][0] != 0 && 
        state->board[0][0] == state->board[1][1] && 
        state->board[0][0] == state->board[2][2]) {
        state->winner.positions[0] = 1;
        state->winner.positions[1] = 5;
        state->winner.positions[2] = 9;
        return true;
    }
    
    if(state->board[0][2] != 0 && 
        state->board[0][2] == state->board[1][1] && 
        state->board[0][2] == state->board[2][0]) {
        state->winner.positions[0] = 3;
        state->winner.positions[1] = 5;
        state->winner.positions[2] = 7;
        return true;
    }
    
    return false;
}

/**
 * Check if the game is a draw
 * @param state: TicTacToeState
 */
static bool check_draw(TicTacToeState* state) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(state->board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Initialize the TicTacToe game state
 * @param state: TicTacToeState
 */
void tictactoe_init(TicTacToeState* state) {
    memset(state->board, 0, sizeof(state->board));
    state->currentPlayer = 1;
    state->isActive = false;
    state->lastUpdate = 0;

    updateDisplay("TicTacToe", 0, 10);
    updateDisplay("Red's turn", 3, 8);
}

/**
 * Update the TicTacToe game state
 * @param state: TicTacToeState
 */
void tictactoe_update(TicTacToeState* state) {
    if (!state->isActive) return;
    
    FastLED.clear();
    
    // Draw any squares that have been claimed.
    for (int pos = 1; pos <= 9; pos++) {
        int row = (pos - 1) / 3;
        int col = (pos - 1) % 3;
        if (state->board[row][col] != 0) {
            CRGB color = (state->board[row][col] == 1) ? PLAYER_1_COLOR : PLAYER_2_COLOR;
            setSquareColor(pos, color);
        }
    }
    
    
    // Check for input in any of the 9 squares.
    for (int pos = 1; pos <= 9; pos++) {
        int boardRow = (pos - 1) / 3;
        int boardCol = (pos - 1) % 3;
        // Only allow input if the square is empty.
        if (state->board[boardRow][boardCol] == 0) {
            if (squarePressed(pos)) {
                state->board[boardRow][boardCol] = state->currentPlayer;
                // Mark the square with the player's color.
                CRGB moveColor = (state->currentPlayer == 1) ? PLAYER_1_COLOR : PLAYER_2_COLOR;
                setSquareColor(pos, moveColor);
                
                // Check for win conditions
                if (check_win(state)) {
                    unsigned long endTime = millis();
                    state->winner.hasWinner = true;
                    // Highlight winning squares with WIN_COLOR.
                    for (int i = 0; i < 3; i++) {
                        setSquareColor(state->winner.positions[i], WIN_COLOR);
                    }
                    FastLED.show();
                    
                    // (Optional) update display message for win.
                    if (state->currentPlayer == 1) {
                        u8g2.clearBuffer();
                        updateDisplay("TicTacToe", 0, 10);
                        updateDisplay("Red wins!", 2, 14);
                    } else {
                        u8g2.clearBuffer();
                        updateDisplay("TicTacToe", 0, 10);
                        updateDisplay("Blue wins!", 2, 14);
                    }
                    while (millis() - endTime < WIN_DISPLAY_TIME) {
                        // Wait to display winning pieces.
                    }
                    state->isActive = false;
                    FastLED.clear();
                    u8g2.clearBuffer();
                    return;
                } else if (check_draw(state)) {
                    // Show the last move and then clear.
                    FastLED.show();
                    state->isActive = false;
                    updateDisplay("Draw!", 5, 8);
                    delay(WIN_DISPLAY_TIME);
                    FastLED.clear();
                    u8g2.clearBuffer();
                    return;
                }
                
                // Switch current player.
                u8g2.clearBuffer();
                updateDisplay("TicTacToe", 0, 10);
                if (state->currentPlayer == 1) {
                    updateDisplay("Blue's turn", 3, 8);
                } else {
                    updateDisplay("Red's turn", 3, 8);
                }
                state->currentPlayer = (state->currentPlayer == 1) ? 2 : 1;
                break; // Process one move per update.
            }
        }
    }
    
    FastLED.show();
}

/**
 * Check if the TicTacToe game is running
 * @param state: TicTacToeState
 * @return true if the game is active
 */
bool tictactoe_is_running(TicTacToeState* state) {
    return state->isActive;
}

/**
 * Start the TicTacToe game
 * @param state: TicTacToeState
 */
void tictactoe_start(TicTacToeState* state) {
    tictactoe_init(state);
    // Wait 500ms to let any bouncing or residual button presses clear out.
    delay(500);
    state->isActive = true;
    while (anyButtonPressed()) {
        delay(10);
    }
}

