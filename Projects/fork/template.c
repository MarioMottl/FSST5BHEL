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
Description: 
*/
int grandchild1_func()
{
    sleep(1);
    /*
    Insert grandchildren only code here
    */
    return 5;
}

/*
Function : grandchild2_func()
Description: 
*/
int grandchild2_func()
{
    sleep(1);
    /*
    Insert grandchildren only code here
    */
    return 6;
}

int child_func()
{
    int grandchild1_pid, grandchild2_pid;
    /*
    Insert children only code here
    */
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
    /*
    Insert parent only code here
    */
    printf("parent pid :%d\r\n",getpid());
    child_pid = fork_child_in_function(child_func, "child1");
    if(child_pid >0)
    {
        wait_child(child_pid, "child1");
    }
    return 0;
}