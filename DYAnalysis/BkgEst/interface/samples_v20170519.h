#ifndef samples_v20170519_h
#define samples_v20170519_h

#include <vector>
#include "analysis.h"

namespace DYana_v20170519 {

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
   const SampleTag VVFirst = WW;
   const SampleTag VVLast = ZZ;
   const SampleTag WFirst = WE;
   const SampleTag WLast = WTau;
   const SampleTag DYFirst = DY1050;
   const SampleTag DYLast = DY4001000;
   const SampleTag DataFirst = Data1;
   const SampleTag DataLast = Data2;

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
         case DY1050:    return 3.*208.*4975.42; break; // MCFM only generates p(p/n) and 1 flavour
         case DY50100:   return 3.*208.*1119.50; break; // MCFM only generates p(p/n) and 1 flavour
         case DY100200:  return 3.*208.*44.2137; break; // MCFM only generates p(p/n) and 1 flavour
         case DY200400:  return 3.*208.*1.39947; break; // MCFM only generates p(p/n) and 1 flavour
         case DY4001000: return 3.*208.*0.114477; break; // MCFM only generates p(p/n) and 1 flavour
         case WW:        return 3.*3.*208.*0.678813; break; // MCFM only generates p(p/n) and 1 flavour 
         case WZ:        return 3.*(1./10.63e-2)*208.*45.9629e-3; break; // MCFM: p(p/n), W(->munu)Z(->ee) / PYTHIA: W(->X)Z(->ll)
         case ZZ:        return 3.*3.*208.*18.0587e-3; break; // MCFM only generates p(p/n) and 1 flavour
         case TT:        return 56.6e3; break; // according to DDE: NNLO (MCFM + Top++, NNPDF3.0+EPS09)
         // // PYTHIA
         // case DY1050:    return 208.*8453; break;
         // case DY50100:   return 208.*1321; break;
         // case DY100200:  return 208.*195.5; break;
         // case DY200400:  return 208.*5.94; break;
         // case DY4001000: return 208.*0.356; break;
         // case WW:        return 208.*3.728; break;
         // case WZ:        return 208.*1.339; break;
         // case ZZ:        return 208.*0.5422; break;
         // case TT:        return 208.*14.42; break;
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
   const char* PathFRHistos2(SampleTag s) {
      switch (s) {
         case DY1050:    return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeDY1050.root"; break;
         case DY50100:   return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeDY50100.root"; break;
         case DY100200:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeDY100200.root"; break;
         case DY200400:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeDY200400.root"; break;
         case DY4001000: return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeDY4001000.root"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeZZ.root"; break;
         case TT:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeTT.root"; break;
         case WE:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeWE.root"; break;
         case WMu:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeWMu.root"; break;
         case WTau:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeWTau.root"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeQCD.root"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeData2.root"; break;
         default:        return "Unknown sample";
      }
   };
   const char* PathFRHistosEmi(SampleTag s) {
      switch (s) {
         case DY1050:    return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiDY1050.root"; break;
         case DY50100:   return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiDY50100.root"; break;
         case DY100200:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiDY100200.root"; break;
         case DY200400:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiDY200400.root"; break;
         case DY4001000: return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiDY4001000.root"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiZZ.root"; break;
         case TT:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiTT.root"; break;
         case WE:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiWE.root"; break;
         case WMu:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiWMu.root"; break;
         case WTau:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiWTau.root"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiQCD.root"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiData2.root"; break;
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

   bool IsDiboson(SampleTag s) {
      if (s==WW || s==WZ || s==ZZ) return true;
      else return false;
   };

   bool IsWjets(SampleTag s) {
      if (s==WE || s==WMu || s==WTau) return true;
      else return false;
   };
};

#endif // #ifndef samples_v20170519_h
