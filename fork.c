#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main ()
{
   int i, j;
   pid_t pid;

   pid = fork();

   if (pid == 0)
   {
      /* Kindprozess
       * wenn fork() eine 0 zurückgibt, befinden wir uns im Kindprozess
       */
      for (j=0; j < 10; j++)
      {
        printf ("Kindprozess:  %d (PID: %d)\n", j, getpid());
        sleep (1);
      }
      exit (0);
   }
   else if (pid > 0)
   {
      /* Elternprozess
       * Gibt fork() einen Wert größer 0 zurück, befinden wir uns im Elternprozess
       * in pid steht die ID des Kindprozesses
       * getpid() gibt die eigene PID zurück
       */
      for (i=0; i < 10; i++)
      {
         printf ("Elternprozess: %d (PID: %d)\n", i, getpid());
         sleep (1);
      }
   }
   else
   {
      /* Wird ein negativer Wert zurückgegeben, ist ein Fehler aufgetreten */
      fprintf (stderr, "Fehler");
      exit (1);
   }
   return 0;
}