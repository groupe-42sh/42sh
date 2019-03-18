#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "extract.h"
#include "ast.h"
#include "print_ast.h"

int test(struct parser_s *p)
{
    p = p;
    return -1;
}

int main(int ac, char **av)
{
  ac = ac;
  FILE *inifile = fopen(av[1], "rb");
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
      full_traversal_print(p);
    }
  else
    {
      printf("parsing failed at %ld\n", p->index);
      /*switch (p->error->type)
	{
	case ON_CHAR:
	  printf("expected '%c'\n", p->error->u.c);
	  break;
	case ON_TEXT:
	  printf("expected \"%s\"\n", p->error->u.text);
	  break;
	case ON_RANGE:
	  printf("expected range \"%d\"\n", p->error->u.range.begin);
	  break;
	case ON_INSET:
	  printf("expected \"%s\"\n", p->error->u.inset);
	  break;
	case ON_OUTSET:
	  printf("expected \"%s\"\n", p->error->u.outset);
	  break;
	default:
	  printf("other error\n");
	  break;
	  }*/
    }
  free(content);
}
