#include "Asteroid.h"

Asteroid::Asteroid(int16_t x, int16_t y, int16_t vY){
    _x = x;
    _y = y; //Posicion en y alzar

	_velX = 0;
    _velY = vY;
}

XLDisplay& Asteroid::render(){
    XLDisplay& disp = XLDisplay::display();
    disp.set_color(XLDisplay::BROWN);
    disp.circle(_x, _y, 10);
    return disp;
}

void Asteroid::move()
{
	_x += _velX;
	_y += _velY;
}

void Asteroid::bulletCollision(int16_t nPlayer, int16_t vX)
{
	_velX *= -1;
}
