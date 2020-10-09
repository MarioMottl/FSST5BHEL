/*
    Project: Aufgabe 11
    Author: Mario Mottl 5BHEL
    Date: 08.10.2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

// returns the exitcode of the child process
typedef int (*child_func_pointer_t)();

/*
Function : createfile()
Description: creates requested file 
*/
void createfile()
{
    int fd;
    if ((fd = open("/tmp/f1",O_CREAT)) != -1)
    {
        printf("Succesfully created file\r\n");
    }
    else
    {
        printf("Error creating file\r\n");
    }
    close(fd);
}

/*
Function : get_vendorid
Description: gets vendor_id from /proc/cpuinfo
*/
void get_vendorid()
{
    char buffer[50];
    int fd;
    char *ptr;
    char delimiter[2] = "\n";
    if ((fd = open("/proc/cpuinfo",O_RDONLY)) != -1)
    {
        printf("Succesfully opened file with name %s\r\n","/proc/cpuinfo");
    }
    else
    {
        printf("Error opening file\r\n");
    }    
    if((read(fd,buffer,sizeof(buffer)))!= -1)
    {
        ptr = strtok(buffer, delimiter);
        ptr = strtok(NULL, delimiter); //disregard first line
        printf("%s\r\n",ptr); 
    }
    else
    {
        printf("Error reading data on file\r\n");
    }
}

/*
Function : fork_child_in_function()
Description: returns -1 on error, child pid otherwise
             prints pid and ppid
*/
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

/*
Function : wait_child()
Description: waits for child process to die
             checks if child died a natural death or terminated abnormally
*/
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

/*
Function : grandchild1_func()
Description: adds x premission to created file
*/
int grandchild1_func()
{
    sleep(5);
    execl("/usr/bin/chmod","chmod","+x","/tmp/f1",NULL);
    return 5;
}

/*
Function : grandchild2_func()
Description: lists the rights of requested file
*/
int grandchild2_func()
{
    sleep(10);
    //getfacl /tmp/f1
    execl("/usr/bin/getfacl","getfacl" , "/tmp/f1",NULL);
    return 6;
}

/*
Function : child_func()
Description: creates the requested file,
             furthermore creates two grandchildren
*/
int child_func()
{
    createfile();
    int grandchild1_pid, grandchild2_pid;
    grandchild1_pid = fork_child_in_function(grandchild1_func, "grandchild1");
    grandchild2_pid = fork_child_in_function(grandchild2_func, "grandchild2");
    if (grandchild1_pid > 0)
        wait_child(grandchild1_pid, "grandchild1");
    if (grandchild2_pid > 0)
        wait_child(grandchild2_pid, "grandchild2");
    return 0;
}

int main(int argc,char **argv)
{
    int child_pid;
    get_vendorid();
    child_pid = fork_child_in_function(child_func, "child1");
    if(child_pid >0)
    {
        wait_child(child_pid, "child1");
    }
    return 0;
}