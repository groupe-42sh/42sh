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

bool parser_readpipeline(struct parser_s *p, struct ast_node_pipeline *pipeline)
{
    p = p;
    pipeline = pipeline;
    return false;
}

bool parser_read_doubleesperluette(struct parser_s *p)
{
    int tmp = p->index;

    if (parser_readchar(p, '&') && parser_readchar(p, '&'))
        return true;
    p->index = tmp;
    return false;
}

bool parser_read_doublepipe(struct parser_s *p)
{
    int tmp = p->index;

    if (parser_readchar(p, '|') && parser_readchar(p, '|'))
        return true;
    p->index = tmp;
    return false;
}

bool parser_readandor (struct parser_s *p, struct ast_node_and_or *cur_andor)
{
    //    if (parser_readpipeline(p, cur_andor->pipeline))
    //    {
    //        enum relation rel = DOUBLE_ESPERLUETTE;
    //        rel = rel;
    //
    //        if (parser_read_doublepipe(p))
    //            rel = DOUBLE_PIPE;
    //        else 
    //        {
    //            parser_read_doubleesperluette(p);
    //            rel = DOUBLE_ESPERLUETTE;
    //        }
    //
    //        struct ast_node_pipeline *new_pipeline = malloc(sizeof(struct
    //                    ast_node_pipeline));
    //
    //        cur_andor->pipeline = new_pipeline;
    //
    //        if (parser_readpipeline(p, new_pipeline))
    //            return parser_readandor(p, cur_andor);
    //    }
    //    else
    //        return true;
    p =p ;
    cur_andor = cur_andor;
    return true;
}

bool recurse_on_andor (struct parser_s *p)
{
    struct ast_node_list *list = p->ast->list;

    enum sep separator = RIEN;

    if (parser_readchar(p, ';'))
        separator = POINT_VIRGULE;
    else
    {
        parser_readchar(p, '&');
        separator = ESPERLUETTE;
    }

    struct ast_node_and_or *new = malloc(sizeof(struct ast_node_and_or));
    new->separator = separator;
    new->pipeline = NULL;
    new->next = NULL;

    struct ast_node_and_or *last = list->and_ors;
    while (last->next)
        last = last->next;

    last->next = new;

    //avancer sur les espaces ici
    if (parser_peekchar(p, '\n') || parser_peekchar(p, EOF)) 
        return true;

    if (parser_readandor(p, new))
        return recurse_on_andor(p);
    else
        return false;

    return true;
}

bool parser_readlist (struct parser_s *p)
{
    int tmp = p->index;
    tmp = tmp;
    struct ast_node_list *list = {NULL};
    p->ast->list = list;

    struct ast_node_and_or *and_or = {RIEN, NULL, NULL}; 

    if (parser_readandor(p, and_or))
    {
        // CAS OU IL Y A PONCTUATION
        if (parser_peekchar(p, ';') || parser_peekchar(p, '&'))
        {
            return recurse_on_andor(p);
        }
        else
            return true;
    }
    tmp = p->index;
    return false;
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
    tmp = p->index;
    return false;
}

bool etoile(int(func(struct parser_s *)), struct parser_s *p)
{
    int r = 1;
    while(r == 1)
        r = func(p);
    if (r == -1)
        return false;
    return true;
}

bool plus(int(func(struct parser_s *)), struct parser_s *p)
{
    int r = -1;
    r = func(p);
    if (r)
    {
        while (r == 1)
            r = func(p);
        if (r == -1)
            return false;
        return true;
    }
    return false;
}

bool interrogation(int(func(struct parser_s *)), struct parser_s *p)
{
    if (func(p) == -1)
        return false;
    return true;
}
