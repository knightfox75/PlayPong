/******************************************************************************

    [N'gine for Playdate SDK]: Metodos de carga de archivos
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
#include "ngn_load.h"



/*** Carga un bitmap desde el sistema de archivos ***/
LCDBitmap* NGN_Load_Bitmap(const char* path) {

    // Gestion de errores
    const char* error = NULL;

    // Intenta cargar la imagen
	LCDBitmap *bitmap = playdate->graphics->loadBitmap(path, &error);

    // Error de carga?
    if (error) playdate->system->logToConsole("[NGN_Load] Error loading image at path: %s. Error: %s", path, error);

    // Devuevel al imagen cargada
    return bitmap;

}



/*** Elimina un bitmap previamente cargado ***/
void NGN_Unload_Bitmap(LCDBitmap* bitmap) {

    // Si el bitmap no existe, sal
    if (!bitmap) return;

    // Elimina el bitmap de la memoria
    playdate->graphics->freeBitmap(bitmap);
    bitmap = NULL;

}




