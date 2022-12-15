#include <Arduino.h>
#include "CapteurDistance.h"

CapteurDistance::CapteurDistance ( unsigned int aPin ) : Composant ( aPin, INPUT ) {

}

int CapteurDistance::readDistance ( unsigned int nbReading ) {
    unsigned long values = 0;
    for (int i=0; i<nbReading; i++) {
        values += analogRead ( pin );
        delay (_READING_DELAY);
    }
    values = values / nbReading;
    int distance = 150 - ( values * _VPERSTEP ) * _RATIO;
    return distance;
}