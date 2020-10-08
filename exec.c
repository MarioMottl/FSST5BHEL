#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char **argv)
{
    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        printf ("Kindprozess:  %d (PID: %d)\n", getpid());
        execl("/usr/bin/ps","ps",NULL);
        exit (0);
    }
    else if (pid > 0)
    {
        int e;
        wait(&e);
        printf ("Elternprozess: %d (PID: %d)\n", getpid());
    }
    else
    {
        fprintf (stderr, "Fehler");
        exit (1);
    }
    return 0;
}