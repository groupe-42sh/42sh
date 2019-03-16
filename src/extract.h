#include "parser.h"
#include "ast.h"
#ifndef EXTRACT_H
# define EXTRACT_H

struct capture_s
{
  int begin;
  int end;
};

struct list_capt_s
{
  const char *tag; 
  struct capture_s capt;
  struct list_capt_s *next;
};

int read_Assign(struct parser_s *p);
void list_capt_store(struct list_capt_s *, const char *, struct capture_s *);
struct capture_s *list_capt_lookup(struct list_capt_s *, const char *);
bool parser_readinput (struct parser_s *p);
bool etoile(int(func)(struct parser_s *), struct parser_s *p);
bool plus(int(func)(struct parser_s *), struct parser_s *p);
bool interrogation(int(func(struct parser_s *)), struct parser_s *p);

#endif
