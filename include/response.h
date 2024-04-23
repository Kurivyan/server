#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DIRNAME "../src/"
#define HEADER_PRESIZE 200
#define STATIC_SRC "src/"

struct response_body
{
    char *header;
    int status;
    int contentsize;
    char *content;
};

struct response_body *create(int, int, const char *);

void home_page_render(int, char *);

void not_found_page_render(int, char *);

void fav_send(int);