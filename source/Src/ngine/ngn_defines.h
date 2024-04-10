/******************************************************************************

    [N'gine for Playdate SDK]: Archivo de definiciones
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



#ifndef NGN_DEFINES_H_INCLUDED
#define NGN_DEFINES_H_INCLUDED



/*** Includes ***/
// SDK
#include "pd_api.h"



/*** Soporte para bool ***/
// Definir bool
#ifndef bool
    #define bool uint8_t
#endif
// Definir true y false si no están definidos
#ifndef true
    #define true 1
#endif
#ifndef false
    #define false 0
#endif


/*** Definiciones de constantes matematicas ***/
#define PI 3.14159265f              // Definicion del numero PI
#define DEG2RAD (PI / 180.0f)       // Constante de conversion Grados a Radianes
#define RAD2DEG (180.0f / PI)       // Constante de conversion Radianes a Grados


/*** Parametros de pantalla ***/
#define SCR_WIDTH 400
#define SCR_HEIGHT 240


/*** Puntero a la API del SDK ***/
extern PlaydateAPI* playdate;



/*** Vector 2 ***/
typedef struct Vector2 {
    float x;
    float y;
} Vector2;

/*** Size 2 ***/
typedef struct Size2 {
    float width;
    float height;
} Size2;



#endif // NGN_DEFINES_H_INCLUDED
