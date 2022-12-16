#ifndef SWITCH_H
#define SWITCH_H
#include "Composant.h"

class Switch : public Composant {
    protected:

    public:
        /**
         * @brief Construct a new LED object
         * 
         * @param pin numéro du pin sur lequel est connecté la LED
         */
        Switch ( unsigned int pin );
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