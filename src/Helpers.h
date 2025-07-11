#pragma once
#include <Arduino.h>

// Function pointer type for print callbacks
typedef void (*PrintCallback)(const String&);

// Global print callbacks (to be set by main.cpp)
extern PrintCallback globalPrint;
extern PrintCallback globalPrintln;

// Helper functions that all handlers can use
inline void debugPrint(const String& msg) {
    if (globalPrint) {
        globalPrint(msg);
    } else {
        Serial.print(msg);
    }
}

inline void debugPrintln(const String& msg) {
    if (globalPrintln) {
        globalPrintln(msg);
    } else {
        Serial.println(msg);
    }
}

// Overload for const char*
inline void debugPrint(const char* msg) {
    debugPrint(String(msg));
}

inline void debugPrintln(const char* msg) {
    debugPrintln(String(msg));
}

// Helper for formatted strings
inline void debugPrintf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    debugPrint(String(buffer));
}