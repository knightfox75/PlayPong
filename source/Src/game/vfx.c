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




/*** Includes ***/
// C
#include <stdio.h>
#include <stdlib.h>
// SDK
#include "pd_api.h"
// Ngine
#include "../ngine/ngine.h"
// Proyecto
#include "vfx.h"



/*** Constantes de configuracion ***/
static const int32_t FADE_BLOCK_SIZE = 20;
static const int32_t FADE_SPEED = 2;



/*** Variables de control ***/
static int32_t fade_level;



/*** Prepara el efecto fade-in ***/
void Vfx_FadeInSetup(void) {

    fade_level = FADE_BLOCK_SIZE;
    Vfx_Cls();

}



/*** Ejecuta el efecto fade-in ***/
bool Vfx_FadeInRun(void) {

    bool active = true;

    fade_level -= FADE_SPEED;
    if (fade_level <= 0) {
        fade_level = 0;
        active = false;
    }

    float v = (1.0f - ((float)fade_level / (float)FADE_BLOCK_SIZE));
    NGN_Sound_BgmSetVolume(v);

    return active;

}



/*** Prepara el efecto fade-out ***/
void Vfx_FadeOutSetup(void) {

    fade_level = 0;

}



/*** Ejecuta el efecto fade-out ***/
bool Vfx_FadeOutRun(void) {

    bool active = true;

    fade_level += FADE_SPEED;
    if (fade_level >= FADE_BLOCK_SIZE) {
        fade_level = FADE_BLOCK_SIZE;
        active = false;
    }

    float v = (1.0f - ((float)fade_level / (float)FADE_BLOCK_SIZE));
    NGN_Sound_BgmSetVolume(v);

    return active;

}



/*** Render del efecto fade ***/
void Vfx_FadeRender(void) {

    // Limite
    if (fade_level <= 0) return;

//    // Bucle de las columnas
//    for (int32_t y = (FADE_BLOCK_SIZE / 2); y < SCR_HEIGHT; y += FADE_BLOCK_SIZE) {
//        playdate->graphics->drawLine(0, y, SCR_WIDTH, y, fade_level, kColorBlack);
//    }
//    // Bucle de las filas
//    for (int32_t x = (FADE_BLOCK_SIZE / 2); x < SCR_WIDTH; x += FADE_BLOCK_SIZE) {
//        playdate->graphics->drawLine(x, 0, x, SCR_HEIGHT, fade_level, kColorBlack);
//    }

    int32_t px = 0, py = 0;
    int32_t offset = (FADE_BLOCK_SIZE / 2);
    for (int32_t y = 0; y < SCR_HEIGHT; y += FADE_BLOCK_SIZE) {
        py = (y + offset - (fade_level / 2));
        for (int32_t x = 0; x < SCR_WIDTH; x += FADE_BLOCK_SIZE) {
            px = (x + offset - (fade_level / 2));
            playdate->graphics->fillRect(px, py, fade_level, fade_level, kColorBlack);
        }
    }

}



// Limpia la pantalla (pintala de negro)
void Vfx_Cls(void) {

    playdate->graphics->fillRect(0, 0, SCR_WIDTH, SCR_HEIGHT, kColorBlack);

}
