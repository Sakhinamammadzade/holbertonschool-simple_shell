#include <unistd.h>
#include<stdio.h>
/**
 * main - PID
 *
 * Return: Always 0.
 */
int main(void)
{
pid_t pid, ppid;
pid = getpid();
ppid = getpid();
printf("pid = %d\n parent pid= %d\n", pid, ppid);
return (0);
}
