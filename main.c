#include "shell.h"
int main(void)
{
	int i = 0;
	char *buffer = NULL;
	size_t buffer_size = 0;
	char *token;
	pid_t child_pid;
	int status;
	char **parsed_command;

	while (1)
	{
		write(1, "#cisfun$ ", 10);
		if (getline(&buffer, &buffer_size, stdin) == -1)
		{
			printf("\n");
			break;
		}
		parsed_command = malloc(sizeof(char *) * 1024);
		token = strtok(buffer, "\t\n");
		while (token)
		{
			parsed_command[i] = token;
			token = strtok(NULL, "\t\n");
			i++;
		}
		parsed_command[i] = NULL;
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Forking Error:");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			if (execve(parsed_command[0], parsed_command, NULL) == -1)
			{	
				perror("Error executing command");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(&status);
			i = 0;
		}
	}
	return (0);
}
