#ifndef OPT_PARSER_H
#define OPT_PARSER_H

struct options {
    unsigned int c:1;
    unsigned int min_zero:1;
    unsigned int plus_zero:1;
    unsigned int norc:1;
    unsigned int print:1;
    unsigned int version:1;
};

int opt_parse(int argv, char *argc[], int *argi, struct options *options);
#endif
