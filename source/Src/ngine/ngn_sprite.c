/******************************************************************************

    [N'gine for Playdate SDK]: Gestion de sprites
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
#include "ngn_sprite.h"




/*** Crea un nuevo sprite en base a la imagen proporcionada ***/
LCDSprite* NGN_Sprite_Create(LCDBitmap* img, float x, float y) {

    // Si la imagen es invalida, sal
    if (!img) return NULL;

    // Crea el sprite
    LCDSprite* spr = playdate->sprite->newSprite();
    // Asignale su imagen
    playdate->sprite->setImage(spr, img, kBitmapUnflipped);
    // Colocalo en sus coordenadas
    playdate->sprite->moveTo(spr, x, y);
    // Y registralo en el displaylist
    playdate->sprite->addSprite(spr);

    // Devuelve el sprite creado
    return spr;

}



/*** Elimina el sprite indicado ***/
void NGN_Sprite_Delete(LCDSprite* spr) {

    // Si el sprite no es valido, sal
    if (!spr) return;

    // Quita el sprite del display list
    playdate->sprite->removeSprite(spr);

    // Elimina el sprite
    playdate->sprite->freeSprite(spr);
    spr = NULL;

}
