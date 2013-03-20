SRC=main.c tool.c query.c event.c util.c
HDR=tool.h query.h event.h util.h ../tooltron_mb.h

FLAGS=-O2 -g -Wall -I.. `pkg-config --cflags --libs libmodbus libcurl`

all: tooltron

tooltron: $(SRC) $(HDR)
	gcc $(SRC) $(FLAGS) -o $@

install: tooltron
	cp tooltron /usr/local/bin

clean:
	rm -f *.o tooltron
