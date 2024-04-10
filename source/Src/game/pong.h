/******************************************************************************

    [Pong for Playdate]: Pong
    Pong for Playdate v0.9.0-beta

    Proyecto iniciado el 22 de Febrero del 2024
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



#ifndef PONG_H_INCLUDED
#define PONG_H_INCLUDED



/*** Includes ***/
// C
#include <stdio.h>
#include <stdlib.h>
// SDK
#include "pd_api.h"
// Ngine
#include "../ngine/ngine.h"



/*** Funciones con llamadas externas ****/

// Nucleo de ejecucion del juego
int8_t Pong(int8_t player, int8_t points);



#endif // PONG_H_INCLUDED
