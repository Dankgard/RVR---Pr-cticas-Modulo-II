#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <iostream>
#include "XLDisplay.h"

class Player{
    public:
        int _x;
        int _y;
		int _nPlayer;
        Player(int x, int y, int _nPlayer);

        XLDisplay& render();        
        void shoot();        
};