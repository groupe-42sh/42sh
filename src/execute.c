#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "extract.h"
#include "parser.h"
#include "ast.h"

#include <stdio.h>
#include <time.h>

int exec_input(struct ast_node_input *input)
{
    if (!input)
    {
        return -1;
    }
    if (input->list)
    {
        return exec_list(input->list);
    }
    return 1;
}

int exec_list(struct ast_node_list *list)
{
    if (!list)
    {
        return -1;
    }
    
    struct ast_node_and_or *last = list->and_ors;

    while (last)
    {
        if (exec_and_or(list) == -1)
        {
            return -1;
        }

        last = last->next;
    }
    return 1;
}

int exec_and_or(struct ast_node_and_or *and_or)
{
    if (!and_or)
    {
        return -1;
    }

    struct ast_node_pipeline *last = and_or->pipeline;
    
    while (last)
    {
        if (exec_pipeline(last) == -1)
        {
            return -1;
        }

        last = last->next;
    }
    return 1;
}
int exec_rule_if(struct ast_node_rule_if *rule_if)
{
    int i = exec_compound_list(rule_if->condition);
    if (!rule_if || i == -1)
    {
        return -1;
    }
    if (i == 1)
    {
        if (exec_compound_list(rule_if->body) == -1)
        {
            return -1;
        }
    }
    if (rule_if->else_clause)
    {
        return exec_else_clause(rule_if->else_clause);
    }
    return 1;
}

int exec_rule_for(struct ast_node_rule_for *rule_for)
{
    int i = exec_word(rule_for->word);
    if (!rule_for || i == -1)
    {
        return -1;
    }

    struct ast_node_word *last = rule_for->word_list;

    while (i == 1)
    {
        while (last)
        {
            if (exec_word(last) == -1)
            {
                return -1;
            }
            last = last->next;
        }
    }
    return exec_do_group(rule_for->do_group);

}

int exec_rule_while(struct ast_node_rule_while *rule_while)
{
    int i = exec_compound_list(rule_while->compound_list);
    if (!rule_while || i == -1)
    {
        return -1;
    }
    return exec_do_group(rule_while->do_group);
}

int exec_rule_until(struct ast_node_rule_until *rule_until)
{
    int i = exec_compound_list(rule_until->compound_list);
    if (!rule_until || i == -1)
    {
        return -1;
    }
    return exec_do_group(rule_until->do_group);
}

int exec_rule_case(struct ast_node_rule_case *rule_case)
{
    int i = exec_word(rule_case->word);
    if (!rule_if || i == -1)
    {
        return -1;
    }
    if (rule_case->case_clause)
    {
        return exec_case_clause(rule_case->case_clause);
    }
    return 1;
}
int exec_compound_list(struct ast_node_compound_list *compound_list)
{
    if (!compound_list)
    {
        return -1;
    }
    return exec_and_or(compound_list->and_or);
}
int exec_do_group(struct ast_node_do_group *do_group)
{
    if (!do_group)
    {
        return -1;
    }
    return exec_compound_list(do_group->compound_list);
}

int exec_case_clause(struct ast_node_case_clause *case_clause)
{
    if (!case_clause)
    {
        return -1;
    }
    struct ast_node_case_clause *last = case_clause->list;
    
    while (last)
    {
        if (exec_case_item(last) == -1)
        {
            return -1;
        }
        last = last->next;
    }
    return 1;
}

int exec_case_item(struct ast_node_case_item *case_item)
{
    if (!case_item)
    {
        return -1;
    }

    struct ast_node_word *last = case_item->word_list;

    while (last)
    {
        if (exec_word(last) == -1)
        {
            return -1;
        }
        last = last->next;
    }

    if (case_item->compound_list)
    {
        return exec_compound_list(case_item->compound_list);
    }
    return 1;
}

int exec_else_clause(struct ast_node_else_clause *else_clause)
{
    if (!else_clause->second)
    {
        return exec_compound_list(else_clause->first);
    }

    struct ast_node_rule_if *if_node = {else_clause->first,
                                        else_clause->second,
                                        else_clause->else_clause
                                        };
    return exec_rule_if(if_node);
}
