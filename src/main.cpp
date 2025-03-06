#include "globals.h"
#include "animations.h"
#include "matrix.h"

#define NEXT_ANIMATION_BUTTON 6
#define PREV_ANIMATION_BUTTON 1
#define PLAY_REACTION_BUTTON 22
#define PLAY_TICTACTOE_BUTTON 21
#define PLAY_CONNECTFOUR_BUTTON 15
#define PLAY_MEMORY_BUTTON 16

enum Animation {
  RANDOM_BLOCK = 0,
  RAINBOW_BORDER,
  RAINBOW_FILL,
  WAVE,
  CHECKERBOARD_FADE,
  NUM_ANIMATIONS
};

enum Game {
  REACTION = 0,
  TICTACTOE,
  CONNECTFOUR,
  NUM_GAMES
};

bool mainMenuActive = true;

ReactionGameState gameState;
TicTacToeState tictactoeState;
ConnectFourState connectfourState;
MemoryGameState memoryState;
Animation currentAnimation = RAINBOW_BORDER;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void mainMenu();

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(4, 30, "Welcome to FHunBox"); 
  u8g2.sendBuffer();

  // Initialize LED matrix
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxRefreshRate(60);
  FastLED.clear();
  FastLED.show();
  buttonsToMatrix();

  // Set up button pins, all connected to ground
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }

  reaction_game_init(&gameState);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  // Blue F
  setButtonColor(0, 0, CRGB::Blue);
  setButtonColor(1, 0, CRGB::Blue);
  setButtonColor(2, 0, CRGB::Blue);
  setButtonColor(2, 1, CRGB::Blue);
  setButtonColor(2, 2, CRGB::Blue);
  setButtonColor(3, 0, CRGB::Blue);
  setButtonColor(4, 0, CRGB::Blue);
  setButtonColor(4, 1, CRGB::Blue);
  setButtonColor(4, 2, CRGB::Blue);

  // White H
  setButtonColor(0, 3, CRGB::White);
  setButtonColor(1, 3, CRGB::White);
  setButtonColor(2, 3, CRGB::White);
  setButtonColor(3, 3, CRGB::White);
  setButtonColor(4, 3, CRGB::White);
  setButtonColor(0, 5, CRGB::White);
  setButtonColor(1, 5, CRGB::White);
  setButtonColor(2, 4, CRGB::White);
  setButtonColor(2, 5, CRGB::White);
  setButtonColor(3, 5, CRGB::White);
  setButtonColor(4, 5, CRGB::White);

  FastLED.show();
  delay(3000);
  FastLED.clear();
  FastLED.show();

  u8g2.clearBuffer();

  // Display main menu
  mainMenu();
}

bool gameIsRunning() {
    return reaction_game_is_running(&gameState) || tictactoe_is_running(&tictactoeState) || connectfour_is_running(&connectfourState);
}

void mainMenu() {
  updateDisplay("         Press to Play", 0, 8);
  delay(10);
  updateDisplay("Red:     Reaction", 2, 8);
  delay(10);
  updateDisplay("Green: TicTacToe", 3, 8);
  delay(10);
  updateDisplay("Blue:   Connect Four", 4, 8);
  delay(10);
  updateDisplay("Lila:    Memory", 5, 8);
}

/*
 Das ist die Hauptschleife des Programms. Hier wird überprüft, ob ein Spiel gestartet werden soll und welches Spiel gestartet werden soll.
 Falls kein Spiel gestartet wird, wird die Animation aktualisiert. Das Hauptmenü wird angezeigt, wenn kein Spiel läuft.
 */
void loop() {
  // Start reaction game when its button is pressed
  if (!gameIsRunning() && checkButton(PLAY_REACTION_BUTTON)) {
      mainMenuActive = false;
      reaction_game_start(&gameState);
  }

  // Start tictactoe game when its button is pressed
  if (!gameIsRunning() && checkButton(PLAY_TICTACTOE_BUTTON)) {
      mainMenuActive = false;
      tictactoe_start(&tictactoeState);
  }

  // Start connect four game when its button is pressed
  if (!gameIsRunning() && checkButton(PLAY_CONNECTFOUR_BUTTON)) {
      mainMenuActive = false;
      connectfour_start(&connectfourState);
  }

  // Start memory sequence game when its button is pressed
  if (!gameIsRunning() && checkButton(PLAY_MEMORY_BUTTON)) {
      mainMenuActive = false;
      memorygame_run(&memoryState);
      u8g2.clearBuffer();
  }

  // Check buttons to cycle through animations
  if (!gameIsRunning()) {
    if (!mainMenuActive) {
      mainMenuActive = true;
      mainMenu();
    }
    if (checkButton(PREV_ANIMATION_BUTTON)) {
        currentAnimation = static_cast<Animation>((currentAnimation + NUM_ANIMATIONS - 1) % NUM_ANIMATIONS);
        FastLED.clear();
        FastLED.show();
    } else if (checkButton(NEXT_ANIMATION_BUTTON)) {
        currentAnimation = static_cast<Animation>((currentAnimation + 1) % NUM_ANIMATIONS);
        FastLED.clear();
        FastLED.show();
    }
  }

  // Update games
  if (tictactoe_is_running(&tictactoeState)) {
      tictactoe_update(&tictactoeState);
  } else if (reaction_game_is_running(&gameState)) {
      reaction_game_update(&gameState);
  } else if (connectfour_is_running(&connectfourState)) {
      connectfour_update(&connectfourState);
  } else {
      // Handle animations
    int num = 4;
    int but[num][2] = {{3, 2}, {3, 3}, {2, 2}, {2, 3}};
    switch (currentAnimation) {
        case RAINBOW_BORDER:
            rainbowBorder(num, but);
            break;
        case RANDOM_BLOCK:
            randomBlock();
            break;
        case RAINBOW_FILL:
            rainbowFillAnimation();
            break;
        case WAVE:
            waveAnimation();
            break;
        case CHECKERBOARD_FADE:
            checkerboardFadeAnimation();
            break;
        default:
            break;
    }
  }
}