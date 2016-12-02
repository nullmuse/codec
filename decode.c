 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/stat.h> 
 #include "codelib.h" 
 
 int main(int argc, char *argv[]) { 
    
    char *pData = NULL; 
    psyData *psy = NULL; 
    psyData **psyList = NULL;
    int psyCount,i; 
    if(argc <= 1) {
       printf("Usage: %s psy.pcap\n",argv[0]);
       goto END;
       }
 pData = readFile(argv[1],1); 
 if (pData == NULL) {
         goto END;
    }
 psyCount = 0;
 psyList = psionicDivagate(pData,getFileSize(argv[1]),&psyCount); 
 for(i = 0;i < psyCount; ++i) { 
    psy = psyList[i];
    switch(psy->type) {
       
       case ZERG_MESSAGE:
          readPsyMessage(psy);
          break;
       case ZERG_STATUS:
          readPsyStatus(psy);
          break;
       case ZERG_COMMAND:
          readPsyCommand(psy);
          break;
       case ZERG_GPS:
          readPsyGps(psy);
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
 free(pData);
 free(psyList);
 return 0; 
 
 } 
 
 
 
