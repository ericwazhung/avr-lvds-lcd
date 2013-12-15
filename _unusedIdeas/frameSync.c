//SHOULD PROBABLY BE REVISITED
// It probably doesn't work with all cases...
// Definitely with drawPix/Images...
//This causes main to update the image *after* FRAME_UPDATE_DELAY
// a/o v59: I don't think this is used anymore...
// a/o v60: Removing it... it can be reimplemented again later if necessary
//#define FRAME_SYNC TRUE


//This was last-used in close-conjunction with FRAME_COUNT_TO_DELAY

// Implementation Details:
//  Heh, I deleted it!

// From Main loop:
//
// #if (defined(FRAME_SYNC) && FRAME_SYNC)
//		if(frameCount==0 && !getbit(OCIE0A, T0_TIMSK))
//		{
//       do-stuff (e.g. update the frame-buffer)
//    }
