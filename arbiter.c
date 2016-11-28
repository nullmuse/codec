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
long double_holder;
int hp,namelen;
float f_cont;
double d_cont;
char *payload;
char *name;
char *param_str;
short param_short;
int param_int;
char *ppoint;
struct zerg_stat zerg;
struct zerg_gps gps; 
struct zerg_command command;
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

case 2:
for(i = 0;data[i] != 'T'; ++i) {
}
for(;data[i] != 0xa && data[i] != 0; ++i) {
}
ppoint = &data[i];
ppoint++; 
for(i = 0;ppoint[i] != 0xa && ppoint[i] != 0; ++i) {
}
for(k = 0;k < 8;++k) {
if(!strncmp(ppoint,ZERG_COMMANDS[k],i)) {
command.command = htons(k);
break;
}
}
switch(k) { 
case 1:
ppoint += (i + 1);
while(*ppoint != ':') {
ppoint++;
}
ppoint += 2; 
for(i = 0; ppoint[i] != 0xa;++i) {
}
float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
f_cont = atof(float_str);
memcpy(&float_holder,&f_cont,sizeof(int));
command.param2[3] = float_holder;
command.param2[2] = float_holder >> 8;
command.param2[1] = float_holder >> 16;
command.param2[0] = float_holder >> 24;
free(float_str); 
while(*ppoint != ' ') {
ppoint++;
}
ppoint++; 
for(i = 0; ppoint[i] != ' ';++i) {
}
param_str = calloc(i + 1,sizeof(char));
memcpy(param_str,ppoint,i);
param_short = atoi(param_str); 
command.param1[1] = param_short;
command.param1[0] = param_short >> 8;
free(param_str); 
break;
case 5:
ppoint += (i + 1);
if(*ppoint == 'A') { 
command.param1[1] = 1;
command.param1[0] = 0;
}
else{
command.param1[1] = 0;
command.param1[0] = 0;
}
while(*ppoint != ':') {
ppoint++;
}
ppoint += 2;
for(i = 0; ppoint[i] != 0xa;++i) {
}
param_str = calloc(i + 1,sizeof(char));
memcpy(param_str,ppoint,i);
param_int = atoi(param_str);
command.param2[3] = param_int;
command.param2[2] = param_int >> 8;
command.param2[1] = param_int >> 16;
command.param2[0] = param_int >> 24;
free(param_str); 
break;
case 7:
command.param1[0] = 0;
command.param1[1] = 0;
while(*ppoint != ':') {
ppoint++;
}
ppoint += 2;
for(i = 0; ppoint[i] != 0xa;++i) {
}
param_str = calloc(i + 1,sizeof(char));
memcpy(param_str,ppoint,i);
param_int = atoi(param_str);
command.param2[3] = param_int;
command.param2[2] = param_int >> 8;
command.param2[1] = param_int >> 16;
command.param2[0] = param_int >> 24;
free(param_str);
break;
default:
command.param1[0] = 0;
command.param1[1] = 0;
command.param2[3] = 0;
command.param2[2] = 0;
command.param2[1] = 0;
command.param2[0] = 0;
break;
}
copy_len =  PSYHDR_SZ + sizeof(struct zerg_command);
payload = calloc(copy_len,sizeof(char));
z_header->length[0] = copy_len  >> 16;
z_header->length[1] = copy_len >> 8;
z_header->length[2] = copy_len;
z_header->dest_id = htons(z_header->dest_id);
z_header->source_id = htons(z_header->source_id);
z_header->sequence = htonl(z_header->sequence);
memcpy(payload,z_header,sizeof(zerg_header));
memcpy((payload + sizeof(zerg_header)),&command,sizeof(struct zerg_command));
*tot_len = copy_len;
break;


case 3:
for(i = 0;data[i] != 'L'; ++i) {
}
ppoint = &data[i];
while(*ppoint != ' ') {
ppoint++;
}
ppoint++;
for(i = 0; ppoint[i] != ' ';++i) {
}
float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
d_cont = atof(float_str);
while(*ppoint != ' ') {
ppoint++;
}
ppoint++;
ppoint += 5;
if(*ppoint > 0x4e) { 
d_cont = d_cont * -1;
};
memcpy(&double_holder,&d_cont,sizeof(long));
gps.lon[7] = double_holder;
gps.lon[6] = double_holder >> 8;
gps.lon[5] = double_holder >> 16;
gps.lon[4] = double_holder >> 24;
gps.lon[3] = double_holder >> 32;
gps.lon[2] = double_holder >> 40;
gps.lon[1] = double_holder >> 48;
gps.lon[0] = double_holder >> 56;
free(float_str); 
while(*ppoint != ' ') {
ppoint++;
}
ppoint++;
for(i = 0; ppoint[i] != ' ';++i) {
}
float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
d_cont = atof(float_str);
while(*ppoint != ' ') {
ppoint++;
}
ppoint++;
ppoint += 5;
if(*ppoint > 0x4e) {
d_cont = d_cont * -1;
};
memcpy(&double_holder,&d_cont,sizeof(long));
gps.lat[7] = double_holder;
gps.lat[6] = double_holder >> 8;
gps.lat[5] = double_holder >> 16;
gps.lat[4] = double_holder >> 24;
gps.lat[3] = double_holder >> 32;
gps.lat[2] = double_holder >> 40;
gps.lat[1] = double_holder >> 48;
gps.lat[0] = double_holder >> 56;
free(float_str);
while(*ppoint != ' ') {
ppoint++;
}
ppoint++;
for(i = 0; ppoint[i] != 'm';++i) {
}
float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
f_cont = atof(float_str);
f_cont /= FATHOM_METERS;
memcpy(&float_holder,&f_cont,sizeof(int));
gps.alt[3] = float_holder;
gps.alt[2] = float_holder >> 8;
gps.alt[1] = float_holder >> 16;
gps.alt[0] = float_holder >> 24;
free(float_str);
while(*ppoint != ' ') {
ppoint++;
}
ppoint++;
for(i = 0; ppoint[i] != ' ';++i) {
}

float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
f_cont = atof(float_str);
memcpy(&float_holder,&f_cont,sizeof(int));
gps.bearing[3] = float_holder;
gps.bearing[2] = float_holder >> 8;
gps.bearing[1] = float_holder >> 16;
gps.bearing[0] = float_holder >> 24;
free(float_str);
while(*ppoint != 0xa) {
ppoint++;
}
while(*ppoint != ' ') {
ppoint++;
}
ppoint++; 
for(i = 0; ppoint[i] != 'm';++i) {
}
float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
f_cont = atof(float_str);
memcpy(&float_holder,&f_cont,sizeof(int));
gps.speed[3] = float_holder;
gps.speed[2] = float_holder >> 8;
gps.speed[1] = float_holder >> 16;
gps.speed[0] = float_holder >> 24;
free(float_str);
while(*ppoint != ' ') {
ppoint++;
}
ppoint++; 
for(i = 0; ppoint[i] != 'm';++i) {
}
float_str = calloc(i + 1,sizeof(char));
memcpy(float_str,ppoint,i);
f_cont = atof(float_str);
memcpy(&float_holder,&f_cont,sizeof(int));
gps.accuracy[3] = float_holder;
gps.accuracy[2] = float_holder >> 8;
gps.accuracy[1] = float_holder >> 16;
gps.accuracy[0] = float_holder >> 24;
free(float_str);
copy_len =  PSYHDR_SZ + sizeof(struct zerg_gps);
payload = calloc(copy_len,sizeof(char)); 
z_header->length[0] = copy_len  >> 16;
z_header->length[1] = copy_len >> 8;
z_header->length[2] = copy_len;
z_header->dest_id = htons(z_header->dest_id);
z_header->source_id = htons(z_header->source_id);
z_header->sequence = htonl(z_header->sequence);
memcpy(payload,z_header,sizeof(zerg_header));
memcpy((payload + sizeof(zerg_header)),&gps,sizeof(struct zerg_gps));
*tot_len = copy_len;
break;

default:

printf("Error: Unrecognized packet type\n");
payload = NULL;
break;
}

return payload; 
}


void size_fixups(char *packet) { 
int zerg_length;
int size_frame = 40;
short udp_length = UDP_SIZE;
short ip_length = IP_SIZE;
int packet_length = ETH_SIZE + UDP_SIZE + IP_SIZE;
char short_t[2];
char int_t[4];
memcpy(&zerg_length,&packet[OFF_LEN + 40],sizeof(short) + 1);
printf("zerg_length %i hex: %x\n",zerg_length,zerg_length);
zerg_length = byte_ritual(zerg_length) >> 8;
printf("zerg_length %i hex: %x\n",zerg_length,zerg_length);
udp_length += zerg_length; 
short_t[1] = udp_length;
short_t[0] = udp_length >> 8;
memcpy(&packet[OFF_UDP + 4],&short_t,sizeof(short));
ip_length += udp_length;
short_t[1] = ip_length;
short_t[0] = ip_length >> 8;
memcpy(&packet[OFF_IP + 2],&short_t,sizeof(short));
packet_length += zerg_length;
printf("packet length %i hex: %x\n",packet_length,packet_length);
memcpy(&packet[OFF_EPOCH + 8],&packet_length,sizeof(int));
memcpy(&packet[OFF_EPOCH + 12],&packet_length,sizeof(int));
}

