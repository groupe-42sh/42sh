#include "ast.h"
#include "parser.h"
#include "extract.h"
#include <stdlib.h>
#include <stdio.h>
#include "print_ast.h"

void *voidifier(void *ptr)
{
    return ptr;
}

FILE *file = NULL;
int i = 0;
int tab [24] = {0};

void assignement_word_traversal_print
    (struct ast_node_assignement_word *assignement_word)
{
    fprintf(file, "ASSIGN_WORD%d -> VAR%d -> %s;\n", tab[19], ++tab[24], assignement_word->var_name);
    fprintf(file, "ASSIGN_WORD%d -> VALUE%d -> %d;\n", tab[19], ++tab[25], assignement_word->value);
}

void word_traversal_print(struct ast_node_word *word)
{
    fprintf(file, "WORD%d -> %s_%d;\n", tab[16], word->str, tab[16]);
}

void case_item_traversal_print(struct ast_node_case_item *case_item)
{
    struct ast_node_word *word_list = case_item->word_list;
    struct ast_node_compound_list *compound_list = case_item->compound_list;

    while (word_list)
    {
        fprintf(file, "WORD%d -> VAR:%s;\n", ++tab[16], word_list->str);
        word_traversal_print(word_list);
        word_list = word_list->next;
    }

    if (compound_list)
    {
        fprintf(file, "CASE_ITEM%d -> COMPOUND_LIST%d;\n", ++tab[23], ++tab[18]);
        compound_list_traversal_print(compound_list);
    }
}

void case_clause_traversal_print(struct ast_node_case_clause *case_clause)
{
    struct ast_node_case_item *list = case_clause->list;

    while (list)
    {
        fprintf(file, "CASE_CLAUSE%d -> CASE_ITEM%d;\n", ++tab[22], ++tab[23]);
        case_item_traversal_print(list);
        list = list->next;
    }
}

void do_group_traversal_print(struct ast_node_do_group *do_group)
{
    struct ast_node_compound_list *compound_list = do_group->compound_list;

    fprintf(file, "DO_GROUP%d -> COMPOUND_LIST%d;\n", ++tab[21], ++tab[18]);
    compound_list_traversal_print(compound_list);
}

void else_clause_traversal_print(struct ast_node_else_clause *else_clause)
{
    struct ast_node_compound_list *first = else_clause->first;
    struct ast_node_compound_list *second = else_clause->second;
    struct ast_node_else_clause *elseclause = else_clause->else_clause;

    compound_list_traversal_print(first);

    ++tab[18];
    if (second)
    {
        fprintf(file, "SECOND%d -> CONDITION%d;\n", tab[18], tab[18]);
        compound_list_traversal_print(second);
    }
    if (elseclause)
    {
        fprintf(file, "ELSE_CLAUSE%d -> CONDITION%d;\n", tab[18], tab[18]);
        else_clause_traversal_print(elseclause);
    }
}

void rule_if_traversal_print(struct ast_node_rule_if *_if)
{
    struct ast_node_compound_list *condition = _if->condition;
    struct ast_node_compound_list *body = _if->body;
    struct ast_node_else_clause *else_clause = _if->else_clause;

    fprintf(file, "IF%d -> CONDITION%d -> COMPOUND_LIST%d;\n", tab[14], ++tab[27], ++tab[18]);
    compound_list_traversal_print(condition);
    fprintf(file, "IF%d -> BODY%d -> COMPOUND_LIST%d;\n", tab[14], ++tab[26], ++tab[18]);
    compound_list_traversal_print(body);

    if (else_clause)
    {
        else_clause_traversal_print(else_clause);
        fprintf(file, "IF%d -> ELSE_CLAUSE%d;\n", tab[14], ++tab[18]);
    }
}

void rule_case_traversal_print(struct ast_node_rule_case *_case)
{
    struct ast_node_word *word = _case->word;
    struct ast_node_case_clause *case_clause = _case->case_clause;

    word_traversal_print(word);
    fprintf(file, "CASE%d -> VAR -> %s;\n", tab[13], word->str);
    if (case_clause)
    {
        case_clause_traversal_print(case_clause);
        fprintf(file, "CASE%d -> CASE_CLAUSE%d;\n", tab[13], tab[22]);
    }
}

void rule_until_traversal_print(struct ast_node_rule_until *_until)
{
    struct ast_node_compound_list *compound_list = _until->compound_list;
    struct ast_node_do_group *do_group = _until->do_group;

    fprintf(file, "UNTIL%d -> COMPOUND_LIST%d;\n", tab[15], ++tab[18]);
    compound_list_traversal_print(compound_list);
    fprintf(file, "UNTIL%d -> COMPOUND_LIST%d;\n", tab[15], ++tab[18]);
    do_group_traversal_print(do_group);
}

void rule_while_traversal_print(struct ast_node_rule_while *_while)
{
    struct ast_node_compound_list *compound_list = _while->compound_list;
    struct ast_node_do_group *do_group = _while->do_group;

    fprintf(file, "WHILE%d -> COMPOUND_LIST%d;\n", tab[12], ++tab[18]);
    compound_list_traversal_print(compound_list);
    fprintf(file, "WHILE%d -> DO_GROUP%d;\n", tab[12], ++tab[21]);
    do_group_traversal_print(do_group);
}

void rule_for_traversal_print(struct ast_node_rule_for *_for)
{
    struct ast_node_word *word = _for->word;
    struct ast_node_word *word_list = _for->word_list;
    struct ast_node_do_group *do_group = _for->do_group;

    word_traversal_print(word);

    while (word_list)
    {
        fprintf(file, "FOR%d -> WORD%d -> VAR%d -> %s\n", ++tab[11], ++tab[16],
        ++tab[24], word_list->str);
        word_traversal_print(word);
        word_list = word_list->next;
    }

    fprintf(file, "FOR%d -> DO_GROUP%d", tab[11], (++tab[21]));
    do_group_traversal_print(do_group);
}

void element_traversal_print(struct ast_node_element* element)
{
    struct ast_node_word *word = element->word;
    struct ast_node_redirection *redirection = element->redirection;

    while (word)
    {
        fprintf(file, "ELEMENT%d -> WORD%d;\n", tab[8], ++tab[16]);
        word_traversal_print(word);
        word = word->next;
    }

    if (redirection)
    {
        fprintf(file, "ELEMENT%d -> REDIRECTION%d;\n", tab[8], ++tab[5]);
        redirection_traversal_print(redirection);
    }
}

void prefix_traversal_print(struct ast_node_prefix* prefix)
{
    struct ast_node_assignement_word *assignement_word =
        prefix->assignement_word;
    struct ast_node_redirection *redirection = prefix->redirection;

    while (assignement_word)
    {
        fprintf(file, "PREFIX%d -> ASSIGN_WORD%d;\n", tab[7], ++tab[19]);
        assignement_word_traversal_print(assignement_word);
        assignement_word = assignement_word->next;
    }

    if (redirection)
    {
        fprintf(file, "PREFIX%d -> REDIRECTION%d;\n", tab[7], ++tab[5]);
        redirection_traversal_print(redirection);
    }
}

void compound_list_traversal_print(struct ast_node_compound_list *compound_list)
{
    struct ast_node_and_or *and_or = compound_list->and_or;

    while (and_or)
    {
        fprintf(file, "COMPOUND_LIST%d -> AND_OR%d;\n", tab[18], ++tab[2]);
        and_or_traversal_print(and_or);
        and_or = and_or->next;
    }
}

void redirection_traversal_print(struct ast_node_redirection *redirection)
{
    fprintf(file, "REDIRECTION%d -> IO_NUMBER%d;\n", tab[5],
        redirection->io_number);

    switch (redirection->redirection_type)
    {
        case GT:
            fprintf(file, "REDIRECTION%d -> GT%d;\n", tab[5], tab[5]);
            break;
        case LT:
            fprintf(file, "REDIRECTION%d -> LT%d;\n", tab[5], tab[5]);
            break;
        case GTGT:
            fprintf(file, "REDIRECTION%d -> GTGT%d;\n", tab[5], tab[5]);
            break;
        case LTLT:
            fprintf(file, "REDIRECTION%d -> LTLT%d;\n", tab[5], tab[5]);
            break;
        case LTLTDASH:
            fprintf(file, "REDIRECTION%d -> LTLTMIN%d;\n", tab[5], tab[5]);
            break;
        case GTESP:
            fprintf(file, "REDIRECTION%d -> GTESP%d;\n", tab[5], tab[5]);
            break;
        case LTESP:
            fprintf(file, "REDIRECTION%d -> LTESP%d;\n", tab[5], tab[5]);
            break;
        case GTPIPE:
            fprintf(file, "REDIRECTION%d -> GTPIPE%d;\n", tab[5], tab[5]);
            break;
        case LTGT:
            fprintf(file, "REDIRECTION%d -> LTGT%d;\n", tab[5], tab[5]);
            break;
        default:
            printf("NON");
    }

    if (redirection->word_heredoc.word)
        fprintf(file, "REDIRECTION%d -> WORD -> %s;\n", tab[5],
        redirection->word_heredoc.word->str);
    //if (redirection->word_heredoc.heredoc)
    //    fprintf(file, "REDIRECTION%d -> HEREDOC:%s;\n", tab[5],
    //    redirection->word_heredoc.heredoc);
}

void shell_command_traversal_print(struct ast_node_shell_command *shell_command)
{
    enum shell_command_type type = shell_command->type;

    switch (type)
    {
        case FOR:
            fprintf(file, "SHELL_COMMAND%d -> FOR%d;\n", tab[10], ++tab[11]);
            rule_for_traversal_print(shell_command->child._for);
            break;
        case WHILE:
            fprintf(file, "SHELL_COMMAND%d -> WHILE%d;\n", tab[10], ++tab[12]);
            rule_while_traversal_print(shell_command->child._while);
            break;
        case CASE:
            fprintf(file, "SHELL_COMMAND%d -> CASE%d;\n", tab[10], ++tab[13]);
            rule_case_traversal_print(shell_command->child._case);
            break;
        case IF:
            fprintf(file, "SHELL_COMMAND%d -> IF%d;\n", tab[10], ++tab[14]);
            rule_if_traversal_print(shell_command->child._if);
            break;
        case UNTIL:
            fprintf(file, "SHELL_COMMAND%d -> UNTIL%d;\n", tab[10], ++tab[15]);
            rule_until_traversal_print(shell_command->child._until);
            break;
        case VOID:
            fprintf(file, "SHELL_COMMAND%d -> COMPOUND_LIST%d;\n", tab[10],
            ++tab[18]);
            compound_list_traversal_print(shell_command->child.compound_list);
             break;
    }
}

void funcdec_traversal_print(struct ast_node_funcdec *funcdec)
{
    fprintf(file, "FUNCDEC%d -> SHELL_COMMAND%d;\n", tab[9], ++tab[10]);
    word_traversal_print(funcdec->word);
    shell_command_traversal_print(funcdec->shell_command);
}

void simple_command_traversal_print(struct ast_node_simple_command *simple_command)
{
    struct ast_node_element *element_list = simple_command->element_list;
    struct ast_node_prefix *prefix_list = simple_command->prefix_list;

    while (prefix_list)
    {
        fprintf(file, "SIMPLE_COMMAND%d -> PREFIX%d;\n", tab[6], ++tab[7]);
        prefix_traversal_print(prefix_list);
        prefix_list = prefix_list->next;
    }

    while (element_list)
    {
        fprintf(file, "SIMPLE_COMMAND%d -> ELEMENT%d;\n", tab[6], ++tab[8]);
        element_traversal_print(element_list);
        element_list = element_list->next;
    }
}

void command_traversal_print(struct ast_node_command *command)
{
    struct ast_node_simple_command *simple_command = command->simple_command;
    struct ast_node_shell_command *shell_command = command->shell_command;
    struct ast_node_funcdec *funcdec = command->funcdec;
    struct ast_node_redirection *redirection_list = command->redirection_list;

    if (simple_command)
    {
        fprintf(file, "COMMAND%d -> SIMPLE_COMMAND%d;\n", tab[4],
        ++tab[6]);
        simple_command_traversal_print(simple_command);
    }

    if (shell_command)
    {
        fprintf(file, "COMMAND%d -> SHELL_COMMAND%d;\n", tab[4],
        ++tab[10]);
        shell_command_traversal_print(shell_command);
    }

    if (funcdec)
    {
        fprintf(file, "COMMAND%d -> FUNCDEC%d;\n", tab[4],
        ++tab[9]);
        funcdec_traversal_print(funcdec);
    }

    if (redirection_list)
    {
        while (redirection_list)
        {
            fprintf(file, "COMMAND%d -> REDIRECTION%d;\n", tab[4], ++tab[5]);
            redirection_traversal_print(redirection_list);
            redirection_list = redirection_list->next;
        }
    }
}

void pipeline_traversal_print(struct ast_node_pipeline *pipeline)
{
    if (pipeline->reverse)
        printf("! ");
    struct ast_node_command *command = pipeline->command;

    while (command)
    {
        fprintf(file, "PIPELINE%d -> COMMAND%d;\n", tab[3], ++tab[4]);
        command_traversal_print(command);
        command = command->next;
    }
}

void and_or_traversal_print(struct ast_node_and_or *and_or)
{
    if (and_or->linked)
        printf(" &&\n");
    else
        printf(" ||\n");

    if (!and_or)
        return;
    
    struct ast_node_pipeline *pipeline = and_or->pipeline;

    while (pipeline)
    {
        fprintf(file, "AND_OR%d -> PIPELINE%d;\n", tab[2], ++tab[3]);
        pipeline_traversal_print(pipeline);
        pipeline = pipeline->next;
    }
}

void list_traversal_print(struct ast_node_list *list)
{
    if (!list)
        return;
    struct ast_node_and_or *and_or = list->and_ors;

    while (and_or)
    {
        fprintf(file, "LIST%d -> AND_OR%d;\n", tab[1], ++tab[2]);
        and_or_traversal_print(and_or);
        and_or = and_or->next;
    }
}

void full_traversal_print(struct parser_s *p)
{
    file = fopen("graph.dot", "w");
    if (!file)
    {
        printf("Unable to open file !\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "digraph our_graph {\n");
    
    fprintf(file, "INPUT%d -> LIST%d;\n", tab[0], ++tab[1]);
    list_traversal_print(p->ast->list);

    fprintf(file, "\n}");

    fclose(file);
}
