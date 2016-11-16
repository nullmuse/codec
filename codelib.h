#include "zerglib.h"

int validate_file(FILE *fp); 
char *read_pcap(char *fname); 
int get_file_size(char *fname);
psy_data *transmute_header(char *pdata); 
void read_psy_message(psy_data *psy);
void read_psy_status(psy_data *psy);
void read_psy_command(psy_data *psy);
void read_psy_gps(psy_data *psy);
