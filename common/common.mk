F_CPU?=8000000UL
MAKE=make
CC=avr-gcc
LD=$(CC)
CC_PLATFORM=attiny2313
INC+=-Iinclude/
CFLAGS=-c -mmcu=$(CC_PLATFORM) -Os -Wall -std=c99 -DF_CPU=$(F_CPU) $(INC)
ASFLAGS=$(CFLAGS) -x assembler-with-cpp -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS=-g -mmcu=$(CC_PLATFORM)

AVR_PLATFORM=t2313
AVR_PROGRAMATOR=usbasp
AVR_PORT=usb
AVRDUDE_FLAGS=-p $(AVR_PLATFORM) -c $(AVR_PROGRAMATOR) -P $(AVR_PORT)

DEPS+=$(LIBS)
DEPS+=$(wildcard include/*.h)
AS_SRC=$(wildcard src/*.S)
SRC=$(wildcard src/*.c)
OBJS=$(patsubst src/%.c, obj/%.o, $(SRC))
AS_OBJS=$(patsubst src/%.S, obj/%.o, $(AS_SRC))
LIB_DEST?=lib

#Building library is always a main target
lib:$(LIB_TARGET)
all: elf hex
install: upload

$(OBJS):obj/%.o:src/%.c
	-@echo $(CC) $(CFLAGS) $< -o $@
	-@$(CC) $(CFLAGS) $< -o $@

$(AS_OBJS):obj/%.o:src/%.S
	-@echo $(CC) $(ASFLAGS) $< -o $@
	-@$(CC) $(ASFLAGS) $< -o $@

#for building lib that this project dependece of
lib%.a:
	-@echo F_CPU=$(F_CPU) LIB_DEST=$(shell pwd) $(MAKE) -C ../$* $@
	-@F_CPU=$(F_CPU) LIB_DEST=$(shell pwd) $(MAKE) -C ../$* $@

$(LIB_TARGET): $(OBJS) $(AS_OBJS) $(LIBS) $(DEPS)
	-@echo ar rcs $(LIB_DEST)/$@ $(OBJS) $(AS_OBJS) $(LIBS)
	-@ar rcs $(LIB_DEST)/$@ $(OBJS) $(AS_OBJS) $(LIBS)

elf: $(OBJS) $(AS_OBJS) $(DEPS)
	-@echo $(LD) $(LDFLAGS) $(OBJS) $(AS_OBJS) $(LIBS) -o $(TARGET).elf
	-@$(LD) $(LDFLAGS) $(OBJS) $(AS_OBJS) $(LIBS) -o $(TARGET).elf

hex: $(TARGET).elf
	-@echo avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	-@avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex

dump: $(TARGET).elf
	-@echo avr-objdump -h -S $(TARGET).elf > $(TARGET).lst
	-@avr-objdump -h -S $(TARGET).elf > $(TARGET).lst

upload: $(TARGET).hex
	-@echo avrdude $(AVRDUDE_FLAGS) -U flash:w:$(TARGET).hex
	-@avrdude $(AVRDUDE_FLAGS) -U flash:w:$(TARGET).hex

fuse:
	-@echo avrdude $(AVRDUDE_FLAGS) -U lfuse:r:-:i
	-@avrdude $(AVRDUDE_FLAGS) -U lfuse:r:-:i

verify:
	-@echo avrdude $(AVRDUDE_FLAGS) -v
	-@avrdude $(AVRDUDE_FLAGS) -v

clean:
	-@echo rm -f $(OBJS) $(LIBS)
	-@rm -f $(OBJS) $(LIBS)
	-@echo rm -f *.elf
	-@rm -f *.elf
	-@echo rm -f *.hex
	-@rm -f *.hex
	-@echo rm -f *.lst
	-@rm -f *.lst

