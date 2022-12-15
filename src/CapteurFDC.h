#ifndef CAPTEURFDC_H
#define CAPTEURFDC_H
#include "Composant.h"

class CapteurFDC : public Composant {
    protected:

    public:
        /**
         * @brief Construct a new Capteur Fin De Course object
         * 
         * @param pin numéro du pin sur lequel est connecté le capteur fin de course
         */
        CapteurFDC ( unsigned int pin );
        /**
         * @brief Indique si le capteur détecte une fin de course ou non
         * 
         * @return true si une FDC est détectée
         * @return false sinon
         */
        bool isFDC ( );
};

#endif