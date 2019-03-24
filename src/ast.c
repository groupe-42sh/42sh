#include "ast.h"
#include "parser.h"
#include "extract.h"
#include <stdlib.h>
#include <stdio.h>

void assignement_word_traversal
    (struct ast_node_assignement_word *assignement_word)
{
    printf("ASSIGNEMENT_VAR = %s\n", assignement_word->var_name);
    printf("ASSIGNEMENT_VALUE = %s\n", assignement_word->value);
}

void word_traversal(struct ast_node_word *word)
{
    printf("WORD: %s\n", word->str);
}

void case_item_traversal(struct ast_node_case_item *case_item)
{
    printf("CASE_ITEM:\n");

    struct ast_node_word *word_list = case_item->word_list;
    struct ast_node_compound_list *compound_list = case_item->compound_list;

    while (word_list)
    {
        word_traversal(word_list);
        word_list = word_list->next;
    }

    if (compound_list)
        compound_list_traversal(compound_list);
}

void case_clause_traversal(struct ast_node_case_clause *case_clause)
{
    printf("CASE_CLAUSE:\n");

    struct ast_node_case_item *list = case_clause->list;

    while (list)
    {
        case_item_traversal(list);
        list = list->next;
    }
}

void do_group_traversal(struct ast_node_do_group *do_group)
{
    printf("DO_GROUP:\n");

    struct ast_node_compound_list *compound_list = do_group->compound_list;

    compound_list_traversal(compound_list);
}

void else_clause_traversal(struct ast_node_else_clause *else_clause)
{
    printf("ELSE_CLAUSE:\n");

    struct ast_node_compound_list *first = else_clause->first;
    struct ast_node_compound_list *second = else_clause->second;
    struct ast_node_else_clause *elseclause = else_clause->else_clause;

    compound_list_traversal(first);

    if (second)
    {
        compound_list_traversal(second);
    }
    if (elseclause)
        else_clause_traversal(elseclause);
}

void rule_if_traversal(struct ast_node_rule_if *_if)
{
    printf("IF:\n");

    struct ast_node_compound_list *condition = _if->condition;
    struct ast_node_compound_list *body = _if->body;
    struct ast_node_else_clause *else_clause = _if->else_clause;

    compound_list_traversal(condition);
    compound_list_traversal(body);

    if (else_clause)
        else_clause_traversal(else_clause);
}

void rule_case_traversal(struct ast_node_rule_case *_case)
{
    printf("CASE:\n");

    struct ast_node_word *word = _case->word;
    struct ast_node_case_clause *case_clause = _case->case_clause;

    word_traversal(word);
    if (case_clause)
        case_clause_traversal(case_clause);
}

void rule_until_traversal(struct ast_node_rule_until *_until)
{
    printf("UNTIL\n");
    struct ast_node_compound_list *compound_list = _until->compound_list;
    struct ast_node_do_group *do_group = _until->do_group;

    compound_list_traversal(compound_list);
    do_group_traversal(do_group);
}

void rule_while_traversal(struct ast_node_rule_while *_while)
{
    printf("WHILE\n");
    struct ast_node_compound_list *compound_list = _while->compound_list;
    struct ast_node_do_group *do_group = _while->do_group;

    compound_list_traversal(compound_list);
    do_group_traversal(do_group);
}

void rule_for_traversal(struct ast_node_rule_for *_for)
{
    printf("FOR:\n");
    struct ast_node_word *word = _for->word;
    struct ast_node_word *word_list = _for->word_list;
    struct ast_node_do_group *do_group = _for->do_group;

    word_traversal(word);

    while (word_list)
    {
        word_traversal(word);
        word_list = word_list->next;
    }

    do_group_traversal(do_group);
}

void element_traversal(struct ast_node_element* element)
{
    printf("ELEMENT:\n");
    struct ast_node_word *word = element->word;
    struct ast_node_redirection *redirection = element->redirection;

    while (word)
    {
        word_traversal(word);
        word = word->next;
    }

    if (redirection)
        redirection_traversal(redirection);
}

void prefix_traversal(struct ast_node_prefix* prefix)
{
    printf("PREFIX:\n");
    struct ast_node_assignement_word *assignement_word =
        prefix->assignement_word;
    struct ast_node_redirection *redirection = prefix->redirection;

    while (assignement_word)
    {
        assignement_word_traversal(assignement_word);
        assignement_word = assignement_word->next;
    }

    if (redirection)
        redirection_traversal(redirection);
}

void compound_list_traversal(struct ast_node_compound_list *compound_list)
{
    printf("COMPOUND_LIST\n");

    struct ast_node_and_or *and_or = compound_list->and_or;

    while (and_or)
    {
        and_or_traversal(and_or);
        and_or = and_or->next;
    }
}

void redirection_traversal(struct ast_node_redirection *redirection)
{
    printf("IO_NUMBER : %d\n", redirection->io_number);
    printf("REDIRECTION :\n");
    switch (redirection->redirection_type)
    {
        case GT:
            printf(">\n");
            break;
        case LT:
            printf("<\n");
            break;
        case GTGT:
            printf(">>\n");
            break;
        case LTLT:
            printf("<<\n");
            break;
        case LTLTDASH:
            printf("<<-\n");
            break;
        case GTESP:
            printf(">&\n");
            break;
        case LTESP:
            printf("<&\n");
            break;
        case GTPIPE:
            printf(">|\n");
            break;
        case LTGT:
            printf("<>\n");
            break;
        default:
            printf("NON");
    }

    printf("WORD_HEREDOC : ");
    if (redirection->word_heredoc.word)
        printf("%s\n", redirection->word_heredoc.word->str);
    if (redirection->word_heredoc.heredoc)
        printf("\"%s\"\n", redirection->word_heredoc.heredoc);
}

void shell_command_traversal(struct ast_node_shell_command *shell_command)
{
    printf("SHELL_COMMAND\n");
    enum shell_command_type type = shell_command->type;

    switch (type)
    {
        case FOR:
            rule_for_traversal(shell_command->child._for);
            break;
        case WHILE:
            rule_while_traversal(shell_command->child._while);
            break;
        case CASE:
            rule_case_traversal(shell_command->child._case);
            break;
        case IF:
            rule_if_traversal(shell_command->child._if);
            break;
        case UNTIL:
            rule_until_traversal(shell_command->child._until);
            break;
        case VOID:
            compound_list_traversal(shell_command->child.compound_list);
            break;
    }
}

void funcdec_traversal(struct ast_node_funcdec *funcdec)
{
    printf("FUNCDEC\n");
    word_traversal(funcdec->word);
    shell_command_traversal(funcdec->shell_command);
}

void simple_command_traversal(struct ast_node_simple_command *simple_command)
{
    printf("SIMPLE_COMMAND\n");
    struct ast_node_element *element_list = simple_command->element_list;
    struct ast_node_prefix *prefix_list = simple_command->prefix_list;

    while (prefix_list)
    {
        prefix_traversal(prefix_list);
        prefix_list = prefix_list->next;
    }

    while (element_list)
    {
        element_traversal(element_list);
        element_list = element_list->next;
    }
}

void command_traversal(struct ast_node_command *command)
{
    printf("COMMAND\n");

    struct ast_node_simple_command *simple_command = command->simple_command;
    struct ast_node_shell_command *shell_command = command->shell_command;
    struct ast_node_funcdec *funcdec = command->funcdec;
    struct ast_node_redirection *redirection_list = command->redirection_list;

    if (simple_command)
    {
        simple_command_traversal(simple_command);
    }

    if (shell_command)
    {
        shell_command_traversal(shell_command);
    }

    if (funcdec)
    {
        funcdec_traversal(funcdec);
    }

    if (redirection_list)
    {
        while (redirection_list)
        {
            redirection_traversal(redirection_list);
            redirection_list = redirection_list->next;
        }
    }
}

void pipeline_traversal(struct ast_node_pipeline *pipeline)
{
    if (pipeline->reverse)
        printf("! ");
    printf("PIPELINE\n");
    struct ast_node_command *command = pipeline->command;

    while (command)
    {
        command_traversal(command);
        command = command->next;
    }
}

void and_or_traversal(struct ast_node_and_or *and_or)
{
    printf("AND_OR");
    if (and_or->linked)
        printf(" &&\n");
    else
        printf(" ||\n");

    struct ast_node_pipeline *pipeline = and_or->pipeline;

    while (pipeline)
    {
        pipeline_traversal(pipeline);
        pipeline = pipeline->next;
    }
}

void list_traversal(struct ast_node_list *list)
{
    printf("LIST\n");
    struct ast_node_and_or *and_or = list->and_ors;
    while (and_or)
    {
        and_or_traversal(and_or);
        and_or = and_or->next;
    }
}

void full_traversal(struct parser_s *p)
{
    printf("INPUT\n");
    list_traversal(p->ast->list);
}
