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

    memory = malloc(sizeof(char *) * 1024);  // Move memory allocation outside the loop

    while (1)
    {
        if (getline(&buffer, &buffer_size, stdin) == -1)
        {
            break;
        }

        // Reset i for each iteration
        i = 0;

        // Tokenize with any whitespace character as delimiter
        token = strtok(buffer, " \t\n");

        // Check if there is at least one non-empty token
        if (token == NULL)
        {
            continue; // Skip empty lines
        }

        while (token != NULL)
        {
            memory[i] = malloc(strlen(token) + 1);  // Allocate memory for each token
            strcpy(memory[i], token);
            token = strtok(NULL, " \t\n");
            i++;
        }

        memory[i] = NULL;

        child_pid = fork();
        if (child_pid == 0)
        {
            if (execve(memory[0], memory, NULL) == -1)
                perror("ERROR execve:");

            // This block should only be reached if execve fails
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(&status);

            // Free allocated memory in the parent process
            for (int j = 0; j < i; j++)
            {
                free(memory[j]);
            }
        }
    }

    free(buffer);
    free(memory);

    return 0;
}
