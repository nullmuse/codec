#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "zerglib.h"


psy_data *transmute_header(char *pdata) {

psy_data *psy = calloc(1,sizeof(psy_data)); 


psy->version = pdata[OFF_VERT]; 
psy->length = pdata[OFF_LEN];
psy->sequence = pdata[OFF_SEQ];
psy->source_id = pdata[OFF_SID];
psy->dest_id = pdata[OFF_DID];

return psy; 
 

} 

 


