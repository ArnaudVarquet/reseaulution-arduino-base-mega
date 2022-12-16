#include "Timer.h"
#include <Arduino.h>

Timer::Timer ( unsigned long aDelay ) {
    delay = aDelay;
}

void Timer::restart ( ) {
    startTime = millis();
}

bool Timer::isExpired ( ) {
    long m = millis ();
    return ((m - startTime) > delay);
}