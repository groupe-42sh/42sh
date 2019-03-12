#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "extract.h"
#include "ast.h"


int read_input(struct parser_s *p)
{
    return parser_readinput(p);
}

bool test(struct parser_s *p)
{
    if (p->index <= 7)
    {
        ++p->index;
        if (p->index == 6)
            return false;
    }
    printf("aAa\n");
    return true;
}


int main(int ac, char **av)
{
    ac = ac;
    av = av;
    struct parser_s *p = parser_new_from_string("content");
    
    bool (*pf)(struct parser_s *);
    pf = &test;
    
    //p->index = 6;
    if (plus(pf, p))
        printf("fail\n");

    //etoile(pf, p);
    //interrogation(pf, p);

//    FILE *inifile = fopen(av[1], "rb");
//    fseek(inifile, 0, SEEK_END);
//    long fsize = ftell(inifile);
//    fseek(inifile, 0, SEEK_SET);
//    char *content = malloc(fsize + 1);
//    fread(content, fsize, 1, inifile);
//    fclose(inifile);
//    content[fsize] = 0;
//    struct parser_s *p = parser_new_from_string(content);
//
//    if (read_input(p))
//    {
//        puts("parsing success");
//    }
//    else
//    {
//        printf("parsing failed at %ld\n", p->index);
//        switch (p->error->type)
//        {
//            case ON_CHAR:
//                printf("expected '%c'\n", p->error->u.c);
//                break;
//            case ON_TEXT:
//                printf("expected \"%s\"\n", p->error->u.text);
//                break;
//            case ON_RANGE:
//                printf("expected range \"%d\"\n", p->error->u.range.begin);
//                break;
//            case ON_INSET:
//                printf("expected \"%s\"\n", p->error->u.inset);
//                break;
//            case ON_OUTSET:
//                printf("expected \"%s\"\n", p->error->u.outset);
//                break;
//            default:
//                printf("other error\n");
//                break;
//        }
//    }
//        struct list_capt_s *curr = p->capture;
//        while (curr)
//        {
//            //        puts(curr->tag);
//            curr = curr->next;
//        }
//
//        free(content);
}
