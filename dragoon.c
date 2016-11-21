#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "templar.h" 
#include "pcap.h"



char *pcap_build_init(void) { 
int holder;
short holder_s; 
char *pcap_header = calloc(PCAP_SIZE, sizeof(char)); 

memcpy(pcap_header,psychic_header,HEADER_SIZE); 

if(!strncmp(pcap_header,psychic_header,HEADER_SIZE)) {
printf("Worked\n"); 
}

memcpy((pcap_header + OFF_MJVER),&MJ_VER,sizeof(short)); 

memcpy((pcap_header + OFF_MNVER),&MN_VER,sizeof(short));


return pcap_header; 

} 


