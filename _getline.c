#include <stdio.h>
#include <stdlib.h>
/**
 * _getline - Entry point of the program
 * @argc: The number of command-line arguments
 * @argv: An array of strings representing the command-line arguments
 *
 * Return: Always 0 on success, 1 on failure
 */

int  _getline(int argc, char *argv[])
       {
           FILE *stream;
           char *line = NULL;
           size_t len = 0;
           ssize_t nread;

           if (argc != 2) {
               fprintf(stderr, "Usage: %s <file>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           stream = fopen(argv[1], "r");
           if (stream == NULL) {
               perror("fopen");
               exit(EXIT_FAILURE);
           }

           while ((nread = getline(&line, &len, stream)) != -1) {
               printf("Retrieved line of length %zd:\n", nread);
               fwrite(line, nread, 1, stdout);
           }

           free(line);
           fclose(stream);
           return (EXIT_SUCCESS);
       }
