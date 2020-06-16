#include "Game.h"
#include <string>

Game::Game(int16_t x1,int16_t y1, int16_t x2, int16_t y2){
    player1 = new Player(x1,y1,1);
    player2 = new Player(x2,y2,2);
}

void Game::to_bin(){
    size_t bSize= bullets.size() * sizeof(int16_t) * 2;
    alloc_data(SIZE);
    char * d = _data;
    memcpy(d, &player1->_x, sizeof(int16_t));                                          
    d += sizeof(int16_t);
    memcpy(d, &player1->_y, sizeof(int16_t));                                          
    d += sizeof(int16_t);
    memcpy(d, &player2->_x, sizeof(int16_t));                                          
    d += sizeof(int16_t);
    memcpy(d, &player2->_y, sizeof(int16_t));                                          
    d += sizeof(int16_t);

    int16_t tam = bullets.size();
    memcpy(d, &tam, sizeof(int16_t));

    for (int i = 0; i < bullets.size(); i++) {
        d += sizeof(int16_t); 
        memcpy(d, &bullets[i]->_x, sizeof(int16_t));
        d += sizeof(int16_t); 
        memcpy(d, &bullets[i]->_y, sizeof(int16_t));
    }
}

int Game::from_bin(char * bobj){
    memcpy(&player1->_x, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&player1->_y, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&player2->_x, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);
    memcpy(&player2->_y, bobj, sizeof(int16_t));
    bobj += sizeof(int16_t);

    int16_t t;
    memcpy(&t, bobj, sizeof(int16_t));
    bullets.clear();

    for (int i = 0; i < t; i++) {
        int16_t _x;
        int16_t _y;
        bobj += sizeof(int16_t); 
        memcpy(&_x, bobj, sizeof(int16_t));
        bobj += sizeof(int16_t); 
        memcpy(&_y, bobj, sizeof(int16_t));
        Bullet* bull = new Bullet(_x, _y, 1);
        bullets.push_back(bull);
    }

    return 0;
}

void Game::createBullet(int16_t nPlayer)
{
	if (nPlayer == 1){
		Bullet* bullet = new Bullet(player1->_x, player1->_y, nPlayer);
        bullets.push_back(bullet);
    }
	else{
        Bullet* bullet2 = new Bullet(player2->_x, player2->_y, nPlayer);
        bullets.push_back(bullet2);
    }	
}

void Game::createAsteroid()
{
	Asteroid* asteroid = new Asteroid(2);
	asteroids.push_back(asteroid);
}

