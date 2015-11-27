![https://sites.google.com/site/geekattempts/_/rsrc/1421450295438/misc-notes/please-donate/Hey%20You_320.jpg](https://sites.google.com/site/geekattempts/_/rsrc/1421450295438/misc-notes/please-donate/Hey%20You_320.jpg)


**I'm REALLY broke, right now...**
Please consider sending a donation my way... even $5 would be greatly-appreciated. There's a paypal link at ----> [my site](https://sites.google.com/site/geekattempts/misc-notes/please-donate).



---

**Drive an LVDS laptop-display with an AVR!**

https://sites.google.com/site/geekattempts/_/rsrc/1386932463576/SEG_SINE_LQ.jpg?width=320

Yep, it's possible!

**Currently tested-functional FPD-Link/LVDS laptop displays:**<br>
(These happen to be 1024x768, but should be modifiable for most resolutions. Donations gladly accepted!)<br>
(See bottom of page for notes)<br>
<ul><li>Samsung LTN121X1-L02 (Two revisions)<br>
</li><li>Chi Mei N121X5-L03<br>
</li><li>Boe-Hydis HT12X21-H240<br>
</li><li>NEW: Sony ACX705AKM 240x160 (9-bit parallel-interfaced NOT LVDS... should work with ANY AVR with enough I/Os. Currently tested on the ATtiny861 as well as the ATtiny84.)<br>
<br></li></ul>

<b>AVRs currently working with LVDS/FPD-Link LCDs:</b><br>
<ul><li>ATTiny861 (most development has been done on this chip)<br>
</li><li>AT90PWM161 (Newly Supported! Twice the SRAM, should come in handy.)<br>
</li><li>ATTiny85 (Newly Supported! Yep, that's an 8-pin 8-bit microcontroller running an LVDS LCD display. (Not yet uploaded to the repository))<br></li></ul>

<b>Untested but likely to work AVRs</b> (with a bit more coding) <br>
(Needed: Power-Stage-Controller/PSC or Dead-Time-Insertion/Generators running at PLL frequencies, minimum two channels)<br>
<ul><li>Other AT90PWMs are almost guaranteed to be usable; three channels may be necessary e.g. PWM316 vs PWM216 (These have PSC's)<br>
<br>
</li><li>The ATmega32M1/64M1 should work. (PSC... PLL?)<br>
</li><li>The ATmega16M1???<br>
</li><li>(ATxmegas: Plausible with <b>really forgiving</b> displays... Unfortunately, as I understand, the Xmega PWM/Dead-time setup doesn't <b>both</b> run at PLL speeds)</li></ul>

See the video at: <a href='http://youtu.be/yvFOrmkshVI'>http://youtu.be/yvFOrmkshVI</a> (I can't figure out how to embed it here).<br>
<br>
There are a few options regarding how to drive the display... basically it boils down to high resolution vs high refresh rate. Some displays can't handle the high-res, some can. More on that later.<br>
<br>
<br>
Below is a Test-Patten showing its high-res capabilities. Rotated 90 degrees due to limited row-memory's limiting long rows of horizontal text.<br>
The color-pattern at the top uses up the remaining row-segment-memory (intentionally set low, for testing-purposes).<br>
<br>
<br>
<img src='https://sites.google.com/site/geekattempts/SEG_SINE_LQ.jpg' />
(SEG_SINE)<br>
<br>
In high-res/low-refresh mode, it currently runs at a maximum resolution of ~341 addressable pixels stretched across 1024 horizontal, by 768 vertical, 48-colors, ~1Hz refresh. In the works: DOUBLED horizontal resolution, so ~680x768 addressable pixels. (Horizontally limited by memory to ~100 "color segments", but they can be any multiple of 1/680th of a screenwidth in length). The current refresh-rate is <b>just about the limit</b> for the Samsung TFTs' ability to retain the image. It's possible to see some degredation near the bottom of the screen (which varies depending on the image above the refresh-line, an interesting effect).<br>
<br>
<img src='https://sites.google.com/site/geekattempts/SEG_RACER2_LQ.jpg' />
(SEG_RACER)<br>
<br>
Higher refresh rates are possible, at the expense of horizontal resolution. (As seen in the video and images below)<br>
Low-res/high-refresh mode hasn't been particularly optimized, but as it stands 16x16 pixels stretched to fill the screen, 16Hz refresh. With the Chi Mei display, refresh is invisible, the Samsung display has noticeable flicker.<br>
<br>
Hardware Specs:<br>
<blockquote>(Don't be fooled by the PCB, I just had a bunch of 'em from another project and needed a tiny-pitch breakout for the LCD cable I had handy. Listed are the <b>only</b> parts necessary for this project.)<br>
<ul><li>Atmel AVR: ATtiny861 -  512Bytes of RAM, 3x Hardware PWM channels with "Dead-Time Generators" and "complementary" outputs (in quotes, because the outputs aren't <b>exactly</b> inverted, the dead-time-generators affect the negative and positive outputs differently, which is essential for simulating FPD-Link compatible signals).<br>
</li><li>2x 74LS86 XORs (mine are from 1980)<br>
</li><li>3.6V<br>
</li><li>A forgiving LCD display (such as the two listed above). Other displays are likely to work, it's a matter of experimenting. See notes in the code and at the bottom.<br>
</li><li>LCD CCFL Inverter<br>
</li><li>(Wire, and a handful of 0.1uF Decoupling Capacitors)</li></ul></blockquote>

Shown on the display: "Racer" a game similar to one from the ol' TI-82 days... turn a potentiometer to keep the "car" (shown here as a black vertical line, too hard to vary the potentiometer and hold a camera at the same time) on the green track. This was a test to see if it was possible to make a high-paced game at slow refresh-rates (Refresh can be seen as a black horizontal line). The red track is what's ahead (wrapping around at the bottom of the screen back to the top). The current position on the track is shown in green just above the refresh-line where it follows the red-track. The previous lap is shown below the refresh-line. So, at any one time you can see the track ahead for one full screen and the track behind for one full screen. (It makes sense when seen in motion...) When the car is on-track, the course is displayed in cyan, when the car is off-track it's displayed in magenta.<br>
<blockquote>Listed Statistics:<br>
<ul><li>Level (Cyan)<br>
</li><li>Number of rows kept-on-track (Green)<br>
</li><li>Number of rows total in this game (Red)<br>
</li><li>Percent of the time on-track (Yellow)</li></ul></blockquote>

Another View:<br>
<img src='https://sites.google.com/site/geekattempts/SEG_RACER_LQ.jpg' />


Another test-pattern, was just an experiment, but I thought it looked cool. And it visualizes something else I'd been working on over the years.<br>
<br>
<img src='https://sites.google.com/site/geekattempts/SEG_HFM_LQ.jpg' />
(SEG_HFM)<br>
<br>
Another use... I envision a world with these hanging over sidewalks and people jumping up to hit the underside to get a reward. (All props to Nintendo for inspiring generations). For now, push the button and get either a flower-power, a star, a biggie-shroom, or a 1-up... Occasionally you might get a Goomba, so be on your toes and hit him again to squash him.<br>
<br>
<img src='https://sites.google.com/site/geekattempts/SEG_QUESTION_LQ.jpg' />
(SEG_QUESTION)<br>
<br>
PCB: Really, it's only being used as a breadboard with a tiny-pitch breakout.<br>
Shown with a 74LS00 NAND and a 74LS32 OR, used as a buffer and inverter. As-Wired it's pin-compatible with the 74LS86's recommended in my documentation. (And I have since swapped them out). However, ideally, (especially at faster bit/refresh rates) the buffer and inverter for each LVDS signal would come from the same chip.<br>
<img src='https://sites.google.com/site/geekattempts/PCB_TOP_LQ.jpg' />
<img src='https://sites.google.com/site/geekattempts/PCB_BOTTOM_LQ.jpg' />


The key here is that none of these "abilities" (functionality at extremely low bit-rates, retaining images at extremely low refresh-rates, syncability with <b>huge</b> horizontal front-porches, repeating of lines, etc.) are documented by the manufacturers, and in fact go completely against the documented limits. They can only be found by experimentation... And maybe a bit of understanding about what you're working with (e.g. TFTs by definition have "memory" at each pixel, so why not take advantage of that?)<br>
<br>
Other extremes having been pushed on the AVR-side include using the PLL at 128MHz (documented to "saturate" at 85MHz) running the system clock at 32MHz (via the PLL, documented limit at 16MHz). Figuring out how to use the PWM outputs to create pseudo-FPD-Link serial-data signals that actually cover a reasonable range of colors and timing-signals. Pushing 512Bytes of RAM to handle a color image at 200x768 (That'd be 153KB in a frame-buffer!), and figuring out how to stretch that into a possible resolution of 600x768 or higher...<br>
<br>
And yet even more extremes pushed: Being able to view these 128MHz signals on an analog oscilloscope rated for 20MHz. Level-triggering the 'scope on the fact that certain digital bit-patterns cause increased analog-overshoot and DC-offsets... (At these bit-rates and with this equipment, the digital bit-streams become quite a bit more analog, and therefore can not only be triggered at a certain point in the refresh, but the various porches and syncs can also be recognized when zoomed way-out).<br>
<br>
...Using TTL chips rated for 4.5-5.5V at 3.3V; rated for loads of >1Kohm at a load of 100ohms and finding the signals output are dang-near perfect for LVDS voltage-requirements. (Running them at frequencies way higher than they're rated and finding the analog effects to be <b>useful</b>).<br>
<br>
And then there're other possibilities... For instance, considering the fact that two of the serial-streams are nothing more than color-data, essentially "red" and "green", it's entirely possible to have the timing generated by the processor and image-data generated by an entirely different source. With little more than two comparators, outputs tied directly to the red and green serial channels, this could easily be turned into a dual-trace oscilloscope. With an actual LVDS serializer chip, or maybe even a simple shift-register, it could be three-trace.<br>
<br>
Then some other possibilities discovered... this "Data-Enable-Only" display (and possibly most DE-Only displays?) tries so hard to find valid syncing-signals that you can get interesting effects by simply sending a constant DE-signal... With nothing more than a crystal oscillator (27MHz in my case), two shift-registers (one for the pixel clock, and one for the sync signal), a buffer, and an inverter, (no AVR necessary!), solid-colors can be generated... Could be used, e.g., for an adjustable color-filter on a stage-light. Or by introducing a bit of randomness it creates some interesting soothing "meditation" patterns. Again, just a handful of TTL parts.<br>
<br>
It's maybe not possible with an 8-bit microcontroller to do full-color-full-resolution-fast-refresh video, but a shitton <b>is</b> possible.<br>
<br>
<hr />
<b>Display/Functionality Notes:</b>
(As of 3/4/14)<br>
<br>
Different displays have different (undocumented) functionality. With the three I've tested, I've attempted to create a step-by-step procedure to get a new display going (or find out early-on if it's not possible).<br>
<br>
The tested displays were stolen from old 12in G3/G4 iBooks. Two revisions of the same Samsung model, and a single Chi-Mei have been tested. All are pretty standard laptop TFTs: 1024x768, FPD-Link/LVDS single-channel, DE-Only displays. (The code is written to work with non DE-Only displays--those that pay attention to the H-sync and V-sync signals--but this is untested.)<br>
<br>
Each display has its quirks and its benefits.<br>
<br>
E.G.: The Samsung displays seem to work well with <b>really long</b> horizontal-blanking-periods, whereas the ChiMei display does not seem to like that at all. (This "feature" is extraordinarily handy for high-resolution/low-refresh, where each row's pixel-data is calculated <b>during</b> the horizontal-blanking-period before it).<br>
E.G.2: One revision of the Samsung display works fine at low bit-rates, but the other requires a faster bit-rate.<br>
E.G.3: The Samsung displays have visible refresh; rows are blacked-out before they're redrawn, resulting in visible flicker at high refresh-rates and noticeable scrolling at low refresh-rates.  The ChiMei does not have this affect and is visually quite appealing (but limited to low-resolution).<br>
<br>
Again, I've done my best to make the testing-procedure relatively step-by-step.<br>
<br>
The first step is pretty much go-no-go. If the display doesn't show anything (is completely blank), then it's unlikely this circuit/code will work with your display. Out of four displays, I've only had one which I couldn't get working, and the final (fateful) attempt was long before this testing procedure was established.<br>
<br>
From there, it's simply a matter of selecting increasingly complex test-patterns... first a solid blue screen, then a blue/black screen divided vertically, then diagonally, and so on. These patterns help to adjust/experiment with timing-values, which may be nowhere near the spec'd values, since we're working <b>way</b> out of specs already.<br>
(In fact, once I had a 1024x768 display which somehow magically stretched 680 pixels across the entire screen when its bit-rate was slowed... odd, since the pixel-clock is sent along with the pixel-data!).<br>
<br>
Basically, if that first test works, then it's quite likely the display can at least handle a very small frame-buffer... which is perfect for, e.g., the "Mario" project, where each full-screen image is only 16x16 anyhow. Or use it to draw solid colors, e.g. as a color-filter for a stage-light. Or, again, the circuit here could be used exclusively to drive the LCD timing signals, and the actual green/red data could come from an external source (like an analog-comparator).<br>
<br>
If you care for higher resolution, at the cost of refresh-rate, then further tests allow for testing that feasibility. Again, increasing the resolution beyond the available memory requires calculating image-data before drawing each row. This has to be done <b>somewhere</b>, so it ends up happening in the horizontal-blanking period. If the display can't handle long horizontal-blanks (like the ChiMei), then this'll be discovered early-on in that part of the testing.<br>
<br>
<br>
<br>
Certainly, this testing-procedure could stand to be improved... more displays to experiment/develop with certainly wouldn't hurt. I'm also somewhat intrigued with the idea of using this system to make use of the still-usable portions of broken displays... maybe the broken part could even be cut off with a glass-cutter, or at least covered by the project-housing. Further, I'm not 100% convinced the row-calculation stuff is a problem with the ChiMei display, so at some point it might be worth fine-tuning those tests a bit more. With its lack of a visible refresh, it would be almost ideal for 1Hz refresh at high resolution. (Except, maybe, for my "Racer" game...?).<br>
<br>
<hr />

I hope this will be useful to others! Please visit the non-code website: <a href='https://sites.google.com/site/geekattempts/'>https://sites.google.com/site/geekattempts/</a>
And if you find it handy, I can always make use of moolah or beer :)<br>
<br>
<hr />

License:<br>
My Work Here (which is dang-near everything, excluding the Mario sprites and where otherwise noted) is free to use, except for the exclusions listed. Please give credit where it's due, and include this license or something like it... If you profit off my work, I'd love a beer, and could certainly make use of cash. If you <b>intend to</b> profit from any part of my work, you need to contact me first. My work <b>may not</b> be used for military, NSA, or other creepy-ass purposes, and if there's any question in your mind you must contact me first. I shouldn't have to be more legal than this... it's called "Basic Human Decency."