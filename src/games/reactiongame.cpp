#include "reactiongame.h"
#include "matrix.h"
#include <EEPROM.h>

#define EEPROM_TOP5_REACT_ADDR 10 // Address in EEPROM to store top 5 reaction scores

unsigned int GAME_DURATION = 15000;  // 20 seconds
#define ACCEPT_BUTTON 22
#define UP_BUTTON 27
#define DOWN_BUTTON 15

int top5ReactionScores[5] = {0};

/**
 * Initialize the reaction game state
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_init(ReactionGameState* state) {
    state->score = 0;
    state->gameStartTime = 0;
    state->activeX = 0;
    state->activeY = 0;
    state->isGameActive = false;
    loadTop5ReactionScores();
}

/**
 * Start the reaction game
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_start(ReactionGameState* state) {
    // Set game duration until 5 pressed. 8 and 2 to increase/decrease duration
    FastLED.clear();
    setButtonColor(4, 2, CRGB::Green);
    setButtonColor(2, 2, CRGB::Red);
    setButtonColor(3, 2, CRGB::Blue);
    FastLED.show();
    u8g2.clearBuffer();
    updateDisplay("Reaction Game", 1, 10);
    updateDisplay("Green: +1s", 3, 1);
    updateDisplay("Red: -1s", 4, 1);
    updateDisplay("Blue: Accept", 5, 1);

    while (!checkButton(ACCEPT_BUTTON)) {
        if (checkButton(UP_BUTTON)) {
            if (GAME_DURATION < 45000) GAME_DURATION += 1000;
            setButtonColor(4, 2, CRGB::Black);
            FastLED.show();
            delay(100);
            setButtonColor(4, 2, CRGB::Green);
            FastLED.show();
        } else if (checkButton(DOWN_BUTTON)) {
            if (GAME_DURATION > 1000) GAME_DURATION -= 1000;
            setButtonColor(2, 2, CRGB::Black);
            FastLED.show();
            delay(100);
            setButtonColor(2, 2, CRGB::Red);
            FastLED.show();
        } else if (checkButton(31)) {
            // reset high scores
            for (int i = 0; i < 5; i++) {
                top5ReactionScores[i] = 0;
            }
            saveTop5ReactionScores();
        }
        // format string with duration
        updateDisplay("Reaction Game", 1, 10);
        char message[16];
        sprintf(message, "Duration: %ds", GAME_DURATION / 1000);
        updateDisplay(message, 3, 1);
    }
    FastLED.clear();

    state->score = 0;
    state->gameStartTime = millis();
    state->isGameActive = true;

    randomSeed(analogRead(A0));

    state->activeColor = randomColor();
    
    reaction_game_set_new_block(state);

    // update while the game is active
    while (state->isGameActive) {
        reaction_game_update(state);
    }
}

/**
 * Set a new random block
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_set_new_block(ReactionGameState* state) {
    // Clear current block
    setButtonColor(state->activeX, state->activeY, CRGB::Black);
    
    // Set new random block, different from the current one
    int rng = random(36);
    int new_x = rng % BUTTON_COLS;
    int new_y = rng / BUTTON_COLS;
    state->activeY = new_y;
    while (new_x == state->activeX) {
        new_x = random(BUTTON_COLS);
    }
    state->activeX = new_x;

    // 1, 3 broken button
    if (state->activeX == 1 && state->activeY == 3) {
        state->activeX = 2;
        new_x = 2;
    }
    
    setButtonColor(new_x, new_y, state->activeColor);
    FastLED.show();
}

/**
 * Update the reaction game state
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_update(ReactionGameState* state) {
    if (!state->isGameActive) return;

    // Check game over
    if (millis() - state->gameStartTime >= GAME_DURATION) {
        state->isGameActive = false;
        state->gameEndTime = millis(); // Record the time when the game ended

        FastLED.clear();
        FastLED.show();
        
        // Display score
        // Display average reaction in ms time on display
        u8g2.clearBuffer();
        char message[32];
        sprintf(message, "Score: %d", state->score);
        updateDisplay(message, 0, 1);

        if (state->score > 0) {
            sprintf(message, "Avg Time: %d ms", GAME_DURATION / state->score);
        } else {
            sprintf(message, "No Hits");
        }
        updateDisplay(message, 3, 1);
        
        while (millis() - state->gameEndTime < 3000) {
            // Wait for 5 seconds before returning to main menu
        }
        FastLED.clear();
        FastLED.show();
        
        u8g2.clearBuffer();

        // Check if the score is in the top 5. Only if played at default duration
        if (GAME_DURATION == 15000) {
            for (int i = 0; i < 5; i++) {
                if (state->score > top5ReactionScores[i]) {
                    for (int j = 4; j > i; j--) {
                        top5ReactionScores[j] = top5ReactionScores[j - 1];
                    }
                    top5ReactionScores[i] = state->score;
                    break;
                }
            }
            saveTop5ReactionScores();
            
            // Print top 5 scores and indicate if the score is new
            updateDisplay("Top 5 Scores", 0, 1);
            bool newScore = false;
            for (int i = 0; i < 5; i++) {
                char scoreMsg[16];
                if ((top5ReactionScores[i] == state->score) && !newScore) {
                    sprintf(scoreMsg, "NEW!    %d", top5ReactionScores[i]);
                    newScore = true;
                } else {
                    sprintf(scoreMsg, "%d.  %d", i + 1, top5ReactionScores[i]);
                }
                updateDisplay(scoreMsg, i + 1, 1);
            }
            delay(4000);
            u8g2.clearBuffer();
        }

        return;
    }

    if (checkButton(ButtonMatrix[state->activeX][state->activeY] + 1)) {
        state->score++;
        char message[16];
        sprintf(message, "Score: %d", state->score);
        u8g2.clearBuffer();
        updateDisplay(message, 1, 2);
        reaction_game_set_new_block(state);
    }
}

/**
 * Check if the reaction game is currently running
 * @param state Pointer to the ReactionGameState struct
 * @return True if the game is active, false otherwise
 */
bool reaction_game_is_running(ReactionGameState* state) {
    return state->isGameActive;
}

/**
 * Get the current score of the reaction game
 * @param state Pointer to the ReactionGameState struct
 * @return Current score
 */
int reaction_game_get_score(ReactionGameState* state) {
    return state->score;
}

void loadTop5ReactionScores() {
    for (int i = 0; i < 5; i++) {
        int val = EEPROM.read(EEPROM_TOP5_REACT_ADDR + i);
        // If EEPROM isn't initialized, it will return 255. Set default score to 0.
        if (val == 255) {
            top5ReactionScores[i] = 0;
        } else {
            top5ReactionScores[i] = val;
        }
    }
}

void saveTop5ReactionScores() {
    for (int i = 0; i < 5; i++) {
        EEPROM.write(EEPROM_TOP5_REACT_ADDR + i, top5ReactionScores[i]);
    }
}