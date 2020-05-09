#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>

class Instruccion{
	public:
		int socket;
		Instruccion(int sock_){ socket = sock_; }

		void tmensaje(char mensj, struct sockaddr client_addr,
		socklen_t client_len){
			char tiempo[80];
			struct tm* local;
			time_t t = time(NULL);
			local = localtime(&t);

			printf("Thread %d\n", std::this_thread::get_id());

			if (mensj == 't') {
				
				local=localtime(&t);
				ssize_t ss = strftime(tiempo,sizeof(tiempo),"%r",local);
				sendto(socket, tiempo,sizeof(char)* ss,0,&client_addr,client_len);
			}
			else if (mensj == 'd') {
				local=localtime(&t);
				ssize_t ss = strftime(tiempo, sizeof(tiempo), "%Y-%m-%d", local);
				sendto(socket, tiempo, sizeof(char) * ss, 0, &client_addr, client_len);
			}
			else {
				printf("Comando no soportado %c", mensj);
			}
		}
};

int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo * res;

	// ---------------------------------------------------------------------- //
	// INICIALIZACIÓN SOCKET & BIND //
	// ---------------------------------------------------------------------- //

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if ( rc != 0 )
	{
		printf("getaddrinfo: %s \n", gai_strerror(rc));
		return -1;
	}

	// res contiene la representación como sockaddr de dirección + puerto

	int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
	{
		printf("bind: \n");
		return -1;
	}

	freeaddrinfo(res);

	// ---------------------------------------------------------------------- //
	// RECEPCIÓN MENSAJE DE CLIENTE //
	// ---------------------------------------------------------------------- //
	char buffer[80];
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	struct sockaddr client_addr;
	socklen_t client_len = sizeof(struct sockaddr);
	
	
	std::vector<std::thread> thr;
	bool exit = false;
	while(!exit){
		ssize_t bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr,
		&client_len);

		getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
		NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		printf("%d bytes de %s:%s\n", bytes, host, service);

		if(buffer[0] == 'q'){
			for(auto &t: thr){
				return 0;
			}
		}

		sleep(3);
		Instruccion i(sd);
		thr.push_back(std::thread(&Instruccion::tmensaje, &i, buffer[0], 
			client_addr, client_len));
		thr.back().join();
	}
	close(sd);
}