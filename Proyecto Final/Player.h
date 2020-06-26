#include <string>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include "XLDisplay.h"

class Player{
    public:
        int16_t _x;
        int16_t _y;
        int16_t _w = 50;
        int16_t _h = 75;
		int16_t _nPlayer;
		int16_t _lives;
        Player(int16_t x, int16_t y, int16_t _nPlayer);
     
        void move(int16_t y); 
};
