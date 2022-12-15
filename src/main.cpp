#include <Arduino.h>
#include "main.h"
#include "Debug.h"
#include "Base.h"

Base base = Base ();

void setup() {
  /* Initialisation des différents ports série */
  Serial.begin( 115200 );

  /* Lancement de la procédure d'initialisation */
  debugln (F("setup - BEGIN"));
  /* Initialisation de la base */
  base.setup ();
  attachInterrupt(base.getTreuil1().getPinChA(), interruptTreuil1_CB, RISING);
  attachInterrupt(base.getTreuil2().getPinChA(), interruptTreuil2_CB, RISING);
  attachInterrupt(base.getTreuil3().getPinChA(), interruptTreuil3_CB, RISING);
  attachInterrupt(base.getRoue1().getPinChA(), interruptRoue1_CB, RISING);
  

  /* Fin de la procédure d'initialisation */
  debugln (F("setup - END"));
}

void loop() {
  // put your main code here, to run repeatedly:
}


void interruptTreuil1_CB () {
  base.getTreuil1().interrupt ();
}
void interruptTreuil2_CB () {
  base.getTreuil2().interrupt ();
}
void interruptTreuil3_CB () {
  base.getTreuil3().interrupt ();
}
void interruptRoue1_CB () {
  base.getRoue1().interrupt ();
}