#ifndef LED_H
#define LED_H
#include "Composant.h"

class LED : public Composant {
    protected:

    public:
        /**
         * @brief Construct a new LED object
         * 
         * @param pin numéro du pin sur lequel est connecté la LED
         */
        LED ( unsigned int pin );
        /**
         * @brief Allume la LED
         */
        void switchOn ( );
        /**
         * @brief Eteind la LED
         */
        void switchOff ( );

};

#endif