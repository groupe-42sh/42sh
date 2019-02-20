#include <stdio.h>
#include "parser.h"

int main(void)
{
    struct parser_s
        *p = parser_new_from_string("a l horizon tjrs 2 ou 3 keufs en civil");

    printf("%ld\n", p->index);
    puts(p->input);
    //int parser_eof(struct parser_s *p);
    putchar(parser_getchar(p));
    printf("index : %ld\n", p->index);
    
    int r = parser_peekchar(p, ' ');
    printf("%d\n", r);
    printf("index : %ld\n", p->index);
    
    r = parser_peekchar(p, 'm');
    printf("%d\n", r);
    printf("index : %ld\n", p->index);
     
    //int parser_readchar(struct parser_s *p, char c);
    r = parser_readchar(p, 'm');
    printf("%d\n", r);
    printf("index : %ld\n", p->index);

    r = parser_readchar(p, ' ');
    printf("%d\n", r);
    printf("index : %ld\n", p->index);

    char *text = "l horizon tjrs"; 
    r = parser_readtext(p, text);
    printf("%d\n", r);
    printf("index : %ld\n", p->index);

    struct parser_s
        *p2 = parser_new_from_string(" \t\n\r");
    p->index++;
    r = parser_readinset(p2, "\\t\\r\\n");
    printf("readinset: %d\n", r);

//    printf("index : %ld\n", p->index);

    p2->input = "\r\r";
    p2->index = 0;
    r = parser_readeol(p2);
    printf("%d\n", r);

    p2->input = "cdfs";
    p2->index = 0;
    r = parser_readidentifier(p2);
    printf("i : %d\n", r);

    p2->input = "12434334432";
    p2->index = 0;
    r = parser_readinteger(p2);
    printf("int i : %d\n", r);


    return 0;
}
