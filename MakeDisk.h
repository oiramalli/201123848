
int MakeDisk(char * parametros[],size_t n0){
    printf("\nIniciando creación del disco...\n");

    char unit = 'm';
    char * path = "";
    int size = 0;

    for (int i = 0; i < n0; ++i)
    {
    	char * argumento[50];
    	size_t n = 0;
	    for (char * p = strtok(parametros[i], "="); p; p = strtok(NULL, "="))
	    {
	        if (n >= 50)
	        {
	            // maximum number of storable tokens exceeded
	            break;
	        }
	        argumento[n++] = p;
	    }
	    for (size_t i = 0; i != n; ++i)
	    {
	        // printf("Token %zu is '%s'.\n", i, argumento[i]);
	    }

	    if ((strcasecmp (argumento[0],"unit ") == 0) || (strcasecmp (argumento[0],"unit") == 0) ){
			unit = argumento[1][0];
			// printf("Unidad del disco: %c \n",unit);
	    }

	    if ((strcasecmp (argumento[0],"size ") == 0) || (strcasecmp (argumento[0],"size") == 0) ){
			size= atoi(argumento[1]);
			// printf("Tamaño del disco: %d \n",size);
	    }

	    if ((strcasecmp (argumento[0],"path ") == 0) || (strcasecmp (argumento[0],"path") == 0) ){
			path = argumento[1];
			removeChar(path,'"');
			// printf("Ubicación del disco: %s \n",path);
	    }
		// printf("Fin del argumento\n\n");
    }
	printf("Creando el disco en: '%s' con una capacidad de %d %cB  \n",path,size,unit);
	
	int realsize = size;

	switch(unit){
		case 'k':
		case 'K':
			realsize = size * 1024;
		break;
		case 'm':
		case 'M':
			realsize = size * 1024 * 1024;
		break;

	}
	char garbage[realsize];
	for (int i = 0; i < sizeof(garbage); ++i)
	{
		garbage[i]='\0';
	}
	FILE *fp;
	fp = fopen(path, "w");
	fwrite(garbage, 1 , sizeof(garbage), fp);
	fclose(fp);

	printf("Real size: '%d'\n", realsize);

    return 1;
}
/*

Mkdisk -Size=3000 -unit=K -path=Disco1.dsk

Mkdisk -path=/home/user/Disco2.dsk -Unit=K -size=3000

mkdisk -size=5 -unit=M -path="/home/mis discos/Disco3.dsk"

Mkdisk -size=10 -path="/home/mis discos/Disco4.dsk"

*/