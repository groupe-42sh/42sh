#ifndef EXTRACT_H
#define EXTRACT_H
#include "parser.h"
#include "ast.h"
struct capture_s
{
  int begin;
  int end;
};

struct list_capt_s
{
  const char *tag; 
  struct capture_s capt;
  struct list_capt_s *next;
};

int read_Assign(struct parser_s *p);
void list_capt_store(struct list_capt_s *, const char *, struct capture_s *);
struct capture_s *list_capt_lookup(struct list_capt_s *, const char *);
bool parser_readinput (struct parser_s *p);
struct ast_node_rule_if *parser_readruleif(struct parser_s *p);
struct ast_node_rule_for *parser_readrulefor(struct parser_s *p);
struct ast_node_assignement_word *parser_readassignementword(struct parser_s *p);
struct ast_node_word *parser_readword(struct parser_s *p);
struct ast_node_compound_list *parser_readcompoundlist(struct parser_s *p);
struct ast_node_case_item *parser_readcaseitem(struct parser_s *p);
struct ast_node_rule_case *parser_readrulecase(struct parser_s *p);
struct ast_node_else_clause *parser_readelseclause(struct parser_s *p);
struct ast_node_do_group *parser_readdogroup(struct parser_s *p);
struct ast_node_rule_until *parser_readruleuntil(struct parser_s *p);
struct ast_node_rule_while *parser_readrulewhile(struct parser_s *p);
struct ast_node_shell_command *parser_readshellcommand(struct parser_s *p);
struct ast_node_funcdec *parser_readfuncdec (struct parser_s *p);
struct ast_node_redirection *parser_readredirection(struct parser_s *p);
struct ast_node_prefix *parser_readprefix(struct parser_s *p);
struct ast_node_element *parser_readelement(struct parser_s *p);
struct ast_node_simple_command *parser_readsimplecommand(struct parser_s *p);
struct ast_node_command *parser_readcommand (struct parser_s *p);
struct ast_node_pipeline *parser_readpipeline (struct parser_s *p);
struct ast_node_and_or *parser_readandor (struct parser_s *p);
struct ast_node_list *parser_readlist (struct parser_s *p);
#endif
