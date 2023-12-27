#include "shell.h"
int main(void)
{
    int status;
    char *buffer = NULL;
    size_t buffer_size = 0;
    char *token;
    int i = 0;
    char **memory;
    pid_t child_pid;
    int j;
	int is_piped_input;
    memory = malloc(sizeof(char *) * 1024);

    while (1)
    {
        if (getline(&buffer, &buffer_size, stdin) == -1)
        {
            break;
        }

        i = 0;

        token = strtok(buffer, " \t\n");

        if (token == NULL)
        {
            continue;
        }

        while (token != NULL)
        {
            memory[i] = malloc(strlen(token) + 1);
            strcpy(memory[i], token);
            token = strtok(NULL, " \t\n");
            i++;
        }

        memory[i] = NULL;

        is_piped_input = isatty(fileno(stdin)) == 0;

        child_pid = fork();
        if (child_pid == 0)
        {
            if (is_piped_input)
            {
                if (execvp(memory[0], memory) == -1)
                {
                    perror("ERROR execvp:");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                if (execlp("/bin/sh", "sh", "-c", buffer, (char *)NULL) == -1)
                {
                    perror("ERROR execlp:");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            wait(&status);
        }

        for (j = 0; j < i; j++)
        {
            free(memory[j]);
        }
    }

    free(buffer);
    free(memory);

    return 0;
}
