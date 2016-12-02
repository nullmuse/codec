 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/stat.h>
 #include <string.h>
 #include "pcap.h"
 
 
 int getFileSize(char *fName) {
    struct stat fileInfo;
    int ret;
    if(stat(fName,&fileInfo) == -1) {
              return -1;
       }
 ret = fileInfo.st_size;
 return ret;
 
 }
 
 
 
 int validateFile(FILE *fp) {
    
    int ret = OP_FAIL; 
    char *pCheck; 
    
    
    pCheck = calloc(HEADER_SIZE + 1,sizeof(char)); 
    
    fread(pCheck,sizeof(char),HEADER_SIZE,fp);
    
    if(!strncmp(pCheck,psychicHeader,HEADER_SIZE)) { 
       ret = OP_SUCCESS; 
       }
 
 free(pCheck); 
 return ret; 
 
 } 
 
 
 char *readFile(char *fName, int isPcap) { 
    
    int fSize = 0; 
    char *retData = NULL;
    FILE *fp = NULL; 
    
    fSize = getFileSize(fName);
    if(fSize <= 0) {
             perror("FATAL: is empty file");
             goto RETURN;
             }
 if ((fp = fopen(fName, "rb")) == NULL) {
         perror("FATAL: cannot open file");
         goto RETURN;
    }
 
 if(isPcap) { 
    if(validateFile(fp) == OP_FAIL) {
       perror("FATAL: Not a valid pcap file"); 
       goto RETURN; 
       } 
 rewind(fp); 
 } 
 retData = calloc(fSize + 1, sizeof(char)); 
 
 fread(retData,sizeof(char),fSize,fp); 
 
 
 RETURN:
 if(fp)
 fclose(fp); 
 return retData; 
 }
 
 int writePcap(char *pcapData,int pcapSize,char *fileName) {
    
    int retData = 0;
    FILE *fp;
    if (getFileSize(fileName) == -1) { 
       
       if ((fp = fopen(fileName, "wb+")) == NULL) {
               perror("FATAL: cannot open file");
               goto RETURN;
          }
 }
 else { 
    if ((fp = fopen(fileName, "ab+")) == NULL) {
            perror("FATAL: cannot open file");
            goto RETURN;
       }
 }
 
 fwrite(pcapData, sizeof(char), pcapSize,fp);
 fclose(fp);
 
 
 RETURN:
 return retData;
 
 }
 
 void initPcap(char *fileName) { 
    if (getFileSize(fileName)) {
       remove(fileName); 
       }
 }
 
 
 
 
