#ifndef MEMORYGAME_H
#define MEMORYGAME_H

#include "globals.h"

// Structure to hold the memory game state.
typedef struct MemoryGameState {
    int sequence[64][2];   // Stores up to 64 moves. Each entry is a {row, col} pair.
    int sequenceLength;
    bool isGameActive;
} MemoryGameState;

// Runs a complete round of the memory game.
void memorygame_run(MemoryGameState* state);

#endif // MEMORYGAME_H