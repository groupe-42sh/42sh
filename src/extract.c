#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "extract.h"
#include "parser.h"
#include "ast.h"

#include <stdio.h>
#include <time.h>

int read_space(struct parser_s *p)
{
    return parser_readchar(p, ' ');
}
int read_tabs(struct parser_s *p)
{
    return parser_readchar(p, '\t');
}

int eatspaces(struct parser_s *p)
{
    while(read_tabs(p) || read_space(p)) //consomme les espaces et tabs
    {}
    return 1;
}

int read_openbracket(struct parser_s *p)
{
    return parser_readchar(p, '[');
}

void list_capt_store(struct list_capt_s *capture, const char *tag,
        struct capture_s *capt)
{
    if (!capture)
        return;

    if (!capt)
        return;

    if (!capture->tag)
    {
        capture->tag = tag;
        capture->capt = *capt;
        capture->next = NULL;
        return;
    }

    struct list_capt_s *prev = capture;
    struct list_capt_s *current = NULL;
    current = capture;

    while (current)
    {
        prev = current;
        current = current->next;
    }

    struct list_capt_s *new = malloc(sizeof(struct list_capt_s));
    if (!new)
        return;
    new->tag = tag;
    new->capt = *capt;
    new->next = NULL;
    prev->next = new;
}

struct capture_s *list_capt_lookup(struct list_capt_s *capture, const char *tag)
{
    if (!capture)
        return NULL;
    struct list_capt_s *current = capture;

    while (current && (strcmp(current->tag, tag) != 0))
        current = current->next;

    return &current->capt;
}

static inline bool parser_begin_capture(struct parser_s *p, const char *tag)
{
    struct capture_s *capt = malloc(sizeof(struct capture_s));

    capt->begin = p->index;
    capt->end = 0;
    list_capt_store(p->capture, tag, capt);
    return true;
}

static inline bool parser_end_capture(struct parser_s *p, const char *tag)
{
    struct capture_s *pcapt = list_capt_lookup(p->capture, tag);
    if (!pcapt)
        return false;
    pcapt->end = p->index;
    return true;
}

static inline char *parser_get_capture(struct parser_s *p, const char *tag)
{
    struct capture_s *pcapt = list_capt_lookup(p->capture, tag);
    if (!pcapt)
        return false;
    return strndup(&p->input[pcapt->begin], pcapt->end - pcapt->begin);
}

void reinit_capture(struct parser_s *p)
{
    struct list_capt_s *curr = p->capture->next;
    struct list_capt_s *delete = curr;
    while (curr)
    {
        delete = curr;
        curr = curr->next;
        free(delete);
    }

    p->capture->next = NULL;
    p->capture->tag = NULL;   
}

bool parser_readandor (struct parser_s *p)
{
    return parser_readidentifier(p);
}

bool recurse_on_andor (struct parser_s *p)
{
    if (parser_readchar(p, ';') || parser_readchar(p, '&'))
    {
        //avancer sur les espaces ici
        if (parser_peekchar(p, '\n') || parser_peekchar(p, EOF)) 
            return true;

        if (parser_readandor(p))
            return recurse_on_andor(p);
        else
            return false;
    }
    return true;
}

bool parser_readlist (struct parser_s *p)
{
    int tmp = p->index;
    tmp = tmp;
    if (parser_readandor(p))
    {
        //struct ast_node_list *list;

        // CAS OU IL Y A PONCTUATION
        if (parser_peekchar(p, ';') || parser_peekchar(p, '&'))
        {
            return recurse_on_andor(p);
        }
        else
            return true;
    }
    
    return true;
}

bool parser_readinput (struct parser_s *p)
{
    int tmp = p->index;
    tmp = tmp;
    if (parser_readchar(p, '\n'))
        return true;
    if (parser_eof(p))
        return true;
    if (parser_readlist(p) && ((parser_readchar(p, '\n') || parser_eof(p))))
        return true;
    //tmp = p->index;
    return false;
}
