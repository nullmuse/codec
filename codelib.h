#include "zerglib.h"

int validate_file(FILE *fp); 
char *read_file(char *fname, int is_pcap); 
int write_pcap(char *pcap_data,int pcap_size,char *filename);
int get_file_size(char *fname);
psy_data *transmute_header(char *pdata); 
void read_psy_message(psy_data *psy);
void read_psy_status(psy_data *psy);
void read_psy_command(psy_data *psy);
void read_psy_gps(psy_data *psy);
psy_data **psionic_divagate(char *stream, int stream_size, int *psy_ptr);
int write_pcap(char *pcap_data,int pcap_size,char *filename); 
char *pcap_build_init(void);
void pcap_build_header(char *pcap_data, int pcap_size);
void pcap_build_eth(char *pcap_data);
void pcap_build_iph(char *pcap_data);
void pcap_build_udp(char *pcap_data); 
zerg_header *transmute_input(char *pdata);
char *zerg_type_encoder(zerg_header *z_header, char *data, int *tot_len);
void size_fixups(char *packet);
