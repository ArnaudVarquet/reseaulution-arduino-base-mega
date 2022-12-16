#ifndef MOUVEMENT_H
#define MOUVEMENT_H

/* Vitesse de la nacelle : lente */
#define VSPEED_LOW                        127
/* Vitesse de la nacelle : moyenne */
#define VSPEED_MEDIUM                     191
/* Vitesse de la nacelle : rapide */
#define VSPEED_HIGH                       255

/* Vitesse de la station : lente */
#define HSPEED_LOW                        80
/* Vitesse de la station : moyenne */
#define HSPEED_MEDIUM                     167
/* Vitesse de la station : rapide */
#define HSPEED_HIGH                       255


typedef enum {
    FORWARD,
    BACKWARD,
    hNONE
} HorizontalDirection;

struct HorizontalInstruction {
    HorizontalDirection direction;
    unsigned int actualSpeed;
    unsigned int targetSpeed;
};

typedef enum {
    UP,
    DOWN,
    vNONE
} VerticalDirection;

struct VerticalInstruction {
    VerticalDirection direction;
    unsigned int actualSpeed;
    unsigned int targetSpeed;
};

#endif