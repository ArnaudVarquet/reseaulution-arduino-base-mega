#include <Arduino.h>
#include "main.h"
#include "Debug.h"
#include "Base.h"

/* Seuil minimum de pourcentage de la batterie de la nacelle ( 20% ). */
#define BMS_NACELLE_SEUIL_MIN             20
/* Seuil minimum de pourcentage de la batterie de la station ( 20% ). */
#define BMS_BASE_SEUIL_MIN                20

Base base = Base ( );

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP
void setup() {
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation des différents ports série
  Serial.begin( 115200 );
  debugln (F("setup - BEGIN"));

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Initialisation de la base
  base.setup ();
  attachInterrupt(base.getTreuil1().getPinChA(), interruptTreuil1_CB, RISING);
  attachInterrupt(base.getTreuil2().getPinChA(), interruptTreuil2_CB, RISING);
  attachInterrupt(base.getTreuil3().getPinChA(), interruptTreuil3_CB, RISING);
  attachInterrupt(base.getRoue1().getPinChA(), interruptRoue1_CB, RISING);

  if ( !base.startupSequence () ) {
    while ( 1 ) {
        debug (".");
        delay ( 2000 );
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Fin de la procédure d'initialisation
  debugln (F("setup - END"));
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
// LOOP
void loop() {
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Vérification des ordres reus sur le port série relié au raspberry

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // On applique les ordres sur les treuils

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // On applique les ordres sur les roues

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Si les batteries de la base et ou de la nacelle ont atteint le seuil bas, on fait un retour
  // à la station de chargement
  if ( base.getBMSInfo().pCentBatt <= BMS_BASE_SEUIL_MIN || false ) {
    
    base.dock ();
  }
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
