#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <iostream>
#include "XLDisplay.h"

class Player{
    public:
        int16_t _x;
        int16_t _y;
		int16_t _nPlayer;
        Player(int16_t x, int16_t y, int16_t _nPlayer);

        XLDisplay& render();        
        void shoot();       
        void move(int16_t y); 
};
