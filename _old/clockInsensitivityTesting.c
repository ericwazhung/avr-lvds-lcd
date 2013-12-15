//a/0 v24, this allows the clock signal to be affected by DT
// if it works, we'll be able to use Green on /OC1B
// Sort of works... Blues with DT=0 are good, timing's not screwed
// but blues with DT!=0 are damn-near black except for a light pixel
// before-hand
//a/0 v26, this is now required for Green IN MODES OTHER THAN DRAWPIX
// Not sure where that note makes sense...
// a/o 56-36-3ish it's been removed completely.
//DO NOT define CLOCK_INSENSITIVITY_TESTING TRUE


