#ifndef defs_h
#define defs_h

#include <vector>
#include "analysis.h"
#include "samples_v20170519.h"
#include "samples_v20170830.h"
#include "samples_v20170830_Pyquen.h"

// UPDATED IN 2017
// 1st part, pPb
// brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_HI2016.json -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285479-285832_HI8TeV_PromptReco_pPb_Collisions16_JSON_NoL1T.txt --hltpath HLT_PAL3Mu12_v1
const double lumi_part1 = 62587.059e-6; // mub-1 -> pb-1
// 2nd part, PbP
// brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/Normtags/normtag_HI2016.json -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285952-286496_HI8TeV_PromptReco_Pbp_Collisions16_JSON_NoL1T.txt --hltpath HLT_PAL3Mu12_v1
const double lumi_part2 = 111912.220e-6; // mub-1 -> pb-1
const double lumi_all = lumi_part1 + lumi_part2;


namespace DYana {
   // put the default samples here
   using namespace DYana_v20170830;

   // kinematic bins
   // mass
   const int binnum = 13;
   double bins[14] = {15,20,30,40,50,60,76,86,96,106,120,150,200,600};
   // pt
   const int ptbinnum_endcap = 10;
   double ptbin_endcap[ptbinnum_endcap+1] = {15,20,25,30,35,40,50,70,100,200,500};
   const int ptbinnum = 13;
   double ptbin[ptbinnum+1] = {15,17,20,22,25,30,35,40,45,50,70,100,200,500};
   const int etabinnum = 2;
   double etabin[etabinnum+1] = {0,1.2,2.4};

   // object selection
   bool MuSel(PhysicsMuon *mu) {
      return mu->acceptance(10,2.4) && mu->tightMuonID() && mu->isolation(0.15);
   };
   bool EleSel(PhysicsElectron *el) {
      return el->acceptance(20,2.4) && el->WPMedium();
   };

   // sgn switch pA vs Ap
   int runsgn(int run) { 
      // 285480-285832: PbP
      // 285956-286496: pPb
      return (run>285470 && run<285900) ? -1 : 1;
   };
};

#endif // ifndef def_h
