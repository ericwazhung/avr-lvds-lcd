# Include this file when making tarballs...
COM_MAKE += $(COMDIR)/_make/commonWarnings.mk



#The remaining are CFLAGS for -W... arguments
# Warnings that I would rather be considered errors...
# THESE ARE gcc-version specific! (Apple's GCC 4.01 doesn't like 'em)

# Stupid Mistake: strcmp(0==instruction,"test")
#  should have been 0==strcmp...
# Gave a warning, but wanted to convert that to error-material, nogo.
# Apparently only works when int is of the wrong size for pointer...
#CFLAGS += -Werror=int-to-pointer-cast
#-Wstrict-aliasing?
# Specifically missing return value
# This is handled with -Wreturn-type "falling out of a function is considered returning void"
CFLAGS += -Werror=return-type
# Watch for use of uninitialized variables...
# This does not handle volatiles! Does (attempt to) look for uninitialized array elements.
# This apparently isn't very smart and is optional because of that. See the manpage
# I'd like to think it's possible to code things so it can be detectable, we'll see.
# Problems? "Some spurious warnings can be avoided if you declare all the
#            functions you use that never return as 'noreturn'."
CFLAGS += -Werror=uninitialized
# This case is just BAD and should not be allowed ever.
# int i = i;
CFLAGS += -Werror=init-self
# The next two could be handled together with "implicit"
# Implicit declarations (no type specified: e.g. "static i;")
CFLAGS += -Werror=implicit-int
CFLAGS += -Werror=implicit-function-declaration
# Detect global functions that aren't in headers...
# This one's kinda annoying... useful, but annoying
#CFLAGS += -Werror=missing-prototypes
#CFLAGS += -Werror=missing-declarations
# Might want to look into -Wignored-qualifiers
# also -Wmissing-braces (just to be paranoid about array-initialization)
CFLAGS += -Werror=missing-field-initializers
# Watch parentheses and nesting... mainly:
#   if (a)
#     if (b)
#       foo ();
#   else
#     bar ();
CFLAGS += -Werror=parentheses
# Watch out for undefined stuff like a = a++;
#  do not fall into these habits as "more complicated cases are not diagnosed"
CFLAGS += -Werror=sequence-point
# Watch out for switches without default cases... I consider this bad practice
# Who knows what'll happen if an integer's bit is affected by protons?
CFLAGS += -Werror=switch-default
# Do not allow float comparisons that are ==
CFLAGS += -Werror=float-equal
# Don't allow bad address comparisons (if(func) instead of if(func()))
CFLAGS += -Werror=address
# Might also want to look into int-to-pointer-cast and pointer-to-int-cast
# Watch for pointer argument passing and assignment with different signedness
CFLAGS += -Werror=pointer-sign
# Stop on the first error
#CFLAGS += -Wfatal-errors
# Warn for variables that might be changed by longjmp or vfork.
CFLAGS += -Wclobbered
# Warn about implicit conversions that may alter a value
# Annoying... especially when using a uint8_t as an array position uint8_t i; noteList[i];
#CFLAGS += -Wconversion
CFLAGS += -Wsign-compare
# -Wstrict-overflow looks worthwhile for many reasons... watch out for optimization errors!
CFLAGS += -Wstrict-overflow=4
# Warn if an undefined identifier is evaluated in an #if directive.
CFLAGS += -Wundef


