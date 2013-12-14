# Modified for reallyCommon2.mk via sdramThing2.0-13

# This gets modified for compilation to _BUILD/
# if needed after common.mk is included Reference ORIGINALTARGET...
TARGET = LCDdirectLVDS

MCU = attiny861

# This is *completely* wrong...
# But it's also unused, I think... (usually for BAUD rates, etc)
# (Maybe it's used by Timer1's OCRC?)
FCPU = 8000000UL

MY_SRC = main.c


CFLAGS += -D'PROJINFO_SHORT=TRUE'

#a/o v59-12ish...
# The PLL can be used for the system clock!
# With OSCCAL at max, we might be able to get a system-clock of up to 32MHz
# Way outta spec, but might be able to get some extra resolution outta it.
# In this case, sysClk = PLL/4, normally PLL = SYSCLK*8
# So, the PWM/LVDS-bit-rate will be half as fast, WRT the system-clock
# (but the same frequency)
# CKSEL3..0 = 0001 -> PLL/4: Nominal Frequency = 16MHz
# (Also potentially useful: CKOUT, bit 6, outputs sysClk on PB5)
# 1, 1, 1, 0, 0, 0, 0, 1
#a/o v59-20ish...
# Works great, implemented in most places necessary for timing
# NOT implemented as far as scaling rowSegs... so all projects will display
# in *half* the screen-width (same height)

# DON'T FORGET: If you change this, rerun 'make fuse'
#PLLSYSCLK = 1

ifdef PLLSYSCLK
FUSEL = 0xe1
# Nominal:
FCPU = 16000000UL
CFLAGS += -D'PLL_SYSCLK=TRUE'
endif

#Slow the whole system to 1MHz (and 8MHz PLL)... CLKDIV8
#CFLAGS += -D'SLOW_EVERYTHING_TEST=TRUE'
# Been here, did this... in LCDdirect (pre LVDS) but we WANTED unrolling
# In any case, it seems to have no effect, and even placing it directly
# in the gcc command, both at the end and the beginning of the flags
# has no effect. no-unroll-loops as well as no-unroll-all-loops
#CGLAGS += -fno-unroll-loops
#HAH! CGLAGS!

# Only run the preprocessor
#CFLAGS += -E
# Display the used headers and other #includes
#CFLAGS += -H
#CFLAGS += -mint8

# Experimenting with Preprocessor output...
# files will still be named .o...
#CFLAGS += -E

# It doesn't make sense to enable this without enabling SLOW_EVERYTHING...
# (does it?)
# And vice-versa
#So SLOW_EVERYTHING enables this...
#CFLAGS += -D'SLOW_LVDS_TEST=TRUE'
#TRUE'

WDT_DISABLE = TRUE
	PROJ_OPT_HDR += WDT_DIS=$(WDT_DISABLE)


#Location of the _common directory, relative to here...
# this should NOT be an absolute path...
# COMREL is used for compiling common-code locally...
COMREL = ../../..
COMDIR = $(COMREL)/_commonCode



################# SHOULD NOT CHANGE THIS BLOCK... FROM HERE ############## 
#                                                                        #
# This stuff has to be done early-on (e.g. before other makefiles are    #
#   included..                                                           #
#                                                                        #
#                                                                        #
# If this is defined, we can use 'make copyCommon'                       #
#   to copy all used commonCode to this subdirectory                     #
#   We can also use 'make LOCAL=TRUE ...' to build from that code,       #
#     rather than that in _commonCode                                    #
LOCAL_COM_DIR = _commonCode_localized
#                                                                        #
#                                                                        #
# If use_LocalCommonCode.mk exists and contains "LOCAL=1"                #
# then code will be compiled from the LOCAL_COM_DIR                      #
# This could be slightly more sophisticated, but I want it to be         #
#  recognizeable in the main directory...                                #
# ONLY ONE of these two files (or neither) will exist, unless fiddled with 
SHARED_MK = __use_Shared_CommonCode.mk
LOCAL_MK = __use_Local_CommonCode.mk
#                                                                        #
-include $(SHARED_MK)
-include $(LOCAL_MK)
#                                                                        #
#                                                                        #
#                                                                        #
#Location of the _common directory, relative to here...                  #
# this should NOT be an absolute path...                                 #
# COMREL is used for compiling common-code into _BUILD...                #
# These are overriden if we're using the local copy                      #
# OVERRIDE the main one...                                               #
ifeq ($(LOCAL), 1)
COMREL = ./
COMDIR = $(LOCAL_COM_DIR)
endif
#                                                                        #
################# TO HERE ################################################





# Common "Libraries" to be included
#  haven't yet figured out how to make this less-ugly...
#DON'T FORGET to change #includes...
#path to the library.c/mk files, including filenames, excluding extensions


VER_ADC = 0.20
CFLAGS += -D'ADC_SUM_REMOVED=TRUE'
ADC_LIB = $(COMDIR)/adc/$(VER_ADC)/adc
include $(ADC_LIB).mk

VER_TIMERCOMMON = 1.21
CFLAGS+=-D'TIMER_SETOUTPUTMODES_UNUSED=TRUE'
CFLAGS+=-D'TIMER_SELECTDIISOR_UNUSED=TRUE'
#CFLAGS+=-D'TIMER_SETWGM_UNUSED=TRUE'
#CFLAGS+=-D'TIMER_COMPAREMATCHINTSETUP_UNUSED=TRUE'
CFLAGS+=-D'TIMER_OVERFLOWINTENABLE_UNUSED=TRUE'
TIMERCOMMON_LIB = $(COMDIR)/timerCommon/$(VER_TIMERCOMMON)/timerCommon
include $(TIMERCOMMON_LIB).mk


#SEE NOTE BELOW IN COM_HEADERS...
HEART_REMOVED = TRUE

VER_HEARTBEAT = 1.21
HEARTBEAT_LIB = $(COMDIR)/heartbeat/$(VER_HEARTBEAT)/heartbeat
HEART_DMS = FALSE
#DMS_EXTERNALUPDATE = FALSE
#override heartBeat's preferred 4s choice...
#CFLAGS += -D'_WDTO_USER_=WDTO_1S'
CFLAGS += -D'HEARTPIN_HARDCODED=TRUE'

#This pinout is... questionable.
# The typical heart-pin is MISO, since the slave device is capable of 
# driving up to 40mA (could easily drive an LED AND the programmer's input)
# But, MISO is an OC1x pin, used by the LCD, in this case...
# Since HEART_REMOVED is true, above, this doesn't matter either way
# BUT, it means we can't use the typical heart-pin as a momentary-low-input
# In other words, this pin is basically unused. See pinout.h
CFLAGS += -D'HEART_PINNUM=PA3'
CFLAGS += -D'HEART_PINPORT=PORTA'
CFLAGS += -D'HEART_LEDCONNECTION=LED_TIED_HIGH'
include $(HEARTBEAT_LIB).mk


VER_SINETABLE = 0.99
SINETABLE_LIB = $(COMDIR)/sineTable/$(VER_SINETABLE)/sineTable
CFLAGS += -D'SINE_DISABLEDOUBLESCALE=TRUE'
SINE_TYPE = 16
CFLAGS+=-D'SINE_RAW8=TRUE'
#CGLAGS += -D'SINE_TABLE_ONLY=TRUE'
include $(SINETABLE_LIB).mk

# HEADERS... these are LIBRARY HEADERS which do NOT HAVE SOURCE CODE
# (That's not entirely true... bithandling has source-code in macro-form.
#  Additionally, this has been hacked a bit to be used for "ncf"
#  aka "not-common-filed" source in _commonCode... meaning these particular
#  bits of code don't have their own makefiles (yet) and are most-likely
#  .c files to be #included in main, etc... it's a hack, it's bad. I know.)
 
# These are added to COM_HEADERS after...
# These are necessary only for 'make tarball'...
# AND POSSIBLY any case where the _commonCode/ stuff is localized...
# (Which would be any case that's downloaded/distributed, etc. Unless 
#  you've created your own _commonCode/ directory somewhere on your system)
#  would be nice to remove this...
# NOTE: These CAN BE MULTIPLY-DEFINED!
#  (That's poorly-worded... It means that COM_HEADERS might include
#   multiple references to different versions of headers...)
#  (E.G. one file may explicitly refer to bithandling 0.94 and another
#   might refer explicitly to 0.93... 
#   So COM_HEADERS might be assigned BOTH:
#      $(COMDIR)/bithandling/0.93/bithandling.h AND
#      $(COMDIR)/bithandling/0.94/bithandling.h
#    in order to make sure that both versions (and more importantly paths)
#    are available in the tarball/localized version.
#   In the vast-majority of cases I have
#    eliminated all #includes that refer to explicit paths, so e.g.
#    most cases #include _BITHANDLING_HEADER_ now, instead of the actual
#    path/file in quotes. This *should* clean things up a bit...
#    Because newer headers almost always include
#     older headers' definitions, as well as new ones
#  the only way to track all this, for sure, is to hunt 'em all down
#  (or try make tarball and see what's missing after the compilation)

# So the above is a bit of explanation for COM_HEADERS, but 
# the CFLAGS += -D'_BITHANDLING_HEADER_=...' stuff is actually necessary
# now regardless of tarballs, etc.

VER_ERRORHANDLING = 0.99
ERRORHANDLING_HDR = $(COMDIR)/errorHandling/$(VER_ERRORHANDLING)/
CFLAGS += -D'_ERRORHANDLING_HEADER_="$(ERRORHANDLING_HDR)/errorHandling.h"'
COM_HEADERS += $(ERRORHANDLING_HDR)
	
VER_BITHANDLING = 0.94
BITHANDLING_HDR = $(COMDIR)/bithandling/$(VER_BITHANDLING)/
# This is so #include _BITHANDLING_HEADER_ can be used in .c and .h files
# It should probably be moved to bithandling.mk
CFLAGS += -D'_BITHANDLING_HEADER_="$(BITHANDLING_HDR)/bithandling.h"'
COM_HEADERS += $(BITHANDLING_HDR)

LCDSTUFF := $(COMDIR)/lcdStuff/0.51ncf-git/lcdStuff.c
COM_HEADERS += $(dir $(LCDSTUFF))
CFLAGS += -D'_LCDSTUFF_CFILE_="$(LCDSTUFF)"'

CHARBITMAP := $(COMDIR)/charBitmap/0.10/charBitmap.h
COM_HEADERS += $(CHARBITMAP)
CFLAGS += -D'_CHARBITMAP_HEADER_="$(CHARBITMAP)"'

# This is a TOTAL hack...
# Apparently I wasn't thinking when creating HEART_REMOVED, which removes
# heartbeat.c from SRC...
# And since SRC is responsible for localizing... we get an issue.
#ifeq ($(HEART_REMOVED),TRUE)
#COM_HEADERS += $(dir $(HEARTBEAT_LIB))
#endif
# Moved to heartbeat.mk

#This is an attempt at trying to figure out whether to use myrm.sh or rm
#MYRMEXISTS = $(shell if [ -e /Users/meh/myexecs/myrm.sh ] ; then echo 1 ; else echo 0 ; fi )
# Moved to reallyCommon2.mk


# Yet another TOTAL HACK...
# Have no idea whether this will work, nor how.
#COM_HEADERS += ../screenshots/
# NOT such a great idea, copies the entire path to screenshots under 
# _commonCode_localized
# Probably need to create a new variable... LOCALIZE_IN_MAINDIR or som'n
# Too much for now...


# USE SPARINGLY! Adding screenshots, or whatnot to this means that a
# download of the distribution (either via git or as a zip) will be huge!
#LOCALIZABLE_OTHERS = ../screenshots/
#OTHERS_DIR = "_otherLocalized"


#.PHONY: timer0calcs
#timer0calcs:
#	@gcc -o timer0Calculator timer0Calculator.c && \
#		./timer0Calculator








include $(COMDIR)/_make/reallyCommon2.mk

