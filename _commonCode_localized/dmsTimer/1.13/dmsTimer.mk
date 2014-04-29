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
# dmsTimer depends on:
#  timer0 library if DMS_EXTERNALUPDATE is false AND this is an AVR
 

# The following variables are expected to be defined in the calling makefile(s)
# DMSTIMER_LIB	- Path to dmsTimer.mk/c including filename, excluding extension
# COMDIR
# DMS_EXTERNALUPDATE	(TRUE or FALSE...)
#   FALSE when Timer0 can be used exclusively for dmsTimer
#   TRUE when Timer0 is used for other purposes... 
#        in which case, call dms_update manually (T0 overflow can work)...


# optional definitions (in gcc -D listing) TRUE or FALSE
# __TODO_WARN__ - warn about TODOs
# __UNKN_WARN__ - warn about unknowns/untesteds
# __OPTIM_WARN__ - warn of potential optimizations
# __ERR_WARN__   - warn of potential errors (divide-by-zero, etc...)


#only include dms_timer once...
ifneq ($(DMSTIMER_INCLUDED),true)

DMSTIMER_INCLUDED = true
CFLAGS += -D'_DMSTIMER_INCLUDED_=$(DMSTIMER_INCLUDED)' \
			 -D'_DMSTIMER_HEADER_="$(DMSTIMER_LIB).h"'


#Add this library's source code to SRC
SRC += $(DMSTIMER_LIB).c

#Wouldn't it make more sense to set this CFLAG either way? probably have to use #ifeq instead of #ifdef...
ifeq ($(DMS_EXTERNALUPDATE),TRUE)
CFLAGS += -D'_DMS_EXTERNALUPDATE_=$(DMS_EXTERNALUPDATE)'

ifndef XYTRACKER_LIB
VER_XYTRACKER = 3.05
XYTRACKER_LIB = $(COMDIR)/xyTracker/$(VER_XYTRACKER)/xyTracker
include $(XYTRACKER_LIB).mk
endif

else
  #Only set it if it's false or true, 
  #if it's undefined, let dmsTimer.h give a warning
  ifeq ($(DMS_EXTERNALUPDATE),FALSE)
    CFLAGS += -D'_DMS_EXTERNALUPDATE_=$(DMS_EXTERNALUPDATE)'
  endif

  # THERE MUST be a better definition to look for...
  # Maybe "AVR" somewhere?
  ifdef MCU

    # The following variables are defined here for called makefile(s)
    ifndef TIMERCOMMON_LIB
     VER_TIMERCOMMON = 1.21
     TIMERCOMMON_LIB = $(COMDIR)/timerCommon/$(VER_TIMERCOMMON)/timerCommon
     include $(TIMERCOMMON_LIB).mk
    endif

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
# * /Users/meh/_avrProjects/LCDdirectLVDS/68-backToLTN/_commonCode_localized/dmsTimer/1.13/dmsTimer.mk
# *
# *    (Wow, that's a lot longer than I'd hoped).
# *
# *    Enjoy!
# */
