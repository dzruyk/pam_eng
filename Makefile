CC=gcc
CFLAGS=-Wall -I./include
BIN=runit
LDFLAGS=-lcairo -lm
OBJS=main.o utils/obj_loader.o core/dbuf.o math/matrix.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm $(BIN) *.o
