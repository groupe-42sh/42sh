#include <stdio.h>
#include "parser.h"
#include <assert.h>
#include <string.h>

int main(void)
{
  char ch = 0;
  size_t i = 0;
  struct parser_s
    *p = parser_new_from_string("a l horizon tjrs 2 ou 3 keufs en civil");

  assert(p->index == 0);
  assert(strcmp(p->input,"a l horizon tjrs 2 ou 3 keufs en civil") == 0);

  i = p->index;
  ch = parser_getchar(p);

  assert(ch == p->input[0]);
  assert(p->index == i + 1);


  i = parser_peekchar(p, ' ');
  assert(i == 1);
  assert(p->index == 1);

  i = parser_peekchar(p, 'm');
  assert(i == 0);
  assert(p->index == 1);


  i = parser_readchar(p, 'm');

  assert(i == 0);
  assert(p->index == 1);

  i = parser_readchar(p , ' ');

  assert(i == 1);
  assert(p->index == 2);

  char *text = "l horizon tjrs";
  i = parser_readtext(p, text);

  assert(i == 1);
  assert(p->index == 16);

  struct parser_s
    *p2 = parser_new_from_string(" \t\n\r");
  p2->index++;
  i = parser_readinset(p2, "\\t\\r\\n");
  assert(i == 0);
  i = parser_readinset(p2, "\t\r\n");

  assert(i == 1);
  assert(p2->index == 2);

  p2->input = "\r\r";
  p2->index = 0;
  i = parser_readeol(p2);

  assert(i == 0);

  p2->input = "\r\n";
  p2->index = 0;
  i = parser_readeol(p2);
  assert(i == 1);

  p2->input = "\n";
  p2->index = 0;
  i = parser_readeol(p2);

  assert(i == 1);
  p2->input = "cdfs";
  p2->index = 0;
  i = parser_readidentifier(p2);

  assert(i == 1);

  p2->input = "_AozleERmezr'";
  p2->index = 0;
  i = parser_readidentifier(p2);

  assert(i == 0);

  p2->input = "_AozleERmez__r___";
  p2->index = 0;
  i = parser_readidentifier(p2);

  assert(i == 1);

  p2->input = "12434334432";
  p2->index = 0;
  i = parser_readinteger(p2);

  assert(i == 1);

  p2->input = "_12434334432";
  p2->index = 0;
  i = parser_readinteger(p2);

  assert(i == 0);

  return 0;
}
