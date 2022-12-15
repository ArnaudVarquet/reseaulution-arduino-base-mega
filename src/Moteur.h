#ifndef MOTEUR_H
#define MOTEUR_H

class Moteur {
    protected:
        /* numéro du pin Enable */
        unsigned int pinEn;
        /* numéro du pin Pont H 1 */
        unsigned int pinIn1;
        /* numéro du pin Pont H 2 */
        unsigned int pinIn2;

    public:
        /**
         * @brief Construct a new Moteur object
         * 
         * @param pinEn numéro du pin Enable
         * @param pinIn1 numéro du pin Pont H 1
         * @param pinIn2 numéro du pin Pont H 2
         */
        Moteur ( unsigned int pinEn, unsigned int pinIn1, unsigned pinIn2 );
        /**
         * @brief Initialise les "pin mode" pour les 3 pins du moteur
         */
        void setup ();
        /**
         * @brief Get the Pin En object
         * 
         * @return unsigned int numéro du pin Enable
         */
        unsigned int getPinEn ();
        /**
         * @brief Get the Pin In1 object
         * 
         * @return unsigned int numéro du pin pont H 1
         */
        unsigned int getPinIn1 ();
        /**
         * @brief Get the Pin In2 object
         * 
         * @return unsigned int numéro du pin pont H 2
         */
        unsigned int getPinIn2 ();
        /**
         * @brief Fait tourner le moteur pour faire monter dans le cas d'un treuil
         * 
         * @param vitesse vitesse à laquelle faire tourner le moteur
         */
        void monter ( unsigned int vitesse );
        /**
         * @brief Fait tourner le moteur dans le sens inverse pour faire descendre dans le cas d'un treuil
         * 
         * @param vitesse vitesse à laquelle faire tourner le moteur
         */
        void descendre ( unsigned int vitesse );
        /**
         * @brief Fait tourner le moteur pour faire avancer dans le cas d'une roue
         * 
         * @param vitesse vitesse à laquelle faire tourner le moteur
         */
        void avancer ( unsigned int vitesse );
        /**
         * @brief Fait tourner le moteur dans le sens inverse pour faire reculer dans le cas d'une roue
         * 
         * @param vitesse vitesse à laquelle faire tourner le moteur
         */
        void reculer ( unsigned int vitesse );
        /**
         * @brief Stop le moteur pour qu'il ne tourne plus
         * 
         */
        void stop ( );
};

#endif