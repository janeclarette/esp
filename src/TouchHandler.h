#pragma once

class TouchHandler {
public:
    void setup();
    void update();
    void reset();
    int getTouchCount() const;
    bool isTouched(int index) const;
    const char* getSensorLabel(int index) const;
    void waitForTouches(int requiredCount);  // blocking test mode
};
