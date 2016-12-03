 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/stat.h>
 #include <arpa/inet.h>
 #include "zerglib.h"
 #include "pcap.h"
 #include "templar.h"
 
 
 
 static const char *zergBreeds[] = {
    "Overmind",
    "Larva",
    "Cerebrate",
    "Overlord",
    "Queen",
    "Drone",
    "Zergling",
    "Lurker",
    "Broodling",
    "Hydralisk",
    "Guardian",
    "Scourge",
    "Ultralisk",
    "Mutalisk",
    "Defiler",
    "Devourer"
    };
 
 
 
 
 psyData *transmuteHeader(char *pData) {
    
    typeVer *tv = calloc(1,sizeof(typeVer)); 
    psyData *psy = calloc(1,sizeof(psyData)); 
    short tVer = 0; 
    tVer = pData[OFF_VERT];
    memcpy(tv,&tVer,sizeof(typeVer)); 
    psy->version = tv->tVer; 
    psy->type =  tv->tType; 
    memcpy(&psy->length,&pData[OFF_LEN],sizeof(short) + 1);
    psy->length = byteRitual(psy->length) >> 8;
    memcpy(&psy->sequence,&pData[OFF_SEQ],sizeof(int));
    psy->sequence = byteRitual(psy->sequence);
    psy->sourceId = htons(pData[OFF_SID]) + ((htons(pData[OFF_SID + 1]) >> 8) & 0xFF);
    psy->destId = htons(pData[OFF_DID]) + ((htons(pData[OFF_DID + 1]) >> 8) & 0xFF);
    psy->payload = &pData[OFF_PAY];
    free(tv); 
    return psy; 
    
    
    }
 
 
 void readPsyMessage(psyData *psy) { 
    
    int messageSize; 
    char *message; 
    messageSize = (psy->length) - PSYHDRSZ; 
    message = calloc(messageSize + 1,sizeof(char)); 
    memcpy(message,psy->payload,messageSize); 
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
Message: %s\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId,message);
    
    free(message);
    return;
    }
 
 void readPsyStatus(psyData *psy) { 
    
    int hp,maxHp,type,spp,nameLen;
    char ac;
    float sp;
    char *name;
    nameLen =(psy->length) - PSYHDRSZ; 
    nameLen -= 12; 
    name = calloc(nameLen + 1,sizeof(char));  
    hp = psy->payload[STAT_HP];
    ac = psy->payload[STAT_AC];
    maxHp = psy->payload[STAT_HP_MAX];
    type = psy->payload[STAT_TYPE];
    memcpy(&spp,&psy->payload[STAT_SP],sizeof(int));
    spp = byteRitual(spp); 
    sp = floatRitual(spp);
    memcpy(name,&psy->payload[STAT_NAME],nameLen);
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
Name: %s\n\
HP: %i/%i\n\
Type: %s\n\
Armor: %i\n\
MaxSpeed: %fm/s\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId,name,hp,maxHp,zergBreeds[type],ac,sp);
    
    
    free(name);
    return; 
    
    } 
 
 void readPsyCommand(psyData *psy) { 
    
    int command; 
    int repseq; 
    float bearFloat;
    int bearInt;  
    int meters; 
    signed int groupId;
    short isAdd;  
    char *addRemove[] = {"Remove from","Add to"};
 command = psy->payload[COMM_TYPE];
 switch(command) { 
    
    case GET_STATUS:
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
GET STATUS\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId); 
       break; 
    case GOTO:
    memcpy(&bearInt,&psy->payload[COMM_PARAM2],sizeof(int)); 
    bearInt = byteRitual(bearInt); 
    bearFloat = floatRitual(bearInt); 
    memcpy(&meters,&psy->payload[COMM_PARAM1],sizeof(short));
    meters = ntohs(meters); 
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
GOTO\n\
Orient to Bearing: %f\n\
Distance: %i meters\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId,bearFloat,meters);
    
       break; 
    case GET_GPS:
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
GET GPS\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId);
       break; 
    case RESERVED:
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
RESERVED\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId);
       break;
    case RETURN:
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
RETURN\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId);
       break; 
    case SET_GROUP:
    memcpy(&groupId,&psy->payload[COMM_PARAM2],sizeof(int)); 
    memcpy(&isAdd,&psy->payload[COMM_PARAM1],sizeof(short));
    isAdd = htons(isAdd); 
    groupId = byteRitual(groupId); 
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
SET GROUP\n\
%s Group ID: %i\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId,addRemove[isAdd],groupId);
    
       break; 
    case STOP:
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
STOP\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId);
       break;
    case REPEAT:
    memcpy(&repseq,&psy->payload[COMM_PARAM2],sizeof(int));
    repseq = byteRitual(repseq); 
    printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
REPEAT\n\
Repeat Sequence: %i\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId,repseq);
       break;
    default:
       printf("Unrecognized command in command packet\n"); 
       break; 
    }
 
 
 
 
 return; 
 
 }
 
 
 
 
 void readPsyGps(psyData *psy) { 
    char ns,we; 
    double fLat,fLong;
    float fAlt,fBear,fSp,fAc;
    unsigned int iAlt,iBear,iSp,iAc; 
    unsigned long latL, longL; 
    memcpy(&longL,&psy->payload[GPS_LONG],sizeof(double)); 
    memcpy(&latL,&psy->payload[GPS_LAT],sizeof(double));
    memcpy(&iAlt,&psy->payload[GPS_ALT],sizeof(int));
    memcpy(&iBear,&psy->payload[GPS_BEAR],sizeof(int));
    memcpy(&iSp,&psy->payload[GPS_SP],sizeof(int));
    memcpy(&iAc,&psy->payload[GPS_ACC],sizeof(int));
    latL =longRitual(latL); 
    longL = longRitual(longL); 
    iAlt = byteRitual(iAlt);
    iBear = byteRitual(iBear);
    iSp = byteRitual(iSp);
    iAc = byteRitual(iAc);
    fLat = doubleRitual(latL); 
    fLong = doubleRitual(longL);
    fAlt = floatRitual(iAlt);
    fBear = floatRitual(iBear);
    fSp = floatRitual(iSp); 
    fAc = floatRitual(iAc);  
    fAlt = fAlt * FATHOM_METERS; 
    if(fLong < 0.0) { 
       we = 'W';
       fLong *= -1; 
       }
 else if(fLong >= 0.0) {
    we = 'E'; 
    }
 if(fLat < 0.0) { 
    ns = 'S';
    fLat *= -1; 
    }
 else if(fLat >= 0.0) { 
    ns = 'N'; 
    }
 
 printf("\
Version: %i\n\
Sequence: %i\n\
From: %i\n\
To: %i\n\
Latitude: %2.9f deg. %c\n\
Longitude: %2.9f deg. %c\n\
Altitude: %2.1fm\n\
Bearing: %2.9f deg.\n\
Speed: %.0fm/s\n\
Accuracy: %.0fm\n\
",psy->version,psy->sequence,psy->sourceId,psy->destId,fLat,ns,fLong,we,fAlt,fBear,fSp,fAc);
 
 
 
 return; 
 
 } 
 
 /*
 int psionicDivagate(char *stream, int streamsize, psyData **psylist) {
    
    int i,psyCount;
    char *tokPtr;
    
    
    tokPtr = calloc(HEADERSIZE + 1,sizeof(char));
    psyCount = 0;
    for(i = 0; i < streamSize; ++i) {
       memcpy(tokPtr,(stream + i),sizeof(int));
       if(!strncmp(tokPtr,psychicheader,HEADERSIZE)) {
          if(psyCount == 0) {
             psyList[psyCount] = transmuteheader((stream + i));
             psyCount++;
             }
 else if(*psyList != NULL) {
    psyList = realloc(psylist,sizeof(psyData *)); 
    psyList[psyCount] = transmuteheader(stream + i);
    psyCount++;
    }
 }
 }
 return psyCount;
 }
 */
  psyData **psionicDivagate(char *stream, int streamSize, int *psyPtr) {
    int psyCount,i;
    char *pPoint;
    psyData **psyList;
    psyData **psyTemp; 
    psyData *headerPointer; 
    int packetLength = 0;
    pPoint = stream + OFF_EPOCH; 
    psyCount = 0;
    long max = (long) stream + streamSize; 
    while((long)pPoint < max) {
       packetLength = *(pPoint + 8);
       pPoint += 16; 
       if(psyCount == 0) {
          psyList = calloc(psyCount + 1,sizeof(psyData *));
          headerPointer = transmuteHeader(pPoint);
          memcpy(&psyList[psyCount],&headerPointer,sizeof(psyData *)); 
          psyCount++; 
          }
 else {
    psyTemp = calloc(psyCount + 1,sizeof(psyData *));
    for(i = 0;i < psyCount;i++) { 
       memcpy(&psyTemp[i],&psyList[i],sizeof(psyData *));
       }
 free(psyList);
 psyList = psyTemp;
 headerPointer = transmuteHeader(pPoint);
 memcpy(&psyList[psyCount],&headerPointer,sizeof(char *));
 psyCount++;
 }
 
 pPoint += packetLength; 
 }
 memcpy(psyPtr,&psyCount,sizeof(int)); 
 return psyList; 
 }
