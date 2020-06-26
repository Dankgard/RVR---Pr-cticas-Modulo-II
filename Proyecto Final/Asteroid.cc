#include "Asteroid.h"

Asteroid::Asteroid(int16_t x, int16_t y, int16_t vY){
    _x = x;
    _y = y;

	_velX = 0;
    _velY = vY;
}

void Asteroid::move()
{
	_x += _velX;
	_y += _velY;
}

//Cambio de direccion cuando colisiona con la bala
void Asteroid::bulletCollision(int16_t nPlayer, int16_t vX)
{
	if(nPlayer == 1)
        _velX = vX;
    else if(nPlayer == 2)
        _velX = -vX;
}
