#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "codelib.h"
#include "pcap.h"

int main(int argc, char *argv[]) {
char *rfile;
zerg_header *z_header; 
char *payload; 
char *pcap_header = pcap_build_init(); 
rfile = read_file(argv[1],0);
pcap_build_header(pcap_header,10); 
pcap_build_eth(pcap_header); 
pcap_build_iph(pcap_header);
pcap_build_udp(pcap_header); 
rfile = read_file(argv[1],0);
z_header = transmute_input(rfile);
printf("%i\n",z_header->type); 
printf("%i\n",z_header->dest_id); 
write_pcap(pcap_header,PCAP_SIZE,"testout.pcap");

return 0; 

}
