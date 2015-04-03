SRC=cache.c main.c tool.c query.c event.c util.c log.c
HDR=tool.h query.h event.h util.h log.h ../tooltron_mb.h

FLAGS=-O2 -g -Wall -I.. `pkg-config --cflags --libs libmodbus libcurl json-c`

all: tooltron

tooltron: $(SRC) $(HDR)
	gcc $(SRC) $(FLAGS) -o $@

install: tooltron
	cp tooltron /usr/local/bin
	cp tooltron.init /etc/init.d/tooltron
	update-rc.d tooltron defaults

clean:
	rm -f *.o tooltron
