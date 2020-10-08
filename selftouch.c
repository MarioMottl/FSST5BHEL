/*
Filename: selftouch.c
Description: linux system function touch programmed in c
Author: Mario Mottl
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    FILE* fhandle;
    if(argc ==1)
    {
        printf("usage: %s <Filename>\n",argv[0]);
        return -1;
    }
    if (argc == 2)
    {
        fhandle = fopen(argv[1],"w");
        char buffer[512];
        scanf("%s",buffer);
        fputs(buffer,fhandle);
        fclose(fhandle);  
    }
    return 0;
}