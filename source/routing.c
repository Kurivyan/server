#include "url_parser.h"
#include "response.h"

void app_routing(struct req_body *req, int client)
{
    char *req_file = NULL;

    if (strlen(req->uri) > 1)
    {
        char *path = strrchr(req->uri, '/');
        if (path != NULL)
        {
            path = '\0';
            req_file = path + 1;
        }
    }
    if (!strcmp(req->uri, "/"))
    {
        home_page_render(client, req_file);
        return;
    }
    if (!strcmp(req->uri, "/favicon.ico"))
    {
        fav_send(client);
        return;
    }
    if (!strcmp(req->uri, "/img.png"))
    {
        img_send(client);
        return;
    }
    not_found_page_render(client, req_file);
    printf("%s %s\n", req->method, req->uri);
}