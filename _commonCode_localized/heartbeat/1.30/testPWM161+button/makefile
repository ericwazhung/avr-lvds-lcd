# Stolen from heartbeat/test
# This is identical, except on the PWM161.


# Stolen from sdramThing3.0-0.7
# Stolen from audioThing17


# Having trouble working with inline assembly experiments;
# the assembler is failing with pretty vague errors, and since I'm using
# macros both in cpp-form and asm-form it's hard to determine what exactly
# is responsible for the syntax errors.
# This is supposed to stop after compilation, prior to assembly
#CFLAGS += -S


# This gets modified for compilation to _BUILD/
# if needed after common.mk is included Reference ORIGINALTARGET...
TARGET = heartbeatTest


MCU = at90pwm161


# For testing... heartblink is way too fast...
# for now, just slow the CPU
#ifeq ($(MCU), at90pwm161)
#	FUSEL = 0x62
#endif

#Default FUSE settings (in _commonCode/_make/atmega8515.mk) for
# external clock at 16MHz

MY_SRC = main.c


#Location of the _common directory, relative to here...
# this should NOT be an absolute path...
# COMREL is used for compiling common-code locally...
COMREL = ../../../..
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
# (should no longer be necessary... e.g. "#include _HEARTBEAT_HEADER_")


# There're lots of options like this for code-size
# Also INLINE_ONLY's see the specific commonCode...
#CFLAGS += -D'TIMER_INIT_UNUSED=TRUE'



######## HEARTBEAT SPECIFIC STUFF #########

# Probably best not to enable the WDT while testing/debugging!
WDT_DISABLE = TRUE
HEART_DMS = FALSE
# This is probably best, otherwise the pin is a variable, making it  slower
# and bigger.
CFLAGS += -D'HEARTPIN_HARDCODED=TRUE'
# Heartbeat is on MISO 
#  since the ATtiny can drive up to 40mA it shouldn't affect programming
CFLAGS += -D'HEART_PINNUM=PB6'
CFLAGS += -D'HEART_PINPORT=PORTB'
# The heartbeat LED is connected: PB6 >----|<|---/\/\/\---> V+
# This should be the case 99% of the time, as it also allows the heart-pin
# to be used for a momentary-pushbutton input.
CFLAGS += -D'HEART_LEDCONNECTION=LED_TIED_HIGH'

#For the heartbeat pin being shared with a momentary pushbutton:
# This value is a number of loops to delay in order to wait for the pull-up
# resistors to overcome various capacitances.
# Its maximum value is 255, (which is the default, if not entered here)
# 255 should be *way* higher than necessary, and should pretty much be
# guaranteed to work.
# If you feel so inclined to optimize things, just keep in mind that
# decreasing this value too low will cause heartPinInputPoll() to return 0
# even if the pushbutton isn't activated.
# Further, keep in mind that the value is sensitive to CPU speed, as well
# as external circuitry (e.g. what's the capacitance of that LED?)
CFLAGS += -D'HEART_PULLUP_DELAY=255'
#This should be set to a specific value for your project. E.G. (uncomment):
#VER_HEARTBEAT = 1.30

# For this test-case, though, we'll extract the version-number from the
# current working directory, so that this file needn't be modified with
# each version... (Comment-out this line):
VER_HEARTBEAT = $(notdir $(patsubst %/,%,$(dir $(CURDIR))))

	
HEARTBEAT_LIB = $(COMDIR)/heartbeat/$(VER_HEARTBEAT)/heartbeat
include $(HEARTBEAT_LIB).mk

######## TO HERE ##########




# HEADERS... these are LIBRARY HEADERS which do NOT HAVE SOURCE CODE
# These are added to COM_HEADERS after...
# These are necessary only for make tarball... 
#   would be nice to remove this...
# NOTE: These CAN BE MULTIPLY-DEFINED! Because newer headers almost always 
#    include older headers' definitions, as well as new ones
#   i.e. bithandling...
#  the only way to track all this, for sure, is to hunt 'em all down
#  (or try make tarball and see what's missing after the compilation)
VER_BITHANDLING = 0.95
BITHANDLING_HDR = $(COMDIR)/bithandling/$(VER_BITHANDLING)/
COM_HEADERS += $(BITHANDLING_HDR)
# This is so #include _BITHANDLING_HEADER_ can be used in .c and .h files.
CFLAGS += -D'_BITHANDLING_HEADER_="$(BITHANDLING_HDR)/bithandling.h"'




include $(COMDIR)/_make/reallyCommon2.mk


