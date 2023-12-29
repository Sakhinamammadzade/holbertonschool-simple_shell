#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

void execute_command(char **command) {
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == 0) {
        if (execvp(command[0], command) == -1) {
            perror("ERROR execvp:");
            fprintf(stderr, "./hsh: 1: %s: not found\n", command[0]);
            exit(EXIT_FAILURE);
        }
    } else {
        wait(&status);

        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                fprintf(stderr, "./hsh: 1: %s: not found\n", command[0]);
                exit(WEXITSTATUS(status));
            }
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "./hsh: 1: %s: terminated by signal %d\n", command[0], WTERMSIG(status));
            exit(EXIT_FAILURE);
        }
    }
}

int main(void) {
    int status;
    char *buffer = NULL;
    size_t buffer_size = 0;
    char *token;
    char **commands;
    int i, j, k;

    while (1) {
        if (getline(&buffer, &buffer_size, stdin) == -1)
            break;

        i = 0;
        token = strtok(buffer, ";\t\n");

        if (token == NULL)
            continue;

        commands = malloc(sizeof(char *) * 1024);

        while (token != NULL) {
            commands[i] = malloc(strlen(token) + 1);
            strcpy(commands[i], token);
            token = strtok(NULL, ";\t\n");
            i++;
        }

        commands[i] = NULL;

        for (j = 0; j < i; j++) {
            if (strcmp(commands[j], "exit") == 0) {
                free(buffer);
                for (k = 0; k < i; k++)
                    free(commands[k]);
                free(commands);
                exit(EXIT_SUCCESS);
            } else {
                execute_command(commands[j]);
            }
        }

        for (j = 0; j < i; j++)
            free(commands[j]);
        free(commands);
    }

    free(buffer);
    return 0;
}

