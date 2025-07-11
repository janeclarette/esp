#include "ServoHandler.h"
#include "Helpers.h"
#include <ESP32Servo.h>

const int servoPin = 13;
static Servo myServo;  // Static to keep it in function scope

void ServoHandler::setup() {
    myServo.attach(servoPin);
}

void ServoHandler::move() {
    debugPrintln("🔧 Moving to 90°...");
    for (int angle = 0; angle <= 90; angle += 5) {
        myServo.write(angle);
        delay(20);
    }

    delay(1000);

    debugPrintln("🔧 Returning to 0°...");
    for (int angle = 90; angle >= 0; angle -= 5) {
        myServo.write(angle);
        delay(20);
    }

    debugPrintln("✅ Servo movement complete.");
}
