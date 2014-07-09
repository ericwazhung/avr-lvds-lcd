#/* mehPL:
# *    This is Open Source, but NOT GPL. I call it mehPL.
# *    I'm not too fond of long licenses at the top of the file.
# *    Please see the bottom.
# *    Enjoy!
# */
#
#
# tcnter (doesn't even) depends only on bithandling, which isn't a library..

# The following variables are expected to be defined 
#  in the calling makefile(s)
# TCNTER_LIB - Path to tcnter.mk/c including filename, 
#  excluding extension
# COMDIR
#
#	TCNTER_INLINE (optional, NOT a CFLAG)
#
# CONSIDERATIONS:
#
# TCNTER_SOURCE_VAR - e.g. TCNT0
#   might take some work if 
#   TCNTER_SOURCE_VAR is a variable which needs to be externed
#   TCNTER_SOURCE_EXTERNED = TRUE to add externing...
# TCNTER_SOURCE_OVERFLOW_VAL   - e.g. OCR0A
#   also, could be a constant value...
#   (if TCNT is 8-bit and overflows on its own, this should be 256)
#
# OPTIONAL (change BOTH if either)
# tcnt_source_t - e.g. uint8_t, uint16_t
# tcnt_compare_t - e.g. int16_t, int32_t

#only include tcnter once...
ifneq ($(TCNTER_INCLUDED),true)
TCNTER_INCLUDED = true

CFLAGS += -D'_TCNTER_INCLUDED_=$(TCNTER_INCLUDED)' 
CFLAGS += -D'_TCNTER_HEADER_="$(TCNTER_LIB).h"' 

ifeq ($(TCNTER_INLINE), TRUE)
CFLAGS += -D'_TCNTER_INLINE_=TRUE'
# Extract the directory we're in and add this to COM_HEADERS...
COM_HEADERS += $(dir $(TCNTER_LIB))
else
CFLAGS += -D'_TCNTER_INLINE_=FALSE'
#Add this library's source code to SRC
SRC += $(TCNTER_LIB).c
endif


# If using an AVR, most-likely we'll be running off of a timer (e.g. TCNT0)
ifdef MCU

# Should we somehow test for TCNTER_SOURCE_EXTERNED==FALSE...?
# (Not currently possible, since it's CFLAGed)
ifndef TIMERCOMMON_LIB

# If TIMERCOMMON_LIB was not previously defined, then we assume that the
# timer is used *only* by tcnter, in which case we can initialize it to
# whatever configuration we please.
# This might be a bit hokey...
# Better, maybe, is to require TCNTER_INITS_TIMER to be manually set...?
CFLAGS += -D'TCNTER_INITS_TIMER=TRUE'

VER_TIMERCOMMON = 1.22
TIMERCOMMON_LIB = $(COMDIR)/timerCommon/$(VER_TIMERCOMMON)/timerCommon
include $(TIMERCOMMON_LIB).mk


else
# TIMERCOMMON_LIB was already included elsewhere, which implies, I guess,
# that it's being used by another piece of the project.
# IN WHICH CASE: it's necessary to set up the appropriate variables
# SEE: (search for TCNTER_INITS_TIMER and the variables it configures)
endif


endif




endif
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
# * /Users/meh/_avrProjects/LCDdirectLVDS/90-reGitting/_commonCode_localized/tcnter/0.30/tcnter.mk
# *
# *    (Wow, that's a lot longer than I'd hoped).
# *
# *    Enjoy!
# */
