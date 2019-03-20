enum token_type {
  /* identifiers and litterals */
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_STRING,

  /* KEYWORDS */
  TOKEN_IF,
  TOKEN_THEN,
  TOKEN_ELSE,
  TOKEN_FI,
  TOKEN_WHILE,
  TOKEN_FOR,
  TOKEN_UNTIL,
  TOKEN_CASE,
  TOKEN_DO,
  TOKEN_IN,
  TOKEN_ESAC,
  TOKEN_DONE,
  TOKEN_AND,

  /* OPERATORS */
  TOKEN_SEMICOLON,
  TOKEN_PVPV,
  TOKEN_PLUS,
  TOKEN_MIN,
  TOKEN_EQ,
  TOKEN_DIV,
  TOKEN_LEFTP,
  TOKEN_RIGHTP,
  TOKEN_GT,
  TOKEN_LT,
  TOKEN_GTGT,
  TOKEN_LTLT,
  TOKEN_ESPR,
  TOKEN_ESPRESPR,
  TOKEN_GTPIPE,
  TOKEN_PIPE,
  TOKEN_PIPEPIPE,
  TOKEN_GTESPR,
  TOKEN_LTESPR,
  TOKEN_LTGT,
  TOKEN_LACC,
  TOKEN_RACC,
  TOKEN_EXCL,
  TOKEN_LTLTMIN,

  /* SPECIALS */
  TOKEN_NEWLINE,
  TOKEN_EOF
};

struct token_list {
  struct token_type type;
  struct token_list *next;
};

struct lexer {
  struct token_list *token_list;
};

struct token_list get_tokens()
{
  struct token_list *tokens = NULL;
  /* traitement */
  return tokens;
}

/* translateEN("peek") == "coup d oeil" */
/* renvoie le type du 1er token dans la liste */
enum token_type peek(struct lexer *lexer)
{
  if (lexer->token_list == NULL)
    lexer->token_list = get_tokens();
  return lexer->token_list->type;
}
/* pop le 1er token de la liste */
struct token_list *pop(struct lexer *lexer)
{
  if (lexer->token_list == NULL)
    lexer->token_list = get_tokens();
  struct token_list *first_token = lexer->token_list;
  lexer->token_list = lexer->token_list->next;
  return first_token;
}
