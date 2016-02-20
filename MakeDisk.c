#include "MakePartition.c"

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
	mbr masterboot;
	masterboot.mbr_tamaño = realsize;
	masterboot.mbr_fecha_creacion=(int)time(NULL);

	masterboot.mbr_disk_signature = rand();
	char garbage[realsize];
	FILE *fp;
	fp = fopen(path, "w+b");
	char part_status ='1';
	char part_type ='p';
	char part_fit ='c';
	int part_start =162;
	int part_size = 10;
	char part_name [16] = "Mario";
	
	fwrite(&masterboot, 1 , sizeof(masterboot), fp);
	
	for (int i = 0; i < sizeof(garbage); ++i)
	{
		garbage[i]='\0';
	}
	fwrite(garbage, 1 , sizeof(garbage) - sizeof(masterboot), fp);
	fclose(fp);

	PartitionData(1, part_status, part_type, part_fit, part_start, part_size, part_name, path);
	PartitionData(2, part_status, part_type, part_fit, part_start, part_size, part_name, path);
	PartitionData(3, part_status, part_type, part_fit, part_start, part_size, part_name, path);
	PartitionData(4, part_status, part_type, part_fit, part_start, part_size, part_name, path);

	// printf("MBR Size: '%lu'\n", sizeof(masterboot)); //120
	printf("Size: '%d bytes'\n", masterboot.mbr_tamaño); //
	// printf("Size of Size: '%lu'\n", sizeof(masterboot.mbr_tamaño)); //
	printf("Time: '%d' (unix timestamp)\n", masterboot.mbr_fecha_creacion); //
	// printf("Size of Time: '%lu'\n", sizeof(masterboot.mbr_fecha_creacion)); //
	printf("Disk Signature: '%d'\n", masterboot.mbr_disk_signature);
	printf("Real size: '%d bytes'\n", realsize); //


    return 1;
}
/*

Mkdisk -Size=3000 -unit=K -path=Disco1.dsk

Mkdisk -path=/home/user/Disco2.dsk -Unit=K -size=3000

mkdisk -size=5 -unit=M -path="/home/mis discos/Disco3.dsk"

Mkdisk -size=10 -path="/home/mis discos/Disco4.dsk"

*/
/* MBR */ // 120?
// int mbr_tamaño; // 4
// time_t mbr_fecha_creacion; // 4 (5-8)
// int mbr_disk_signature; // 4 (9-12)
// partition mbr_partition_1; // 24 (13-36)
// partition mbr_partition_2; // 24 (37-60)
// partition mbr_partition_3; // 24 (61-84)
// partition mbr_partition_4; // 24 (85-108)
/* Partition */ // 24
// char part_status; // 1 (13) (37) (61) (85)
// char part_type; // 1 (14) (38) (62) (86)
// char part_fit; // 1 (15) (39) (63) (87)
// char part_start; // 1 (16) (40) (64) (88)
// int part_size; // 4 (17-20) (41-44) (65-68) (89-92)
// char part_name [16]; // 16 (21-36) (45-60) (69-84) (93-108)