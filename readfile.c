#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main ()
{
   FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;

   pFile = fopen ( "Disco1.dsk" , "rb" );
   if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {
      fputs ("Memory error",stderr); exit (2);
      printf("Error de memoria\n" );

   }

   // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {
      fputs ("Reading error",stderr); exit (3);
      printf("Error de lectura\n" );
   }

   /* the whole file is now loaded in the memory buffer. */
   printf("Tamaño Archivo: '%ld'\nContenido del archivo: '%s'\n",lSize,buffer );
   printf("Iniciando el otro método de lectura...\n");

   // terminate
   fclose (pFile);
   free (buffer);
   // return 0;
   FILE *fp;
   int c;

   fp = fopen("Disco1.dsk","r");
   while(1)
   {
      c = fgetc(fp);
      if( feof(fp) )
      {
         break;
      }
      printf("%c", c);
   }
   printf("\n");
   fclose(fp);


   printf("Iniciando el otro método de lectura...\n");

   FILE *f=NULL;
   int rec=0;
   mbr masterboot;
   ebr extendedboot;

   f=fopen("D1.dsk","r");

   fseek(f, 0, SEEK_SET);
   fread(&masterboot, sizeof(mbr), 1, f);

   printf("Size: '%dB', Timestamp: '%d', Disk Signature: '%d'\n", masterboot.mbr_tamaño, masterboot.mbr_fecha_creacion, masterboot.mbr_disk_signature);
   printf("Part1... part_status: '%c', part_type: '%c',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", masterboot.mbr_partition_1.part_status, masterboot.mbr_partition_1.part_type, masterboot.mbr_partition_1.part_fit, masterboot.mbr_partition_1.part_start, masterboot.mbr_partition_1.part_size, masterboot.mbr_partition_1.part_name);
   if ( masterboot.mbr_partition_1.part_type=='E' || masterboot.mbr_partition_1.part_type=='e' )
   {
      fseek(f, masterboot.mbr_partition_1.part_start, SEEK_SET);
      fread(&extendedboot, sizeof(ebr), 1, f);
      printf("EBR... part_status: '%c', part_next: '%d',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", extendedboot.part_status, extendedboot.part_next, extendedboot.part_fit, extendedboot.part_start, extendedboot.part_size, extendedboot.part_name);
   }
   printf("Part2... part_status: '%c', part_type: '%c',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", masterboot.mbr_partition_2.part_status, masterboot.mbr_partition_2.part_type, masterboot.mbr_partition_2.part_fit, masterboot.mbr_partition_2.part_start, masterboot.mbr_partition_2.part_size, masterboot.mbr_partition_2.part_name);
   if ( masterboot.mbr_partition_2.part_type=='E' || masterboot.mbr_partition_1.part_type=='e' )
   {
      fseek(f, masterboot.mbr_partition_2.part_start, SEEK_SET);
      fread(&extendedboot, sizeof(ebr), 1, f);
      printf("EBR... part_status: '%c', part_next: '%d',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", extendedboot.part_status, extendedboot.part_next, extendedboot.part_fit, extendedboot.part_start, extendedboot.part_size, extendedboot.part_name);
   }
   printf("Part3... part_status: '%c', part_type: '%c',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", masterboot.mbr_partition_3.part_status, masterboot.mbr_partition_3.part_type, masterboot.mbr_partition_3.part_fit, masterboot.mbr_partition_3.part_start, masterboot.mbr_partition_3.part_size, masterboot.mbr_partition_3.part_name);
   if ( masterboot.mbr_partition_3.part_type=='E' || masterboot.mbr_partition_1.part_type=='e' )
   {
      fseek(f, masterboot.mbr_partition_3.part_start, SEEK_SET);
      fread(&extendedboot, sizeof(ebr), 1, f);
      printf("EBR... part_status: '%c', part_next: '%d',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", extendedboot.part_status, extendedboot.part_next, extendedboot.part_fit, extendedboot.part_start, extendedboot.part_size, extendedboot.part_name);
   }
   printf("Part4... part_status: '%c', part_type: '%c',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", masterboot.mbr_partition_4.part_status, masterboot.mbr_partition_4.part_type, masterboot.mbr_partition_4.part_fit, masterboot.mbr_partition_4.part_start, masterboot.mbr_partition_4.part_size, masterboot.mbr_partition_4.part_name);
   if ( masterboot.mbr_partition_4.part_type=='E' || masterboot.mbr_partition_1.part_type=='e' )
   {
      fseek(f, masterboot.mbr_partition_4.part_start, SEEK_SET);
      fread(&extendedboot, sizeof(ebr), 1, f);
      printf("EBR... part_status: '%c', part_next: '%d',  part_fit: '%c', part_start: '%d', part_size: '%d', part_name: '%s'\n", extendedboot.part_status, extendedboot.part_next, extendedboot.part_fit, extendedboot.part_start, extendedboot.part_size, extendedboot.part_name);
   }
   fclose(f);

   return(0);
}
