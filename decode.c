#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include "codelib.h" 



int main(int argc, char *argv[]) { 

char *pdata; 
int i,size; 
psy_data *psy; 

if(argc == 1) {
printf("Usage: ./%s psy.pcap",argv[0]);
goto RETURN;
}
pdata = read_pcap(argv[1]); 

if (pdata == NULL) {
     goto RETURN;
}


psy = transmute_header(pdata); 

printf("psy->version = %i\npsy->length = %i\n",psy->version,psy->length); 
printf("psy->source_id = %i\npsy->dest_id = %i\n",psy->source_id,psy->dest_id);





RETURN:
if(pdata)
free(pdata);
return 0; 

} 



