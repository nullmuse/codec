#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "petal.h"


int petal_interpreter(char *petal, int petal_size,int *line) { 

char *strs[] = {"Version: ","Sequence: ","From: ","To: "};
int i;
int line_no = 0;
if(strncmp(strs[0],petal,strlen(strs[0]))) {
line_no = 0;
memcpy(line,&line_no,sizeof(int)); 
return INVALID_PETAL_STANZA_VER; 
}
for(i = 0;i < petal_size && petal[i] != 0xa; ++i) {
}
if(i == petal_size) { 
return INVALID_PETAL_SCRIPT;
}
++i;
line_no = 1;
if(strncmp(strs[1],&petal[i],strlen(strs[1]))) {
memcpy(line,&line_no,sizeof(int));
return INVALID_PETAL_STANZA_SEQ;
}
for(;i < petal_size && petal[i] != 0xa; ++i) {
}
if(i == petal_size) {
return INVALID_PETAL_SCRIPT;
}
++i;
line_no++;
if(strncmp(strs[2],&petal[i],strlen(strs[2]))) {
memcpy(line,&line_no,sizeof(int));
return INVALID_PETAL_STANZA_SID;
}
for(;i < petal_size && petal[i] != 0xa; ++i) {
}
if(i == petal_size) {
return INVALID_PETAL_SCRIPT;
}
++i;
line_no++;
if(strncmp(strs[3],&petal[i],strlen(strs[3]))) {
memcpy(line,&line_no,sizeof(int));
return INVALID_PETAL_STANZA_DID;
}

return VALID_PETAL_SCRIPT; 

} 


int petal_handler(char *petal, int petal_size) {
int retcode = 0;
int line_no = 0;
retcode = petal_interpreter(petal,petal_size,&line_no);
switch(retcode) {
case INVALID_PETAL_STANZA_VER:
printf("PETAL ERROR: Invalid Version at line %i\n",line_no);
break;
case INVALID_PETAL_STANZA_SEQ:
printf("PETAL ERROR: Invalid Sequence at line %i\n",line_no);
break;
case INVALID_PETAL_STANZA_SID:
printf("PETAL ERROR: Invalid Source ID at line %i\n",line_no);
break;
case INVALID_PETAL_STANZA_DID:
printf("PETAL ERROR: Invalid Destination ID at line %i\n",line_no);
break;
case INVALID_PETAL_SCRIPT:
printf("PETAL ERROR: Invalid PETAL script at line %i\n",line_no);
break;
}
return retcode; 

} 
