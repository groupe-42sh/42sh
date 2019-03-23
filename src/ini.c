#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "extract.h"
#include "ast.h"
#include "print_ast.h"
#include "opt-parser.h"

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int test(struct parser_s *p)
{
    p = p;
    return -1;
}

int main(int ac, char **av)
{
    int argi = 1;
    struct options options = { 0 };

    if (opt_parse(ac, av, &argi, &options))
        exit(EXIT_FAILURE);

    FILE *inifile = fopen(av[argi], "rb");
    if (!inifile)
        exit(EXIT_FAILURE);

    FILE *shrc1 = fopen("/etc/42shrc", "r");
    char *shrcc = concat(getenv("HOME"), "/.42shrc");
    FILE *shrc2 = fopen(shrcc, "r");
    FILE *shrc=NULL;

    if (shrc1==NULL && shrc2==NULL)
        printf("No 42shrc found\n");
    else
    {
        if (shrc1==NULL)
            shrc = shrc2;
        else
            shrc = shrc1;
    }
    fclose(shrc);

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

        if (options.print)
            full_traversal_print(p);
    }
    else
    {
        printf("parsing failed at %ld\n", p->index);
    }
    free(content);
    fclose(shrc1);
    fclose(shrc2);
    return 0;
}
