
	
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int status;
    char *buffer = NULL;
    size_t buffer_size = 0;
    char *token, *saveptr;
    char **memory;
    pid_t child_pid;

    while (1)
    {
        if (getline(&buffer, &buffer_size, stdin) == -1)
        {
            printf("\n");
            break;
        }

        token = strtok_r(buffer, " \t\n", &saveptr);
        memory = malloc(sizeof(char *));
        int i = 0;
        while (token != NULL)
        {
            memory = realloc(memory, (i + 1) * sizeof(char *));
            memory[i] = strdup(token);
            token = strtok_r(NULL, " \t\n", &saveptr);
            i++;
        }
        memory[i] = NULL;

        child_pid = fork();
        if (child_pid == 0)
        {
            if (execvp(memory[0], memory) == -1)
                perror("ERROR execvp:");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(&status);
            for (int j = 0; j <= i; j++)
            {
                free(memory[j]);
            }
            free(memory);
        }
    }
    free(buffer);
    return 0;
}
