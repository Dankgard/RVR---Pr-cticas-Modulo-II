#include <string>
#include <unistd.h>
#include <string.h>
#include "Chat.h"

#include "XLDisplay.h"

extern "C" void * _client_thread(void *arg)
{
    ChatClient * server = static_cast<ChatClient *>(arg);

    server->net_thread();

    return 0;
}

int main(int argc, char **argv)
{
    ChatClient ec(argv[1], argv[2], argv[3]);

    pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&id, &attr, _client_thread, static_cast<void *>(&ec));

    ec.login();

    std::cout << "ENTRA AL RENDERIZADO\n";
    XLDisplay::init(800, 400, "Jugador1");

    Player player1(0,100);

    XLDisplay& dpy = player1.render(1, 0);

    dpy.flush();

    char k;
    int x=0;

    do {
        k = dpy.wait_key();
        switch(k){
            case 'w':
                x++;
                dpy.clear();
                XLDisplay& dpy = player1.render(1, x);
                dpy.flush();
            break;
        }
    } while (k != 'q');

    //dpy.clear();

    //dpy.flush();

    sleep(1);
}

