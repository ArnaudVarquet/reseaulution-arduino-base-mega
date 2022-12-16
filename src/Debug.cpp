#include "Debug.h"


void debug (String message) {
    #ifdef DEBUG
        Serial.print (message);
    #endif
}

void debug (unsigned int value, int base = DEC) {
    #ifdef DEBUG
        Serial.print (value, base);
    #endif
}

void debug (int value, int base = DEC) {
    #ifdef DEBUG
        Serial.print (value, base);
    #endif
}

void debug (long value, int base = DEC) {
    #ifdef DEBUG
        Serial.print (value, base);
    #endif
}

void debug (float value, int base = DEC) {
    #ifdef DEBUG
        Serial.print (value, base);
    #endif
}

void debug (unsigned long value, int base = DEC) {
    #ifdef DEBUG
        Serial.print (value, base);
    #endif
}

void debug (bool message) {
    #ifdef DEBUG
        Serial.print (message);
    #endif
}

void debugln (String message) {
    debug (message);
    debug (F("\n"));
}

void debugln (unsigned int value, int base = DEC) {
    debug (value, base);
    debug (F("\n"));
}

void debugln (int value, int base = DEC) {
    debug (value, base);
    debug (F("\n"));
}

void debugln (long value, int base = DEC) {
    debug (value, base);
    debug (F("\n"));
}

void debugln (float value, int base = DEC) {
    debug (value, base);
    debug (F("\n"));
}

void debugln (bool message) {
    debug (message);
    debug (F("\n"));
}