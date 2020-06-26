#include "Player.h"

Player::Player(int16_t x, int16_t y, int16_t nPlayer){
    _x = x;
    _y = y;
	_nPlayer = nPlayer;
	_lives = 10;
}

void Player::move(int16_t y){
    if((y < 0 && _y + y >= 0) ||(y > 0 && _y + y + _h <= 400))
        _y += y;  
}