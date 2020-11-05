#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h>
#define msgsize 16

int main()
{
    int fd[2];
    int pid;
    char inbuf[msgsize];

    if(pipe(fd) < 0)
    {
        exit(1);
    }

    if((pid = fork()) > 0)
    {
        //Kindprozess
        write(fd[1],"Hallo Welt #1",msgsize);
        write(fd[1],"Hallo Welt #2",msgsize);
        write(fd[1],"Hallo Welt #3",msgsize);
    }
    else
    {
        //Elternprozess
        while(read(fd[0],inbuf,msgsize))
        {
            printf("%s \n", inbuf);
        }
    }
    return 0;
}

