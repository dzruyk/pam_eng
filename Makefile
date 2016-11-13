CC=gcc
CFLAGS=-Wall -I./include
BIN=runit
LDFLAGS=-lcairo -lm
OBJS=main.o obj_loader.o dbuf.o matrix.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm $(BIN) *.o
