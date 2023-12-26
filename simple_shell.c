#include "shell.h"
/**
 * main - main function to start
 * Return: Always 0
 */
int main(void)
{
	char *buffer = NULL;
	size_t buffer_size = 0;
	int i = 0;
	int j;
	char *token;
	char **memory;
	int status;
	pid_t child_pid;

	while (1)
	{
		write(1, "#cisfun$ ", 9);
		if (getline(&buffer, &buffer_size, stdin) == -1)
		{
			perror("getline");
			break;
		}
		if (buffer[0] == '\n')
		{
			free(buffer);
			continue;
		}
		token = strtok(buffer, "\t\n");
		memory = malloc(sizeof(char *) * 1024);
		while (token != NULL)
		{
			memory[i] = strdup(token);
			token = strtok(NULL, "\t\n");
			i++;
		}
		memory[i] = NULL
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Fork");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			if (execve(memory[0], memory) == -1)
			{
				perror("./shell: ");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
		wait(&status);
		for (j = 0; j < i; j++)
			free(memory[j]);
		free(memory);
		i = 0;
		}
		free(buffer);
		buffer = NULL;
		buffer_size = 0;
	}
	return (0);
}
