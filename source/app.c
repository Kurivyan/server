#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "req_parser.h"
#include "res_creator.h"
#include "socket.h"

#define PORT 3000

int main()
{
    app_listen(PORT);
    app_accept_connections();
    printf("Server started on : %d\n", PORT);
    return 0;
}
