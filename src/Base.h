#ifndef BASE_H
#define BASE_H
#include "Moteur.h"
#include "MoteurEncode.h"
#include "CapteurDistance.h"
#include "CapteurFDC.h"
#include "LED.h"
#include "BMS.h"
#include "Timer.h"
#include "Mouvement.h"
#include "CanalBLE.h"
#include "Switch.h"

#define PIN_EN_TREUIL1           13
#define PIN_IN1_TREUIL1          12
#define PIN_IN2_TREUIL1          11
#define PIN_CHA_TREUIL1          2
#define PIN_CHB_TREUIL1          5
#define PIN_EN_TREUIL2           8
#define PIN_IN1_TREUIL2          22
#define PIN_IN2_TREUIL2          23
#define PIN_CHA_TREUIL2          3
#define PIN_CHB_TREUIL2          4
#define PIN_EN_TREUIL3           9
#define PIN_IN1_TREUIL3          24
#define PIN_IN2_TREUIL3          25
#define PIN_CHA_TREUIL3          18
#define PIN_CHB_TREUIL3          42
#define PIN_EN_ROUE1             44
#define PIN_IN1_ROUE1            46
#define PIN_IN2_ROUE1            47
#define PIN_CHA_ROUE1            19
#define PIN_CHB_ROUE1            43
#define PIN_EN_ROUE2             45
#define PIN_IN1_ROUE2            48
#define PIN_IN2_ROUE2            49
#define PIN_DISTANCE_AV          A15
#define PIN_DISTANCE_AR          A0
#define PIN_FDC                  38
#define PIN_LED_ONOFF            53
#define TIMER_BMS                30000
#define PIN_EN_CHARGE_BASE       10
        
class Base {
    
    protected:
        /* Canal BLE utilisé pour communiquer avec la nacelle */
        CanalBLE canalBLE = CanalBLE ( );
        /* Treuil 1 permettant de faire monter et descendre la nacelle (avec roue codeuse) */
        MoteurEncode treuil1 = MoteurEncode ( PIN_EN_TREUIL1, PIN_IN1_TREUIL1, PIN_IN2_TREUIL1, PIN_CHA_TREUIL1, PIN_CHB_TREUIL1 );
        /* Treuil 2 permettant de faire monter et descendre la nacelle (avec roue codeuse) */
        MoteurEncode treuil2 = MoteurEncode ( PIN_EN_TREUIL2, PIN_IN1_TREUIL2, PIN_IN2_TREUIL2, PIN_CHA_TREUIL2, PIN_CHB_TREUIL2 );
        /* Treuil 3 permettant de faire monter et descendre la nacelle (avec roue codeuse) */
        MoteurEncode treuil3 = MoteurEncode ( PIN_EN_TREUIL3, PIN_IN1_TREUIL3, PIN_IN2_TREUIL3, PIN_CHA_TREUIL3, PIN_CHB_TREUIL3 );
        /* Seconde roue permettant de déplacement horizontale sur le rail (avec roue codeuse) */
        MoteurEncode roue1 = MoteurEncode ( PIN_EN_ROUE2, PIN_IN1_ROUE2, PIN_IN2_ROUE2, PIN_CHA_ROUE1, PIN_CHB_ROUE1 );
        /* Seconde roue permettant de déplacement horizontale sur le rail (sans roue codeuse) */
        Moteur roue2 = Moteur ( PIN_EN_ROUE2, PIN_IN1_ROUE2, PIN_IN2_ROUE2 );
        /* Capteur de distance avant */
        CapteurDistance capteurDistAvant = CapteurDistance ( PIN_DISTANCE_AV );
        /* Capteur de distance arriere */
        CapteurDistance capteurDistArriere = CapteurDistance ( PIN_DISTANCE_AR );
        /* Capteur de fin de crouse qui se trouve à l'avant et à l'arrière de la station ( 1 seul object pour tous les capteur )*/
        CapteurFDC capteurFDC = CapteurFDC ( PIN_FDC );
        /* LED du bouton ON/OFF de la station */
        LED ledOnOff = LED ( PIN_LED_ONOFF);
        /* Information sur les batteries */
        BMSInfo bmsInfo = { 0, 0, 0, 0, 0, 0, false };
        /* Timer pour mesurer la fraicheur des information BMS */
        Timer timerBMSInfo = Timer ( TIMER_BMS );
        /* Timer pour les demandes des capteurs fin de course */
        Timer timerFDCStream = Timer ( 2000 );
        /* Instructions pour les mouvements horizontaux */
        HorizontalInstruction hInstruction = { hNONE, 0, 0 };
        /* Instructions pour les mouvements verticaux */
        VerticalInstruction vInstruction = { vNONE, 0, 0 };
        /* intérrupteur pour l'alimentation de la batterie de la base */
        Switch switchBMS = Switch ( PIN_EN_CHARGE_BASE );
        /**
         * @brief Cette fonction permet d'ajuster le Pitch ou le Roll de la nacelle
         * 
         * @param type indique si on souhaite changer le Pitch ou Roll (utilisé uniquement pour les logs)
         * @param treuil treuil sur lequel appliquer les ajustements
         * @param getInfo fonction à utiliser pour obtenir soit le Pitch soit le Roll
         * @return true si un ajustement a été fait
         * @return false sinon
         */
        bool adjustPoRNacelle ( String type, MoteurEncode &treuil );


    public:
        /**
         * @brief Construct a new Base object
         */
        Base ();
        /**
         * @brief Initialise les différents éléments de la base
         */
        void setup ();
        /**
         * @brief Get the Treuil1 object
         * 
         * @return MoteurEncode retourne le treuil 1
         */
        MoteurEncode getTreuil1 ();
        /**
         * @brief Get the Treuil2 object
         * 
         * @return MoteurEncode retourne le treuil 2
         */
        MoteurEncode getTreuil2 ();
        /**
         * @brief Get the Treuil3 object
         * 
         * @return MoteurEncode retourne le treuil 3
         */
        MoteurEncode getTreuil3 ();
        /**
         * @brief Get the Roue1 object
         * 
         * @return MoteurEncode retourne la roue 1
         */
        MoteurEncode getRoue1 ();
        /**
         * @brief Get the Roue2 object
         * 
         * @return Moteur retourne le Roue 2
         */
        Moteur getRoue2 ();
        /**
         * @brief Get the Led On Off object
         * 
         * @return LED LED du bouton on/off du robot
         */
        LED getLedOnOff ();
        /**
         * @brief Retourne les informations sur la batterie de la base
         * 
         * @return BMSInfo informations sur la batterie
         */
        BMSInfo getBMSInfo ();
        /**
         * @brief Fonction qui fait faire un retour à la station de chargement de la base
         * 
         */
        void dock ( HorizontalDirection direction = FORWARD );
        /**
         * @brief Fonction qui reboot l'arduino
         */
        void reboot();
        /**
         * @brief Gère les mouvements horizontaux (sur le rail) du robot
         * La gestion de ces mouvements se fait en fonction de la variable hinstruction qui est une variable globale
         */
        void H_MOOVE ();
        /**
         * @brief Retourne la position de la base
         * 
         * @return HorizontalDirection position de la base
         */
        HorizontalDirection getBasePosition ();
        /**
         * @brief Arrête les mouvements de la base
         */
        void stopBase ();
        /**
         * @brief Lance la procédure de démarrage du robot. Cette procédure va simplement vérifier que la base et la nacelle
         * sont sur leur base de chargement. Si non, elle va les docker
         * 
         * @return true si la procédure à pu aller au bout
         * @return false sinon
         */
        bool startupSequence ( );
        /**
         * @brief Envoi à la nacelle une demande d'activation du Streaming des capteurs fin de course si la dernière demande
         * est obsolète
         */
        void enableNacelleFDCStream ( );
        /**
         * @brief Fonction qui agit en fonction du type de message BLE passé en paramètre
         * 
         * @param messageType type du message BLE
         */
        void handleBLEMessage ( BLEMessageType messageType );
        /**
         * @brief Fait descendre les 3 moteurs (treuils) de la nacelle
         * Attention : il est déconseillé d'utiliser cette fonction pour
         * manipuler les treuils car il ne permet pas d'adapter la vitesse
         * de chacun de façon indépendante afin de concerver une assiette
         * droite de la nacelle
         * 
         * @param vitesse vitesse à laquelle descendre
         */
        void descendreNacelle (unsigned int vitesse);
        /**
         * @brief Stop le mouvement des 3 moteurs (treuils) de la nacelle
         * 
         */
        void stopNacelle ( );
        /**
         * @brief Ajuste automatiquement l'assiette de la nacelle
         * 
         * @return boolean true si la procédure a pu se dérouler jusqu'au bout, false sinon
         */
        void autoLevelNacelle ( );
        /**
         * @brief Fonction qui demande les informations à la nacelle sur l'état du roll et du pitch de la nacelle
         * 
         * @param timeout temps au bout duquel on concidère que la demande a échouée
         * @return boolean true si on a eu les informations, false sinon
         */
        boolean getNacelleAngleInfo ( unsigned long timeout = 2000 );

};



#endif