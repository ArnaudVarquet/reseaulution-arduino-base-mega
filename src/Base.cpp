#include <Arduino.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "Base.h"
#include "Debug.h"

Base::Base () {
    
}

void Base::setup () {
    debug (F("base::setup - initalisation de la librairie Wire :"));
    Wire.begin();
    debugln (F("OK !"));

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

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Initialisation du canal BLE pour communiquer avec la nacelle
    debugln (F("Base::setup - Initialisation du canal BLE pour communiquer avec la nacelle :"));
    if ( !canalBLE.setup() ) {
        debugln (F("Base::setup - !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
        debugln (F("Base::setup - !!! Erreur lors de l'initialisation du canal BLE - Reboot !!!"));
        debugln (F("Base::setup - !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
        reboot ();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Connexion à la nacelle en BLE
    while ( !canalBLE.isConnected( ) ) {
        canalBLE.connect (  );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // une fois la connexion établie, on allume la led du bouton on/off pour indiquer que c'est bon
    ledOnOff.switchOn ();
    debugln (F("Base::setup -     Connexion à la nacelle en BLE : OK"));

}

bool Base::startupSequence ( ) {
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // On lance la séquence de démarrage ( retour Home de la nacelle et de la base )
    debugln (F("\n\n"));
    debugln (F("Base::startupSequence - -----------------------------------------------------"));
    debugln (F("Base::startupSequence - Lancement de la séquence de démarrage..."));
    canalBLE.enableLightAlert ( true );     // On active les bips sonores sur la nacelle
    canalBLE.enableNacelleCharger ( true ); // On active les flash lumineux sur la nacelle
    canalBLE.sendTrame ( );                 // On envoi l'ordre à la nacelle

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // On commence par contrôler que la nacelle soit bien remonté sur la station
    // Pour cela, il faut d'abord demander les informations sur les capteurs
    // fin de course de la nacelle
    debugln (F("Base::startupSequence - Vérification des capteurs fin de course de la nacelle..."));
    Timer timeout ( 6000 );                 // On initialise un chrono pour voir quand on à commencé à essayer d'obtenir les informations
    timeout.restart ();
    while ( canalBLE.isFDCTrameExpired ( ) && !timeout.isExpired ( ) ) {
        // Si les données de la nacelle ne sont plus suffisament récentes on les redemande
        enableNacelleFDCStream ();
        handleBLEMessage (canalBLE.readBLE ( ));
    }
    if ( canalBLE.isFDCTrameExpired ( ) ) {
        debugln (F("Base::startupSequence - !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
        debugln (F("Base::startupSequence - !!! Erreur lors du chargement des informations fin de course de la nacelle !!!"));
        debugln (F("Base::startupSequence - !!! Impossible de finir la phase d'initialisation du robot                 !!!"));
        debugln (F("Base::startupSequence - !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
        return false;
    }
    canalBLE.enableFdcStream ( false ); // On désactive l'envoie des trames FDC
    canalBLE.sendTrame (  );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Une fois les informations chargées, on peut les analyser
    // Si un des 3 capteurs fin de course de la nacelle est à 0 (c'est à dire pas enclenché)
    // alors c'est que la nacelle n'est pas remontée
    if ( ! canalBLE.isDocked () ) {
        debugln (F("Base::startupSequence - La nacelle n'est pas correctement dockée à la station !"));
        // Dans ce cas il faut un peut descendre la nacelle pour s'éloigner de la base
        descendreNacelle ( VSPEED_LOW );
        delay ( 1500 );
        stopNacelle ();
        // Ensuite, on lance un autoLevel
        debugln (F("Base::startupSequence - Lancement de la procédure de remise à niveau automatique de la nacelle..."));
        autoLevelNacelle ();
        debugln (F("Base::startupSequence - Procédure de remise à niveau automatique de la nacelle terminée"));
        // RAZ Offset et encodeur des treuil
        //setEncodeursNacelle ( );
        //setOffsetNacelle ( );
        // On lance un retour à la base de la nacelle
        debugln (F("Base::startupSequence - Lancement de la procédure de retour à la station de la nacelle..."));
        //dockNacelle  ( );
        debugln (F("Base::startupSequence - Procédure de retour à la station de la nacelle terminée"));
    }
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

LED Base::getLedOnOff () {
    return ledOnOff;
}

BMSInfo Base::getBMSInfo () {
    if ( timerBMSInfo.isExpired () ) {
        debugln (F("Base::getBMSInfo - Mise à jour des données sur la baterie de la base"));
        unsigned int temp = 0;
        ///////////////////////////////////////////////////////////////////////////
        // SOC
        bmsInfo.pCentBatt = 0;
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0x06);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        bmsInfo .pCentBatt = Wire.read();
        bmsInfo .pCentBatt = bmsInfo .pCentBatt << 8;
        bmsInfo .pCentBatt |= temp;
        bmsInfo .pCentBatt = bmsInfo .pCentBatt * (1.0 / 256.0);
        debug   (F("Base::getBMSInfo -   pCentBattStation : "));
        debugln (bmsInfo .pCentBatt);

        ///////////////////////////////////////////////////////////////////////////
        // TTF
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0x20);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        bmsInfo .ttf = Wire.read();
        bmsInfo .ttf = bmsInfo .ttf << 8;
        bmsInfo .ttf |= temp;
        bmsInfo .ttf = (bmsInfo .ttf * 5.625) / 60.0;
        debug   (F("Base::getBMSInfo -   ttfStation : "));
        debugln (bmsInfo .ttf);

        ///////////////////////////////////////////////////////////////////////////
        // TTE
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0x11);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        bmsInfo .tte = Wire.read();
        bmsInfo .tte = bmsInfo .tte << 8;
        bmsInfo .tte |= temp;
        bmsInfo .tte = (bmsInfo .tte * 5.625) / 60.0;
        debug   (F("Base::getBMSInfo -   tteStation : "));
        debugln (bmsInfo .tte);

        ///////////////////////////////////////////////////////////////////////////
        // Current
        Wire.beginTransmission(0x36);     // transmit to device #8
        Wire.write(0x1c);                 // sends five bytes
        Wire.endTransmission(false);      // stop transmitting
        Wire.requestFrom(0x36, 2);        // request 6 bytes from slave device #8

        temp = Wire.read();
        bmsInfo .current = Wire.read();
        bmsInfo .current = bmsInfo .current << 8;
        bmsInfo .current |= temp;
        bmsInfo .current = (bmsInfo .current * (1.5625 / 0.005) / 1000);
        debug   (F("Base::getBMSInfo -   currentStation : "));
        debugln (bmsInfo .current);

        ///////////////////////////////////////////////////////////////////////////
        // CHARGE
        bmsInfo .charge = ( bmsInfo .current > 50 );
        debug   (F("Base::getBMSInfo -   stationEnCharge : "));
        debugln (bmsInfo .charge);

        ///////////////////////////////////////////////////////////////////////////
        // BATTERIE VOLTAGE
        //Cell 1 Avg Voltage -  Result x 0.078125 = mV
        unsigned int vcell1 = 0;
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0xd4);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        vcell1 = Wire.read();
        vcell1 = vcell1 << 8;
        vcell1 |= temp;
        vcell1 = vcell1 * 0.078125;
    
        //Cell 2 Avg Voltage -  Result x 0.078125 = mV
        unsigned int vcell2 = 0;
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0xd3);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        vcell2 = Wire.read();
        vcell2 = vcell2 << 8;
        vcell2 |= temp;
        vcell2 = vcell2 * 0.078125;

        //Cell 3 Avg Voltage -  Result x 0.078125 = mV
        unsigned int vcell3 = 0;
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0xd2);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        vcell3 = Wire.read();
        vcell3 = vcell3 << 8;
        vcell3 |= temp;
        vcell3 = vcell3 * 0.078125;

        bmsInfo .vBatt = vcell1 + vcell2 + vcell3;
        debug   (F("Base::getBMSInfo -   vBattStation : "));
        debugln (float(bmsInfo .vBatt / 1000.0));

        ///////////////////////////////////////////////////////////////////////////
        //Pack T°
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0x1b);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        bmsInfo .temperaturePack = Wire.read();
        bmsInfo .temperaturePack = bmsInfo .temperaturePack << 8;
        bmsInfo .temperaturePack |= temp;
        bmsInfo .temperaturePack = (bmsInfo .temperaturePack * (1.0 / 256.0)) * 100;  
        debug   (F("Base::getBMSInfo -   temperaturePackStation : "));
        debugln (bmsInfo .temperaturePack / 100);               

        ///////////////////////////////////////////////////////////////////////////
        // Alert Status
        int protectionStatus = 0;
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0xd9);               // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        protectionStatus = Wire.read();
        protectionStatus = protectionStatus << 8;
        protectionStatus |= temp;
        debug (F("Base::getBMSInfo -   Protection status : "));
        debug (protectionStatus, BIN);
        if (bitRead(protectionStatus, 0)) debugln(F(" > Ship"));
        if (bitRead(protectionStatus, 1)) debugln(F(" > ResDFault"));
        if (bitRead(protectionStatus, 2)) debugln(F(" > ODCP"));
        if (bitRead(protectionStatus, 3)) debugln(F(" > UVP"));
        if (bitRead(protectionStatus, 4)) debugln(F(" > TooHotD"));
        if (bitRead(protectionStatus, 5)) debugln(F(" > DieHot"));
        if (bitRead(protectionStatus, 6)) debugln(F(" > PermFail"));
        if (bitRead(protectionStatus, 7)) debugln(F(" > Imbalance"));
        if (bitRead(protectionStatus, 8)) debugln(F(" > PreqF")); 
        if (bitRead(protectionStatus, 9)) debugln(F(" > Qovflw"));
        if (bitRead(protectionStatus, 10)) debugln(F(" > OCCP"));
        if (bitRead(protectionStatus, 11)) debugln(F(" > OVP"));
        if (bitRead(protectionStatus, 12)) debugln(F(" > TooColdC"));
        if (bitRead(protectionStatus, 13)) debugln(F(" > Full"));
        if (bitRead(protectionStatus, 14)) debugln(F(" > TooHotC"));
        if (bitRead(protectionStatus, 15)) debugln(F(" > ChgWDT"));
                            
        ///////////////////////////////////////////////////////////////////////////
        // Fet Status
        int fetStatus = 0;
        Wire.beginTransmission(0x36);   // transmit to device #8
        Wire.write(0x1f1);              // sends five bytes
        Wire.endTransmission(false);    // stop transmitting
        Wire.requestFrom(0x36, 2);      // request 6 bytes from slave device #8

        temp = Wire.read();
        fetStatus = Wire.read();
        fetStatus = fetStatus << 8;
        fetStatus |= temp;
        debug   (F("Base::getBMSInfo -   Station FETs status : "));
        debugln (fetStatus, BIN);
        debug   (F("Base::getBMSInfo -       > Charge FET = "));
        debugln (String(bitRead(fetStatus, 0)));
        debug   (F("Base::getBMSInfo -       > Discharge FET = "));
        debugln (String(bitRead(fetStatus, 1)));

        timerBMSInfo.restart ();
    }
    return bmsInfo;
}

void Base::dock ( HorizontalDirection direction ) {
    debugln (F("Base::dock - BEGIN"));
    hInstruction.direction = direction;
    hInstruction.targetSpeed = HSPEED_HIGH;
    hInstruction.actualSpeed = 0;
    while ( hInstruction.direction != hNONE ) {
        H_MOOVE ();
        /* On fait une lecture du serial BLE et on gère le résultat */
        // handleBLEMessage ( comNacelle.readBLE ( Serial2 ) );
    }
    debugln (F("Base::dock - END"));
}

void Base::H_MOOVE () {
    debugln ("Base::H_MOOVE - BEGIN");
    switch ( hInstruction.direction ) {
        case hNONE: // Pas de direction -> on sort de la fonction
        break;
        case FORWARD : // On avance vers l'avant    -> même fonction pour avancer ou reculer
        case BACKWARD : // On avance vers l'arrière -> même fonction pour avancer ou reculer
        if (hInstruction.direction == FORWARD) {
            debugln (F("Base::H_MOOVE - Direction : FORWARD"));
        } else {
            debugln (F("Base::H_MOOVE - Direction : BACKWARD"));
        }
        HorizontalDirection stationPos = getBasePosition ( );
        if (stationPos == FORWARD) {
            debugln (F("Base::H_MOOVE - Position : FORWARD"));
        } else if (stationPos == BACKWARD ) {
            debugln (F("Base::H_MOOVE - Position : BACKWARD"));
        } else {
            debugln (F("Base::H_MOOVE - Position : NONE"));
        }
        /* Avant d'essayer de bouger, on regarde si le mouvement est possible ou non */
        /* Le mouvement est possible si : */
        /*      - la station n'est pas dockée */
        /*   OU - la station est dockée à l'arrière et on veut aller vers l'avant */
        /*   OU - la station est dockée à l'avant et on veut aller à l'arrière */
        if ( stationPos == hNONE || ( stationPos == FORWARD && hInstruction.direction == BACKWARD)  || ( stationPos == BACKWARD && hInstruction.direction == FORWARD ) ) {
            /* Le mouvement est autorisé */
            /* On commence par regarder si on est docké (c'est le cas où on vient de lancer le mouvement et que nous sommes encore docké ) */
            if ( stationPos != hNONE ) {
            /* Dans ce cas il faut commencer par couper l'alimentation de la batterie de la nacelle et de la station */
            canalBLE.enableNacelleCharger ( false );
            canalBLE.sendTrame (  );
            switchBMS.switchOn (); // Attention pour désactiver, il faut switchOn
            delay ( 2000 );
            }
            /* On contrôle notre proximité avec la station qui est dans la direction qu'on vise */
            if ( ( hInstruction.direction == FORWARD && capteurDistAvant.readDistance () < 20 ) || ( hInstruction.direction == BACKWARD && capteurDistArriere.readDistance () < 20 ) ) {
            debugln (F("Base::H_MOOVE - On est à moins de 20cm de la station de rechargemenent : on ralenti !"));
            hInstruction.targetSpeed = HSPEED_LOW;
            }
            /* On adapte la vitesse ( accélération ou décélération ) */
            if ( hInstruction.actualSpeed < hInstruction.targetSpeed ) {
            hInstruction.actualSpeed = min ( hInstruction.targetSpeed, hInstruction.actualSpeed + 6 );
            } else if ( hInstruction.actualSpeed > hInstruction.targetSpeed ) {
            hInstruction.actualSpeed = max ( hInstruction.targetSpeed, hInstruction.actualSpeed - 6 );
            }
            /* On applique la vitesse et la direction sur les deux roues */
            if (hInstruction.direction == FORWARD ) {
            roue1.avancer ( hInstruction.actualSpeed );
            roue2.avancer ( hInstruction.actualSpeed );
            } else if (hInstruction.direction == BACKWARD ) {
            roue1.reculer ( hInstruction.actualSpeed );
            roue2.reculer ( hInstruction.actualSpeed );
            }


        } else {
            /* Le mouvement n'est pas autorisé -> la station est arrivée sur une station de rechargement */
            debugln (F("Base::H_MOOVE - Mouvement non authorisé : arrêt de la station"));
            stopBase ();
            debugln (F("Base::H_MOOVE - Mise en charge de la station et de la nacelle"));
            delay ( 1000 );
            switchBMS.switchOff (); // Attention : pour activer, il faut switchOff
            delay ( 2000 );
            canalBLE.enableNacelleCharger ( true );
            canalBLE.sendTrame ( );
        }
        break;
    }
    debugln ("Base::H_MOOVE - END");
}

HorizontalDirection Base::getBasePosition () {
    if ( capteurFDC.isFDC () ) {
        unsigned long avant = capteurDistAvant.readDistance ();
        unsigned long arriere = capteurDistArriere.readDistance ();
        if ( avant < arriere ) {
            return FORWARD;
        } else {
            return BACKWARD;
        }
    } else {
        return hNONE;
    }
}

void Base::stopBase ( ) {
    roue1.stop ( );
    roue2.stop ( );
    hInstruction.direction = hNONE;
    hInstruction.actualSpeed = 0;
    hInstruction.targetSpeed = 0;
}

void Base::reboot ( ) {
  delay(500);
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}

void Base::enableNacelleFDCStream ( ) {
    if ( timerFDCStream.isExpired ( ) && canalBLE.isFDCTrameExpired () ) {
        debugln ("Base::enableNacelleFDCStream - Donnée de FDC ne la nacelle obsolète. Demande de mise à jour...");
        canalBLE.enableFdcStream ( true );
        canalBLE.sendTrame ( );
        timerFDCStream.restart ( );
    }
}

void Base::handleBLEMessage ( BLEMessageType type ) {
  switch ( type ) {
    case BLE_TRAME_ANGLE:
      // comRasp.sendNacelleAngleInfo ( Serial3, comNacelle.getAngleInfo () );
      break;
    case BLE_TRAME_GIMBALL:
      // comRasp.sendGimbalInfo ( Serial3, comNacelle.getGimbalInfo () );
      break;
  }
}

void Base::descendreNacelle ( unsigned int vitesse ) {
    treuil1.descendre ( vitesse );
    treuil2.descendre ( vitesse );
    treuil3.descendre ( vitesse );
}

void Base::stopNacelle ( )  {
  treuil1.stop ();
  treuil2.stop ();
  treuil3.stop ();
  vInstruction.direction    = vNONE;
  vInstruction.targetSpeed  = 0;
  vInstruction.actualSpeed  = 0;
}

void Base::autoLevelNacelle ( ) {
  debugln (F("Base::autoLevelNacelle - BEGIN"));
  boolean pitchChanged            = false;
  boolean rollChanged             = false;
  unsigned int nbGoodLoop         = 0;

  /* On commence par désactiver les capteurs ultrason */
  debugln (F("Base::autoLevelNacelle - Désactivation des Ultra Son"));
  canalBLE.enableUsStream ( false );
  canalBLE.sendTrame ( );
  delay ( 1000 );

  /* On stock la valeur de l'encodeur du second treuil comme référence */
  long prevEncodeurT2 = treuil2.getEncodeur ();
  debug   (F("Base::autoLevelNacelle - Valeur de référence de l'encodeur du Treuill n°2 : "));
  debugln (prevEncodeurT2);

  /* On boucle jusqu'à ce que tout soit ok */
  while ( nbGoodLoop < 3 ) {
    /* Vérification du ROLL */
    debugln (F("Base::autoLevelNacelle - Vérification du Roll"));
    rollChanged = adjustPoRNacelle ( "Roll", treuil3 );
    
    /* Vérification du PITCH */
    debugln (F("Base::autoLevelNacelle - Vérification du Pitch"));
    pitchChanged = adjustPoRNacelle ( "Pitch", treuil1 );
    
    /* On regarde si le ROLL ou le PITCH on changé */
    debugln (F("Base::autoLevelNacelle - Vérification de la présence de changement du Roll ou du Pitch"));
    debug   (F("                     Changement sur le roll : "));
    debugln (rollChanged);
    debug   (F("                     Changement sur le pitch : "));
    debugln (pitchChanged);
    if ( pitchChanged || rollChanged ) nbGoodLoop = 0;  // Si oui c'est que ce n'est pas encore stabilisé
    else nbGoodLoop++;                                  // On a fait un tour stable
    debug   (F("                     Nombre de boucle(s) valide(s) : "));
    debugln (nbGoodLoop);
  }

  /* On ajuste les offset des treuils */
  debugln (F("Base::autoLevelNacelle - Mise à jour de l'offset des treuils"));
  treuil1.setOffset ( treuil1.getOffset() - prevEncodeurT2);
  treuil2.setOffset ( treuil2.getOffset() - prevEncodeurT2);
  treuil3.setOffset ( treuil3.getOffset() - prevEncodeurT2);

  /* Avant de finir, on réactive les capteurs ultrason */
  // canalBLE.enableUsStream ( true );
  // canalBLE.sendTrameEnables ( Serial2 );
  delay ( 1000 );

  debugln (F("Base::autoLevelNacelle - END"));
}

bool Base::adjustPoRNacelle ( String type, MoteurEncode &treuil ) {
  debug  (F("Base::adjustPoRNacelle - BEGIN ["));
  debug  (type);
  debugln(F("]"));
  boolean changeMade              = false;
  boolean isOk                    = false;
  const float seuilBas            = 0.5;
  const unsigned int defaultSpeed = 80;
  const long timerOn              = 100;
  const long timerOff             = 500;
  const unsigned int stepLow      = 5;
  int sensCorrection              = 0;
  int prevSensCorrection          = 0;
  unsigned int speed              = 0;
  long prevEncoder                = 0;

  while ( !isOk ) {
    /* On charge les données d'angle de la nacelle */
    debugln (F("Base::adjustPoRNacelle - Chargement des données de pitch et de roll de la nacelle"));
    if ( !getNacelleAngleInfo ( 5000 ) ) {
      debugln (F("Base::adjustPoRNacelle - !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
      debugln (F("Base::adjustPoRNacelle - !!!                                 ATTENTION                                 !!!"));
      debugln (F("Base::adjustPoRNacelle - !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
      debugln (F("Base::adjustPoRNacelle - !!! Erreur lors de la mise à jour des informations d'assiette de la nacelle   !!!"));
      debugln (F("Base::adjustPoRNacelle - END [true]"));
      return true;    // On retourne true comme ca le programme principale croit que nous avons fait un changement et
                      // il continuera donc d'essayer de corriger l'assiette
    }
    /* On determine le sens de correction */
    float infos = 0;
    if ( type == "pitch" ) {
        infos = canalBLE.getAngleInfo().pitch;
    } else {
        infos = canalBLE.getAngleInfo().roll;
    }

    debug   (F("Base::adjustPoRNacelle - Ajustement du "));
    debug   (type);
    debug   (F(" : "));
    debugln (infos);
    if ( infos > seuilBas ) sensCorrection =  -1;
    else if ( infos < -seuilBas ) sensCorrection = 1;
    else {
      stopNacelle (); // J'ai des doutes sur le fait que ca soit utile !
      isOk = true;
    }
    /* On applique la correction */
    if ( !isOk ) {
      if ( prevSensCorrection == 0 ) { // C'est la première itération
        debug   (F("Base::adjustPoRNacelle - Première itération de la correction du "));
        debug   (type);
        debug   (F(". Sens de correction : "));
        debugln (sensCorrection);
        prevSensCorrection = sensCorrection;
        speed = defaultSpeed;
      } else if ( prevSensCorrection != sensCorrection ) { // On a changé de sens de correction
        debug   (F("Base::adjustPoRNacelle - Changement du sens de correction du "));
        debug   (type);
        debug   (F(". Sens de correction : "));
        debugln (sensCorrection);
        prevSensCorrection = sensCorrection;
        speed = defaultSpeed;
      }
      /* On actionne le treuil pour essayer d'améliorer le paramètre */
      if ( sensCorrection == -1 ) {
        debug   (F("Base::adjustPoRNacelle - Action sur le treuil : Descendre [vitesse="));
        debug   (speed);
        debugln (F("]"));
        treuil.descendre ( speed );   // On fait descendre le treuil
        delay ( timerOn );            // On attend un peu
        treuil.stop ();               // On arrête de descendre le treuil
        delay ( timerOff );           // On attend un peu pour stabiliser le tout
        changeMade = true;
      } else if ( sensCorrection == 1 ) {
        debug   (F("Base::adjustPoRNacelle - Action sur le treuil : Monter [vitesse="));
        debug   (speed);
        debugln (F("]"));
        treuil.monter ( speed );      // On fait monter le treuil
        delay ( timerOn );            // On attend un peu
        treuil.stop ();               // On arrête de monter le treuil
        delay ( timerOff );           // On attend un peu pour stabiliser le tout
        changeMade = true;
      }
      /* Vérification que la puissance du moteur (vitesse) est adaptée */
      debugln (F("Base::adjustPoRNacelle - Vérification que la puissance est adaptée"));
      long abs1 = abs(treuil.getEncodeur());
      long abs2 = abs(prevEncoder);
      long temp = abs1 - abs2;
      long diff = abs(temp);
      float coef = 1.5;
      if ( type == "pitch" ) coef = 2.0;
      if ( diff < stepLow ) {
        /* On augmente la vitesse */
        speed = min (255, speed + 2);
        debug   (F("Base::adjustPoRNacelle - Augmentation de la vitesse. Nouvelle vitesse : "));
        debugln (speed);
      } else if ( diff > stepLow * coef ) {
        /* On réduit la vitesse */
        speed = max ( defaultSpeed, speed - 1);
        debug   (F("Base::adjustPoRNacelle - Diminution de la vitesse. Nouvelle vitesse : "));
        debugln (speed);
      }
      prevEncoder = treuil.getEncodeur ();
    }
  }

  debug   (F("Base::adjustPoRNacelle - END ["));
  debug   (changeMade);
  debugln (F("]"));
  return changeMade;
}

boolean Base::getNacelleAngleInfo ( unsigned long timeout ) {
    debugln (F("Base::getNacelleAngleInfo - BEGIN"));
    Timer entryTime ( timeout );            // On initialise un chrono pour voir quand on à commencé à essayer d'obtenir les informations
    entryTime.restart ();
    Timer lastResend ( 1500 );              // On initialise un chrono pour voir quand on à envoyé une demande d'info pour la dernière fois
    int tentative = 1;
    boolean received = false;
    while ( !entryTime.isExpired ( ) && !received ) {
        // Si les données de la nacelle ne sont plus suffisament récentes on les redemande
        debugln (F("Base::getNacelleAngleInfo - En attente des données de l'assiette de la nacelle..."));
        if ( lastResend.isExpired() ) {  // Si la dernière demande est trop vieille on en renvoie une autre
            debug   (F("Base::getNacelleAngleInfo - Demande d'information sur l'assiette de la nacelle envoyée [ tentative : "));
            debug   ( tentative++ );
            debugln (F(" ]"));
            canalBLE.enableAngleDataRequest ( true );
            canalBLE.sendTrame ( );
            lastResend.restart ();
        }
        BLEMessageType type = canalBLE.readBLE (  );
        handleBLEMessage ( type );
        received = ( type == BLE_TRAME_ANGLE );
    }
    canalBLE.enableAngleDataRequest ( false );
    debug   (F("Base::getNacelleAngleInfo - END ["));
    debug   (received);
    debugln (F("]"));
    return received;
}