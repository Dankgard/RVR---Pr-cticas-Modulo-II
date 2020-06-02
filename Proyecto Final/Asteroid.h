#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <iostream>
#include "XLDisplay.h"

class Asteroid{
    public:
        int _x;
        int _y;

		int _velX;
		int _velY;
        Asteroid(int vY);

        XLDisplay& render();
		void move();
        void bulletCollision(int nPlayerm, int vX);
};
