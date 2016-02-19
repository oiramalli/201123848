// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// /* Maximum comando size + 1. */
// #define MAX_COMMAND_SZ 256

// int main(int argC, char *argV[]) {
//     char *comando;
//     do{
//         /* Allocate memory and check if okay. */
//         comando = malloc (MAX_COMMAND_SZ);
//         if (comando == NULL) {
//             printf ("No memory\n");
//             return 1;
//         }

//         /* Ask user for comando. */
//         printf("What is your command?>");

//         /* Get the comando, with size limit. */
//         fgets (comando, MAX_COMMAND_SZ, stdin);

//         /* Remove trailing newline, if there. */
//         if ((strlen(comando)>0) && (comando[strlen (comando) - 1] == '\n'))
//             comando[strlen (comando) - 1] = '\0';
//         if (strcasecmp (comando,"exit") != 0 ){
//             return 0;
//         }

//         char * params, * params2;
//         params = strtok (comando,"-");
//         params = strtok (NULL, "-");
//         char* strArray[10];
//         int i=0;
//         while (params != NULL)
//         {
//             strArray[i] = malloc(strlen(params) + 1);
//             strcpy(strArray[i], params);
//             printf ("Algo: %s\n",params);
//             /*char * args;
//             args = strdup(params);
//             params2 = strtok (params,"-");
//             args = strtok (args,"=");
//             while (args != NULL)
//             {
//                 printf ("args: %s\n",args);
//                 args = strtok (NULL, "=");
//             }*/
//             i++;
//             params = strtok (NULL, "-");
//             //strdup(variable)
//         }//afda -asffgh=6654 -kjhg=764 -nvbbc=8548

//         /* Say hello. */
//         printf("El comando fué: %s \n", comando);

//         i=0;

//         for (i=0;i < 3;i++) {
//             printf("Array en %d: %s\n",i,strArray[i]);

//         }

//         /* Free memory and exit. */
//     }while(strcasecmp (comando,"exit") != 0 );
//     free (comando);
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        char * tokens[50];
        size_t n = 0;
        for (char * p = strtok(comando, "-"); p; p = strtok(NULL, "-"))
        {
            if (n >= 50)
            {
                // maximum number of storable tokens exceeded
                break;
            }
            tokens[n++] = p;
        }

        for (size_t i = 0; i != n; ++i)
        {
             printf("Token %zu is '%s'.\n", i, tokens[i]);
        }
        /* /EDIT */
        char * params;
        params = strtok (comando,"-");
        params = strtok (NULL, "-");
        // printf("Antes del if: %s \n", comando);

        if( strcasecmp (comando,"exit") == 0 ){
            // printf("dentro del if: %s \n", comando);
            return 0;
        }

        while (params != NULL)
        {
            printf ("Algo: %s\n",params);
            
            params = strtok (NULL, "-");
        }

        /* Say hello. */
        printf("El comando fué: %s \n", comando);

        /* Free memory and exit. */
    }while(strcasecmp (comando,"exit") != 0 );
    free (comando);
    return 0;
}