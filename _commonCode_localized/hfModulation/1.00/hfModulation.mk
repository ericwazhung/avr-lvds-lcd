# hfModulation depends only on bithandling, which isn't a library..

# The following variables are expected to be defined in the calling makefile(s)
# HFMODULATION_LIB - Path to hfModulation.mk/c including filename, excluding extension
# TCOMDIR
#
#Optional:
# HFMODULATION_REMOVED = TRUE -- remove HFMODULATION for code-size

#only include hfModulation once...
ifneq ($(HFMODULATION_INCLUDED),true)

HFMODULATION_INCLUDED = true

CFLAGS += -D'_HFMODULATION_INCLUDED_=$(HFMODULATION_INCLUDED)'
CFLAGS += -D'_HFMODULATION_HEADER_="$(HFMODULATION_LIB).h"'

ifneq ($(HFMODULATION_REMOVED),TRUE)
#Add this library's source code to SRC
SRC += $(HFMODULATION_LIB).c
else
CFLAGS += -D'__HFMODULATION_REMOVED__=TRUE'
endif

endif
