int RemoveDisk(char * parametros[],size_t n0){
  printf("\nIniciando eliminación del disco...\n");

  char * path = "";
  bool flagPath=0;

  // if (n0!=2){
  //  printf("ERROR 0x02 0x00 0x00.\nError en la eliminación del disco, demasiados parametros (%lu).\n",n0);
  //  return 0;
  // }
  int i=1;
  char * argumento[50];
  size_t n = 0;
  for (char * p = strtok(parametros[1], "::"); p; p = strtok(NULL, "::"))
  {
    if (n >= 50)
    {
      // maximum number of storable tokens exceeded
      break;
    }
    argumento[n++] = p;
  }

  if ((strcasecmp (argumento[0],"-path ") == 0) || (strcasecmp (argumento[0],"-path") == 0) ){
    char lastChar = argumento[1][strlen (argumento[1])-1];
    path = strdup(argumento[1]);
    int j = i+1;
    while(lastChar!='"'){
      printf(">'%s' - '%s'\n",path,parametros[i+1]);
      char * tmp = strdup(parametros[j]);
      j++;
      strcat(path," ");
      printf(">'%s'\n",path);
      strcat(path,tmp);
      printf(">'%s'\n",path);
      lastChar = path[strlen(path)-1];
    }
    lastChar = path[strlen(path)-1];
    if (lastChar=='/')
    {
      path[strlen (path) - 1] = '\0';
    }
    removeChar(path,'"');
    flagPath=1;
  }

  if ( !(flagPath) ) {
    printf("ERROR 0x02 0x00 0x01.\nError en la eliminación del disco, al paracer falta un parametro obligatorio\n\n");
    return 0;
  }

  if(remove(path) == 0) {
    printf("Disco eliminado con éxito.\n");
  }
  else {
  printf("ERROR 0x02 0x00 0x02.\nError en la eliminación del disco, porfavor verifique la ubicación del disco. (%s)\n",path);
  }
  return 1;
}
/*

rmDisk –path=Disco2.dsk
RmDisK –path="Disco3.dsk"

*/
