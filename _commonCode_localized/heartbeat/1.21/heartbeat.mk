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
