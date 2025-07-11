// main.ino (modular test format)
#include <Arduino.h>
#include "NetworkManager.h"
#include "TouchHandler.h"
#include "ProximityHandler.h"
#include "ServoHandler.h"
#include "LEDHandler.h"
#include "Helpers.h"

// WiFi and network configuration
const char* ssid = "eliboi";
const char* password = "12345678";
const char* netHost = "192.168.137.184";
const int   netPort = 1883;

// Global state
bool networkInitialized = false;
bool testInProgress = false;

// Handler instances
NetworkManager net(ssid, password, netHost, netPort);
TouchHandler touchHandler;
ServoHandler servoHandler;
ProximityHandler proximityHandler;
LEDHandler ledHandler;

// Utility functions for printing and sending over network
void netPrint(const String& msg) {
  Serial.print(msg);
  if (networkInitialized && msg.length() > 0) {
    String payload = "{\"type\":\"SERIAL\",\"data\":\"" + msg + "\"}";
    net.send("esp32/test/serial", payload.c_str());
  }
}

void netPrintln(const String& msg) {
  Serial.println(msg);
  if (networkInitialized) {
    String payload = "{\"type\":\"SERIAL\",\"data\":\"" + msg + "\"}";
    net.send("esp32/test/serial", payload.c_str());
  }
}

void initializeNetwork() {
  netPrintln("🌐 Initializing network...");
  net.begin();
  delay(2000);
  for (int i = 0; i < 5; i++) {
    net.loop();
    delay(1000);
  }
  networkInitialized = true;
  netPrintln("✅ Network ready!");
}

// Main test runner
void runTest(const String& command) {
  if (testInProgress) {
    netPrintln("⚠️ Test already running.");
    return;
  }

  testInProgress = true;
  netPrintln("\n🔧 Running: " + command);

  if (command == "TEST_TOUCH") {
    touchHandler.reset();
    touchHandler.waitForTouches(3);
  } else if (command == "TEST_SERVO") {
    servoHandler.move();
  } else if (command == "TEST_PROXIMITY") {
    proximityHandler.reset();
    proximityHandler.scan();
    proximityHandler.displayResults();
  } else if (command == "TEST_LED") {
    ledHandler.run();
  } else if (command == "TEST_ALL") {
    touchHandler.reset();
    touchHandler.waitForTouches(3);
    delay(1000);
    servoHandler.move();
    delay(500);
    proximityHandler.reset();
    proximityHandler.scan();
    delay(500);
    ledHandler.run();
    delay(500);
    proximityHandler.displayResults();
  } else {
    netPrintln("❌ Unknown command");
  }

  testInProgress = false;
}

// Arduino setup function
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Set up global print callbacks
  globalPrint = netPrint;
  globalPrintln = netPrintln;
  
  netPrintln("===========================");
  netPrintln("ESP32 TOUCH+PROXIMITY+SERVO+LED");
  netPrintln("===========================");
  initializeNetwork();
  netPrintln("Send: TEST_TOUCH, TEST_SERVO, TEST_PROXIMITY, TEST_LED, TEST_ALL");

  // Setup individual components
  touchHandler.setup();
  servoHandler.setup();
  proximityHandler.setup();
  ledHandler.setup();
}

// Arduino loop function
void loop() {
  if (networkInitialized) {
    net.loop();
    String msg = NetworkManager::getStatusFromLastMessage();
    if (!msg.isEmpty()) {
      runTest(msg);
      NetworkManager::clearLastMessage();  // ✅ Clear so it doesn't rerun!
    }
  }

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    runTest(cmd);
  }
}
