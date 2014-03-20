#/* mehPL:
# *    This is Open Source, but NOT GPL. I call it mehPL.
# *    I'm not too fond of long licenses at the top of the file.
# *    Please see the bottom.
# *    Enjoy!
# */
#
#
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
