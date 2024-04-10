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



#ifndef NGN_SYSTEM_H_INCLUDED
#define NGN_SYSTEM_H_INCLUDED



/*** Includes ***/
// C
#include <stdio.h>
#include <stdlib.h>
// SDK
#include "pd_api.h"
// Ngine
#include "ngn_defines.h"
#include "ngn_settings.h"



/*** Prototipos de datos ***/
// Funcion de actualizacion del programa
typedef void (*NGN_System_UpdFnc)(void);



/*** Declaracion de las funciones del nucleo (llamadas externas) ***/

// Nucleo de ejecucion del programa
void NGN_System_CallbackFunction(PlaydateAPI* pd_api, PDSystemEvent event, uint32_t arg);

// Establece la funcion de actualizacion para el proyecto
void NGN_System_SetUpdateFunction(NGN_System_UpdFnc f);

// Gestion del callback
void NGN_System_EnableCallback(void);
void NGN_System_DisableCallback(void);
bool NGN_System_GetCallbackState(void);

// Gestion de los eventos
void NGN_System_EnableEvents(void);
void NGN_System_DisableEvents(void);
bool NGN_System_GetEventsState(void);

// Calcula el tiempo delta del frame, si es inferior al deseado, espera
void NGN_System_FrameSync(void);



#endif // NGN_SYSTEM_H_INCLUDED
