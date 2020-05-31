#include "Player.h"

Player::Player(int x, int y){
    _x = x;
    _y = y;
}

XLDisplay& Player::render(int nPlayer){
    XLDisplay& disp = XLDisplay::display();
    if(nPlayer==1)
        disp.set_color(XLDisplay::BLUE);
    else
        disp.set_color(XLDisplay::RED);

    disp.rectangle(_x, _y, 50, 75);
    return disp;
}

void Player::shoot(int nPlayer){
    if(nPlayer == 1)
        std::cout << "Disparo: " << _x << " " << _y << " speed: " << "5" "\n";
    else if(nPlayer == 2)
        std::cout << "Disparo: " << _x << " " << _y << " speed: " << "-5" "\n";
}