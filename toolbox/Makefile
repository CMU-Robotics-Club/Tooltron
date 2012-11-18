PART=t1634
MCU=attiny1634
#MCU=atmega2560
F_CPU=1000000UL
SRC=main.c serial.c rfid.c \
		freemodbus/port/*.c \
		freemodbus/modbus/mb.c \
		freemodbus/modbus/rtu/mbrtu.c \
		freemodbus/modbus/ascii/mbascii.c \
		freemodbus/modbus/functions/*.c
HDR=../tooltron_mb.h serial.h rfid.h freemodbus/port/*.h

FLAGS=-Os -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I.. \
			-DRTS_ENABLE \
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
	avrdude -p $(PART) -c $(PROG) -P usb -B 3 -F -U flash:w:toolbox.hex

clean:
	rm -f *.elf *.hex
