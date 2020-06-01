#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <iostream>
#include "XLDisplay.h"

class Bullet{
    public:
        int _x;
        int _y;
		int _nPlayer;

		int _velX;
		int _velY;
        Bullet(int x, int y, int _nPlayer);

        XLDisplay& render();
		void move();
};
