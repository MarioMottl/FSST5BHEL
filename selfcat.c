/*
Filename: selflcat.c
Description: linux system function cat programmed in c
Author: Mario Mottl
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    FILE* fhandle;
    char dataToBeRead [50];
    if(argc ==1)
    {
        printf("usage: %s <Filename>\n",argv[0]);
        return -1;
    }
    if (argc == 2){
        fhandle = fopen(argv[1],"r");
        while( fgets ( dataToBeRead, 50, fhandle ) != NULL ) 
        {   
            printf( "%s" , dataToBeRead ); 
        }
        fclose(fhandle);  
    }
    return 0;
}