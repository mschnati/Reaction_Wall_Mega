#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "globals.h"
#include <stdbool.h>

typedef struct {
    int board[3][3];  // 0=empty, 1=X, 2=O
    int currentPlayer; // 1=X, 2=O
    bool isActive;
    unsigned long lastUpdate;
    struct {
        int positions[3];  // Store square numbers of winning line
        bool hasWinner;
    } winner;
} TicTacToeState;


void tictactoe_init(TicTacToeState* state);
void tictactoe_start(TicTacToeState* state);
void tictactoe_update(TicTacToeState* state);
bool tictactoe_is_running(TicTacToeState* state);

#endif