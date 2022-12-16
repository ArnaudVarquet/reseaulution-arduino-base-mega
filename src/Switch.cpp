#include <Arduino.h>
#include "Switch.h"

Switch::Switch ( unsigned int aPin ) : Composant ( aPin, OUTPUT ) {

}

void Switch::switchOn ( ) {
    digitalWrite ( pin, HIGH );
}

void Switch::switchOff ( ) {
    digitalWrite ( pin, LOW );
}