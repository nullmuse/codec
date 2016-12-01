#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "codelib.h"
#include "pcap.h"

int main(int argc, char *argv[]) {
char *rfile = NULL;
char *rtok = NULL;
int zerg_len; 
int psize = PCAP_SIZE;
int header_add = 0;
zerg_header *z_header; 
char *payload; 
char *total_data;
char *pcap_header; 
if(argc != 2) { 
printf("%s description_file\n",argv[0]); 
return 1;
}
init_pcap("encoded.pcap");
rfile = read_file(argv[1],0);
if(petal_handler(rfile,get_file_size(argv[1]))) { 
free(rfile); 
return 1;
}
rtok = strtok(rfile, "\t");
while(rtok != NULL) {
if(!strcmp(rtok,"\n"))
break;
pcap_header = pcap_build_init();
pcap_build_header(pcap_header,0); 
pcap_build_eth(pcap_header); 
pcap_build_iph(pcap_header);
pcap_build_udp(pcap_header); 
z_header = transmute_input(rtok);  
payload = zerg_type_encoder(z_header,rtok, &zerg_len);
total_data = calloc(PCAP_SIZE + zerg_len,sizeof(char)); 
memcpy(total_data,pcap_header,PCAP_SIZE); 
memcpy(total_data + PCAP_SIZE,payload,zerg_len);
size_fixups(total_data);
write_pcap((total_data + header_add),psize + zerg_len,"encoded.pcap");
free(total_data); 
free(pcap_header);
free(payload); 
free(z_header); 
rtok = strtok(NULL, "\t");
psize = PCAP_SIZE - 24;
header_add = 24;
}
free(rfile); 
return 0; 

}
