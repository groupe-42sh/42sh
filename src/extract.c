#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include "extract.h"
#include "parser.h"
#include "ast.h"

#include <stdio.h>
#include <time.h>
void eat_newlines(struct parser_s *p)
{
  while(parser_readchar(p,'\n'))
    {
    }
}
int read_space(struct parser_s *p)
{
    return parser_readchar(p, ' ');
}
int read_tabs(struct parser_s *p)
{
    return parser_readchar(p, '\t');
}

int eat_spaces(struct parser_s *p)
{
    while(read_tabs(p) || read_space(p)) //consomme les espaces et tabs
    {}
    return 1;
}

int read_openbracket(struct parser_s *p)
{
    return parser_readchar(p, '[');
}

void list_capt_store(struct list_capt_s *capture, const char *tag,
        struct capture_s *capt)
{
    if (!capture)
        return;

    if (!capt)
        return;

    if (!capture->tag)
    {
        capture->tag = tag;
        capture->capt = *capt;
        capture->next = NULL;
        return;
    }

    struct list_capt_s *prev = capture;
    struct list_capt_s *current = NULL;
    current = capture;

    while (current)
    {
        prev = current;
        current = current->next;
    }

    struct list_capt_s *new = malloc(sizeof(struct list_capt_s));
    if (!new)
        return;
    new->tag = tag;
    new->capt = *capt;
    new->next = NULL;
    prev->next = new;
}

struct capture_s *list_capt_lookup(struct list_capt_s *capture, const char *tag)
{
    if (!capture)
        return NULL;
     struct list_capt_s *current = capture;

    while (current && (strcmp(current->tag, tag) != 0))
        current = current->next;

    return &current->capt;
}

static inline bool parser_begin_capture(struct parser_s *p, const char *tag)
{
    struct capture_s *capt = malloc(sizeof(struct capture_s));

    capt->begin = p->index;
    capt->end = 0;
    list_capt_store(p->capture, tag, capt);
    return true;
}

static inline bool parser_end_capture(struct parser_s *p, const char *tag)
{
    struct capture_s *pcapt = list_capt_lookup(p->capture, tag);
    if (!pcapt)
        return false;
    pcapt->end = p->index;
    return true;
}

static inline char *parser_get_capture(struct parser_s *p, const char *tag)
{
    struct capture_s *pcapt = list_capt_lookup(p->capture, tag);
    if (!pcapt)
        return false;
    return strndup(&p->input[pcapt->begin], pcapt->end - pcapt->begin);
}

void reinit_capture(struct parser_s *p)
{
    struct list_capt_s *curr = p->capture;
    struct list_capt_s *delete = curr;
    while (curr)
    {
        delete = curr;
        curr = curr->next;
        free(delete);
    }   
}
char *get_word(struct parser_s *p)
{
  char *word = NULL; 
  if(parser_begin_capture(p,"word") && parser_readidentifier(p) && parser_end_capture(p,"word"))
    word = parser_get_capture(p, "word");
  reinit_capture(p);
  return word;
}
bool parser_read_doubleesperluette(struct parser_s *p)
{
    int tmp = p->index;

    if (parser_readchar(p, '&') && parser_readchar(p, '&'))
        return true;
    p->index = tmp;
    return false;
}

bool parser_read_doublepipe(struct parser_s *p)
{
  int tmp = p->index;
  
  if (parser_readchar(p, '|') && parser_readchar(p, '|'))
    return true;
  p->index = tmp;
  return false;
}
struct ast_node_word *get_word_list(struct parser_s *p)
{
    struct ast_node_word *root = parser_readword(p);
    if (!root)
        return NULL;

    struct ast_node_word *word = NULL;
    struct ast_node_word *last = NULL;
    word = root;

    while ((last = parser_readword(p)))
    {
        word->next = last;
        word = last;
    }

    return root;
}
struct ast_node_rule_if *parser_readruleif(struct parser_s *p)
{
    struct ast_node_rule_if *rule_if =
        malloc(sizeof(struct ast_node_rule_if));

    rule_if->condition = NULL;
    rule_if->body = NULL;
    rule_if->else_clause = NULL;

    if (!(rule_if->condition = parser_readcompoundlist(p))
        || !(parser_readtext(p, "then"))
        || !(rule_if->body = parser_readcompoundlist(p)))
        return NULL;
    rule_if->else_clause = parser_readelseclause(p);
    if (!(parser_readtext(p, "fi")))
        return NULL;

    return rule_if;
}

struct ast_node_rule_for *parser_readrulefor(struct parser_s *p)
{
    struct ast_node_rule_for *rule_for =
        malloc(sizeof(struct ast_node_rule_for));
    
    rule_for->word_list = NULL;
    rule_for->do_group = NULL;

    if (!(rule_for->word = parser_readword(p)))
        return NULL;

    if (!parser_readchar(p, ';') /* && eat_newlines(p) */)
    {
        eat_newlines(p);
        
        if (parser_readtext(p, "in"))
        {
            rule_for->word_list = get_word_list(p);
    
            if (!parser_readchar(p, ';') && !parser_readchar(p, '\n'))
                return NULL;
        }
    }   
    
    eat_newlines(p);

    if (!(rule_for->do_group = parser_readdogroup(p)))
        return NULL;

    return rule_for;
}
struct ast_node_assignement_word *get_assignement_word(struct parser_s *p)
{
     struct ast_node_assignement_word *assignement_word
        = malloc(sizeof(struct ast_node_assignement_word));
   
     if (!(assignement_word->var_name = get_word(p))
        || (!parser_readchar(p, '='))
        || (!(assignement_word->value = atoi(get_word(p)))))
        return NULL;
    return assignement_word;
}
struct ast_node_assignement_word *parser_readassignementword(struct parser_s *p)
{
    struct ast_node_assignement_word *assignement_word = NULL;
    if (!(assignement_word = get_assignement_word(p)))
        return NULL;

//    int tmp = p->index;
//
//    if (parser_begin_capture(p, "assign") && parser_readidentifier(p)
//        && parser_end_capture(p, "assign"))
//        && parser_readchar(p, '=')
//        && parser_begin_capture(p, "value") && parser_readinteger(p)
//        && parser_end_capture(p, "value")
//    {
//        assignement_word->var_name = parser_get_capture("assign");
//        assignement_word->value = atoi(parser_get_capture("value"));
//        assignement_word->next = NULL;
//        
//        reinit_capture(p);
//        return assignement_word;
//    }
//    
//    p->index = tmp;

    struct ast_node_assignement_word *current = assignement_word; 
    struct ast_node_assignement_word *last = NULL;
    
    eat_newlines(p);

    while ((last = get_assignement_word(p)))
    {
        current->next = last;
        current = last;
        
        eat_newlines(p);
    }

    return assignement_word;
}
struct ast_node_word *parser_readword(struct parser_s *p)
{
  struct ast_node_word *word = malloc(sizeof(struct ast_node_word));
  word->next = NULL;
  if(!(word->str = get_word(p)))
    return NULL;
  return word;
}
struct ast_node_compound_list *parser_readcompoundlist(struct parser_s *p)
{
  struct ast_node_compound_list *compound_list = malloc(sizeof(struct ast_node_compound_list));
  compound_list->and_or = NULL;
  eat_newlines(p);
  struct ast_node_and_or *and_or = NULL;
  if(!(and_or = parser_readandor(p)))
    return NULL;
  compound_list->and_or = and_or;
  struct ast_node_and_or *last;
  last = NULL;
  bool c = false;
  while(parser_readchar(p,';') || (c=parser_readchar(p,'&')) || parser_readchar(p,'\n'))
    {
      eat_newlines(p);
      and_or->linked = c;
      if((last = parser_readandor(p)))
	{
	  and_or->next = last;
	  and_or = last;
	}
      c = false;
    }
  return compound_list; 
}
struct ast_node_case_item *parser_readcaseitem(struct parser_s *p)
{
  bool c = false;
  struct ast_node_case_item *case_item = malloc(sizeof(struct ast_node_case_item));
  case_item->word_list = NULL;
  case_item->compound_list = NULL;
  case_item->next = NULL;
  c = parser_readchar(p,'(');
  struct ast_node_word *word = NULL;
  if(!(word = parser_readword(p)))
    return NULL;
  case_item->word_list = word;
  struct ast_node_word *last_word = NULL;
  while(parser_readchar(p,'|') && (last_word = parser_readword(p)))
    {
      word->next = last_word;
      word = last_word;
    }
  if(c && !(parser_readchar(p,')')))
    return NULL;
  struct ast_node_compound_list *compound_list = NULL;
  if((compound_list = parser_readcompoundlist(p)))
    case_item->compound_list = compound_list;
  return case_item;
}
struct ast_node_case_clause *parser_readcaseclause(struct parser_s *p)
{
  struct ast_node_case_clause *case_clause = malloc(sizeof(struct ast_node_case_clause));
  case_clause->list = NULL;
  struct ast_node_case_item *case_item = parser_readcaseitem(p);
  if(!case_item)
    return NULL;
  case_clause->list = case_item;
  struct ast_node_case_item *last_case_item = NULL;
  while(parser_readtext(p,";;"))
    {
      eat_newlines(p);
      if((last_case_item = parser_readcaseitem(p)))
	{
	  case_item->next = last_case_item;
	  case_item = last_case_item;
	}
    }
  eat_newlines(p);
  return case_clause;
}
struct ast_node_rule_case *parser_readrulecase(struct parser_s *p)
{
  struct ast_node_rule_case *rule_case = malloc(sizeof(struct ast_node_rule_case));
  rule_case->case_clause = NULL;
  if(!(rule_case->word = parser_readword(p)))
    return NULL;
  eat_newlines(p);
  if(!parser_readtext(p,"in"))
    return NULL;
  eat_newlines(p);
  rule_case->case_clause = parser_readcaseclause(p);
  if(!parser_readtext(p,"esac"))
    return NULL;
  return rule_case;
  
}
struct ast_node_else_clause *parser_readelseclause(struct parser_s *p)
{
  struct ast_node_else_clause *else_clause = malloc(sizeof(struct ast_node_else_clause));
  else_clause->second = NULL; 
  else_clause->else_clause = NULL;
  if(parser_readtext(p,"else") && (else_clause->first = parser_readcompoundlist(p)))
    return else_clause;
  if(parser_readtext(p,"elif") && (else_clause->first = parser_readcompoundlist(p)) &&
     parser_readtext(p,"then") && (else_clause->second = parser_readcompoundlist(p)))
    {
      else_clause->else_clause = parser_readelseclause(p);
      return else_clause;
    }
  return NULL;
}
struct ast_node_do_group *parser_readdogroup(struct parser_s *p)
{
  struct ast_node_do_group* do_group = malloc(sizeof(struct ast_node_do_group));
  struct ast_node_compound_list *compound_list = NULL;
  if(!parser_readtext(p,"do") || !(compound_list = parser_readcompoundlist(p)) || !parser_readtext(p,"done"))
    return NULL;
  do_group->compound_list = compound_list;
  return do_group;
}
struct ast_node_rule_until *parser_readruleuntil(struct parser_s *p)
{
  struct ast_node_rule_until *rule_until = malloc(sizeof(struct ast_node_rule_until));
  rule_until->compound_list = NULL;
  rule_until->do_group = NULL;
  struct ast_node_compound_list *compound_list = NULL;
  struct ast_node_do_group *do_group = NULL;
  if(!(compound_list = parser_readcompoundlist(p))||!(do_group = parser_readdogroup(p)))
    return NULL;
  rule_until->compound_list = compound_list;
  rule_until->do_group = do_group;
  return rule_until;
}
struct ast_node_rule_while *parser_readrulewhile(struct parser_s *p)
{
  struct ast_node_rule_while *rule_while = malloc(sizeof(struct ast_node_rule_while));
  rule_while->compound_list = NULL;
  rule_while->do_group = NULL;
  struct ast_node_compound_list *compound_list = NULL;
  struct ast_node_do_group *do_group = NULL;
  if(!(compound_list = parser_readcompoundlist(p))||!(do_group = parser_readdogroup(p)))
    return NULL;
  rule_while->compound_list = compound_list;
  rule_while->do_group = do_group;
  return rule_while;
}

int get_shell_command_type(char *word)
{
  int i = 0;
  char *all_of_types[] = {"for","while","case","if","until"};
  while(i<5)
    {
      if(strcmp(word,all_of_types[i]) == 0)
	return (i+1);
      i++;
    }
  return 0;
}
struct ast_node_shell_command *parser_readshellcommand(struct parser_s *p)
{
  bool c = false;
  struct ast_node_shell_command *shell_command = malloc(sizeof(struct ast_node_shell_command));
  shell_command->type = VOID;
  shell_command->child.compound_list = NULL;
  struct ast_node_compound_list *compound_list = NULL;
  if ((c=parser_readchar(p,'(')) ||  parser_readchar(p,'{'))
    {
      if(!(compound_list = parser_readcompoundlist(p)))
	return NULL;
      shell_command->child.compound_list = compound_list;
      if((c && !(parser_readchar(p,')'))) || (!c && !(parser_readchar(p,'}'))))
	return NULL;
      return shell_command;
    }
  int b;
  char *command_word = get_word(p);
  switch((b=get_shell_command_type(command_word)))
    {
    case FOR:
      {
	shell_command->child._for = parser_readrulefor(p);
	break;
      }
    case WHILE:
      {
	shell_command->child._while = parser_readrulewhile(p);
	break;
      }
    case CASE:
      {
	shell_command->child._case = parser_readrulecase(p);
	break;
      }
    case IF:
      {
	shell_command->child._if = parser_readruleif(p);
	break;
      }
    case UNTIL:
      {
	shell_command->child._until = parser_readruleuntil(p);
	break;
      }
    default:
      return NULL;
    }
  shell_command->type = b;
  return shell_command;
  
}

struct ast_node_funcdec *parser_readfuncdec (struct parser_s *p)
{
  struct ast_node_funcdec *funcdec = malloc(sizeof(struct ast_node_funcdec));
  funcdec->word = NULL;
  funcdec->shell_command = NULL;
  parser_readtext(p,"function");
  if(!((funcdec->word = parser_readword(p)) && parser_readchar(p,'(') && parser_readchar(p,')')))
     return NULL;
  eat_newlines(p);
  if(!(funcdec->shell_command = parser_readshellcommand(p)))
    return NULL;
  return funcdec;
}

int get_io_number(struct parser_s *p)
{
  int tmp= p->index;
  if(parser_begin_capture(p,"io_number") && parser_readinteger(p) && parser_end_capture(p,"io_number"))
    {
      char *num = parser_get_capture(p, "io_number");
      reinit_capture(p);
      return atoi(num);
    }
  reinit_capture(p);
  p->index=tmp;
  return -1;
}
char *get_redirection(struct parser_s *p)
{
  int tmp = p->index;
  while(parser_readinset(p,"<>-&|"))
    p->index++;
  return strndup(&p->input[tmp],p->index - tmp);
}
int get_redirection_type(struct parser_s *p)
{
  int i = 0;
  char *all_of_types[] = {">","<",">>","<<","<<-",">&","<&",">|","<>"};
  char *redirection = get_redirection(p);
  while(i<10)
    {
      if(strcmp(redirection, all_of_types[i]) == 0)
	return i+1;
      i++;
    }
  return 0;
}
struct ast_node_redirection *parser_readredirection(struct parser_s *p)
{
  struct ast_node_redirection *redirection = malloc(sizeof(struct ast_node_redirection));
  redirection->word_heredoc.word = NULL;
  redirection->next = NULL;
  redirection->io_number = get_io_number(p);
  redirection->redirection_type = get_redirection_type(p);
  if((redirection->io_number == -1) && !(parser_peekinset(p,"<>-&|")))
     return NULL;
  else
    redirection->redirection_type = 1;
  if(!redirection->redirection_type)
    return NULL;
  struct ast_node_word *word = parser_readword(p);
  if(!word)
    return NULL;
  switch (redirection->redirection_type)
    {
    case VOID:
      return NULL;
    case LTLT:
      redirection->word_heredoc.heredoc = word->str;
      redirection->word_heredoc.word = NULL;
      break;
    case LTLTDASH:
      redirection->word_heredoc.heredoc = word->str;
      redirection->word_heredoc.word = NULL;
      break;
    default:
      redirection->word_heredoc.word = word;
      redirection->word_heredoc.heredoc = NULL;
      break;
    }
    return redirection;
  
}
struct ast_node_prefix *parser_readprefix(struct parser_s *p)
{
  struct ast_node_prefix *prefix = malloc(sizeof(struct ast_node_element));
  prefix->next = NULL;
  if(!(prefix->redirection = parser_readredirection(p)) &&
     !(prefix->assignement_word = parser_readassignementword(p)))
    return NULL;
  return prefix;
}
struct ast_node_element *parser_readelement(struct parser_s *p)
{
  struct ast_node_element *element = malloc(sizeof(struct ast_node_element));
  element->next = NULL;
  if(!(element->redirection = parser_readredirection(p)) && !(element->word = parser_readword(p)))
    return NULL;
  return element;
}
struct ast_node_simple_command *parser_readsimplecommand(struct parser_s *p)
{
  struct ast_node_simple_command *simple_command = malloc(sizeof(struct ast_node_simple_command));
  simple_command->element_list = NULL;
  simple_command->prefix_list = NULL;
  struct ast_node_prefix *prefix = parser_readprefix(p);
  simple_command->prefix_list=prefix;
  struct ast_node_prefix *last_prefix = NULL;
  while((last_prefix=parser_readprefix(p)))
    {
      prefix->next = last_prefix;
      prefix = last_prefix;
    }
  struct ast_node_element *element = parser_readelement(p);
  simple_command->element_list=element;
  struct ast_node_element *last_element= NULL;
  while((last_element=parser_readelement(p)))
    {
      element->next = last_element;
      element = last_element;
    }
  if(!(simple_command->element_list) && !(simple_command->prefix_list))
    return NULL;
  return simple_command;
  
  
}
int type_of_command(struct parser_s *p)
{
  int tmp = p->index;
  if(parser_readchar(p,'{') || parser_readchar(p,'(') || parser_readtext(p,"while") || parser_readtext(p,"for") || parser_readtext(p,"if") || parser_readtext(p,"until") || parser_readtext(p,"case"))
    {
      p->index=tmp;
      return SHELL;
    }
  if(parser_readtext(p,"function") || (parser_readidentifier(p) && parser_readchar(p,'(')))
    {
      p->index=tmp;
      return FUNCDEC;
    }
  p->index=tmp;
  return SIMPLE;
}
struct ast_node_command *parser_readcommand (struct parser_s *p)
{
  struct ast_node_command *command = malloc(sizeof(struct ast_node_command));
  command->simple_command = NULL;
  command->shell_command = NULL;
  command->funcdec = NULL;
  command->next = NULL;
  command->redirection_list = NULL;
  command->pipe = false;
  if(type_of_command(p) == SIMPLE)
    {
      command->simple_command = parser_readsimplecommand(p);
      return command;
    }
  if(type_of_command(p) == SHELL)
    command->shell_command = parser_readshellcommand(p);
  if(type_of_command(p) == FUNCDEC)
    command->funcdec = parser_readfuncdec(p);
  struct ast_node_redirection *redirection = parser_readredirection(p);
  command->redirection_list = redirection;
  struct ast_node_redirection *last = NULL;
  while(eat_spaces(p) && (last=parser_readredirection(p)))
    {
      redirection->next = last;
      redirection = last;
    }
    return command;
}
struct ast_node_pipeline *parser_readpipeline (struct parser_s *p)
{
  struct ast_node_pipeline *pipeline = NULL;
  pipeline->relation = VOID;
  pipeline->command = NULL;
  pipeline->reverse = false;
  pipeline->next = NULL;
  if(parser_readchar(p,'!'))
    pipeline->reverse = true;
  struct ast_node_command *command = NULL;
  if (!(command = parser_readcommand(p)))
    return NULL;
  pipeline->command = command;
  struct ast_node_command *last;
  last = NULL;
  while((parser_readchar(p,'|')) && (last = parser_readcommand(p)))
    {
      command->pipe = true;
      eat_newlines(p);
      command->next = last;
      command = last;
    }
  return pipeline;
}
struct ast_node_and_or *parser_readandor (struct parser_s *p)
{
  struct ast_node_and_or *and_or = NULL;
  and_or->linked = false;
  and_or->next = NULL;
  and_or->pipeline = NULL;
  struct ast_node_pipeline *pipeline = NULL;
  if (!(pipeline = parser_readpipeline(p)))
      return NULL;
  p->ast->list->and_ors->pipeline = pipeline;
  struct ast_node_pipeline *last;
  last = NULL;
  bool c = false;
  while((c=(parser_readtext(p,"||"))||(parser_readtext(p,"&&"))) && (last = parser_readpipeline(p)))
      {
	if (c)
	  pipeline->relation = TOKEN_DOUBLE_PIPE;
	else
	  pipeline->relation = TOKEN_DOUBLE_AMPERSAND;
	eat_newlines(p);
	pipeline->next = last;
	pipeline = last;
      }
    return and_or;
}

struct ast_node_list *parser_readlist (struct parser_s *p)
{
    p->ast->list = malloc(sizeof(struct ast_node_list));
    p->ast->list->and_ors = NULL;
    struct ast_node_and_or *and_or = NULL;
    if (!(and_or = parser_readandor(p)))
      return NULL;
    p->ast->list->and_ors = and_or;
    struct ast_node_and_or *last = NULL;
    bool c = false;
    while((parser_readchar(p,';')||(c=(parser_readchar(p,'&')))) && (last = parser_readandor(p)))
      {
	and_or->linked = c;
	and_or->next = last;
	and_or = last;
	c = false;
      }
    return p->ast->list;
    
}

bool parser_readinput (struct parser_s *p)
{
    int tmp = p->index;
    p->ast = malloc(sizeof(struct ast_node_input));
    p->ast->list = NULL;
    if (parser_readchar(p, '\n'))
        return true;
    if (parser_eof(p))
        return true;
    if ((p->ast->list=parser_readlist(p)) && ((parser_readchar(p, '\n') || parser_eof(p))))
        return true;
    p->index = tmp;
    return false;
}
