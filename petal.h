 #define VALID_PETAL_SCRIPT 0
 #define INVALID_PETAL_STANZA_VER 1
 #define INVALID_PETAL_STANZA_SEQ 2
 #define INVALID_PETAL_STANZA_SID 3
 #define INVALID_PETAL_STANZA_DID 4
 #define INVALID_PETAL_SCRIPT -1
 
 
 static const char *messageStanza[] = {"Message: "}; 
 static const char *statusStanza[] = {"Name: ","HP: ","Type: ","Armor: ","MaxSpeed: "}; 
 static const char *gpsStanza[] = {"Latitude: ","Longitude: ","Altitude: ","Bearing: ","Speed: ","Accuracy: "};  
 static const char *zergCommands[] = {"GET STATUS","GOTO","GET GPS","RESERVED","RETURN","SET GROUP","STOP","REPEAT"};
 
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

