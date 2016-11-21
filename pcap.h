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
static const short IP_VER = 4;
static const short IP_IHL = 5; 
static unsigned const short UDP_PORT = 42766;


static const short OFF_MJVER = 4;
static const short OFF_EPOCH = 24; 
static const short OFF_DMAC = 40;
static const short OFF_IP = 54; 
static const short OFF_UDP = 74;

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
char  hl:4,ver:4; 
char dscp;
short tot_len; 
short ident;
short flags:3,offset:13; 
char ttl;
char protocol;  
short checksum; 
int src_ip;
int dest_ip; 
}; 

struct udp_header { 
unsigned short src_port;
unsigned short dest_port; 
unsigned short len; 
unsigned short checksum; 
}; 




