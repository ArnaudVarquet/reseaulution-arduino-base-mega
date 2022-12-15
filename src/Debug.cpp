#include "Debug.h"


void debug (String message) {
    #ifdef DEBUG
        Serial.print (message);
    #endif
}

void debug (int value, int base = DEC) {
    #ifdef DEBUG
        Serial.print (value, base);
    #endif
}

void debugln (String message) {
    debug (message);
    debug (F("\n"));
}

void debugln (int value, int base = DEC) {
    debug (value, base);
    debug (F("\n"));
}