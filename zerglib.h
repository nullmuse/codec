 #define ZERG_MESSAGE  0
 #define ZERG_STATUS  1
 #define ZERG_COMMAND  2
 #define ZERG_GPS  3
 
 #define PSYHDRSZ 12
 
 #define GET_STATUS 0
 #define GOTO 1
 #define GET_GPS 2
 #define RESERVED 3
 #define RETURN 4 
 #define SET_GROUP 5 
 #define STOP 6 
 #define REPEAT 7
 
 /*
 static const int OFFLEN = 83;
 static const int OFFVERT = 82;
 static const int OFFSEQ = 90;
 static const int OFFSID = 86;
 static const int OFFDID = 88;
 static const int OFFPAY = 94;
 */
 
 static const int OFF_LEN = 43;
 static const int OFF_VERT = 42;
 static const int OFF_SEQ = 50;
 static const int OFF_SID = 46;
 static const int OFF_DID = 48;
 static const int OFF_PAY = 54;
 
 
 
 static const int STAT_HP = 2; 
 static const int STAT_AC = 3; 
 static const int STAT_HP_MAX = 6;
 static const int STAT_TYPE = 7; 
 static const int STAT_SP = 8;
 static const int STAT_NAME = 12; 
 
 static const int COMM_TYPE = 1; 
 static const int COMM_PARAM1 = 2; 
 static const int COMM_PARAM2 = 4; 
 
 
 static const float FATHOM_METERS = 1.8288;
 static const int GPS_LONG = 0; 
 static const int GPS_LAT = 8; 
 static const int GPS_ALT = 16;
 static const int GPS_BEAR = 20; 
 static const int GPS_SP = 24; 
 static const int GPS_ACC = 28; 
 
 typedef struct { 
    short version;
    short type;   
    int length; 
    short destId; 
    short sourceId; 
    int sequence; 
    char *payload;  
    } psyData;
 
 typedef struct { 
    unsigned short tType:4;
    unsigned short tVer:4;
    } typeVer;  
 
 
 
 typedef struct { 
    char type:4,version:4; 
    char length[3]; 
    short sourceId; 
    short destId; 
    int sequence; 
    } zergHeader; 
 
 struct zergStat{ 
    char hp[3];
    char ac;
    char maxHp[3];
    char type;
    char maxSpeed[4];
    };  
 
 struct zergGps{ 
    char lat[8];
    char lon[8];
    char alt[4];
    char bearing[4];
    char speed[4];
    char accuracy[4];
    };
 
 struct zergCommand{
    short command;
    char param1[2];
    char param2[4];
    };
 
 
 
 
