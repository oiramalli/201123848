#include <ctype.h>
#include <assert.h>
typedef struct EBR {
	char part_status;
	char part_fit;
	int part_start;
	int part_size;
	int part_next;
	char part_name [16];
} ebr;
typedef struct PARTITIONS {
	char part_status;
	char part_type;
	char part_fit;
	int part_start;
	int part_size;
	char part_name [16];
} partition;
typedef struct MBR {
	int mbr_tamaño;
	int mbr_fecha_creacion;
	int mbr_disk_signature;
	partition mbr_partition_1;
	partition mbr_partition_2;
	partition mbr_partition_3;
	partition mbr_partition_4;
} mbr;

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void stripLeadingAndTrailingSpaces(char* string){

     assert(string);

     /* First remove leading spaces */

     const char* firstNonSpace = string;

     while(*firstNonSpace != '\0' && isspace(*firstNonSpace))
     {
          ++firstNonSpace;
     }

     size_t len = strlen(firstNonSpace)+1;         

     memmove(string, firstNonSpace, len);

     /* Now remove trailing spaces */

     char* endOfString = string + len;

     while(string < endOfString  && isspace(*endOfString))
     {
          --endOfString ;
     }

     *endOfString = '\0';

}

void remove_extra_spaces(char* str) {
  int i,x;
  for(i=x=1; str[i]; ++i)
    if(!isspace(str[i]) || (i>0 && !isspace(str[i-1])))
      str[x++] = str[i];
  str[x] = '\0';
  stripLeadingAndTrailingSpaces(str);
}

#define bool int
#define true 1
#define false 0