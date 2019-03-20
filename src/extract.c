#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "extract.h"
#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <time.h>

void eat_spaces(struct parser_s *p)
{
    while (parser_peekchar(p, ' '))
    {
        p->index++;
    }
}

void eat_newlines(struct parser_s *p)
{
    eat_spaces(p);
    while (parser_readchar(p,'\n'))
    {
    }
}

char *get_word(struct parser_s *p)
{
    int tmp = p->index;
    char *word = NULL;
    if (parser_readidentifier(p))
    {
        word = strndup(&p->input[tmp], p->index - tmp);
    }
    return word;
}

int get_value(struct parser_s *p)
{
    int r = 0;
    int tmp = p->index;
    bool ltz = false;
    if (parser_peekchar(p,'-'))
        ltz = true;
    if (parser_readinteger(p))
        r = atoi(strndup(&p->input[tmp], p->index - tmp));
    return ltz ? -r : r;
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

struct ast_node_rule_if *parser_readruleif (struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_rule_if *rule_if = malloc(sizeof(struct ast_node_rule_if));
    rule_if->condition = NULL;
    rule_if->body = NULL;
    rule_if->else_clause = NULL;

    if (!(rule_if->condition = parser_readcompoundlist(p))
        || !(parser_readtext(p, "then"))
        || !(rule_if->body = parser_readcompoundlist(p)))
    {
        p->index = tmp;
        return NULL;
    }
    rule_if->else_clause = parser_readelseclause(p);
    if (!(parser_readtext(p, "fi")))
    {
        p->index = tmp;
        return NULL;
    }
    return rule_if;
}

struct ast_node_rule_for *parser_readrulefor(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_rule_for *rule_for =
        malloc(sizeof(struct ast_node_rule_for));
    rule_for->word_list = NULL;
    rule_for->do_group = NULL;
    if (!(rule_for->word = parser_readword(p)))
    {
        p->index = tmp;
        return NULL;
    }
    if (!parser_readchar(p, ';'))
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
    {
        p->index = tmp;
        return NULL;
    }
    return rule_for;
}

void exit_on_error(struct parser_s *p)
{
    printf("parsing failed at %ld\n", p->index);
    switch (p->error->type)
    {
    case ON_CHAR:
        printf("expected '%c'\n", p->error->u.c);
        break;
    case ON_TEXT:
        printf("expected \"%s\"\n", p->error->u.text);
        break;
    case ON_RANGE:
        printf("expected range \"%d\"\n", p->error->u.range.begin);
        break;
    case ON_INSET:
        printf("expected \"%s\"\n", p->error->u.inset);
        break;
    case ON_OUTSET:
        printf("expected \"%s\"\n", p->error->u.outset);
        break;
    default:
        printf("other error\n");
        break;
    }
    exit(EXIT_FAILURE);
}

struct ast_node_assignement_word *get_assignement_word(struct parser_s *p)
{
    int tmp = p->index;
    eat_spaces(p);
    int c =0;
    struct ast_node_assignement_word *assignement_word =
        malloc(sizeof(struct ast_node_assignement_word));
    if (!(assignement_word->var_name = get_word(p))
            || (!(c = parser_readchar(p, '=')))
            || (!(assignement_word->value = get_value(p))))
    {
        if (!is_delimiter(p) && !get_word(p) && !parser_peekinset(p, "<>-&|")
            && (get_io_number(p) == -1))
            exit_on_error(p);
        p->index = tmp;
        return NULL;
    }
    return assignement_word;
}

struct ast_node_assignement_word *parser_readassignementword(struct parser_s *p)
{
    eat_spaces(p);
    struct ast_node_assignement_word *assignement_word = NULL;
    if (!(assignement_word = get_assignement_word(p)))
        return NULL;
    struct ast_node_assignement_word *current = assignement_word;
    struct ast_node_assignement_word *last = NULL;
    //eat_newlines(p);
    while ((last = get_assignement_word(p)))
    {
        current->next = last;
        current = last;

        //eat_newlines(p);
    }
    return assignement_word;
}

struct ast_node_word *parser_readword(struct parser_s *p)
{
    eat_spaces(p);
    struct ast_node_word *word = malloc(sizeof(struct ast_node_word));
    word->next = NULL;
    if (!(word->str = get_word(p)))
        return NULL;
    return word;
}

struct ast_node_compound_list *parser_readcompoundlist(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_compound_list *compound_list =
        malloc(sizeof(struct ast_node_compound_list));
    compound_list->and_or = NULL;
    eat_newlines(p);
    struct ast_node_and_or *and_or = NULL;
    if (!(and_or = parser_readandor(p)))
    {
        p->index = tmp;
        return NULL;
    }

    compound_list->and_or = and_or;
    struct ast_node_and_or *last = compound_list->and_or;
    bool c = false;
    struct ast_node_and_or *new = NULL;
    while (!(parser_peektext(p,";;")) && (parser_readchar(p,';')
        || (c = parser_readchar(p,'&')) || parser_readchar(p,'\n')))
    {
        eat_newlines(p);
        and_or->linked = c;
        if ((new = parser_readandor(p)))
        {
            while (last->next)
                last = last->next;

            last->next = new;
        }
        c = false;
    }
    return compound_list;
}

struct ast_node_case_item *parser_readcaseitem(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_case_item *case_item =
        malloc(sizeof(struct ast_node_case_item));
    case_item->word_list = NULL;
    case_item->compound_list = NULL;
    case_item->next = NULL;
    parser_readchar(p,'(');
    struct ast_node_word *word = NULL;
    if (!(word = parser_readword(p)))
    {
        p->index = tmp;
        return NULL;
    }
    case_item->word_list = word;
    struct ast_node_word *last_word = NULL;
    while (parser_readchar(p,'|') && (last_word = parser_readword(p)))
    {
        word->next = last_word;
        word = last_word;
    }
    if (!(parser_readchar(p,')')))
    {
        p->index = tmp;
        return NULL;
    }
    eat_newlines(p);
    struct ast_node_compound_list *compound_list = NULL;
    if ((compound_list = parser_readcompoundlist(p)))
        case_item->compound_list = compound_list;
    return case_item;
}

struct ast_node_case_clause *parser_readcaseclause(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_case_clause *case_clause =
        malloc(sizeof(struct ast_node_case_clause));
    case_clause->list = NULL;
    struct ast_node_case_item *case_item = parser_readcaseitem(p);
    if (!case_item)
    {
        p->index = tmp;
        return NULL;
    }
    case_clause->list = case_item;
    struct ast_node_case_item *last_case_item = NULL;
    while (parser_readtext(p,";;"))
    {
        eat_newlines(p);
        if ((last_case_item = parser_readcaseitem(p)))
        {
            case_item->next = last_case_item;
            case_item = last_case_item;
        }
    }
    eat_newlines(p);
    return case_clause;
}

struct ast_node_rule_case *parser_readrulecase(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_rule_case *rule_case =
        malloc(sizeof(struct ast_node_rule_case));
    rule_case->case_clause = NULL;
    if (!(rule_case->word = parser_readword(p)))
    {
        p->index = tmp;
        return NULL;
    }
    eat_newlines(p);
    if (!parser_readtext(p,"in"))
    {
        p->index = tmp;
        return NULL;
    }
    eat_newlines(p);
    rule_case->case_clause = parser_readcaseclause(p);
    if (!parser_readtext(p,"esac"))
    {
        p->index = tmp;
        return NULL;
    }
    return rule_case;
}

struct ast_node_else_clause *parser_readelseclause(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_else_clause *else_clause =
        malloc(sizeof(struct ast_node_else_clause));
    else_clause->second = NULL;
    else_clause->else_clause = NULL;
    if (parser_readtext(p,"else")
        && (else_clause->first = parser_readcompoundlist(p)))
        return else_clause;
    if (parser_readtext(p,"elif")
        && (else_clause->first = parser_readcompoundlist(p))
        && parser_readtext(p,"then")
        && (else_clause->second = parser_readcompoundlist(p)))
    {
        else_clause->else_clause = parser_readelseclause(p);
        return else_clause;
    }
    p->index = tmp;
    return NULL;
}
struct ast_node_do_group *parser_readdogroup(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_do_group* do_group =
        malloc(sizeof(struct ast_node_do_group));
    struct ast_node_compound_list *compound_list = NULL;
    if (!parser_readtext(p,"do")
        || !(compound_list = parser_readcompoundlist(p))
        || !parser_readtext(p,"done"))
    {
        p->index = tmp;
        return NULL;
    }
    do_group->compound_list = compound_list;
    return do_group;
}

struct ast_node_rule_until *parser_readruleuntil(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_rule_until *rule_until =
        malloc(sizeof(struct ast_node_rule_until));
    rule_until->compound_list = NULL;
    rule_until->do_group = NULL;
    struct ast_node_compound_list *compound_list = NULL;
    struct ast_node_do_group *do_group = NULL;
    if (!(compound_list = parser_readcompoundlist(p))
        || !(do_group = parser_readdogroup(p)))
    {
        p->index = tmp;
        return NULL;
    }
    rule_until->compound_list = compound_list;
    rule_until->do_group = do_group;
    return rule_until;
}

struct ast_node_rule_while *parser_readrulewhile (struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_rule_while *rule_while =
        malloc(sizeof(struct ast_node_rule_while));
    rule_while->compound_list = NULL;
    rule_while->do_group = NULL;
    struct ast_node_compound_list *compound_list = NULL;
    struct ast_node_do_group *do_group = NULL;
    if (!(compound_list = parser_readcompoundlist(p))
        || !(do_group = parser_readdogroup(p)))
    {
        p->index = tmp;
        return NULL;
    }
    rule_while->compound_list = compound_list;
    rule_while->do_group = do_group;
    return rule_while;
}

int get_shell_command_type(char *word)
{
    int i = 0;
    char *all_of_types[] = {"for","while","case","if","until"};
    while (i < 5)
    {
        if (strcmp(word,all_of_types[i]) == 0)
            return (i + 1);
        i++;
    }
    return 0;
}

struct ast_node_shell_command *parser_readshellcommand(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    bool c = false;
    struct ast_node_shell_command *shell_command =
        malloc(sizeof(struct ast_node_shell_command));
    shell_command->type = VOID;
    shell_command->child.compound_list = NULL;
    struct ast_node_compound_list *compound_list = NULL;
    if ((c = parser_readchar(p,'(')) ||  parser_readchar(p,'{'))
    {
        if (!(compound_list = parser_readcompoundlist(p)))
        {
            p->index = tmp;
            return NULL;
        }
        shell_command->child.compound_list = compound_list;
        if ((c && !(parser_readchar(p,')')))
            || (!c && !(parser_readchar(p,'}'))))
        {
            p->index = tmp;
            return NULL;
        }
        return shell_command;
    }
    struct ast_node_word *command_word = NULL;
    if (!(command_word = parser_readword(p)))
    {
        p->index = tmp;
        return NULL;
    }
    int b = 0;
    switch ((b = get_shell_command_type(command_word->str)))
    {
    case FOR:
        shell_command->child._for = parser_readrulefor(p);
        break;
    case WHILE:
        shell_command->child._while = parser_readrulewhile (p);
        break;
    case CASE:
        shell_command->child._case = parser_readrulecase(p);
        break;
    case IF:
        shell_command->child._if = parser_readruleif (p);
        break;
    case UNTIL:
        shell_command->child._until = parser_readruleuntil(p);
        break;
    default:
        p->index = tmp;
        return NULL;
    }
    shell_command->type = b;
    return shell_command;
}

struct ast_node_funcdec *parser_readfuncdec (struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_funcdec *funcdec = malloc(sizeof(struct ast_node_funcdec));
    funcdec->word = NULL;
    funcdec->shell_command = NULL;
    parser_readtext(p,"function");
    if (!((funcdec->word = parser_readword(p)) && parser_readchar(p,'(') && parser_readchar(p,')')))
    {
        p->index = tmp;
        return NULL;
    }
    eat_newlines(p);
    if (!(funcdec->shell_command = parser_readshellcommand(p)))
    {
        p->index = tmp;
        return NULL;
    }
    return funcdec;
}

int get_io_number(struct parser_s *p)
{
    int tmp= p->index;
    if (parser_readinteger(p))
    {
        char *num = strndup(&p->input[tmp],p->index - tmp);
        return atoi(num);
    }
    p->index = tmp;
    return -1;
}

char *get_redirection(struct parser_s *p)
{
    int tmp = p->index;
    while (parser_readinset(p,"<>-&|"))
        p->index++;
    return strndup(&p->input[tmp],p->index - tmp);
}

int get_redirection_type(struct parser_s *p)
{
    int i = 0;
    char *all_of_types[] = {">","<",">>","<<","<<-",">&","<&",">|","<>"};
    char *redirection = get_redirection(p);
    while (i < 9)
    {
        if (strcmp(redirection, all_of_types[i]) == 0)
            return i + 1;
        i++;
    }
    return 0;
}

struct ast_node_redirection *parser_readredirection(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_redirection *redirection =
        malloc(sizeof(struct ast_node_redirection));
    redirection->word_heredoc.word = NULL;
    redirection->next = NULL;
    redirection->io_number = get_io_number(p);
    if (redirection->io_number == -1)
    {
        if (!(redirection->redirection_type = get_redirection_type(p)))
        {
            p->index = tmp;
            return NULL;
        }
        else
            redirection->io_number = 1;
    }
    struct ast_node_word *word = parser_readword(p);
    if (!word)
    {
        p->index = tmp;
        return NULL;
    }
    switch (redirection->redirection_type)
    {
    case VOID:
        p->index = tmp;
        return NULL;
    case LTLT:
        redirection->word_heredoc.heredoc = word->str;
        redirection->word_heredoc.word = NULL;
        break;
    case LTLTDASH:
        redirection->word_heredoc.heredoc = word->str;
        redirection->word_heredoc.word = NULL;
        break;
    default:
        redirection->word_heredoc.word = word;
        redirection->word_heredoc.heredoc = NULL;
        break;
    }
    return redirection;
}

struct ast_node_prefix *parser_readprefix(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_prefix *prefix = malloc(sizeof(struct ast_node_element));
    prefix->next = NULL;
    if (!(prefix->redirection = parser_readredirection(p))
        && !(prefix->assignement_word = parser_readassignementword(p)))
    {
        p->index = tmp;
        return NULL;
    }
    return prefix;
}

bool is_reserved_word(char *word)
{
    return !strcmp(word, "then") || !strcmp(word, "do")
        || !strcmp(word, "esac") || !strcmp(word, "fi")
        || !strcmp(word, "done");
}

struct ast_node_element *parser_readelement(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_element *element = malloc(sizeof(struct ast_node_element));
    element->next = NULL;
    if (!(element->redirection = parser_readredirection(p))
        && !(element->word = parser_readword(p)))
    {
        p->index = tmp;
        return NULL;
    }
    if (is_reserved_word(element->word->str))
    {
        p->index = tmp;
        return NULL;
    }
    return element;
}

struct ast_node_simple_command *parser_readsimplecommand(struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_simple_command *simple_command =
        malloc(sizeof(struct ast_node_simple_command));
    simple_command->element_list = NULL;
    simple_command->prefix_list = NULL;
    struct ast_node_prefix *new = parser_readprefix(p);
    simple_command->prefix_list = new;
    struct ast_node_prefix *last = simple_command->prefix_list;    
    while ((new = parser_readprefix(p)))
    {
        while (last->next)
            last = last->next;
        last->next = new;
    }
    struct ast_node_element *new_element = parser_readelement(p);
    simple_command->element_list = new_element;
    struct ast_node_element *last_element = new_element;
    while ((new_element = parser_readelement(p)))
    {
        while (last_element->next)
            last_element = last_element->next;

        last_element->next = new_element;
    }
    if (!(simple_command->element_list) && !(simple_command->prefix_list))
    {
        p->index = tmp;
        return NULL;
    }
    return simple_command;
}

int type_of_command(struct parser_s *p)
{
    int tmp = p->index;
    if (parser_readchar(p,'{') || parser_readchar(p,'(')
        || ((parser_readtext(p,"while") || parser_readtext(p,"for")
        || parser_readtext(p,"if") || parser_readtext(p,"until")
        || parser_readtext(p,"case")) && parser_readchar(p,' ')))
    {
        p->index = tmp;
        return SHELL;
    }
    if ((parser_readtext(p,"function") && parser_readchar(p,' '))
        || (parser_readidentifier(p) && parser_readchar(p,'(')))
    {
        p->index = tmp;
        return FUNCDEC;
    }
    p->index = tmp;
    return SIMPLE;
}

struct ast_node_command *parser_readcommand (struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_command *command = malloc(sizeof(struct ast_node_command));
    command->simple_command = NULL;
    command->shell_command = NULL;
    command->funcdec = NULL;
    command->next = NULL;
    command->redirection_list = NULL;
    command->pipe = false;
    enum command_type type = type_of_command(p);
    if (type == SIMPLE)
    {
        if (!(command->simple_command = parser_readsimplecommand(p)))
        {
            p->index = tmp;
            return NULL;
        }
        return command;
    }
    if (type == SHELL)
    {
        if (!(command->shell_command = parser_readshellcommand(p)))
        {
            p->index = tmp;
            return NULL;
        }
    }
    if (type == FUNCDEC)
    {
        if (!(command->funcdec = parser_readfuncdec(p)))
        {
            p->index = tmp;
            return NULL;
        }
    }
    struct ast_node_redirection *new = NULL;
    command->redirection_list = new;
    struct ast_node_redirection *last = command->redirection_list;
    while ((new = parser_readredirection(p)))
    {
        while (last->next != NULL)
            last = last->next;

        last->next = new;
    }
    return command;
}

struct ast_node_pipeline *parser_readpipeline (struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    struct ast_node_pipeline *pipeline =
        malloc(sizeof(struct ast_node_pipeline));
    pipeline->relation = VOID;
    pipeline->command = NULL;
    pipeline->reverse = false;
    pipeline->next = NULL;
    if (parser_readchar(p,'!'))
        pipeline->reverse = true;
    struct ast_node_command *new = NULL;
    if (!(new = parser_readcommand(p)))
    {
        p->index = tmp;
        return NULL;
    }
    pipeline->command = new;
    struct ast_node_command *last = pipeline->command;
    while ((parser_readchar(p,'|')) && (new = parser_readcommand(p)))
    {
        while (last->next != NULL)
            last = last->next;

        new->pipe = true;
        eat_newlines(p);
        last->next = new;
    }
    return pipeline;
}

struct ast_node_and_or *parser_readandor (struct parser_s *p)
{
    eat_spaces(p);
    eat_newlines(p);
    int tmp = p->index;
    struct ast_node_and_or *and_or = malloc(sizeof(struct ast_node_and_or));
    and_or->linked = false;
    and_or->next = NULL;
    and_or->pipeline = NULL;        
    struct ast_node_pipeline *new = NULL;
    if (!(new = parser_readpipeline(p)))
    {
        p->index = tmp;
        return NULL;
    }
    and_or->pipeline = new;
    struct ast_node_pipeline *last = and_or->pipeline;
    while (last->next)
        last = last->next;

    bool c = false;
    while ((c = (parser_readtext(p,"||")) || (parser_readtext(p,"&&")))
        && (new = parser_readpipeline(p)))
    {
        if (c)
            new->relation = TOKEN_DOUBLE_PIPE;
        else
            new->relation = TOKEN_DOUBLE_AMPERSAND;
        eat_newlines(p);
        while (last->next)
            last = last->next;
        last->next = new;
    }
    eat_spaces(p);
    return and_or;
}

struct ast_node_list *parser_readlist (struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    p->ast->list = malloc(sizeof(struct ast_node_list));
    p->ast->list->and_ors = NULL;
    struct ast_node_and_or *new = NULL;
    if (!(new = parser_readandor(p)))
    {
        p->index=tmp;
        return NULL;
    }
    p->ast->list->and_ors = new;
    struct ast_node_and_or *last = p->ast->list->and_ors;
    bool c = false;
    while ((parser_readchar(p,';') || (c = (parser_readchar(p,'&'))))
        && (new = parser_readandor(p)))
    {
        while (last->next != NULL)
            last = last->next;
        new->linked = c;
        last->next = new;
        
        c = false;
    }
    return p->ast->list;
}

bool parser_readinput (struct parser_s *p)
{
    eat_spaces(p);
    int tmp = p->index;
    p->ast = malloc(sizeof(struct ast_node_input));
    p->ast->list = NULL;
    if (parser_readchar(p, '\n'))
        return true;
    if (parser_eof(p))
        return true;
    if ((p->ast->list = parser_readlist(p))
        && ((parser_readchar(p, '\n') || parser_eof(p))))
        return true;
    p->index = tmp;
    return false;
}
