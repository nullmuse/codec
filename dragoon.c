#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "templar.h"
#include <sys/socket.h>
#include <sys/ioctl.h> 
#include "pcap.h"
#include <sys/time.h>
#include <net/if.h> 
#include <unistd.h> 

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


void pcap_build_header(char *pcap_data, int pcap_size) { 

struct pcap_header_p p_header; 
struct timeval tv; 
gettimeofday(&tv,NULL); 
p_header.epoch = tv.tv_sec; 
p_header.epoch_us = tv.tv_usec;
p_header.data_len = pcap_size + ETH_SIZE + IP_SIZE + UDP_SIZE; 
p_header.packet_len = pcap_size + ETH_SIZE + IP_SIZE + UDP_SIZE;
memcpy((pcap_data + OFF_EPOCH),&p_header,sizeof(struct pcap_header_p));

return; 

}


void pcap_build_eth(char *pcap_data) { 

struct eth_frame ethernet; 
/*
int fd; 
struct ifreq ifr; 
char *interface = "eth0"; 
char *mac = NULL; 

fd = socket(AF_INET, SOCK_DGRAM, 0); 

ifr.ifr_addr.sa_family = AF_INET; 
memcpy(ifr.ifr_name, interface, IFNAMESIZ - 1); 
ioctl(fd, SIOCGIFHWADDR, &ifr);
memcpy(ethernet.dest_mac,(unsigned char *)ifr.ifr_hwaddr.sa_data,6); 
memcpy(ethernet.src_mac, (unsigned char *)ifr.ifr_hwaddr.sa_data,6);
*/
memcpy(ethernet.dest_mac,"000000",6); 
memcpy(ethernet.src_mac,"111111",6); 
ethernet.eth_type = 8; 
memcpy((pcap_data + OFF_DMAC),&ethernet,sizeof(struct eth_frame));

return; 

} 




