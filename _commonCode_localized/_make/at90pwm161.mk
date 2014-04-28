# Sample makefile 
# original by Eric B. Weddington, et al. for WinAVR
# modified for _common "libraries" and other goodies by Eric Hungerford
#
# Maybe someday to be released to the Public Domain
# Please read the make user manual!
#
#
# On command line:
# make = Make software.
# make clean = Clean out built project files.
#
# To rebuild project do "make clean" then "make".
#


# APPARENTLY ORDER-WRITTEN MATTERS
# (old message from another chip...)
# HOWEVER: writing FUSEX does not seem to work... 
# Keeps returning 0x01 which isn't even possible...
# Since the default value is 0xff
# However, all bits except bit0 are unused
# and the default value is fine.

# Usually FUSEX is for the extended HIGH byte, no?
# Either way, for now the default value should be fine.
#FUSEX = 0xff


#Defaults (Read from chip):
# E:FD, H:D9, L:62 -> CLKDIV8

FUSE_NEWCHIP_WARN = TRUE
ifndef FUSEH
FUSEH = 0xd9
endif
# ifndef because FUSEL = 0xe1 -> 16MHz via PLL (why not default this?)
ifndef FUSEL
FUSEL = 0xe2
endif
# Fuses:
# 1 = not-programmed (disabled)

# Fuse extended low byte:
# 0xfd = 1 1 1 1   1 1 0 1 <-- BODLEVEL2
#        \----+----/ ^ ^------ BODLEVEL1
#             |      +-------- BODLEVEL0
#             |
#             +--------------- PSCx I/O reset behavior

# Fuse high byte:
# 0xd9 = 1 1 0 1   1 0 0 1 <-- BOOTRST (reset vector)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE
#        | | | +-------------- WDTON
#        | | +---------------- SPIEN
#        | +------------------ DWEN
#        +-------------------- RSTDISBL

# Fuse low byte:
#        0 1 1 0   0 0 1 0 = DEFAULT (8Mhz internal RC oscillator / 8 ???)
# 0xe2 = 1 1 1 0   0 0 1 0
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (external clock NOT selected)
#        | |  |                   0001 -> 16MHz via PLL
#        | |  +--------------- SUT 1..0 (max start-up time)
#        | +------------------ CKOUT
#        +-------------------- CKDIV8 (Don't divide)


# Could this be IFed for different MCUs?
#.PHONY: fuse
#fuse:
#	$(AVRDUDE) -U lfuse:w:$(FUSEL):m -U efuse:w:$(FUSEX):m -U hfuse:w:$(FUSEH):m
