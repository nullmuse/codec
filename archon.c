#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "zerglib.h"
#include <arpa/inet.h>



float float_ritual(int object) { 

asm(".intel_syntax noprefix");
asm("fld DWORD PTR [rbp-0x4]");
asm("fst DWORD PTR [rbp-0x32]"); 
asm("movd xmm0, DWORD PTR [rbp-0x32]");
asm("pop rbp");
asm("ret");
asm(".att_syntax noprefix");
return;
}

int byte_ritual(int object) {
asm(".intel_syntax noprefix");
asm("mov eax, DWORD PTR [rbp-0x4]");
asm("xchg ah, al");
asm("ror eax, 16");
asm("xchg ah, al");  
asm("pop rbp");
asm("ret");
asm(".att_syntax noprefix");
return;
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

void read_psy_status(psy_data *psy) { 

int hp,max_hp,ac,type,spp;
float sp;
char *name;
hp = psy->payload[STAT_HP];
ac = htons(psy->payload[STAT_AC]);
max_hp = psy->payload[STAT_HP_MAX];
type = psy->payload[STAT_TYPE];
memcpy(&spp,&psy->payload[STAT_SP],sizeof(int));
spp = byte_ritual(spp); 
sp = float_ritual(spp);
name = &psy->payload[STAT_NAME];
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
Name:%s\n\
HP:%i/%i\n\
Type:%s\n\
Armor:%i\n\
MaxSpeed:%fm/s\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id,name,hp,max_hp,zerg_breeds[type],ac,sp);



return; 

} 

void read_psy_command(psy_data *psy) { 

int command; 
float bearing;
int bearint;  
int meters; 
int group_id; 
int is_true; 
command = psy->payload[COMM_TYPE]; 

switch(command) { 

case GET_STATUS:
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
GET STATUS\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id); 
   break; 
case GOTO:
memcpy(&bearint,&psy->payload[COMM_PARAM2],sizeof(int)); 
bearint = byte_ritual(bearint); 
bearing = float_ritual(bearint); 
meters = psy->payload[COMM_PARAM1];
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
GOTO\n\
Orient to bearing %f\n\
Distance: %i meters\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id,bearing,meters);

   break; 
case GET_GPS:
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
GET GPS\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id);
   break; 
case RESERVED:
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
RESERVED\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id);
   break;
case RETURN:
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
RETURN\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id);
   break; 
case SET_GROUP:
   break; 
case STOP:
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
STOP\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id);
   break;
case REPEAT:
   break;
default:
   printf("Unrecognized command in command packet\n"); 
   break; 
}




return; 

}




void read_psy_gps(psy_data *psy) { return; } 




