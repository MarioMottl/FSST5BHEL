/*
usage: gcc copy.c -o copy
./copy test.txt test2.txt
Erste File muss vorhanden sein
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    FILE* fhandlein;
    FILE* fhandleout;

    char dataToBeRead [50];
    if(argc < 3)
    {
        printf("usage: %s <Filename> <Filename2>\n",argv[0]);
        return -1;
    }
    else if (argc == 3)
    {
        fhandlein = fopen(argv[1],"r");
        fhandleout = fopen(argv[2],"w"); 

        fseek(fhandlein,10,SEEK_SET); //disregard first 10 byte
        
        while( fgets ( dataToBeRead, 50, fhandlein ) != NULL ) //read file in 50 byte blocks
        {   
            printf( "%s" , dataToBeRead ); //output it to console
            fwrite(dataToBeRead,1,strlen(dataToBeRead),fhandleout); //output it to the second file
        }
        
        fclose(fhandlein);
        fclose(fhandleout);
    }
    return 0;
}