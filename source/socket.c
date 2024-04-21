#include "socket.h"
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
    struct sockaddr_in cnt_addr;
    socklen_t cnt_addr_len;
    while (1)
    {
        int cnt_sck = accept(srv_sck, (struct sockaddr *)&cnt_addr, &cnt_addr_len);
        if (cnt_sck == -1)
            continue;
        if (!fork())
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
    char req[2048];
    recv(client, (void *)&req, 2048, 0);
    char res[2048] = "HTTP/1.1 200 OK\n"
                     "Content-Type: text/html\n"
                     "Content-Length: 1234\n"
                     "Connection: keep-alive\n\n"

                     "<!DOCTYPE html>\n"
                     "<html lang=\"ru\">\n"
                     "<head>\n"
                     "\t<meta charset=\"UTF-8\">\n"
                     "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                     "\t<title>Document</title>\n"
                     "</head>\n"
                     "<body>\n"
                     "\t<p>Hello World.</p>\n"
                     "</body>\n"
                     "</html>\n";
    send(client, (void *)&res, sizeof(res), 0);
    return;
}