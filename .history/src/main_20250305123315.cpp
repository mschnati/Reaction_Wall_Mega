#include "globals.h"
#include "animations.h"
#include "matrix.h"
#include "games/reactiongame.h"
#include "games/tictactoe.h"

#define NEXT_ANIMATION_BUTTON 6
#define PREV_ANIMATION_BUTTON 1
#define PLAY_REACTION_BUTTON 22
#define PLAY_TICTACTOE_BUTTON 21
#define PLAY_CONNECTFOUR_BUTTON 15

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

ReactionGameState gameState;
TicTacToeState tictactoeState;
ConnectFourState connectfourState;
Animation currentAnimation = RAINBOW_BORDER;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    // Initialize I2C communication with specified SDA and SCL pins
    // Initialize the display
  Wire.begin(SCK_PIN
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.fillScreen(SSD1306_BLACK);
  display.display();


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
  tictactoe_init(&tictactoeState);
  connectfour_init(&connectfourState);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  updateDisplay("FHMS", 0, 2);
  updateDisplay("Middle to Start", 2, 1);

  rainbowButton(3, 2);
  FastLED.show();
}

bool gameIsRunning() {
    return reaction_game_is_running(&gameState) || tictactoe_is_running(&tictactoeState) || connectfour_is_running(&connectfourState);
}

void loop() {
  // Start reaction game when middle button is pressed
  if (!gameIsRunning() && checkButton(PLAY_REACTION_BUTTON)) {
      FastLED.clear();
      FastLED.show();
      display.clearDisplay();
      updateDisplay("Reaction Game", 0, 1);
      reaction_game_start(&gameState);
  }

  // Start tictactoe game when top middle button is pressed
  if (!gameIsRunning() && checkButton(PLAY_TICTACTOE_BUTTON)) {
      FastLED.clear();
      FastLED.show();
      display.clearDisplay();
      updateDisplay("TicTacToe Game", 0, 1);
      tictactoe_start(&tictactoeState);
  }

  // Start connect four game when top right button is pressed
  if (!gameIsRunning() && checkButton(PLAY_CONNECTFOUR_BUTTON)) {
      FastLED.clear();
      FastLED.show();
      display.clearDisplay();
      updateDisplay("Connect Four", 0, 1);
      connectfour_start(&connectfourState);
  }

  // Cycle through animations with buttons 4 and 6
  if (!gameIsRunning()) {
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
    int num = 3;
    int but[num][2] = {{3, 2}, {3, 3}, {2, 2}};
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