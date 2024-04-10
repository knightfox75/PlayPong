/******************************************************************************

    [Pong for Playdate]: Archivo MAIN

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



/*** Includes ***/
// C
#include <stdio.h>
#include <stdlib.h>
// SDK
#include "pd_api.h"
// Libreria Ngine
#include "ngine/ngine.h"
// Proyecto
#include "game/game.h"



/*** Windows OS DLL files ***/
#ifdef _WINDLL
__declspec(dllexport)
#endif



/*** Main event handler ***/
int eventHandler(PlaydateAPI* pd_api, PDSystemEvent event, uint32_t arg) {

	// Nucleo de ejecucion
	NGN_System_CallbackFunction(pd_api, event, arg);

	// Establece la funcion de actualizacion del programa a ejecutar (solo una vez, al iniciar)
    if (event == kEventInit) NGN_System_SetUpdateFunction(Game);

	// Return 0
	return 0x00;

}
