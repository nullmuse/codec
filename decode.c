#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include "codelib.h" 

int main(int argc, char *argv[]) { 

char *pdata = NULL; 
psy_data *psy; 
if(argc <= 1) {
printf("Usage: %s psy.pcap\n",argv[0]);
goto RETURN;
}
pdata = read_pcap(argv[1]); 

if (pdata == NULL) {
     goto RETURN;
}


psy = transmute_header(pdata); 

switch(psy->type) {

case ZERG_MESSAGE:
   read_psy_message(psy);
   break;
case ZERG_STATUS:
   read_psy_status(psy);
   break;
case ZERG_COMMAND:
   read_psy_command(psy);
   break;
case ZERG_GPS:
   read_psy_gps(psy);
   break;

default:
   printf("Unrecognized/non-Zerg psionic capture stream.\n"); 
   printf("Use Zerg psionic capture stream or recalibrate psi receiver\n"); 
   break; 
}

RETURN:
if(pdata != NULL)
free(pdata);
if(psy != NULL)
free(psy);
return 0; 

} 



