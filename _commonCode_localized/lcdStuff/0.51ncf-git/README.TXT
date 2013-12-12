This is only a tiny fraction of the overall code. In fact, ultimately, this
is intended to be more generalized for graphic-LCDs (using H/V/DE timing).

As it stands, it currently contains notes from three (or more?) separate 
projects, though as of this more-generalized version has only for certain
been used in one... it is, as I recall, very-much compatible with all, 
with a few not-all-yet-written wrapper-functions. Notes are highly out of
order, and the vast-majority of the code here is commented/#if 0'ed out.
Actually, besides DE, this needn't be specific to LCDs at all, could serve
just as well for VGA, etc.

* LCDdirect - The original project
	Atmel 90s8515 AVR on a 30MHz crystal /2
	15in desktop TFT 1024x768, removed from VGA converter-board
		dual-pixel, individual pins for each signal, ~40 wires (NOT LVDS)
	~ 32 pixels by 32 pixels stretched across the full screen, 8 colors
	~ 30Hz refresh
* LCDdirectLVDS - The second and revisited project
	Atmel ATtiny861 using internal R/C oscillator at 16MHz, 512B RAM
	12.1in laptop TFT LCD 1024x768
		Samsung LTN121X1-L02 from a dead iBook G3
		LVDS/FPD-Link, single-pixel (4 signals on 8 wires)
	2x 74LS86 XOR chips used for LVDS level-conversion
	~ 20Hz refresh using the PWM PLL at ~128MHz, ~32x32 pixels stretched
	~ 1Hz refresh at ~341x768 pixels, stretched, 48 colors.
* SDRAMthing - Unrelated project, just using the LCD as a test...
	Atmel 90s8515 directly-connected to a 128MB SDRAM DIMM
	All FPD-Link signals for a single frame were loaded into the SDRAM and
		burst out to the LVDS display repeatedly
	1024x768 resolution, full-color (6bits per R/G/B)
	~1Hz refresh (but reloading the SDRAM for a new image takes about 2 
		minutes!)

LCDdirectLVDS uses this exact code, with wrapper-functions for timing, etc.
This file shouldn't contain that LVDS-specific information, but I haven't
broken the files up yet.

Some other notes:
This is intended, as it stands (bad coding practices, I know) to be 
#included in main... prior to its inclusion should be project-specific
functions that this calls (e.g. loadData(), drawPix(), DE_DotDelay()...).
Yeah, it'd be nice to use function-pointers and whatnot, but we're talking
about 8KB of ROM and only 512Bytes of RAM (most of which is used for
display data)... every byte counts... inlines are used almost exclusively,
or macros.
Most of the stuff, again, is commented-out... but was left for examples.
Who knows, maybe it'll be useful to someone as-is. But the rest of the code
(the real stuff) should be going up sooner or later.

And, as it stands, figuring out this git thing was quite exhausting and
dang-near magic when it worked, so it may be a while until the rest of the
code gets posted.

For sure contact me if you're interested!

----
Licensing: Ugh I hate having this shit thrown in my face all the time, so
here's the deal. Be Respectful and Concientious and unless you're a 
corporation or other organization, you're free to use and modify it.
Otherwise contact me. Cash and beer supplies are running low, so I'm always
open to donations!

