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

char *
pcapBuildInit(void)
{
    char *pcapHeader = calloc(PCAP_SIZE, sizeof(char));
    struct pcapHeaderF fileHeader; 
    fileHeader.majorVersion = MJ_VER;
    fileHeader.minorVersion = MN_VER; 
    fileHeader.gmtOffset = 0; 
    fileHeader.accDelta = 0; 
    fileHeader.maxLen = 65536; 
    fileHeader.linkType = 1; 
    memcpy(pcapHeader,psychicHeader,HEADER_SIZE); 
    memcpy((pcapHeader + OFF_MJ_VER),&fileHeader,sizeof(struct pcapHeaderF)); 
    return pcapHeader;
}

void 
pcapBuildHeader(
char *pcapData,
int pcapSize)
{
    struct pcapHeaderP pHeader; 
    struct timeval tv; 
    gettimeofday(&tv,NULL); 
    pHeader.epoch = tv.tv_sec; 
    pHeader.epochUs = tv.tv_usec;
    pHeader.dataLen = pcapSize + ETH_SIZE + IP_SIZE + UDP_SIZE; 
    pHeader.packetLen = pcapSize + ETH_SIZE + IP_SIZE + UDP_SIZE;
    memcpy((pcapData + OFF_EPOCH),&pHeader,sizeof(struct pcapHeaderP));
    return; 
    
}
 
 
void 
pcapBuildEth(
char *pcapData)
{ 
    
    struct ethFrame ethernet; 
    memcpy(ethernet.destMac,"000000",6); 
    memcpy(ethernet.srcMac,"111111",6); 
    ethernet.ethType = 8; 
    memcpy((pcapData + OFF_DMAC),&ethernet,sizeof(struct ethFrame));
    
    return; 
    
}
 
void
pcapBuildIph(
char *pcapData)
{
    struct ip4Header ipHeader; 
    int datLen; 
    memcpy(&datLen,(pcapData + OFF_EPOCH  + 8),sizeof(int)); 
    ipHeader.ver = IP_VER; 
    ipHeader.hl = IP_IHL; 
    ipHeader.dscp = 0x2e; //Expedited forwarding (101 Critical) 
    ipHeader.totLen =  datLen -  ETH_SIZE;  
    ipHeader.ident = 0; 
    ipHeader.flags = 2; //DF flag set
    ipHeader.offset = 0; 
    ipHeader.ttl = 4; //arbitrary 
    ipHeader.protocol = 17;  //UDP 
    ipHeader.checksum = 0; 
    ipHeader.srcIp = 12345678; 
    ipHeader.destIp = 87654321; 
    memcpy((pcapData + OFF_IP),&ipHeader,sizeof(struct ip4Header));
    return; 
}
 
void
pcapBuildUdp(
char *pcapData)
{ 
    struct udpHeader udpHead; 
    int datLen;
    memcpy(&datLen,(pcapData + OFF_EPOCH  + 8),sizeof(int));
    udpHead.srcPort = 1337; 
    udpHead.destPort = UDP_PORT; 
    udpHead.len = datLen - ETH_SIZE - IP_SIZE; 
    udpHead.checksum = 0; 
    memcpy((pcapData + OFF_UDP),&udpHead,sizeof(struct udpHeader));
    
    return; 
}
 
