#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"
#include "Player.h"

class Game : public Serializable {
public:
    static const size_t SIZE = sizeof(int16_t) * 4;
    Player player1;
    Player player2;
    Game(int16_t x1,int16_t y1, int16_t x2, int16_t y2);
    void to_bin();
    int from_bim(char * bobj);
};
