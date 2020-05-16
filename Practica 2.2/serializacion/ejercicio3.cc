#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <fstream>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, MAX_NAME);
    };

    virtual ~Jugador(){};

    void to_bin()
    {	
	int32_t size = sizeof(char) * MAX_NAME + sizeof(int16_t) + sizeof(int16_t);
        alloc_data(size);

	memcpy(_data, (void*)&name, MAX_NAME * sizeof(char));
	_data += MAX_NAME * sizeof(char);
	memcpy(_data, (void*)&x, sizeof(int16_t));
	_data += sizeof(int16_t);
	memcpy(_data, (void*)&y, sizeof(int16_t));
	_data += sizeof(int16_t);
	_data -= size;
    }

    int from_bin(char * data)
    {  
	memcpy((void*)&name, _data, MAX_NAME * sizeof(char));
	_data += MAX_NAME * sizeof(char);
	memcpy((void*)&x, _data, sizeof(int16_t));
	_data += sizeof(int16_t);
	memcpy((void*)&y, _data, sizeof(int16_t));
	_data += sizeof(int16_t);
        return 0;
    }

public:
    static const size_t MAX_NAME = 20;

    char name[MAX_NAME];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{       
    Jugador one_w("Jugador dos", 123, 987);

    //Serializar y escribir one_w en un fichero
	one_w.to_bin();

	std::fstream f;
	f.open("Prueba3", std::fstream::out);
	f.write(one_w.data(), one_w.size());

	size_t s = one_w.size();	
	char* archivo;

	f.read(archivo,s);
	printf("Name: %s \nPosition: X: %d Y: %d\n", one_w.name, one_w.x, one_w.y);

    f.close();
    return 0;
}
