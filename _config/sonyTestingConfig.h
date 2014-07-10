#ifndef __SONY_TESTING_CONFIG_H__
#define __SONY_TESTING_CONFIG_H__

//This configuration is for a Sony LCD used as a Mario Question-Box which
//happens to have a window that's slightly smaller than the display...





//This is an attempt at moving calculations *before* Hsync, so as to work
//with a non-DE display... had a *minor* effect...
//Interestingly, without this, the codesize increased from 6888 to 6906
// (No visible effect, with bitbanged)
#define WC_SETUP TRUE




//So far, this is only implemented with the Sony parallel-LCD
// It should be *really easy* to implement with other displays/interfaces
// Just grep this.
// If the display is partially covered by display-housing, then we'd like
// to fit the frame-buffer into the *visible* portion
// This'll likely be something *near* but *smaller than* DE_ACTIVE_DOTS
//(This assumes the covered-up portion is at the right of the display)
#define VISIBLE_ROW_DOTS  225
//Override the default WHITE at the end of each row with BLACK...
#define FB_END_OF_ROW_COLOR  _K
//Later, maybe, _LCD_INTERFACE_CFILE should be handled in
// _displays/sonyACX705AKM.h?
// (Though, technically, the display *could* be interfaced via an FPD-Link
//  receiver chip, using _interfaces/lvds.c... Unlikely, but maybe useful
//  to keep these things separate for future displays/interfaces?)
#define _LCD_SPECIFIC_HEADER_ "_displays/sonyACX705AKM.h"
#define _LCD_INTERFACE_CFILE_ "_interfaces/6bitParallel.c"
//Since the MISO pin is available for the heart, it's used instead of the
//default assigned by the makefile
#define _HEART_PINNUM_ PB1
#define _HEART_PINPORT_   PORTB




#endif

