#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stddef.h>
#include "Functions.h"
#include "MakeDisk.c"
#include "RemoveDisk.c"
#include "FormatDisk.c"

//        Mkdisk –Size::3000   +unit::K   –path::   "/home/user/"        
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
        if ((strlen(comando)>0) && (comando[strlen (comando) - 1] == '\n')){
            comando[strlen (comando) - 1] = '\0';
        }

        char lastChar=comando[strlen (comando)-1];
        
        remove_extra_spaces(comando);

        // printf("Last: '%c'\n",lastChar);

        while(lastChar=='\\'){
            comando[strlen (comando) - 1] = '\0';
            char * newLine = malloc (MAX_COMMAND_SZ);
            printf(">");
            scanf ("%[^\n]%*c", newLine);
            strcat(comando,newLine);
            // printf("\n El comando va: '%s'",comando);
            lastChar=comando[strlen (comando)-1];

        }

        /* EDIT */
        char * parametros[10];
        size_t n = 0;
        for (char * p = strtok(comando, " "); p; p = strtok(NULL, " "))
        {
            if (n >= 50)
            {
                // maximum number of storable tokens exceeded
                break;
            }
            parametros[n++] = p;
        }

        for (size_t i = 0; i != n; ++i)
        {
             printf("Token %zu is '%s'.\n", i, parametros[i]);
        }
        /* /EDIT */

        printf("El comando fué: '%s' \n", parametros[0]);

        if ( strcasecmp (parametros[0], "mkdisk") == 0 )
        {
            MakeDisk(parametros,n);
        }
        else if ((strcasecmp (parametros[0],"rmdisk ") == 0) || (strcasecmp (parametros[0],"rmdisk") == 0) )
        {
            RemoveDisk(parametros,n);
        }
        // else if ((strcasecmp (parametros[0],"fdisk ") == 0) || (strcasecmp (parametros[0],"fdisk") == 0) )
        // {
        //     FormatDisk(parametros,n);
        // }else if ((strcasecmp (parametros[0],"exit ") == 0) || (strcasecmp (parametros[0],"exit") == 0) )
        // {
        //     printf("Espero haber sido util ;)...\nHasta la próxima... \n");
        // }else
        // {
        //     printf("ERROR 0x00 0x00 0x00\nNo se reconoce el comando '%s' \n", parametros[0]);
        // }

        /* Free memory and exit. */
        free (comando);
        // comando = malloc (MAX_COMMAND_SZ);
    }while(strcasecmp (comando,"exit") != 0 );
    free (comando);
    return 0;
}
/*
Test:

Mkdisk –Size=3000 –unit=K –path=Disco1.dsk

   Mkdisk     –Size=3000     –unit=K    –path="Disco1.dsk"    

fdisk –Size=300 –path=Disco1.dsk –name=Particion1

fdisk –type=E –path=Disco1.dsk –Unit=K \

–name=Particion2 –size=300

fdisk –size=1 –type=L –unit=M –fit=BF \

–path="Disco1.dsk" –name="Particion2"

*/
