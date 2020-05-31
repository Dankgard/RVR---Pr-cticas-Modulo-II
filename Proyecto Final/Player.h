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
        Player(int x, int y);

        XLDisplay& render(int nPlayer);        
        void shoot(int nPlayer);        
};
