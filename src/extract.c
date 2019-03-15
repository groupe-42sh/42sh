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

int eat_spaces(struct parser_s *p)
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
    struct list_capt_s *curr = p->capture;
    struct list_capt_s *delete = curr;
    while (curr)
    {
        delete = curr;
        curr = curr->next;
        free(delete);
    }
}

void eat_newlines(struct parser_s *p)
{
    while(parser_readchar(p, '\n'))
    {}
}

bool parser_read_double_ampersand(struct parser_s *p)
{
    int tmp = p->index;

    if (parser_readchar(p, '&') && parser_readchar(p, '&'))
        return true;

    p->index = tmp;
    return false;
}

bool parser_read_double_pipe(struct parser_s *p)
{
    int tmp = p->index;

    if (parser_readchar(p, '|') && parser_readchar(p, '|'))
        return true;

    p->index = tmp;
    return false;
}

int type_of_command(struct parser_s *p)
{
    int tmp = p->index;

    if (parser_readchar(p, '{') || parser_readchar(p, '(')
        || parser_readtext(p, "while") || parser_readtext(p, "for")
        || parser_readtext(p, "case") || parser_readtext(p, "until")
        || parser_readtext(p, "if"))
        return SHELL;

    if (parser_readtext(p, "function")
        || (parser_readidentifier(p) && parser_readchar(p, '(')))
    {
        p->index = tmp;
        return FUNCDEC; 
    }

    p->index = tmp;
    return SIMPLE;
}

int get_io_number(struct parser_s *p)
{
    int tmp = p->index;
    if (parser_begin_capture(p, "ionumber") && parser_readinteger(p) &&
        parser_end_capture(p, "ionumber"))
    {
        char *num = parser_get_capture(p, "ionumber");
        reinit_capture(p);
        /* voir si interdite */
        return atoi(num);
    }

    reinit_capture(p);
    p->index = tmp;
    return -1;
}

char *get_redirection(struct parser_s *p)
{
    int begin = p->index;

    while (parser_readinset(p, "<>-&|"))
        ++p->index;

    return strndup(&p->input[begin], p->index - begin);
}

int get_redirection_type(struct parser_s *p)
{
    size_t i = 0;
    size_t redirections_number = 10;
    char *redirections[] = {">", "<", ">>", "<<", "<<-", ">&", ">&", "<&", ">|", "<>"};
    
    char *redirection = get_redirection(p);

    for (i = 0; i < redirections_number; ++i)
    {
        if (strcmp(redirections[i], redirection) == 0)
            return i + 1; /* a cause du void */
    }
    return VOID;
}

char *get_word(struct parser_s *p)
{
    char *word = NULL;
    if (parser_begin_capture(p, "word") && parser_readidentifier(p)
        && parser_end_capture(p, "word"))
        word = parser_get_capture(p, "word"); 
    reinit_capture(p);
    /*free word*/
    return word;
}

struct ast_node_word *parser_readword(struct parser_s *p)
{
    struct ast_node_word *word = malloc(sizeof(struct ast_node_word));
    word->next = NULL;

    if (!(word->str = get_word(p)))
        return NULL;
    return word;
}

struct ast_node_redirection *parser_readredirection(struct parser_s *p)
{
    struct ast_node_redirection *redirection =
        malloc(sizeof(struct ast_node_redirection));

    redirection->io_number = get_io_number(p);
    if ((redirection->io_number == -1) && (!parser_peekinset(p, "<>-&|")))
        return NULL;
    else
        redirection->io_number = 1;

    redirection->redirection_type = get_redirection_type(p);
    if (!redirection->redirection_type)
        return NULL;
  
    struct ast_node_word *word = parser_readword(p);

    if (!word)
        return NULL;

    if ((redirection->redirection_type == LTLT)
        || (redirection->redirection_type == LTLTDASH))
    {
        redirection->word_heredoc.heredoc = word->str;
        redirection->word_heredoc.word->str = NULL;
    }
    else
    {
        redirection->word_heredoc.heredoc = NULL;
        redirection->word_heredoc.word->str = word->str;
    }
    
    redirection->next = NULL;
    return redirection;
}

struct ast_node_compound_list *parser_readcompoundlist(struct parser_s *p)
{
//    struct ast_node_compound_list *compound_list =
//        malloc(sizeof(struct ast_node_compound_list));
//
//    compound_list->next = NULL;
//
//    eat_newlines(p);
//    
//    struct ast_node_and_or *and_ors = NULL;
//    struct ast_node_and_or *last = NULL;
//
//    while ((last = parser_readandor(p)))
//    {
//        
//    }
    p = p;
    return NULL;
}

struct ast_node_do_group *parser_readdogroup(struct parser_s *p)
{
    struct ast_node_do_group *do_group =
        malloc(sizeof(struct ast_node_do_group));

    if (!parser_readtext(p, "do")
        || !(do_group->compound_list = parser_readcompoundlist(p))
        || !parser_readtext(p, "done"))
        return NULL;
    
    return do_group;
}


struct ast_node_element *parser_readelement(struct parser_s *p)
{
    struct ast_node_element *element = malloc(sizeof(struct ast_node_element));
    element->next = NULL;

    if (!(element->word = parser_readword(p))
        && !(element->redirection = parser_readredirection(p)))
            return NULL;
    return element;
}

struct ast_node_assignement_word *get_assignement_word(struct parser_s *p)
{
     struct ast_node_assignement_word *assignement_word
        = malloc(sizeof(struct ast_node_assignement_word));
   
     if (!(assignement_word->var_name = get_word(p))
        || (!parser_readchar(p, '='))
        || (!(assignement_word->value = atoi(get_word(p)))))
        return NULL;
    return assignement_word;
}

struct ast_node_assignement_word *parser_readassignementword(struct parser_s *p)
{
    struct ast_node_assignement_word *assignement_word = NULL;
    if (!(assignement_word = get_assignement_word(p)))
        return NULL;

//    int tmp = p->index;
//
//    if (parser_begin_capture(p, "assign") && parser_readidentifier(p)
//        && parser_end_capture(p, "assign"))
//        && parser_readchar(p, '=')
//        && parser_begin_capture(p, "value") && parser_readinteger(p)
//        && parser_end_capture(p, "value")
//    {
//        assignement_word->var_name = parser_get_capture("assign");
//        assignement_word->value = atoi(parser_get_capture("value"));
//        assignement_word->next = NULL;
//        
//        reinit_capture(p);
//        return assignement_word;
//    }
//    
//    p->index = tmp;

    struct ast_node_assignement_word *current = assignement_word; 
    struct ast_node_assignement_word *last = NULL;
    
    eat_newlines(p);

    while ((last = get_assignement_word(p)))
    {
        current->next = last;
        current = last;
        
        eat_newlines(p);
    }

    return assignement_word;
}

struct ast_node_prefix *parser_readprefix(struct parser_s *p)
{
    struct ast_node_prefix *prefix = malloc(sizeof(struct ast_node_prefix));
    prefix->next = NULL;
    if (!(prefix->assignement_word = parser_readassignementword(p))
        && !(prefix->redirection = parser_readredirection(p)))
        return NULL;
    return prefix;
}

struct ast_node_simple_command *parser_readsimplecommand(struct parser_s *p)
{
    struct ast_node_simple_command *simple_command =
        malloc(sizeof(struct ast_node_simple_command));
    
    simple_command->element_list = NULL;
    simple_command->prefix_list = NULL;
    
    struct ast_node_prefix *prefix = NULL;
    struct ast_node_prefix *last = NULL;
    simple_command->prefix_list = parser_readprefix(p);
    prefix = simple_command->prefix_list;

    while ((last = parser_readprefix(p)))
    {
        prefix->next = last;
        prefix = last;
    }

    struct ast_node_element *element = NULL;
    struct ast_node_element *last_e = NULL;
    simple_command->element_list = parser_readelement(p);
    element = simple_command->element_list;

    while ((last_e = parser_readelement(p)))
    {
        element->next = last_e;
        element = last_e;
    }

    if (!simple_command->prefix_list && !simple_command->element_list)
        return NULL;

    return simple_command;
}

int get_shell_command_type(char *word)
{
    size_t index = 0;
    size_t commands_number = 5;
    char *shell_command_words[] = {"for", "while", "case", "if", "until"};

    for (index = 0; index < commands_number; ++index)
    {
        if (strcmp(shell_command_words[index], word) == 0)
            return index + 1;
    }
    return VOID;
}

struct ast_node_word *get_word_list(struct parser_s *p)
{
    struct ast_node_word *root = parser_readword(p);
    if (!root)
        return NULL;

    struct ast_node_word *word = NULL;
    struct ast_node_word *last = NULL;
    word = root;

    while ((last = parser_readword(p)))
    {
        word->next = last;
        word = last;
    }

    return root;
}

struct ast_node_else_clause *parser_readelseclause(struct parser_s *p)
{
    p = p;
    return NULL;
}

struct ast_node_rule_if *parser_readruleif(struct parser_s *p)
{
    struct ast_node_rule_if *rule_if =
        malloc(sizeof(struct ast_node_rule_if));

    rule_if->condition = NULL;
    rule_if->body = NULL;
    rule_if->else_clause = NULL;

    if (!(rule_if->condition = parser_readcompoundlist(p))
        || !(parser_readtext(p, "then"))
        || !(rule_if->body = parser_readcompoundlist(p)))
        return NULL;
    rule_if->else_clause = parser_readelseclause(p);
    if (!(parser_readtext(p, "fi")))
        return NULL;

    return rule_if;
}

struct ast_node_rule_for *parser_readrulefor(struct parser_s *p)
{
    struct ast_node_rule_for *rule_for =
        malloc(sizeof(struct ast_node_rule_for));
    
    rule_for->word_list = NULL;
    rule_for->do_group = NULL;

    if (!(rule_for->word = parser_readword(p)))
        return NULL;

    if (!parser_readchar(p, ';') /* && eat_newlines(p) */)
    {
        eat_newlines(p);
        
        if (parser_readtext(p, "in"))
        {
            rule_for->word_list = get_word_list(p);
    
            if (!parser_readchar(p, ';') && !parser_readchar(p, '\n'))
                return NULL;
        }
    }   
    
    eat_newlines(p);

    if (!(rule_for->do_group = parser_readdogroup(p)))
        return NULL;

    return rule_for;
}

struct ast_node_rule_while *parser_readrulewhile(struct parser_s *p)
{
    struct ast_node_rule_while *rule_while = 
        malloc(sizeof(struct ast_node_rule_while));
    
    rule_while->compound_list = parser_readcompoundlist(p);

    if (!(rule_while->compound_list = parser_readcompoundlist(p))
        || !(rule_while->do_group = parser_readdogroup(p)))
        return NULL;

    return rule_while;
}

struct ast_node_rule_until *parser_readruleuntil(struct parser_s *p)
{
    struct ast_node_rule_until *rule_until = 
        malloc(sizeof(struct ast_node_rule_until));
    
    rule_until->compound_list = parser_readcompoundlist(p);

    if (!(rule_until->compound_list = parser_readcompoundlist(p))
        || !(rule_until->do_group = parser_readdogroup(p)))
        return NULL;
    
    return rule_until;
}

struct ast_node_rule_case *parser_readrulecase(struct parser_s *p)
{
    p = p;
    return NULL;
}

struct ast_node_shell_command *parser_readshellcommand(struct parser_s *p)
{
    struct ast_node_shell_command *shell_command =
        malloc(sizeof(struct ast_node_shell_command));
    shell_command->type = VOID;
    shell_command->child.compound_list = NULL;

    bool opening = false;

    struct ast_node_compound_list *compound_list = NULL;
    
    if ((opening = parser_readchar(p, '(')) || (parser_readchar(p, '{')))
    {
        if (!(compound_list = parser_readcompoundlist(p)))
            return NULL;
        
        shell_command->child.compound_list = compound_list;

        if ((opening && !parser_readchar(p, ')')) || (!opening &&
            !parser_readchar(p, '}')))
            return NULL;
        return shell_command;
    }

    char *command_word = get_word(p);
    enum shell_command_type type = get_shell_command_type(command_word);
    switch (type)
    {
        case FOR:
            shell_command->child._for = parser_readrulefor(p);
            break;
        case WHILE:
            shell_command->child._while = parser_readrulewhile(p);
            break;
        case CASE:
            shell_command->child._case = parser_readrulecase(p);
            break;
        case IF:
            shell_command->child._if = parser_readruleif(p);
            break;
        case UNTIL:
            shell_command->child._until = parser_readruleuntil(p);
            break;
        default:
            return NULL;
    }

    shell_command->type = type;
    
    return shell_command;
}



struct ast_node_funcdec *parser_readfuncdec(struct parser_s *p)
{
    struct ast_node_funcdec *funcdec = malloc(sizeof(struct ast_node_funcdec));
    funcdec->word = NULL;
    funcdec->shell_command = NULL;

    parser_readtext(p, "function");

    if (!((funcdec->word = parser_readword(p)) && parser_readchar(p, '(')
        && parser_readchar(p, ')')))
        return NULL;     
   
    eat_newlines(p);

    if (!(funcdec->shell_command = parser_readshellcommand(p)))
        return NULL;    
    
    return funcdec;
}

struct ast_node_command *parser_readcommand(struct parser_s *p)
{
    //int tmp = p->index;
    
    struct ast_node_command *command = malloc(sizeof(struct ast_node_command));
    command->piped = false;
    command->simple_command = NULL;
    command->shell_command = NULL;
    command->funcdec = NULL;
    command->next = NULL;
    command->redirection_list = NULL;

    int type = type_of_command(p);
    
    switch (type)
    {
        case SIMPLE:
            command->simple_command = parser_readsimplecommand(p);
            return command;
        case SHELL:
            command->shell_command = parser_readshellcommand(p); 
            break;
        case FUNCDEC:
            command->funcdec = parser_readfuncdec(p);
            break;
        default:
            return NULL;
    }

    struct ast_node_redirection *redirection = NULL;
    redirection = parser_readredirection(p);
    command->redirection_list = redirection;

    struct ast_node_redirection *last = NULL;
    //int io = 1;
    //enum shell_command_type rt = VOID;

    while (eat_spaces(p) && (last = parser_readredirection(p)))
    {
         redirection->next = last;
         redirection = last;
    }

    return command;
}

struct ast_node_pipeline *parser_readpipeline(struct parser_s *p)
{
    //int tmp = p->index;

    struct ast_node_pipeline *pipeline =
        malloc(sizeof(struct ast_node_pipeline));

    pipeline->reverse = false;
    if (parser_readchar(p, '!'))
        pipeline->reverse = true;
    pipeline->relation = VOID;
    pipeline->command = NULL;
    pipeline->next = NULL;

    struct ast_node_command *command = NULL; 
    if (!(command = parser_readcommand(p)))
        return NULL;
    pipeline->command = command;
    struct ast_node_command *last = NULL;

    while (parser_readchar(p, '|') && (last = parser_readcommand(p)))
    {
        command->piped = true;
        eat_newlines(p);
        command->next = last;
        command = last;
    }
    return pipeline;
}

struct ast_node_and_or *parser_readandor(struct parser_s *p)
{
    //int tmp = p->index;

    struct ast_node_and_or *and_or = malloc(sizeof(struct ast_node_and_or));
    and_or->linked = false;
    and_or->pipeline = NULL;
    and_or->next = NULL;

    struct ast_node_pipeline *pipeline = NULL;

    if (!(pipeline = parser_readpipeline(p)))
        return NULL;

    struct ast_node_pipeline *last = NULL;
    bool c = false;

    while ((c = parser_read_double_ampersand(p) || parser_read_double_pipe(p))
            && (last = parser_readpipeline(p)))
    {
        pipeline->relation = c ? TOKEN_DOUBLE_AMPERSAND : TOKEN_DOUBLE_PIPE;
        eat_newlines(p);
        pipeline->next = last;
        pipeline = last;
    } 

    return and_or;    
}

struct ast_node_list *parser_readlist(struct parser_s *p)
{
    //int tmp = p->index;

    /* init de la liste */
    p->ast->list = malloc(sizeof(struct ast_node_list));
    p->ast->list->and_ors = NULL;

    struct ast_node_and_or *and_or = NULL;

    /* cas ou ca marche pas */
    if (!(and_or = parser_readandor(p)))
        return NULL;

    p->ast->list->and_ors = and_or;

    struct ast_node_and_or *last = NULL;
    bool c = false;

    while ((parser_readchar(p, ';') || (c = parser_readchar(p, '&')))
            && (last = parser_readandor(p)))
    {
        and_or->linked = c;
        and_or->next = last;
        and_or = last;
        c = false;
    }

    return p->ast->list;
}

bool parser_readinput(struct parser_s *p)
{
    int tmp = p->index;

    /* init de l ast */
    p->ast = malloc(sizeof(struct ast_node_input));
    p->ast->list = NULL; 

    if (parser_readchar(p, '\n'))
        return true;

    if (parser_eof(p))
        return true;

    if ((p->ast->list = parser_readlist(p)) && ((parser_readchar(p, '\n') || parser_eof(p))))
        return true;

    p->index = tmp;
    return false;
}

bool etoile(int(func(struct parser_s *, void*)), void *struc,
        struct parser_s *p)
{
    int r = 1;
    while(r == 1)
        r = func(p, struc);
    if (r == -1)
        return false;
    return true;
}

bool plus(int(func(struct parser_s *, void*)), void *struc, struct parser_s *p)
{
    int r = -1;
    r = func(p, struc);
    if (r)
    {
        while (r == 1)
            r = func(p, struc);
        if (r == -1)
            return false;
        return true;
    }
    return false;
}

bool interrogation(int(func(struct parser_s *, void*)), void *struc,
        struct parser_s *p)
{
    if (func(p, struc) == -1)
        return false;
    return true;
}
