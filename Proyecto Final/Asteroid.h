#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <iostream>
#include "XLDisplay.h"

class Asteroid{
    public:
        int16_t _x;
        int16_t _y;

		int16_t _velX;
		int16_t _velY;
        Asteroid(int16_t x, int16_t y, int16_t vY);

        XLDisplay& render();
		void move();
        void bulletCollision(int16_t nPlayerm, int16_t vX);
};
