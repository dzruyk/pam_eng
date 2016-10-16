CFLAGS=-Wall
BIN=runit
LDFLAGS=-lcairo

all: $(BIN)

$(BIN): main.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm $(BIN) *.o
