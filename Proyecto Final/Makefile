CC=g++
CFLAGS=-g -I.
DEPS = Socket.h Game.h Chat.h XLDisplay.h Player.h Bullet.h Asteroid.h
OBJ = Socket.o Game.o Chat.o XLDisplay.o Player.o Bullet.o Asteroid.o
LIBS=-lpthread -lm -lX11

%.o: %.cc $(DEPS)
	$(CC) -std=c++11 -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) ChatServer.o
	g++ -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -std=c++11 -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

