#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"
#include "Player.h"
#include "Bullet.h"
#include "Asteroid.h"

class Game : public Serializable {
public:
    Game(int16_t x1,int16_t y1, int16_t x2, int16_t y2);
    static const size_t SIZE = sizeof(int16_t) * 4;
    Player* player1;
    Player* player2;
	std::vector<Bullet> bullets;
	std::vector<Asteroid> asteroids;
	int16_t asteroidFrames = 0;
	int16_t asteroidMaxFrames = 180;
    void to_bin();
    int from_bin(char * bobj);
	void createBullet(int16_t nPlayer);
	void createAsteroid();
};
