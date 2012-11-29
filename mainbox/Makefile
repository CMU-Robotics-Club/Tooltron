SRC=main.c tool.c
HDR=tool.h ../tooltron_mb.h

FLAGS=-O2 -g -Wall -I.. `pkg-config --cflags --libs libmodbus`

all: tooltron

tooltron: $(SRC) $(HDR)
	gcc $(SRC) $(FLAGS) -o $@

install: tooltron
	cp tooltron /usr/local/bin

clean:
	rm -f *.o tooltron
