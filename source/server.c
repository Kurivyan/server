#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "url_parser.h"
#include "res_creator.h"
#include "app.h"

#define PORT 3000

int main()
{
    app_listen(PORT);
    printf("[~] Server started on : %d.\n"
           "URL : \033[1mhttp://localhost:%d\033[m \n",
           PORT, PORT);
    app_accept_connections();
    return 0;
}
