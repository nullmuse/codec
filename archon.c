#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

type_ver *tv = calloc(1,sizeof(type_ver)); 
psy_data *psy = calloc(1,sizeof(psy_data)); 
short tver = 0; 

tver = pdata[OFF_VERT];
memcpy(tv,&tver,sizeof(type_ver)); 
psy->version = tv->t_ver; 
psy->type =  tv->t_type; 
psy->length = pdata[OFF_LEN];
psy->sequence = pdata[OFF_SEQ];
psy->source_id = htons(pdata[OFF_SID]) + ((htons(pdata[OFF_SID + 1]) >> 8) & 0xFF);
psy->dest_id = htons(pdata[OFF_DID]) + ((htons(pdata[OFF_DID + 1]) >> 8) & 0xFF);

free(tv); 
return psy; 


}

