#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

//int read_input();
struct parser_s *parser_new_from_string(const char *text)
{
    struct parser_s *p=NULL;
    p->index=0;
    p->input=text;
    return p;
}
