#include "Game.h"
#include <string>

Game::Game(int16_t x1,int16_t y1, int16_t x2, int16_t y2){
    player1 = new Player(x1,y1,1);
    player2 = new Player(x2,y2,2);
}

void Game::to_bin(){
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
    return 0;
}

