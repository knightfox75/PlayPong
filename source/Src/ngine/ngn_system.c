/******************************************************************************

    [N'gine for Playdate SDK]: Nucleo de ejecucion
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


	N'gine Lib for Playdate is under MIT License

	Copyright (c) 2024 by Cesar Rincon "NightFox"

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal	in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER	LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************************/



/*** Includes ***/
// C
#include <stdlib.h>
#include <stdio.h>
// Ngine
#include "ngn_system.h"
#include "ngn_settings.h"
#include "ngn_input.h"
#include "ngn_sound.h"
// Proyecto a ejecutar
#include "../game/game.h"



/*** Define la funcion de actualizacion del programa***/
static NGN_System_UpdFnc ngn_update_function = NULL;



/*** Variables del nucleo de ejecucion del programa ***/
// Variables de control del sistema
static bool callback_enabled;
static bool events_enabled;



/*** Declaracion de las funciones del nucleo (llamadas internas) ***/
// Configuracion inicial del programa
static void Setup(void);
// Funcion de actualizacion del callback
static int Update(void* upd);



/*** Nucleo de ejecucion del programa ***/
void NGN_System_CallbackFunction(PlaydateAPI* pd_api, PDSystemEvent event, uint32_t arg) {

    // Prioridad del evento de inicializacion
    if (event == kEventInit) {
        // Establece el puntero a la API
        playdate = pd_api;
        // Configura la frecuencia del refresco
        pd_api->display->setRefreshRate(GAME_FRAME_RATE);
        // Establece la funcion del callback
        pd_api->system->setUpdateCallback(Update, NULL);
        // Configuracion inicial
        Setup();
        // Sal de la funcion
        return;
    } else if (kEventTerminate) {       // Evento de finalizacion de la ejecucion
        return;
    }

    // Si el gestor de eventos no esta habilitado, sal de la funcion
    if (!events_enabled) return;

    // Gestion de eventos
    switch (event) {

        default:
            break;

    }

}



/*** Establece la funcion de actualizacion para el proyecto ***/
void NGN_System_SetUpdateFunction(NGN_System_UpdFnc f) {

    ngn_update_function = f;

}



/*** Calcula el tiempo delta del frame, si es inferior al deseado, espera ***/
void NGN_System_FrameSync() {

    // Debug FPS
    #if defined (ENABLE_DEBUG_FRAME_COUNTER)
        playdate->system->drawFPS(8, 8);
    #endif

    // Sync del frame
    while (playdate->system->getElapsedTime() < FRAME_DELTA_TIME);
    playdate->system->resetElapsedTime();

}



/*** Configuracion inicial del programa ***/
void Setup(void) {

    // Semilla para el random
    srand(playdate->system->getSecondsSinceEpoch(NULL));

    // Control de ejecucion
    NGN_System_EnableCallback();
    NGN_System_EnableEvents();

    // Metodos de entrada
    NGN_Input_Reset();

    // Sistema de sonido
    NGN_Sound_Init();

    // Temporizador de alta precision
    playdate->system->resetElapsedTime();

}



/*** Funcion de actualizacion del callback ***/
int Update(void* upd) {

    // Si el callback no esta habilitado, sal de la funcion (sin indicar dibujado 0x00)
    if (!callback_enabled) return 0x00;

    // Antes de nada, actualiza el estado de las entradas
    NGN_Input_Update();

    /*** Funcion de actualizacion del programa ***/
    if (ngn_update_function) ngn_update_function();
    /*** --- ***/

    // Devuelve el resultado (indicando dibujado 0x01)
	return 0x01;

}



/*** Habilita el callback ***/
void NGN_System_EnableCallback(void) {
    callback_enabled = true;
}
/*** Deshabilita el callback ***/
void NGN_System_DisableCallback(void) {
    callback_enabled = false;
}
/*** Devuelve el estado actual del callback ***/
bool NGN_System_GetCallbackState(void) {
    return callback_enabled;
}



/*** Habilita los eventos ***/
void NGN_System_EnableEvents(void) {
    events_enabled = true;
}
/*** Deshabilita el callback ***/
void NGN_System_DisableEvents(void) {
    events_enabled = false;
}
/*** Devuelve el estado actual del callback ***/
bool NGN_System_GetEventsState(void) {
    return events_enabled;
}

