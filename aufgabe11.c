#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid,pid2,pid3;
    pid = fork();

    if (pid == 0)
    {
        pid2 = fork();
        pid3 = fork();
        if (pid2 == 0)
        {
            sleep(5);
            execl("/usr/bin/chmod","chmod","+x","/tmp/f1",NULL);
        }
        else
        {
            fprintf (stderr, "Fehler chmod\r\n");
            exit (1);
        }
        if (pid3 == 0)
        {
            sleep(10);
            //find selftouch.c -printf "%m\n"
            execl("/usr/bin/find","find","/tmp/f1","-printf ","""%m\n""",NULL);
        }
        else
        {
            fprintf (stderr, "Fehler find\r\n");
            exit (1);
        }
        printf ("Kindprozess:  %d (PID: %d)\n", getpid());
        execl("/home/mariomottl/Schreibtisch/FSST5BHEL/selftouch","./selftouch","/tmp/f1",NULL);
    }
    else if (pid > 0)
    {
        int e;
        waitpid(pid,&e,0);
        printf ("Elternprozess: %d (PID: %d)\n", getpid());
    }
    else
    {
        fprintf (stderr, "Fehler");
        exit (1);
    }
    return 0;
}
