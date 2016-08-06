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

#define bool int
#define true 1
#define false 0