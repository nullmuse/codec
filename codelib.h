#include "zerglib.h"

int validate_file(FILE *fp); 
char *read_pcap(char *fname); 
int get_file_size(char *fname);
psy_data *transmute_header(char *pdata); 
