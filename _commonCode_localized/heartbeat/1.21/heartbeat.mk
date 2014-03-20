#/* mehPL:
# *    This is Open Source, but NOT GPL. I call it mehPL.
# *    I'm not too fond of long licenses at the top of the file.
# *    Please see the bottom.
# *    Enjoy!
# */
#
#
# heartbeat depends on the hfModulation library, as well as dmsTimer when __HEART_NODMS__ is not defined elsewhere...

# The following variables are expected to be defined in the calling makefile(s)
# HEARTBEAT_LIB
# COMDIR
# HEART_DMS (TRUE or FALSE)
# DMS_EXTERNALUPDATE (Only required if HEART_DMS is TRUE)	(TRUE or FALSE)
# WDT_DISABLE (TRUE or FALSE optional?)

# optional definitions (in gcc -D listing) TRUE or FALSE
# __TODO_WARN__ - warn about TODOs
# __UNKN_WARN__ - warn about unknowns/untesteds
# __OPTIM_WARN__ - warn of potential optimizations
# __ERR_WARN__   - warn of potential errors (divide-by-zero, etc...)
# HEART_REMOVED = TRUE/FALSE -- Remove all heart code for size-testing

#only include heartbeat once...
ifneq ($(HEARTBEAT_INCLUDED),true)

HEARTBEAT_INCLUDED = true

CFLAGS += -D'_HEART_DMS_=$(HEART_DMS)'
CFLAGS += -D'_HEARTBEAT_HEADER_="$(HEARTBEAT_LIB).h"'

ifeq ($(HEART_REMOVED),TRUE)
CFLAGS += -D'__HEART_REMOVED__=TRUE'
endif

#Don't define it otherwise, because ifdef is used rather than if true...
ifeq ($(WDT_DISABLE), TRUE)
CFLAGS += -D'_WDT_DISABLE_=$(WDT_DISABLE)'
endif

# The following variables are defined here for called makefile(s)
ifndef HFMODULATION_LIB
VER_HFMODULATION = 0.93
HFMODULATION_LIB = $(COMDIR)/hfModulation/$(VER_HFMODULATION)/hfModulation
#include $(HFMODULATION_LIB).mk
endif

#Add this library's source code to SRC
ifneq ($(HEART_REMOVED),TRUE)
SRC += $(HEARTBEAT_LIB).c
else
# This is a TOTAL hack...
# Apparently I wasn't thinking when creating HEART_REMOVED, which removes
# heartbeat.c from SRC...
# And since SRC is responsible for localizing... we get an issue.
COM_HEADERS += $(dir $(HEARTBEAT_LIB))
endif


#include the libraries this one depends on
# WTF. Why shouldn't this be in the ifndef above? It's not functioning
# with LCDreIDer47 that way...
include $(HFMODULATION_LIB).mk

#only include dmsTimer if HEART_DMS is TRUE
ifeq ($(HEART_DMS), TRUE)

ifndef DMSTIMER_LIB
VER_DMSTIMER = 1.13
DMSTIMER_LIB = $(COMDIR)/dmsTimer/$(VER_DMSTIMER)/dmsTimer
include $(DMSTIMER_LIB).mk
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
# *    2) Please do not change/remove others' credit/licensing/copywrite 
# *         info, where noted. 
# *    3) If you find yourself profitting from my work, please send me a
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
# *    (Wow, that's a lot longer than I'd hoped).
# *
# *    Enjoy!
# */
