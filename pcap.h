#include <time.h>

static const int OP_SUCCESS = 0;
static const int OP_FAIL = 1;
static const int HEADER_SIZE = 4;
static const int PCAP_SIZE = 82;
static const int ETH_SIZE = 14; 
static const int IP_SIZE = 20; 
static const int UDP_SIZE = 8;
static const char psychic_header[5] = {(char)0xd4,(char)0xc3,(char)0xb2,(char)0xa1,(char)0x00};
static const short MJ_VER = 2; 
static const short MN_VER = 4; 


static const short OFF_MJVER = 4;
static const short OFF_EPOCH = 24; 
static const short OFF_DMAC = 40;

struct pcap_header_f { 
short major_version;
short minor_version;
int gmt_offset; 
int acc_delta; 
int maxlen; 
int link_type;
}; 

struct pcap_header_p { 
int  epoch;
int  epoch_us; 
int data_len; 
int packet_len; 
};

struct eth_frame { 
char dest_mac[6]; 
char src_mac[6]; 
short eth_type; 
}; 

struct ip4_header { 
char  ver:4,hl:4; 
char dscp:6,ecn:2;
short tot_len; 
short ident;
short flags:4,offset:13; 
char ttl;
char protocol;  
short checksum; 
int src_ip;
int dest_ip; 
}; 

struct udp_header { 
short src_port;
short dest_port; 
short len; 
short checksum; 
}; 




