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
		token = strtok(buffer, "\t\n");
		memory = malloc(sizeof(char *) * 1024);
		while (token != NULL)
		{
			memory[i] = strdup(token);
			token = strtok(NULL, "\t\n");
			i++;
		}
		memory[i] = NULL;
		free(buffer);
		buffer = NULL;
		buffer_size = 0;
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Fork");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			if (execve(memory[0], memory, NULL) == -1)
			{
				perror("./shell: ");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
		wait(&status);
		int j;
		for (j = 0; j < i; j++)
			free(memory[j]);
		free(memory);
		i = 0;
		}
	}
	free(buffer);
	return (0);
}
