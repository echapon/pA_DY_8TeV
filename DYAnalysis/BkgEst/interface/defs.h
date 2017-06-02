#ifndef defs_h
#define defs_h

#include <vector>
#include "analysis.h"

// UPDATED IN 2017
// brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285479-285832_HI8TeV_PromptReco_pPb_Collisions16_JSON_NoL1T.txt --hltpath HLT_PAL3Mu12_v1
const double lumi_part1 = 64413.589e-6; // mub-1 -> pb-1
// brilcalc lumi -i /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285952-286496_HI8TeV_PromptReco_Pbp_Collisions16_JSON_NoL1T.txt --hltpath HLT_PAL3Mu12_v1
const double lumi_part2 = 115274.084e-6; // mub-1 -> pb-1
const double lumi_all = lumi_part1 + lumi_part2;

namespace DYana {

   // list of available processes (MC + data)
   enum SampleTag : int {
      TT=0,
      WW,
      WZ,
      ZZ,
      WE,
      WMu,
      WTau,
      DY1050,
      DY50100,
      DY100200,
      DY200400,
      DY4001000,
      Data1,
      Data2,
      ALL,
      QCD // put QCD after all so that it is not included in the "default" list of samples
   };
   const int NSamples = ALL;

   std::vector<SampleTag> allSamples() {
      std::vector<SampleTag> v;
      v.push_back(TT);
      v.push_back(WW);
      v.push_back(WZ);
      v.push_back(ZZ);
      v.push_back(WE);
      v.push_back(WMu);
      v.push_back(WTau);
      v.push_back(DY1050);
      v.push_back(DY50100);
      v.push_back(DY100200);
      v.push_back(DY200400);
      v.push_back(DY4001000);
      v.push_back(Data1);
      v.push_back(Data2);
      return v;
   };


   // xsec for each process (in pb)
   double Xsec(SampleTag s) {
      switch (s) {
         // MCFM
         case DY1050:    return 208.*4975.42; break;
         case DY50100:   return 208.*1119.50; break;
         case DY100200:  return 208.*44.2137; break;
         case DY200400:  return 208.*1.39947; break;
         case DY4001000: return 208.*0.114477; break;
         // // PYTHIA
         // case DY1050:    return 208.*8453; break;
         // case DY50100:   return 208.*1321; break;
         // case DY100200:  return 208.*195.5; break;
         // case DY200400:  return 208.*5.94; break;
         // case DY4001000: return 208.*0.356; break;
         case WW:        return 208.*3.728; break;
         case WZ:        return 208.*1.339; break;
         case ZZ:        return 208.*0.5422; break;
         case TT:        return 208.*14.42; break;
         case WE:        return 208.*4199; break;
         case WMu:       return 208.*4579; break;
         case WTau:      return 208.*3600; break;
         case QCD:       return 208.*3.68e11*0.00039; break;
         default:        return -1;
      }
   };

   // Nevts for each process
   int Nevts(SampleTag s) {
      switch (s) {
         case DY1050:    return 100e3; break;
         case DY50100:   return 99e3; break;
         case DY100200:  return 100e3; break;
         case DY200400:  return 100e3; break;
         case DY4001000: return 94e3; break;
         case WW:        return 100e3; break;
         case WZ:        return 97e3; break;
         case ZZ:        return 100e3; break;
         case TT:        return 94e3; break;
         case WE:        return 99e3; break;
         case WMu:       return 99e3; break;
         case WTau:      return 100e3; break;
         case QCD:       return 905011; break;
         default:        return -1;
      }
   };

   // path to the ntuple
   const char* PathTuple(SampleTag s) {
      switch (s) {
         case DY1050:    return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_DY1050.root"; break;
         case DY50100:   return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_DY50100.root"; break;
         case DY100200:  return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_DY100200.root"; break;
         case DY200400:  return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_DY200400.root"; break;
         case DY4001000: return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_DY4001000.root"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_WW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_WZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_ZZ.root"; break;
         case TT:        return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_TT.root"; break;
         case WE:        return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_WENu.root"; break;
         case WMu:       return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_WMuNu.root"; break;
         case WTau:      return "/afs/cern.ch/user/e/echapon/workspace/public/DY_pA_2016/trees_20170518/tree_WTauNu.root"; break;
                         // case Data1:     return "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/trees_20170518/tree_data1.root"; break;
         case Data1:     return "/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20170518/170517_220343/0000/ntuple_skim_*root"; break;
                         // case Data2:     return "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/trees_20170518/tree_data2.root"; break;
         case Data2:     return "/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20170518/170517_220714/0000/ntuple_skim_*root"; break;
         case QCD:       return "/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/DYtuples/QCD/crab_QCD_DYntuple_20170522/170522_124914/0000/ntuple_skim*root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to the histos for emu
   const char* PathHistos(SampleTag s) {
      switch (s) {
         case DY1050:    return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDY1050.root"; break;
         case DY50100:   return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDY50100.root"; break;
         case DY100200:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDY100200.root"; break;
         case DY200400:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDY200400.root"; break;
         case DY4001000: return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDY4001000.root"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histZZ.root"; break;
         case TT:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histTT.root"; break;
         case WE:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWE.root"; break;
         case WMu:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWMu.root"; break;
         case WTau:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWTau.root"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histQCD.root"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histData2.root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to the histos for fake rate
   const char* PathFRHistos(SampleTag s) {
      switch (s) {
         case DY1050:    return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRDY1050.root"; break;
         case DY50100:   return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRDY50100.root"; break;
         case DY100200:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRDY100200.root"; break;
         case DY200400:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRDY200400.root"; break;
         case DY4001000: return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRDY4001000.root"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRZZ.root"; break;
         case TT:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRTT.root"; break;
         case WE:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRWE.root"; break;
         case WMu:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRWMu.root"; break;
         case WTau:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRWTau.root"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRQCD.root"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRData2.root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to ntuple in EOS
   const char* NtupleDir(SampleTag s) {
      switch(s) {
         case DY1050:    return "Pyquen_DYtoLL_M_10to50_embd_pPb/crab_DY1050_DYntuple_20170516/170517_221329/0000/";
         case DY50100:   return "Pyquen_DYtoLL_M_50to100_embd_pPb/crab_DY50100_DYntuple_20170516/170517_221504/0000/";
         case DY100200:  return "Pyquen_DYtoLL_M_100to200_embd_pPb/crab_DY100200_DYntuple_20170516/170517_221541/0000/";
         case DY200400:  return "Pyquen_DYtoLL_M_200to400_embd_pPb/crab_DY200400_DYntuple_20170516/170517_221931/0000/";
         case DY4001000: return "Pyquen_DYtoLL_M_400to1000_embd_pPb/crab_DY4001000_DYntuple_20170516/170517_222251/0000/";
         case WW:        return "Pythia6_WW_embd_pPb/crab_WW_DYntuple_20170518/170517_222918/0000/";
         case WZ:        return "Pythia6_WZ_embd_pPb/crab_WZ_DYntuple_20170518/170517_223145/0000/";
         case ZZ:        return "Pythia6_ZZ_embd_pPb/crab_ZZ_DYntuple_20170518/170517_223512/0000/";
         case TT:        return "Pythia6_TTall_embd_pPb/crab_TTall_DYntuple_20170518/170517_224711/0000/";
         case WE:        return "Pyquen_WjetsToENu_embd_pPb/crab_WjetsENu_DYntuple_20170518/170517_223816/0000/";
         case WMu:       return "Pyquen_WjetsToMuNu_embd_pPb/crab_WjetsToMuNu_DYntuple_20170516/170516_163243/0000/";
         case WTau:      return "Pyquen_WjetsToTauNu_embd_pPb/crab_WjetsTauNu_DYntuple_20170518/170517_224500/0000/";
         case QCD:       return "QCD/crab_QCD_DYntuple_20170522/170522_124914/0000/";
         default:        return "Unknown sample";
      }
   };

   // sample names
   const char* Name(SampleTag s) {
      switch (s) {
         case DY1050:    return "DY1050"; break;
         case DY50100:   return "DY50100"; break;
         case DY100200:  return "DY100200"; break;
         case DY200400:  return "DY200400"; break;
         case DY4001000: return "DY4001000"; break;
         case WW:        return "WW";   
         case WZ:        return "WZ";   
         case ZZ:        return "ZZ";   
         case TT:        return "TT";   
         case WE:        return "WE";
         case WMu:       return "WMu";
         case WTau:      return "WTau";
         case QCD:       return "QCD";
         case Data1:     return "Data1";
         case Data2:     return "Data2";
         default:        return "UnknownSample";
      }
   };

   bool IsData(SampleTag s) {
      if (s==Data1 || s==Data2) return true;
      else return false;
   };

   bool IsDY(SampleTag s) {
      if (s==DY1050 || s==DY50100 || s==DY100200 || s==DY200400 || s==DY4001000) return true;
      else return false;
   };

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
      return (run>285470 && run<285900) ? -1 : 1;
   };
};

#endif // ifndef def_h
