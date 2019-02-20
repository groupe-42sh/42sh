#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

struct parser_s *parser_new_from_string(const char *text)
{
    struct parser_s *p = malloc(sizeof(struct parser_s));
    p->input = text;
    p->index = 0;
    
    return p;
}

void parser_clean(struct parser_s *p)
{
    p->input = NULL;
    p->index = 0;
    free(p);
}

int parser_eof(struct parser_s *p)
{
   if (p->input[p->index] == EOF)
       return 1;
    return 0;
}

char parser_getchar(struct parser_s *p)
{
    p->index += 1;
    return p->input[p->index - 1];
}

int parser_peekchar(struct parser_s *p, char c)
{
    if (p->input[p->index] == c)
        return 1;
    return 0;
}

int parser_readchar(struct parser_s *p, char c)
{
    if (parser_peekchar(p, c) == 1)
    {
        p->index += 1;
        return 1;
    }
    return 0;
}

int parser_readtext(struct parser_s *p, char *text)
{
    size_t index = p->index;
    size_t i = 0;

    for (i = 0; text[i] != '\0'
        && ((p->input[i] != EOF) & (p->input[i] != '\n')); ++i, ++index)
    {
        if (p->input[index] != text[i])
            return 0;
    }
    p->index += i;
    return 1;
}

int parser_readrange(struct parser_s *p, char begin, char end)
{
    if (begin > end) /* la mettre ?*/
        return 0;
    if ((begin <= p->input[p->index]) && (p->input[p->index] <= end))
    {
        p->index += 1;
        return 1;
    }
    return 0;
}

int parser_readinset(struct parser_s *p, char *set)
{
    size_t i = 0;

    while (set[i] != '\0')
    {
        if (p->input[p->index] == set[i])
        {
            p->index += 1;
            return 1;
        }
        ++i;
    }
    return 0;
}

int parser_readoutset(struct parser_s *p, char *set)
{
    size_t i = 0;

    while (set[i] != '\0')
    {
        if (p->input[p->index] == set[i])
            return 0;
        ++i;
    }
    p->index += 1;
    return 1;
}

int read_nl(struct parser_s *p)
{
    return parser_readchar(p, '\n');
}

int read_tab(struct parser_s *p)
{
    return parser_readchar(p, '\t');
}

int read_rc(struct parser_s *p)
{
    return parser_readchar(p, '\r');
}

int read_min(struct parser_s *p)
{
    return parser_readrange(p, 'a', 'z');
}

int read_maj(struct parser_s *p)
{
    return parser_readrange(p, 'A', 'Z');
}

int read_digit(struct parser_s *p)
{
    return parser_readrange(p, '0', '9');
}

int read_underscore(struct parser_s *p)
{
    return parser_readchar(p, '_');
}

int parser_readeol(struct parser_s *p)
{
    if (p->input[p->index] == '\0')
        return 1;

    if (read_nl(p))
        return 1;

    if (read_rc(p) & read_nl(p))
        return 1;

    return 0;
}

int parser_readidentifier(struct parser_s *p)
{
    if (read_min(p) || read_maj(p) || read_underscore(p))
    {
        while (!parser_readeol(p))
        {
            if (!(read_min(p) || read_maj(p)
                || read_underscore(p) || read_digit(p)))
                return 0;
        }
        
        return 1;
    }
    return 0;
}

int parser_readinteger(struct parser_s *p)
{
    while (!parser_readeol(p))
        if (!read_digit(p))
            return 0;
    return 1;
}
