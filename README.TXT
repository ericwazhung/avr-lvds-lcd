Dang-near every line of code here and in the subdirectories is my work
including makefiles, etc. I have made-reference where others' work has 
been helpful.

Please be respectful... if you find anything here useful, awesome!
If you use it verbatim, please give credit where it's due. 
If you find yourself profitting from it, please send me a beer (or cash is
always handy).

The only BIG restriction is to organizations: If you *intend* to profit off
my work, you must contact me first. And if your intent is related to
military, NSA, or other creepy purposes, you do NOT have my permission to
use my work.

I'm EricWazHung [at] gmail dotcom


----------

(I hate this legal bullshit, people should just have basic human decency
 then none of this would be necessary.)

----------


I'll say up-front there's QUITE A BIT of redundancy and left-over notes
which are no longer relevent to the current version. 
Also, there're a *ton* of configurable options which render much of the
code unused... e.g. "racer" is a game I've been working on, it uses a
potentiometer connected to an ADC as a controller... the ADC is not used
at all for the "Mario"/"QUESTION" stuff... 
Similar for sineTable, and a few others...

Stuff in _commonCode_localized/ is stuff I use regularly for various
projects... _commonCode.../lcdStuff is a good place to start looking for
graphical LCD stuff, specifically.

Most of the remaining graphical LCD stuff is located in the main directory.


I'm only *barely* familiar with this git/version-tracking stuff... (first
attempt was yesterday)
Previously, I had my own version-tracking scheme. They're not particularly
compatible... As of this writing, this first "distribution" contains
the 59th functional version of the code... when I move on to 60, it may be
difficult to use my versioning-scheme along with git to upload it...
So the updates may be pretty irregular. We'll see how it goes.

As it stands, this is meant to work with an ATtiny861, the pinout should
be burried in various places in code. YAY! 
(check pinout.h, and possibly the makefile)
To use as-is:
You'll need an ATtiny861
	(The AVR model is *very* specific, as this *requires* PWM Dead-Timers
	 and Three PWM channels, with bipolar outputs. I've only seen these 
	 options available on two AVR chips, this and the AT90PWM-som'n)
Two 74LS86 XOR chips (or buffers and inverters...?)
A Samsung LTN121X1-L02 12.1in 1024x768 display 
(and an inverter to drive its backlight) 
	(other displays *may* work, but I can't vouch for 'em. I have another 
	 that won't sync at all...)
and a 3.3 or 3.6V supply.

I use a USB-Tiny for programming my AVRs
(look into _commonCode.../_make/avrCommon.mk if you need to change this or
 anything else... also reallyCommon2.mk)
You should be able to do 
("make clean" ?)
"make"
"make fuse"
"make flash"

There are *several* options that are currently unused and haven't been
tested for quite some time... E.G. it has been used at much higher refresh
rates (and much lower resolution) with a tiny frame-buffer...

Currently it uses a "Row-Segment Buffer" which is reloaded after each row
is drawn... These row-segments are defined by a color-value and a number of
pixels... I think it's got a maximum of 96 row-segments...
There're 341 drawable pixels per row (streched across 1024 physical)...
So, you can see it's impossible to have a different color in every drawable
pixel in a row... (not enough memory!)

This means text is hard to display in a row... (But you can rotate the 
screen and text 90-degrees, as the 768 rows are each drawn individually)




Again, this is my first distribution of any sort, so I have no idea what'll
happen on others' systems, though I did my best to make it distributable).

I look forward to hearing others' experiences!

(See also the README file in _commonCode.../lcdStuff/...)
