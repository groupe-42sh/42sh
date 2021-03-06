#ifndef PARSER_H
# define PARSER_H
#include <stddef.h>

struct parser_s {
    const char *input;
    size_t index;
};

struct parser_s *parser_new_from_string(const char *text);
void parser_clean(struct parser_s *p);
int parser_eof(struct parser_s *p);
char parser_getchar(struct parser_s *p);
int parser_peekchar(struct parser_s *p, char c);
int parser_readchar(struct parser_s *p, char c);
int parser_readtext(struct parser_s *p, char *text);
int parser_readinset(struct parser_s *p, char *set);
int parser_readrange(struct parser_s *p, char begin, char end);
int parser_readoutset(struct parser_s *p, char *set);
int parser_readeol(struct parser_s *p);
int parser_readidentifier(struct parser_s *p);
int parser_readinteger(struct parser_s *p);

#endif
