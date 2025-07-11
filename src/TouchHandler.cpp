#include "TouchHandler.h"
#include "Helpers.h"
#include <Arduino.h>

const int touchPins[5] = {27, 26, 25, 33, 32};
const char* sensorLabels[5] = {
    "Touch sensor 1", "Touch sensor 2", "Touch sensor 3",
    "Touch sensor 4", "Touch sensor 5"
};

bool touched[5] = {false};
int touchCount = 0;

void TouchHandler::setup() {
    for (int i = 0; i < 5; i++) {
        pinMode(touchPins[i], INPUT);
    }
}

void TouchHandler::update() {
    for (int i = 0; i < 5; i++) {
        if (!touched[i] && digitalRead(touchPins[i]) == HIGH) {
            touched[i] = true;
            touchCount++;
            debugPrintln(sensorLabels[i]);

            // Wait until released
            while (digitalRead(touchPins[i]) == HIGH) delay(10);
            delay(200); // debounce
        }
    }
}

void TouchHandler::reset() {
    for (int i = 0; i < 5; i++) touched[i] = false;
    touchCount = 0;
}

int TouchHandler::getTouchCount() const {
    return touchCount;
}

bool TouchHandler::isTouched(int index) const {
    return index >= 0 && index < 5 ? touched[index] : false;
}

const char* TouchHandler::getSensorLabel(int index) const {
    return index >= 0 && index < 5 ? sensorLabels[index] : "";
}

void TouchHandler::waitForTouches(int requiredCount) {
    reset();
    debugPrint("👆 Waiting for "); debugPrint(String(requiredCount)); debugPrintln(" touches...");
    while (touchCount < requiredCount) {
        update();
        delay(50);
    }
    debugPrintln("✅ Touch test passed.");
}
