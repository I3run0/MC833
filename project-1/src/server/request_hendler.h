#ifndef REQUEST_HENDLER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "process_request.h"

#define BUFFERLEN 1024
#define ENDTOKEN "\n\n"

void request_hendler(int socket);

#endif /*REQUEST_HENDLER*/
