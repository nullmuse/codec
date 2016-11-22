#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include "zerglib.h"
#include "pcap.h"
#include "templar.h"


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


zerg_header *transmute_input(char *pdata) {


char *strs[] = {"Version:","Sequence","From","To"};

char *ptok;
char *data_point; 
char **saveptr = &pdata; 
zerg_header *z_header = calloc(1,sizeof(zerg_header));;
ptok = strtok_r(pdata,"\n",saveptr);
if(!strncmp(ptok,strs[0],strlen(strs[0]))) { 
data_point = strstr(ptok," "); 
if(data_point != NULL) { 
z_header->version = transmute_char(data_point + 1); 
}
}

ptok = strtok_r(NULL,"\n",saveptr);
if(!strncmp(ptok,strs[1],strlen(strs[1]))) {
data_point = strstr(ptok," ");
if(data_point != NULL) {
z_header->sequence = transmute_char(data_point + 1);
}
}

ptok = strtok_r(NULL,"\n",saveptr);
if(!strncmp(ptok,strs[2],strlen(strs[2]))) {
data_point = strstr(ptok," ");
if(data_point != NULL) {
z_header->source_id = transmute_char(data_point + 1);
}
}

ptok = strtok_r(NULL,"\n",saveptr);
if(!strncmp(ptok,strs[3],strlen(strs[3]))) {
data_point = strstr(ptok," ");
if(data_point != NULL) {
z_header->dest_id = transmute_char(data_point + 1);
}
}
ptok = strtok_r(NULL,"\n",saveptr);
z_header->length[0] = 0;
z_header->length[1] = 0;
z_header->length[2] = 0; 
switch (ptok[0]) { 
case 'M':
z_header->type = 0;
break;
case 'N':
z_header->type = 1;
break;
case 'L':
z_header->type = 3;
break;
default:
z_header->type = 2;

} 
return z_header;

}

char *zerg_type_encoder(zerg_header *z_header, char *data, int *tot_len) { 

char *hps;
char *arm;
char *float_str;
int float_holder;
int hp,namelen;
float f_cont;
char *payload;
char *name;
char *ppoint;
struct zerg_stat zerg;
int i,k,l,length,copy_len;  
switch(z_header->type) { 

case 0:
for(i = 0;data[i] != 'M'; ++i) { 
}
payload = &data[i]; 
ppoint = payload + 9;
for(i=0; ppoint[i] != 0;++i) { 
}
length = i; 
copy_len = length + PSYHDR_SZ;
payload = calloc(copy_len,sizeof(char));
z_header->length[0] = copy_len  >> 16; 
z_header->length[1] = copy_len >> 8; 
z_header->length[2] = copy_len; 
z_header->dest_id = htons(z_header->dest_id); 
z_header->source_id = htons(z_header->source_id);
z_header->sequence = htonl(z_header->sequence);
memcpy(payload,z_header,sizeof(zerg_header)); 
memcpy(payload + PSYHDR_SZ,ppoint,length);
*tot_len = copy_len; 
printf("%i\n",byte_ritual(z_header->dest_id)); 
break;


case 1:
for(i = 0;data[i] != 'N'; ++i) {
}
ppoint = &data[i]; 
while(*ppoint != ' ') { 
ppoint++; 
}
ppoint++;
for(i=0; ppoint[i] != 0;++i) {
printf("%c %x\n",ppoint[i],ppoint[i]);
}
namelen = i; 
name = ppoint;
while(*ppoint != ' ') { 
ppoint++;
}
ppoint++;
for(i=0; ppoint[i] != '/';++i) {
}
hps = calloc(i + 1,sizeof(char)); 
memcpy(hps,ppoint,i); 
hp = transmute_char(hps); 
zerg.hp[0] = hp >> 16;
zerg.hp[1] = hp >> 8;
zerg.hp[2] = hp;
ppoint = &ppoint[i] + 1;
for(i = 0; ppoint[i] != 0xa;++i) {
}
memcpy(hps,ppoint,i);
printf("%s\n",hps);
hp = transmute_char(hps);
zerg.max_hp[0] = hp >> 16;
zerg.max_hp[1] = hp >> 8;
zerg.max_hp[2] = hp;
free(hps); 
while(*ppoint != ' ') { 
ppoint++;
}
ppoint++;
for(i = 0; ppoint[i] != 0xa;++i) {
}
 
for(k = 0;k < 16;++k) { 
if(!strncmp(ppoint,zerg_breeds[k],i)) { 
zerg.type = k;
break;
}
}

while(*ppoint != ' ') { 
ppoint++;
}
ppoint++;
for(i = 0; ppoint[i] != 0xa;++i) {
}
arm = calloc(i + 1,sizeof(char)); 
zerg.ac = transmute_char(arm);
free(arm);   
while(*ppoint != ' ') { 
ppoint++;
}
ppoint++;
for(i = 0; ppoint[i] != 'm';++i) {
}
float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
copy_len =  PSYHDR_SZ + namelen + sizeof(struct zerg_stat);
f_cont = atof(float_str);
memcpy(&float_holder,&f_cont,sizeof(int)); 
zerg.max_speed[3] = float_holder;
zerg.max_speed[2] = float_holder >> 8;
zerg.max_speed[1] = float_holder >> 16;
zerg.max_speed[0] = float_holder >> 24;
free(float_str);
printf("%i\n",namelen); 
payload = calloc(copy_len,sizeof(char)); 
z_header->length[0] = copy_len  >> 16;
z_header->length[1] = copy_len >> 8;
z_header->length[2] = copy_len;
z_header->dest_id = htons(z_header->dest_id);
z_header->source_id = htons(z_header->source_id);
z_header->sequence = htonl(z_header->sequence);
memcpy(payload,z_header,sizeof(zerg_header));
memcpy((payload + PSYHDR_SZ),&zerg,sizeof(struct zerg_stat)); 
memcpy((payload + PSYHDR_SZ + sizeof(struct zerg_stat)),name,namelen);
*tot_len = copy_len;
break;
}
return payload; 
} 



