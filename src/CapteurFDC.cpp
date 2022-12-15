#include <Arduino.h>
#include "CapteurFDC.h"

CapteurFDC::CapteurFDC ( unsigned int aPin ) : Composant ( aPin, INPUT ) {

}

bool CapteurFDC::isFDC ( ) {
    if ( digitalRead ( pin ) ) {
        return true;
    } else {
        return false;
    }
}