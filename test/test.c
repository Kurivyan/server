#include <stdio.h>
#include <string.h>

char *req_method_parse(char *str)
{
    static char method[10];
    sscanf(str, "%s", method);
    return method;
}

char *req_URI_parse(char *str)
{
    static char URI[100];
    sscanf(str, "%*s %s", URI);
    return URI;
}

int main()
{
    char str[] = "GET /index.html HTTP/1.1";
    char *method = req_method_parse(str);
    char *URI = req_URI_parse(str);

    printf("Method: %s\n", method);
    printf("URI: %s\n", URI);

    return 0;
}