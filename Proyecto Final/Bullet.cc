#include "Bullet.h"

Bullet::Bullet(int x, int y, int nPlayer){
    _x = x;
    _y = y;
	_nPlayer = nPlayer;

	_vy = 0;
	if (nPlayer == 1)
		_vx = 1;
	else
		_vx = -1;

}

XLDisplay& Bullet::render(){
    XLDisplay& disp = XLDisplay::display();
    if(nPlayer==1)
        disp.set_color(XLDisplay::BLUE);
    else
        disp.set_color(XLDisplay::RED);

    disp.circle(_x, _y, 10);
    return disp;
}

void Bullet::move()
{
	_x += _vx;
	_y += _vy;
}
