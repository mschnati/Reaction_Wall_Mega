#ifndef GLOBALS_H
#define GLOBALS_H

// 36 Capacitive touch sensor pins
#define CAP_TOUCH_1 37

#define CAP_TOUCH_2 2
#define CAP_TOUCH_3 3
#define CAP_TOUCH_4 4
#define CAP_TOUCH_5 5
#define CAP_TOUCH_6 6
#define CAP_TOUCH_7 7
#define CAP_TOUCH_8 8
#define CAP_TOUCH_9 9
#define CAP_TOUCH_10 10
#define CAP_TOUCH_11 11
#define CAP_TOUCH_12 12
#define CAP_TOUCH_13 13

#define CAP_TOUCH_14 44
#define CAP_TOUCH_15 45
#define CAP_TOUCH_16 46
#define CAP_TOUCH_17 47
#define CAP_TOUCH_18 48
#define CAP_TOUCH_19 49
#define CAP_TOUCH_20 50
#define CAP_TOUCH_21 51

#define CAP_TOUCH_22 22
#define CAP_TOUCH_23 23
#define CAP_TOUCH_24 24
#define CAP_TOUCH_25 25
#define CAP_TOUCH_26 26
#define CAP_TOUCH_27 27
#define CAP_TOUCH_28 28
#define CAP_TOUCH_29 29
#define CAP_TOUCH_30 30
#define CAP_TOUCH_31 31
#define CAP_TOUCH_32 32
#define CAP_TOUCH_33 33
#define CAP_TOUCH_34 34
#define CAP_TOUCH_35 35
#define CAP_TOUCH_36 36

#define NUM_BUTTONS 36

// Display
#include <Wire.h>
#include <Gy
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SDA_PIN 21
#define SCL_PIN 22
// extern Adafruit_SSD1306 display;
extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;

#include <Arduino.h>
#include <FastLED.h>
#include "config.h"
#include "matrix.h"
#include "utils.h"
#ifdef REACTION_GAME
#include "games/reactiongame.h"
#endif
#ifdef TICTACTOE_GAME
#include "games/tictactoe.h"
#endif

#ifdef CONNECT_FOUR
#include "games/connectfour.h"
#endif

#define NUM_LEDS 216
#define BUTTON_ROWS 6
#define BUTTON_COLS 6
#define DATA_PIN 52
#define DEBOUNCE_TIME 40
#define DISPLAY_TIME 1000
#define BRIGHTNESS 255


extern CRGB leds[NUM_LEDS];
extern int ButtonMatrix[BUTTON_COLS][BUTTON_ROWS];

#endif