#ifndef CANAL_H
#define CANAL_H
#include <Arduino.h>

class Canal {
    protected:
        /**
         * @brief Permet de se mettre en écoute sur le port série et d'attendre une réponse spécifique
         * 
         * @param serialPort port série sur lequel écouter
         * @param strwait chaine que l'on attent
         * @param strinfo chaine à afficher pour information
         * @param timeout timeout au bout duquel on arrête d'attendre
         * @param attente delais entre chaque recherche sur le port série
         * @return true si la chaine strwait a été trouvé
         * @return false sinon
         */
        static bool attente_reponse (Stream &serialPort, char strwait[], char strinfo[], unsigned int timeout, unsigned int attente);
        /**
         * @brief Flush le port série
         * 
         * @param serialPort port série à flusher
         */
        static void flush_serial(Stream &serialPort);
        /**
         * @brief Convertie les int en hexa
         * 
         * @param _input int à convertir
         * @param _nbChar nombre de caractère sur lequel convertir
         * @return String chaîne de caractère représentant l'int sous forme d'hexa
         */
        static String intToHex( int _input, int _nbChar );
        /**
         * @brief Fonction permettant de lire un port série
         * 
         * @param serial port série à lire
         * @param buffer buffer dans lequel est stocké ce qui est lue
         * @param delimiteur délimiteur de fin de commande
         * @param timeout temps au bout duquel on arrête de lire
         * @return true  si un délimiteur a été trouvé
         * @return false sinon
         */
        static bool readSerial ( Stream &serial, String &buffer, char delimiteur, unsigned long timeout = 1000 );
        /* buffer dans lequel stocker les message lus sur le canal */
        String buffer = "";
    public:
        /**
         * @brief Construct a new Canal object
         */
        Canal ();
};

#endif