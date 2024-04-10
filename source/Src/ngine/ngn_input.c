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



/*** Includes ***/
// C
#include <stdlib.h>
#include <stdio.h>
// Ngine
#include "ngn_input.h"



/*** Variables del nucleo de ejecucion del programa ***/
InputVars ngn_input;



/*** Declaracion de las funciones de los metodos de entrada (llamadas internas) ***/
// Actualiza el estado de una tecla concreta
static void KeyUpdate(InputKeyStates *_key, bool _state);



// Reinicia los datos de las entradas
void NGN_Input_Reset(void) {

    // D-PAD
    ngn_input.dpad.up.held = ngn_input.dpad.up.press = ngn_input.dpad.up.release = ngn_input.dpad.up.last = false;
    ngn_input.dpad.down.held = ngn_input.dpad.down.press = ngn_input.dpad.down.release = ngn_input.dpad.down.last = false;
    ngn_input.dpad.left.held = ngn_input.dpad.left.press = ngn_input.dpad.left.release = ngn_input.dpad.left.last = false;
    ngn_input.dpad.right.held = ngn_input.dpad.right.press = ngn_input.dpad.right.release = ngn_input.dpad.right.last = false;
    // A & B
    ngn_input.button.a.held = ngn_input.button.a.press = ngn_input.button.a.release = ngn_input.button.a.last = false;
    ngn_input.button.b.held = ngn_input.button.b.press = ngn_input.button.b.release = ngn_input.button.b.last = false;

    // Crank
    ngn_input.crank.angle = 0.0f;
    ngn_input.crank.delta = 0.0f;
    ngn_input.crank.docked.held = ngn_input.crank.docked.press = ngn_input.crank.docked.release = ngn_input.crank.docked.last = false;

    // Acelerometro
    ngn_input.accelerometer.x = 0.0f;
    ngn_input.accelerometer.y = 0.0f;
    ngn_input.accelerometer.z = 0.0f;

}



// Actualiza el estado de las entradas
void NGN_Input_Update(void) {

    // Accede al sistema y obten el estado actual de las entradas
	PDButtons state;
	playdate->system->getButtonState(&state, NULL, NULL);

	// Directional pad
	KeyUpdate(&ngn_input.dpad.up, (state & kButtonUp));
	KeyUpdate(&ngn_input.dpad.down, (state & kButtonDown));
	KeyUpdate(&ngn_input.dpad.left, (state & kButtonLeft));
	KeyUpdate(&ngn_input.dpad.right, (state & kButtonRight));

	// Botones A y B
	KeyUpdate(&ngn_input.button.a, (state & kButtonA));
	KeyUpdate(&ngn_input.button.b, (state & kButtonB));

	// Crank
	ngn_input.crank.angle = playdate->system->getCrankAngle();
	ngn_input.crank.delta = playdate->system->getCrankChange();
	KeyUpdate(&ngn_input.crank.docked, playdate->system->isCrankDocked());

	// Acelerometro
	playdate->system->getAccelerometer(&ngn_input.accelerometer.x, &ngn_input.accelerometer.y, &ngn_input.accelerometer.z);

	// Cualquier tecla
    bool all_helds =
        ngn_input.dpad.up.held | ngn_input.dpad.down.held | ngn_input.dpad.left.held | ngn_input.dpad.right.held |
        ngn_input.button.a.held | ngn_input.button.b.held
    ;
    KeyUpdate(&ngn_input.any_key, all_helds);


}



// Actualiza el estado de una tecla concreta
void KeyUpdate(InputKeyStates *_key, bool _state) {

    _key->held = (_state != 0) ? true:false;
    _key->press = (_key->held & ~_key->last);
    _key->release = (~_key->held & _key->last);
    _key->last = _key->held;

}

