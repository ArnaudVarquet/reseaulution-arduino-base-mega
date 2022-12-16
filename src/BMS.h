#ifndef BMS_H
#define BMS_H

/**
 * @brief Structure représentant les informations des batteries (nacelle et station)
 * 
 */
struct BMSInfo {
    int pCentBatt;
    int tte;
    int ttf;
    int vBatt;
    int temperaturePack;
    int current;
    bool charge;
};

#endif