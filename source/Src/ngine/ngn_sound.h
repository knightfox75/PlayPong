/******************************************************************************

    [N'gine for Playdate SDK]: Gestion del sistema de sonido
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



#ifndef NGN_SOUND_H_INCLUDED
#define NGN_SOUND_H_INCLUDED



/*** Includes ***/
// C
#include <stdio.h>
#include <stdlib.h>
// SDK
#include "pd_api.h"
// Ngine
#include "ngn_defines.h"
#include "ngn_settings.h"



/*** Declaracion de las variables globales (uso externo) ***/





/*** Declaracion de las funciones comunes ***/

// Inicia el subsistema de sonido
void NGN_Sound_Init(void);



/*** Declaracion de las funciones de la musica de fondo (BGM) ***/

// Abre un stream de musica en el reproductor (mp3)
bool NGN_Sound_BgmOpen(const char* path);

// Cierra el stream de musica
void NGN_Sound_BgmClose(void);

// Inicia la reproduccion del stream de musica
void NGN_Sound_BgmPlay(uint32_t loops);

// Deten el stream de musica
void NGN_Sound_BgmStop(void);

// Pausa el stream de musica
void NGN_Sound_BgmPause(void);

// Establece el volumen de la musica
void NGN_Sound_BgmSetVolume(float v);

// Devuelve el volumen de la musica
float NGN_Sound_BgmGetVolume(void);

// Establece el panning de la musica
void NGN_Sound_BgmSetPanning(float p);

// Devuelve el panning de la musica
float NGN_Sound_BgmGetPanning(void);



/*** Declaracion de las funciones de efectos de sonido (SFX) ***/

// Elimina el reproductor de samples
void NGN_Sound_FreeSfx(SamplePlayer* sample_player);

// Elimina el sample cargado
void NGN_Sound_FreeSample(AudioSample* sample);




#endif // NGN_SOUND_H_INCLUDED
