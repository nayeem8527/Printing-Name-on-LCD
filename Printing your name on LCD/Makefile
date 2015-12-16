CC=avr-gcc
CFLAGS=-Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
MCU=atmega328p
F_CPU=16000000UL

OBJCOPY=avr-objcopy
BIN_FORMAT=ihex

PORT=/dev/ttyACM0
BAUD=115200
PROTOCOL=arduino
PART=m328p
AVRDUDE=avrdude -F -V

RM=rm -f

all: LCD_FINAL.hex
	sudo $(AVRDUDE) -c$(PROTOCOL) -p $(MCU) -P $(PORT) -b $(BAUD) -D -U flash:w:$<

compile: LCD_FINAL.hex

LCD_FINAL.hex: LCD_FINAL.elf 
	 $(OBJCOPY) -O $(BIN_FORMAT) -R .eeprom $< $@

LCD_FINAL.elf: LCD_FINAL.o 
	 $(CC) $(CFLAGS) -o $@ $<

LCD_FINAL.o: LCD_FINAL.c 
	 $(CC) $(CFLAGS) -c -o $@ $<

upload : 
	sudo $(AVRDUDE) -c $(PROTOCOL) -p $(PART) -P $(PORT) $(BAUD) -U flash:w:blink.hex

clean:
	$(RM) LCD_FINAL.elf LCD_FINAL.hex LCD_FINAL.s LCD_FINAL.o


