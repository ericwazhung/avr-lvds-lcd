# timerCommon doesn't depend on other libraries...

# The following variables are expected to be defined in the calling makefile(s)
# TIMERCOMMON_LIB	- Path to timerCommon library files, including filename, excluding extensions
# TCOMDIR

# optional definitions (in gcc -D listing) TRUE or FALSE
# __TODO_WARN__ - warn about TODOs
# __UNKN_WARN__ - warn about unknowns/untesteds
# __OPTIM_WARN__ - warn of potential optimizations
# __ERR_WARN__   - warn of potential errors (divide-by-zero, etc...)


#only include timerCommon once...
ifneq ($(TIMERCOMMON_INCLUDED),true)

TIMERCOMMON_INCLUDED = true
CFLAGS += -D'_TIMERCOMMON_INCLUDED_=$(TIMERCOMMON_INCLUDED)' -D'_TIMERCOMMON_HEADER_="$(TIMERCOMMON_LIB).h"'

# The following variables are defined here for called makefile(s)

#Add this library's source code to SRC
SRC += $(TIMERCOMMON_LIB).c
#include the libraries this one depends on 

endif

