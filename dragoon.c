#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "templar.h" 
#include "pcap.h"



char *pcap_build_init(void) { 
int holder;
short holder_s; 
char *pcap_header = calloc(PCAP_SIZE, sizeof(char)); 

struct pcap_header_f file_header; 

file_header.major_version = MJ_VER;
file_header.minor_version = MN_VER; 
file_header.gmt_offset = 0; 
file_header.acc_delta = 0; 
file_header.maxlen = 65536; 
file_header.link_type = 1; 
 

memcpy(pcap_header,psychic_header,HEADER_SIZE); 

if(!strncmp(pcap_header,psychic_header,HEADER_SIZE)) {
printf("Worked\n"); 
}

memcpy((pcap_header + OFF_MJVER),&file_header,sizeof(struct pcap_header_f)); 

//memcpy((pcap_header + OFF_MNVER),&MN_VER,sizeof(short));


return pcap_header; 

} 


