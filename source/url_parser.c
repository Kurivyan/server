#include "url_parser.h"

void app_requiest_handle(char *req, struct req_body *trg)
{
    url_method_parse(req, trg);
    url_uri_parse(req, trg);
    url_params_parse(req, trg);
}

void url_method_parse(char *str, struct req_body *trg)
{
    trg->method = (char *)calloc(5, sizeof(char));
    sscanf(str, "%s", trg->method);
}

void url_uri_parse(char *str, struct req_body *trg)
{
    trg->uri = (char *)calloc(100, sizeof(char));
    sscanf(str, "%*s %s", trg->uri);
    char *param_start = strchr(trg->uri, '?');
    if (param_start != NULL)
    {
        *param_start = '\0';
    }
}

void url_params_parse(char *str, struct req_body *trg)
{
    char *param_start = strchr(str, '?');
    if (param_start != NULL)
    {
        trg->params = (char ***)malloc(10 * sizeof(char **));
        int param_count = 0;
        char *token = strtok(param_start + 1, "&");
        while (token != NULL)
        {
            trg->params[param_count] = (char **)malloc(2 * sizeof(char *));
            trg->params[param_count][0] = (char *)malloc(20 * sizeof(char));
            trg->params[param_count][1] = (char *)malloc(20 * sizeof(char));
            sscanf(token, "%[^=]=%s", trg->params[param_count][0], trg->params[param_count][1]);
            param_count++;
            token = strtok(NULL, "&");
        }
        trg->params[param_count] = NULL; // Для обозначения конца массива параметров
    }
    else
    {
        trg->params = NULL;
    }
}