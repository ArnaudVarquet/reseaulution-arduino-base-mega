#ifndef CAPTEURDISTANCE_H
#define CAPTEURDISTANCE_H
#include "Composant.h"

const float _RATIO                  = 130.0 / 2.05;
const float _VPERSTEP               = 5.0 / 1024;
const unsigned int _READING_DELAY   = 50;

class CapteurDistance : public Composant {
    protected:

    public:
        /**
         * @brief Construct a new Capteur Distance object
         * 
         * @param pin numéro du pin sur lequel le capteur est connecté
         */
        CapteurDistance ( unsigned int pin );
        /**
         * @brief Retourne la distance mesurée par le capteur
         * 
         * @param nbReading nombre de fois qu'on interroge le capteur afin de fournir une moyenne
         * @return int la distance mesurée en cm
         */
        int readDistance ( unsigned int nbReading = 10 );
};


#endif