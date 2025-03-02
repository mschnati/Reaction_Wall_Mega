#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <FastLED.h>
#include "globals.h"

typedef struct {
    int board[6][6];       // 0 = empty, 1 = player1, 2 = player2
    uint8_t currentPlayer; // 1 or 2
    bool isActive;
    unsigned long lastUpdate;
    bool hasWinner;
    int winningCells[4][2];  // [i][0] is row; [i][1] is col for the winning four cells
} ConnectFourState;

// Initializes the Connect Four board and game state.
void connectfour_init(ConnectFourState* state);

// Processes input, updates the board, and checks for win/draw conditions.
void connectfour_update(ConnectFourState* state);

// Starts the Connect Four game.
void connectfour_start(ConnectFourState* state);

// Returns true if the game is currently active.
bool connectfour_is_running(ConnectFourState* state);

#endif // CONNECTFOUR_H