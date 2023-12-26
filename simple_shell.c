#include "shell.h"
int is_interactive(void) {
    return isatty(STDIN_FILENO);
}
int main(void)
{
	int status;
	char *buffer = NULL;
	size_t buffer_size = 0;
	char *token;
	int i = 0;
	char **memory;
	pid_t child_pid;
	int interactive = is_interactive();

	while (1)
	{
		if (interactive)
			write(1, "#cisfun$ ", 10);
		if (getline(&buffer, &buffer_size, stdin) == -1)
		{
			free(buffer);
			printf("\n");
			exit(EXIT_SUCCESS);
		}
		if (buffer[strlen(buffer) - 1] == '\n')
		{
			buffer[strlen(buffer) - 1] = '\0';
		}
		token = strtok(buffer, "\t\n");
		memory = malloc(sizeof(char *) * 1024);
		while (token != NULL)
		{
			memory[i] =  token;
			token = strtok(NULL, "\t\n");
			i++;
		}
		memory[i] = NULL;
		if (i > 1)
		{
			fprintf(stderr, "Error: Only one-word commands are allowed\n");
			free(memory);
			continue;
		}
		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(memory[0], memory, NULL) == -1)
				perror("ERROR execve:");
		}
		else
			wait(&status);
		i = 0;
		free(memory);
		if (!interactive)
			break;
	}
	free(buffer);
	return (0);
}
