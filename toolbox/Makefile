PART=t1634
MCU=attiny1634
#MCU=atmega2560
F_CPU=8000000UL
SRC=*.c \
		freemodbus/port/*.c \
		freemodbus/modbus/mb.c \
		freemodbus/modbus/rtu/mbrtu.c \
		freemodbus/modbus/ascii/mbascii.c \
		freemodbus/modbus/functions/*.c
HDR=../tooltron_mb.h *.h freemodbus/port/*.h
TOOL_ADDRESS?=-1

FLAGS=-Os -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I.. \
			-DRTS_ENABLE -DTOOL_ADDRESS=$(TOOL_ADDRESS) \
			-Ifreemodbus/port \
			-Ifreemodbus/modbus/rtu \
			-Ifreemodbus/modbus/ascii \
			-Ifreemodbus/modbus/include

PROG=avrispMKII

all: toolbox.hex

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

toolbox.elf: $(SRC) $(HDR)
	avr-gcc $(FLAGS) $(SRC) -o $@

%.asm: %.c
	avr-gcc -S $(FLAGS) $< -o $@

program: toolbox.hex
	avrdude -p $(PART) -c $(PROG) -P usb -B 3 -U flash:w:toolbox.hex

readfuse:
	avrdude -p $(PART) -c $(PROG) -P usb -B 3 -U lfuse:r:-:b -U hfuse:r:-:b -U efuse:r:-:b -U lock:r:-:b

clean:
	rm -f *.elf *.hex
