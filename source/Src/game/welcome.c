/******************************************************************************

    [Pong for Playdate]: Pantallas de bienvenida
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
#include "welcome.h"
#include "vfx.h"



/*** Funciones con llamadas internas ***/
// Procesos iniciales
static bool Start(void);
// Funcion inicio previa
static void Init(void);
// Funcion de carga de archivos
static bool Load(void);
// Funcion de creacion de la escena
static void Create(void);
// Funcion de render de los graficos
static void Render(void);
// Limpieza de los datos antes de salir
static void CleanUp(void);



/*** Maquina de estados ***/
enum {
    STATE_IDLE,
    STATE_START,
    STATE_WAIT_NGINE,
    STATE_FADEIN_NGINE,
    STATE_RUN_NGINE,
    STATE_FADEOUT_NGINE,
    STATE_WAIT_NFOX,
    STATE_FADEIN_NFOX,
    STATE_RUN_NFOX,
    STATE_FADEOUT_NFOX,
    STATE_WAIT_EXIT,
    STATE_EXIT
};
static uint8_t state = STATE_START;
static uint8_t next_state = STATE_START;



/*** Graficos ***/
// Imagenes de los sprites
static LCDBitmap* ngine_img;
static LCDBitmap* nightfoxandco_img;
// Sprites
static LCDSprite* background_spr;



/*** Efectos de sonido ***/
static AudioSample* voice_wav;
static SamplePlayer* voice_sfx;



/*** Control de escenas ***/
static const int8_t DELAY = 15;
static const int8_t SCR_TIME = 45;
static int8_t timer;




/*** Nucleo de ejecucion del menu ***/
int8_t Welcome(void) {

    // Deshabilita el callback
    NGN_System_DisableCallback();

    // Resultado
    int8_t r = -1;

    // Maquina de estados
    switch (state) {

        case STATE_IDLE:
            break;

        case STATE_START:
            Start();
            break;

        case STATE_WAIT_NGINE:
            timer ++;
            if (timer < DELAY) break;
            timer = 0;
            Vfx_FadeInSetup();
            next_state = STATE_FADEIN_NGINE;
            break;

        case STATE_FADEIN_NGINE:
            if (!Vfx_FadeInRun()) {
                playdate->sound->sampleplayer->play(voice_sfx, 1, 1.0f);
                next_state = STATE_RUN_NGINE;
            }
            Render();
            break;

        case STATE_RUN_NGINE:
            timer ++;
            if (timer < SCR_TIME) break;
            if (playdate->sound->sampleplayer->isPlaying(voice_sfx)) break;
            timer = 0;
            Vfx_FadeOutSetup();
            next_state = STATE_FADEOUT_NGINE;
            break;

        case STATE_FADEOUT_NGINE:
            if (!Vfx_FadeOutRun()) next_state = STATE_WAIT_NFOX;
            Render();
            break;

        case STATE_WAIT_NFOX:
            timer ++;
            if (timer < DELAY) break;
            timer = 0;
            playdate->sprite->setImage(background_spr, nightfoxandco_img, kBitmapUnflipped);
            Vfx_FadeInSetup();
            next_state = STATE_FADEIN_NFOX;
            break;

        case STATE_FADEIN_NFOX:
            if (!Vfx_FadeInRun()) next_state = STATE_RUN_NFOX;
            Render();
            break;

        case STATE_RUN_NFOX:
            timer ++;
            if (timer < SCR_TIME) break;
            timer = 0;
            Vfx_FadeOutSetup();
            next_state = STATE_FADEOUT_NFOX;
            break;

        case STATE_FADEOUT_NFOX:
            if (!Vfx_FadeOutRun()) next_state = STATE_WAIT_EXIT;
            Render();
            break;

        case STATE_WAIT_EXIT:
            timer ++;
            if (timer < DELAY) break;
            timer = 0;
            next_state = STATE_EXIT;
            break;

        case STATE_EXIT:
            CleanUp();
            r = 0;
            break;

    }
    state = next_state;

    // Frame Sync
    NGN_System_FrameSync();
    // Habilita el callback
    NGN_System_EnableCallback();

    // Devuelve el resultado
    return r;

}



/*** Procesos iniciales del juego ***/
bool Start(void) {

    // Procesos previos
    Init();

    // Carga de recursos
    if (!Load()) {
        // Error de carga
        playdate->system->logToConsole("ERROR: Resources loading failed!");
        next_state = STATE_IDLE;
        return false;
    }

    // Crea la escena
    Create();

    // Cambia de estado
    next_state = STATE_WAIT_NGINE;

    // Proceso correcto
    return true;

}



// Funcion inicio previa
void Init(void) {

    // Imagenes de los sprites
    ngine_img = NULL;
    nightfoxandco_img = NULL;

    // Sprites
    background_spr = NULL;

    // Samples de audio para los SFX
    voice_wav = NULL;
    // Reproductores para los SFX
    voice_sfx = NULL;

    // Variables
    timer = 0;

}



/*** Funcion de carga de archivos ***/
bool Load(void) {

    // Carga correcta?
    bool successful = true;

    // Logo N'gine
    ngine_img = NGN_Load_Bitmap("data/images/ngine_400x240.png");
    if (!ngine_img) successful = false;

    // NightFox & Co presents
    nightfoxandco_img = NGN_Load_Bitmap("data/images/nightfoxandco_400x240.png");
    if (!nightfoxandco_img) successful = false;

    // Carga los efectos de sonido
    voice_wav = playdate->sound->sample->load("data/sfx/ngine_sfx.wav");
    if (!voice_wav) successful = false;

    // Carga correcta
    return successful;

}



/*** Funcion de creacion de la escena ***/
void Create(void) {

    // Crea el fondo
    background_spr = NGN_Sprite_Create(ngine_img, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

     // Asigna los samples de los sfx
    voice_sfx = playdate->sound->sampleplayer->newPlayer();
    playdate->sound->sampleplayer->setSample(voice_sfx, voice_wav);

    // Pantalla en negro
    Vfx_Cls();

}



/*** Funcion de render de los graficos ***/
void Render(void) {

    // Sprites
    playdate->sprite->drawSprites();

    // Efecto FADE
    Vfx_FadeRender();

}



/*** Limpieza de los datos antes de salir ***/
void CleanUp(void) {

    // Efectos de sonido
    NGN_Sound_FreeSfx(voice_sfx);

    // Archivos de samples
    NGN_Sound_FreeSample(voice_wav);

    // Elimina los sprites creados
    NGN_Sprite_Delete(background_spr);

    // Vacia el displaylist
    playdate->sprite->removeAllSprites();

    // Elimina las imagenes
    NGN_Unload_Bitmap(ngine_img);
    NGN_Unload_Bitmap(nightfoxandco_img);

    // Configura la maquina de estados para la siguiente ejecucion
    state = next_state = STATE_START;

}
