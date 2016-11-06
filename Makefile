CC=gcc
CFLAGS=-Wall -I./include
BIN=runit
LDFLAGS=-lcairo
OBJS=main.o obj_loader.o vector.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm $(BIN) *.o
