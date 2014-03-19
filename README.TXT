The original code-site for this code is located at:
https://code.google.com/p/avr-lvds-lcd/

If that ever changes, I will be sure to note it here, and link from the
google-code page.

As it stands, this project is in no way associated with or hosted at GitHub

As I'm relatively unfamiliar with versioning utilities like Git, and am new
to contributing to the OpenSource scene, I'm trying to keep an open mind 
about the fact that someone seems to be hosting my code on GitHub.com.
(And that that site is the first result in a Google Search!). 

I'm glad for the exposure, and grateful to know that even if I lose access
to my account or die or something, my hard work will still be available to
others! But, I thought part of the point of these OpenSource/Versioning 
systems is that of trying to keep an original master-branch somewhere 
locatable, along with keeping contributors in contact... To that end:

Again, the original code-site for this code is located at:

https://code.google.com/p/avr-lvds-lcd/

And the non-code site is at:

https://sites.google.com/site/geekattempts/home-1/drive-an-old-laptop-display-from-an-avr

And, I'd absolutely be grateful to anyone attempting projects based on my
code to get in touch with me, just so I know it wasn't a complete waste of
time. Email below.

--------------
As-of 3-19-14, there are no contributors to this project besides myself.

Dang-near every line of code here and in the subdirectories is my work
including makefiles, etc. I have made-reference where others' work has 
been helpful. Some of the code here has been developed over more than a
decade, and this project itself is several years in the making.

Please be respectful... if you find anything here useful, awesome!
If you use it verbatim, please give credit where it's due, and it'd be
awesome if you'd let me know (I'll link yah).
If you find yourself profitting from it, please send me a beer (or cash is
always handy).

The only BIG restrictions are:
 If you *intend* to profit off my work, you must contact me first.
  ** That includes, if you host my work on a revenue-making site! **
 If your intent is related to military, NSA, or other creepy purposes, 
 you do NOT have my permission to use my work.

I'm EricWazHung [at] gmail dotcom

or visit:
https://sites.google.com/site/geekattempts/


(I hate this legal bullshit, people should just have basic human decency
 then none of this would be necessary.)

The End Of the Legalities
----------

(This document written at the time of the first upload version 60ish. 
 Some of it may be outdated. 
 This is version 65. Though it's quite likely I'll forget to update this 
 note, come v66).

I'll say up-front there's QUITE A BIT of redundancy and left-over notes
which are no longer relevent to the current version. (This has been cleaned
up *quite a bit* as of the latest version... those no-longer-relevent bits
have been moved to _old/ and _unusedIdeas/).

Also, there're a *ton* of configurable options which render much of the
code unused... e.g. "racer" is a game I've been working on, it uses a
potentiometer connected to an ADC as a controller... the ADC is not used
at all for the "Mario"/"QUESTION" stuff... 
Similar for sineTable, and a few others...
(a/o v65: the above is related to "SEG_QUESTION."
 FB_QUESTION is a different story, the ADC is used in FB_QUESTION for
 "hit-detection" via piezo-element. This isn't well-documented, yet.
 And SEG_QUESTION is *quite old* in comparison to FB_QUESTION)


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
(a/o v61: yeah, git and my versioning scheme are *not at all* compatible.
 Trying to keep it git-friendly is making things I've done for years quite
 a bit more difficult...)

So the updates may be pretty irregular. We'll see how it goes.

As it stands, this is meant to work with an ATtiny861, the pinout should
be burried in various places in code. YAY! 
(a/o v61: check pinout.h, it should be all contained in there)

To use as-is (with an LVDS/FPD-Link display):
You'll need an ATtiny861
	(For LVDS the AVR model is *very* specific, as this pseudo-FPD-Link
	 implementation *requires* PWM Dead-Timers and Three PWM channels, with
	 complementary outputs. I've only seen these options available on two 
	 AVR chips, this and the AT90PWM-som'n)
Two 74LS86 XOR chips (or buffers and inverters...?)
A Samsung LTN121X1-L02 12.1in 1024x768 display 
(and an inverter to drive its backlight) 
	(other displays *may* work, but I can't vouch for 'em. I have another 
	 that won't sync at all...)
and a 3.3 or 3.6V supply.

I use a USB-Tiny, avr-gcc, and avr-dude for programming my AVRs
(look into _commonCode.../_make/avrCommon.mk if you need to change this or
 anything else... also reallyCommon2.mk)
I generally upload it after a compile, so you might be able to get away
with just "make fuse" and "make flash". Otherwise:
You should be able to do 
"make clean"
"make"
"make fuse"
"make flash"

There are *several* options that are currently unused and haven't been
tested for quite some time... E.G. it has been used at much higher refresh
rates (and much lower resolution) with a tiny frame-buffer...

Currently it uses a "Row-Segment Buffer" which is reloaded after each row
is drawn... These row-segments are defined by a color-value and a number of
pixels... Depending on the amount of memory used otherwise, the number of
row-segments can be increased or decreased. (They take 2 Bytes apiece, so
there's a *definite* limit of 256 row-segments).
There're something like 341 drawable pixels per row 
(streched across 1024 physical)...
So, you can see it's impossible to have a different color in every drawable
pixel in a row... (not enough memory!) (Note that the AT90PWM161 has 1K.
It may not be difficult to port to this chip, in which case, every drawable
pixel could be individually written!)

Because of the limited memory and the limited horizontal resolution, text
is hard to display in a row... (But you can rotate the screen and text 
90-degrees, as the 768 rows are each calculated and drawn individually)


There are several drawing/game/... modes that have been tested recently
(a/o v61: all SEG_<mode>'s have been tested. See their descriptions in
 mainConfig.h)
Here are a couple examples:
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
pretty regularly across the image. The image displays and syncs for the
most part, but the colors are coming through flakily. 
Not sure what's wrong, but switching to the other display and it works 
perfectly.

(a/o v61: doubling the LVDS-bit-rate has fixed the problem, and it now 
 works with *both* LTN121X1 displays in my posession. Let this be a lesson:
 If you see *something* on your screen, it's *quite likely* you'll be able
 to sync it up properly with a little work. 
 Note that my two LTN121X1 displays have different revisions of the same
 LVDS receiver chip, I don't have the part-numbers in front of me.
 See below for a bunch of theorizing as to why it wasn't working, as 
 written *when* it wasn't working.)

---- Theories ---
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
---- End of theorizing ----

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
(a/o v61: the latest configuration is with the doubled-pixel-clock, so it
 should be more functional with more displays)

Another note: The display used is DE-Only, which means it doesn't pay 
attention to the Hsync and Vsync bits. As far as I'm aware, that code is
implemented properly. My other two displays (also from iBook G3's) are
*not* DE-Only, and neither of them sync. OTOH, one explicitly states in its
data-sheet that refresh-rates that are too slow will be disregarded "to
protect the liquid crystals". If you see that note, you'll be hard-pressed
to get the display working with this system... it relies heavily on 
stretching the displays' limits.)

Good luck and have fun!

