#ifndef COMPOSANT_H
#define COMPOSANT_H

class Composant {
    protected:
        /* numéro du pin sur lequel est branché le composant */
        unsigned int pin;
        /* mode du composant (INPUT ou OUTPUT) */
        unsigned int mode;
    public:
        /**
         * @brief Construct a new Composant object
         * 
         * @param pin numéro du pin sur lequel est branché le composant
         * @param mode mode du composant (INPUT ou OUTPUT)
         */
        Composant ( unsigned int pin, unsigned int mode );
        /**
         * @brief Initialise le composant
         */
        void setup ();
};

#endif