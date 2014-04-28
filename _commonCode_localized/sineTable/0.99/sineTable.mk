# sineTable 0.90 doesn't depend on any other libraries
# stolen from LCD1.00, adding new LIB CFLAG for #includes

# The following variables are expected to be defined in the calling makefile(s)
# SINETABLE_LIB	- Path to sineTable.mk/c/h including filename, excluding extension
# SINE_TYPE		("16" or "32" for INT16_T or INT32_T)
# COMDIR

# The following are defined for calling C/header files
# _SINETABLE_INCLUDED_	(TRUE or FALSE)
# _SINETABLE_HEADER_	(full path to sineTable.h, including version and .h, for #include _SINETABLE_HEADER_)

# optional definitions (in gcc -D listing) TRUE or FALSE
# __TODO_WARN__ - warn about TODOs
# __UNKN_WARN__ - warn about unknowns/untesteds
# __OPTIM_WARN__ - warn of potential optimizations
# __ERR_WARN__   - warn of potential errors (divide-by-zero, etc...)


#only include sineTable once...
ifneq ($(SINETABLE_INCLUDED),true)
SINETABLE_INCLUDED = true

CFLAGS += -D'_SINETABLE_INCLUDED_=$(SINETABLE_INCLUDED)' \
-D'_SINETABLE_HEADER_="$(SINETABLE_LIB).h"' 

ifdef SINE_TYPE
CFLAGS += -D'_SINE_TYPE_=$(SINE_TYPE)'
endif

#Add this library's source code to SRC
SRC += $(SINETABLE_LIB).c

endif

