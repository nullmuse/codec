 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/stat.h>
 #include "codelib.h"
 #include "pcap.h"
 
 int main(int argc, char *argv[]) {
    char *rFile = NULL;
    char *rTok = NULL;
    int zergLen; 
    int pSize = PCAP_SIZE;
    int headerAdd = 0;
    zergHeader *zHeader; 
    char *payload; 
    char *totalData;
    char *pcapHeader; 
    if(argc != 2) { 
       printf("%s descriptionFile\n",argv[0]); 
       return 1;
       }
 initPcap("encoded.pcap");
 rFile = readFile(argv[1],0);
 if(petalHandler(rFile,getFileSize(argv[1]))) { 
    free(rFile); 
    return 1;
    }
 rTok = strtok(rFile, "\t");
 while(rTok != NULL) {
    if(!strcmp(rTok,"\n"))
    break;
    pcapHeader = pcapBuildInit();
    pcapBuildHeader(pcapHeader,0); 
    pcapBuildEth(pcapHeader); 
    pcapBuildIph(pcapHeader);
    pcapBuildUdp(pcapHeader); 
    zHeader = transmuteInput(rTok);  
    payload = zergTypeEncoder(zHeader,rTok, &zergLen);
    totalData = calloc(PCAP_SIZE + zergLen,sizeof(char)); 
    memcpy(totalData,pcapHeader,PCAP_SIZE); 
    memcpy(totalData + PCAP_SIZE,payload,zergLen);
    sizeFixups(totalData);
    writePcap((totalData + headerAdd),pSize + zergLen,"encoded.pcap");
    free(totalData); 
    free(pcapHeader);
    free(payload); 
    free(zHeader); 
    rTok = strtok(NULL, "\t");
    pSize = PCAP_SIZE - 24;
    headerAdd = 24;
    }
 free(rFile); 
 return 0; 
 
 }
