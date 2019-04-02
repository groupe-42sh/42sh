#ifndef AST_H
#define AST_H
#include "parser.h"

enum shell_command_type {
  VOID,
  FOR,
  WHILE,
  CASE,
  IF,
  UNTIL
};  
enum relation {
  TOKEN_VOID,
  TOKEN_DOUBLE_AMPERSAND,
  TOKEN_DOUBLE_PIPE
};

enum command_type {
    SIMPLE,
    SHELL,
    FUNCDEC
};

enum redirection {
  NOREDIR,
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
  struct ast_node_simple_command *simple_command;/*1 seul*/
  struct ast_node_shell_command *shell_command;/*1 seul*/
  struct ast_node_funcdec *funcdec;/*1 seul*/
  struct ast_node_redirection *redirection_list ;
  struct ast_node_command *next;
  bool pipe; 
};

struct ast_node_simple_command {
  struct ast_node_element *element_list;
  struct ast_node_prefix *prefix_list;
};

struct ast_node_element { 
  struct ast_node_word *word;
  struct ast_node_redirection *redirection;
  struct ast_node_element *next;
};

struct ast_node_prefix { 
  struct ast_node_assignement_word *assignement_word;
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
    struct ast_node_word *word;
    struct ast_node_shell_command *shell_command;
};

union word_heredoc {
  struct ast_node_word *word;
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
    struct ast_node_word *word;
    struct ast_node_word *word_list;
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
    struct ast_node_word *word;
    struct ast_node_case_clause *case_clause;
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
    struct ast_node_word *word_list;
    struct ast_node_compound_list *compound_list;
    struct ast_node_case_item *next;
};
struct ast_node_word {
  char *str;
  struct ast_node_word *next;
};
struct ast_node_assignement_word {
  char *var_name;
  char *value;
  struct ast_node_assignement_word *next;
};
void assignement_word_traversal(struct ast_node_assignement_word *assignement_word);
void word_traversal(struct ast_node_word *word);
void case_item_traversal(struct ast_node_case_item *case_item);
void case_clause_traversal(struct ast_node_case_clause *case_clause);
void case_clause_traversal(struct ast_node_case_clause *case_clause);
void do_group(struct ast_node_do_group *do_group);
void else_clause_traversal(struct ast_node_else_clause *else_clause);
void rule_if_traversal(struct ast_node_rule_if *_if);
void rule_case_traversal(struct ast_node_rule_case *_case);
void rule_until_traversal(struct ast_node_rule_until *_until);
void rule_while_traversal(struct ast_node_rule_while *_while);
void rule_for_traversal(struct ast_node_rule_for *_for);
void element_traversal(struct ast_node_element* element);
void prefix_traversal(struct ast_node_prefix* prefix);
void compound_list_traversal(struct ast_node_compound_list *compound_list);
void redirection_traversal(struct ast_node_redirection *redirection);
void shell_command_traversal(struct ast_node_shell_command *shell_command);
void funcdec_traversal(struct ast_node_funcdec *funcdec);
void simple_command_traversal(struct ast_node_simple_command *simple_command);
void command_traversal(struct ast_node_command *command);
void pipeline_traversal(struct ast_node_pipeline *pipeline);
void and_or_traversal(struct ast_node_and_or *and_or);
void list_traversal(struct ast_node_list *list);
void full_traversal(struct parser_s *p);

#endif
