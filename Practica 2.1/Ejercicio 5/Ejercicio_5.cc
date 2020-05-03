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
  // INICIALIZACIÓN SOCKET & BIND //
  // ---------------------------------------------------------------------- //

  memset (&hints, 0, sizeof (struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int rc = getaddrinfo (argv[1], argv[2], &hints, &res);

  if (rc != 0)
    {
      printf ("getaddrinfo: %s \n", gai_strerror (rc));
      return -1;
    }

  int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  freeaddrinfo (res);

// ---------------------------------------------------------------------- //
// GESTION DE LAS CONEXIONES AL SERVIDOR //
// ---------------------------------------------------------------------- //
  struct sockaddr client_addr;
  socklen_t client_len = sizeof (struct sockaddr);

  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  //Conectando con el servidor
  connect(sd, res->ai_addr, res->ai_addrlen);

  getnameinfo (&client_addr, client_len, host, NI_MAXHOST, service,
           NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

// ---------------------------------------------------------------------- //
// GESTION DE LA CONEXION CLIENTE //
// ---------------------------------------------------------------------- //
  char buffer[80];
  bool salir = false;
  while(!salir)
  {
	  scanf("%s", buffer);

	  if (buffer[0] == 'Q')
		salir=true;
          else{	  
                //Envio instruccion al servidor
		send(sd, buffer,  79 * sizeof (char), 0);
                //Recibo la informacion del servidor
		recv(sd, buffer,  79 * sizeof (char), 0);
                //Escribo en el cliente lo que me ha llegado del servidor
		printf ("%s \n", buffer);
	  }
  }
  return 0;
}