#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include "zerglib.h"
#include "pcap.h"
#include "templar.h"


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



