#include <unistd.h>
#include<stdio.h>
/**
 * main - PID
 *
 * Return: Always 0.
 */
char **splitter(char *str)
{
char *token, **tokens = NULL;
int i = 0;

token = strtok(str, " \t\n");
while (token != NULL)
{
i++;
tokens = realloc(tokens, i*sizeof(char *));
if (tokens == NULL)
{
perror("realloc");
exit(EXIT_FAILURE);
}
tokens[i - 1] = token;
token = strtok(NULL, " \t\n");
}
for (i = 0; tokens[i]; i++)
printf("%s\n", tokens[i]);
return (tokens);
}


int main(int ac, char **av)
{
(void)ac;
char *buffer = NULL;
size_t size_buffer = 0;
int n_char = 0;

write(1, "$ ", 2);
n_char = getline(&buffer, &size_buffer, stdin);
if (n_char == EOF)
   perror("getline");
buffer[n_char - 1] = '\0';
printf("%s\n", buffer);
return (0);
}
