#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include "codelib.h" 



int main(int argc, char *argv[]) { 

char *pdata; 
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

printf("psy->version = %x\npsy->length = %x\n",(psy->version >> 12)& 0xFFFF,psy->length); 
printf("psy->source_id =%u\npsy->dest_id=%u\n",psy->source_id,psy->dest_id);
printf("psy->sequence = %x\n",psy->sequence); 




RETURN:
if(pdata)
free(pdata);
return 0; 

} 



