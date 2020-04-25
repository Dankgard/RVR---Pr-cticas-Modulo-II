#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

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
printf("getaddrinfo:%s\n",gai_strerror(rc));
return -1;
}
// res contiene la representación como sockaddr de dirección + puerto

int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
{
printf("bind:\n");
return -1;
}

freeaddrinfo(res);
// ---------------------------------------------------------------------- //
// RECEPCIÓN MENSAJE DE CLIENTE //
// ---------------------------------------------------------------------- //
char buffer[80];
char host[NI_MAXHOST];
char service[NI_MAXSERV];
//Time
struct tm* local;
time_t t = time(NULL);
char tiempo[80];

struct sockaddr client_addr;
socklen_t client_len = sizeof(struct sockaddr);

bool exit=false;
while(!exit){
ssize_t bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr,
&client_len);

if ( bytes == -1){
printf("recvfrom:\n");
return -1;
}

getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

printf("%d bytes de %s:%s \n", bytes, host, service);

if(buffer[0]=='t'){
local=localtime(&t);
ssize_t ss = strftime(tiempo,sizeof(tiempo),"%r",local);
sendto(sd, tiempo,sizeof(char)* ss,0,&client_addr,client_len);
}
else if(buffer[0]=='d'){
local=localtime(&t);
ssize_t ss = strftime(tiempo, sizeof(tiempo), "%Y-%m-%d", local);
sendto(sd,tiempo, sizeof(char) * ss, 0, &client_addr, client_len);
}
else if(buffer[0]=='q'){
printf("Saliendo...\n");
exit=true;
}
else{
printf("Comando no soportado %s \n", buffer);
}
}
return 0;
}