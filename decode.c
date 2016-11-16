#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include "codelib.h" 



int main(int argc, char *argv[]) { 

char *pdata = NULL; 
psy_data *psy; 
if(argc <= 1) {
printf("Usage: %s psy.pcap\n",argv[0]);
goto RETURN;
}
pdata = read_pcap(argv[1]); 

if (pdata == NULL) {
     goto RETURN;
}


psy = transmute_header(pdata); 

printf("psy version: %i\n",psy->version); 
printf("psy type: %i\n",psy->type);
printf("psy length: %i\n",psy->length);
printf("psy source ID: %i\n",psy->source_id);
printf("psy destination ID: %i\n",psy->dest_id);
printf("psy sequence number: %i\n",psy->sequence);



RETURN:
if(pdata != NULL)
free(pdata);
return 0; 

} 



