#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

int
main (int argc, char **argv)
{
  struct addrinfo hints;
  struct addrinfo *res;
  char addrstr[100];
  void *ptr;

  memset (&hints, 0, sizeof (struct addrinfo));

  hints.ai_family = AF_UNSPEC;

  int rc = getaddrinfo (argv[1], argv[2], &hints, &res);

  if (rc != 0)
    {
      printf ("getaddrinfo: %s \n", gai_strerror (rc));
      return -1;
    }
  while (res)
    {
      inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

      if (res->ai_family == AF_INET)
	{
	  ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
	}
      else if (res->ai_family == AF_INET6)
	{
	  ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
	}

      inet_ntop (res->ai_family, ptr, addrstr, 100);

      char host[NI_MAXHOST];
      char service[NI_MAXSERV];

      getnameinfo (res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, service,
		   NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

      printf ("%s %d    %d\n", addrstr, res->ai_family, res->ai_socktype);
      res = res->ai_next;
    }


  freeaddrinfo (res);

  return 0;
}