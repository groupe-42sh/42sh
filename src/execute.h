#ifndef EXECUTE_H
#define EXECUTE_H
int exec_input(struct ast_node_input *input);
int exec_list(struct ast_node_list *list);
int exec_and_or(struct ast_node_and_or *and_or);
int exec_rule_if(struct ast_node_rule_if *rule_if)
int exec_rule_for(struct ast_node_rule_for *rule_for);
int exec_rule_while(struct ast_node_rule_while *rule_while);
int exec_rule_until(struct ast_node_rule_until *rule_until);
int exec_rule_case(struct ast_node_rule_case *rule_case);
int exec_compound_list(struct ast_node_compound_list *compound_list);
int exec_do_group(struct ast_node_do_group *do_group);
int exec_case_clause(struct ast_node_case_clause *case_clause);
int exec_case_item(struct ast_node_case_item *case_item);
int exec_else_clause(struct ast_node_else_clause *else_clause);
#endif
