#include "utils.h"

static bool lastButtonStates[NUM_BUTTONS] = {LOW};
static bool buttonStates[NUM_BUTTONS] = {LOW};
static unsigned long lastDebounceTime[NUM_BUTTONS] = {0};

/**
 * Check if a button has been pressed.
 * @param buttonNum The button number to check.
 * @return True if the button has been pressed.
 */
bool checkButton(int buttonNum) {
    int reading = digitalRead(buttons[buttonNum - 1]);
    
    if (reading != lastButtonStates[buttonNum]) {
        lastDebounceTime[buttonNum] = millis();
    }
    
    if ((millis() - lastDebounceTime[buttonNum]) > DEBOUNCE_TIME) {
        if (reading != buttonStates[buttonNum]) {
            buttonStates[buttonNum] = reading;
            if (buttonStates[buttonNum] == HIGH) {
                return true;
            }
        }
    }
    
    lastButtonStates[buttonNum] = reading;
    return false;
}

/**
 * Update the display with the given message.
 * @param message The message to display.
 */
void updateDisplay(const char* message, int line, int textSize) {
    // display.setTextSize(textSize);
    // display.setTextColor(SSD1306_WHITE);
    // display.setCursor(0, line * 8 * textSize);
    // display.println(message);
    // display.display();
    u8
}

#ifdef TEST
void buttonTest() {

  Serial.begin(9600);  // Add debug output
  unsigned long lastPrintTime = 0;
  const unsigned long PRINT_INTERVAL = 1000; // 1 second between prints
  int readings[] = {digitalRead(CAP_TOUCH_1), digitalRead(CAP_TOUCH_2), digitalRead(CAP_TOUCH_3), digitalRead(CAP_TOUCH_4), digitalRead(CAP_TOUCH_5), digitalRead(CAP_TOUCH_6), 
                    digitalRead(CAP_TOUCH_7), digitalRead(CAP_TOUCH_8), digitalRead(CAP_TOUCH_9), digitalRead(CAP_TOUCH_10), digitalRead(CAP_TOUCH_11), digitalRead(CAP_TOUCH_12), 
                    digitalRead(CAP_TOUCH_13), digitalRead(CAP_TOUCH_14), digitalRead(CAP_TOUCH_15), digitalRead(CAP_TOUCH_16), digitalRead(CAP_TOUCH_17), digitalRead(CAP_TOUCH_18), 
                    digitalRead(CAP_TOUCH_19), digitalRead(CAP_TOUCH_20), digitalRead(CAP_TOUCH_21), digitalRead(CAP_TOUCH_22), digitalRead(CAP_TOUCH_23), digitalRead(CAP_TOUCH_24), 
                    digitalRead(CAP_TOUCH_25), digitalRead(CAP_TOUCH_26), digitalRead(CAP_TOUCH_27), digitalRead(CAP_TOUCH_28), digitalRead(CAP_TOUCH_29), digitalRead(CAP_TOUCH_30), 
                    digitalRead(CAP_TOUCH_31), digitalRead(CAP_TOUCH_32), digitalRead(CAP_TOUCH_33), digitalRead(CAP_TOUCH_34), digitalRead(CAP_TOUCH_35), digitalRead(CAP_TOUCH_36), };
  
    if (millis() - lastPrintTime >= PRINT_INTERVAL) {
        for (int i = 0; i < 9; i++) {
            if (readings[i] != lastButtonStates[i]) {
                Serial.print("Button ");
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.println(readings[i] ? "HIGH" : "LOW");
            }
        }
        lastPrintTime = millis();
    }

  for (int i = 0; i < 9; i++) {
    if (readings[i] != lastButtonStates[i]) {
      lastDebounceTime[i] = millis();
      Serial.print("Button ");
      Serial.print(i+1);
      Serial.println(" state changed");
    }

    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_TIME) {
      if (readings[i] != buttonStates[i]) {
        buttonStates[i] = readings[i];
        if (buttonStates[i] == LOW) {
          // set to random color
          setButtonColor(i / 3, i % 3, CRGB(random(255), random(255), random(255)));
          FastLED.show();
          delay(DISPLAY_TIME);
          setButtonColor(i / 3, i % 3, CRGB::Black);
          FastLED.show();
        }
      }
    }
    lastButtonStates[i] = readings[i];
  }
}
  #endif