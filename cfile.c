#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "pcap.h"


int get_file_size(char *fname) {
struct stat file_info;
int ret;
if(stat(fname,&file_info) == -1) {
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


char *read_file(char *fname, int is_pcap) { 

int fsize = 0; 
char *retdata = NULL;
FILE *fp = NULL; 

fsize = get_file_size(fname);
if(fsize <= 0) {
      perror("FATAL: is empty file");
      goto RETURN;
      }
if ((fp = fopen(fname, "rb")) == NULL) {
     perror("FATAL: cannot open file");
     goto RETURN;
}

if(is_pcap) { 
if(validate_file(fp) == OP_FAIL) {
perror("FATAL: Not a valid pcap file"); 
goto RETURN; 
} 
rewind(fp); 
} 
retdata = calloc(fsize + 1, sizeof(char)); 

fread(retdata,sizeof(char),fsize,fp); 


RETURN:
if(fp)
fclose(fp); 
return retdata; 
}

int write_pcap(char *pcap_data,int pcap_size,char *filename) {

int retdata = 0;
FILE *fp;
if (get_file_size(filename) == -1) { 

if ((fp = fopen(filename, "wb+")) == NULL) {
     perror("FATAL: cannot open file");
     goto RETURN;
}
}
else { 
if ((fp = fopen(filename, "ab+")) == NULL) {
     perror("FATAL: cannot open file");
     goto RETURN;
}
}

fwrite(pcap_data, sizeof(char), pcap_size,fp);
fclose(fp);


RETURN:
return retdata;

}

void init_pcap(char *filename) { 
if (get_file_size(filename)) {
remove(filename); 
}
}




