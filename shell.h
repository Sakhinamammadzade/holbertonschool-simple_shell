#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Function prototypes */
int  _getline(int argc, char *argv[]);
int is_interactive(void);
int execute_command(char **args);

#endif /* SHELL_H */

