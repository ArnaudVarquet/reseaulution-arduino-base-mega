#include <Arduino.h>
#include "CanalBLE.h"
#include "Debug.h"

CanalBLE::CanalBLE ( ) {
    
}

bool CanalBLE::setup ( ) {
    debugln ("CanalBLE::setup - BEGIN");
    Serial2.begin ( 115200 ); // Initialisation du port série
    // Initialisation du mode du pin
    pinMode ( PIN_WKE_BLE, OUTPUT );
    digitalWrite ( PIN_WKE_BLE, 1 );

    // On écrit sur le port série les infos pour l'initialisation du BLE
    debugln (F("CanalBLE::setup - Initialisation de la trame SF"));
    Serial2.println("SF,2");
    if (!attente_reponse(Serial2, "AOK", "SF", 2000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CanalBLE::setup - Initialisation de la trame Reboot"));
    Serial2.println("R,1");
    if (!attente_reponse(Serial2, "CMD", "Reboot", 4000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CanalBLE::setup - Initialisation de la trame SR"));
    Serial2.println("SR,00540000");
    if (!attente_reponse(Serial2, "AOK", "SR", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CCanalBLE::setup - Initialisation de la trame SN"));
    Serial2.println("SN,BASER");
    if (!attente_reponse(Serial2, "AOK", "SN", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CanalBLE::setup - Initialisation de la trame SS"));
    Serial2.println("SS,00000001");
    if (!attente_reponse(Serial2, "AOK", "SS", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CanalBLE::setup - Initialisation de la trame PZ"));
    Serial2.println("PZ");
    if (!attente_reponse(Serial2, "AOK", "PZ", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CanalBLE::setup - Initialisation de la trame PS"));
    Serial2.println("PS,c06dcd10e5db4d0c8240fe39f5e9605d");
    if (!attente_reponse(Serial2, "AOK", "PS", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    //------------------------Nacelle vers Base
    //Trameinfo BMS
    debugln (F("CanalBLE::setup - Initialisation de la trame BMS"));
    Serial2.println("PC,57b44761a46a4c4396432e9c1d8c3478,04,12,22");
    if (!attente_reponse(Serial2, "AOK", "PC", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    //Trame FDC
    debugln (F("CanalBLE::setup - Initialisation de la trame FDC"));
    Serial2.println("PC,ed5873c822d4495a86be61479435c806,04,01,22");
    if (!attente_reponse(Serial2, "AOK", "PC", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    //Trame distance US
    debugln (F("CanalBLE::setup - Initialisation de la trame US"));
    Serial2.println("PC,9f27001b94bf4685ac5124f65aa3b5c3,04,06,22");
    if (!attente_reponse(Serial2, "AOK", "PC", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    //Trame gimbal
    debugln (F("CanalBLE::setup - Initialisation de la trame Gimball"));
    Serial2.println("PC,e7908e861ba740088bee704cd7052c04,04,08,22");
    if (!attente_reponse(Serial2, "AOK", "PC", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    //------------------------Base vers Nacelle
    //Trame enables
    debugln (F("CanalBLE::setup - Initialisation de la trame Enable"));
    Serial2.println("PC,91b1b2a5d4534a08b153c0902d2e9e88,18,02,22");
    if (!attente_reponse(Serial2, "AOK", "PC", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    //Trame gimbal
    debugln (F("CanalBLE::setup - Initialisation de la trame Gimball"));
    Serial2.println("PC,f0b7f97fe92b440b82f1599c2f2e9fa2,18,09,22");
    if (!attente_reponse(Serial2, "AOK", "PC", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    //------------------------Nacelle vers Base
    //Trame POS ANG
    debugln (F("CanalBLE::setup - Initialisation de la trame Angle"));
    Serial2.println("PC,e50aef2aef8211eb9a030242ac130003,04,04,22");
    if (!attente_reponse(Serial2, "AOK", "PC", 1000, 100)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CanalBLE::setup - Initialisation de la trame Reboot"));
    Serial2.println("R,1");
    if (!attente_reponse(Serial2, "CMD", "Reboot", 4000, 3000)) {
        debugln (F("CanalBLE::setup - END [false]"));
        return false;
    }

    debugln (F("CanalBLE::setup - END [true]"));
    return true;     
}

bool CanalBLE::isConnected ( ) {
    return connected;
}

bool CanalBLE::connect ( unsigned long timeout ) {
    debugln ("CanalBLE::connect - BEGIN");

    Serial2.println("E,0,049162E1B86F");
    if (!attente_reponse(Serial2, "Connected", "Connexion..", timeout, 100)) {
        debugln (F("CanalBLE::connect - END [false]"));
        connected = false;
        return false;
    }
    
    Serial2.println("B,0");
    if (!attente_reponse(Serial2, "Passcode:", "Passcode", 500, 100)) {
        debugln (F("CanalBLE::connect - END [false]"));
        connected = false;
        return false;
    }

    Serial2.println(_BLE_PASSCODE);
    if (!attente_reponse(Serial2, "Bonded", "bonding", 5000, 100)) {
        debugln (F("CanalBLE::connect - END [false]"));
        connected = false;
        return false;
    }

    connected = true;
    debugln (F("CanalBLE::connect - END [true]"));
    return true;
    debugln ("CanalBLE::connect - END");
}

void CanalBLE::setLedPosition ( unsigned int aPosition = 0 ) {
    trame.ledPosition = aPosition;
}

void CanalBLE::enableLightAlert ( bool b ) {
    trame.lightAlert = b;
}

void CanalBLE::enableSoundAlert ( bool b ) {
    trame.soundAlert = b;
}

void CanalBLE::enableAlimGimbal ( bool b ) {
    trame.alimGimbal = b;
}

void CanalBLE::enableAlimJetson ( bool b ) {
    trame.alimJetson = b;
}

void CanalBLE::enableNacelleCharger ( bool b ) {
    trame.nacelleCharger = b;
}

void CanalBLE::enableUsStream ( bool b ) {
    trame.usStream = b;
}

void CanalBLE::enableFdcStream ( bool b ) {
    trame.fdcStream = b;
}

void CanalBLE::enableBmsDataRequest ( bool b ) {
    trame.bmsDataRequest = b;
}

void CanalBLE::enableAngleDataRequest ( bool b ) {
    trame.angleDataRequest = b;
}

void CanalBLE::sendTrame ( ) {
    debugln ("CanalBLE::sendTrame - BEGIN");
    unsigned int octet1 = 0;
    unsigned int octet2 = trame.ledPosition;

    if (trame.lightAlert) bitSet(octet1, 0);
    if (trame.soundAlert) bitSet(octet1, 1);
    if (trame.alimGimbal) bitSet(octet1, 2);
    if (trame.alimJetson) bitSet(octet1, 3);
    if (trame.usStream) bitSet(octet1, 4);
    if (trame.fdcStream) bitSet(octet1, 5);
    if (trame.bmsDataRequest) bitSet(octet1, 6);
    if (trame.angleDataRequest) bitSet(octet1, 7);
    if (trame.nacelleCharger) bitSet(octet2, 4);

    String frame = "SUW," + _ID_TRAME_ENABLE_LONG + "," + intToHex(octet1, 2) + intToHex(octet2, 2);
    debug   (F("CanalBLE::sendTrame - Envoi de la trame suivante à la nacelle via BLE : "));
    debugln (frame);
    Serial2.println (frame);
    debugln (F("CanalBLE::sendTrame - END"));
}

bool CanalBLE::isFDCTrameExpired ( ) {
    return timerFDC.isExpired ();
}

BLEMessageType CanalBLE::readBLE ( ) {
    debugln (F("CanalBLE::readBLE - BEGIN"));
    debugln (F("CanalBLE::readBLE - reading Serial... "));
    if ( readSerial ( Serial2, buffer, '\n' ) ) {
        debugln (F("CanalBLE::readBLE - données disponibles"));
        if ( buffer.startsWith ("Connected") ) {
            connected = true;
            debugln (F("CanalBLE::readBLE - END [ BLE_CONNECTED ]"));
            buffer = "";
            return BLE_CONNECTED;
        } else if ( buffer.startsWith ("Connection End") ) {
            connected = false;
            debugln (F("CanalBLE::readBLE - END [ BLE_CONNECTION_END ]"));
            buffer = "";
            return BLE_CONNECTION_END;
        } else if ( buffer.startsWith ("WV,") && buffer.endsWith(".\r") ) {
            debugln (F("CanalBLE::readBLE - les données commencent par WV. On analyse les données"));
            String _frameId = buffer.substring (3, 7);
            String _frame = buffer.substring (8, buffer.length() - 2);
            debug (F("CanalBLE::readBLE - frameId : "));
            debugln ( _frameId );
            debug (F("CanalBLE::readBLE - frame : "));
            debugln ( _frame );
            if ( _frameId == _IDTRAME_BMSSHORT ) { // Trame Système de gestion des batteries
                debugln (F("CanalBLE::readBLE - Analyse des données du système de gestion des batteries"));
                //% Batt
                bmsInfo.pCentBatt = parseInfo (_frame, 0, 2);
                if ( bmsInfo.pCentBatt == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [%batt] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                //Vbatt
                bmsInfo.vBatt = parseInfo (_frame, 2, 6);
                if ( bmsInfo.vBatt == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [Vbatt] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                //TTe
                bmsInfo.tte = parseInfo (_frame, 6, 10);
                if ( bmsInfo.tte == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [TTe] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                //TTf
                bmsInfo.ttf = parseInfo (_frame, 10, 14);
                if ( bmsInfo.ttf == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [TTf] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                //En Charge
                int temp = parseInfo (_frame, 14, 16);
                switch ( temp ) {
                    case -1 :
                        debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [EnCharge] !!!"));
                        buffer = "";
                        return BLE_ERROR;
                        break;
                    case 0 :
                        bmsInfo.charge = false;
                        break;
                    case 1 :
                        bmsInfo.charge = true;
                        break;
                }
                //Temperature
                bmsInfo.temperaturePack = parseInfo (_frame, 16, 20);
                if ( bmsInfo.temperaturePack == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [Température] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                //Courant
                bmsInfo.current = parseInfo (_frame, 16, 20);
                if ( bmsInfo.current == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [Courant] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                if (bmsInfo.current > 0x7fffL)
                bmsInfo.current -= 0x10000L;
                
                debugln (F("CanalBLE::readBLE - Frame BMS"));
                debug   (F("CanalBLE::readBLE -   pCentBattNacelle : "));
                debugln (bmsInfo.pCentBatt);
                debug   (F("CanalBLE::readBLE -   vBattNacelle : "));
                debugln (float(bmsInfo.vBatt / 1000.0));
                debug   (F("CanalBLE::readBLE -   tteNacelle : "));
                debugln (bmsInfo.tte);
                debug   (F("CanalBLE::readBLE -   ttfNacelle : "));
                debugln (bmsInfo.ttf);
                debug   (F("CanalBLE::readBLE -   nacelleEnCharge : "));
                debugln (bmsInfo.charge);
                debug   (F("CanalBLE::readBLE -   temperaturePack : "));
                debugln (bmsInfo.temperaturePack / 100);
                debug   (F("CanalBLE::readBLE -   currentNacelle : "));
                debugln (bmsInfo.current);

                // On met à jour l'horodatage pour connaître la "fraicheur" de cette trame
                timerBMS.restart ();

                debugln (F("CanalBLE::readBLE - END [ BLE_TRAME_BMS ]"));
                buffer = "";
                return BLE_TRAME_BMS;
            } else if ( _frameId == _IDTRAME_FDCSHORT ) { // Trame Fin de course
                debugln (F("CanalBLE::readBLE - Analyse des données des capteurs de fin de course"));
                // FDC
                int octet = parseInfo (_frame, 0, 2);
                if ( octet == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [FDC] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }

                fdc1 = (bitRead(octet, 0)?true:false);
                fdc2 = (bitRead(octet, 1)?true:false);
                fdc3 = (bitRead(octet, 2)?true:false);

                debugln (F("CanalBLE::readBLE - Frame FDC"));
                debug   (F("CanalBLE::readBLE -   Fin de course 1 : "));
                debugln (fdc1);
                debug   (F("CanalBLE::readBLE -   Fin de course 2 : "));
                debugln (fdc2);
                debug   (F("CanalBLE::readBLE -   Fin de course 3 : "));
                debugln (fdc3);

                // On met à jour l'horodatage pour connaître la "fraicheur" de cette trame
                timerFDC.restart ();

                debugln (F("CanalBLE::readBLE - END [ BLE_TRAME_FDC ]"));
                buffer = "";
                return BLE_TRAME_FDC;
            } else if ( _frameId == _IDTRAME_USSHORT ) { // Trame Ultra Son
                debugln (F("CanalBLE::readBLE - Analyse des données des ultra son"));
                // Distance 1
                usInfo.us1 = parseInfo ( _frame, 0, 4);
                if ( usInfo.us1 == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [distanceUs1] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                // Distance 2
                usInfo.us2 = parseInfo ( _frame, 4, 8);
                if ( usInfo.us2 == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [distanceUs2] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                // Distance 3
                usInfo.us3 = parseInfo ( _frame, 8, 12);
                if ( usInfo.us3 == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [distanceUs3] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }

                debugln (F("CanalBLE::readBLE - Frame US"));
                debug   (F("CanalBLE::readBLE -   Distance US 1 "));
                debugln (usInfo.us1);
                debug   (F("CanalBLE::readBLE -   Distance US 2 "));
                debugln (usInfo.us2);
                debug   (F("CanalBLE::readBLE -   Distance US 3 "));
                debugln (usInfo.us3);

                // On met à jour l'horodatage pour connaître la "fraicheur" de cette trame
                timerUS.restart ();

                debugln (F("CanalBLE::readBLE - END [ BLE_TRAME_US ]"));
                buffer = "";
                return BLE_TRAME_US;
            } else if ( _frameId == _IDTRAME_GIMBALSHORT ) { // Trame Gimball
                debugln (F("CanalBLE::readBLE - Analyse des données de la gimball"));
                // Batterie State
                gimbalInfo.gimbalBatterieState = parseInfo ( _frame, 0, 4 );
                if ( gimbalInfo.gimbalBatterieState == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [gimballBatterieState] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                // State
                gimbalInfo.gimbalState = parseInfo ( _frame, 4, 6);
                if ( gimbalInfo.gimbalState == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [gimballState] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                // Pan Mode
                gimbalInfo.gimbalPanMode = parseInfo ( _frame, 6, 8);
                if ( gimbalInfo.gimbalPanMode == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [gimballPanMode] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                // Pos X
                gimbalInfo.gimbalPosX = parseInfo ( _frame, 8, 12 );
                if ( gimbalInfo.gimbalPosX == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [gimballPosX] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                // Pos Y
                gimbalInfo.gimbalPosY = parseInfo ( _frame, 12, 16 );
                if ( gimbalInfo.gimbalPosY == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [gimballPosY] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }

                debugln (F("CanalBLE::readBLE - Frame Gimball"));
                debug   (F("CanalBLE::readBLE -   State "));
                debugln (gimbalInfo.gimbalState);
                debug   (F("CanalBLE::readBLE -   Pan Mode "));
                debugln (gimbalInfo.gimbalPanMode);
                debug   (F("CanalBLE::readBLE -   Batterie State "));
                debugln (gimbalInfo.gimbalBatterieState);
                debug   (F("CanalBLE::readBLE -   Pos X "));
                debugln (gimbalInfo.gimbalPosX);
                debug   (F("CanalBLE::readBLE -   Pos Y "));
                debugln (gimbalInfo.gimbalPosY);

                debugln (F("CanalBLE::readBLE - END [ BLE_TRAME_GIMBAL ]"));
                buffer = "";
                return BLE_TRAME_GIMBALL;
            } else if ( _frameId == _IDTRAME_POSANGSHORT ) { // Trame Angle de la nacelle
                debugln (F("CanalBLE::readBLE - Analyse des données des angles de la nacelle"));
                // Roll
                long rollTemp = parseInfo ( _frame, 0, 4 );
                if ( rollTemp == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [roll] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                if (rollTemp > 0x7fffL) rollTemp -= 0x10000L;
                angleInfo.roll = rollTemp / 100.0;
                // Pitch
                long pitchTemp = parseInfo ( _frame, 4, 8 );
                if ( pitchTemp == -1 ) {
                    debugln (F("CanalBLE::readBLE - !!! Erreur lors de l'analyse de la trame [pitch] !!!"));
                    buffer = "";
                    return BLE_ERROR;
                }
                if (pitchTemp > 0x7fffL) pitchTemp -= 0x10000L;
                angleInfo.pitch = pitchTemp / 100.0;

                debugln (F("CanalBLE::readBLE - Frame Angle"));
                debug   (F("CanalBLE::readBLE -   Roll "));
                debugln (angleInfo.roll);
                debug   (F("CanalBLE::readBLE -   Pitch "));
                debugln (angleInfo.pitch);


                debugln (F("CanalBLE::readBLE - END [ BLE_TRAME_ANGLE ]"));
                buffer = "";
                return BLE_TRAME_ANGLE;
            } else {
                debug   (F("CanalBLE::readBLE - !!! Identifiant de trame inconnu : "));
                debug   (_frameId);
                debugln (F(" !!!"));
                debugln (F("CanalBLE::readBLE - END [ BLE_ERROR ]"));
                buffer = "";
                return BLE_ERROR;
            }
        } else {
            debugln (F("CanalBLE::readBLE - !!! Données non gérées !!!"));
            debug (F("CanalBLE::readBLE - "));
            debugln (buffer);
            debugln (F("CanalBLE::readBLE - END [ BLE_ERROR ]"));
            buffer = "";
            return BLE_ERROR;
        }
    } else {
        debugln (F("CanalBLE::readBLE - pas de délimiteur trouvé..."));
        debugln (F("CanalBLE::readBLE - END [ BLE_NOTHING ]"));
        return BLE_NOTHING;
    }
}

int CanalBLE::parseInfo (String _frame, unsigned int start, unsigned int end) {
    char * endPtr;
    int str_len = _frame.substring(start, end).length() + 1;
    char char_array[str_len];

    _frame.substring(start, end).toCharArray(char_array, str_len);

    int result = strtol(char_array, &endPtr, 16);
    if ( endPtr == char_array ) return -1;
    return result;
}

bool CanalBLE::isDocked () {
    return ( fdc1 || fdc2 || fdc3 );
}

AngleInfo CanalBLE::getAngleInfo ( ) {
    return angleInfo;
}