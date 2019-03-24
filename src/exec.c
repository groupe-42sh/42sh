#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ast.h"
#include "exe.h"
#include "hashmap.h"
#define HASHTABLE_SIZE 15

struct ht *table = NULL;

int  execute(char **argv)
{
    pid_t  pid;
    int    status;

    /* pour eviter les erreurs quand argv[1] est vide
     * voir si autre solution
     */
    if ((strcmp(argv[0], "ls") == 0) && (strcmp(argv[1], "") == 0))
    {
        strcpy(argv[1], ".");
    }

    /*
     * en attendant de savoir d 'ou provient l espace additionnel...
     */
    if ((strcmp(argv[1], "") == 0))
    {
        argv[1] = NULL;
    }

    if ((pid = fork()) < 0)
    {
        perror("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        if (execvp(*argv, argv) < 0)
        {
            perror("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else
    {
        while (wait(&status) != pid)
        {
        }
    }
    return 1;
}

void  parse(char *line, char **argv)
{
    while (*line != '\0')
    {
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';
        *argv++ = line;
        while (*line != '\0' && *line != ' ' &&
                *line != '\t' && *line != '\n')
            line++;
    }
    *argv = '\0';
}


void analyze_prefix(struct ast_node_command *command, char **instruction)
{
    struct ast_node_prefix *prefix = command->simple_command->prefix_list;

    char *str = malloc(200);

    while (prefix)
    {
        if (prefix->assignement_word)
        {
            //traitement pour les assignements
            strcat(*instruction, prefix->assignement_word->var_name);
            strcat(*instruction, "=");
            strcat(*instruction, prefix->assignement_word->value);
            //boucler sur les differents assign ???
        }
        else
        {
            //traitement pour les redirections
            struct ast_node_redirection *redirection = prefix->redirection;

            snprintf(str, 10, "%d", redirection->io_number);
            strcat(*instruction, str);

            //!!! normalement en fonction du type redirection...
            //mais pour aller plus vite:
            strcat(*instruction, ">");

            if (redirection->word_heredoc.heredoc)
            {
                //on stocke les '"' ou PAS ?
                //si oui:
                strcat(*instruction, redirection->word_heredoc.heredoc);
            }
            else
            {
                strcat(*instruction, redirection->word_heredoc.word->str);
            }
        }
        prefix = prefix->next;
    }
    //free(str);
}

void analyze_element(struct ast_node_command *command, char **instruction)
{
    struct ast_node_element *element =
        command->simple_command->element_list;

    char *str = malloc(200);
    while (element)
    {
        if (element->word)
        {
            strcat(*instruction, element->word->str);
            strcat(*instruction, " ");
        }
        else
        {
            struct ast_node_redirection *redirection =
                element->redirection;

            snprintf(str, 10, "%d", redirection->io_number);
            strcat(*instruction, str);
            //!!! normalement en fonction du type redirection...
            //mais pour aller plus vite:
            strcat(*instruction, ">");

            if (redirection->word_heredoc.heredoc)
            {
                //on stocke les '"' ou PAS ?
                //si oui:
                strcat(*instruction, redirection->word_heredoc.heredoc);
            }
            else
            {
                strcat(*instruction, redirection->word_heredoc.word->str);
            }
        }

        element = element->next;
    }
    //free(str);
}


int build_str(struct ast_node_command *command)
{
    char *instruction = malloc(200);
    memset(instruction, '\0', 200);

    //si c'est simple command
    if (command->simple_command)
    {
        // on va chercher et les "ELEMENTS" et les "PREFIX"
        // dans l'ordre
        // on les concatene

        analyze_prefix(command, &instruction);
        analyze_element(command, &instruction);
    }
    //valeur arbitraire
    char **instr = malloc(50 * sizeof(char *));
    if (!instr)
        return 0;
    parse(instruction, instr);
    int r = execute(instr);
    free(instruction);
    //free(instr);
    return r;
}
int get_simple_command_pipe(struct ast_node_pipeline *pipeline)
{
    struct ast_node_command *command = pipeline->command;

    //faire pareil pour funcdec, redirection, et tout ?

    return build_str(command);
}

int get_simple_command_ao(struct ast_node_and_or *and_or)
{
    struct ast_node_pipeline *current = and_or->pipeline;

    int res = 1;
    while (current)
    {
        res = get_simple_command_pipe(current);
        current = current->next;
    }
    return res;
}

int get_simple_command_cl(struct ast_node_compound_list *compound_list)
{
    struct ast_node_and_or *current = compound_list->and_or;

    int res = 1;
    while (current)
    {
        res = get_simple_command_ao(current);
        // ou ici ?
        current = current->next;
    }
    return res;
}

int exec_compound_list(struct ast_node_compound_list *compound_list)
{
    //on la recupere ici, la ligne de commande ?
    int res = 1;
    res = get_simple_command_cl(compound_list);
    return res;
}

int exec_else_clause(struct ast_node_else_clause *else_clause)
{
    else_clause = else_clause;
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
        return exec_else_clause(_if->else_clause);
    }
    return 0;

}

int exec_do_group(struct ast_node_do_group *do_group)
{
    return exec_compound_list(do_group->compound_list);
}

int exec_rule_while(struct ast_node_rule_while *_while)
{
    if (exec_compound_list(_while->compound_list))
    {
        return exec_do_group(_while->do_group);
    }
    return 0;
}



int exec_assignement_word(struct ast_node_assignement_word *assignement_word)
{
    if (!table)
    {
        table = ht_malloc(HASHTABLE_SIZE);
    }
    
    char *var_name = assignement_word->var_name;
    char *value = assignement_word->value;

    ht_find_or_put(var_name, value, table, fvn_hash);
    ht_print(stdout, table); 

    
    //char *instruction = malloc(200);
    //memset(instruction, '\0', 200);
    //
    //strcpy(instruction, "0 -eq 1");
    //char **instr = malloc(50 * sizeof(char *));
    //if (!instr)
    //    return 0;
    //parse(instruction, instr);
    //int r = execute(instr);
    //free(instruction);
    //r =r ;
    return 0;
}
