#ifndef AST_H
#define AST_H
#include "parser.h"

enum sep {
    RIEN,
    POINT_VIRGULE,
    ESPERLUETTE
};

enum relation {
    DOUBLE_ESPERLUETTE,
    DOUBLE_PIPE
};

enum command_type {
    SIMPLE,
    SHELL,
    FUNDEC
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
    enum sep separator;
    struct ast_node_pipeline *pipeline;
    struct ast_node_and_or *next;
};

struct ast_node_pipeline {
    enum relation rel;
    union ast_node_command *command;
    bool inverse; /*[!]*/
};

union ast_node_command {
    struct ast_node_simple_command *simple_command;/*1 seul*/
    union ast_node_shell_command *shell_command;/*1 seul*/
    struct ast_node_fundec *fundec;/*1 seul*/
};

struct ast_node_command_container {
    enum command_type type;
    union ast_node_command *command;
    struct ast_node_redirection *redirection;
};

struct ast_node_simple_command {
    struct ast_node_element *element; 
    struct ast_node_prefix *prefix;
};

struct ast_node_element { 
    char *word;
    struct ast_node_redirection *redirection;
};

struct ast_node_prefix { 
    char *assignement_word;
    struct ast_node_redirection *redirection;
};

union ast_node_shell_command {
    struct ast_node_rule_for *_for;
    struct ast_node_rule_while *_while;
    struct ast_node_rule_case *_case;
    struct ast_node_rule_if *_if;
    struct ast_node_rule_until *_until;
    struct ast_node_compound_list *compound_list;
};

struct ast_node_fundec {
    char *word;
    union ast_node_shell_command *command;
};

union word_heredoc {
    char *word;
    char *heredoc;
};

struct ast_node_redirection {
    size_t io_number;
    enum redirection redirection_type;
    union word_heredoc word_heredoc;
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
