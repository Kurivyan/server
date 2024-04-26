#include "app.h"
#include "url_parser.h"
#include "response.h"
#include "routing.h"

#define MAX_CLIENTS 5

int srv_sck;
struct sockaddr_in srv_addr;

int app_listen(int port)
{
    if ((srv_sck = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[~] Server : unable to create socket.\n");
        return 1;
    }

    memset(&srv_addr, 0, sizeof(srv_addr));

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(srv_sck, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(srv_sck, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1)
    {
        perror("[~] Server : ubable to bind socket to domain name.\n");
        return 1;
    }

    if (listen(srv_sck, MAX_CLIENTS) == -1)
    {
        perror("[~] Server: unable to listen socket.\n");
        return 1;
    }
    return 0;
}

void app_accept_connections()
{

    while (1)
    {
        int cnt_sck = accept(srv_sck, 0, 0);
        if (cnt_sck == -1)
            continue;
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("[~] Server : ");
        }
        else if (pid == 0)
        {
            app_work(cnt_sck);
            exit(0);
        }
        close(cnt_sck);
    }
}

void app_work(int client)
{
    close(srv_sck);

    char *req_buff = NULL;
    req_buff = calloc(4096, sizeof(char));

    recv(client, req_buff, 4096, 0);
    struct req_body req;
    app_requiest_handle(req_buff, &req);

    app_routing(&req, client);
    free(req.method);
    free(req.uri);
    for (int i = 0; req.params[i] != NULL; i++)
    {
        free(req.params[i][0]);
        free(req.params[i][1]);
    }
    free(req.params);

    return;
}