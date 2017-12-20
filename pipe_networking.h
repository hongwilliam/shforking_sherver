#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#ifndef NETWORKING_H
#define NETWORKING_H

#define ACK "HOLA"
#define HANDSHAKE_BUFFER_SIZE 100
#define BUFFER_SIZE 1000
#define WKP "WKP"

int print_errno();
int server_handshake(int *to_client);
int client_handshake(int *to_server);










int server_setup();
int server_connect(int from_client);

#endif
