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
	char *token;
	int i = 0;
	char **memory;
	pid_t child_pid;

	while (1)
	{
		write(1, "#cisfun$ ", 10);
		getline(&buffer, &buffer_size, stdin);
		token = strtok(buffer, "\t\n");
		memory = malloc(sizeof(char *) * 1024);
		while (token != NULL)
		{
			memory[i] =  token;
			token = strtok(NULL, "\t\n");
			i++;
		}
		memory[i] = NULL;
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
	}
		free(buffer);
	return (0);
}
