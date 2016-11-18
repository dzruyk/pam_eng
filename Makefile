CC=gcc
CFLAGS=-Wall -I./include
BIN=runit
LDFLAGS=-lcairo -lm
OBJS=main.o core/dbuf.o render/render.c math/matrix.o utils/obj_loader.o \
render/surface.c

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm $(BIN) *.o
