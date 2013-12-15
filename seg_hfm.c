
void segHFM_loadRow(uint16_t rowNum) {
   segClear();
   //Just some experimenting... 1024/3+16 seems to be the width...
   // == 357 segments (Where does this come from?)
//Pixel Segments...
#define BORDER_PSEGS   1
#define SEGS_AVAILABLE (NUM_SEGMENTS-2)
#define PSEGS_AVAILABLE   (NUM_PSEGS-BORDER_PSEGS*2)
#define HFM_PSEGS   (PSEGS_AVAILABLE/SEGS_AVAILABLE)
#define HFM_PSEGS_REMAINDER   (PSEGS_AVAILABLE%SEGS_AVAILABLE)
   addSegfb(BORDER_PSEGS,_W);

   hfm_t hfmThing;
   hfm_setup(&hfmThing, (uint8_t)((uint32_t)rowNum*(uint32_t)SEGS_AVAILABLE/V_COUNT), SEGS_AVAILABLE);

   hfm_t hfmColStretcher;
   hfm_setup(&hfmColStretcher, HFM_PSEGS_REMAINDER, SEGS_AVAILABLE);

   uint16_t pseg;
   for(pseg=0; pseg<SEGS_AVAILABLE; pseg++)
   {
      if(hfm_nextOutput(&hfmThing))
         addSegfb(HFM_PSEGS + hfm_nextOutput(&hfmColStretcher),_R);
      else
         addSegfb(HFM_PSEGS + hfm_nextOutput(&hfmColStretcher),_C);
   }

   //addSegfb(NUM_PSEGS-BORDER_PSEGS*2-SEGS_AVAILABLE*HFM_PSEGS, _M);
   addSegfb(BORDER_PSEGS,_W);
   segTerminate();
}
