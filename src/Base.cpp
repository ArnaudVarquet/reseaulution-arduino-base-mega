#include <Arduino.h>
#include "Base.h"
#include "Debug.h"

Base::Base () {
    
}

void Base::setup () {
    debug (F("Base::setup - Initialisation des moteurs :"));
    treuil1.setup ();
    treuil2.setup ();
    treuil3.setup ();
    roue1.setup ();
    roue2.setup ();
    debugln (F("OK !"));

    debug (F("Base::setup - Initialisation des capteurs de distance :"));
    capteurDistAvant.setup ();
    capteurDistArriere.setup ();
    debugln (F("OK !"));

    debug (F("Base::setup - Initialisation du capteur fin de course de la station :"));
    capteurFDC.setup ();
    debugln ("OK !");

    debug (F("Base::setup - Initialisation de la led du bouton on/off :"));
    ledOnOff.setup ();
    ledOnOff.switchOff ();
    debugln ("OK !");
}

MoteurEncode Base::getTreuil1 () {
    return treuil1;
}

MoteurEncode Base::getTreuil2 () {
    return treuil2;
}

MoteurEncode Base::getTreuil3 () {
    return treuil3;
}

MoteurEncode Base::getRoue1 () {
    return roue1;
}

Moteur Base::getRoue2 () {
    return roue2;
}