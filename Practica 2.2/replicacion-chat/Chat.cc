#include "Chat.h"

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);
	
    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
	memcpy(_data, (void*)&type, sizeof(uint8_t));
	_data += sizeof(uint8_t);
	memcpy(_data, (void*)&nick, sizeof(nick));
	_data += sizeof(nick);
	memcpy(_data, (void*)&message, sizeof(message));
	_data += sizeof(message);
	_data -= MESSAGE_SIZE;
	
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
	memcpy((void*)&type, _data, sizeof(uint8_t));
	_data += sizeof(uint8_t);
	memcpy((void*)&nick, _data, sizeof(nick));
	_data += sizeof(nick);
	memcpy((void*)&message, _data, sizeof(message));
	_data += sizeof(message);
	_data -= MESSAGE_SIZE;

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
			case 0:
				clients.push_back(newSd);
			break;

			case 1:
				for(auto s : clients){
					if(s != newSd){
						socket.send(mensj, *s);
					}
				}
			break;

			case 2:
				int i = 0;
				bool encontrado = false;
				
				while(!encontrado && i != clients.size()){
					if(clients[i] == newSd){
						clients.erase(clients.begin() + i);
						encontrado =true;
					}
					else i++;
				}
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
			std::cout << recibido.nick << ": " << recibido.message << "\n";
    }
}