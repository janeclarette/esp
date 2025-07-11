#include "ProximityHandler.h"
#include "Helpers.h"
#include <Arduino.h>

void ProximityHandler::setup() {
    for (int i = 0; i < numSensors; i++) {
        pinMode(pins[i], INPUT);
        hitCount[i] = 0;
    }
}

void ProximityHandler::reset() {
    for (int i = 0; i < numSensors; i++) {
        hitCount[i] = 0;
    }
}

void ProximityHandler::scan() {
    debugPrintln("📡 Scanning proximity sensors for 5 seconds...");
    bool prevState[numSensors];
    for (int i = 0; i < numSensors; i++) {
        prevState[i] = digitalRead(pins[i]);
    }

    unsigned long start = millis();
    while (millis() - start < 5000) {
        for (int i = 0; i < numSensors; i++) {
            bool currentState = digitalRead(pins[i]);
            if (prevState[i] == HIGH && currentState == LOW && hitCount[i] < 3) {
                hitCount[i]++;
            }
            prevState[i] = currentState;
        }
        delay(20);
    }

    debugPrintln("✅ Proximity scan complete.");
}

int ProximityHandler::getHitCount(int index) {
    return (index >= 0 && index < numSensors) ? hitCount[index] : 0;
}

int ProximityHandler::getTotalPoints() {
    int points = 0;
    for (int i = 0; i < numSensors && points < 30; i++) {
        points += min(3, hitCount[i]) * 10;
        if (points > 30) points = 30;
    }
    return points;
}

void ProximityHandler::displayResults() {
    int total = 0;
    debugPrintln("📊 Proximity Results:");
    for (int i = 0; i < numSensors; i++) {
        int allowedHits = min(3, hitCount[i]);
        int score = 10 * allowedHits;
        total += score;
        debugPrint(labels[i]);
        debugPrint(" → Detected ");
        debugPrint(String(hitCount[i]));
        debugPrint(" time(s): +");
        debugPrint(String(score));
        debugPrintln(" points");
    }
    if (total > 30) total = 30;
    debugPrint("🏁 Total Points: ");
    debugPrintln(String(total));
}
