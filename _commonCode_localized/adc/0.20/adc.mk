# adc doesn't depend on any other libraries...

# The following variables are expected to be defined in the calling makefile(s)
# ADC_LIB	- Path to motorPoxn library files, including filename, excluding extensions
# TCOMDIR

# optional definitions (in gcc -D listing) TRUE or FALSE
# __TODO_WARN__	- warn about TODOs
# __UNKN_WARN__ - warn about unknowns/untesteds
# __OPTIM_WARN__ - warn of potential optimizations
# __ERR_WARN__   - warn of potential errors (divide-by-zero, etc...)



#only include adc once...
ifneq ($(ADC_INCLUDED),true)
ADC_INCLUDED = true


CFLAGS += -D'_ADC_HEADER_="$(ADC_LIB).h"'

# The following variables are defined here for called makefile(s)
#VER_PWM_DEP = 0.91
#PWM_DEP_LIB = $(TCOMDIR)/pwm_dep/$(VER_PWM_DEP)/pwm_dep

#Add this library's source code to SRC
SRC += $(ADC_LIB).c

#include the libraries this one depends on 
#include $(LMD18200_LIB).mk	\

endif

