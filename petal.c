#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "petal.h"

 



 int advanceInterpreter(int i, int petalSize, char *petal) {
    for(;i < petalSize && petal[i] != 0xa; ++i) {
    }
 if(i == petalSize) {
    return INVALID_PETAL_SCRIPT;
    }
    i++;
    return i; 
    } 

int interpretMessage(char *petalMessage) { 
 int i;
 printf("Entering interpretMessage\n");
 int messageLength = 0;
 i = strlen(messageStanza[0]); 
 for(;petalMessage[i] != 0xa;++i) {
 messageLength++;
 }
 if(messageLength < 1)
    return INVALID_PETAL_SCRIPT;
 return VALID_PETAL_SCRIPT;
 }
int interpretStatus(char *petalStatus, int *lineNo) {
printf("Entering interpretStatus\n");
int k,m,localLineNo;
int i = 0;
int messageLength = 0;
localLineNo = *lineNo;
int canary;
for(k = 0;k < 5;++k) {
if(strncmp(&petalStatus[i],statusStanza[k],strlen(statusStanza[k]))) {
          memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
          }
printf("k iteration %i\n",k); 
canary = 0;
if(k == 1 || k == 2 || k == 4) {
canary = -1;
}
messageLength = 0;
for(;petalStatus[i] != 0xa;++i) {
printf("rolling %c %x\n",petalStatus[i],petalStatus[i]);
if(k == 1 && petalStatus[i] == '/') {
canary = 0;
}
if(k == 2 && petalStatus[(i - 1)] == ' ') { 
for(m = 0; m < 15; ++m) { 
if(!strncmp(&petalStatus[i],zergBreeds[m],strlen(zergBreeds[m]))) { 
canary = 0; 
}
}
}
if(k == 4) { 
if(!strncmp(&petalStatus[i],"m/s",3)) { 
canary = 0; 
} 


}
}
if(petalStatus[(i - 1)] == ' ') { 
memcpy(lineNo,&localLineNo,sizeof(int));
return INVALID_PETAL_SCRIPT;
}
++i;
localLineNo++;
if(messageLength - strlen(statusStanza[k]) == 0) {
          localLineNo--;
          memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
if(canary < 0) { 
          localLineNo--;
          memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
}
return VALID_PETAL_SCRIPT;
}

int interpretGps(char *petalGps, int *lineNo) { 

printf("Entering interpretGps\n");
int k,m,localLineNo;
int i = 0;
int messageLength = 0;
int savePoint = 0;
localLineNo = *lineNo;
for(k = 0;k < 6;++k) { 
if(strncmp(&petalGps[i],gpsStanza[k],strlen(gpsStanza[k]))) {
          memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
          }
printf("k iteration %i\n",k);
messageLength = 0;
for(;petalGps[i] != 0xa;++i) {
}
switch(k) { 
case 0:
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[0],strlen(gpsTokens[0])) == NULL) { 
memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[1],strlen(gpsTokens[1])) == NULL) {
   if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[0],strlen(gpsTokens[0])) == NULL) {
      memcpy(lineNo,&localLineNo,sizeof(int));
      return INVALID_PETAL_SCRIPT;
}
}
break;
case 1:
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[0],strlen(gpsTokens[0])) == NULL) {
memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[3],strlen(gpsTokens[3])) == NULL) {
   if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[4],strlen(gpsTokens[4])) == NULL) {
      memcpy(lineNo,&localLineNo,sizeof(int));
      return INVALID_PETAL_SCRIPT;
}
}
break;
case 2:
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[6],strlen(gpsTokens[6])) == NULL) {
memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
break; 
case 3:
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[0],strlen(gpsTokens[0]) - 1) == NULL) {
memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
break;
case 4:
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[5],strlen(gpsTokens[5])) == NULL) {
memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
break;
case 5:
if(memmem(&petalGps[savePoint],(i - savePoint),gpsTokens[6],strlen(gpsTokens[6])) == NULL) {
memcpy(lineNo,&localLineNo,sizeof(int));
          return INVALID_PETAL_SCRIPT;
}
break;
}
++i;
localLineNo++;
savePoint = i; 
}


return VALID_PETAL_SCRIPT; 
}

 int checkStanzaSignature(char *petalSignature,int *lineNo) { 
   printf("Entering checkStanzaSignature()\n");
   int errType = 0;
    switch(petalSignature[0]) { 
       case 'M':
          if(strncmp(petalSignature,messageStanza[0],strlen(messageStanza[0]))) {
          return INVALID_PETAL_SCRIPT;
          }
          errType = interpretMessage(petalSignature);
          break;
       case 'N':
          if(strncmp(petalSignature,statusStanza[0],strlen(statusStanza[0]))) {
          return INVALID_PETAL_SCRIPT;
          }
          errType = interpretStatus(petalSignature,lineNo);
          break;
       case 'L':
          if(strncmp(petalSignature,gpsStanza[0],strlen(gpsStanza[0]))) {
          return INVALID_PETAL_SCRIPT;
          }
          errType = interpretGps(petalSignature,lineNo);
          break;
       default:
          printf("Default case found\n");
          if(strncmp(petalSignature,zergCommands[0],strlen(zergCommands[0]))) {
          return INVALID_PETAL_SCRIPT;
          }
          break;
    }
    return errType;
}

 int petalInterpreter(char *petal, int petalSize,int *line) { 
 int errType = 0;
 char *strs[] = {"Version: ","Sequence: ","From: ","To: "};
 int i = 0;
 int lineNo = 0;
 const char **stanzaList = NULL;
 if(strncmp(strs[0],petal,strlen(strs[0]))) {
    lineNo = 0;
    memcpy(line,&lineNo,sizeof(int)); 
    return INVALID_PETAL_STANZA_VER;
    }
 i = advanceInterpreter(i,petalSize,petal);
 if(i < 0)
 return INVALID_PETAL_SCRIPT;
 lineNo = 1;
 if(strncmp(strs[1],&petal[i],strlen(strs[1]))) {
    memcpy(line,&lineNo,sizeof(int));
    return INVALID_PETAL_STANZA_SEQ;
    }
 i = advanceInterpreter(i,petalSize,petal);
 if(i < 0)
 return INVALID_PETAL_SCRIPT;
 lineNo++;
 if(strncmp(strs[2],&petal[i],strlen(strs[2]))) {
    memcpy(line,&lineNo,sizeof(int));
    return INVALID_PETAL_STANZA_SID;
    }
 i = advanceInterpreter(i,petalSize,petal);
 if(i < 0)
 return INVALID_PETAL_SCRIPT;
 lineNo++;
 if(strncmp(strs[3],&petal[i],strlen(strs[3]))) {
    memcpy(line,&lineNo,sizeof(int));
    return INVALID_PETAL_STANZA_DID;
    }
 i = advanceInterpreter(i,petalSize,petal);
 if(i < 0)
 return INVALID_PETAL_SCRIPT;
 lineNo++;
 errType = checkStanzaSignature(&petal[i],&lineNo);
 memcpy(line,&lineNo,sizeof(int));
 return errType;

 }


 int petalHandler(char *petal, int petalSize) {
    int retCode = 0;
    int lineNo = 0;
    retCode = petalInterpreter(petal,petalSize,&lineNo);
    lineNo++;
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
