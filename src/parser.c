/*!
 * \file parser.c
 */
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "extract.h"
/*!
 * \struct parser_s
 * \brief returns a pointer to a parser_s type structure
 */
struct parser_s *parser_new_from_string(const char *text)
{
    struct parser_s *p = malloc(sizeof(struct parser_s));
    p->input = text;
    p->index = 0;
    p->ast = NULL;
    p->error = malloc(sizeof(struct error_s));
    return p;
}

/*!
 * \fn void parser_clean(struct parser_s *p)
 * \brief this function cleans the content of parser_s
 * \param strcuct parser_s 
 */

void parser_clean(struct parser_s *p)
{
    p->input = NULL;
    p->index = 0;
    /*free tree*/
    free(p);
}

/*!
 * \fn int parser_eof(struct parser_s *p)
 * \brief this function checks EOF and returns 1 or 0
 * \param struct parser_s
 */
int parser_eof(struct parser_s *p)
{
    if ((p->input[p->index] == '\0') || (p->input[p->index] == EOF))
        return 1;
    return 0;
}

/*!
 * \fn char parser_getchar(struct parser_s *p)
 * \brief This function permits to retrieve a char
 * \param struct parser_s *p)
 */
char parser_getchar(struct parser_s *p)
{
    p->index += 1;
    return p->input[p->index - 1];
}

/*!
 * \fn int parser_peekchar(struct parser_s *p, char c)
 * \brief This function returns 1 if the current char = C else it returns 0
 * \param struct parser_p
 */
int parser_peekchar(struct parser_s *p, char c)
{
    if (p->input[p->index] == c)
        return 1;
    return 0;
}

int parser_readchar(struct parser_s *p, char c)
{
    if (c != ' ')
        eat_spaces(p);
    p->error->type = ON_CHAR;
    p->error->u.c = c;

    if (parser_peekchar(p, c) == 1)
    {
        p->index += 1;
        return 1;
    }
    return 0;
}
int parser_readrange(struct parser_s *p, char begin, char end)
{
    p->error->type = ON_RANGE;
    p->error->u.range.begin = begin;
    p->error->u.range.end = end;
    if (begin > end) /* la mettre ?*/
        return 0;
    if ((begin <= p->input[p->index]) && (p->input[p->index] <= end))
    {
        p->index += 1;
        return 1;
    }
    return 0;
}
int parser_peekinset(struct parser_s *p, char *set)
{
    p->error->type = ON_INSET;
    p->error->u.inset = set;

    size_t i = 0;

    while (set[i] != '\0')
    {
        if (p->input[p->index] == set[i])
            return 1;
        ++i;
    }
    return 0;
}
int parser_readinset(struct parser_s *p, char *set)
{
    p->error->type = ON_INSET;
    p->error->u.inset = set;

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
    eat_spaces(p);
    p->error->type = ON_OUTSET;
    p->error->u.outset = set;
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

bool parser_readeol(struct parser_s *p)
{
    eat_spaces(p);
    if (p->input[p->index] == '\0')
    {
        p->index += 1;
        return true;
    }

    if (read_nl(p))
        return true;

    if (read_rc(p) & read_nl(p))
        return true;

    return false;
}


bool is_eol(struct parser_s *p)
{
    if (p->input[p->index] == '\0')
        return true;
    if (p->input[p->index] == '\n')
        return true;
    if ((p->input[p->index] == '\r') && (p->input[p->index] == '\n'))
        return true;
    return false;
}


bool is_delimiter(struct parser_s *p)
{
    //faire correspondre avec enum des TOKEN, boucler renvoyer
    //tu regarde si tu veux la valeur
    return parser_peekchar(p, ' ') || parser_peekchar(p, '\t') ||
        parser_peekchar(p, ';') || parser_peekchar(p, '&')
        || parser_peekchar(p,'\n') || parser_peekchar(p, '(')
        || parser_peekchar(p,EOF)|| parser_peekchar(p, '\0')
        || parser_peekchar(p, ')') || parser_peekchar(p, '=')
        || parser_peekchar(p, '|') || parser_peektext(p,"&&")
        || parser_peektext(p,"||");
}
bool parser_readidentifier(struct parser_s *p)
{
    eat_spaces(p);
    if (read_min(p) || read_maj(p) || read_underscore(p))
    {
        while (!is_delimiter(p))
        {
            if (!(read_min(p) || read_maj(p)
                        || read_underscore(p) || read_digit(p)))
                return false;
        }
        return true;
    }
    return false;
}

//bool parser_readword(struct parser_s *p)
//{
//  int tmp = p->index;

//  while (!is_delimiter(p))
//    {
//           if (!(read_min(p) || read_maj(p)
//		 || read_underscore(p) || read_digit(p) || parser_readinset("-[]")))
//	     return false;
//    }
// return true;

//}


bool parser_readinteger(struct parser_s *p)
{
    eat_spaces(p);
    parser_readchar(p,'-');
    while (!is_eol(p) && !is_delimiter(p))
        if (!read_digit(p))
            return false;
    return true;
}
int parser_readtext(struct parser_s *p, char *text)
{
    eat_spaces(p);
    p->error->type = ON_TEXT;
    p->error->u.text = text;
    size_t i = 0;

    for (i = 0; text[i] != '\0' || !is_delimiter(p) ; ++i)
    {
        if (p->input[p->index] != text[i])
            return 0;
        p->index++;
    }
    return 1;
}
int parser_peektext(struct parser_s *p, char *text)
{
    eat_spaces(p);
    p->error->type = ON_TEXT;
    p->error->u.text = text;
    size_t i = 0;

    for (i = 0; text[i] != '\0' || !is_delimiter(p) ; ++i)
    {
        if (p->input[p->index + i] != text[i])
            return 0;
    }
    return 1;
}
