#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H
#pragma once
#include <Arduino.h>

class ButtonManager {
public:
    enum EventType {
        NONE,
        SHORT_PRESS,
        DOUBLE_CLICK,
        LONG_PRESS
    };

    explicit ButtonManager(uint8_t pin);
    void begin();
    EventType check();

private:
    uint8_t _pin;
    int _lastState = HIGH;
    unsigned long _lastPressTime = 0;
    uint8_t _clickCount = 0;

    static const uint8_t DEBOUNCE_DELAY = 50;
    static const unsigned long LONG_PRESS_THRESHOLD = 1000;
    static const unsigned long DOUBLE_CLICK_TIMEOUT = 300;
};

#endif