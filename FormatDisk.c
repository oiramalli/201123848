//#include "MakePartition.c"
#include <stddef.h>
int FormatDisk(char * parametros[],size_t n0){
    printf("\nIniciando el formato del disco...\n");

    if (n0>7){
		printf("ERROR 0x02 0x0F 0x0F.\nError en el formateo del disco, demasiados parametros (%lu).\n",n0);
		return 0;
	}

    int size = 0;// Obligatorio al crear
    char unit = 'K';
    char * path = "";
    char type = 'P';
    char fit = 'W';
    char delete[4];
    char name[16];

    bool flagSize=0;// Obligatorio al crear
	bool flagPath=0;
	bool flagName=0;

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
			flagSize=1;
			// printf("Tamaño del disco: %d \n",size);
	    }

	    if ((strcasecmp (argumento[0],"path ") == 0) || (strcasecmp (argumento[0],"path") == 0) ){
			path = argumento[1];
			removeChar(path,'"');
			removeChar(path,' ');
			flagPath=1;
			printf("Ubicación del disco: '%s' \n",path);
	    }

	    if ((strcasecmp (argumento[0],"type ") == 0) || (strcasecmp (argumento[0],"type") == 0) ){
			type = argumento[1][0];
	    }

	    if ((strcasecmp (argumento[0],"fit ") == 0) || (strcasecmp (argumento[0],"fit") == 0) ){
			if ( (strcasecmp (argumento[1],"WF") ) )
			{
				fit='W';
			}else if ( (strcasecmp (argumento[1],"FF") ) )
			{
				fit='F';
			}else if ( (strcasecmp (argumento[1],"BF") ) )
			{
				fit='B';
			}else{
				printf("ERROR, No se reconoce el \"fit\".\n");
			}
	    }

	    if ((strcasecmp (argumento[0],"delete ") == 0) || (strcasecmp (argumento[0],"delete") == 0) ){
			for (int j = 0; j < sizeof(fit); ++j)
			{
				delete[j] = argumento[1][j];
			}
	    }

	    if ((strcasecmp (argumento[0],"name ") == 0) || (strcasecmp (argumento[0],"name") == 0) ){
			for (int j = 0; j < sizeof(name); ++j)
			{
				name[j] = argumento[1][j];
			}
			removeChar(name,' ');
			removeChar(name,'"');
			flagPath=1;
	    }
		// printf("Fin del argumento\n\n");
    }
    if ( !(flagPath && flagSize) ) {
		printf("ERROR 0x02 0x0%d 0x0%d.\nError en el formateo del disco, al paracer falta un parametro obligatorio\n\n",flagPath,flagSize);
    	return 0;
    }

	int realsize = size;

	switch(unit){
		case 'b':
			unit='B';
		case 'B':
			realsize = size;
		break;
		case 'k':
			unit='K';
		case 'K':
			realsize = size * 1024;
		break;
		case 'm':
			unit='M';
		case 'M':
			realsize = size * 1024 * 1024;
		break;

	}
	// Read Disk Info...
	FILE *f;
	mbr masterboot;
	printf("Abriendo el disco.. \n");
	f = fopen(path,"r+b");
	if (f==NULL)
	{
		printf("Error abriendo el disco.. '%s'\n",path);
		return 0;
	}
	printf("Seek set.. \n");
	fseek(f, 0, SEEK_SET);
	printf("Leyendo MBR.. \n");
	fread(&masterboot, sizeof(mbr), 1, f);

	if (type=='P' || type=='p'){
		if (masterboot.mbr_partition_1.part_status=='N')
		{
			masterboot.mbr_partition_1.part_status = '1';
			masterboot.mbr_partition_1.part_type = type;
			masterboot.mbr_partition_1.part_fit = fit;
			masterboot.mbr_partition_1.part_start = sizeof(masterboot)+1;
			masterboot.mbr_partition_1.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_1.part_name[i]= name[i];
			}
			rewind(f);
			printf("Escribiendo MBR (Part 1) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fclose(f);
			printf("Escribiendo MBR (Part 1) \n");
		}else if (masterboot.mbr_partition_2.part_status=='N')
		{
			masterboot.mbr_partition_2.part_status = '1';
			masterboot.mbr_partition_2.part_type = type;
			masterboot.mbr_partition_2.part_fit = fit;
			masterboot.mbr_partition_2.part_start = masterboot.mbr_partition_1.part_start + masterboot.mbr_partition_1.part_size + 1;
			masterboot.mbr_partition_2.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_2.part_name[i]= name[i];
			}
			rewind(f);
			printf("Escribiendo MBR (Part 2) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fclose(f);
		}else if (masterboot.mbr_partition_3.part_status=='N')
		{
			masterboot.mbr_partition_3.part_status = '1';
			masterboot.mbr_partition_3.part_type = type;
			masterboot.mbr_partition_3.part_fit = fit;
			masterboot.mbr_partition_3.part_start = masterboot.mbr_partition_2.part_start + masterboot.mbr_partition_2.part_size + 1;
			masterboot.mbr_partition_3.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_3.part_name[i]= name[i];
			}
			rewind(f);
			printf("Escribiendo MBR (Part 3) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fclose(f);
		}else if (masterboot.mbr_partition_4.part_status=='N')
		{
			masterboot.mbr_partition_4.part_status = '1';
			masterboot.mbr_partition_4.part_type = type;
			masterboot.mbr_partition_4.part_fit = fit;
			masterboot.mbr_partition_4.part_start = masterboot.mbr_partition_3.part_start + masterboot.mbr_partition_3.part_size + 1;
			masterboot.mbr_partition_4.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_4.part_name[i]= name[i];
			}
			rewind(f);
			printf("Escribiendo MBR (Part 4) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fclose(f);
		}else{
			printf("ERROR, Ya no se puede crear mas particiones.\n");
			return 0;
		}
	}else if (type=='E' || type=='e'){
		printf("Intentado crear partición extendida: '%s'\n",name);

		int extendidas=0;
		if ( (masterboot.mbr_partition_1.part_type=='e') || (masterboot.mbr_partition_1.part_type=='E') )
		{
			extendidas=extendidas+1;
		}if ( (masterboot.mbr_partition_2.part_type=='e') || (masterboot.mbr_partition_2.part_type=='E') )
		{
			extendidas=extendidas+1;
		}
		if ( (masterboot.mbr_partition_3.part_type=='e') || (masterboot.mbr_partition_3.part_type=='E') )
		{
			extendidas=extendidas+1;
		}
		if ( (masterboot.mbr_partition_4.part_type=='e') || (masterboot.mbr_partition_4.part_type=='E') )
		{
			extendidas=extendidas+1;
		}

		if (extendidas>1)
		{
			printf("ERROR. Demasiadas particiones extendidas\n");
			return 0;
		}

		if (masterboot.mbr_partition_1.part_status=='N')
		{
			masterboot.mbr_partition_1.part_status = '1';
			masterboot.mbr_partition_1.part_type = type;
			masterboot.mbr_partition_1.part_fit = fit;
			masterboot.mbr_partition_1.part_start = sizeof(masterboot)+1;
			masterboot.mbr_partition_1.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_1.part_name[i]= name[i];
			}
			rewind(f);
			// printf("Escribiendo MBR (Part 1) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fseek(f, masterboot.mbr_partition_1.part_start, SEEK_SET);
			ebr extendedboot;
			// char part_status;
			// char part_fit;
			// int part_start;
			// int part_size;
			// int part_next;
			// char part_name [16];
			extendedboot.part_status='N';
			extendedboot.part_next=-1;
			fwrite(&extendedboot, sizeof(ebr), 1,f);

			fclose(f);
			// printf("Escribiendo MBR (Part 1) \n");
		}else if (masterboot.mbr_partition_2.part_status=='N')
		{
			masterboot.mbr_partition_2.part_status = '1';
			masterboot.mbr_partition_2.part_type = type;
			masterboot.mbr_partition_2.part_fit = fit;
			masterboot.mbr_partition_2.part_start = masterboot.mbr_partition_1.part_start + masterboot.mbr_partition_1.part_size + 1;
			masterboot.mbr_partition_2.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_2.part_name[i]= name[i];
			}
			rewind(f);
			// printf("Escribiendo MBR (Part 2) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fseek(f, masterboot.mbr_partition_2.part_start, SEEK_SET);
			ebr extendedboot;
			extendedboot.part_status='N';
			extendedboot.part_next=-1;
			fwrite(&extendedboot, sizeof(ebr), 1,f);
			fclose(f);
		}else if (masterboot.mbr_partition_3.part_status=='N')
		{
			masterboot.mbr_partition_3.part_status = '1';
			masterboot.mbr_partition_3.part_type = type;
			masterboot.mbr_partition_3.part_fit = fit;
			masterboot.mbr_partition_3.part_start = masterboot.mbr_partition_2.part_start + masterboot.mbr_partition_2.part_size + 1;
			masterboot.mbr_partition_3.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_3.part_name[i]= name[i];
			}
			rewind(f);
			// printf("Escribiendo MBR (Part 3) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fseek(f, masterboot.mbr_partition_3.part_start, SEEK_SET);
			ebr extendedboot;
			extendedboot.part_status='N';
			extendedboot.part_next=-1;
			fwrite(&extendedboot, sizeof(ebr), 1,f);
			fclose(f);
		}else if (masterboot.mbr_partition_4.part_status=='N')
		{
			masterboot.mbr_partition_4.part_status = '1';
			masterboot.mbr_partition_4.part_type = type;
			masterboot.mbr_partition_4.part_fit = fit;
			masterboot.mbr_partition_4.part_start = masterboot.mbr_partition_3.part_start + masterboot.mbr_partition_3.part_size + 1;
			masterboot.mbr_partition_4.part_size= realsize;
			for (int i = 0; i < sizeof(name); ++i)
			{
				masterboot.mbr_partition_4.part_name[i]= name[i];
			}
			rewind(f);
			// printf("Escribiendo MBR (Part 4) \n");
			fwrite(&masterboot, sizeof(mbr), 1,f);
			fseek(f, masterboot.mbr_partition_4.part_start, SEEK_SET);
			ebr extendedboot;
			extendedboot.part_status='N';
			extendedboot.part_next=-1;
			fwrite(&extendedboot, sizeof(ebr), 1,f);
			fclose(f);
		}else{
			printf("ERROR, Ya no se puede crear mas particiones.\n");
			return 0;
		}
	}else if (type=='L' || type=='l'){
		int extendida=0;
		if ( (masterboot.mbr_partition_1.part_type=='e') || (masterboot.mbr_partition_1.part_type=='E') )
		{
			extendida = masterboot.mbr_partition_1.part_start;
			if (size > masterboot.mbr_partition_1.part_size - sizeof(ebr) )
			{
				printf("Error, no hay suficiente espacio\n");
				return 0;
			}
		}else if ( (masterboot.mbr_partition_2.part_type=='e') || (masterboot.mbr_partition_2.part_type=='E') )
		{
			extendida = masterboot.mbr_partition_2.part_start;
			if (size > masterboot.mbr_partition_2.part_size - sizeof(ebr) )
			{
				printf("Error, no hay suficiente espacio\n");
				return 0;
			}
		}else if ( (masterboot.mbr_partition_3.part_type=='e') || (masterboot.mbr_partition_3.part_type=='E') )
		{
			extendida = masterboot.mbr_partition_3.part_start;
			if (size > masterboot.mbr_partition_3.part_size - sizeof(ebr) )
			{
				printf("Error, no hay suficiente espacio\n");
				return 0;
			}
		}else if ( (masterboot.mbr_partition_4.part_type=='e') || (masterboot.mbr_partition_4.part_type=='E') )
		{
			extendida = masterboot.mbr_partition_4.part_start;
			if (size > masterboot.mbr_partition_4.part_size - sizeof(ebr) )
			{
				printf("Error, no hay suficiente espacio\n");
				return 0;
			}
		}
		ebr extendedboot;
		printf("Vamos bien...\n");
		fseek(f, extendida, SEEK_SET);
      	fread(&extendedboot, sizeof(ebr), 1, f);
		printf("EBR: part_status: '%c',part_fit: '%c',part_start: '%d',part_size: '%d',part_next: '%d'  \n",extendedboot.part_status,extendedboot.part_fit,extendedboot.part_start,extendedboot.part_size,extendedboot.part_next);
      	if(extendedboot.part_status=='N'){
      		extendedboot.part_status = '1';
			extendedboot.part_fit = fit;
			extendedboot.part_start = extendida + sizeof(ebr) + 1;
			extendedboot.part_size = size;
			extendedboot.part_next = -1;
			for (int i = 0; i < sizeof(name); ++i)
			{
				extendedboot.part_name[i]= name[i];
			}
			printf("EBR: part_status: '%c',part_fit: '%c',part_start: '%d',part_size: '%d',part_next: '%d'  \n",extendedboot.part_status,extendedboot.part_fit,extendedboot.part_start,extendedboot.part_size,extendedboot.part_next);
			fseek(f, extendida, SEEK_SET);
			fwrite(&extendedboot, sizeof(ebr), 1,f);
			fclose(f);
      	}else if(extendedboot.part_status=='1'){

			extendedboot.part_next = extendedboot.part_start + size + 1;
			ebr extendedboot_new;
      		extendedboot.part_status = '1';
			extendedboot.part_fit = fit;
			extendedboot.part_start = extendedboot.part_next + sizeof(ebr) + 1;
			extendedboot.part_size = size;
			extendedboot.part_next = -1;
			for (int i = 0; i < sizeof(name); ++i)
			{
				extendedboot.part_name[i]= name[i];
			}
			fseek(f, extendedboot.part_next, SEEK_SET);
			fwrite(&extendedboot_new, sizeof(ebr), 1,f);
			fclose(f);
      	}


	}

    return 1;
}
/*
#Crea una partición primaria llamada Particion1 de 300 kb
#con el peor ajuste en el disco Disco1.dsk
fdisk –Size=300 –path=Disco1.dsk –name=Particion1

#Crea una partición extendida dentro de Disco2 de 300 kb
#Tiene el peor ajuste
fdisk –type=E –path=Disco1.dsk –Unit=K \
–name=Particion1 –size=300

#Crea una partición lógica con el mejor ajuste, llamada Paricion3,
#de 1 Mb en el Disco3
fdisk –size=1 –type=L –unit=M –fit=BF \
–path="/mis discos/Disco3.dsk" –name="Particion3"

#Intenta crear una partición extendida dentro de Disco2 de 200 kb
#Debería mostrar error ya que ya existe una partición extendida
#dentro de Disco2
fdisk –type=E –path=/home/Disco2.dsk –name=Part3 –Unit=K –size=200

#Elimina de forma rápida una partición llamada Particion1
fdisk –delete=fast –name="Particion1" –path=/home/Disco1.dsk

#Elimina de forma completa una partición llamada Particion1
fdisk –name=Particion1 –delete=full –path=/home/Disco1.dsk

#Quitan 500 Kb de Particion4 en Disco4.dsk
#Ignora los demás parametros (size, delete)
#Se toma como válido el primero que aparezca, en este caso add
fdisk –add=–500 –size=10 –unit=K \
–path="/home/mis discos/Disco4.dsk" –name="Particion4"

#Agrega 1 Mb a la partición Particion4 del Disco4.dsk
#Se debe validar que haya espacio libre después de la partición
fdisk –add=1 –unit=M –path="/home/mis discos/Disco4.dsk" \
–name="Particion 4"

*/
// char part_status;
// char part_type;
// char part_fit;
// int part_start;
// int part_size;
// char part_name [16];
