#ifndef AST_H
#define AST_H
#include "parser.h"

enum shell_command_type
{
    VOID,
    FOR,
    WHILE,
    CASE,
    IF,
    UNTIL
};

enum relation {
    TOKEN_DOUBLE_AMPERSAND,
    TOKEN_DOUBLE_PIPE
};

enum command_type {
    SIMPLE,
    SHELL,
    FUNCDEC
};

enum redirection {
    GT,
    LT,
    GTGT,
    LTLT,
    LTLTDASH,
    GTESP,
    LTESP,
    GTPIPE,
    LTGT
};

struct ast_node_input {
    struct ast_node_list *list; /*1 seule liste*/
};

struct ast_node_list {
    struct ast_node_and_or *and_ors;
};

struct ast_node_and_or {
    bool linked;
    struct ast_node_pipeline *pipeline;
    struct ast_node_and_or *next;
};

struct ast_node_pipeline {
    enum relation relation;
    struct ast_node_command *command;
    bool reverse; /*[!]*/
    struct ast_node_pipeline *next;
};

struct ast_node_command {
    bool piped;
    struct ast_node_simple_command *simple_command;/*1 seul*/
    struct ast_node_shell_command *shell_command;/*1 seul*/
    struct ast_node_funcdec *funcdec;/*1 seul*/
    struct ast_node_redirection *redirection_list;
    struct ast_node_command *next;
};

//struct ast_node_command_container {
//    enum command_type type;
//    struct ast_node_command *command;
//    struct ast_node_redirection *redirection;
//};

struct ast_node_simple_command {
    struct ast_node_element *element_list; 
    struct ast_node_prefix *prefix_list;
};

struct ast_node_element { 
    char *word;
    struct ast_node_redirection *redirection;
    struct ast_node_element *next;
};

struct ast_node_prefix { 
    char *assignement_word;
    struct ast_node_redirection *redirection;
    struct ast_node_prefix *next;
};

union ast_node_shell_command_child {
    struct ast_node_rule_for *_for;
    struct ast_node_rule_while *_while;
    struct ast_node_rule_case *_case;
    struct ast_node_rule_if *_if;
    struct ast_node_rule_until *_until;
    struct ast_node_compound_list *compound_list;
};

struct ast_node_shell_command {
    enum shell_command_type type;   
    union ast_node_shell_command_child child;
};

struct ast_node_funcdec {
    char *word;
    struct ast_node_shell_command *shell_command;
};

union word_heredoc {
    char *word;
    char *heredoc;
};

struct ast_node_redirection {
    int io_number;
    enum redirection redirection_type;
    union word_heredoc word_heredoc;
    struct ast_node_redirection *next;
};

struct ast_node_compound_list {
    struct ast_node_and_or *and_or;
};

struct ast_node_rule_for {
    char *word;
    char **words;
    struct ast_node_do_group *do_group;
};

struct ast_node_rule_while {
    struct ast_node_compound_list *compound_list;
    struct ast_node_do_group *do_group;
};

struct ast_node_rule_until {
    struct ast_node_compound_list *compound_list;
    struct ast_node_do_group *do_group;
};

struct ast_node_rule_case {
    char *word;
    struct ast_node_else_clause *else_clause;
};

struct ast_node_rule_if {
    struct ast_node_compound_list *condition;
    struct ast_node_compound_list *body;
    struct ast_node_else_clause *else_clause; 
};

struct ast_node_else_clause {
    struct ast_node_compound_list *first;
    struct ast_node_compound_list *second;
    struct ast_node_else_clause *else_clause;
};

struct ast_node_do_group {
    struct ast_node_compound_list *compound_list;
};

struct ast_node_case_clause {
    struct ast_node_case_item *list;
};

struct ast_node_case_item {
    char *word;
    char **words;
    struct ast_node_compound_list *compound_list;
    struct ast_node_case_item *next;
};
#endif
