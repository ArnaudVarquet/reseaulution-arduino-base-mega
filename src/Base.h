#ifndef BASE_H
#define BASE_H
#include "Moteur.h"
#include "MoteurEncode.h"
#include "CapteurDistance.h"
#include "CapteurFDC.h"
#include "LED.h"

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
 

        
class Base {
    
    protected:
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
        LED ledOnOff = LED ( PIN_LED_ONOFF );
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
        
};



#endif