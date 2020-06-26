#include "ClientServer.h"
#include <string>
#include <stdlib.h>

void Message::to_bin()
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

int Message::from_bin(char * bobj)
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

void Server::do_messages()
{
    while (true)
    {
		Message mensj;
		Socket* newSd;
		socket.recv(mensj, newSd);
		switch(mensj.type){
			case Message::LOGIN:
                if(client1 == nullptr){
                    client1 = newSd;
                    player1 = mensj.nick;                           
                    std::cout << "LOGIN: " << player1 << "\n";
					if(client2 !=nullptr)
						game->start = 1;
                }
                else if(client2 == nullptr){
                    client2 = newSd;
                    player2 = mensj.nick;                           
                    std::cout << "LOGIN: " << player2 << "\n";
					if(client1 !=nullptr)
						game->start = 1;
                }
                else{
                    std::cout << "LOBBY IS FULL\n";
                }
			break;         

			case Message::LOGOUT:     
				if(mensj.nick == player1){                           
                    std::cout << "LOGGED OUT: " << player1 << "\n";
					delete newSd;
                    client1 = nullptr;
                    player1 = "";
                }
                else if(mensj.nick == player2){
					std::cout << "LOGGED OUT: " << player2 << "\n";
					delete newSd;
                    client2 = nullptr;
                    player2 = "";                
                }
				game->start = 0;
			break;  

            case Message::MOVEUP:
                if(mensj.nick == player1)
                    game->player1->move(-10);
                else if(mensj.nick == player2)
                    game->player2->move(-10);
            break;  

            case Message::MOVEDOWN:
                if(mensj.nick == player1)
                    game->player1->move(10);
                else if(mensj.nick == player2)
                    game->player2->move(10);
            break;

            case Message::SHOOT:
				if (mensj.nick == player1)
				{
					std::cout << player1 << "SHOOT\n";
					game->createBullet(1);
				}
				else if (mensj.nick == player2)
				{
					std::cout << player2 << "SHOOT\n";
					game->createBullet(2);
				}
            break;

			case Message::RESET:
				game->reset_game();
			break;        

			default:
				break;
		}
    }
}

void Server::bullet_behaviour(){
	// balas
	for (int i = 0;i < game->bullets.size();i++)
	{
		// movimiento
		game->bullets[i].move();

		// COLISIONES

		// con bordes
		if (game->bullets[i]._x - game->bullets[i]._r > 600 || game->bullets[i]._x + game->bullets[i]._r < 0)
			game->bullets.erase(game->bullets.begin() + i);

		// con asteroides
		for (int j = 0;j < game->asteroids.size();j++)
		{
			if (game->asteroids[j]._x - game->asteroids[j]._r < game->bullets[i]._x && game->asteroids[j]._x + game->asteroids[i]._r > game->bullets[i]._x && game->asteroids[j]._y + game->asteroids[i]._r > game->bullets[i]._y && game->asteroids[j]._y - game->asteroids[i]._r < game->bullets[i]._y)
			{
				game->bullets.erase(game->bullets.begin() + i);
				game->asteroids[j].bulletCollision(game->bullets[i]._nPlayer, 2);
			}
		}
		
		// con jugador 1
		if (game->bullets[i]._nPlayer == 2)
		{
			if (game->bullets[i]._x > game->player1->_x && game->bullets[i]._x < game->player1->_x + game->player1->_w && game->bullets[i]._y > game->player1->_y && game->bullets[i]._y < game->player1->_y + game->player1->_h)
			{
				game->bullets.erase(game->bullets.begin() + i);
				game->player1->_lives--;
			}
		}

		// con jugador 2
		if (game->bullets[i]._nPlayer == 1)
		{
			if (game->bullets[i]._x > game->player2->_x && game->bullets[i]._x < game->player2->_x + game->player2->_w && game->bullets[i]._y > game->player2->_y && game->bullets[i]._y < game->player2->_y + game->player2->_h)
			{
				game->bullets.erase(game->bullets.begin() + i);
				game->player2->_lives--;
			}
		}
	}
}

void Server::asteroid_behaviour(){
	// asteroides
	for (int i = 0;i < game->asteroids.size();i++)
	{
		// movimiento
		game->asteroids[i].move();

		// COLISIONES

		// con bordes verticales (rebote)
		if (game->asteroids[i]._y + game->asteroids[i]._r > 400 || game->asteroids[i]._y - game->asteroids[i]._r < 0)
			game->asteroids[i]._velY *= -1;

		// con bordes horizontales (rebote)
		if (game->asteroids[i]._x - 5 > 600 || game->asteroids[i]._x + 5 < 0)
			game->asteroids[i]._velX *= -1;
		
		// con jugador 1
		if (game->asteroids[i]._x >= game->player1->_x && game->asteroids[i]._x <= game->player1->_x + game->player1->_w && game->asteroids[i]._y >= game->player1->_y && game->asteroids[i]._y <= game->player1->_y + game->player1->_h)
		{
			if(game->asteroids[i]._y == game->player1->_y + game->player1->_h || game->asteroids[i]._y == game->player1->_y)
				game->asteroids[i]._velY *= -1;
			else
				game->asteroids[i]._velX *= -1;
			game->player1->_lives -= 2;
		}

		// con jugador 2
		if (game->asteroids[i]._x >= game->player2->_x && game->asteroids[i]._x <= game->player2->_x + game->player2->_w && game->asteroids[i]._y >= game->player2->_y && game->asteroids[i]._y <= game->player2->_y + game->player2->_h)
		{
			game->asteroids[i]._velX *= -1;
			if((game->asteroids[i]._y == game->player2->_y + game->player2->_h) || game->asteroids[i]._y == game->player2->_y)
				game->asteroids[i]._velY *= -1;
			game->player2->_lives -= 2;
		}
	}
}

void Server::update_server(){
    while(true){        
        usleep(10000);

		if(!game->end_game() && game->start_game()){
			// creacion de asteroides
			int r = rand() % 1000000;
			if(game->asteroids.size() < 3 && r<100)
				game->createAsteroid();
			
			// COLISIONES Y MOVIMIENTOS

			bullet_behaviour();
			asteroid_behaviour();
		}
				
		if(client1 != nullptr){           
            socket.send(*game,*client1);
        }
        if(client2 != nullptr)
            socket.send(*game,*client2);
    }        
}

void Client::login()
{
    std::string msg;

    Message em(nick);
    em.type = Message::LOGIN;

    socket.send(em, socket);
}

void Client::logout()
{
	std::string msg;

	Message em(nick);
    em.type = Message::LOGOUT;
	
	socket.send(em, socket);
}

void Client::input_thread()
{            
    char c;
    Message m(nick);
    do{
        c = dpy->wait_key();
        switch(c){
            case 'w':
				if(game->start_game() && !game->end_game()){
					m.type = Message::MOVEUP;
					socket.send(m, socket);
				}
            break;
            case 'd':
				if(game->start_game() && !game->end_game()){
                	m.type = Message::MOVEDOWN;
                	socket.send(m, socket);
				}
            break;
            case ' ':
				if(game->start_game() && !game->end_game()){
                	m.type = Message::SHOOT;
                	socket.send(m, socket);
				}
            break;
            case 'q':
                logout();
                exit = true;
            break;

			case 'p':
				if(game->end_game()){
					std::cout<<"REINICIO\n";
					m.type = Message::RESET;
                	socket.send(m, socket);
				}
            break;
        }
    }while(!exit);
}

void Client::net_thread()
{
    while(!exit)
    {
        socket.recv(*game);        
    }
}

void Client::render_thread()
{
	while(!exit)
    {
		usleep(10000);
		dpy->clear();

		// RENDERS
		if(game->start_game()){
			if(!game->end_game()){

				// render jugadores
				dpy->set_color(XLDisplay::BLUE);
				dpy->rectangle(game->player1->_x, game->player1->_y, game->player1->_w, game->player1->_h);
				dpy->set_color(XLDisplay::RED);
				dpy->rectangle(game->player2->_x, game->player2->_y, game->player1->_w, game->player1->_h);		
							
				// render balas
				for (int i = 0;i < game->bullets.size();i++)
				{			
					if (game->bullets[i]._nPlayer == 1)
						dpy->set_color(XLDisplay::BLUE);
					else
						dpy->set_color(XLDisplay::RED);

					dpy->circle(game->bullets[i]._x, game->bullets[i]._y, game->bullets[i]._r);
				}

				// render asteroides
				for (int i = 0;i < game->asteroids.size();i++)
				{
					dpy->set_color(XLDisplay::BROWN);
					dpy->circle(game->asteroids[i]._x, game->asteroids[i]._y, game->asteroids[i]._r);
				}
				
				//Vidas
				dpy->set_color(XLDisplay::BROWN);
				std::string p1 = "Jugador 1: " + std::to_string(game->player1->_lives);
				std::string p2 = "Jugador 2: " + std::to_string(game->player2->_lives);
				dpy->text(50, 10, p1);
				dpy->text(500, 10, p2);
			}
			else{ //Cuando termina la partida
				dpy->set_color(XLDisplay::BROWN);
				std::string win;
				if(game->player1->_lives <= 0)
					win = "VICTORIA JUGADOR 2";
				else if(game->player2->_lives <= 0)
					win = "VICTORIA JUGADOR 1";
				dpy->text(250,200, win);
				dpy->text(200,250, "Pulsa P para comenzar una nueva partida");
			}
		}
		else{ //Cuando se espera a que entren dos jugadores
			dpy->set_color(XLDisplay::BROWN);
			dpy->text(200,200, "Esperando al otro jugador...");
		}
		dpy->flush();
	}
}