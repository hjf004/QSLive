

#ifndef UTILS_H
#define UTILS_H
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>

int connect_to_server(char *host, int portnum);
//void killProcess (char *processName);

#endif
