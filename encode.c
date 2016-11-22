#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "codelib.h"
#include "pcap.h"

int main(int argc, char *argv[]) {
char *rfile;
int i;
int zerg_len; 
zerg_header *z_header; 
char *payload; 
char *total_data;
char *pcap_header = pcap_build_init(); 
rfile = read_file(argv[1],0);
pcap_build_header(pcap_header,10); 
pcap_build_eth(pcap_header); 
pcap_build_iph(pcap_header);
pcap_build_udp(pcap_header); 
z_header = transmute_input(rfile);
printf("%i\n",z_header->type); 
printf("%i\n",z_header->dest_id); 
printf("%s\n",rfile); 
payload = zerg_type_encoder(z_header,rfile, &zerg_len);
printf("%i\n",zerg_len);
total_data = calloc(PCAP_SIZE + zerg_len,sizeof(char)); 
memcpy(total_data,pcap_header,PCAP_SIZE); 
memcpy(total_data + PCAP_SIZE,payload,zerg_len);
write_pcap(total_data,PCAP_SIZE + zerg_len,"testout.pcap");

return 0; 

}
