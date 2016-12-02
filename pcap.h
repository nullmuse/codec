#include <time.h>

static const int OP_SUCCESS = 0;
static const int OP_FAIL = 1;
static const int HEADER_SIZE = 4;
static const int PCAP_SIZE = 82;
static const int ETH_SIZE = 14; 
static const int IP_SIZE = 20; 
static const int UDP_SIZE = 8;
static const char psychicHeader[5] = {(char)0xd4,(char)0xc3,(char)0xb2,(char)0xa1,(char)0x00};
static const short MJ_VER = 2; 
static const short MN_VER = 4; 
static const short IP_VER = 4;
static const short IP_IHL = 5; 
static unsigned const short UDP_PORT = 42766;


static const short OFF_MJ_VER = 4;
static const short OFF_EPOCH = 24; 
static const short OFF_DMAC = 40;
static const short OFF_IP = 54; 
static const short OFF_UDP = 74;
static const short OFF_ZERG = 82;

struct pcapHeaderF { 
short majorVersion;
short minorVersion;
int gmtOffset; 
int accDelta; 
int maxLen; 
int linkType;
}; 

struct pcapHeaderP { 
int  epoch;
int  epochUs; 
int dataLen; 
int packetLen; 
};

struct ethFrame { 
char destMac[6]; 
char srcMac[6]; 
short ethType; 
}; 

struct ip4Header { 
char  hl:4,ver:4; 
char dscp;
short totLen; 
short ident;
short flags:3,offset:13; 
char ttl;
char protocol;  
short checksum; 
int srcIp;
int destIp; 
}; 

struct udpHeader { 
unsigned short srcPort;
unsigned short destPort; 
unsigned short len; 
unsigned short checksum; 
}; 




