#pragma once

class ProximityHandler {
public:
    void setup();
    void reset();
    void scan();                  // Start 5-second scan and update hit counts
    int getHitCount(int index);   // Get hit count for a specific sensor
    int getTotalPoints();         // Return score (max 30)
    void displayResults();        // Print scoring summary
private:
    static const int numSensors = 5;
    const int pins[numSensors] = {15, 2, 4, 16, 17};
    const char* labels[numSensors] = {
        "Touch sensor 1", "Touch sensor 2", "Touch sensor 3",
        "Touch sensor 4", "Touch sensor 5"
    };

    int hitCount[numSensors] = {0};
};
