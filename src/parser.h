#ifndef PARSER_H
# define PARSER_H
#include <stddef.h>

enum error_type_e
{
    ON_CHAR,
    ON_TEXT,
    ON_RANGE,
    ON_INSET,
    ON_OUTSET
};

struct error_s
{
    enum error_type_e type;
    union
    {
        char c;
        char *text;
        char *inset;
        char *outset;
        struct
        {
            char begin;
            char end;
        } range;
    } u;
};


struct parser_s {
    const char *input;
    size_t index;
    struct list_capt_s *capture;
    struct ast_node_input *ast; 
    struct error_s *error;
};

typedef enum bool {
    false,
    true
}bool;

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
bool parser_readeol(struct parser_s *p);
bool parser_readidentifier(struct parser_s *p);
bool parser_readinteger(struct parser_s *p);
int parser_peekinset(struct parser_s *p, char *set);
int read_closebracket(struct parser_s *p);
#endif
