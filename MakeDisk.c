#include "MakePartition.c"

int MakeDisk(char * parametros[],size_t n0){
  printf("\nIniciando creación del disco...\n");

 //    if ( n0 > 4 ){
  //  printf("ERROR 0x01 0x0F 0x0F.\nError en la creación del disco, demasiados parametros (%lu).\n",n0);
  //  return 0;
  // }

  char unit = 'M';
  char * path = "";
  char * name = "";
  char * fullPath = "";
  int size = 0;

  bool flagSize=0;
  bool flagPath=0;

  for (int i = 0; i < n0; ++i)
  {
    char * argumento[50];
    size_t n = 0;
    for (char * p = strtok(parametros[i], "::"); p; p = strtok(NULL, "::"))
    {
      if (n >= 50)
      {
        // maximum number of storable tokens exceeded
        break;
      }
      argumento[n++] = p;
    }
    // for (size_t i = 0; i != n; ++i)
    // {
    //     printf("Token %zu is '%s'.\n", i, argumento[i]);
    // }

    if ((strcasecmp (argumento[0],"+unit ") == 0) || (strcasecmp (argumento[0],"+unit") == 0) ){
      unit = argumento[1][0];
      // printf("Unidad del disco: %c \n",unit);
    }

    if ((strcasecmp (argumento[0],"-size ") == 0) || (strcasecmp (argumento[0],"-size") == 0) ){
      size= atoi(argumento[1]);
      if (size < 1)
      {
        printf("ERROR 0x01 0x0F 0x02.\nError en la creación del disco, El tamaño debe ser positivo y mayor que cero (%d).\n",size);
        return 0;
      }
      flagSize=1;
      // printf("Tamaño del disco: %d \n",size);
    }

    if ((strcasecmp (argumento[0],"-path ") == 0) || (strcasecmp (argumento[0],"-path") == 0) ){
      char lastChar = argumento[1][strlen (argumento[1])-1];
      path = strdup(argumento[1]);
      while(lastChar!='"'){
        printf("\n>'%s' - '%s'",path,parametros[i+1]);
        char * tmp = strdup(parametros[i+1]);
        strcat(path," ");
        printf("\n>'%s'",path);
        strcat(path,tmp);
        printf("\n>'%s'",path);
        lastChar = path[strlen(path)-1];
      }
      removeChar(path,'"');
      flagPath=1;
      // printf("Ubicación del disco: %s \n",path);
    }

    if ((strcasecmp (argumento[0],"-name ") == 0) || (strcasecmp (argumento[0],"-name") == 0) ){
      name = argumento[1];
      removeChar(name,'"');
      flagPath=1;
      // printf("Ubicación del disco: %s \n",path);
    }
    // printf("Fin del argumento\n\n");
  }
  if ( !(flagPath && flagSize) ) {
    printf("ERROR 0x01 0x0%d 0x0%d.\nError en la creación del disco, al paracer falta un parametro obligatorio\n\n",flagPath,flagSize);
    return 0;
  }
  
  int realsize = size;

  switch(unit){
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
    default:
      printf("ERROR 0x01 0x0F 0x01.\nError en la creación del disco, No se reconoce la unidad (%c).\n",unit);
      return 0;
  }
  mbr masterboot;
  masterboot.mbr_tamaño = realsize;
  masterboot.mbr_fecha_creacion=(int)time(NULL);
  masterboot.mbr_disk_signature = rand();
  masterboot.mbr_partition_1.part_status = 'N';
  masterboot.mbr_partition_2.part_status = 'N';
  masterboot.mbr_partition_3.part_status = 'N';
  masterboot.mbr_partition_4.part_status = 'N';

  char garbage[realsize];
  FILE *fp;

  struct stat st = {0};
  printf("Verificando si existe el directorio...\n");
  if (stat(path, &st) == -1) {
    printf("Creando el directorio...\n");
    if( mkdir(path, 0700) != 0 ){
      printf("ERROR 0x01 0x0F 0x04.\nError desconocido en la creación del directorio.\n");
      return 0;
    }
    //sprintf("Path creado\n");
  }
  fullPath = strcat(path,"/");
  fullPath = strcat(path,name);
  printf("\nCreando el disco en: '%s' con una capacidad de %d %cB  \n",fullPath,size,unit);
  fp = fopen(fullPath, "w+b");
  // char part_status ='1';
  // char part_type ='p';
  // char part_fit ='c';
  // int part_start =0;
  // int part_size = 0;
  // char part_name [16] = "MarioA.";
  for (int i = 0; i < sizeof(garbage); ++i)
  {
    fputc('\0',fp);
  }
  
  rewind(fp);
  fwrite(&masterboot, sizeof(mbr), 1,fp);
  //fwrite (buffer , sizeof(char), sizeof(buffer), pFile);

  // fprintf(fp,"%d",masterboot.mbr_tamaño);
  // fprintf(fp,"%d",masterboot.mbr_fecha_creacion);
  // fprintf(fp,"%d",masterboot.mbr_disk_signature);
  
  fclose(fp);

  printf("MBR Size: '%lu'\n", sizeof(masterboot)); //120
  printf("Size: '%d bytes'\n", masterboot.mbr_tamaño); //
  // printf("Size of Size: '%lu'\n", sizeof(masterboot.mbr_tamaño)); //
  printf("Time: '%d' (unix timestamp)\n", masterboot.mbr_fecha_creacion); //
  // printf("Size of Time: '%lu'\n", sizeof(masterboot.mbr_fecha_creacion)); //
  printf("Disk Signature: '%d'\n", masterboot.mbr_disk_signature);
  printf("Real size: '%d bytes'\n", realsize); //



  return 1;
}
/*

Mkdisk –Size=3000 –unit=K –path=Disco1.dsk

Mkdisk –Size=3000 –unit=K –path=/Users/marioalvarado/Documents/USAC/MIA/Progra/ejemplo03.02.16/Prueba/Disco1.dsk

Mkdisk –path=/home/user/Disco2.dsk –Unit=K –size=3000

mkdisk –size=5 –unit=M –path="/home/mis discos/Disco3.dsk"

Mkdisk –size=10 –path="/home/mis discos/Disco4.dsk"

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