CC=gcc
CFLAGS=-Wall -I./include -ggdb
BIN=runit
LDFLAGS=-lcairo -lm
OBJS=main.o core/dbuf.o render/render.o math/matrix.o utils/objloader.o \
render/surface.o utils/mesh.o utils/camera.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm $(BIN) $(OBJS)
