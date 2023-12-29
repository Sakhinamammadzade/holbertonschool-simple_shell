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

            // Check if the command exists in PATH
            if (access(memory[0], X_OK) == 0)
            {
                child_pid = fork();
                if (child_pid == 0)
                {
                    if (is_piped_input)
                    {
                        // Use popen to handle pipes
                        FILE *pipe_fp = popen(buffer, "r");
                        if (pipe_fp == NULL)
                        {
                            perror("ERROR popen:");
                            exit(EXIT_FAILURE);
                        }

                        // Read from the pipe
                        char line[1024];
                        while (fgets(line, sizeof(line), pipe_fp) != NULL)
                        {
                            printf("%s", line);
                        }

                        // Close the pipe
                        if (pclose(pipe_fp) == -1)
                        {
                            perror("ERROR pclose:");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        if (execvp(memory[0], memory) == -1)
                        {
                            perror("ERROR execvp:");
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                else
                {
                    wait(&status);
                    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
                    {
                        fprintf(stderr, "Command failed with exit status: %d\n", WEXITSTATUS(status));
                    }
                }
            }
            else
            {
                fprintf(stderr, "Command not found: %s\n", memory[0]);
            }
        }

        for (j = 0; j < i; j++)
            free(memory[j]);
    }

    free(buffer);
    free(memory);

    return 0;
}
