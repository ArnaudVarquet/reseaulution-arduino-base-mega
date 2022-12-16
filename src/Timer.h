#ifndef TIMER_H
#define TIMER_H

class Timer {
    private:
        /* timestamp du démarrage du chrono */
        unsigned long startTime = 0;
        /* Durée au delà duquel le chrono expire */
        unsigned long delay     = 0;
    
    public:
        /**
         * @brief Construct a new Timer object
         * 
         * @param aDelay Durée au delà duquel le chrono expire 
         */
        Timer ( unsigned long aDelay );
        /**
         * @brief Relance le chrono
         */
        void restart ( );
        /**
         * @brief Indique si le chrono est expiré ou non
         * 
         * @return true si le chrono est expiré
         * @return false sinon
         */
        bool isExpired ( );
};

#endif