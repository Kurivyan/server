#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct req_body
{
    char *method;
    char *uri;
    char ***params;
};
void app_requiest_handle(char *, struct req_body *);
void url_method_parse(char *, struct req_body *);
void url_uri_parse(char *, struct req_body *);
void url_params_parse(char *, struct req_body *);