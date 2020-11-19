#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

typedef int (*child_func_pointer_t)();

#define LFDNR 11;

pid_t fork_child_in_function(child_func_pointer_t child_func, const char child_name[])
{
    pid_t child_pid = fork();
    if (child_pid == 0)
    {
        printf("child %s  pid=%d (parent_pid=%d)\n", child_name, getpid(), getppid());
        exit(child_func());
    }
    if (child_pid < 0)
        printf("Error forking %s!\n", child_name);
    return child_pid;
}

void wait_child(pid_t child_pid, const char child_name[])
{
    int child_status;
    waitpid(child_pid, &child_status, 0);
    if (WIFEXITED(child_status))
        printf("%s has exited with exit code %d\n", child_name, WEXITSTATUS(child_status));
    else
        printf("%s has terminated abnormally\n", child_name);
}

void fork_and_wait_child(child_func_pointer_t child_func, const char child_name[])
{
    pid_t child_pid = fork_child_in_function(child_func, child_name);
    wait_child(child_pid, child_name);
}

void createfile(const char filename[])
{
    int fd = open(filename, O_CREAT | 00777);
    close(fd);
}

void writetofile(const char filename[], const char message[])
{
    int fd = open(filename, O_WRONLY | O_APPEND);
    if((write(fd,message,strlen(message))) != -1)
    {
        printf("Written %s to %s\n",message,filename);
    }
    else
    {
        printf("Error writting\n");
    }
    close(fd);
}

void readfromfile(const char filename[],char buffer[],size_t len)
{
    int fd = open(filename,O_RDONLY);
    if((read(fd,buffer,len))!= -1)
    {
        printf("%s\n", buffer);
    }
    else
    {
        printf("Error couldnt read from file %s\n", filename);
    }
    close(fd);
}

void contains(char buffer[], const char message[])
{
    char *ptr;
    ptr = strstr(buffer,message);
    if(ptr != NULL)
    {
        printf("Searched for : '%s' \t Found : '%s'\r\n",message,ptr);
    }
    else
    {
        printf("Cant find %s\r\n",message);
    }   
}

void getkey(char buffer[],const char key[])
{
    char *ptr;
    ptr = strpbrk(buffer,key);
    if(ptr != NULL)
    {
        printf("Found = '%s' with key = '%s'\r\n",ptr,key);
    }
}

bool checkforfile(const char filename[])
{
    return (access(filename, F_OK) == -1) ? false : true;
}

int grandchild1()
{
    sleep(1);
    writetofile("/tmp/help/familysecret", "secret:1337\n");
    sleep(1);
    return 10;
}

int child1()
{
    sleep(2);
    mkdir("/tmp/help",0755);
    createfile("/tmp/help/familysecret");
    writetofile("/tmp/help/familysecret", "secret:654723\n");
    sleep(2);
    fork_and_wait_child(grandchild1, "Gnackfotznhansl");
    sleep(2);
    writetofile("/tmp/help/familysecret", "secret:1007\n");
    return 5;
}
/*
Paste child/grandchild functions calls here
*/

int main()
{
    sleep(3);
    fork_and_wait_child(child1,"Rucksocksepp");
    char buff[1024];
    readfromfile("/tmp/help/familysecret", buff, sizeof(buff));
    getkey(buff, "0123456789");
    sleep(2);
    execlp("rm", "rm", "-r", "/tmp/help", NULL);
    return 0;
}