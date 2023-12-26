#include "main.h"
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
		getline(&buffer, &buffer_size, stdin);
		token = strtok(buffer, "\t\n");
		memory = malloc(sizeof(char *) * 1024);
		while (token != NULL)
		{
			memory[i] = token;
			token = strtok(NULL, "\t\n");
			i++;
		}
		memory[i] = NULL;
		free(memory);
		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(memory[0], memory, NULL) == -1)
				perror("./shell: ");
		}
		else
			wait(&status);
		i = 0;
	}
	free(buffer);
	return (0);
}
