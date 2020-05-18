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

    d += 7 * sizeof(char);

	for (unsigned int i = 0; i < message.length(); i++, d++) {
	 *d = message[i];
	}

	d = _data + sizeof(uint8_t) + 8* sizeof(char);
	
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    type = ((uint8_t) *_data);

    char * _nick = _data + sizeof(uint8_t);
    char * _msg = _nick + sizeof(char) * 8;

    std::string n(_nick, 8);
    std::string m(_msg, 80);

    message = m;
    nick = n;
    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
		ChatMessage mensj;
		Socket* newSd;
		socket.recv(mensj, newSd);
		switch(mensj.type){
			case ChatMessage::LOGIN:                
				clients.push_back(newSd);                            
                std::cout << "LOGIN: " << *newSd << "\n";
			break;
            

			case ChatMessage::MESSAGE:   
				for(auto it = clients.begin(); it != clients.end(); ++it){
					if(!(*(*it) == *newSd)){
						socket.send(mensj, *(*it));
					}
				}         
                std::cout << "MESSAGE: " << *newSd << "\n";
			break;            

			case ChatMessage::LOGOUT:               

                std::cout << "LOGOUT: " << *newSd << "\n";         
				for (auto it = clients.begin(); it != clients.end(); ++it) {
                    if (*(*it) == *newSd) {
                        delete *it;
                        clients.erase(it);
                        break;
                    }
                }
			break;            

			default:
				break;
		}
    }
}

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
	std::string msg;

	ChatMessage em(nick, msg);
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
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
		ChatMessage recibido;
		socket.recv(recibido);

		if(recibido.type == ChatMessage::MESSAGE)
			std::cout <<"[ "<< recibido.nick << " ]: " << recibido.message << "\n";
    }
}