#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "codelib.h"
#include "pcap.h"

int main(int argc, char *argv[]) {


char *pcap_header = pcap_build_init(); 

pcap_build_header(pcap_header,10); 
 
write_pcap(pcap_header,PCAP_SIZE,"testout.pcap");


return 0; 

}
