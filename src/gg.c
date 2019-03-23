#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define ARGS_SIZE 64

struct command_archive
{
    char *string;
    struct command_archive *next;
};

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

void archive_command(struct command_archive *commands_history, char *line)
{
    struct command_archive *archive = commands_history;

    if (!archive)
    {
        archive = malloc(sizeof(struct command_archive));
    }
    else
    {
        while (archive->next)
        {
            archive = archive->next;
        }
    }

    archive->next = malloc(sizeof(struct command_archive));
    archive->string = line;
}

void print_history(struct command_archive *command_archive)
{
    if (command_archive)
    {
        return NULL;
    }

    struct command_archive *current = command_archive;
    while(command_archive)
    {
        puts(command);
    }
}

int  main()
{
    char  line[BUFFER_SIZE];
    char  *argv[ARGS_SIZE];
    struct command_archive *commands_history = NULL;
    while (1)
    {
        printf("42sh$ ");
        char *s = fgets(line, BUFFER_SIZE, stdin);
        int size = strlen(s);
        
        archive_command(commands_history, line);

        printf("\n");

        if (line[0] == '\n')
        {
            continue;
        }

        line[size - 1] = '\0';
        parse(line, argv);

        if (strcmp(argv[0], "exit") == 0)
        {
            exit(0);
        }
        execute(argv);
        //parser_readinput(line);
    }
}
