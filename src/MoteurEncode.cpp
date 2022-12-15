#include <Arduino.h>
#include "MoteurEncode.h"

MoteurEncode::MoteurEncode ( unsigned int aPinEn, unsigned int aPinIn1, unsigned aPinIn2, unsigned int aPinChA, unsigned int aPinChB ) : Moteur ( aPinEn, aPinIn1, aPinIn2 ) {
    pinChA = aPinChA;
    pinChB = aPinChB;
    encodeur = 0;
    offset = 0;
}

void MoteurEncode::setup () {
    Moteur::setup ();
    pinMode (pinChA, INPUT);
    pinMode (pinChB, INPUT);
}

unsigned int MoteurEncode::getPinChA () {
    return pinChA;
}

unsigned int MoteurEncode::getPinChB () {
    return pinChB;
}

void MoteurEncode::interrupt () {
    if (digitalRead(pinChB) == 1) {
        encodeur++;
    } else {
        encodeur--;
    }
}