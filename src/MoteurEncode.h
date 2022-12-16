#ifndef MOTEUR_ENCODE_H
#define MOTEUR_ENCODE_H

#include "Moteur.h"

class MoteurEncode : public Moteur {
    protected:
        /* numéro du pin Ch A */
        unsigned int pinChA;
        /* numéro du pin Ch B */
        unsigned int pinChB;
        /* numéro du pin encodeur */
        long encodeur;
        /* numéro du pin offset */
        long offset;

    public:
        /**
         * @brief Construit un objet MoteurEncode 
         * 
         * @param aPinEn numéro du pin Enable
         * @param aPinIn1 numéro du pin Pont H 1
         * @param aPinIn2 numéro du pin Pont H 2
         * @param aPinChA numéro du pin Ch A
         * @param aPinChB numéro du pin Cb B
         */
        MoteurEncode ( unsigned int aPinEn, unsigned int aPinIn1, unsigned aPinIn2, unsigned int aPinChA, unsigned int aPinChB );
        /**
         * @brief Initialise les "pin mode" pour les pins du moteur
         */
        void setup ();
        /**
         * @brief Get the Pin Ch A object
         * 
         * @return unsigned int numéro du pin Ch A
         */
        unsigned int getPinChA ();
        /**
         * @brief Get the Pin Ch B object
         * 
         * @return unsigned int numéro du pin Ch B
         */
        unsigned int getPinChB ();
        /**
         * @brief Gère l'increment ou le decrement de l'encodeur
         */
        void interrupt ();
        /**
         * @brief Set the Offset object
         * 
         * @param offset valeur de l'offset à affecter
         */
        void setOffset ( long offset );
        /**
         * @brief Get the Offset object
         * 
         * @return long valeur de l'offset
         */
        long getOffset ();
        /**
         * @brief Set the Encodeur object
         * 
         * @param encodeur valeur de l'encodeur à affecter
         */
        void setEncodeur ( long encodeur );
        /**
         * @brief Get the Encodeur object
         * 
         * @return long valeur de l'encodeur
         */
        long getEncodeur ();

};

#endif