#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

// Check for button presses
const int buttons[] = {
    CAP_TOUCH_1, CAP_TOUCH_2, CAP_TOUCH_3, CAP_TOUCH_4, CAP_TOUCH_5, CAP_TOUCH_6, CAP_TOUCH_7, CAP_TOUCH_8,
    CAP_TOUCH_9, CAP_TOUCH_10, CAP_TOUCH_11, CAP_TOUCH_12, CAP_TOUCH_13, CAP_TOUCH_14, CAP_TOUCH_15, CAP_TOUCH_16,
    CAP_TOUCH_17, CAP_TOUCH_18, CAP_TOUCH_19, CAP_TOUCH_20, CAP_TOUCH_21, CAP_TOUCH_22, CAP_TOUCH_23, CAP_TOUCH_24,
    CAP_TOUCH_25, CAP_TOUCH_26, CAP_TOUCH_27, CAP_TOUCH_28, CAP_TOUCH_29, CAP_TOUCH_30, CAP_TOUCH_31, CAP_TOUCH_32,
    CAP_TOUCH_33, CAP_TOUCH_34, CAP_TOUCH_35, CAP_TOUCH_36
};

bool checkButton(int buttonNum);
void updateDisplay(const char* message, int line, int textSize);
#ifdef TEST
void buttonTest();
#endif

#endif