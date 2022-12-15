#include <Arduino.h>
#include "LED.h"

LED::LED ( unsigned int aPin ) : Composant ( aPin, OUTPUT ) {

}

void LED::switchOn ( ) {
    digitalWrite ( pin, HIGH );
}

void LED::switchOff ( ) {
    digitalWrite ( pin, LOW );
}