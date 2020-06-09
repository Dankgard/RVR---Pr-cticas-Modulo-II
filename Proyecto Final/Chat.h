#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "XLDisplay.h"
#include "Game.h"


class ChatMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);

    enum MessageType
    {
        LOGIN   = 0,
        LOGOUT  = 1,
        MOVEUP = 2,
        MOVEDOWN = 3,
        SHOOT = 4
    };

    ChatMessage(){};

    ChatMessage(const std::string& n):nick(n){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;

    std::string nick;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el servidor de chat
 */
class ChatServer
{
public:
    ChatServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
        game = new Game(0, 100, 500, 300);
    };

    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();
    void update_server();

private:
    /**
     *  Lista de clientes conectados al servidor de Chat, representados por
     *  su socket
     */    
    Socket* client1 = nullptr;
    Socket* client2 = nullptr;

    /**
     * Socket del servidor
     */
    Socket socket;
    Game* game;
    std::string player1;
    std::string player2;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el cliente de chat
 */
class ChatClient
{
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
    ChatClient(const char * s, const char * p, const char * n):socket(s, p), nick(n){
        game = new Game(0, 100, 500, 300);
        dpy = &XLDisplay::display();
    };

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

    /**
     *  Rutina principal para el Thread de E/S. Lee datos de STDIN (std::getline)
     *  y los envía por red vía el Socket.
     */
    void input_thread();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void net_thread();
    /**
     * Socket para comunicar con el servidor
     */
    Socket socket;

    /**
     * Nick del usuario
     */
    std::string nick;
    bool exit = false;
    Game* game;
    XLDisplay* dpy;
};

