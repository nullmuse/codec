 #include "zerglib.h"
 
 int validateFile(FILE *fp); 
 char *readFile(char *fname, int ispcap); 
 int writePcap(char *pcapdata,int pcapsize,char *filename);
 int getFileSize(char *fname);
 psyData *transmuteHeader(char *pdata); 
 void readPsyMessage(psyData *psy);
 void readPsyStatus(psyData *psy);
 void readPsyCommand(psyData *psy);
 void readPsyGps(psyData *psy);
 psyData **psionicDivagate(char *stream, int streamsize, int *psyptr);
 int writePcap(char *pcapdata,int pcapsize,char *filename); 
 void initPcap(char *filename);
 char *pcapBuildInit(void);
 void pcapBuildHeader(char *pcapdata, int pcapsize);
 void pcapBuildEth(char *pcapdata);
 void pcapBuildIph(char *pcapdata);
 void pcapBuildUdp(char *pcapdata); 
 zergHeader *transmuteInput(char *pdata);
 char *zergTypeEncoder(zergHeader *zheader, char *data, int *totlen);
 void sizeFixups(char *packet);
 int petalHandler(char *petal, int petalsize);
 int petalInterpreter(char *petal, int petalsize,int *line);
