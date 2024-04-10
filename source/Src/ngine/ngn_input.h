/******************************************************************************

    [N'gine for Playdate SDK]: Metodos de entrada
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



#ifndef NGN_INPUT_H_INCLUDED
#define NGN_INPUT_H_INCLUDED



/*** Includes ***/
// C
#include <stdio.h>
#include <stdlib.h>
// SDK
#include "pd_api.h"
// Ngine
#include "ngn_defines.h"
#include "ngn_settings.h"



/*** Declaracion de variables ***/

// Control de la entrada (Estados de los botones)
typedef struct InputKeyStates{
    bool held;
    bool press;
    bool release;
    bool last;
} InputKeyStates;

// Variables de los metodos de entrada
typedef struct InputVars {
    // Digital PAD
    struct dpad {
        InputKeyStates up;
        InputKeyStates down;
        InputKeyStates left;
        InputKeyStates right;
    } dpad;
    // Botones
    struct button {
        InputKeyStates a;
        InputKeyStates b;
    } button;
    // Manivela
    struct crank {
        float angle;
        float delta;
        InputKeyStates docked;
    } crank;
    // Acelerometro
    struct accelerometer {
        float x;
        float y;
        float z;
    } accelerometer;
    // Cualquier tecla
    InputKeyStates any_key;
} InputVars;

// Acceso externo a los metodos de entrada
extern InputVars ngn_input;



/*** Declaracion de las funciones de los metodos de entrada (llamadas externas) ***/

// Reinicia los datos de las entradas
void NGN_Input_Reset(void);

// Actualiza el estado de las entradas
void NGN_Input_Update(void);



#endif // NGN_INPUT_H_INCLUDED
