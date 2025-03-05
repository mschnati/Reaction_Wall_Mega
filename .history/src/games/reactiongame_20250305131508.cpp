#include "reactiongame.h"
#include "matrix.h"

unsigned int GAME_DURATION = 20000;  // 20 seconds
#define ACCEPT_BUTTON 22
#define UP_BUTTON 27
#define DOWN_BUTTON 15

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
}

/**
 * Start the reaction game
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_start(ReactionGameState* state) {
    // Set game duration until 5 pressed. 8 and 2 to increase/decrease duration
    setButtonColor(4, 2, CRGB::Green);
    setButtonColor(2, 2, CRGB::Red);
    rainbowButton(3, 2);
    FastLED.show();
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
        }
        // format string with duration
        // display.clearDisplay();
        updateDisplay("Reaction Game", 0, 1);
        char message[16];
        sprintf(message, "Duration: %d", GAME_DURATION / 1000);
        updateDisplay(message, 3, 1);
    }
    FastLED.clear();

    state->score = 0;
    state->gameStartTime = millis();
    state->isGameActive = true;
    reaction_game_set_new_block(state);
}

/**
 * Set a new random block
 * @param state Pointer to the ReactionGameState struct
 */
void reaction_game_set_new_block(ReactionGameState* state) {
    // Clear current block
    setButtonColor(state->activeX, state->activeY, CRGB::Black);
    
    // Set new random block, different from the current one
    int new_x = random(BUTTON_COLS);
    int new_y = random(BUTTON_ROWS);
    state->activeY = new_y;
    while (new_x == state->activeX) {
        new_x = random(BUTTON_COLS);
    }
    state->activeX = new_x;
    // setBlockColor_3(newX, newY, BLOCK_COLOR);
    setButtonColor(new_x, new_y, CRGB(random(255), random(255), random(255)));
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
        // display.clearDisplay();
        char message[32];
        sprintf(message, "Score: %d", state->score);
        updateDisplay(message, 0, 1);

        if (state->score > 0) {
            sprintf(message, "Avg Time: %d ms", GAME_DURATION / state->score);
        } else {
            sprintf(message, "No Hits");
        }
        updateDisplay(message, 3, 1);
        
        while (millis() - state->gameEndTime < 2000) {
            // Wait for 5 seconds before returning to main menu
        }
        FastLED.clear();
        FastLED.show();
        // mainMenu();
        return;
    }

    if (checkButton(ButtonMatrix[state->activeX][state->activeY] + 1)) {
        char message[16];
        sprintf(message, "Score: %d", state->score);
        display.clearDisplay();
        updateDisplay(message, 1, 2);
        state->score++;
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
