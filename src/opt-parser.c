#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "opt-parser.h"

int opt_parse(int argv, char *argc[], int *argi)
{
    unsigned mult_char_opt = 1;
    if (argc[*argi][0] == '-' && argc[*argi][1] == '-')
    {
	for(; *argi < argv && mult_char_opt; *argi += 1)
	{
	    if (!strcmp(argc[*argi], "--version"))
		puts("Version 0.3");	

	    else if (!strcmp(argc[*argi], "--norc"))
		puts("\"--norc\" option not implemented yet");	

	    else if (!strcmp(argc[*argi], "--ast-print"))
		puts("\"--ast-print\" option not implemented yet");

	    else
	    {
		puts("Invalid multi-character option");
		return 1;
	    }

	    mult_char_opt = 0;
	}
    }

    for (; *argi < argv && (argc[*argi][0] == '-' || argc[*argi][0] == '+');
	    *argi += 1)
    {
	if (!strcmp(argc[*argi], "-c"))
	    puts("\"-c\" option not implemented yet");	

	else if (!strcmp(argc[*argi], "-0") || !strcmp(argc[*argi], "+0"))
	    puts("\"[-+]0\" option not implemented yet");

	else
	{
	    puts("Invalid option");
	    return 1;


	}	
    }

    return 0;
}