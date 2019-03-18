#ifndef PRINT_AST_H
#define PRINT_AST_H
void full_traversal_print(struct parser_s *p);
void assignement_word_traversal_print
    (struct ast_node_assignement_word *assignement_word);
void assignement_word_traversal_print
    (struct ast_node_assignement_word *assignement_word);
void word_traversal_print(struct ast_node_word *word);
void case_item_traversal_print(struct ast_node_case_item *case_item);
void case_clause_traversal_print(struct ast_node_case_clause *case_clause);
void do_group_traversal_print(struct ast_node_do_group *do_group);
void else_clause_traversal_print(struct ast_node_else_clause *else_clause);
void rule_if_traversal_print(struct ast_node_rule_if *_if);
void rule_case_traversal_print(struct ast_node_rule_case *_case);
void rule_until_traversal_print(struct ast_node_rule_until *_until);
void rule_while_traversal_print(struct ast_node_rule_while *_while);
void rule_for_traversal_print(struct ast_node_rule_for *_for);
void element_traversal_print(struct ast_node_element* element);
void prefix_traversal_print(struct ast_node_prefix* prefix);
void compound_list_traversal_print(struct ast_node_compound_list
*compound_list);
void redirection_traversal_print(struct ast_node_redirection *redirection);
void shell_command_traversal_print(struct ast_node_shell_command
*shell_command);
void funcdec_traversal_print(struct ast_node_funcdec *funcdec);
void simple_command_traversal_print(struct ast_node_simple_command
*simple_command);
void command_traversal_print(struct ast_node_command *command);
void pipeline_traversal_print(struct ast_node_pipeline *pipeline);
void and_or_traversal_print(struct ast_node_and_or *and_or);
void list_traversal_print(struct ast_node_list *list);
#endif
