#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "pcap.h"


int get_file_size(char *fname) {
struct stat file_info;
int ret;
if(stat(fname,&file_info) == -1) {
       perror("FATAL: file does not exist");
       return -1;
}
ret = file_info.st_size;
return ret;

}



int validate_file(FILE *fp) {

int ret = OP_FAIL; 
char *pcheck; 


pcheck = calloc(HEADER_SIZE + 1,sizeof(char)); 

fread(pcheck,sizeof(char),HEADER_SIZE,fp);

if(!strncmp(pcheck,psychic_header,HEADER_SIZE)) { 
ret = OP_SUCCESS; 
}

free(pcheck); 
return ret; 

} 


char *read_pcap(char *fname) { 

int fsize = 0; 
char *retdata = NULL;
FILE *fp = NULL; 

fsize = get_file_size(fname);
if(fsize <= 0) {
      perror("FATAL: capture is empty file");
      goto RETURN;
   }

if ((fp = fopen(fname, "rb")) == NULL) {
     perror("FATAL: cannot open file");
     goto RETURN;
}

if(validate_file(fp) == OP_FAIL) {
perror("FATAL: Not a valid pcap file"); 
goto RETURN; 
} 
rewind(fp); 

retdata = calloc(fsize + 1, sizeof(char)); 

fread(retdata,sizeof(char),fsize,fp); 


RETURN:
if(fp)
fclose(fp); 
return retdata; 
}



