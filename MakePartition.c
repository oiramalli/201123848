int PartitionData(int partition_number, char part_status, char part_type, char part_fit, int part_start, int part_size, char part_name [16], char* path){
	FILE *fp;
	fp = fopen(path, "r+b");
	rewind(fp);

	int part_initialbyte=0;
	switch(partition_number){
		case 1:
			part_initialbyte = 13*4;
		break;
		case 2:
			part_initialbyte = 37*4;
		break;
		case 3:
			part_initialbyte = 61*4;
		break;
		case 4:
			part_initialbyte = 85*4;
		break;
	}
	int part_status_byte = part_initialbyte;
	int part_type_byte = part_initialbyte + 1;
	int part_fit_byte = part_initialbyte + 2;
	int part_start_byte = part_initialbyte + 3;
	int part_size_byte = part_initialbyte + 4;
	int part_name_byte = part_initialbyte + 8;

	fseek ( fp , part_status_byte , SEEK_SET);
	printf("Posicionandonos en %d...  %c \n", part_status_byte,part_status); //
	fwrite( &part_status, sizeof(part_status),1,fp);
	// fputc( part_status,fp );
	// rewind ( fp );

	fseek ( fp , part_type_byte , SEEK_SET);
	printf("Posicionandonos en %d...\n", part_type_byte); //
	fwrite( &part_type, sizeof(part_type),1,fp);
	// fputc( part_type,fp );
	// rewind ( fp );

	fseek ( fp , part_fit_byte , SEEK_SET);
	printf("Posicionandonos en %d...\n", part_fit_byte); //
	// fputc( part_fit,fp );
	fwrite( &part_fit, sizeof(part_fit),1,fp);
	// rewind ( fp );

	fseek ( fp , part_start_byte , SEEK_SET);
	printf("Posicionandonos en %d...\n", part_start_byte); //
	fwrite( &part_start, sizeof(part_start),1,fp);
	// fprintf(fp,"%d", part_start);
	// rewind ( fp );

	fseek ( fp , part_size_byte , SEEK_SET);
	printf("Posicionandonos en %d...\n", part_size_byte); //
	fwrite( &part_size, sizeof(part_size),1,fp);
	// fprintf(fp,"%d", part_size);
	// rewind ( fp );

	fseek ( fp , part_name_byte , SEEK_SET);
	printf("Posicionandonos en %d...\n", part_name_byte); //
	fputs( part_name,fp );
	// fwrite( &part_name, 16,1,fp);
	// fprintf(fp,"%s", part_name);
	//fwrite( &part_name, 16,1,fp);
	// rewind ( fp );
	fclose(fp);

	return 1;
}


/* Partition */ // 24
// char part_status; // 1 (13) (37) (61) (85)
// char part_type; // 1 (14) (38) (62) (86)
// char part_fit; // 1 (15) (39) (63) (87)
// int part_start; // 1 (16) (40) (64) (88)
// int part_size; // 4 (17-20) (41-44) (65-68) (89-92)
// char part_name [16]; // 16 (21-36) (45-60) (69-84) (93-108)

// int fseek ( FILE * stream, long int offset, int origin );
// size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
// ptr
// Pointer to the array of elements to be written, converted to a const void*.
// size
// Size in bytes of each element to be written.
// size_t is an unsigned integral type.
// count
// Number of elements, each one with a size of size bytes.
// size_t is an unsigned integral type.
// stream
// Pointer to a FILE object that specifies an output stream.
