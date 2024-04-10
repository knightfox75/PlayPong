/******************************************************************************

    [Pong for Playdate]: Efectos visuales
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



#ifndef VFX_H_INCLUDED
#define VFX_H_INCLUDED



/*** Includes ***/
// C
#include <stdio.h>
#include <stdlib.h>
// SDK
#include "pd_api.h"
// Ngine
#include "../ngine/ngine.h"



/*** Variables de control ***/


/*** Funciones con llamadas externas ***/

// Procesos iniciales
void Vfx_Init(void);

// Prepara el efecto fade-in
void Vfx_FadeInSetup(void);
// Ejecuta el efecto fade-in
bool Vfx_FadeInRun(void);

// Prepara el efecto fade-out
void Vfx_FadeOutSetup(void);
// Ejecuta el efecto fade-out
bool Vfx_FadeOutRun(void);

// Render del efecto fade
void Vfx_FadeRender(void);

// Limpia la pantalla (pintala de negro)
void Vfx_Cls(void);



#endif // VFX_H_INCLUDED
