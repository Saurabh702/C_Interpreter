#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef struct String
{
    char *ptr;
    size_t len;
} string;

void init_string(string *,int);
void display_string(string);

#endif // STR_H_INCLUDED