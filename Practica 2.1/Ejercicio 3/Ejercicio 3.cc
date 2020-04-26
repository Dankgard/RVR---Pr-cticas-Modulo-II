#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int
main (int argc, char **argv)
{
  struct addrinfo hints;
  struct addrinfo *res;

// ---------------------------------------------------------------------- //
// INICIALIZACION SOCKET & BIND //
// ---------------------------------------------------------------------- //

  memset (&hints, 0, sizeof (struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  int rc = getaddrinfo (argv[1], argv[2], &hints, &res);

  if (rc != 0)
    {
      printf ("getaddrinfo:%s\n", gai_strerror (rc));
      return -1;
    }
// res contiene la representaciC3n como sockaddr de direcciC3n + puerto

  int sd = socket (res->ai_family, res->ai_socktype, res->ai_protocol);

  freeaddrinfo (res);


  char buffer[80];
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  struct sockaddr client_addr;
  socklen_t client_len = sizeof (struct sockaddr);

  //Envio instruccion al servidor
  sendto(sd, argv[3], strlen(argv[3]), 0, res->ai_addr, res->ai_addrlen);
  //Recibo la informacion del servidor
  recvfrom (sd, buffer, 79 * sizeof (char), 0, res->ai_addr, &res->ai_addrlen);

  //Escribo en el cliente lo que me ha llegado del servidor
  printf ("%s\n", buffer);  

  return 0;
}