#ifndef __GPIXELVAL_H__
#define __GPIXELVAL_H__


//So far, all icons are the same height, and this is the only shared-space.
#define ICON_HEIGHT	16
#define ICON_WIDTH	16


//The gimp pixels are 0-48 in value, we use 2 bits per color, which is 64
// despite the fact that blue 0=1 (or 2=3?)
// Might be able to get away with just + 0x10 in all cases?
#define gimpPixelValToLColor(gPixelVal) \
	((((gPixelVal)&0x30)<0x20) ? (gPixelVal) : ((gPixelVal)+0x10))

/*  GIMP header image file format (INDEXED): /Users/meh/Desktop/Mario/1up.h  */
#endif

