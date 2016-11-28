#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h> 
#include <sys/time.h>
#include <net/if.h> 
#include <unistd.h> 
#include "pcap.h"
#include "codelib.h"


char *pcap_build_init(void) { 
char *pcap_header = calloc(PCAP_SIZE, sizeof(char));
struct pcap_header_f file_header; 

file_header.major_version = MJ_VER;
file_header.minor_version = MN_VER; 
file_header.gmt_offset = 0; 
file_header.acc_delta = 0; 
file_header.maxlen = 65536; 
file_header.link_type = 1; 
 

memcpy(pcap_header,psychic_header,HEADER_SIZE); 

memcpy((pcap_header + OFF_MJVER),&file_header,sizeof(struct pcap_header_f)); 

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

void pcap_build_iph(char *pcap_data) { 

struct ip4_header ip_header; 
int dat_len; 
memcpy(&dat_len,(pcap_data + OFF_EPOCH  + 8),sizeof(int)); 
ip_header.ver = IP_VER; 
ip_header.hl = IP_IHL; 
ip_header.dscp = 0x2e; //Expedited forwarding (101 Critical) 
ip_header.tot_len =  dat_len -  ETH_SIZE; 
printf("%i\n",ip_header.tot_len); 
ip_header.ident = 0; 
ip_header.flags = 2; //DF flag set
ip_header.offset = 0; 
ip_header.ttl = 4; //arbitrary 
ip_header.protocol = 17;  //UDP 
ip_header.checksum = 0; 
ip_header.src_ip = 12345678; 
ip_header.dest_ip = 87654321; 
 

memcpy((pcap_data + OFF_IP),&ip_header,sizeof(struct ip4_header));

return; 

}

void pcap_build_udp(char *pcap_data) { 

struct udp_header udp_head; 
int dat_len;
memcpy(&dat_len,(pcap_data + OFF_EPOCH  + 8),sizeof(int));
udp_head.src_port = 1337; 
udp_head.dest_port = UDP_PORT; 
udp_head.len = dat_len - ETH_SIZE - IP_SIZE; 
udp_head.checksum = 0; 

memcpy((pcap_data + OFF_UDP),&udp_head,sizeof(struct udp_header));

return; 
}

