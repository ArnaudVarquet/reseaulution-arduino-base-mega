#include <Arduino.h>
#include "Moteur.h"

Moteur::Moteur ( unsigned int aPinEn, unsigned int aPinIn1, unsigned aPinIn2 ) {
    pinEn = aPinEn;
    pinIn1 = aPinIn1;
    pinIn2 = aPinIn2;
}

void Moteur::setup () {
    pinMode (pinEn, OUTPUT);
    pinMode (pinIn1, OUTPUT);
    pinMode (pinIn2, OUTPUT);
}

unsigned int Moteur::getPinEn () {
    return pinEn;
}

unsigned int Moteur::getPinIn1 () {
    return pinIn1;
}

unsigned int Moteur::getPinIn2 () {
    return pinIn2;
}

void Moteur::monter ( unsigned int vitesse ) {
    digitalWrite( pinIn1, 0 );
    digitalWrite( pinIn2, 1 );
    analogWrite ( pinEn, vitesse );
}

void Moteur::descendre ( unsigned int vitesse ) {
    digitalWrite( pinIn1, 1 );
    digitalWrite( pinIn2, 0 );
    analogWrite ( pinEn, vitesse );
}

void Moteur::avancer ( unsigned int vitesse ) {
    digitalWrite( pinIn1, 0 );
    digitalWrite( pinIn2, 1 );
    analogWrite ( pinEn, vitesse );
}

void Moteur::reculer ( unsigned int vitesse ) {
    digitalWrite( pinIn1, 1 );
    digitalWrite( pinIn2, 0 );
    analogWrite ( pinEn, vitesse );
}

void Moteur::stop () {
    digitalWrite ( pinIn1, 1 );
    digitalWrite ( pinIn2, 1 );
    analogWrite ( pinEn, 0 );
}