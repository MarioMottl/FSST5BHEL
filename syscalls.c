#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

/*
Implementation of the syscalls

Terminal:
    gcc syscalls.c -o syscalls
    ./syscalls

Short introduction:
    Flags:
        O_CREAT : creats file if it doesnt exist. If it exists O_CREAT is useless
        O_APPEND : opens file to write at the end
        O_EXCL : combined with O_CREAT means that file cant be opened open() return -1
        O_TRUNC : opened file will be cleared
        O_RDONLY : file will be opend to read only
        O_WRONLY : file will be opend to write only
        O_BINARY : opens file in binary
        O_TEXT : opens file in text mode
        ... many more
    open:
        Returns an int, -1 on error, != -1 success
        Syntax:
            int open(const char *pfad, int modus);
            int open(const char *pfadname, int flags, mode_t zugriffsrechte);
            e.g. open("/home/text.txt",O_RDONLY)
                 open("/home/text.txt",O_RDONLY|O_CREAT)
    write:
        Returns an int, -1 on error otherwise returns count of succesfull writen bytes
        Syntax:
            int write(int fd, const void *puffer, size_t bytezahl);
    read:
        Returns an int, -1 on error otherwise count of successfully read bytes
        Syntax:
            int read(int fd, const void *puffer, site_t bytezahl);
    close:
        Returns an int, -1 on error othwerwise 0 if succesfull
        Syntax:
            int close(int fd);
*/

int exampleopen(char **argv,char mode);
void exampleread(int fd);
void examplewrite(int fd);

int exampleopen(char **argv,char mode)
{
    int fd;
    if(mode == 'w')
    {
        if ((fd = open(argv[1],O_WRONLY|O_CREAT)) != -1)
        {
            printf("Succesfully created file with name %s\r\n",argv[1]);
        }
        else
        {
            printf("Error creating file\r\n");
        }
    }
    if(mode == 'r')
    {
        if ((fd = open(argv[1],O_RDONLY)) != -1)
        {
            printf("Succesfully opened file with name %s\r\n",argv[1]);
        }
        else
        {
            printf("Error opening file\r\n");
        }       
    }
    return fd;
}

void examplewrite(int fd)
{
    char buffer[24];
    strcpy(buffer,"Hello World");
    if((write(fd,buffer,strlen(buffer)))!= -1)
    {
        printf("Succesfully wrote %s\r\n",buffer);
    }
    else
    {
        printf("Error writing data on file\r\n");
    }
}

void exampleread(int fd)
{
    char buffer[24];
    if((read(fd,buffer,sizeof(buffer)))!= -1)
    {
        printf("Succesfully read %s\r\n",buffer);
    }
    else
    {
        printf("Error reading data on file\r\n");
    }
}

int main(int argc,char **argv)
{
    int fd;
    if(argc == 1)
    {
        printf("usage: ./syscalls <filename>\r\n");
        return -1;
    }
    else
    {
        fd = exampleopen(argv,'w');
        examplewrite(fd);
        close(fd);
        fd = exampleopen(argv,'r');
        exampleread(fd);
        close(fd);
    }
    return 0;
}