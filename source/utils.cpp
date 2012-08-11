/* 
*
* Copyright (C) 2007,2010 Wei Lian <lianwei3 at gmail.com>
*
* This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>


int
connect_to_server (char *host, int portnum)
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

//  int flags = fcntl (sock, F_GETFL, 0);
//  fcntl (sock, F_SETFL, flags | O_NONBLOCK);
   if (::connect
       (sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) == -1) {
      ::close (sock);
      return -1;
   }
   return sock;
}

////search for process name and kill them
//void
//killProcess (char *processName)
//{
//   DIR *dir = opendir ("/proc");
//   if (!dir) {
//      fprintf (stderr, "Cannot open /proc\n");
//      exit (1);
//   }
//   ///loop
//   struct dirent *next;
//   while ((next = readdir (dir)) != NULL) {
//      if (strcmp (next->d_name, "..") == 0)
//	 continue;
//      if (!isdigit (*next->d_name))
//	 continue;
//      char filename[64];
//      sprintf (filename, "/proc/%s/cmdline", next->d_name);
//      FILE *status;
//      if (!(status = fopen (filename, "r"))) {
//	 continue;
//      }
//      int n;
//      char buffer[256];
//      if ((n = fread (buffer, 1, sizeof (buffer), status)) == 0) {
//	 fclose (status);
//	 continue;
//      }
//      fclose (status);
//      ///replace 0 with ' '
//      while (char *ptr = (char *)memchr (buffer, 0, n - 1))
//	 *ptr = ' ';
//      ///find process pid and kill
//      if (strstr (buffer, processName)) {
//	 kill (strtol (next->d_name, NULL, 0), SIGKILL);
//      }
//   }
//   closedir (dir);
//}
