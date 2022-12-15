#include <Arduino.h>
#include "Composant.h"

Composant::Composant ( unsigned int aPin, unsigned int aMode ) {
    pin = aPin;
    mode = aMode;
}

void Composant::setup ( ) {
    pinMode (pin, mode);
}