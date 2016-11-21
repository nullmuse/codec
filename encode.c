#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "codelib.h"
#include "pcap.h"

int main(int argc, char *argv[]) {


char *pcap_header = pcap_build_init(); 

write_pcap(pcap_header,PCAP_SIZE,"testout.pcap");


return 0; 

}
