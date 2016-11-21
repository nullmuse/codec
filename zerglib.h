#define ZERG_MESSAGE  0
#define ZERG_STATUS  1
#define ZERG_COMMAND  2
#define ZERG_GPS  3

#define PSYHDR_SZ 12

#define GET_STATUS 0
#define GOTO 1
#define GET_GPS 2
#define RESERVED 3
#define RETURN 4 
#define SET_GROUP 5 
#define STOP 6 
#define REPEAT 7

/*
static const int OFF_LEN = 81; 
static const int OFF_VERT = 78; 
static const int OFF_SEQ = 89;
static const int OFF_SID = 82;
static const int OFF_DID = 84; 
static const int OFF_PAY = 90; 
*/
static const int OFF_LEN = 85;
static const int OFF_VERT = 82;
static const int OFF_SEQ = 93;
static const int OFF_SID = 86;
static const int OFF_DID = 88;
static const int OFF_PAY = 94;



static const int STAT_HP = 2; 
static const int STAT_AC = 3; 
static const int STAT_HP_MAX = 6;
static const int STAT_TYPE = 7; 
static const int STAT_SP = 8;
static const int STAT_NAME = 12; 

static const int COMM_TYPE = 1; 
static const int COMM_PARAM1 = 2; 
static const int COMM_PARAM2 = 4; 

static const int GPS_LONG = 0; 
static const int GPS_LAT = 8; 
static const int GPS_ALT = 16;
static const int GPS_BEAR = 20; 
static const int GPS_SP = 24; 
static const int GPS_ACC = 28; 

typedef struct { 
short version;
short type;   
short length; 
short dest_id; 
short source_id; 
short sequence; 
char *payload;  
} psy_data;

typedef struct { 
unsigned short t_type:4;
unsigned short t_ver:4;
} type_ver;  



typedef struct { 
char type:4,version:4; 
char length[3]; 
short source_id; 
short dest_id; 
int sequence; 
} zerg_header; 





