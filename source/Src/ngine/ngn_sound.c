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



/*** Includes ***/
// C
#include <stdlib.h>
#include <stdio.h>
// Ngine
#include "ngn_sound.h"


/*** Stream de musica ***/
static FilePlayer* bgm;             // Reproductor de musica
static float bgm_volume;            // Volumen de la musica
static float bgm_panning;           // Panning de la musica
static void BgmApplyLevel(void);    // Aplica el volumen y el panning a la musica



/*** Inicia el subsistema de sonido ***/
void NGN_Sound_Init(void) {

    // Reproductor de musica
    bgm = NULL;
    bgm_volume = 1.0f;
    bgm_panning = 0.0f;

}



/*** Abre un stream de musica en el reproductor (mp3) ***/
bool NGN_Sound_BgmOpen(const char* path) {


    // Si existe un stream previo, eliminalo
    NGN_Sound_BgmClose();

    // Crea el reproductor de musica
    bgm = playdate->sound->fileplayer->newPlayer();
    if (!bgm) {
        // En caso de error...
        playdate->system->logToConsole("[NGN_Sound] Error creating music player.");
        return false;
    }

    // Intenta cargar el archivo en el reproductor
    if (!playdate->sound->fileplayer->loadIntoPlayer(bgm, path)) {
        // En caso de error...
        playdate->system->logToConsole("[NGN_Sound] Error opening music stream at path: %s.", path);
        playdate->sound->fileplayer->freePlayer(bgm);
        bgm = NULL;
        return false;
    }

    // Restablece los valores volumen y panning por defecto
    bgm_volume = 1.0f;
    bgm_panning = 0.0f;

    // Devuelve el resultado
    return true;

}



/*** Cierra el stream de musica ***/
void NGN_Sound_BgmClose(void) {

    if (!bgm) return;

    playdate->sound->fileplayer->stop(bgm);
    playdate->sound->fileplayer->freePlayer(bgm);
    bgm = NULL;

}



/*** Inicia la reproduccion del stream de musica ***/
void NGN_Sound_BgmPlay(uint32_t loops) {

    if (!bgm) return;

    playdate->sound->fileplayer->play(bgm, loops);

}



/*** Deten el stream de musica ***/
void NGN_Sound_BgmStop(void) {

    if (!bgm) return;

    playdate->sound->fileplayer->stop(bgm);

}



/*** Pausa el stream de musica ***/
void NGN_Sound_BgmPause(void) {

    if (!bgm) return;

    playdate->sound->fileplayer->pause(bgm);

}



/*** Establece el volumen de la musica ***/
void NGN_Sound_BgmSetVolume(float v) {

    bgm_volume = v;
    if (bgm_volume < 0.0f) bgm_volume = 0.0f;
    if (bgm_volume > 1.0f) bgm_volume = 1.0f;

    BgmApplyLevel();

}



/*** Devuelve el volumen de la musica ***/
float NGN_Sound_BgmGetVolume(void) {

    return bgm_volume;

}



/*** Establece el panning de la musica ***/
void NGN_Sound_BgmSetPanning(float p) {

    bgm_panning = p;
    if (bgm_panning < -1.0f) bgm_panning = -1.0f;
    if (bgm_panning > 1.0f) bgm_panning = 1.0f;

    BgmApplyLevel();

}



/*** Devuelve el panning de la musica ***/
float NGN_Sound_BgmGetPanning(void) {

    return bgm_panning;

}



/*** Aplica el volumen y el panning a la musica ***/
void BgmApplyLevel(void) {

    if (!bgm) return;

    float right = fabsf(1.0f + bgm_panning);
    if (right > 1.0f) right = 1.0f;

    float left = fabsf(1.0f - bgm_panning);
    if (left > 1.0f) left = 1.0f;

    float r_level = (bgm_volume * right);
    float l_level = (bgm_volume * left);

    playdate->sound->fileplayer->setVolume(bgm, l_level, r_level);

}



/*** Elimina el reproductor de samples ***/
void NGN_Sound_FreeSfx(SamplePlayer* sample_player) {

    if (!sample_player) return;

    playdate->sound->sampleplayer->stop(sample_player);
    playdate->sound->sampleplayer->freePlayer(sample_player);
    sample_player = NULL;

}



/*** Elimina el sample cargado ***/
void NGN_Sound_FreeSample(AudioSample* sample) {

    if (!sample) return;

    playdate->sound->sample->freeSample(sample);
    sample = NULL;

}
