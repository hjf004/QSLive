#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>

int connect_to_server (char *host, int portnum)
{
   int sock;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   sock =::socket (AF_INET, SOCK_STREAM, 0);
   if (sock == -1) {
      perror ("ERROR opening socket");
      return -3;
   }
   server =::gethostbyname (host);
   if (server == NULL) {
      perror ("ERROR, no such host");
      close (sock);
      return -2;
   }
   ::memset ((char *) &serv_addr, 0, sizeof (serv_addr));
   serv_addr.sin_family = AF_INET;
   ::memcpy ((char *) &serv_addr.sin_addr.s_addr,
	     (char *) server->h_addr, server->h_length);
   serv_addr.sin_port = htons (portnum);


   if (::connect
       (sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) == -1) {
      ::close (sock);
      return -1;
   }
   return sock;
}
