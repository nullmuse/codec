#include "zerglib.h"

int validate_file(FILE *fp); 
char *read_pcap(char *fname); 
int get_file_size(char *fname);
psy_data *transmute_header(char *pdata); 
void read_psy_message(psy_data *psy);
void read_psy_status(psy_data *psy);
void read_psy_command(psy_data *psy);
void read_psy_gps(psy_data *psy);
int psionic_divagate(char *stream, int stream_size, psy_data **psy_list);
int write_pcap(char *pcap_data,int pcap_size,char *filename); 
char *pcap_build_init(void);
void pcap_build_header(char *pcap_data, int pcap_size);
void pcap_build_eth(char *pcap_data);
void pcap_build_iph(char *pcap_data);
void pcap_build_udp(char *pcap_data); 
