#include "shell.h"

extern char **environ;

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
            break;
        i = 0;
        token = strtok(buffer, " \t\n");
        if (token == NULL)
            continue;
        while (token != NULL)
        {
            memory[i] = malloc(strlen(token) + 1);
            strcpy(memory[i], token);
            token = strtok(NULL, " \t\n");
            i++;
        }
        memory[i] = NULL;
        if (i > 0 && strcmp(memory[0], "exit") == 0)
        {
            for (j = 0; j < i; j++)
                free(memory[j]);
            free(buffer);
            free(memory);
            exit(EXIT_SUCCESS);
        }
        else if (i > 0 && strcmp(memory[0], "env") == 0)
        {
            char **env_ptr = environ;
            while (*env_ptr != NULL)
            {
                printf("%s\n", *env_ptr);
                env_ptr++;
            }
        }
        else
        {
            is_piped_input = isatty(fileno(stdin)) == 0;
            child_pid = fork();
            if (child_pid == 0)
            {
                if (is_piped_input)
                {
                    if (execvp(memory[0], memory) == -1)
                    {
                        perror("ERROR execvp:");
                        fprintf(stderr, "./hsh: 1: %s: not found\n", memory[0]);
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    if (execvp(memory[0], memory) == -1)
                    {
                        perror("ERROR execvp:");
                        fprintf(stderr, "./hsh: 1: %s: not found\n", memory[0]);
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else
            {
                wait(&status);
                if (WIFEXITED(status))
                {
                    if (WEXITSTATUS(status) != 0)
                    {
                        exit(WEXITSTATUS(status));
                    }
                }
                else if (WIFSIGNALED(status))
                {
                    fprintf(stderr, "./hsh: 1: %s: terminated by signal %d\n", memory[0], WTERMSIG(status));
                    exit(EXIT_FAILURE);
                }
            }
        }
        for (j = 0; j < i; j++)
            free(memory[j]);
    }
    free(buffer);
    free(memory);
    return 0;
}
