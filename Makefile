CFLAGS = -Wall -Wextra -pedantic -std=c99
D_OBJS = decode.o cfile.o archon.o
E_OBJS = encode.o dragoon.o cfile.o
all: decode encode

decode: $(D_OBJS)
	$(CC) $(D_OBJS) -o decode

encode: $(E_OBJS)
	$(CC) $(E_OBJS) -o encode


clean:
	rm -f $(D_OBJS) decode

