#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <iostream>
#include "XLDisplay.h"

class Bullet{
    public:
        int16_t _x;
        int16_t _y;
		int16_t _nPlayer;

		int16_t _velX;
		int16_t _velY;
        Bullet(int16_t x, int16_t y, int16_t _nPlayer);

        XLDisplay& render();
		void move();
};
