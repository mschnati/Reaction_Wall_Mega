#ifndef REACTIONGAME_H
#define REACTIONGAME_H

#include "globals.h"
#include <stdbool.h>

typedef struct {
    int score;
    unsigned long gameStartTime;
    int activeX;
    int activeY;
    bool isGameActive;
    unsigned long gameEndTime; // Time when the game ended
    CRGB activeColor;
} ReactionGameState;

// Function prototypes
void reaction_game_init(ReactionGameState* state);
void reaction_game_start(ReactionGameState* state);
void reaction_game_update(ReactionGameState* state);
bool reaction_game_is_running(ReactionGameState* state);
int reaction_game_get_score(ReactionGameState* state);
void reaction_game_set_new_block(ReactionGameState* state);


#endif