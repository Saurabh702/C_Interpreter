#include "str.h"

void init_string(string *s,int len)
{
    s->len = len;
    s->ptr = malloc(s->len+1);

    if (s->ptr == NULL)
    {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }

    s->ptr[0] = '\0';
}

void display_string(string s)
{
    printf("%s",s.ptr);
}