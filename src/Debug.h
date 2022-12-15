#include <Arduino.h>
#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG

/**
 * @brief Affiche un message sur le Serial
 * 
 * @param message message à afficher
 */
void debug (String message);

/**
 * @brief Affiche un int sur le Serial
 * 
 * @param value le int à afficher
 * @param base baes sur laquelle afficher le int (DEC, HEX...)
 */
void debug (int value, int base = DEC);

/**
 * @brief Affiche un message sur le Serial en ajoutant un retour à la ligne (ln) à la fin
 * 
 * @param message message à afficher
 */
void debugln (String message);

/**
 * @brief Affiche un int sur le Serial en ajoutant un retour à la ligne (ln) à la fin
 * 
 * @param value le int à afficher
 * @param base baes sur laquelle afficher le int (DEC, HEX...)
 */
void debugln (int value, int base = DEC);


#endif