/******************************************************************************

    [N'gine for Playdate SDK]: Colisiones
    N'gine for Playdate v0.1.0-alpha

    Proyecto iniciado el 20 de Febrero del 2024
    (c) 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere Playdate SDK
	(c) Panic
	https://play.date/dev/

    Requiere GCC ARM EABI compiler
    sudo apt install binutils-arm-none-eabi
    sudo apt install gcc-arm-none-eabi

******************************************************************************/



/*** Includes ***/
// C
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// Ngine
#include "ngn_collisions.h"



/*** Colision por cajas entre 2 sprites ***/
bool NGN_Collisions_SpriteBox(LCDSprite* spr_a, LCDSprite* spr_b) {

    // Proteccion contra NULL
    if (!spr_a || !spr_b) return false;

    // Limites del sprite
    PDRect bounds;

    // Variables para el primer sprite (a)
    bounds = playdate->sprite->getBounds(spr_a);
    float w1 = bounds.width;
    float h1 = bounds.height;
    float x1 = (bounds.x + (w1 / 2.0f));
    float y1 = (bounds.y + (h1 / 2.0f));

    // Variables para el segundo sprite (b)
    bounds = playdate->sprite->getBounds(spr_b);
    float w2 = bounds.width;
    float h2 = bounds.height;
    float x2 = (bounds.x + (w2 / 2.0f));
    float y2 = (bounds.y + (h2 / 2.0f));


    // Calculos previos (distancia entre sprites)
    Vector2 distance;
    distance.x = fabs(x1 - x2);
    distance.y = fabs(y1 - y2);
    // Calculos previos (tamaño de la colision)
    Size2 collision_size;
    collision_size.width = (w1 / 2.0f) + (w2 / 2.0f);
    collision_size.height = (h1 / 2.0f) + (h2 / 2.0f);

    // Verifica si existe la colision
    if ((distance.x < collision_size.width) && (distance.y < collision_size.height)) {
        return true;
    } else {
        return false;
    }

}
