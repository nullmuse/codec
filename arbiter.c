#define _GNU_SOURCE
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/stat.h> 
#include <arpa/inet.h> 
#include "zerglib.h"
#include "pcap.h"
#include "templar.h"

static
const char *zergBreeds[] = {
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

static const char *ZERG_COMMANDS[] = {
    "GET STATUS",
    "GOTO",
    "GET GPS",
    "RESERVED",
    "RETURN",
    "SET GROUP",
    "STOP",
    "REPEAT"
};

zergHeader *
transmuteInput(
char * pData) 
{

   char *strs[] = {
        "Version:",
        "Sequence",
        "From",
        "To"
    };

    char *pTok;
    char *dataPoint;
    char **savePtr = &pData;
    zergHeader *zHeader = calloc(1, sizeof(zergHeader));
    pTok = strtok_r(pData, "\n", savePtr);
    if (!strncmp(pTok, strs[0], strlen(strs[0]))) 
    {
        dataPoint = strstr(pTok, " ");
        if (dataPoint != NULL) 
        {
            zHeader->version = transmuteChar(dataPoint + 1);
        }
    }

    pTok = strtok_r(NULL, "\n", savePtr);
    if (!strncmp(pTok, strs[1], strlen(strs[1]))) 
    {
        dataPoint = strstr(pTok, " ");
        if (dataPoint != NULL) 
        {
            zHeader->sequence=transmuteChar(dataPoint + 1);
        }
    }

    pTok = strtok_r(NULL, "\n", savePtr);
    if (!strncmp(pTok, strs[2], strlen(strs[2]))) 
    {
        dataPoint = strstr(pTok, " ");
        if (dataPoint != NULL) 
        {
            zHeader->sourceId = transmuteChar(dataPoint + 1);
        }
    }

    pTok = strtok_r(NULL, "\n", savePtr);
    if (!strncmp(pTok, strs[3], strlen(strs[3]))) 
    {
        dataPoint = strstr(pTok, " ");
        if (dataPoint != NULL) 
        {
            zHeader->destId = transmuteChar(dataPoint + 1);
        }
    }
    pTok = strtok_r(NULL, "\n", savePtr);
    zHeader->length[0] = 0;
    zHeader->length[1] = 0;
    zHeader->length[2] = 0;
    switch(pTok[0]) 
    {
        case 'M':
            zHeader->type = 0;
            break;
        case 'N':
            zHeader->type = 1;
            break;
        case 'L':
           zHeader->type = 3;
           break;
        default:
           zHeader->type = 2;

    }
    return zHeader;

}

char *
zergTypeEncoder(
    zergHeader *zHeader,
    char *data,
    int *totLen)
{

    char *hps;
    char *arm;
    char *floatStr;
    int floatHolder;
    long doubleHolder;
    int hp, nameLen;
    float fCont;
    double dCont;
    char *payload;
    char *name;
    char *paramStr;
    short paramShort;
    int paramInt;
    char *pPoint;
    struct zergStat zerg;
    struct zergGps gps;
    struct zergCommand command;
    int i, k, length, copyLen;
    switch (zHeader->type)
    {
        case 0:
            for (i = 0; data[i] != 'M'; ++i) {}
            payload = &data[i];
            pPoint = payload + 9;
            for (i = 0; pPoint[i] != 0; ++i) {}
            length = i;
            copyLen = length + PSYHDRSZ;
            payload = calloc(copyLen, sizeof(char));
            zHeader->length[0] = copyLen >> 16;
            zHeader->length[1] = copyLen >> 8;
            zHeader->length[2] = copyLen;
            zHeader->destId = htons(zHeader->destId);
            zHeader->sourceId = htons(zHeader->sourceId);
            zHeader->sequence = htonl(zHeader->sequence);
            memcpy(payload, zHeader, sizeof(zergHeader));
            memcpy(payload + PSYHDRSZ, pPoint, length);
            *totLen = copyLen;
            break;

        case 1:
            for (i = 0; data[i] != 'N'; ++i) {}
            pPoint = & data[i];
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != 0; ++i) {}
            nameLen = i;
            name = pPoint;
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != '/'; ++i) {}
            hps = calloc(i + 1, sizeof(char));
            memcpy(hps, pPoint, i);
            hp = transmuteChar(hps);
            zerg.hp[0] = hp >> 16;
            zerg.hp[1] = hp >> 8;
            zerg.hp[2] = hp;
            pPoint = &pPoint[i] + 1;
            for (i = 0; pPoint[i] != 0xa; ++i) {}
            memcpy(hps, pPoint, i);
            hp = transmuteChar(hps);
            zerg.maxHp[0] = hp >> 16;
            zerg.maxHp[1] = hp >> 8;
            zerg.maxHp[2] = hp;
            free(hps);
            while ( *pPoint != 0 && *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != 0xa; ++i) {}
            for (k = 0; k < 16; ++k)
            {
                if (!strncmp(pPoint, zergBreeds[k], i))
                {
                    zerg.type = k;
                    break;
                }
            }

            while ( *pPoint != 0 && *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != 0xa; ++i) {}
            arm = calloc(i + 1, sizeof(char));
            memcpy(arm, pPoint, i);
            zerg.ac = transmuteChar(arm);
            free(arm);
            while ( *pPoint != 0 && *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != 'm'; ++i) {}
            floatStr = calloc(i + 1, sizeof(char));
            memcpy(floatStr, pPoint, i);
            copyLen = PSYHDRSZ + nameLen + sizeof(struct zergStat);
            fCont = atof(floatStr);
            memcpy( &floatHolder, &fCont, sizeof(int));
            zerg.maxSpeed[3] = floatHolder;
            zerg.maxSpeed[2] = floatHolder >> 8;
            zerg.maxSpeed[1] = floatHolder >> 16;
            zerg.maxSpeed[0] = floatHolder >> 24;
            free(floatStr);
            payload = calloc(copyLen, sizeof(char));
            zHeader->length[0] = copyLen >> 16;
            zHeader->length[1] = copyLen >> 8;
            zHeader->length[2] = copyLen;
            zHeader->destId = htons(zHeader->destId);
            zHeader->sourceId = htons(zHeader->sourceId);
            zHeader->sequence = htonl(zHeader->sequence);
            memcpy(payload, zHeader, sizeof(zergHeader));
            memcpy((payload + PSYHDRSZ), &zerg, sizeof(struct zergStat));
            memcpy((payload + PSYHDRSZ + sizeof(struct zergStat)), name, nameLen);
            *totLen = copyLen;
            break;

        case 2:
            for (i = 0; data[i] != 'T'; ++i) {}
            for (; data[i] != 0xa && data[i] != 0; ++i) {}
            pPoint = &data[i];
            pPoint++;
            for (i = 0; pPoint[i] != 0xa && pPoint[i] != 0; ++i) {}
            for (k = 0; k < 8; ++k)
            {
                if (!strncmp(pPoint, ZERG_COMMANDS[k], i))
                {
                    command.command = htons(k);
                    break;
                }
            }

            switch (k)
            {
                case 1:
                    pPoint += (i + 1);
                    while ( *pPoint != ':')
                    {
                        pPoint++;
                    }
                    pPoint += 2;
                    for (i = 0; pPoint[i] != 0xa; ++i) {}
                    floatStr = calloc(i + 1, sizeof(char));
                    memcpy(floatStr, pPoint, i);
                    fCont = atof(floatStr);
                    memcpy( &floatHolder, &fCont, sizeof(int));
                    command.param2[3] = floatHolder;
                    command.param2[2] = floatHolder >> 8;
                    command.param2[1] = floatHolder >> 16;
                    command.param2[0] = floatHolder >> 24;
                    free(floatStr);
                    while ( *pPoint != ' ')
                    {
                        pPoint++;
                    }
                    pPoint++;
                    for (i = 0; pPoint[i] != 0xa; ++i) {}
                    paramStr = calloc(i + 1, sizeof(char));
                    memcpy(paramStr, pPoint, i);
                    paramShort = atoi(paramStr);
                    command.param1[1] = paramShort;
                    command.param1[0] = paramShort >> 8;
                    free(paramStr);
                    break;

                case 5:
                    pPoint += (i + 1);
                    if ( *pPoint == 'A')
                    {
                        command.param1[1] = 1;
                        command.param1[0] = 0;

                    }
                    else
                    {
                        command.param1[1] = 0;
                        command.param1[0] = 0;
                    }
                    while (* pPoint != ':')
                    {
                        pPoint++;
                    }
                    pPoint += 2;
                    for (i = 0; pPoint[i] != 0xa; ++i) {}
                    paramStr = calloc(i + 1, sizeof(char));
                    memcpy(paramStr, pPoint, i);
                    paramInt = atoi(paramStr);
                    command.param2[3] = paramInt;
                    command.param2[2] = paramInt >> 8;
                    command.param2[1] = paramInt >> 16;
                    command.param2[0] = paramInt >> 24;
                    free(paramStr);
                    break;

                case 7:
                    command.param1[0] = 0;
                    command.param1[1] = 0;
                    while ( *pPoint != ':')
                    {
                        pPoint++;
                    }
                    pPoint += 2;
                    for (i = 0; pPoint[i] != 0xa; ++i) {}
                    paramStr = calloc(i + 1, sizeof(char));
                    memcpy(paramStr, pPoint, i);
                    paramInt = atoi(paramStr);
                    command.param2[3] = paramInt;
                    command.param2[2] = paramInt >> 8;
                    command.param2[1] = paramInt >> 16;
                    command.param2[0] = paramInt >> 24;
                    free(paramStr);
                    break;
                default:
                    command.param1[0] = 0;
                    command.param1[1] = 0;
                    command.param2[3] = 0;
                    command.param2[2] = 0;
                    command.param2[1] = 0;
                    command.param2[0] = 0;
                    break;
            }
            copyLen = PSYHDRSZ + sizeof(struct zergCommand);
            payload = calloc(copyLen, sizeof(char));
            zHeader->length[0] = copyLen >> 16;
            zHeader->length[1] = copyLen >> 8;
            zHeader->length[2] = copyLen;
            zHeader->destId = htons(zHeader->destId);
            zHeader->sourceId = htons(zHeader->sourceId);
            zHeader->sequence = htonl(zHeader->sequence);
            memcpy(payload, zHeader, sizeof(zergHeader));
            memcpy((payload + sizeof(zergHeader)), &command, sizeof(struct zergCommand));
            *totLen = copyLen;
            break;

        case 3:
            for (i = 0; data[i] != 'L'; ++i) {}
            pPoint = &data[i];
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != ' '; ++i) {}
            floatStr = calloc(i + 1, sizeof(char));
            memcpy(floatStr, pPoint, i);
            dCont = atof(floatStr);
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            pPoint += 5;
            if ( *pPoint > 0x4e)
            {
                dCont = dCont * -1;
            }
            memcpy( &doubleHolder, &dCont, sizeof(long));
            gps.lon[7] = doubleHolder;
            gps.lon[6] = doubleHolder >> 8;
            gps.lon[5] = doubleHolder >> 16;
            gps.lon[4] = doubleHolder >> 24;
            gps.lon[3] = doubleHolder >> 32;
            gps.lon[2] = doubleHolder >> 40;
            gps.lon[1] = doubleHolder >> 48;
            gps.lon[0] = doubleHolder >> 56;
            free(floatStr);
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != ' '; ++i) {}
            floatStr = calloc(i + 1, sizeof(char));
            memcpy(floatStr, pPoint, i);
            dCont = atof(floatStr);
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            pPoint += 5;
            if ( *pPoint > 0x4e)
            {
                dCont = dCont * -1;
            }
            memcpy( &doubleHolder, &dCont, sizeof(long));
            gps.lat[7] = doubleHolder;
            gps.lat[6] = doubleHolder >> 8;
            gps.lat[5] = doubleHolder >> 16;
            gps.lat[4] = doubleHolder >> 24;
            gps.lat[3] = doubleHolder >> 32;
            gps.lat[2] = doubleHolder >> 40;
            gps.lat[1] = doubleHolder >> 48;
            gps.lat[0] = doubleHolder >> 56;
            free(floatStr);
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != 'm'; ++i) {}
            floatStr = calloc(i + 1, sizeof(char));
            memcpy(floatStr, pPoint, i);
            fCont = atof(floatStr);
            fCont /= FATHOM_METERS;
            memcpy( &floatHolder, &fCont, sizeof(int));
            gps.alt[3] = floatHolder;
            gps.alt[2] = floatHolder >> 8;
            gps.alt[1] = floatHolder >> 16;
            gps.alt[0] = floatHolder >> 24;
            free(floatStr);
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != ' '; ++i) {}
            floatStr = calloc(i + 1, sizeof(char));
            memcpy(floatStr, pPoint, i);
            fCont = atof(floatStr);
            memcpy( &floatHolder, &fCont, sizeof(int));
            gps.bearing[3] = floatHolder;
            gps.bearing[2] = floatHolder >> 8;
            gps.bearing[1] = floatHolder >> 16;
            gps.bearing[0] = floatHolder >> 24;
            free(floatStr);
            while ( *pPoint != 0xa)
            {
                pPoint++;
            }
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != 'm'; ++i) {}
            floatStr = calloc(i + 1, sizeof(char));
            memcpy(floatStr, pPoint, i);
            fCont = atof(floatStr);
            memcpy( &floatHolder, &fCont, sizeof(int));
            gps.speed[3] = floatHolder;
            gps.speed[2] = floatHolder >> 8;
            gps.speed[1] = floatHolder >> 16;
            gps.speed[0] = floatHolder >> 24;
            free(floatStr);
            while ( *pPoint != ' ')
            {
                pPoint++;
            }
            pPoint++;
            for (i = 0; pPoint[i] != 'm'; ++i) {}
            floatStr = calloc(i + 1, sizeof(char));
            memcpy(floatStr, pPoint, i);
            fCont = atof(floatStr);
            memcpy( &floatHolder, &fCont, sizeof(int));
            gps.accuracy[3] = floatHolder;
            gps.accuracy[2] = floatHolder >> 8;
            gps.accuracy[1] = floatHolder >> 16;
            gps.accuracy[0] = floatHolder >> 24;
            free(floatStr);
            copyLen = PSYHDRSZ + sizeof(struct zergGps);
            payload = calloc(copyLen, sizeof(char));
            zHeader->length[0] = copyLen >> 16;
            zHeader->length[1] = copyLen >> 8;
            zHeader->length[2] = copyLen;
            zHeader->destId = htons(zHeader->destId);
            zHeader->sourceId = htons(zHeader->sourceId);
            zHeader->sequence = htonl(zHeader->sequence);
            memcpy(payload, zHeader, sizeof(zergHeader));
            memcpy((payload + sizeof(zergHeader)), &gps, sizeof(struct zergGps));
            *totLen = copyLen;
            break;

        default:
            printf("Error: Unrecognized packet type\n");
            payload = NULL;
            break;
    }

    return payload;
}

void
sizeFixups(
char *packet)
{
    int zergLength;
    short udpLength = UDP_SIZE;
    short ipLength = IP_SIZE;
    int packetLength = ETH_SIZE + UDP_SIZE + IP_SIZE;
    char shortT[2];
    memcpy( &zergLength, &packet[OFF_LEN + 40], sizeof(short) + 1);
    zergLength = byteRitual(zergLength) >> 8;
    udpLength += zergLength;
    shortT[1] = udpLength;
    shortT[0] = udpLength >> 8;
    memcpy( &packet[OFF_UDP + 4], &shortT, sizeof(short));
    ipLength += udpLength;
    shortT[1] = ipLength;
    shortT[0] = ipLength >> 8;
    memcpy( &packet[OFF_IP + 2], &shortT, sizeof(short));
    packetLength += zergLength;
    memcpy( &packet[OFF_EPOCH + 8], &packetLength, sizeof(int));
    memcpy( &packet[OFF_EPOCH + 12], &packetLength, sizeof(int));
}
