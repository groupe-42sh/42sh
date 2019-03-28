#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "extract.h"
#include "ast.h"
#include "print_ast.h"
#include "opt-parser.h"
#include "exec.h"

int main(int ac, char **av)
{
    int argi = 1;
    struct options options = { 0 };

    if (opt_parse(ac, av, &argi, &options))
        exit(EXIT_FAILURE);

    FILE *inifile = fopen(av[argi], "rb");
    if (!inifile)
        exit(EXIT_FAILURE);

    fseek(inifile, 0, SEEK_END);
    long fsize = ftell(inifile);
    fseek(inifile, 0, SEEK_SET);
    char *content = malloc(fsize + 1);
    fread(content, fsize, 1, inifile);
    fclose(inifile);
    content[fsize] = 0;
    struct parser_s *p = parser_new_from_string(content);

    if (parser_readinput(p))
    {
        puts("parsing success");
            
//        struct ast_node_rule_if *_if =
//            p->ast->list->and_ors->pipeline->command->shell_command->child._if;
//
//        exec_rule_if(_if);
        /*struct ast_node_rule_while *_while =
            p->ast->list->and_ors->pipeline->command->shell_command->child._while;
	    exec_rule_while(_while);*/
     
//        struct ast_node_assignement_word *aw =
//            malloc(sizeof(struct ast_node_assignement_word));
//        aw->var_name = "my_var";
//        aw->value = "avalue";
//        aw->next = NULL;
//        //build_str(p->ast->list->and_ors->pipeline->command);
//        exec_assignement_word(aw);
        if (options.print)
            full_traversal_print(p);
    }
    else
    {
        printf("parsing failed at %ld\n", p->index);
    }
    free(content);
}
