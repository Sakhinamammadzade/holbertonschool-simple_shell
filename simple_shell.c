#include "shell.h"
/**
 * main - main function to start
 * Return: Always 0
 */
int main(void)
{
	char *buffer = NULL;
	size_t buffer_size = 0;
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
		buffer[strcspn(buffer, "\n")] = '\0';
		if (buffer[0] == '\0')
		{
			free(buffer);
			continue;
		}
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Fork");
			exit(EXIT_FAILURE);
		}
		if (child_pid == 0)
		{
			if (execlp(buffer, buffer, NULL) == -1)
			{
				perror("./shell: ");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			waitpid(child_pid, &status, 0);
			if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
				fprintf(stderr, "%s: command not found\n", buffer);
			free(buffer);
			buffer = NULL;
			buffer_size = 0;
		}
	}
	return (0);
}
