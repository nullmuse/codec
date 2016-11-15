CFLAGS = -Wall -Wextra -pedantic
D_OBJS = cdecode.o cfile.o

all: decode

decode:$(D_OBJS)
	$(CC) $(CFLAGS) $(D_OBJS) -o decode

