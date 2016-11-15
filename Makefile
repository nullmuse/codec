CFLAGS = -Wall -Wextra -pedantic
D_OBJS = decode.o cfile.o archon.o

all: decode

decode: $(D_OBJS)
	$(CC) $(D_OBJS) -o decode

