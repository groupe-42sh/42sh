#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define ARGS_SIZE 64

void  parse(char *line, char **argv)
{
    while (*line != '\0')
    {
        while (*line == ' ' || *line == '\t' || *line == '\n')
        {
            *line++ = '\0';
        }

        *argv++ = line;

        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
        {
            line++;
        }
    }

    *argv = '\0';
}

void  execute(char **argv)
{
    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0)
    {
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        if (execvp(*argv, argv) < 0)
        {
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else
    {
        while (wait(&status) != pid)
        {
        }
    }
}

int  main()
{
    char  line[BUFFER_SIZE];
    char  *argv[ARGS_SIZE];
    
    while (1)
    {
        printf("42sh$ ");
        fgets(line, BUFFER_SIZE, stdin);
        printf("\n");
        //parse(line, argv);
        if (strcmp(argv[0], "exit") == 0)
            exit(0);
        //execute(argv);
        //parser_readinput(line);
    }
}
