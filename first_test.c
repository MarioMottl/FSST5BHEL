#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

typedef int (*child_func_pointer_t)();

pid_t fork_child_in_function(child_func_pointer_t child_func, const char* child_name)
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

void wait_child(pid_t child_pid, const char* child_name)
{
    int child_status;
    waitpid(child_pid, &child_status, 0);
    if (WIFEXITED(child_status))
        printf("%s has exited with exit code %d\n", child_name, WEXITSTATUS(child_status));
    else
        printf("%s has terminated abnormally\n", child_name);
}

void fork_and_wait_child(child_func_pointer_t child_func, const char* child_name)
{
    pid_t child_pid = fork_child_in_function(child_func, child_name);
    if (child_pid > 0)
        wait_child(child_pid, child_name);
}

void childfunc1()
{
    sleep(2);
    char *ptr;
    char delimiter[2] = "\n";
    int fd = open("/tmp/family",O_RDONLY);
    char buffer[24];
    if((read(fd,buffer,sizeof(buffer)))!= -1)
    {
        ptr = strtok(buffer,delimiter);
        ptr = strtok(NULL,delimiter);
        printf("Family Secret %s",ptr);
    }
    else
    {
        printf("Error reading data on file\r\n");
    }
}
void childfunc2()
{
    //familycode:xxxxx
    //11:xxx
    int fd = open("/tmp/family",O_RDONLY);
    char buffer[24];
    char *ptr;
    char delimiter[2] = "\n";
    if((read(fd,buffer,sizeof(buffer)))!= -1)
    {
        printf("Family Secret %s",buffer);
        ptr = strtok(buffer,delimiter);
        ptr = strtok(NULL,delimiter);
        printf("Family Secret %s",ptr+12);
    }
    else
    {
        printf("Error reading data on file\r\n");
    }
    remove(&fd);
    close(fd);

}
void childfunc3()
{
    sleep(10);
}

void writesecret()
{
    int fd = open("/tmp/family",O_WRONLY|O_CREAT);
    char buffer[24];
    strcpy(buffer,"\nfamilycode:654321");
    if((write(fd,buffer,strlen(buffer)))!= -1)
    {
        printf("Succesfully wrote %s\r\n",buffer);
    }
    else
    {
        printf("Error writing data on file\r\n");
    }
}

int main()
{
    sleep(3); //lebt 3 Jahre 
    writesecret();
    //firstchild
    fork_and_wait_child(childfunc1,"Kind1");
    sleep(1);
    //secondchild
    fork_and_wait_child(childfunc2,"Kind2");
    sleep(1);
    //third 
    fork_and_wait_child(childfunc3,"Kind3");
    sleep(1);
    //check for file
    int fd = open("/tmp/family",O_RDONLY);
    char buffer[24];
    if((read(fd,buffer,sizeof(buffer)))!= -1)
    {
        printf("vorhanden");
    }
    else
    {
        printf("Was habe ich nur falsch gemacht");
    }
    return 0;
}