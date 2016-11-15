#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "zerglib.h"
#include <arpa/inet.h>



int decouple_psydata(int byte, int data) { 

int ret = 0; 
switch(data) { 

case 0:
ret = (byte >> 12)& 0xFFFF; 
break; 

} 
return ret; 


} 


psy_data *transmute_header(char *pdata) {

psy_data *psy = calloc(1,sizeof(psy_data)); 


psy->version = htons(pdata[OFF_VERT]); 
psy->length = pdata[OFF_LEN];
psy->sequence = pdata[OFF_SEQ];
psy->source_id = htons(pdata[OFF_SID]) + ((htons(pdata[OFF_SID + 1]) >> 8) & 0xFF);
psy->dest_id = htons(pdata[OFF_DID]) + ((htons(pdata[OFF_DID + 1]) >> 8) & 0xFF);

return psy; 


}

