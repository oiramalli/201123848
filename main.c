#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Functions.h"
#include "MakeDisk.c"

/* Maximum comando size + 1. */
#define MAX_COMMAND_SZ 256

int main(int argC, char *argV[]) {
    char *comando;
    do{
        /* Allocate memory and check if okay. */
        comando = malloc (MAX_COMMAND_SZ);
        if (comando == NULL) {
            printf ("No memory\n");
            return 1;
        }

        /* Ask user for comando. */
        printf("What is your commad?>");

        /* Get the comando, with size limit. */
        fgets (comando, MAX_COMMAND_SZ, stdin);

        /*Remove trailing newline, if there. */
        if ((strlen(comando)>0) && (comando[strlen (comando) - 1] == '\n'))
            comando[strlen (comando) - 1] = '\0';

        /* EDIT */
        char * parametros[50];
        size_t n = 0;
        for (char * p = strtok(comando, "-"); p; p = strtok(NULL, "-"))
        {
            if (n >= 50)
            {
                // maximum number of storable tokens exceeded
                break;
            }
            parametros[n++] = p;
        }

        // for (size_t i = 0; i != n; ++i)
        // {
        //      printf("Token %zu is '%s'.\n", i, parametros[i]);
        // }
        /* /EDIT */
        printf("El comando fué: '%s' \n", parametros[0]);

        if ((strcasecmp (parametros[0],"mkdisk ") == 0) || (strcasecmp (parametros[0],"mkdisk") == 0) )
        {
            MakeDisk(parametros,n);
        }else if ((strcasecmp (parametros[0],"rmdisk ") == 0) || (strcasecmp (parametros[0],"rmdisk") == 0) )
        {
            // RemoveDisk(parametros,n);
        }

        /* Free memory and exit. */
    }while(strcasecmp (comando,"exit") != 0 );
    free (comando);
    return 0;
}