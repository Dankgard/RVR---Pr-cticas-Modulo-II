#include "Chat.h"
#include <string>

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
                    game->player1.move(-10);
                else if(mensj.nick == player2)
                    game->player.move(-10);
            break;  

            case ChatMessage::MOVEDOWN:
                if(mensj.nick == player1)
                    game->player1.move(10);
                else if(mensj.nick == player2)
                    game->player.move(10);
            break;

            case ChatMessage::SHOOT:
                if(mensj.nick == player1)
                    std::cout << player1 << "SHOOT\n";
                else if(mensj.nick == player2)
                    std::cout << player2 << "SHOOT\n";
            break;        

			default:
				break;
		}
    }
}

void ChatServer::update_server(){
    while(true){
        usleep(10000);
        if(client1 != nullptr)
            socket.send(*game,*client1);
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
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
		std::string m;
		std::getline(std::cin,m);	

        ChatMessage chat(nick,m);

        if(m == "q" || m == "Q"){
            chat.type = ChatMessage::LOGOUT;
            socket.send(chat,socket);
            break;
        }
		
		chat.type = ChatMessage::MESSAGE;

		socket.send(chat,socket);
    }
}

void ChatClient::net_thread()
{
    while(!exit)
    {
        socket.recv(*game);
        dpy.clear();
        dpy = game->player1.render();
        dpy = game->player2.render();
    }
}