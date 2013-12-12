# Stuff specific to AVRs but not specific to a particular AVR...

#Include this in the tarball...
COM_MAKE += $(COMDIR)/_make/avrCommon.mk

#if the project has set the OSCCAL register, 
# It's handy to set OSCCAL_SET = TRUE in its makefile
# Then projects which require a calibrated oscillator will
# notify you to do calibration when burning fuses (e.g. for a new chip)
#   Also it'll remove warnings/errors from things like USI_UART...
# where timing is specific
ifdef OSCCAL_SET
 CFLAGS+=-D'_OSCCAL_SET_=$(OSCCAL_SET)'
endif


# MCU-specific make stuff...
include $(COMDIR)/_make/$(MCU).mk
# And, since it's not yet in the mcu files:
COM_MAKE += $(COMDIR)/_make/$(MCU).mk



#TODO:
#include $(suffix $(LIBLIST), .mk)



AVRDUDE = avrdude -c usbtiny -p$(MCU)

# Create a list of fuse commands (this must occur AFTER including MCU.mk): 
# This order mattered at one point... (?)
# -U lfuse:w:$(FUSEL):m -U efuse:w:$(FUSEX):m -U hfuse:w:$(FUSEH):m
FUSE_CMD =
ifdef FUSEL
 FUSE_CMD += -U lfuse:w:$(FUSEL):m
endif
ifdef FUSEX
 FUSE_CMD += -U efuse:w:$(FUSEX):m
endif
ifdef FUSEH
 FUSE_CMD += -U hfuse:w:$(FUSEH):m
endif
ifeq ($(FUSE_NEWCHIP_WARN),TRUE)
 FUSE_NOTICE = "!!!!!!!!!!IMPORTANT: Calibrate the oscillator for a new chip! IMPORTANT!!!!!!!!!"
 ifeq ($(OSCCAL_SET),TRUE)
  FUSE_NOTICE += "!!!THIS PROJECT DEPENDS ON IT! MODIFY OSCCAL AND DON'T FORGET TO RECOMPILE!!!"
 else
	FUSE_NOTICE += " This project doesn't necessarily depend on it (OSCCAL_SET is not TRUE) "
 endif	
else
 FUSE_NOTICE = ""
endif

# Output format. (can be srec, ihex)
FORMAT = ihex


# Optimization level (can be 0, 1, 2, 3, s) 
# (Note: 3 is not always the best optimization level. See avr-libc FAQ)
# This ifndef allows for makefile's predefining it differently...
ifndef OPT
OPT = s
endif


CFLAGS += -D'F_CPU=$(FCPU)'


# WOULDN'T IT MAKE (MORE) SENSE TO PUT THIS IN ASFLAGS?
# (but then it wouldn't be in CFLAGS, is that necessary?)
# This is how it was... we'll just leave it for now.
# Pass on some assembler options. Specifically: create a listing file
# (.lst) (is this useful on a computer?)
#patsubts:
# First, replace a leading occurances of "../.." with $(BUILD_DIR)
# Then, replace the extension .c with .lst
# This works for commonFiles, but not for localFiles...
# What if we strip ../.. (if it exists)
#   Then prefix that with $(BUILD_DIR)?
#  DUH!!!
CFLAGS += -Wa,-ahlms=$(patsubst %.c,%.lst,$(patsubst %,$(BUILD_DIR)/%,$(patsubst $(COMREL)/%,%,$<)))
#-Wa,-ahlms=$(patsubst %.c,%.lst,$(patsubst $(COMREL)/%,$(BUILD_DIR)/%,$<))
#-Wa,-ahlms=$(<:.c=.lst)

# Optional assembler flags.
ASFLAGS = -Wa,-ahlms=$(<:.S=.lst),-gstabs 

# Optional linker flags.
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref

# Additional libraries
# Apparently the min and floating can (and should?) both be included
# (as I have some projects whose makefiles include the floating line
#  which also included this file...)
# This could be ifdef'd...?
#
# Minimalistic printf version
LDFLAGS += -Wl,-u,vfprintf -lprintf_min
#
# Floating point printf version (requires -lm below)
#LDFLAGS +=  -Wl,-u,vfprintf -lprintf_flt
#
# -lm = math library
#LDFLAGS += -lm



# ---------------------------------------------------------------------------

CC = avr-gcc

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump


# Doesn't appear to exist nor to be called...
# (I've never seen a complaint about it not existing, so it mustn't be
#  called, despite its appearing to, below)
#ELFCOFF = objtool

HEXSIZE = avr-size --target=$(FORMAT) $(TARGET).hex
ELFSIZE = avr-size -A $(TARGET).elf



# Combine all necessary flags and optional flags. Add target processor to flags.
ALL_CFLAGS += -mmcu=$(MCU)
ALL_ASFLAGS += -mmcu=$(MCU)


BACKUP_DIR = "backup_$(MCU)_$(shell date '+%Y-%m-%d_%H.%M.%S')"

PROJINFO_TARGET = TRUE

TARGETS = $(TARGET).elf $(TARGET).hex $(TARGET).eep $(TARGET).lss

#Weird... apparently the default one is the first one?
# So by including avrCommon.mk before reallyCommon.mk's targets
# Then this is the default... so call all instead
.PHONY: default
default: all

.PHONY: view
view:
	@$(AVRDUDE) -t [0]<$(COMDIR)/_make/otherFiles/avrdudeFlashDump.txt

#	@$(AVRDUDE) -U flash:r:../readFlash.bin:r
#	@less -f ../readFlash.bin

.PHONY: read
read:
	@mkdir -p $(BACKUP_DIR)
	@$(AVRDUDE) -U flash:r:$(BACKUP_DIR)/flash.hex:i
	@$(AVRDUDE) -U eeprom:r:$(BACKUP_DIR)/eeprom.hex:i

#Just prints out the avrdude command, since I usually forget
.PHONY: avrdude
avrdude:
	@echo "$(AVRDUDE) ..."

.PHONY: halt
halt:
	@$(AVRDUDE) -t

.PHONY: terminal
terminal:
	@$(AVRDUDE) -t
	

#Apparently the .eep file is written regardless of whether data exists in
# The .eeprom section... 	
# Ideally this would only write it if it's non-empty, but I can deal with
# a few extra seconds flashing time, for now...
# This should be fixed now, or soon...
# This ugly parsing checks whether the size is zero...
# It was pretty thoroughly tested with if/else statements and echos...
# My only project with programmed-eeprom isn't at-hand, so I haven't tested
# Yet...
.PHONY: flash
flash:
	$(AVRDUDE) -U flash:w:$(TARGET).hex:i
	@shopt -s extglob ; \
		sizes="`avr-size $(TARGET).eep`" ; \
			 s1="$${sizes%+([[:blank:]])"$(TARGET).eep"}" ; \
			 s2="$${s1#*$$'\n'}" ; \
			 s3="$${s2##+([[:blank:]])}" ; \
			 s4="$${s3##+([[:digit:]])+([[:blank:]])}" ; \
			 s5="$${s4%%+([[:blank:]])*}" ; \
			 if [ "$$s5" != "0" ] ; \
	  			then $(AVRDUDE) -U eeprom:w:$(TARGET).eep:i ; \
	 		 fi
	@echo "****************************************************************"
	@echo "* EEPROM WRITING HASN'T BEEN TESTED SINCE EMPTY-TESTING        *"
	@echo "* (I don't have such a project at-hand... this warning just to *"
	@echo "*  remind me to check next time I have one.)                   *"
	@echo "****************************************************************"
	@echo ""

# Could this be IFed for different MCUs?
.PHONY: fuse
fuse:
	$(AVRDUDE) $(FUSE_CMD)
	@echo $(FUSE_NOTICE)
	@echo ""

.PHONY: reset
reset:
	$(AVRDUDE)


.PHONY: projInfo
projInfo:
	@echo "//Auto-generated by makefile" > projInfo.h
	@echo "" >> projInfo.h
	@echo "#ifndef __PROJINFO_H__" >> projInfo.h
	@echo "#define __PROJINFO_H__" >> projInfo.h
	@echo "#include <inttypes.h>"  >> projInfo.h
	@echo >> projInfo.h
	@echo "#if (!defined(PROJINFO_SHORT) || !PROJINFO_SHORT)" >> projInfo.h
	@echo " uint8_t __attribute__ ((progmem)) \\" >> projInfo.h
	@echo "   header0[] = \" $(PWD) \";" >> projInfo.h
	@echo " uint8_t __attribute__ ((progmem)) \\" >> projInfo.h
	@echo "   header1[] = \" $(shell date) \";" >> projInfo.h
	@echo " uint8_t __attribute__ ((progmem)) \\" >> projInfo.h
	@echo "   headerOpt[] = \" $(PROJ_OPT_HDR) \";" >> projInfo.h
	@echo "#else //projInfo Shortened" >> projInfo.h
	@echo " uint8_t __attribute__ ((progmem)) \\" >> projInfo.h
	@echo "   header[] = \"$(ORIGINALTARGET)$(VER_NUM) $(shell date "+%Y-%m-%d %H:%M:%S")\";" >> projInfo.h
	@echo "#endif" >> projInfo.h
	@echo >> projInfo.h
	@echo "//For internal use..." >> projInfo.h
	@echo "//Currently only usable in main.c" >> projInfo.h
	@echo "#define PROJ_VER $(VER_NUM)" >> projInfo.h
	@echo "#define COMPILE_YEAR $(shell date "+%Y")" >> projInfo.h
	@echo >> projInfo.h
	@echo "#endif" >> projInfo.h
	@echo "" >> projInfo.h


#Note that this is not the same as the similar target in reallyCommon.mk
# This one's $(TARGET).elf that one's $(TARGET)
#This had some strange effects when $(OBJ) was assigned *after* this file
# was included in reallyCommon.mk
# (see notes there)
# echo "$^" was empty, whereas echo "$(OBJ)" was not
.SECONDARY: $(TARGET).elf
.PRECIOUS: $(OBJ)
$(TARGET).elf: $(OBJ)
	@echo "### .elf:OBJ   Linking $@ from $^"
	@$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)



# Target: Convert ELF to COFF for use in debugging / simulating in AVR Studio.

# I've never used this. Further, objtool doesn't exist on my system...
# But I guess I can leave this. See notes at ELFCOFF definition too.
.PHONY: coff
coff: $(TARGET).cof end

%.cof: %.elf
	@echo "### .cof:.elf   Converting $@ from $^ for debugging"
	$(ELFCOFF) loadelf $< mapfile $*.map writecof $@





#MOST files will not have .eepFont
#This was created for audioThing to locate raw data offset by 0x100 in the
# eeprom. It shouldn't cause trouble here, but surely there's a more
# general way to do this... -w (wildcard) is not it, does not work with
# sections, works with *symbols*
#
# It seems like it would work, but it's probably not the best idea...
# Better to put raw data at the beginning of the EEPROM
# And use later addresses for changing data...
# WAS: -R .eeprom -R .eepFont and -j .eeprom and -j .eepFont

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo "### .hex:.elf   Creating Output File $@ from $^"
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo "### .eep:.elf   Creating EEPROM File $@ from $^"
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
# Can create an lss from a .o directly:
#  avr-objdump -h -S file.o > file.lss
#  -z == --disassemble-zeroes
#  (otherwise a slew of nops will give "...")
#    The following generates raw nops rather than a loop
#     (with default optimization options...)
#    for(i=0; i<5; i++) 
#    { asm("nop"); asm("nop"); }
#    This is quite confusing to view, because NONE are shown, nor is the
#    code leading to it.
%.lss: %.elf
	@echo "### .lss:.elf   Creating Extended Listing File $@ from $^"
	$(OBJDUMP) --disassemble-zeroes -h -S $< > $@


