/******************************************************************************

    [Pong for Playdate]: Menu del juego
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
#include "menu.h"
#include "vfx.h"



/*** Funciones con llamadas internas ***/
// Procesos iniciales del menu
static bool Start(void);
// Funcion de loop del menu (actualizacion)
static void Update(void);
// Funcion inicio previa
static void Init(void);
// Funcion de carga de archivos
static bool Load(void);
// Funcion de creacion de la escena
static void Create(void);
// Funcion con la logica del menu
static void Logic(void);
// Funcion de render de los graficos
static void Render(void);
// Limpieza de los datos antes de salir
static void CleanUp(void);



/*** Maquina de estados ***/
enum {
    STATE_IDLE,
    STATE_START,
    STATE_UPDATE,
    STATE_GOTO_GAME,
    STATE_FADE_IN,
    STATE_FADE_OUT,
    STATE_EXIT
};
static uint8_t state = STATE_START;
static uint8_t next_state = STATE_START;
static void StGotoGame(void);
static void StFadeIn(void);
static void StFadeOut(void);



/*** Graficos ***/
// Imagenes de los sprites
static LCDBitmap* background_img;
static LCDBitmap* players_img;
static LCDBitmap* points_img;
static LCDBitmap* credits_img;
static LCDBitmap* cursor_img;
// Sprites
static LCDSprite* background_spr;
static LCDSprite* menu_spr;
static LCDSprite* cursor_spr;


/*** Datos del menu ***/
static const float CURSOR_X = 56.0f;
static const float CURSOR_Y = 120.0f;
static const float CURSOR_GAP = 25.0f;
static const int32_t OPTION_FIRST = 0;
static const int32_t OPTION_LAST = 2;
static const int32_t MENU_TIME_OUT = 300;       // 10 segundos
static const int32_t CREDITS_TIME_OUT = 300;    // 10 segundos
static int8_t option = 0, _option = 0;
static uint8_t players = 0, points = 0;
static int32_t time_out = 0;


/*** Efectos de sonido ***/
static AudioSample* menu_select_wav;
static AudioSample* menu_ok_wav;
static AudioSample* title_wav;
static SamplePlayer* menu_select_sfx;
static SamplePlayer* menu_ok_sfx;
static SamplePlayer* title_sfx;



/*** Maquina de estados del menu ***/
enum {
    MENU_SELECT_PLAYER,
    MENU_SELECT_POINTS,
    MENU_CREDITS
};
static uint8_t menu_state = MENU_SELECT_PLAYER;
static uint8_t menu_next_state = MENU_SELECT_PLAYER;
static void MenuSelectPlayers(void);
static void MenuSelectPoints(void);
static void MenuCredits(void);
static void MenuSelector(void);



/*** Nucleo de ejecucion del menu ***/
uint32_t Menu(void) {

    // Deshabilita el callback
    NGN_System_DisableCallback();

    // Resultado
    uint32_t r = 0xFFFFFFFF;

    // Maquina de estados
    switch (state) {

        case STATE_IDLE:
            break;

        case STATE_START:
            Start();
            break;

        case STATE_UPDATE:
            Update();
            break;

        case STATE_GOTO_GAME:
            StGotoGame();
            break;

        case STATE_FADE_IN:
            StFadeIn();
            break;

        case STATE_FADE_OUT:
            StFadeOut();
            break;

        case STATE_EXIT:
            // Limpieza
            CleanUp();
            // Devuelve la opcion seleccionada
            r = ((points << 8) | players);
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

    // Efecto fade-in
    Vfx_Cls();
    Vfx_FadeInSetup();

    // Cambia de estado
    next_state = STATE_FADE_IN;

    // Proceso correcto
    return true;

}



/*** Funcion de loop del juego (actualizacion) ***/
void Update(void) {

    // Logica del juego
    Logic();

    // Render
    Render();

}



// Funcion inicio previa
void Init(void) {

    // Imagenes de los sprites
    background_img = NULL;
    players_img = NULL;
    points_img = NULL;
    credits_img = NULL;
    cursor_img = NULL;

    // Sprites
    background_spr = NULL;
    menu_spr = NULL;
    cursor_spr = NULL;

    // Samples de audio para los SFX
    menu_select_wav = NULL;
    menu_ok_wav = NULL;
    title_wav = NULL;
    // Reproductores para los SFX
    menu_select_sfx = NULL;
    menu_ok_sfx = NULL;
    title_sfx = NULL;

    // Datos del menu
    _option = option = OPTION_FIRST;
    players = OPTION_FIRST;
    points = OPTION_FIRST;
    menu_state = menu_next_state = MENU_SELECT_PLAYER;
    time_out = 0;

}



/*** Funcion de carga de archivos ***/
bool Load(void) {

    // Carga correcta?
    bool successful = true;

    // Fondo
    background_img = NGN_Load_Bitmap("data/images/title_bg_400x240.png");
    if (!background_img) successful = false;

    // Seleccion de jugadores
    players_img = NGN_Load_Bitmap("data/images/title_players_400x240.png");
    if (!players_img) successful = false;
    // Seleccion de puntos
    points_img = NGN_Load_Bitmap("data/images/title_points_400x240.png");
    if (!points_img) successful = false;
    // Creditos
    credits_img = NGN_Load_Bitmap("data/images/title_credits_400x240.png");
    if (!credits_img) successful = false;

    // Cursor
    cursor_img = NGN_Load_Bitmap("data/sprites/cursor_16x16.png");
    if (!cursor_img) successful = false;

    // Carga los efectos de sonido
    menu_select_wav = playdate->sound->sample->load("data/sfx/menu_select_sfx.wav");
    if (!menu_select_wav) successful = false;
    menu_ok_wav = playdate->sound->sample->load("data/sfx/menu_ok_sfx.wav");
    if (!menu_ok_wav) successful = false;
    title_wav = playdate->sound->sample->load("data/sfx/play_pong_sfx.wav");
    if (!title_wav) successful = false;

    // Carga la musica de fondo
    if (!NGN_Sound_BgmOpen("data/music/menu.mp3")) successful = false;

    // Carga correcta
    return successful;

}



/*** Funcion de creacion de la escena ***/
void Create(void) {

    // Crea el fondo
    background_spr = NGN_Sprite_Create(background_img, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

    // Opciones del menu a mostrar
    menu_spr = NGN_Sprite_Create(players_img, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

    // Crea el cursor
    cursor_spr = NGN_Sprite_Create(cursor_img, CURSOR_X, CURSOR_Y);

    // Asigna los samples de los sfx
    menu_select_sfx = playdate->sound->sampleplayer->newPlayer();
    playdate->sound->sampleplayer->setSample(menu_select_sfx, menu_select_wav);
    menu_ok_sfx = playdate->sound->sampleplayer->newPlayer();
    playdate->sound->sampleplayer->setSample(menu_ok_sfx, menu_ok_wav);
    title_sfx = playdate->sound->sampleplayer->newPlayer();
    playdate->sound->sampleplayer->setSample(title_sfx, title_wav);

    // Inicia la musica
    NGN_Sound_BgmPlay(0);
    NGN_Sound_BgmSetVolume(0.0f);

}



/*** Funcion con la logica de la pantalla de menu ***/
void Logic(void) {

    // Maquina de estados del menu
    switch (menu_state) {

        case MENU_SELECT_PLAYER:
            MenuSelectPlayers();
            break;

        case MENU_SELECT_POINTS:
            MenuSelectPoints();
            break;

        case MENU_CREDITS:
            MenuCredits();
            break;

    }

    menu_state = menu_next_state;

}



/*** Funcion de render de los graficos ***/
void Render(void) {

    // Sprites
    playdate->sprite->drawSprites();

}



/*** Limpieza de los datos antes de salir ***/
void CleanUp(void) {

    // Elimina el reproductor de musica
    NGN_Sound_BgmClose();

    // Efectos de sonido
    NGN_Sound_FreeSfx(menu_select_sfx);
    NGN_Sound_FreeSfx(menu_ok_sfx);
    NGN_Sound_FreeSfx(title_sfx);

    // Archivos de samples
    NGN_Sound_FreeSample(menu_select_wav);
    NGN_Sound_FreeSample(menu_ok_wav);
    NGN_Sound_FreeSample(title_wav);

    // Elimina los sprites creados
    NGN_Sprite_Delete(background_spr);
    NGN_Sprite_Delete(menu_spr);
    NGN_Sprite_Delete(cursor_spr);

    // Vacia el displaylist
    playdate->sprite->removeAllSprites();

    // Elimina las imagenes
    NGN_Unload_Bitmap(background_img);
    NGN_Unload_Bitmap(players_img);
    NGN_Unload_Bitmap(points_img);
    NGN_Unload_Bitmap(credits_img);
    NGN_Unload_Bitmap(cursor_img);

    // Configura la maquina de estados para la siguiente ejecucion
    state = next_state = STATE_START;

}



/*** Espera a que termine el efecto de sonido y cambia de estado apra iniciar el juego ***/
void StGotoGame(void) {

    // Mientras este sonando el efecto de sonido...
    if (playdate->sound->sampleplayer->isPlaying(menu_ok_sfx)) return;

    // Prepara el fade out
    Vfx_FadeOutSetup();

    // Siguiente estado
    next_state = STATE_FADE_OUT;

}



/*** Realiza el efecto fade-in ***/
void StFadeIn(void) {

    // Fade in
    bool r = Vfx_FadeInRun();
    Render();
    Vfx_FadeRender();

    // Si continua activo, sal
    if (r) return;

    // Voz
    playdate->sound->sampleplayer->play(title_sfx, 1, 1.0f);

    // Siguiente estado
    next_state = STATE_UPDATE;

}



/*** Realiza el efecto fade-out ***/
void StFadeOut(void) {

    // Fade out
    bool r = Vfx_FadeOutRun();
    Vfx_FadeRender();

    // Si continua activo, sal
    if (r) return;

    // Borra la pantalla
    Vfx_Cls();

    // Siguiente estado
    next_state = STATE_EXIT;

}



/*** Selecciona el numero de jugadores ***/
void MenuSelectPlayers(void) {

    // Selector del menu
    MenuSelector();

    // Si no se ha movido el selector
    if (!ngn_input.any_key.held) {
        // Si se supera el timeout, muestra los creditos
        time_out ++;
        if (time_out >= MENU_TIME_OUT) {
            playdate->sprite->setImage(menu_spr, credits_img, kBitmapUnflipped);
            playdate->sprite->setVisible(cursor_spr, false);
            time_out = 0;
            menu_next_state = MENU_CREDITS;
        }
    } else {
        time_out = 0;
    }

    // Mientras no se pulse A [Aceptar]
    if (!ngn_input.button.a.press) return;

    // Opcion aceptada
    players = option;

    // Prepara el menu para la siguiente pantalla
    option = _option = points;
    playdate->sprite->moveTo(cursor_spr, CURSOR_X, (CURSOR_Y + (CURSOR_GAP * option)));
    playdate->sprite->setImage(menu_spr, points_img, kBitmapUnflipped);

    // Efecto de sonido
    playdate->sound->sampleplayer->play(menu_ok_sfx, 1, 1.0f);

    // Siguiente estado
    menu_next_state = MENU_SELECT_POINTS;

}



/*** Selecciona el numero de puntos ***/
void MenuSelectPoints(void) {

    // Selector del menu
    MenuSelector();

    // Si se pulsa B, vuelve al menu anterior
    if (ngn_input.button.b.press) {
        // Prepara el menu para la siguiente pantalla
        option = _option = players;
        playdate->sprite->moveTo(cursor_spr, CURSOR_X, (CURSOR_Y + (CURSOR_GAP * option)));
        playdate->sprite->setImage(menu_spr, players_img, kBitmapUnflipped);
        // Siguiente estado
        menu_next_state = MENU_SELECT_PLAYER;
        // Sal
        return;
    }

    // Mientras no se pulse A [Aceptar]
    if (!ngn_input.button.a.press) return;

    // Opcion aceptada
    points = option;

    // Efecto de sonido
    playdate->sound->sampleplayer->play(menu_ok_sfx, 1, 1.0f);

    // Indica la que ha de ejecutarse el juego
    next_state = STATE_GOTO_GAME;

}



/*** Muestra los creditos ***/
void MenuCredits(void) {

    // Control del tiempo que se muestra
    time_out ++;

    // Si se pulsa cualquier tecla, indica que ha de volver al menu
    if (ngn_input.any_key.press) time_out = CREDITS_TIME_OUT;

    // Si no se ha llegado a la marca de tiempo, sal
    if (time_out < CREDITS_TIME_OUT) return;

    // Vuelve al menu
    time_out = 0;
    playdate->sprite->setImage(menu_spr, players_img, kBitmapUnflipped);
    playdate->sprite->setVisible(cursor_spr, true);
    menu_next_state = MENU_SELECT_PLAYER;



}



/*** Menu de seleccion ***/
void MenuSelector(void) {

    // Si se pulsa arriba
    if (ngn_input.dpad.up.press) {
        option --;
        if (option < OPTION_FIRST) option = OPTION_FIRST;
    }

    // Si se pulsa abajo
    if (ngn_input.dpad.down.press) {
        option ++;
        if (option > OPTION_LAST) option = OPTION_LAST;
    }

    // Cambio de opcion
    if (option != _option) {
        playdate->sprite->moveTo(cursor_spr, CURSOR_X, (CURSOR_Y + (CURSOR_GAP * option)));
        playdate->sound->sampleplayer->play(menu_select_sfx, 1, 1.0f);
    }
    _option = option;

}