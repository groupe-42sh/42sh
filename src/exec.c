#include "ast.h"
#include "execute.h"
#include <stdlib.h>

int build_str(struct ast_node_command *command)
{
    char *instruction = malloc(50);
    //si c'est simple command
    if (command->simple_command)
    {
        // on va chercher et les "ELEMENTS" et les "PREFIX"
        // dans l'ordre
        // on les concatene
        struct ast_node_prefix *prefix = command->simple_command->prefix_list;

        while (prefix)
        {
            if (prefix->assignement_word)
            {
                //traitement pour les assignements
                strcat(instruction, prefix->assignement_word->var_name);
                strcat(instruction, "=");
                strcat(instruction, itoa(prefix->assignement_word->value))
                //boucler sur les differents assign ???
            }
            else
            {
                //traitement pour les redirections
                struct ast_node_redirection *redirection = prefix->redirection;

                strcat(instruction, itoa(redirection->io_number));

                //!!! normalement en fonction du type redirection...
                //mais pour aller plus vite:
                strcat(instruction, ">");
                
                if (redirection->word_heredoc.heredoc)
                {
                    //on stocke les '"' ou PAS ?
                    //si oui:
                    strcat(instruction, redirection->word_heredoc.heredoc);
                }
                else
                {
                    strcat(instruction, redirection->word_heredoc.word->str);
                }
            }
            prefix = prefix->next;
        }

        struct ast_node_element *element =
            command->simple_command->element_list;

        while (element)
        {
            strcat(instruction, " ");

            if (element->word)
            {
                strcat(instruction, element->word->str);
            }
            else
            {
                struct ast_node_redirection *redirection =
                    prefix->redirection;

                strcat(instruction, itoa(redirection->io_number));
                //!!! normalement en fonction du type redirection...
                //mais pour aller plus vite:
                strcat(instruction, ">");
                
                if (redirection->word_heredoc.heredoc)
                {
                    //on stocke les '"' ou PAS ?
                    //si oui:
                    strcat(instruction, redirection->word_heredoc.heredoc);
                }
                else
                {
                    strcat(instruction, redirection->word_heredoc.word->str);
                }
            }
        }
    }
}

int get_simple_command_pipe(struct ast_node_pipeline *pipeline)
{
    struct ast_node_command *command = pipeline->command;

    //faire pareil pour funcdec, redirection, et tout ?
    
    build_str(command);
}

int get_simple_command_ao(struct ast_node_and_or *and_or)
{
    struct ast_node_pipeline *current = and_or->pipeline;

    while (current)
    {
        get_simple_command_pipe(current);
        current = current->next;
    }
}

int get_simple_command_cl(struct ast_node_compound_list *compound_list)
{
    struct ast_node_and_or *current = compound_list->and_or;

    while (current)
    {
        get_simple_command_ao(current);
        // ou ici ?
        current = current->next;
    }
}

int exec_compound_list(struct ast_node_compound_list *compound_list)
{
    //on la recupere ici, la ligne de commande ?
    get_simple_command_cl(compound_list);
    return 0;
}

int exec_rule_if(struct ast_node_rule_if *_if)
{
    if (exec_compound_list(_if->condition))
    {
        return exec_compound_list(_if->body);
    }
    
    if (_if->else_clause)
    {
        return exec_compound_list(_if->else_clause);
    }
    return 0;
}
