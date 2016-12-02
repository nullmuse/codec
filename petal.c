 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "petal.h"
 
 
 int petalInterpreter(char *petal, int petalSize,int *line) { 
    
    char *strs[] = {"Version: ","Sequence: ","From: ","To: "};
 int i;
 int lineNo = 0;
 if(strncmp(strs[0],petal,strlen(strs[0]))) {
    lineNo = 0;
    memcpy(line,&lineNo,sizeof(int)); 
    return INVALID_PETAL_STANZA_VER; 
    }
 for(i = 0;i < petalSize && petal[i] != 0xa; ++i) {
    }
 if(i == petalSize) { 
    return INVALID_PETAL_SCRIPT;
    }
 ++i;
 lineNo = 1;
 if(strncmp(strs[1],&petal[i],strlen(strs[1]))) {
    memcpy(line,&lineNo,sizeof(int));
    return INVALID_PETAL_STANZA_SEQ;
    }
 for(;i < petalSize && petal[i] != 0xa; ++i) {
    }
 if(i == petalSize) {
    return INVALID_PETAL_SCRIPT;
    }
 ++i;
 lineNo++;
 if(strncmp(strs[2],&petal[i],strlen(strs[2]))) {
    memcpy(line,&lineNo,sizeof(int));
    return INVALID_PETAL_STANZA_SID;
    }
 for(;i < petalSize && petal[i] != 0xa; ++i) {
    }
 if(i == petalSize) {
    return INVALID_PETAL_SCRIPT;
    }
 ++i;
 lineNo++;
 if(strncmp(strs[3],&petal[i],strlen(strs[3]))) {
    memcpy(line,&lineNo,sizeof(int));
    return INVALID_PETAL_STANZA_DID;
    }
 
 
 
 return VALID_PETAL_SCRIPT; 
 
 } 
 
 
 int petalHandler(char *petal, int petalSize) {
    int retCode = 0;
    int lineNo = 0;
    retCode = petalInterpreter(petal,petalSize,&lineNo);
    switch(retCode) {
       case INVALID_PETAL_STANZA_VER:
       printf("PETAL ERROR: Invalid Version at line %i\n",lineNo);
       break;
       case INVALID_PETAL_STANZA_SEQ:
       printf("PETAL ERROR: Invalid Sequence at line %i\n",lineNo);
       break;
       case INVALID_PETAL_STANZA_SID:
       printf("PETAL ERROR: Invalid Source ID at line %i\n",lineNo);
       break;
       case INVALID_PETAL_STANZA_DID:
       printf("PETAL ERROR: Invalid Destination ID at line %i\n",lineNo);
       break;
       case INVALID_PETAL_SCRIPT:
       printf("PETAL ERROR: Invalid PETAL script at line %i\n",lineNo);
       break;
       }
 return retCode; 
 
 } 
