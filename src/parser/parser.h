#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct parser_s
{
    size_t index;
    const char *input;
};

struct parser_s *parser_new_from_string(const char *text);
void parser_clean(struct parser_s *p);
