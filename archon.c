#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include "zerglib.h"
#include "pcap.h"
#include "templar.h"
const float FATHOM_METERS = 1.8288;


static const char *zerg_breeds[] = {
"Overmind",
"Larva",
"Cerebrate",
"Overlord",
"Queen",
"Drone",
"Zergling",
"Lurker",
"Broodling",
"Hydralisk",
"Guardian",
"Scourge",
"Ultralisk",
"Mutalisk",
"Defiler",
"Devourer"
};




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
int repseq; 
float bearing;
int bearint;  
int meters; 
int group_id,is_add;  
char *add_remove[] = {"Remove from","Add to"};
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
memcpy(&meters,&psy->payload[COMM_PARAM1],sizeof(short));
meters = byte_ritual(meters); 
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
memcpy(&group_id,&psy->payload[COMM_PARAM2],sizeof(int)); 
is_add = psy->payload[COMM_PARAM1];
group_id = byte_ritual(group_id); 
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
SET GROUP\n\
%s Group ID:%i\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id,add_remove[is_add],group_id);

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
memcpy(&repseq,&psy->payload[COMM_PARAM2],sizeof(int));
repseq = byte_ritual(repseq); 
printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
REPEAT\n\
Repeat Sequence:%i\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id,repseq);
   break;
default:
   printf("Unrecognized command in command packet\n"); 
   break; 
}




return; 

}




void read_psy_gps(psy_data *psy) { 
char ns,we; 
double flat,flong;
float falt,fbear,fsp,fac;
unsigned int alt_i,bear_i,sp_i,ac_i; 
unsigned long lat_l, long_l; 
memcpy(&long_l,&psy->payload[GPS_LONG],sizeof(double)); 
memcpy(&lat_l,&psy->payload[GPS_LAT],sizeof(double));
memcpy(&alt_i,&psy->payload[GPS_ALT],sizeof(int));
memcpy(&bear_i,&psy->payload[GPS_BEAR],sizeof(int));
memcpy(&sp_i,&psy->payload[GPS_SP],sizeof(int));
memcpy(&ac_i,&psy->payload[GPS_ACC],sizeof(int));
lat_l =long_ritual(lat_l); 
long_l = long_ritual(long_l); 
alt_i = byte_ritual(alt_i);
bear_i = byte_ritual(bear_i);
sp_i = byte_ritual(sp_i);
ac_i = byte_ritual(ac_i);
flat = double_ritual(lat_l); 
flong = double_ritual(long_l);
falt = float_ritual(alt_i);
fbear = float_ritual(bear_i);
fsp = float_ritual(sp_i); 
fac = float_ritual(ac_i);  
falt = falt * FATHOM_METERS; 
if(flong < 0.0) { 
we = 'W';
flong *= -1; 
}
else if(flong >= 0.0) {
we = 'E'; 
}
if(flat < 0.0) { 
ns = 'S';
flat *= -1; 
}
else if(flat >= 0.0) { 
ns = 'N'; 
}

printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
GPS\n\
Latitude: %2.9f deg. %c\n\
Longitude: %2.9f deg. %c\n\
Altitude: %2.1fm\n\
Bearing: %2.9f deg.\n\
Speed: %.0fm/s\n\
Accuracy: %.0fm\n\
",psy->version,psy->sequence,psy->source_id,psy->dest_id,flat,ns,flong,we,falt,fbear,fsp,fac);



return; 

} 


int psionic_divagate(char *stream, int stream_size, psy_data **psy_list) {

int i,psy_count;
char *tok_ptr;


tok_ptr = calloc(HEADER_SIZE + 1,sizeof(char));
psy_count = 0;
for(i = 0; i < stream_size; ++i) {
memcpy(tok_ptr,(stream + i),sizeof(int));
if(!strncmp(tok_ptr,psychic_header,HEADER_SIZE)) {
if(psy_count == 0) {
psy_list[psy_count] = transmute_header((stream + i));
psy_count++;
}
else if(*psy_list != NULL) {
psy_list = realloc(psy_list,sizeof(psy_data *)); 
psy_list[psy_count] = transmute_header(stream + i);
psy_count++;
}
}
}
return psy_count;
}

