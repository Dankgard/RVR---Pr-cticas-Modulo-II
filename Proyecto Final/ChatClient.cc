#include <string>
#include <unistd.h>
#include <string.h>
#include "Chat.h"

#include "XLDisplay.h"
#include "Player.h"

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
    XLDisplay::init(800, 400, "Space Showdown");

    Player player1(0,100);

    XLDisplay& dpy = player1.render(1);

    dpy.flush();

    char k;
    int x=0;

    do {
        k = dpy.wait_key();
        switch(k) {
            case 'w':
            player1._y -= 1;
            break;
            case 'd':
            player1._y += 1;
            break;
            case 'a':
            player1.shoot(1);
            break;
        }
        dpy.clear();
        XLDisplay& dpy = player1.render(1);
        dpy.flush();
    } while (k != 'q');

    dpy.clear();

    dpy.flush();

    sleep(1);
}

