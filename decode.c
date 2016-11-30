#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include "codelib.h" 

int main(int argc, char *argv[]) { 

char *pdata = NULL; 
psy_data *psy = NULL; 
psy_data **psy_list = NULL;
int psy_count,i; 
if(argc <= 1) {
printf("Usage: %s psy.pcap\n",argv[0]);
goto END;
}
pdata = read_file(argv[1],1); 
printf("%p\n",pdata); 
if (pdata == NULL) {
     goto END;
}
psy_count = 0;
//psy_list = calloc(1,sizeof(psy_data *));
psy_list = psionic_divagate(pdata,get_file_size(argv[1]),&psy_count); 
for(i = 0;i < psy_count; ++i) { 
psy = psy_list[i];
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
printf("\t\n");
free(psy);
}

END:
free(pdata);
//printf("%p\n",psy_list); 
//if(psy_list != NULL)
free(psy_list);
return 0; 

} 



