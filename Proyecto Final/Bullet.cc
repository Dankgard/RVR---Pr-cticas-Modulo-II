#include "Bullet.h"

Bullet::Bullet(int16_t x, int16_t y, int16_t nPlayer){
    _x = x;
    _y = y;
	_nPlayer = nPlayer;

	_velY = 0;
	if (_nPlayer == 1)
		_velX = 4;
	else
		_velX = -4;

}

void Bullet::move()
{
	_x += _velX;
	_y += _velY;
}
