#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "zerglib.h"
#include <arpa/inet.h>



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
psy->payload = &pdata[OFF_PAY];
free(tv); 
return psy; 


}

void read_psy_message(psy_data *psy) { 

int message_size; 
char *message; 
message_size = (psy->length) - PSYHDR_SZ; 
message = calloc(message_size + 1,sizeof(char)); 
memcpy(message,psy->payload,message_size); 
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
Message: %s\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id,message);

free(message);
return;
}

void read_psy_status(psy_data *psy) { return; } 

void read_psy_command(psy_data *psy) { return; }

void read_psy_gps(psy_data *psy) { return; } 




