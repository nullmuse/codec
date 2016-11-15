#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include "codelib.h" 



int main(int argc, char *argv[]) { 

char *pdata; 
int i,size; 


if(argc == 1) {
printf("Usage: ./%s psy.pcap",argv[0]);
goto RETURN;
}
pdata = read_pcap(argv[1]); 

if (pdata == NULL) {
     goto RETURN;
}


size = get_file_size(argv[1]); 

for(i=0;i<size;++i)
printf("%X at location %i: looks like %c\n",pdata[i], i, pdata[i]); 


RETURN:
if(pdata)
free(pdata);
return 0; 

} 



