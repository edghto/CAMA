CC=avr-gcc
LD=$(CC)
CC_PLATFORM=attiny2313
CFLAGS=-c -mmcu=$(CC_PLATFORM) -Os -Wall -std=c99
ASFLAGS=$(CFLAGS) -x assembler-with-cpp -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS=-g -mmcu=$(CC_PLATFORM)

AVR_PLATFORM=t2313
AVR_PROGRAMATOR=usbasp
AVR_PORT=usb
AVRDUDE_FLAGS=-p $(AVR_PLATFORM) -c $(AVR_PROGRAMATOR) -P $(AVR_PORT)

DEPS=*.h
AS_SRC=$(wildcard *.S)
SRC=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRC))
AS_OBJS=$(patsubst %.S, %.o, $(AS_SRC))
TARGET=ethchk

all: elf hex
install: upload

$(OBJS):%.o:%.c
	-@echo $(CC) $(CFLAGS) $< -o $@
	-@$(CC) $(CFLAGS) $< -o $@

$(AS_OBJS):%.o:%.S
	-@echo $(CC) $(ASFLAGS) $< -o $@
	-@$(CC) $(ASFLAGS) $< -o $@

elf: $(OBJS) $(AS_OBJS) $(DEPS)
	-@echo $(LD) $(LDFLAGS) $(OBJS) $(AS_OBJS) -o $(TARGET).elf
	-@$(LD) $(LDFLAGS) $(OBJS) $(AS_OBJS) -o $(TARGET).elf

hex: $(TARGET).elf
	-@echo avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	-@avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex

dump: $(TARGET).elf
	-@echo avr-objdump -h -S $(TARGET).elf > $(TARGET).lst
	-@avr-objdump -h -S $(TARGET).elf > $(TARGET).lst

upload: $(TARGET).hex
	-@echo avrdude $(AVRDUDE_FLAGS) -U flash:w:$(TARGET).hex
	-@avrdude $(AVRDUDE_FLAGS) -U flash:w:$(TARGET).hex

verify:
	-@echo avrdude $(AVRDUDE_FLAGS) -v
	-@avrdude $(AVRDUDE_FLAGS) -v

clean:
	-@echo rm -f *.o
	-@rm -f *.o
	-@echo rm -f *.elf
	-@rm -f *.elf
	-@echo rm -f *.hex
	-@rm -f *.hex
	-@echo rm -f *.lst
	-@rm -f *.lst

