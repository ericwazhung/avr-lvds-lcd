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


(I hate this legal bullshit, people should just have basic human decency
 then none of this would be necessary.)

The End Of the Legalities
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


There are two modes that have been tested recently:
In main.c, near the top (only one should be TRUE):
SEG_QUESTION displays 16x16 pixel images, which are changed by pressing
	the button (It uses sprites from Mario Brothers... Thanks Nintendo!)
	Some day I'll build a box and attach the button to a bump-sensor and
	hang 'em around the city... but you might beat me to it. If you do, I
	want to see pictures!
SEG_RACER is a game similar to "Racer" from the ol' TI-82 days...
	It's an attempt at a few things:
		Is it possible to make a fast-pace game with a slow-refresh-rate?
		Let's make use of that high resolution!
	Keep the "car" in the green track by moving a potentiometer.
	The further you go, the harder it gets.


Again, this is my first distribution of any sort, so I have no idea what'll
happen on others' systems, though I did my best to make it distributable).

I look forward to hearing others' experiences!

(See also the README file in _commonCode.../lcdStuff/...)

	------
Give it a few seconds (several in fact) to "boot". I don't know why,
because code-wise I think it starts sending signals immediately...
The screen usually appears black with a single row of white at the top
until the first frame syncs up.

------

Another Note: I have two of these Samsung displays... I've had *both* of
them working at one time or another. But doing some testing for this
distribution, one is quite flakey. It seems it's not syncing correctly with
the LVDS data-bits. The effect-of-which is what I'd call "snow" appearing
pretty regularly across the image. The image displays and syncs, but the
colors are coming through flakily. Not sure what's wrong, but switching to
the other display and it works perfectly.
Thing is, these data-rates are *horrendously* slow compared to the display
specifications... It's quite plausible (in fact *likely*) that I'm right on
the threshold of the rates the LVDS chipsets in these displays can sync 
with (certainly *way* below their ratings). We're not talking about syncing
in terms of the display's syncing with the image-data, we're talking about
each packet of (pixel) data syncing up with the receiver.

So here's a bit of the technicalities in case you're wondering, as far as
what I think might be happening (I've documented similar elsewhere).
There's a PLL (clock regenerator) in the LVDS-receiver.... it takes the
actual "Clock" signal, which is 1/7th of the bit-rate, and bumps that
signal up by 7 in order to know when to sample each bit. These circuits
usually have lower frequency limits... (How do they multiply a frequency?
Do they use capacitors?) Anyways, the result is it seems it's sampling the
bits at the wrong times. The effect is like a baud-rate that's not quite
right... the first few bits would come through properly, but the later bits
might get sampled at the wrong time.
So, why would the display have worked previously, and show snow now?
Well, it's winter-time... maybe it's a thermal thing. Or maybe I'm just
crazy and the thing was never tested.

Anyways, it's just a theory.

Actually, another theory might be the new circuit-board. I might not have
tested this board with both displays, and the main display I'd been working
with broke off the connector, so I had to try the less-tested display first
upon revisiting this project... The new circuit-board totally disregards
some of my notes elsewhere: To use gates within the *same* XOR chip for
both the negative and positive inputs of each channel, in order to avoid
potential speed-differences between chips.
(In fact, I built this board with a 74LS00 and 74LS32, NAND and OR, wired 
to act as an inverter and a buffer... because I have them in much greater
supply. I tried swapping them with my trusty XORs and the effect was the 
same... Maybe this other display is less sensitive to misalignment between
the positive and negative inputs... or the resistance in the display is
different, or a million other things. I haven't scoped it.)

As it is, the code is pretty limited to the current state-of-the-art...
I haven't messed with lower resolutions (which equate to faster-bit-rates
and faster refresh-rates) for quite some time. The code's in there, I
think, but it's burried so deep it'll take me some time to dig it up.

That said... Don't give up if your display doesn't work right off the bat.
Likewise, starting with the current state of the code is a bit of a crap-
shoot if you're not working with the exact same display. When starting
with a new display, I usually start with "DE_BLUE" which is one of the
simplest LVDS signals for the display to receive (displays a full blue
screen, if it's syncing correctly). Then "BLUE_VERT_BAR" and go on from
there. Again, it's been a while.

If you have identical equipment, you might be lucky with the code 
as-configured. Or you might get some ugliness as I just experienced.
