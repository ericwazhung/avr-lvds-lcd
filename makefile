#/* mehPL:
# *    This is Open Source, but NOT GPL. I call it mehPL.
# *    I'm not too fond of long licenses at the top of the file.
# *    Please see the bottom.
# *    Enjoy!
# */
#
#
#
#
#
#
#
#
#
#
#

# a/o v92: (copy-over from v66.51-64 a/o v91):
#
# VERSION 66 WAS WRITTEN PRIOR TO AVR-GCC 4.8
# (which requires "const" before PROGMEM, etc)
# ALSO: gcc4.8 appears to do some weird stuff that 4.4 didn't...
# like changing ((n*7)/4) to mult/div function-calls
# whereas 4.4. apparently did ((n<<3)-n)>>2
# which was *MUCH* faster
# See notes in Delay_Dots()
#
# Otherwise, this may be useful for later testing, so left here
# Commented-out is 4.8 (on my system)
# Uncommented is 4.4 (on my system)
#CC = /usr/local/bin/avr-gcc
#





#This bit (.mainConfig.preprocessed.mk) handles making #defines in
#mainConfig.h available to make for conditional inclusion...
# I guess it's sort of like an autoconfigure script
# that automatically runs each time mainConfig.h is modified.

#This is added to the default 'make' target in reallyCommon2.mk
# See the target at the bottom of this file.
#This is unnecessary, see 'info make' "3.5 How Makefiles Are Remade"
#PROJ_PREMAKE=.mainConfig.preprocessed.mk
#-include this so we can at least get to create it if nonexistent
# But otherwise it should definitely be included.

#BEWARE: Only certain thingamagigs are usable... 
# e.g. #define MAIN_MS TCNTER_MS but TCNTER_MS isn't defined in mainConfig
# nor its inclusions... so MAIN_MS == TCNTER_MS but NOT the *value* of
# TCNTER_MS
include .mainConfig.preprocessed.mk




# This makefile scheme is highly-dependent on the inclusion of various
# other makefiles, including _commonCode.../_make/reallyCommon2.mk
# (Modified for reallyCommon2.mk via sdramThing2.0-13)

#These don't handle all cases, and are in commonWarnings.mk, now, anyhow?
CFLAGS += -Werror=uninitialized
CFLAGS += -Werror=maybe-uninitialized

# Uncomment this for macroExpander.sh
# (Currently located in cTools/macroExpansions/macroExpander.sh)
# Actually, use macroSearch.sh, instead. It's *way* faster, and more
# functional.
#CFLAGS += -E -dM

###### Listed Early-On because THIS IS BAD if it's TRUE
#CFLAGS += -D'ALLOW_UNIMPLEMENTED=TRUE'
###### In other words, this should be commented-out


#####
# As-Of installation of avr-libc 1.8, avr-gcc 4.82, etc. 4-20-14:
# Message about "const" being necessary with PROGMEM...
# From avr-libc's "Summary of changes in 1.8.html"
# This might have something to do with it...
# "all the type names starting with prog_ (like prog_char, prog_uint16_t
# and so on) are now no longer declared by default."
CFLAGS += -D'__PROG_TYPES_COMPAT__=TRUE'
# Or not... seems to have no effect on this error:
#error: variable ‘sineTable’ must be const in order to be put into 
#read-only section by means of ‘__attribute__((progmem))’
#  theta_t sineTable[129] PROGMEM = {
# But it should be helpful in other such cases... for now.
#####


#This is the project-name... the compiled (e.g. hex) files will have this
# as their filename (e.g. _BUILD/<TARGET>.hex)
#
# This variable ("TARGET") gets modified in an included .mk file, later
# down the line (for compilation to '_BUILD/').
# If you need to refer to this value, refer to ORIGINALTARGET instead.
# (This hasn't been tested, but should work anywhere)
# In other words, you're best NOT referring to TARGET directly anywhere...
TARGET = LCDdirectLVDS

#This is the selected AVR... Used in MANY cases, including configuration of
# avr-dude, avr-gcc, and more.
#MCU = at_dneTest
#MCU = at90pwm161
MCU = attiny861

# This is the CPU frequency. Generally it's used for calculating BAUD
# rates, and timer compare-match values. In C/H code, its value can be
# referenced via the preprocessor macro F_CPU
#
# HERE: This value is *completely* wrong...
# But it's also unused, I think...
# (Maybe it's used by Timer1's OCRC?)
FCPU = 8000000UL


# This is the project-specific source-code to be compiled.
# This does *not* include code found in _commonCode.../
#  (That code is added automagically, when properly implemented)
# NOTE: This particular project isn't particularly well-implemented in the
# c/h-file domain... main.c literally #includes many other .c files.
# This isn't good-practice, as, among other things, it leads to quite a bit
# of confusion regarding the proper organization of the #includes. 
# But it also allows for inline code that gets highly-optimized, prevents
# compilation of functions that are never referenced, etc.
#  (There may be a way around this, and I don't deny a bit of laziness in
#  trying to understand that scheme merely for the sake of trying to avoid
#  #inclusion of .c files. But the #inclusion organization issue is darned
#  frustrating at times, so I should TODO this).
MY_SRC = main.c


# This can be commented-out as desired.
# projInfo.h is generated automatically during "make"
# This file contains header-information which is viewable whenever reading
# the chip-contents... (e.g. "make view")
# When program-memory isn't lacking, it's nice to be able to see what
# project/version is loaded in the chip (especially when I pull a chip from
# a socket for use in another project, or when revisiting a project months
# later)
# When program-memory is running-short, using PROJINFO_SHORT=TRUE limits
# the information written in the chip's program-memory to only the
# bare-essentials.
CFLAGS += -D'PROJINFO_SHORT=TRUE'



#PROJECT-SPECIFIC:
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

#a/o v95:
#Currently, ROW_SEG_BUFFER does not work with PLLSYSCLK
# On some displays it just squishes the image horizontally into half the
# display, on others (BOE) it causes syncing problems.
# (Ultimately, PLLSYSCLK would probably allow for twice the row-seg-buffer
# resolution, so it's in the works)
# So, until then...
ifeq "$(MAKE_MAINCONFIG__ROW_SEG_BUFFER)" "TRUE"
#SOMETHING IS BROKEN:
# The fuses need to be default, (not as overridden by PLLSYSCLK)
# But PLL_SYSCLK needs to be true...
#PLLSYSCLK = 1
CFLAGS += -D'PLL_SYSCLK=TRUE'
#OR MAYBE It has to do with FCPU??? and delay_loop_2, ish?
# No Change
FCPU = 16000000UL
else
#SERIOUSLY?! A FRIGGIN' TAB AT THE END.
#PLLSYSCLK=1	
PLLSYSCLK=1
endif




ifeq ($(PLLSYSCLK), 1)
#ifeq "$(PLLSYSCLK)" "1"
#This value is the same for the ATTiny861 and AT90PWM161
FUSEL = 0xe1
# Nominal:
FCPU = 16000000UL
CFLAGS += -D'PLL_SYSCLK=TRUE'
endif

#These haven't been tested for quite some time...
# It was used, mainly, for the sake of oscilloscoping
#Slow the whole system to 1MHz (and 8MHz PLL)... CLKDIV8
#CFLAGS += -D'SLOW_EVERYTHING_TEST=TRUE'

# It doesn't make sense to enable this without enabling SLOW_EVERYTHING...
# (does it?)
# And vice-versa
#So SLOW_EVERYTHING enables this...
#CFLAGS += -D'SLOW_LVDS_TEST=TRUE'




# Been here, did this... in LCDdirect (pre LVDS) but we WANTED unrolling
# In any case, it seems to have no effect, and even placing it directly
# in the gcc command, both at the end and the beginning of the flags
# has no effect. no-unroll-loops as well as no-unroll-all-loops
#CGLAGS += -fno-unroll-loops
#HAH! CGLAGS! No Wonder???



# Experimenting with Preprocessor output...
# Only run the preprocessor (I think there's a 'make <something>' option
# for this, now).
# files will still be named .o...
#CFLAGS += -E

# Display the used headers and other #includes
#CFLAGS += -H
#CFLAGS += -mint8



	
	
	
# Disable the Watch-Dog Timer
# It's not used in this project...
WDT_DISABLE = TRUE
# And add a note to projinfo.h for the progmem header info...
	PROJ_OPT_HDR += WDT_DIS=$(WDT_DISABLE)



#hsyncTimerStuff.c/h handle the atTiny861 by default... these override that
ifeq "$(MCU)" "at90pwm161"
CFLAGS += -D'HSYNC_TIMER_NUM=1'
CFLAGS += -D'HSYNC_TIMER_OCR=ICR1'
CFLAGS += -D'HSYNC_TIMER_MAX=0xffff'
#PWM161's Timer1 only has CLKDIV1... so if it doesn't fit, we're screwed.
CFLAGS += -D'HSYNC_TIMER_CLKDIV=CLKDIV1'
CFLAGS += -D'HSYNC_TIMER_INTERRUPT_VECT=TIMER1_CAPT_vect'
CFLAGS += -D'HSYNC_TIMER_TCNT=TCNT1'
else
#Apparently it doesn't *quite* handle the atTiny861 by default anymore...

#ATTiny861's Timer0 is 8/16-bit... for some reason I decided to use it in
#8-bit mode. And, oddly, TCNT0 is not defined, only TCNT0L/H
# (Discovered a/o v77)
CFLAGS += -D'HSYNC_TIMER_INTERRUPT_VECT=TIMER0_COMPA_vect'
CFLAGS += -D'HSYNC_TIMER_MAX=0xff'
CFLAGS += -D'HSYNC_TIMER_TCNT=TCNT0L'
endif





#COMREL is the location of the _common directory, relative to here...
# this should NOT be an absolute path because:
# COMREL is also used for compiling common-code into _BUILD
# These values are for a *shared* (system-wide) _commonCode/ directory
#
# For a distributed (aka "localized") project, these values will be
# modified in the block below. 
COMREL = ../../..
COMDIR = $(COMREL)/_commonCode


#This block can be found in _commonCode.../_make/reallyCommon.mk:
# BUT it must be included here, immediately after the definitions above,
# and long before including reallyCommon.mk
# (Why didn't I just have another .mk to include here? Something about the
# path?)
#
################# SHOULD NOT CHANGE THIS BLOCK... FROM HERE ############## 
#                                                                        #
# This stuff has to be done early-on (e.g. before other makefiles are    #
#   included..                                                           #
#                                                                        #
#                                                                        #
# If this is defined, we can use 'make copyCommon'                       #
#   to copy all used commonCode to this subdirectory                     #
#   We can also use 'make LOCAL=TRUE ...' to build from that code,       #
#     rather than that in _commonCode/                                   #
LOCAL_COM_DIR = _commonCode_localized
#                                                                        #
#                                                                        #
# If __use_Local_CommonCode.mk exists and contains "LOCAL=1"             #
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







# Common "Libraries" to be included for this project
# (Note that "libraries" usually implies precompiled code, which isn't the
# case, here, because these are used on many different processors)
# I haven't yet figured out how to make this less-ugly...

# These "Libraries" (or _commonCode) can include others, as dependencies.
# Where that's the case, it is handled automatically.
# However, by *explicitly* listing them here, the versions listed here will
# override those that would be included as a dependency.

# xxx_LIB is:
#path to the library.c/mk files, including filenames, excluding extensions

#There are two methods for passing configuration-options to the commonCode
#  CFLAGS += -D'xxx=yyy'
# or just
#  xxx = yyy
# There's no real distinct reason for my having chosen one method over the
# other, except that the 'xxx = yyy' method is easier to implement, as it
# doesn't require modifying the commonCode's .mk file to add CFLAGS+=...
# Anyways, it's just the way it is.
# Many commonCodes have options for code-size optimization, so you'll see
# lots of "REMOVED" options, below. 

#The usage-explanations below may not be all-inclusive...

#hfModulation is used in many places and in many ways...
# For certain, it's used by SEG_RACER to create smooth lines between
# distant points. Though, generally, it's similar to
# Pulse-Width-Modulation, in the sense that it creates a more analog-like
# power output using a digital "output".
VER_HFMODULATION = 1.00
HFMODULATION_LIB = $(COMDIR)/hfModulation/$(VER_HFMODULATION)/hfModulation
include $(HFMODULATION_LIB).mk


#adc is used by fb_question for the "hit-sensor" (a piezo-element), as well
# by SEG_RACER for the potentiometer.
VER_ADC = 0.20
CFLAGS += -D'ADC_SUM_REMOVED=TRUE'
# For early testing toward the pwm161
CFLAGS += -D'ADC_MCU_NYI_ERROR_DISABLED=TRUE'
ADC_LIB = $(COMDIR)/adc/$(VER_ADC)/adc
ifeq "$(MAKE_MAINCONFIG__USE_ADC)" "TRUE"
include $(ADC_LIB).mk
else
#But make sure it's still included for distro
COM_HEADERS+=$(COMDIR)/adc/$(VER_ADC)
endif

#timerCommon is used by everything timer-related... 
#With the ATtiny861:
# timer1 is used for Pulse-Width-Modulation which simulates FPD-Link. 
# timer0 is used for the regular LCD timing-signals 
#        (interrupts at the beginning of each Hsync)
#With the AT90PWM161:
# timer0 DNE
# timer1 will likely be used for the LCD timing-signals
VER_TIMERCOMMON = 1.22
CFLAGS+=-D'TIMER_SETOUTPUTMODES_UNUSED=TRUE'
CFLAGS+=-D'TIMER_SELECTDIISOR_UNUSED=TRUE'
#CFLAGS+=-D'TIMER_SETWGM_UNUSED=TRUE'
#CFLAGS+=-D'TIMER_COMPAREMATCHINTSETUP_UNUSED=TRUE'
CFLAGS+=-D'TIMER_OVERFLOWINTENABLE_UNUSED=TRUE'
TIMERCOMMON_LIB = $(COMDIR)/timerCommon/$(VER_TIMERCOMMON)/timerCommon
include $(TIMERCOMMON_LIB).mk




#heartbeat implements a fading heartbeat LED on the specified pin.
# In addition, the same pin can be used as a momentary-pushbutton input.
# Generally, the heartbeat pin is connected to MISO, which, among other
# things, allows for easy addition of a pushbutton to the project, by
# merely connecting an adapter between my programmer and the CPU.
# USUALLY it's in *all* my projects, until either code-space runs out, or
# there are no remaining pins. 
#
# This particular project is an exception.
# The heart is explicitly removed from this project, so it would make sense
# to just remove all references to it. But it's useful and should probably
# be reimplemented for the sake of early tests, in order to make sure the
# CPU isn't crashing when running with PLL_SYSCLK (32MHz when rated for 16)
# Setting HEART_REMOVED=TRUE causes all references to heart functions to be
# replaced with '0;' which optimizes out to nothing.
#
# And, for now, it *must* be the case, due to pinout stuff, described below
# The above notes re: Tiny861, AT90PWM161 has MISO on an otherwise unused
# pin, so therefore heartbeat can be used with the default configuration
# (on MISO)
#SEE NOTE BELOW IN COM_HEADERS...
#ifneq "$(MCU)" "at90pwm161"

#### TODO:
# a/o v82, attempting HEART_REMOVED to reduce memory usage... (RAM)
# As it appears that the stack is overwriting data.


##### a/o v85: Readding heart for testing of __flash...
#HEART_REMOVED = TRUE



#endif

VER_HEARTBEAT = 1.50
HEARTBEAT_LIB = $(COMDIR)/heartbeat/$(VER_HEARTBEAT)/heartbeat

#Code-size was running-out on the ATTiny861, so don't include DMS_TIMER
# if we're compiling on that chip.
# Things're probably getting hokey, now, because I don't have an 861 to
# continue development alongside.
#DMS is used by FB_QUESTION to determine when to change the images... a/o
# v68
#ifeq "$(MCU)" "at90pwm161"
#HEART_DMS = TRUE
#DMS_EXTERNALUPDATE = TRUE
#CFLAGS += -D'DMS_FRAC_UNUSED=TRUE'
HEART_TCNTER = TRUE

# Use the HSYNC_TIMER for the tcnter, as well... since it's already running
#This is necessary to make these HSYNC_TIMER_... definitions available to
#tcnter:
CFLAGS += -D'_SPECIALHEADER_FOR_TCNTER_="hsyncTimerStuff.h"'
#These set up the TCNTER timing stuff...
CFLAGS += -D'TCNTER_SOURCE_VAR=HSYNC_TIMER_TCNT'
CFLAGS += -D'TCNTER_SOURCE_OVERFLOW_VAL=HSYNC_TIMER_OCRVAL'
CFLAGS += -D'TCNTS_PER_SECOND=(F_CPU/(1<<HSYNC_TIMER_CLKDIV))'

#Because the hsync timer-interrupt function takes *so much time*
# It's very likely the main-loop doesn't even run a full-loop between each
# interrupt. So, tcnter_update() wouldn't be called often-enough.
# Use this, and put tcnter_overflowUpdate() in the interrupt
CFLAGS += -D'TCNT_UPDATE_ONCE_PER_OVERFLOW=TRUE'


#else
#HEART_DMS = FALSE
#DMS_EXTERNALUPDATE = FALSE
#endif

#override heartBeat's preferred 4s choice...
#CFLAGS += -D'_WDTO_USER_=WDTO_1S'
CFLAGS += -D'HEARTPIN_HARDCODED=TRUE'

#This pinout is... questionable.
# The typical heart-pin is MISO, since the slave device (CPU) is capable of
# driving up to 40mA (could easily drive an LED AND the programmer's input)

#Tiny861:
# But, MISO is an OC1x pin, used by the LCD, in this case...
# Since HEART_REMOVED is true, above, this doesn't matter either way
# BUT, it means we can't use the typical heart-pin as a momentary-low-input
# In other words, this pin is basically unused. See pinout.h
#PWM161:
# MISO is otherwise unused, so can therefore be used for the heart
# (as long as there's enough code-space and whatnot)

# IF mainConfig.h contains a definition for _HEART_PINNUM_
#  then use that instead (e.g. a/o v80, the Tiny861 can also be used with
#  the Sony Parallel-interfaced LCD, which doesn't have the limitation
#  described above (MISO is available for the heart)
# So, mainConfig.h can override these defaults...
ifdef MAKE_MAINCONFIG___HEART_PINNUM_
CFLAGS += -D'HEART_PINNUM=$(MAKE_MAINCONFIG___HEART_PINNUM_)'
CFLAGS += -D'HEART_PINPORT=$(MAKE_MAINCONFIG___HEART_PINPORT_)'
else
ifeq "$(MCU)" "at90pwm161"
#PWM161
CFLAGS += -D'HEART_PINNUM=PB6'
CFLAGS += -D'HEART_PINPORT=PORTB'
else
#TINY861
#THIS IS NOT the normal pin for heartbeat (on the programming-header)
# as that pin has to be used for FPD-Link...
CFLAGS += -D'HEART_PINNUM=PA6'
CFLAGS += -D'HEART_PINPORT=PORTA'
endif
endif

CFLAGS += -D'HEART_LEDCONNECTION=LED_TIED_HIGH'
include $(HEARTBEAT_LIB).mk



#Trying to free-up space for FB_QUESTION on Tiny861, sineTable is only used
# by a few options, but has been compiled in for everything until now.
#This isn't the best test... but it's good-nough for now
#sineTable is a table of values for sin(), but not using floating-point.
# Here it's used by SEG_RACER to generate the track, by SEG_SINE to
# generate the sine-wave image, and possibly elsewhere.
VER_SINETABLE = 0.99
SINETABLE_LIB = $(COMDIR)/sineTable/$(VER_SINETABLE)/sineTable
CFLAGS += -D'SINE_DISABLEDOUBLESCALE=TRUE'
SINE_TYPE = 16
CFLAGS+=-D'SINE_RAW8=TRUE'
#CGLAGS += -D'SINE_TABLE_ONLY=TRUE'

ifneq "$(MAKE_MAINCONFIG__FB_QUESTION)" "TRUE"
include $(SINETABLE_LIB).mk
else
#Make sure it's still available in the distro:
COM_HEADERS += $(COMDIR)/sineTable/$(VER_SINETABLE)
endif





# HEADERS... these are 'library' HEADERS which do NOT HAVE SOURCE CODE
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

#errorHandling is a bit lame... Its main use is in timerCommon, it's
# supposed to give an error if a timer is configured with a value that's
# not implementable. But where does that error go? That's why it's a bit
# hokey. Ultimately it shoulda been passed up the call-tree and end up
# causing the heartbeat to blink an error-number, but it never got that
# far. Regardless, it's necessary here, to compile timerCommon (and maybe
# others). I don't know why I didn't set up timerCommon.mk to handle this.
VER_ERRORHANDLING = 0.99
ERRORHANDLING_HDR = $(COMDIR)/errorHandling/$(VER_ERRORHANDLING)/
CFLAGS += -D'_ERRORHANDLING_HEADER_="$(ERRORHANDLING_HDR)/errorHandling.h"'
COM_HEADERS += $(ERRORHANDLING_HDR)
	
#bithandling is used *everywhere*
# Maybe it'd make sense to have it automatically included in every
# commonCode's .mk file, but then, still, it'd probably have to be in
# makefile for projects that don't include any other commonCode... so I
# guess it gets to go here all the time.
VER_BITHANDLING = 0.95
BITHANDLING_HDR = $(COMDIR)/bithandling/$(VER_BITHANDLING)/
# This is so #include _BITHANDLING_HEADER_ can be used in .c and .h files
# It should probably be moved to bithandling.mk (?)
CFLAGS += -D'_BITHANDLING_HEADER_="$(BITHANDLING_HDR)/bithandling.h"'
COM_HEADERS += $(BITHANDLING_HDR)


#piezoHitDetector is used by FB_QUESTION to detect physical hits to the
# project-box
# It uses the ADC and a piezo-element (and a tiny bit of circuitry)
PIEZOHITDETECTOR := $(COMDIR)/piezoHitDetector/0.10ncf/piezoHitDetector.c
COM_HEADERS += $(PIEZOHITDETECTOR)
CFLAGS += -D'_PIEZOHITDETECTOR_CFILE_="$(PIEZOHITDETECTOR)"'

#stringify is used by delayCyc (see below)
# It handles creating strings, creating custom variable-names, etc. in
# preprocessor definitions
STRINGIFY := $(COMDIR)/__std_wrappers/stringify/0.10/stringify.h
COM_HEADERS += $(STRINGIFY)
CFLAGS += -D'_STRINGIFY_HEADER_="$(STRINGIFY)"'

#delayCyc is used by the lvds161.c in order to synchronize PSCR and PSC2
# Rather than only conditionally including it in COM_HEADERS, I'll include
# it regardless. That way, if a distribution goes out configured for the
# Tiny861, and someone decides to reconfigure it for the PWM161, then the
# code will be available...
#NOTE: delayCyc in _commonCode.../ is NOT THE SAME as delay_cyc.c/h in the
# main project directory
DELAYCYC := $(COMDIR)/delayCyc/0.20ncf/delayCyc.h
COM_HEADERS += $(DELAYCYC)
CFLAGS += -D'_DELAYCYC_HEADER_="$(DELAYCYC)"'

#lcdStuff handles timing for H-sync/blank, V-sync/blank, DataEnable, etc.
# Yes, this is one of those cases where COM_HEADERS is being hacked for
# direct-inclusion of a .c file. It's ugly. lcdStuff.h and lcdStuff.mk
# should be created. They haven't yet.
#
# a/o v92: lcdStuff0.80ncfDD6651 is the version used in fixing
# v66->v66.51-64 for GCC4.8... (specifically: BLUE_FRAME, and
# BLUE_DIAG_BAR(_SCROLL)
# v0.90 is such a minor change from v0.80 that it's probably not even worth
# reusing...
LCDSTUFF := $(COMDIR)/lcdStuff/0.80ncfDD6651/lcdStuff.c
COM_HEADERS += $(dir $(LCDSTUFF))
CFLAGS += -D'_LCDSTUFF_CFILE_="$(LCDSTUFF)"'

#charBitmap is a "font"
# It's used by SEG_SINE and SEG_RACER for displaying text.
# Not much to say here, except that this originated in one of my earliest
# microcontroller projects. As I recall, originally from screenshots of 
# Microsoft Windows' 8x8 command-prompt font.
CHARBITMAP := $(COMDIR)/charBitmap/0.10/charBitmap.h
COM_HEADERS += $(CHARBITMAP)
CFLAGS += -D'_CHARBITMAP_HEADER_="$(CHARBITMAP)"'


#This is a total hack...
# For distribution-purposes, we need to have the makefile-snippet for the
# non-selected MCU available, as well
#COM_HEADERS is really nothing more than a list of specific files which
# need to be copied-over to _commonCode_localized/
ifeq "$(MCU)" "at90pwm161"
COM_HEADERS += $(COMDIR)/_make/attiny861.mk
else
COM_HEADERS += $(COMDIR)/_make/at90pwm161.mk
endif



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







#reallyCommon2.mk is the third version of my common.mk scheme...
# yeahp: common.mk -> reallyCommon.mk -> reallyCommon2.mk
# Take a look at it, it's fun! I don't even know how the vast-majority of
# it works, even though I wrote the vast-majority of the damned thing. 
# makefiles is fun!
# It also includes other files in _make/ such as avrCommon.mk
# THOUGH reallyCommon2.mk is *NOT* specific to AVRs 
# (and will not include avrCommon.mk, if not compiling for an AVR).
include $(COMDIR)/_make/reallyCommon2.mk




.mainConfig.preprocessed.mk: mainConfig.h _config/*
	@echo ""
	@echo " #################################################################"
	@echo " ## mainConfig.h has changed or otherwise needs to be processed ##"
	@echo " ## !!! probably best to run 'make clean' !!!!                  ##"
	@echo " #################################################################"
	@echo ""
	@_tools/configParser2.sh > .mainConfig.preprocessed.mk
#	@touch main.c
# This is all unnecessary.
# see 'info make' "3.5 How Makefiles Are Remade"
#
#	if [ "$${?}" != "0" ] ;\
#	then \
#		echo "WTF...? Check _tools/configParser2.sh...?" ;\
#		exit 1 ;\
#	fi
#	@echo " This will exit with a failure, but actually it worked."
#	@echo " It's just that since it modifies an included makefile,"
#	@echo " make needs to be run a second time.... So..."
#	@echo " ##################################################"
#	@echo " ### Just run 'make' again, and be on your way  ###"
#	@echo " ##################################################"
#	@exit 1



#/* mehPL:
# *    I would love to believe in a world where licensing shouldn't be
# *    necessary; where people would respect others' work and wishes, 
# *    and give credit where it's due. 
# *    A world where those who find people's work useful would at least 
# *    send positive vibes--if not an email.
# *    A world where we wouldn't have to think about the potential
# *    legal-loopholes that others may take advantage of.
# *
# *    Until that world exists:
# *
# *    This software and associated hardware design is free to use,
# *    modify, and even redistribute, etc. with only a few exceptions
# *    I've thought-up as-yet (this list may be appended-to, hopefully it
# *    doesn't have to be):
# * 
# *    1) Please do not change/remove this licensing info.
# *    2) Please do not change/remove others' credit/licensing/copyright 
# *         info, where noted. 
# *    3) If you find yourself profiting from my work, please send me a
# *         beer, a trinket, or cash is always handy as well.
# *         (Please be considerate. E.G. if you've reposted my work on a
# *          revenue-making (ad-based) website, please think of the
# *          years and years of hard work that went into this!)
# *    4) If you *intend* to profit from my work, you must get my
# *         permission, first. 
# *    5) No permission is given for my work to be used in Military, NSA,
# *         or other creepy-ass purposes. No exceptions. And if there's 
# *         any question in your mind as to whether your project qualifies
# *         under this category, you must get my explicit permission.
# *
# *    The open-sourced project this originated from is ~98% the work of
# *    the original author, except where otherwise noted.
# *    That includes the "commonCode" and makefiles.
# *    Thanks, of course, should be given to those who worked on the tools
# *    I've used: avr-dude, avr-gcc, gnu-make, vim, usb-tiny, and 
# *    I'm certain many others. 
# *    And, as well, to the countless coders who've taken time to post
# *    solutions to issues I couldn't solve, all over the internets.
# *
# *
# *    I'd love to hear of how this is being used, suggestions for
# *    improvements, etc!
# *         
# *    The creator of the original code and original hardware can be
# *    contacted at:
# *
# *        EricWazHung At Gmail Dotcom
# *
# *    This code's origin (and latest versions) can be found at:
# *
# *        https://code.google.com/u/ericwazhung/
# *
# *    The site associated with the original open-sourced project is at:
# *
# *        https://sites.google.com/site/geekattempts/
# *
# *    If any of that ever changes, I will be sure to note it here, 
# *    and add a link at the pages above.
# *
# * This license added to the original file located at:
# * /Users/meh/_avrProjects/LCDdirectLVDS/93-checkingProcessAgain/makefile
# *
# *    (Wow, that's a lot longer than I'd hoped).
# *
# *    Enjoy!
# */
