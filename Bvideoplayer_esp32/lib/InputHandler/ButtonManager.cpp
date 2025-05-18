#include "ButtonManager.h"

ButtonManager::ButtonManager(uint8_t pin) : _pin(pin) {
    pinMode(_pin, INPUT_PULLUP);
}

void ButtonManager::begin() {
    // 初始化代码（如有需要）
}

ButtonManager::EventType ButtonManager::check() {
    int state = digitalRead(_pin);
    unsigned long now = millis();

    if (state != _lastState) {
        if (state == LOW) {
            _lastPressTime = now;
            _clickCount++;
        } else {
            if (now - _lastPressTime > LONG_PRESS_THRESHOLD) {
                _lastState = state;
                return LONG_PRESS;
            }
        }
        _lastState = state;
    }

    if (_clickCount > 0 && (now - _lastPressTime > DOUBLE_CLICK_TIMEOUT)) {
        EventType event = (_clickCount >= 2) ? DOUBLE_CLICK : SHORT_PRESS;
        _clickCount = 0;
        return event;
    }

    return NONE;
}