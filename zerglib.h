#include <stdint.h>

static const int OFF_LEN = 75; 
static const int OFF_VERT = 78; 
static const int OFF_SEQ = 81;
static const int OFF_SID = 82;
static const int OFF_DID = 84; 
static const int OFF_PAY = 89; 

typedef struct { 
short version;   
short length; 
short dest_id; 
short source_id; 
short sequence; 
char *payload; 
} psy_data;

 





