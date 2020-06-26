CC=g++
CFLAGS=-g -I. -std=c++11
DEPS = Socket.h Game.h ClientServer.h XLDisplay.h Player.h Bullet.h Asteroid.h
OBJ = Socket.o Game.o ClientServer.o XLDisplay.o Player.o Bullet.o Asteroid.o
LIBS=-lpthread -lm -lX11 -std=c++11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: server client

server: $(OBJ) SpaceShodownServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ) SpaceShodownClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o server client

