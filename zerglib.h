#define ZERG_MESSAGE  0
#define ZERG_STATUS  1
#define ZERG_COMMAND  2
#define ZERG_GPS  3
#define PSYHDR_SZ 12


static const int OFF_LEN = 81; 
static const int OFF_VERT = 78; 
static const int OFF_SEQ = 89;
static const int OFF_SID = 82;
static const int OFF_DID = 84; 
static const int OFF_PAY = 90; 

static const int STAT_HP = 2; 
static const int STAT_AC = 3; 
static const int STAT_HP_MAX = 6;
static const int STAT_TYPE = 7; 
static const int STAT_SP = 8;
static const int STAT_NAME = 12; 

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

static const char *zerg_breeds[] = {
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






