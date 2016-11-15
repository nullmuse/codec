static const int OFF_LEN = 75; 
static const int OFF_VERT = 78; 
static const int OFF_SEQ = 81;
static const int OFF_SID = 82;
static const int OFF_DID = 84; 
static const int OFF_PAY = 89; 


typedef struct { 
int version;   
int length; 
int dest_id; 
int source_id; 
int sequence; 
char *payload; 
} psy_data;

 





