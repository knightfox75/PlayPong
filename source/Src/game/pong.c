/******************************************************************************

    [Pong for Playdate]: Pong
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
#include <math.h>
// SDK
#include "pd_api.h"
// Ngine
#include "../ngine/ngine.h"
// Proyecto
#include "pong.h"
#include "defines.h"
#include "vfx.h"


/*** Definiciones del programa ***/
#define SCORE_NUMBERS 10



/*** Constantes de configuracion del programa ***/
static const float VERTICAL_OFFSET = 12.0f;
static const float HORIZONTAL_OFFSET = 24.0f;
static const float PLAYER_SPEED = 10.0f;
static const float PLAYER_SNAP = 0.25f;
static const float PLAYER_SMOOTH = 4.0f;
static const float CRANK_SMOOTH = 4.0f;
static const float BALL_MIN_SPEED = 5.0f;
static const float BALL_MAX_SPEED = 15.0f;
static const float BALL_ACCELERATION = 1.20f;
static const float BALL_MIN_ANGLE = (5.0f * DEG2RAD);
static const float BALL_MAX_ANGLE = (60.0f * DEG2RAD);
static const float BALL_SERVICE_OFFSET = 24.0f;




/*** Funciones con llamadas internas ***/
// Procesos iniciales del juego
static bool Start(int8_t player, int8_t points);
// Funcion de loop del juego (actualizacion)
static void Update(void);
// Funcion inicio previa
static void Init(void);
// Funcion de carga de archivos
static bool Load(void);
// Funcion de creacion de la escena
static void Create(void);
// Funcion con la logica del juego
static void Logic(void);
// Funcion de render de los graficos
static void Render(void);
// Datos iniciales de la partida
static void Reset(void);
// Limpieza de los datos antes de salir
static void CleanUp(void);



/*** Maquina de estados ***/
enum {
    STATE_IDLE,
    STATE_START,
    STATE_UPDATE,
    STATE_FADE_IN,
    STATE_FADE_OUT,
    STATE_EXIT
};
static uint8_t state = STATE_START;
static uint8_t next_state = STATE_START;


/*** Graficos ***/
// Imagenes de los sprites
static LCDBitmap* background_img;
static LCDBitmap* paddle_img;
static LCDBitmap* numbers_img[SCORE_NUMBERS];
static LCDBitmap* left_wins_img;
static LCDBitmap* right_wins_img;
// Sprites
static LCDSprite* background_spr;
static LCDSprite* score_left_t_spr;
static LCDSprite* score_left_u_spr;
static LCDSprite* score_right_t_spr;
static LCDSprite* score_right_u_spr;
static LCDSprite* left_wins_spr;
static LCDSprite* right_wins_spr;


/*** Efectos de sonido ***/
static AudioSample* paddle_hit_wav;
static AudioSample* ball_hit_wav;
static AudioSample* game_point_wav;
static AudioSample* player_left_wav;
static AudioSample* player_right_wav;
static SamplePlayer* paddle_hit_sfx;
static SamplePlayer* ball_hit_sfx;
static SamplePlayer* game_point_sfx;
static SamplePlayer* winner_sfx;



/*** Palas ***/
// Control del jugador izquierdo
static void PaddleLeftUpdate(void);
// Control del jugador derecho
static void PaddleRightUpdate(void);
// Estructura de datos de las palas
typedef struct PlayerStruct {
    LCDBitmap* img;
    LCDSprite* sprite;
    Vector2 position;
    Vector2 last_position;
    Vector2 target_position;
    int width;
    int height;
    Vector2 speed;
    struct StageBoundaries {
        float top;
        float bottom;
    } stage_bounds;
    uint8_t player_id;
} PlayerStruct;
// Palas
static PlayerStruct paddle_left, paddle_right;
static float crank_angle;


/*** Bola ***/
// Control de la bola
static void BallUpdate(void);
// Estructura de datos de la bola
typedef struct BallStruct {
    LCDBitmap* img;
    LCDSprite* sprite;
    Vector2 position;
    Vector2 last_position;
    int width;
    int height;
    Vector2 speed;
    float velocity;
    struct BallCollisions {
        float top;
        float bottom;
        float left;
        float right;
    } collisions;
} BallStruct;
// Bola
static BallStruct ball;


/*** Colisiones ***/
// Colision de la bola con el jugador izquierdo
static void CollisionsPlayerLeft(void);
// Colision de la bola con el jugador derecho
static void CollisionsPlayerRight(void);
// Collision con las palas (calculo por integracion)
static bool PaddleHit(float hit_line, float top, float bottom);
// Colision con el muro superior
static void CollisionWallTop(void);
// Colision con el muro inferior
static void CollisionWallBottom(void);
// Collision con los muros (calculo por integracion)
static void WallHit(float hit_line);
// Devuelve el nuevo angulo de rebote
static float GetNewAngle(PlayerStruct paddle);
// Devuelve el angulo de servicio
static float GetServiceAngle(PlayerStruct paddle);
// Resuelve las colisiones con los bordes de la pala con la bola
static void PaddleEdgeCollisions(PlayerStruct *paddle);


/*** IA ***/
enum {
    AI_DIFFICULTY_VERY_EASY,
    AI_DIFFICULTY_EASY,
    AI_DIFFICULTY_NORMAL,
    AI_DIFFICULTY_HARD,
    AI_DIFFICULTY_VERY_HARD
};
typedef struct AiStruct {
    uint8_t player_id;          // Jugador controlado por la IA (NEUTRAL = Ninguno)
    uint8_t difficulty;         // Nivel de dificultad actual
    int32_t activity_time;      // Tiempo de actividad
    int32_t colddown_time;      // Tiempo de inactividad
    bool mode_activity;         // Modo actividad?
    int32_t timer;              // Variable para el temporizador
    float min_distance;         // Distancia minima de reaccion
    bool enabled;               // IA habilitada
    uint8_t state;              // Estado de las funciones de la IA
    uint8_t next_state;         // Siguiente estado
    bool direction;             // Direccion
} AiStruct;
static AiStruct ai;
// Control de un jugador a traves de la IA
static void AiControl(PlayerStruct *paddle);
// Calculo de la dificultad segun el handicap del marcador
static void AiGetDifficultyLevel(void);
// Servicio a cargo de la IA
static bool AiService(PlayerStruct *paddle, bool serving);



/*** Maquina de estados de la logica del juego ***/
enum {
    GAME_STATE_SERVICE,
    GAME_STATE_PLAY,
    GAME_STATE_POINT,
    GAME_STATE_NEXT_SET,
    GAME_STATE_GAME_OVER
};
static uint8_t game_state, game_next_state;
// Funciones de la maquina de estados
static void StGameService(void);            // Servicio
static void StGamePlay(void);               // Desarrollo de la partida
static void StGamePoint(void);              // Se ha anotado un punto
static void StGameNextSet(void);            // Espera al siguiente SET
static void StGameOver(void);               // Fin de partida
static void StFadeIn(void);                 // Efecto Fade in
static void StFadeOut(void);                // Efecto Fade out

/*** Datos de la partida ***/
typedef struct GameDataStruct {
    uint8_t max_score;              // Puntuacion maxima selecionada
    uint8_t score_left;             // Jugador izquierdo (1)
    uint8_t score_right;            // Jugador derecho (2)
    uint8_t who_serves;             // Quien saca (0 == saque neutral) ?
    uint8_t who_scored;             // Quien ha marcado ?
    bool input_enabled;             // Entrada de los comandos de los jugadores habilitados?
} GameDataStruct;
static GameDataStruct game_data;



/*** Nucleo de ejecucion del juego ***/
int8_t Pong(int8_t player, int8_t points) {

    // Deshabilita el callback
    NGN_System_DisableCallback();

    // Resultado
    int8_t r = -1;

    // Maquina de estados
    switch (state) {

        case STATE_IDLE:
            break;

        case STATE_START:
            Start(player, points);
            break;

        case STATE_UPDATE:
            Update();
            break;

        case STATE_FADE_IN:
            StFadeIn();
            break;

        case STATE_FADE_OUT:
            StFadeOut();
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
bool Start(int8_t player, int8_t points) {

    // Procesos previos
    Init();

    // Carga de recursos
    if (!Load()) {
        // Error de carga
        playdate->system->logToConsole("Loading failed!");
        next_state = STATE_IDLE;
        return false;
    }

    // Crea la escena
    Create();

    // Valores iniciales de la partida
    Reset();

    // Segun la opcion del menu, selecciona el modo de juego
    switch (player) {
        case PLAYER_ID_LEFT:
            ai.player_id = PLAYER_ID_RIGHT;
            break;
        case PLAYER_ID_RIGHT:
            ai.player_id = PLAYER_ID_LEFT;
            break;
        default:
            ai.player_id = PLAYER_ID_NEUTRAL;
            break;
    }

    // Puntuacion maxima seleccionada
    game_data.max_score = points;

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
    paddle_img = NULL;
    for (uint8_t i = 0; i < SCORE_NUMBERS; i ++) numbers_img[i] = NULL;
    left_wins_img = NULL;
    right_wins_img = NULL;

    // Sprites
    background_spr = NULL;
    score_left_t_spr = NULL;
    score_left_u_spr = NULL;
    score_right_t_spr = NULL;
    score_right_u_spr = NULL;

    // Samples de audio para los SFX
    paddle_hit_wav = NULL;
    ball_hit_wav = NULL;
    game_point_wav = NULL;
    player_left_wav = NULL;
    player_right_wav = NULL;
    // Reproductores para los SFX
    paddle_hit_sfx = NULL;
    ball_hit_sfx = NULL;
    game_point_sfx = NULL;
    winner_sfx = NULL;

}



/*** Funcion de carga de archivos ***/
bool Load(void) {

    // Carga correcta?
    bool successful = true;

    // Fondo
    background_img = NGN_Load_Bitmap("data/images/background_400x240.png");
    if (!background_img) successful = false;

    // Pala
    paddle_img = NGN_Load_Bitmap("data/sprites/paddle_16x64.png");
    if (!paddle_img) successful = false;

    // Pelota
    ball.img = NGN_Load_Bitmap("data/sprites/ball_16x16.png");
    if (!ball.img) successful = false;

    // Numeros del marcador
    char* path = NULL;
    for (uint8_t i = 0; i < SCORE_NUMBERS; i ++) {
        playdate->system->formatString(&path, "data/sprites/numbers/n%d_39x55.png", i);
        numbers_img[i] = NGN_Load_Bitmap(path);
        if (!numbers_img[i]) successful = false;
    }
    playdate->system->realloc(path, 0);

    // Mensages
    left_wins_img = NGN_Load_Bitmap("data/images/msg_left_wins_234x87.png");
    if (!left_wins_img) successful = false;
    right_wins_img = NGN_Load_Bitmap("data/images/msg_right_wins_255x87.png");
    if (!right_wins_img) successful = false;

    // Carga los efectos de sonido
    paddle_hit_wav = playdate->sound->sample->load("data/sfx/paddle_hit_sfx.wav");
    if (!paddle_hit_wav) successful = false;
    ball_hit_wav = playdate->sound->sample->load("data/sfx/ball_hit_sfx.wav");
    if (!ball_hit_wav) successful = false;
    game_point_wav = playdate->sound->sample->load("data/sfx/game_point_sfx.wav");
    if (!game_point_wav) successful = false;
    player_left_wav = playdate->sound->sample->load("data/sfx/player_left_wins_sfx.wav");
    if (!player_left_wav) successful = false;
    player_right_wav = playdate->sound->sample->load("data/sfx/player_right_wins_sfx.wav");
    if (!player_right_wav) successful = false;

    // Carga la musica de fondo
    if (!NGN_Sound_BgmOpen("data/music/game.mp3")) successful = false;

    // Carga correcta
    return successful;

}



/*** Funcion de creacion de la escena ***/
void Create(void) {

    // Crea el fondo
    background_spr = NGN_Sprite_Create(background_img, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

    // Marcador del jugador izquierdo
    score_left_t_spr = NGN_Sprite_Create(numbers_img[0], 115.0f, 52.0f);
    score_left_u_spr = NGN_Sprite_Create(numbers_img[0], (115.0f + 44.0f), 52.0f);

    // Marcador del jugador derecho
    score_right_t_spr = NGN_Sprite_Create(numbers_img[0], 240.0f, 52.0f);
    score_right_u_spr = NGN_Sprite_Create(numbers_img[0], (240.0f + 44.0f), 52.0f);

    // Crea al jugador izquierdo
    paddle_left.position.x = HORIZONTAL_OFFSET;
    paddle_left.position.y = (SCR_HEIGHT / 2);
    paddle_left.last_position = paddle_left.target_position = paddle_left.position;
    paddle_left.sprite = NGN_Sprite_Create(paddle_img, paddle_left.position.x, paddle_left.position.y);
    playdate->graphics->getBitmapData(paddle_img, &paddle_left.width, &paddle_left.height, NULL, NULL, NULL);
    paddle_left.img = paddle_img;
    paddle_left.stage_bounds.top = (VERTICAL_OFFSET + (paddle_left.height / 2));
    paddle_left.stage_bounds.bottom = (SCR_HEIGHT - (VERTICAL_OFFSET + (paddle_left.height / 2)));
    paddle_left.player_id = PLAYER_ID_LEFT;

    // Crea al jugador derecho
    paddle_right.position.x = (SCR_WIDTH - HORIZONTAL_OFFSET);
    paddle_right.position.y = (SCR_HEIGHT / 2);
    paddle_right.last_position = paddle_right.target_position = paddle_right.position;
    paddle_right.sprite = NGN_Sprite_Create(paddle_img, paddle_right.position.x, paddle_right.position.y);
    playdate->graphics->getBitmapData(paddle_img, &paddle_right.width, &paddle_right.height, NULL, NULL, NULL);
    paddle_right.img = paddle_img;
    paddle_right.stage_bounds.top = (VERTICAL_OFFSET + (paddle_right.height / 2));
    paddle_right.stage_bounds.bottom = (SCR_HEIGHT - (VERTICAL_OFFSET + (paddle_right.height / 2)));
    paddle_right.player_id = PLAYER_ID_RIGHT;

    // Crea la bola
    ball.position.x = (SCR_WIDTH / 2);
    ball.position.y = (SCR_HEIGHT / 2);
    ball.last_position = ball.position;
    ball.sprite = NGN_Sprite_Create(ball.img, ball.position.x, ball.position.y);
    playdate->graphics->getBitmapData(ball.img, &ball.width, &ball.height, NULL, NULL, NULL);
    ball.speed.x = BALL_MIN_SPEED;
    ball.speed.y = BALL_MIN_SPEED;
    ball.collisions.top = (VERTICAL_OFFSET + (ball.height / 2));
    ball.collisions.bottom = (SCR_HEIGHT - (VERTICAL_OFFSET + (ball.height / 2)));
    //ball.collisions.left = (ball.width / 2);
    ball.collisions.left = 0.0f;
    //ball.collisions.right = (SCR_WIDTH - (ball.width / 2));
    ball.collisions.right = SCR_WIDTH;
    ball.velocity = BALL_MIN_SPEED;

    // Crea los mensages
    left_wins_spr = NGN_Sprite_Create(left_wins_img, (SCR_WIDTH / 2), 154);
    playdate->sprite->setVisible(left_wins_spr, false);
    right_wins_spr = NGN_Sprite_Create(right_wins_img, (SCR_WIDTH / 2), 154);
    playdate->sprite->setVisible(right_wins_spr, false);

    // Asigna los samples de los sfx
    paddle_hit_sfx = playdate->sound->sampleplayer->newPlayer();
    playdate->sound->sampleplayer->setSample(paddle_hit_sfx, paddle_hit_wav);
    ball_hit_sfx = playdate->sound->sampleplayer->newPlayer();
    playdate->sound->sampleplayer->setSample(ball_hit_sfx, ball_hit_wav);
    game_point_sfx = playdate->sound->sampleplayer->newPlayer();
    playdate->sound->sampleplayer->setSample(game_point_sfx, game_point_wav);
    winner_sfx = playdate->sound->sampleplayer->newPlayer();

    // Inicia la musica
    NGN_Sound_BgmPlay(0);
    NGN_Sound_BgmSetVolume(0.0f);

}



/*** Funcion con la logica del juego ***/
void Logic(void) {

    // Maquina de estados del juego
    switch (game_state) {
        case GAME_STATE_SERVICE:
            StGameService();
            break;
        case GAME_STATE_PLAY:
            StGamePlay();
            break;
        case GAME_STATE_POINT:
            StGamePoint();
            break;
        case GAME_STATE_NEXT_SET:
            StGameNextSet();
            break;
        case GAME_STATE_GAME_OVER:
            StGameOver();
            break;
        default:
            break;
    }
    game_state = game_next_state;

}



/*** Funcion de render de los graficos ***/
void Render(void) {

    // Sprites
    playdate->sprite->drawSprites();

}



/*** Datos iniciales de la partida ***/
void Reset(void) {

    // Oculta los mensajes
    playdate->sprite->setVisible(left_wins_spr, false);
    playdate->sprite->setVisible(right_wins_spr, false);

    // Datos iniciales
    game_data.who_serves = PLAYER_ID_NEUTRAL;
    game_data.who_scored = PLAYER_ID_NEUTRAL;
    game_data.score_left = 0;
    game_data.score_right = 0;
    game_data.input_enabled = true;
    crank_angle = 0.0f;

    // Maquina de estados del juego
    game_state = game_next_state = GAME_STATE_SERVICE;

    // IA
    ai.timer = 0;
    ai.state = ai.next_state = 0;

}



/*** Limpieza de los datos antes de salir ***/
void CleanUp(void) {

    // Elimina el reproductor de musica
    NGN_Sound_BgmClose();

    // Efectos de sonido
    NGN_Sound_FreeSfx(paddle_hit_sfx);
    NGN_Sound_FreeSfx(ball_hit_sfx);
    NGN_Sound_FreeSfx(game_point_sfx);
    NGN_Sound_FreeSfx(winner_sfx);

    // Archivos de samples
    NGN_Sound_FreeSample(paddle_hit_wav);
    NGN_Sound_FreeSample(ball_hit_wav);
    NGN_Sound_FreeSample(game_point_wav);
    NGN_Sound_FreeSample(player_left_wav);
    NGN_Sound_FreeSample(player_right_wav);

    // Elimina los sprites creados
    NGN_Sprite_Delete(background_spr);
    NGN_Sprite_Delete(score_left_t_spr);
    NGN_Sprite_Delete(score_left_u_spr);
    NGN_Sprite_Delete(score_right_t_spr);
    NGN_Sprite_Delete(score_right_u_spr);
    NGN_Sprite_Delete(left_wins_spr);
    NGN_Sprite_Delete(right_wins_spr);

    // Vacia el displaylist
    playdate->sprite->removeAllSprites();

    // Elimina las imagenes
    NGN_Unload_Bitmap(background_img);
    NGN_Unload_Bitmap(paddle_img);
    for (uint8_t i = 0; i < SCORE_NUMBERS; i ++) NGN_Unload_Bitmap(numbers_img[i]);
    NGN_Unload_Bitmap(left_wins_img);
    NGN_Unload_Bitmap(right_wins_img);

    // Configura la maquina de estados para la siguiente ejecucion
    state = next_state = STATE_START;

}



/*** Pala izquierda ***/
void PaddleLeftUpdate(void) {

    // Si la entrada esta deshabilitada, anula el comando
    if (!game_data.input_enabled) return;

    // Control por DPAD
    float speed = 0.0f;
    if (ngn_input.dpad.up.held) speed -= PLAYER_SPEED;
    if (ngn_input.dpad.down.held) speed += PLAYER_SPEED;

    // Aplica el vector de velocidad
    paddle_left.target_position.y += speed;

    // Limites
    if (paddle_left.target_position.y < paddle_left.stage_bounds.top) paddle_left.target_position.y = paddle_left.stage_bounds.top;
    if (paddle_left.target_position.y > paddle_left.stage_bounds.bottom) paddle_left.target_position.y = paddle_left.stage_bounds.bottom;

    // Lerp
    float v = ((paddle_left.target_position.y - paddle_left.position.y) / PLAYER_SMOOTH);
    if (fabsf(v) < PLAYER_SNAP) {
        paddle_left.position.y = paddle_left.target_position.y;
    } else {
        paddle_left.position.y += v;
    }

    // Actualiza la posicion
    playdate->sprite->moveTo(paddle_left.sprite, paddle_left.position.x, paddle_left.position.y);
    paddle_left.last_position = paddle_left.position;

}



/*** Pala derecha ***/
void PaddleRightUpdate(void) {

    // Si la entrada esta deshabilitada, anula el comando
    if (!game_data.input_enabled) return;

    // Control por Manivela
    float angle = (ngn_input.crank.angle > 180.0f) ? (360.0f - ngn_input.crank.angle) : ngn_input.crank.angle;
    float delta = ((angle - crank_angle) / CRANK_SMOOTH);
    crank_angle += delta;
    float gap = (paddle_right.stage_bounds.bottom - paddle_right.stage_bounds.top);
    float offset = ((crank_angle * gap) / 180.0f);
    paddle_right.position.y = (paddle_right.stage_bounds.top + offset);

    // Limites
    if (paddle_right.position.y < paddle_right.stage_bounds.top) paddle_right.position.y = paddle_right.stage_bounds.top;
    if (paddle_right.position.y > paddle_right.stage_bounds.bottom) paddle_right.position.y = paddle_right.stage_bounds.bottom;

    // Actualiza la posicion
    playdate->sprite->moveTo(paddle_right.sprite, paddle_right.position.x, paddle_right.position.y);
    paddle_right.last_position = paddle_right.position;

}



/*** Bola ***/
void BallUpdate(void) {

    // Aplica el vector de desplazamiento
    ball.position.x += ball.speed.x;
    ball.position.y += ball.speed.y;

    // Colision con los muros
    CollisionWallTop();
    CollisionWallBottom();

    // Colisiones con los jugadores
    CollisionsPlayerLeft();
    CollisionsPlayerRight();

    // Punto del jugador derecho (2)
    if (ball.position.x < ball.collisions.left) {
        ball.position.x = ball.collisions.left;
        game_data.who_scored = PLAYER_ID_RIGHT;
        game_data.who_serves = PLAYER_ID_LEFT;
        game_next_state = GAME_STATE_POINT;
    }
    // Punto del jugador izquierdo (1)
    if (ball.position.x > ball.collisions.right) {
        ball.position.x = ball.collisions.right;
        game_data.who_scored = PLAYER_ID_LEFT;
        game_data.who_serves = PLAYER_ID_RIGHT;
        game_next_state = GAME_STATE_POINT;
    }

    // Actualiza la posicion
    playdate->sprite->moveTo(ball.sprite, ball.position.x, ball.position.y);
    ball.last_position = ball.position;

}



/*** Colisiones con el jugador izquierdo ***/
void CollisionsPlayerLeft(void) {

    // Si va en direccion contraria, sal
    if (ball.speed.x > 0.0f) return;

    // Define los limites
    float hit_line = (paddle_left.position.x + (paddle_left.width / 2) + (ball.width / 2));
    float top = (paddle_left.position.y - (paddle_left.height / 2) - (ball.height / 2));
    float bottom = (paddle_left.position.y + (paddle_left.height / 2) + (ball.height / 2));

    // Si no has cruzado la linea de impacto, sal
    if (ball.position.x > hit_line) return;

    // De haber cuzado la linea de impacto, en la posicion anterior, ya la habias cruzado?
    if (ball.last_position.x < hit_line) {
        // Colisiones con el borde de la pala
        PaddleEdgeCollisions(&paddle_left);
    } else {
        // Hay colision con la pala?
        if (PaddleHit(hit_line, top, bottom)) {
            // Aceleracion
            ball.velocity *= BALL_ACCELERATION;
            if (ball.velocity > BALL_MAX_SPEED) ball.velocity = BALL_MAX_SPEED;
            // Rebote
            float angle = GetNewAngle(paddle_left);
            ball.speed.x = (cosf(angle) * ball.velocity);
            ball.speed.y = (sinf(angle) * ball.velocity);
        }
    }

}



/*** Colisiones con el jugador derecho ***/
void CollisionsPlayerRight(void) {

    // Si va en direccion contraria, sal
    if (ball.speed.x < 0.0f) return;

    // Define los limites
    float hit_line = (paddle_right.position.x - (paddle_right.width / 2) - (ball.width / 2));
    float top = (paddle_right.position.y - (paddle_right.height / 2) - (ball.height / 2));
    float bottom = (paddle_right.position.y + (paddle_right.height / 2) + (ball.height / 2));

    // Si no has cruzado la linea de impacto, sal
    if (ball.position.x < hit_line) return;

    // De haber cuzado la linea de impacto, en la posicion anterior, ya la habias cruzado?
    if (ball.last_position.x > hit_line) {
        // Colisiones con el borde de la pala
        PaddleEdgeCollisions(&paddle_right);
    } else {
        // Hay colision con la pala?
        if (PaddleHit(hit_line, top, bottom)) {
            // Aceleracion
            ball.velocity *= BALL_ACCELERATION;
            if (ball.velocity > BALL_MAX_SPEED) ball.velocity = BALL_MAX_SPEED;
            // Rebote
            float angle = (PI - GetNewAngle(paddle_right));
            ball.speed.x = (cosf(angle) * ball.velocity);
            ball.speed.y = (sinf(angle) * ball.velocity);
        }
    }

}


/*** Collision con las palas (calculo por integracion) ***/
bool PaddleHit(float hit_line, float top, float bottom) {

    // Colision?
    bool collision = false;

    // Interpola el punto de colision
    Vector2 distance;
    distance.x = fabsf(ball.position.x - ball.last_position.x);
    distance.y = (ball.position.y - ball.last_position.y);
    Vector2 delta;
    delta.x = fabsf(ball.last_position.x - hit_line);
    delta.y = ((distance.y * delta.x) / distance.x);
    Vector2 hit_point;
    hit_point.x = hit_line;
    hit_point.y = (ball.last_position.y + delta.y);

    // Ahora verifica si hay colision en ese punto
    if ((hit_point.y >= top) && (hit_point.y <= bottom)) {
        // Ajusta la posicion de la pelota
        ball.position.x = hit_point.x;
        ball.position.y = hit_point.y;
        //ball.last_position = ball.position;
        playdate->sprite->moveTo(ball.sprite, ball.position.x, ball.position.y);
        // Indica que hay rebote
        collision |= true;
        // Efecto de sonido
        playdate->sound->sampleplayer->play(paddle_hit_sfx, 1, 1.0f);
    }

    // Resultado de la colision
    return collision;

}



/*** Colision con el muro superior ***/
void CollisionWallTop(void) {

    // Si no vas hacia arriba...
    if (ball.speed.y > 0.0f) return;

    // Si no has superado el punto de colision...
    if (ball.position.y > ball.collisions.top) return;

    // Ajuste de posicion
    WallHit(ball.collisions.top);

    // Y rebote
    ball.speed.y = fabsf(ball.speed.y);

}



/*** Colision con el muro inferior ***/
void CollisionWallBottom(void) {

    // Si no vas hacia abajo...
    if (ball.speed.y < 0.0f) return;

    // Si no has superado el punto de colision...
    if (ball.position.y < ball.collisions.bottom) return;

    // Ajuste de posicion
    WallHit(ball.collisions.bottom);

    // Y rebote
    ball.speed.y = -fabsf(ball.speed.y);

}



/*** Collision con los muros (calculo por integracion) ***/
void WallHit(float hit_line) {

    // Interpola el punto de colision
    Vector2 distance;
    distance.y = fabsf(ball.position.y - ball.last_position.y);
    distance.x = (ball.position.x - ball.last_position.x);
    Vector2 delta;
    delta.y = fabsf(ball.last_position.y - hit_line);
    delta.x = ((distance.x * delta.y) / distance.y);
    Vector2 hit_point;
    hit_point.y = hit_line;
    hit_point.x = (ball.last_position.x + delta.x);

    // Ajusta la posicion de la pelota
    ball.position.x = hit_point.x;
    ball.position.y = hit_point.y;
    //ball.last_position = ball.position;
    playdate->sprite->moveTo(ball.sprite, ball.position.x, ball.position.y);

    // Efecto de sonido
    playdate->sound->sampleplayer->play(ball_hit_sfx, 1, 1.0f);

}



/*** Devuelve el nuevo angulo de rebote ***/
float GetNewAngle(PlayerStruct paddle) {

    float height = (paddle.height / 2.0f);
    float distance = (ball.position.y - paddle.position.y);
    float angle = ((distance * BALL_MAX_ANGLE) / height);
    if (fabsf(angle) < BALL_MIN_ANGLE) {
        if (distance > 0.0f) {
            angle = BALL_MIN_ANGLE;
        } else {
            angle = -BALL_MIN_ANGLE;
        }
    }
    return angle;

}



/*** Devuelve el angulo de servicio ***/
float GetServiceAngle(PlayerStruct paddle) {

    float distance = ((SCR_HEIGHT / 2) - paddle.position.y);
    float max_distance = fabsf(paddle.stage_bounds.bottom - (SCR_HEIGHT / 2));
    float angle = ((distance * BALL_MAX_ANGLE) / max_distance);
    if (fabsf(angle) < BALL_MIN_ANGLE) {
        if (distance > 0.0f) {
            angle = BALL_MIN_ANGLE;
        } else {
            angle = -BALL_MIN_ANGLE;
        }
    }
    return angle;

}



// Resuelve las colisiones con los bordes de la pala con la bola
void PaddleEdgeCollisions(PlayerStruct *paddle) {

    // Se ha cruzado, colisiones verticales con la pala
    if (NGN_Collisions_SpriteBox(ball.sprite, paddle->sprite)) {
        // Asegurate que el control de las palas este deshabilitado
        game_data.input_enabled = false;
        if (ball.position.y < paddle->position.y) {
            // Si la pelota esta por encima...
            ball.speed.y = -fabsf(ball.speed.y);
            paddle->target_position.y = ball.position.y + ((paddle->height / 2) + (ball.height / 2) + 2);
        } else {
            // Si esta por debajo...
            ball.speed.y = fabsf(ball.speed.y);
            paddle->target_position.y = ball.position.y - ((paddle->height / 2) + (ball.height / 2) + 2);
        }
        // Limites
        if (paddle->target_position.y < paddle->stage_bounds.top) paddle->target_position.y = paddle->stage_bounds.top;
        if (paddle->target_position.y > paddle->stage_bounds.bottom) paddle->target_position.y = paddle->stage_bounds.bottom;
        // Actualiza la posicion de la pala
        paddle->last_position = paddle->position = paddle->target_position;
        playdate->sprite->moveTo(paddle->sprite, paddle->position.x, paddle->position.y);
    }

}



/*** Control de un jugador a traves de la IA ***/
void AiControl(PlayerStruct *paddle) {

    // Si la entrada esta deshabilitada, anula el comando
    if (!game_data.input_enabled) return;

    // Control por IA
    float distance_x = fabsf(ball.position.x - paddle->position.x);
    if (distance_x > ai.min_distance) {
        ai.timer --;
        if (ai.mode_activity) {
            //playdate->system->logToConsole("Active");
            if (ai.timer < 0) {
                ai.mode_activity = false;
                ai.timer = ai.colddown_time;
            }
        } else {
            //playdate->system->logToConsole("idle");
            if (ai.timer > 0) return;
            ai.mode_activity = true;
            ai.timer = ai.activity_time;
        }
    } else {
        //playdate->system->logToConsole("FULL");
        ai.mode_activity = true;
        ai.timer = ai.activity_time;
    }

    // Direccion
    float distance_y = (ball.position.y - paddle->position.y);
    if (distance_y > PLAYER_SPEED) {
        paddle->target_position.y += PLAYER_SPEED;
    } else if (distance_y < -PLAYER_SPEED) {
        paddle->target_position.y -= PLAYER_SPEED;
    }

    // Limites
    if (paddle->target_position.y < paddle->stage_bounds.top) paddle->target_position.y = paddle->stage_bounds.top;
    if (paddle->target_position.y > paddle->stage_bounds.bottom) paddle->target_position.y = paddle->stage_bounds.bottom;

    // Lerp
    float v = ((paddle->target_position.y - paddle->position.y) / PLAYER_SMOOTH);
    if (fabsf(v) < PLAYER_SNAP) {
        paddle->position.y = paddle->target_position.y;
    } else {
        paddle->position.y += v;
    }

    // Actualiza la posicion
    playdate->sprite->moveTo(paddle->sprite, paddle->position.x, paddle->position.y);
    paddle->last_position = paddle->position;

}



/*** Calculo de la dificultad segun el handicap del marcador ***/
void AiGetDifficultyLevel(void) {

    // Diferencia de puntos segun que jugador controla la IA
    int32_t handicap = 0;
    if (ai.player_id == PLAYER_ID_LEFT) {
        handicap = (game_data.score_right - game_data.score_left);
    } else if (ai.player_id == PLAYER_ID_RIGHT) {
        handicap = (game_data.score_left - game_data.score_right);
    }

    // Calculo de la dificultad segun el handicap
    if ((handicap >= -3) && (handicap <= 3)) {
        // Normal
        //playdate->system->logToConsole("Level: normal");
        ai.difficulty = AI_DIFFICULTY_NORMAL;
        ai.activity_time = 20;
        ai.colddown_time = 10;
        ai.min_distance = 100.0f;
    } else if (handicap > 7) {
        // Muy dificil
        //playdate->system->logToConsole("Level: very hard");
        ai.difficulty = AI_DIFFICULTY_VERY_HARD;
        ai.activity_time = 30;
        ai.colddown_time = 0;
        ai.min_distance = 250.0f;
    } else if (handicap > 3) {
        // Dificil
        //playdate->system->logToConsole("Level: hard");
        ai.difficulty = AI_DIFFICULTY_HARD;
        ai.activity_time = 24;
        ai.colddown_time = 8;
        ai.min_distance = 200.0f;
    } else if (handicap < -7) {
        // Muy facil
        //playdate->system->logToConsole("Level: very easy");
        ai.difficulty = AI_DIFFICULTY_VERY_EASY;
        ai.activity_time = 10;
        ai.colddown_time = 20;
        ai.min_distance = 50.0f;
    } else if (handicap < -3) {
        // Facil
        //playdate->system->logToConsole("Level: easy");
        ai.difficulty = AI_DIFFICULTY_EASY;
        ai.activity_time = 15;
        ai.colddown_time = 15;
        ai.min_distance = 75.0f;
    }

    // Prepara los datos de la IA
    ai.mode_activity = true;
    ai.enabled = true;
    ai.timer = ai.activity_time;

}



/*** Servicio a cargo de la IA ***/
bool AiService(PlayerStruct *paddle, bool serving) {

    // Resultado, servicio?
    bool r = false;

    // Teclas virtuales
    bool up = false, down = false;


    if (serving) {

        // Si saca la IA...

        switch (ai.state) {

            // Decide la espera
            case 0:
                ai.timer = 0;
                ai.activity_time = ((rand() % 20) + 20);
                ai.next_state = 1;
                break;

            // Espera
            case 1:
                ai.timer ++;
                if (ai.timer < ai.activity_time) break;
                ai.next_state = 2;
                break;

            // Decide la direccion y la duracion
            case 2:
                ai.timer = 0;
                ai.activity_time = ((rand() % 6) + 4);
                ai.direction = (((rand() % 8192) % 256) >= 127) ? true:false;
                ai.next_state = 3;
                break;

            // Muevete
            case 3:
                if (ai.direction) {
                    up = true;
                } else {
                    down = true;
                }
                ai.timer ++;
                if (ai.timer < ai.activity_time) break;
                ai.timer = 0;
                ai.activity_time = 0;
                ai.next_state = ai.state = 0;
                r |= true;
                break;
        }

        ai.state = ai.next_state;

    } else {

        // Si saca el jugador humano...
        float m = ((float)SCR_HEIGHT / 2.0f);
        float d1 = (fabsf(m - ball.position.y) / 2.0f);
        float d2 = fabsf(m - paddle->position.y);
        if (ball.position.y < m) {
            if (paddle->position.y < m) {
                down = true;
            } else if ((d1 - d2) > PLAYER_SPEED) {
                down = true;
            } else if ((d2 - d1) > PLAYER_SPEED) {
                up = true;
            }        } else {
            if (paddle->position.y > m) {
                up = true;
            } else if ((d1 - d2) > PLAYER_SPEED) {
                up = true;
            } else if ((d2 - d1) > PLAYER_SPEED) {
                down = true;
            }
        }

    }

    // Direccion
    if (down) {
        paddle->target_position.y += PLAYER_SPEED;
    } else if (up) {
        paddle->target_position.y -= PLAYER_SPEED;
    }

    // Limites
    if (paddle->target_position.y < paddle->stage_bounds.top) paddle->target_position.y = paddle->stage_bounds.top;
    if (paddle->target_position.y > paddle->stage_bounds.bottom) paddle->target_position.y = paddle->stage_bounds.bottom;

    // Lerp
    float v = ((paddle->target_position.y - paddle->position.y) / PLAYER_SMOOTH);
    if (fabsf(v) < PLAYER_SNAP) {
        paddle->position.y = paddle->target_position.y;
    } else {
        paddle->position.y += v;
    }

    // Actualiza la posicion
    playdate->sprite->moveTo(paddle->sprite, paddle->position.x, paddle->position.y);
    paddle->last_position = paddle->position;

    // Resultado
    return r;

}



/*** Estado de juego: Servicio ***/
void StGameService(void) {

    // Asegurate que el control de las palas este habilitado
    game_data.input_enabled = true;

    // Variables de control
    bool service = false;
    bool ai_service = false;
    float angle = 0.0f;

    // Control de las palas (manual)
    if (ai.player_id != PLAYER_ID_LEFT) PaddleLeftUpdate();
    if (ai.player_id != PLAYER_ID_RIGHT) PaddleRightUpdate();

    // Control de las palas (IA)
    if (ai.player_id == PLAYER_ID_LEFT) ai_service |= AiService(&paddle_left, (game_data.who_serves == PLAYER_ID_LEFT));
    if (ai.player_id == PLAYER_ID_RIGHT) ai_service |= AiService(&paddle_right, (game_data.who_serves == PLAYER_ID_RIGHT));

    // Segun quien saca...
    switch (game_data.who_serves) {
        case PLAYER_ID_NEUTRAL:
            if (ngn_input.button.a.press || ngn_input.button.b.press) {
                switch ((rand() % 4)) {
                    case 0:
                        angle = -BALL_MIN_ANGLE;
                        break;
                    case 1:
                        angle = BALL_MIN_ANGLE;
                        break;
                    case 2:
                        angle = (PI + BALL_MIN_ANGLE);
                        break;
                    case 3:
                        angle = (PI - BALL_MIN_ANGLE);
                        break;
                }
                service |= true;
            }
            break;
        case PLAYER_ID_LEFT:
            ball.position.x = (paddle_left.position.x + BALL_SERVICE_OFFSET);
            ball.position.y = paddle_left.position.y;
            if (
                (ngn_input.button.b.press && (ai.player_id != PLAYER_ID_LEFT))
                ||
                ((ai.player_id == PLAYER_ID_LEFT) && ai_service)
            ) {
                angle = GetServiceAngle(paddle_left);
                service |= true;
            }
            break;
        case PLAYER_ID_RIGHT:
            ball.position.x = (paddle_right.position.x - BALL_SERVICE_OFFSET);
            ball.position.y = paddle_right.position.y;
            if (
                (ngn_input.button.a.press && (ai.player_id != PLAYER_ID_RIGHT))
                ||
                ((ai.player_id == PLAYER_ID_RIGHT) && ai_service)
            ) {
                angle = (PI - GetServiceAngle(paddle_right));
                service |= true;
            }
            break;
    }
    // Actualiza la posicion de la bola
    playdate->sprite->moveTo(ball.sprite, ball.position.x, ball.position.y);
    ball.last_position = ball.position;

    // Si no se solicita el servicio, sal
    if (!service) return;

    // Calcula el vector de movimiento de la bola
    ball.velocity = BALL_MIN_SPEED;
    ball.speed.x = (cosf(angle) * ball.velocity);
    ball.speed.y = (sinf(angle) * ball.velocity);

    // Si estas jugando conta la IA, calculo del nivel de dificultad
    if (ai.player_id != PLAYER_ID_NEUTRAL) AiGetDifficultyLevel();

    // Siguiente estado
    game_next_state = GAME_STATE_PLAY;

}



/*** Estado de juego: Desarrollo de la partida ***/
void StGamePlay(void) {

    // Control de las palas (jugador izquierdo)
    if (ai.player_id != PLAYER_ID_LEFT) {
        PaddleLeftUpdate();
    } else {
        AiControl(&paddle_left);
    }

    // Control de las palas (jugador derecho)
    if (ai.player_id != PLAYER_ID_RIGHT) {
        PaddleRightUpdate();
    } else {
        AiControl(&paddle_right);
    }

    // Bola
    BallUpdate();

}



/*** Estado de juego: Se ha anotado un punto ***/
void StGamePoint(void) {

    // Calculo de indices
    uint8_t tens = 0;
    uint8_t units = 0;

    // Actualiza los marcadores
    switch (game_data.who_scored) {
        case PLAYER_ID_LEFT:
            game_data.score_left ++;
            tens = (game_data.score_left / 10);
            units = (game_data.score_left % 10);
            playdate->sprite->setImage(score_left_t_spr, numbers_img[tens], kBitmapUnflipped);
            playdate->sprite->setImage(score_left_u_spr, numbers_img[units], kBitmapUnflipped);
            break;
        case PLAYER_ID_RIGHT:
            game_data.score_right ++;
            tens = (game_data.score_right / 10);
            units = (game_data.score_right % 10);
            playdate->sprite->setImage(score_right_t_spr, numbers_img[tens], kBitmapUnflipped);
            playdate->sprite->setImage(score_right_u_spr, numbers_img[units], kBitmapUnflipped);
            break;
    }

//    // Debug - Console score
//    char *txt;
//    playdate->system->formatString(&txt, "%d : %d", game_data.score_left, game_data.score_right);
//    playdate->system->logToConsole(txt);
//    playdate->system->realloc(txt, 0);

    // Efecto de sonido
    playdate->sound->sampleplayer->play(game_point_sfx, 1, 1.0f);

    // Siguiente estado
    game_next_state = GAME_STATE_NEXT_SET;

}



/*** Espera al siguiente SET ***/
void StGameNextSet(void) {

    // Mientras este sonando el efecto de sonido...
    if (playdate->sound->sampleplayer->isPlaying(game_point_sfx)) return;

    // Mientras no se pulse ni A ni B
    if (!ngn_input.button.a.press && !ngn_input.button.b.press) return;

    if (game_data.score_left >= game_data.max_score) {
        // Si ha ganado el jugador izquierdo...
        playdate->sprite->setVisible(left_wins_spr, true);
        playdate->sound->sampleplayer->setSample(winner_sfx, player_left_wav);
        playdate->sound->sampleplayer->play(winner_sfx, 1, 1.0f);
        // Fin de partida
        game_next_state = GAME_STATE_GAME_OVER;
    } else if (game_data.score_right >= game_data.max_score) {
        // Si ha ganado el jugador derecho...
        playdate->sprite->setVisible(right_wins_spr, true);
        playdate->sound->sampleplayer->setSample(winner_sfx, player_left_wav);
        playdate->sound->sampleplayer->play(winner_sfx, 1, 1.0f);
        // Fin de partida
        game_next_state = GAME_STATE_GAME_OVER;
    } else {
        // Reinicia la posicion de los jugadores
        paddle_left.target_position.y = paddle_left.position.y = paddle_left.last_position.y = (SCR_HEIGHT / 2);
        paddle_right.target_position.y = paddle_right.position.y = paddle_right.last_position.y = (SCR_HEIGHT / 2);
        playdate->sprite->moveTo(paddle_left.sprite, paddle_left.position.x, paddle_left.position.y);
        playdate->sprite->moveTo(paddle_right.sprite, paddle_right.position.x, paddle_right.position.y);
        // Siguiente SET
        game_next_state = GAME_STATE_SERVICE;
    }

}



/*** Fin de partida ***/
void StGameOver(void) {

    // Mientras este sonando el efecto de sonido...
    if (playdate->sound->sampleplayer->isPlaying(winner_sfx)) return;

    // Mientras no se pulse ni A ni B
    if (!ngn_input.button.a.press && !ngn_input.button.b.press) return;

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
