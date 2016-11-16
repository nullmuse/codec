#define ZERG_MESSAGE  0
#define ZERG_STATUS  1
#define ZERG_COMMAND  2
#define ZERG_GPS  3



static const int OFF_LEN = 81; 
static const int OFF_VERT = 78; 
static const int OFF_SEQ = 89;
static const int OFF_SID = 82;
static const int OFF_DID = 84; 
static const int OFF_PAY = 89; 


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






