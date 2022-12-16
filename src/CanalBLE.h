#ifndef CANALBLE_H
#define CANALBLE_H
#include <Arduino.h>
#include "Canal.h"
#include "Timer.h"
#include "BMS.h"

#define PIN_WKE_BLE              6
/* mot de passe pour la connexion BLE */
const unsigned long _BLE_PASSCODE           = 38243845;
/* identifiant long de la trame contenant les requêtes envoyée à la nacelle */
const String _ID_TRAME_ENABLE_LONG          = "91b1b2a5d4534a08b153c0902d2e9e88";
/* */
const String _ID_TRAME_GIMBAL_LONG          = "f0b7f97fe92b440b82f1599c2f2e9fa2";
/* */
const String _IDTRAME_BMSSHORT              = "000B";
/* */
const String _IDTRAME_FDCSHORT              = "000D";
/* */
const String _IDTRAME_USSHORT               = "000F";
/* */
const String _IDTRAME_POSANGSHORT           = "0019";
/* */
const String _IDTRAME_GIMBALSHORT           = "0011";

/**
 * @brief Structure des trames envoyées à la nacelle par BLE
 */
struct TrameBLE {
    unsigned int ledPosition;   // indicateur de la positio ndes LED à allumer
    bool lightAlert;            // indicateur de l'état des lumières
    bool soundAlert;            // indicateur de l'était du signal d'alert
    bool alimGimbal;            // indicateur d'alimentation de la Gimbal
    bool alimJetson;            // indicateur d'alimentation de la Jetson
    bool nacelleCharger;        // indicateur d'activation du chargement de la nacelle
    bool usStream;              // indicateur de mise en route du streaming des données UltraSon
    bool fdcStream;             // indicateur de mise en route du streaming des données des capteurs fin de course
    bool bmsDataRequest;        // indicateur de demande d'information de la batterie
    bool angleDataRequest;      // indicateur de demande d'information sur l'assiette de la nacelle
};

/**
 * @brief Structure de donnée pour les mesure des ultrasons
 */
struct USInfo {
    long us1;
    long us2;
    long us3;
};

/**
 * @brief Structure de donnée pour les informations remontées par la gimbal
 */
struct GimbalInfo {
    int gimbalState;
    int gimbalPanMode;
    int gimbalBatterieState;
    int gimbalPosX;
    int gimbalPosY;
};

/**
 * @brief Structure de données pour les informations d'assiette de la nacelle
 */
struct AngleInfo {
    float pitch;
    float roll;
};

/**
 * @brief Enumération des type de message pouvant être analysé sur le canal BLE
 * 
 */
typedef enum {
    BLE_CONNECTED,
    BLE_CONNECTION_END,
    BLE_ERROR,
    BLE_TRAME_BMS,
    BLE_TRAME_FDC,
    BLE_TRAME_US,
    BLE_TRAME_ANGLE,
    BLE_TRAME_GIMBALL,
    BLE_NOTHING
} BLEMessageType;

class CanalBLE : public Canal {
    protected:
        /* indique si on est connecté en BLE ou non */
        bool connected;
        /* Trame qu'on envoi à la nacelle en BLE */
        TrameBLE trame = { 0, false, false, false, false, false, false, false, false, false };
        /* Etat du capteur de fin de course de la nacelle numéro 1 */
        bool fdc1;
        /* Etat du capteur de fin de course de la nacelle numéro 2 */
        bool fdc2;
        /* Etat du capteur de fin de course de la nacelle numéro 3 */
        bool fdc3;
        /* Information sur la batterie de la nacelle */
        BMSInfo bmsInfo = { 0, 0, 0, 0, 0, 0, false };
        /* Distance mesuré par les ultrasons */
        USInfo usInfo = { 0, 0, 0 };
        /* Information remontées par la gimbal */
        GimbalInfo gimbalInfo = { 0, 0, 0, 0, 0 };
        /* Information sur l'assiette de la nacelle */
        AngleInfo angleInfo = { 0, 0 };
        /* Timer pour mesurer la fraicheur des informations sur les capteurs fin de course */
        Timer timerFDC = Timer ( 500 );   // Une trame FDC est obsolète au bout d'une demi seconde
        /* Timer pour mesurer la fraicheur des informations sur les Ultra Sons */
        Timer timerUS = Timer ( 500 );    // Une trame US est obsolète au bout d'une demi seconde
        /* Timer pour mesurer la fraicheur des informations sur la batterie */
        Timer timerBMS = Timer ( 30000 ); // Une trame BMS est obsolète au bout de 30 secondes

    public:
        /**
         * @brief Construit un canal de communication avec la nacelle via BLE
         */
        CanalBLE (  );
        /**
         * @brief initialise ne canal de communication avec la nacelle via BLE
         * 
         */
        bool setup ( );
        /**
         * @brief Indique si on est connecté à la nacelle en BLE ou non 
         * 
         * @return true si on est connecté
         * @return false sinon
         */
        bool isConnected ( );
        /**
         * @brief Se connecte via le canal BLE à la nacelle
         * 
         * @param timeout temps en millisecondes après lequel on abandonne la connexion initiale
         * @return true si la connexion est établie
         * @return false sinon
         */
        bool connect ( unsigned long timeout = 2000 );
        /**
         * @brief Set the Led Position value
         * 
         * @param ledPosition position des led à allumer
         */
        void setLedPosition ( unsigned int ledPosition = 0 );
        /**
         * @brief Active ou désactive l'indicateur pour l'allumage des lumières d'alerte
         * 
         * @param b indicateur d'activation
         */
        void enableLightAlert ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour l'allumage de l'alerte sonore
         * 
         * @param b indicateur d'activation
         */
        void enableSoundAlert ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour l'allumage de la gimbal
         * 
         * @param b indicateur d'activation
         */
        void enableAlimGimbal ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour l'allumage de la jetson
         * 
         * @param b indicateur d'activation
         */
        void enableAlimJetson ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour la mise en charge de la nacelle
         * 
         * @param b indicateur d'activation
         */
        void enableNacelleCharger ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour l'envoi des informations des UltraSons
         * 
         * @param b indicateur d'activation
         */
        void enableUsStream ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour l'envoi des informations des capteurs de fin de course
         * 
         * @param b indicateur d'activation
         */
        void enableFdcStream ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour la demande d'information sur la batterie de la nacelle
         * 
         * @param b indicateur d'activation
         */
        void enableBmsDataRequest ( bool b );
        /**
         * @brief Active ou désactive l'indicateur pour la demande d'information sur l'assiette de la nacelle
         * 
         * @param b indicateur d'activation
         */
        void enableAngleDataRequest ( bool b );
        /**
         * @brief Envoie la trame à la nacelle via le BLE
         */
        void sendTrame ( );
        /**
         * @brief Indique si la trame avec les iformation de fin de course est obsolète ou non
         * 
         * @return true si la trame est obsolète
         * @return false sinon
         */
        bool isFDCTrameExpired ( );
        /**
         * @brief lit les message sur le port BLE
         * 
         * @return BLEMessageType Type du message lu
         */
        BLEMessageType readBLE ( );
        /**
         * @brief Fonction qui parse une trame
         * 
         * @param _frame la trame à analyser
         * @param start point de départ de l'analyse
         * @param end point de fin de l'analyse
         * @return int l'entier extrait de la trame entre le point de départ et le point de fin.
         */
        static int parseInfo (String _frame, unsigned int start, unsigned int end);
        /**
         * @brief Indique si la nacelle est dockée ou non. Pour cela, il suffit qu'un
         * des trois capteurs de fin de course soit enclenché
         * 
         * @return true si la nacelle est dockée
         * @return false sinon
         */
        bool isDocked ();
        /**
         * @brief Retourne les informations d'assiette de la nacelle
         * 
         * @return AngleInfo assiette de la nacelle
         */
        AngleInfo getAngleInfo ( );
};

#endif