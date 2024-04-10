/******************************************************************************

    [Pong for Playdate]: Nucleo de ejecucion del juego
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
#include "game.h"
#include "defines.h"
#include "welcome.h"
#include "menu.h"
#include "pong.h"



/*** Funciones con llamadas internas ***/



/*** Maquina de estados ***/
enum {
    STATE_IDLE,
    STATE_BOOT_UP,
    STATE_WELCOME,
    STATE_MENU,
    STATE_PONG
};
static uint8_t state = STATE_BOOT_UP;
static uint8_t next_state = STATE_BOOT_UP;

/*** Control del menu ***/
static int8_t player = 0, points = 0;

/*** Declaracion de funciones ***/
// Procesos al iniciar
static void StBootUp(void);
// Pantallas de presentacion
static void StWelcome(void);
// Menu del juego
static void StMenu(void);
// Ejecucion del juego
static void StPong(void);



/*** Nucleo de ejecucion ***/
void Game(void) {

    // Maquina de estados
    switch (state) {

        case STATE_IDLE:
            break;

        case STATE_BOOT_UP:
            StBootUp();
            break;

        case STATE_WELCOME:
            StWelcome();
            break;

        case STATE_MENU:
            StMenu();
            break;

        case STATE_PONG:
            StPong();
            break;

    }

    state = next_state;

}



/*** Procesos al iniciar ***/
void StBootUp(void) {

    // Siguiente modulo del programa
    next_state = STATE_WELCOME;

}



/*** Pantallas de presentacion ***/
void StWelcome(void) {

    // Ejecuta el modulo
    int8_t r = Welcome();
    if (r < 0) return;

    // Siguiente modulo del programa
    next_state = STATE_MENU;

}



/*** Menu del juego ***/
void StMenu(void) {

    // Ejecuta el menu
    uint32_t r = Menu();
    if (r == 0xFFFFFFFF) return;

    // Segun la opcion del menu, numero de jugadores
    switch (r & 0xFF) {
        case 0:     // 1 Jugador (izquierdo, D-PAD)
            player = PLAYER_ID_LEFT;
            break;
        case 1:     // 1 Jugador (derecho, Manivela)
            player = PLAYER_ID_RIGHT;
            break;
        case 2:     // 2 Jugadores (versus)
            player = PLAYER_ID_NEUTRAL;
            break;
        default:
            break;
    }

    // Segun la opcion del menu, numero de puntos
    switch ((r >> 8) & 0xFF) {
        case 0:
            points = 3;
            break;
        case 1:
            points = 7;
            break;
        case 2:
            points = 15;
            break;
        default:
            break;
    }

    // Siguiente modulo del programa
    next_state = STATE_PONG;

}



/*** Ejecucion del juego ***/
void StPong(void) {

    // Ejecuta el juego
    int8_t r = Pong(player, points);
    if (r < 0) return;

    // Siguiente modulo del programa
    next_state = STATE_MENU;

}
