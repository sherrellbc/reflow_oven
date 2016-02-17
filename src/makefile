blddir         = ../build
PRG            = thermo
OBJ            = main.c usart/usart.c spi/spi.c control/feedback.c pwm/pwm.c control/pid.c
PROGRAMMER     = usbasp
PORT           = usb
MCU_TARGET     = atmega88p 
AVRDUDE_TARGET = atmega88p
OPTIMIZE       = -Os
DEFS           =
LIBS           =
FLAGS          = -Wl,-u,vfprintf -lprintf_flt

HZ          = 16000000

# You should not have to change anything below here.

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        = $(FLAGS) -g -DF_CPU=$(HZ) -std=c99 -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(blddir)/$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(blddir)/$(PRG).elf lst text #eeprom

$(blddir)/$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf           *.o   $(blddir)/$(PRG).elf $(blddir)/*.eps $(blddir)/*.png 
	rm -rf $(blddir)/*.pdf $(blddir)/*.bak      $(blddir)/*.hex $(blddir)/*.bin $(blddir)/*.srec
	rm -rf $(blddir)/*.lst $(blddir)/*.map      

lst:  $(blddir)/$(PRG).lst

$(blddir)/%.lst: $(blddir)/%.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for building the .text rom images

text: hex bin srec

hex:  $(blddir)/$(PRG).hex
bin:  $(blddir)/$(PRG).bin
srec: $(blddir)/$(PRG).srec

$(blddir)/%.hex: $(blddir)/%.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(blddir)/%.srec: $(blddir)/%.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

$(blddir)/%.bin: $(blddir)/%.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

#this has been left untouched as I didn't test anything 
# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec


ehex:  $(PRG)_eeprom.hex
#ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

#%_eeprom.srec: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@


# command to program chip (invoked by running "make install")
install:  $(blddir)/$(PRG).hex
	avrdude -B 1 -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v  \
         -U flash:w:$(blddir)/$(PRG).hex 

fuse:
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v \
	-U lfuse:w:0xc6:m -U hfuse:w:0xd9:m 	

ddd: gdbinit
	ddd --debugger "avr-gdb -x $(GDBINITFILE)"

gdbserver: gdbinit
	simulavr --device $(MCU_TARGET) --gdbserver

gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(blddir)/$(PRG).hex
	@echo "file $(blddir)/$(PRG).elf" > $(GDBINITFILE)

	@echo "target remote localhost:1212" >> $(GDBINITFILE)
	@echo "load"                         >> $(GDBINITFILE)
	@echo "break main"                   >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"
