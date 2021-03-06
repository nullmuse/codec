CFLAGS = -Wall -Wextra -pedantic -std=c99
D_OBJS = decode.o cfile.o archon.o templar.o
E_OBJS = encode.o dragoon.o cfile.o arbiter.o templar.o petal.o
all: decode encode

decode: $(D_OBJS)
	$(CC) $(D_OBJS) -o decode

encode: $(E_OBJS)
	$(CC) $(E_OBJS) -o encode

debug: $(D_OBJS) $(E_OBJS)
	$(CC) $(CFLAGS) -g $(D_OBJS) -o decode 
	$(CC) $(CFLAGS) -g $(E_OBJS) -o encode
	

clean:
	rm -f $(D_OBJS) $(E_OBJS) decode encode

