#ifndef EXE_H
#define EXE_H
int build_str(struct ast_node_command *command);
int get_simple_command_pipe(struct ast_node_pipeline *pipeline);
int get_simple_command_ao(struct ast_node_and_or *and_or);
int get_simple_command_cl(struct ast_node_compound_list *compound_list);
int exec_compound_list(struct ast_node_compound_list *compound_list);
int exec_rule_if(struct ast_node_rule_if *_if);
#endif
