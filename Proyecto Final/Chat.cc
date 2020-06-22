#include "Chat.h"
#include <string>
#include <stdlib.h>

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    char * d = _data;

    memcpy(d, &type, sizeof(uint8_t));
    d += sizeof(uint8_t);

    for (unsigned int i = 0; i < 7 && i < nick.length(); i++, d++) {
     *d = nick[i];
    }	
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    type = ((uint8_t) *_data);

    char * _nick = _data + sizeof(uint8_t);

    std::string n(_nick, 8);

    nick = n;
    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
		ChatMessage mensj;
		Socket* newSd;
		socket.recv(mensj, newSd);
		switch(mensj.type){
			case ChatMessage::LOGIN:
                if(client1 == nullptr){
                    client1 = newSd;
                    player1 = mensj.nick;                           
                    std::cout << "LOGIN: " << player1 << "\n";
                }
                else if(client2 == nullptr){
                    client2 = newSd;
                    player2 = mensj.nick;                           
                    std::cout << "LOGIN: " << player2 << "\n";
                }
                else{
                    std::cout << "LOBBY IS FULL\n";
                }
			break;         

			case ChatMessage::LOGOUT:     
				if(mensj.nick == player1){
                    client1 = nullptr;
                    player1 = "";                           
                    std::cout << player1 << "logged out\n";
                }
                else if(mensj.nick == player2){
                    client2 = nullptr;
                    player2 = "";                           
                    std::cout << player2 << "logged out\n";
                }
			break;  

            case ChatMessage::MOVEUP:
                if(mensj.nick == player1)
                    game->player1->move(-10);
                else if(mensj.nick == player2)
                    game->player2->move(-10);
            break;  

            case ChatMessage::MOVEDOWN:
                if(mensj.nick == player1)
                    game->player1->move(10);
                else if(mensj.nick == player2)
                    game->player2->move(10);
            break;

            case ChatMessage::SHOOT:
				if (mensj.nick == player1)
				{
					std::cout << player1 << "SHOOT\n";
					game->createBullet(1);
					std::cout<<game->bullets.size() << "\n";
				}
				else if (mensj.nick == player2)
				{
					std::cout << player2 << "SHOOT\n";
					game->createBullet(2);
				}
            break;        

			default:
				break;
		}
    }
}

void ChatServer::update_server(){
    while(true){        
        usleep(10000);

		// creacion de asteroides
		game->asteroidFrames++;
		//if (game->asteroidFrames == game->asteroidMaxFrames)
		int r = rand() % 1000000;
		if(r<10)
		{
			game->asteroidFrames = 0;
			game->createAsteroid();
		}
		
		// COLISIONES Y MOVIMIENTOS

		// balas
		for (int i = 0;i < game->bullets.size();i++)
		{
			// movimiento
			game->bullets[i].move();

			// COLISIONES

			// con bordes
			if (game->bullets[i]._x - 5 > 800 || game->bullets[i]._x + 5 < 0)
				game->bullets.erase(game->bullets.begin() + i);

			// con asteroides
			for (int j = 0;j < game->asteroids.size();j++)
			{
				if (game->asteroids[j]._x - 5 > game->bullets[i]._x - 5 && game->asteroids[j]._x + 5 < game->bullets[i]._x + 5 && game->asteroids[j]._y + 5 < game->bullets[i]._y + 5 && game->asteroids[j]._y - 5 > game->bullets[i]._y - 5)
				{
					game->bullets.erase(game->bullets.begin() + i);
					game->asteroids[j].bulletCollision(game->bullets[i]._nPlayer, 2);
				}
			}
			
			// con jugador 1
			if (game->bullets[i]._nPlayer == 2)
			{
				if (game->bullets[i]._x - 5 > game->player1->_x - 25 && game->bullets[i]._x + 5 < game->player1->_x + 25 && game->bullets[i]._y + 5 < game->player1->_y + 37 && game->bullets[i]._y - 5 > game->player1->_y - 37)
				{
					game->bullets.erase(game->bullets.begin() + i);
					game->player1->_lives--;
					if (game->player1->_lives <= 0)
						std::cout << "GAME OVER. PLAYER 2 WINS";
				}
			}

			// con jugador 2
			if (game->bullets[i]._nPlayer == 1)
			{
				if (game->bullets[i]._x - 5 > game->player2->_x - 25 && game->bullets[i]._x + 5 < game->player2->_x + 25 && game->bullets[i]._y + 5 < game->player2->_y + 37 && game->bullets[i]._y - 5 > game->player2->_y - 37)
				{
					game->bullets.erase(game->bullets.begin() + i);
					game->player2->_lives--;
					if (game->player2->_lives <= 0)
						std::cout << "GAME OVER. PLAYER 1 WINS";
				}
			}
		}

		// asteroides
		for (int i = 0;i < game->asteroids.size();i++)
		{
			// movimiento
			game->asteroids[i].move();

			// COLISIONES

			// con bordes verticales (rebote)
			if (game->asteroids[i]._y + 5 > 400 || game->asteroids[i]._y - 5 < 0)
				game->asteroids[i]._velY *= -1;

			// con bordes horizontales (destruir)
			if (game->asteroids[i]._x - 5 > 800 || game->asteroids[i]._x + 5 < 0)
				game->asteroids.erase(game->asteroids.begin() + i);

			// con jugador 1
			if (game->asteroids[i]._x - 5 > game->player1->_x - 25 && game->asteroids[i]._x + 5 < game->player1->_x + 25 && game->asteroids[i]._y + 5 < game->player1->_y + 37 && game->asteroids[i]._y - 5 > game->player1->_y - 37)
			{
				game->asteroids.erase(game->asteroids.begin() + i);
				game->player1->_lives--;
				if (game->player1->_lives <= 0)
					std::cout << "GAME OVER. PLAYER 2 WINS";
			}

			// con jugador 2
			if (game->asteroids[i]._x - 5 > game->player2->_x - 25 && game->asteroids[i]._x + 5 < game->player2->_x + 25 && game->asteroids[i]._y + 5 < game->player2->_y + 37 && game->asteroids[i]._y - 5 > game->player2->_y - 37)
			{
				game->asteroids.erase(game->asteroids.begin() + i);
				game->player2->_lives--;
				if (game->player2->_lives <= 0)
					std::cout << "GAME OVER. PLAYER 1 WINS";
			}
		}		
		if(client1 != nullptr){           
            socket.send(*game,*client1);
        }
        if(client2 != nullptr)
            socket.send(*game,*client2);
    }        
}

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
	std::string msg;

	ChatMessage em(nick);
    em.type = ChatMessage::LOGOUT;
	
	socket.send(em, socket);
}

void ChatClient::input_thread()
{            
    char c;
    ChatMessage m(nick);
    do{
        c = dpy->wait_key();
        switch(c){
            case 'w':
                m.type = ChatMessage::MOVEUP;
                socket.send(m, socket);
            break;
            case 'd':
                m.type = ChatMessage::MOVEDOWN;
                socket.send(m, socket);
            break;
            case ' ':
				std::cout<<"Bala\n";
                m.type = ChatMessage::SHOOT;
                socket.send(m, socket);
            break;
            case 'q':
                logout();
                exit = true;
            break;
        }
    }while(!exit);
}

void ChatClient::net_thread()
{
    while(!exit)
    {
        socket.recv(*game);        
    }
}

void ChatClient::render_thread()
{
	 while(!exit)
    {
		usleep(1000);
		dpy->clear();

		// RENDERS

		// render jugadores
        dpy->set_color(XLDisplay::BLUE);
        dpy->rectangle(game->player1->_x, game->player1->_y, 50, 75);
        dpy->set_color(XLDisplay::RED);
        dpy->rectangle(game->player2->_x, game->player2->_y, 50, 75);		
		
		//std::cout<<"Client: " << game->bullets.size() <<"\n";	
		// render balas
		for (int i = 0;i < game->bullets.size();i++)
		{				
			if (game->bullets[i]._nPlayer == 1)
				dpy->set_color(XLDisplay::BLUE);
			else
				dpy->set_color(XLDisplay::RED);

			dpy->circle(game->bullets[i]._x, game->bullets[i]._y, 10);
		}

		// render asteroides
		for (int i = 0;i < game->asteroids.size();i++)
		{
			dpy->set_color(XLDisplay::BROWN);
			dpy->circle(game->asteroids[i]._x, game->asteroids[i]._y, 10);
		}
		dpy->flush();
	}
}