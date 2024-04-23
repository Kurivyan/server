#include "response.h"

struct response_body *create(int status, int size, const char *type)
{
    struct response_body *ptr = (struct response_body *)calloc(1, sizeof(struct response_body));
    ptr->status = status;
    ptr->contentsize = size;
    ptr->header = (char *)calloc(HEADER_PRESIZE, sizeof(char));
    sprintf(ptr->header, "HTTP/1.1 %d OK\r\n"
                         "Content-Type: %s\r\n"
                         "Content-Length: %d\r\n"
                         "Connection: keep-alive\r\n\r\n",
            ptr->status, type, ptr->contentsize);
    ptr->content = (char *)calloc(size, sizeof(char));
    return ptr;
}

void not_found_page_render(int client, char *file)
{
    int fd = open("src/views/not_found.html", O_RDONLY);
    if (fd == -1)
        perror("[~] Server : ");

    struct stat filestats;
    stat("src/views/not_found.html", &filestats);

    struct response_body *res = create(200, filestats.st_size, "text/html");
    read(fd, res->content, filestats.st_size);
    close(fd);

    char *response = (char *)calloc(strlen(res->content) + strlen(res->header), sizeof(char));
    strncat(response, res->header, strlen(res->header));
    strcat(response, res->content);

    send(client, response, res->contentsize + strlen(res->header), 0);
    shutdown(client, SHUT_RDWR);
    close(client);
    free(res->header);
    free(res->content);
    free(res);
}

void home_page_render(int client, char *file)
{
    int fd = open("src/views/index.html", O_RDONLY);
    if (fd == -1)
        perror("[~] Server : ");

    struct stat filestats;
    stat("src/views/index.html", &filestats);

    struct response_body *res = create(200, filestats.st_size, "text/html");
    read(fd, res->content, filestats.st_size);
    close(fd);

    char *response = (char *)calloc(strlen(res->content) + strlen(res->header), sizeof(char));
    strcat(response, res->header);
    strcat(response, res->content);

    send(client, response, res->contentsize + strlen(res->header), 0);
    shutdown(client, SHUT_RDWR);
    close(client);
    free(res->header);
    free(res->content);
    free(res);
}

void fav_send(int client)
{
    int fd = open("src/img/favicon.ico", O_RDONLY);
    if (fd == -1)
        perror("[~] Server : ");

    struct stat filestats;
    stat("src/img/favicon.ico", &filestats);

    struct response_body *res = create(200, filestats.st_size, "image/x-icon");
    read(fd, res->content, filestats.st_size);
    close(fd);

    char *response = (char *)calloc(res->contentsize + strlen(res->header), sizeof(char));
    strcat(response, res->header);
    strcat(response, res->content);

    send(client, response, res->contentsize + strlen(res->header), 0);

    size_t total_bytes = res->contentsize + strlen(res->header);
    size_t bytes_sent = 0;
    while (bytes_sent < total_bytes)
    {
        ssize_t sent = send(client, response + bytes_sent, (res->contentsize + strlen(res->header)) - bytes_sent, 0);
        if (sent == -1)
        {
            break;
        }
        bytes_sent += sent;
    }
    shutdown(client, SHUT_RDWR);
    close(client);
    free(res->header);
    free(res->content);
    free(res);
}

void img_send(int client)
{
    int fd = open("src/img/img.png", O_RDONLY);
    if (fd == -1)
        perror("[~] Server : ");

    struct stat filestats;
    stat("src/img/img.png", &filestats);

    struct response_body *res = create(200, filestats.st_size, "image/png");
    read(fd, res->content, filestats.st_size);
    close(fd);

    char *response = (char *)calloc(res->contentsize + strlen(res->header), sizeof(char));
    strcat(response, res->header);
    strcat(response, res->content);

    send(client, response, res->contentsize + strlen(res->header), 0);

    size_t total_bytes = res->contentsize + strlen(res->header);
    size_t bytes_sent = 0;
    while (bytes_sent < total_bytes)
    {
        ssize_t sent = send(client, response + bytes_sent, (res->contentsize + strlen(res->header)) - bytes_sent, 0);
        if (sent == -1)
        {
            break;
        }
        bytes_sent += sent;
    }
    shutdown(client, SHUT_RDWR);
    close(client);
    free(res->header);
    free(res->content);
    free(res);
}