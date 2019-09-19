#ifndef samples_v20180814_CT14_h
#define samples_v20180814_CT14_h

#include <vector>
#include "analysis.h"

namespace DYana_v20180814_CT14 {

   // list of available processes (MC + data)
   enum SampleTag : int {
      TW=0,
      TbarW,
      TT,
      WW,
      WZ,
      ZZ,
      WpMu,
      WmMu,
      WpTau,
      WmTau,
      DYTauTau1030,
      DYTauTau30,
      DYMuMu1030,
      DYMuMu30,
      DYMuMu1030_PbP,
      DYMuMu30_PbP,
      Data1,
      Data2,
      ALL,
      QCD // put QCD after all so that it is not included in the "default" list of samples
   };
   const int NSamples = ALL;
   const SampleTag VVFirst = WW;
   const SampleTag VVLast = ZZ;
   const SampleTag WFirst = WpMu;
   const SampleTag WLast = WmTau;
   const SampleTag DYFirst = DYTauTau1030;
   const SampleTag DYLast = DYMuMu30_PbP;
   const SampleTag DataFirst = Data1;
   const SampleTag DataLast = Data2;

   std::vector<SampleTag> allSamples() {
      std::vector<SampleTag> v;
      v.push_back(DYMuMu1030_PbP);
      v.push_back(DYMuMu30_PbP);
      return v;
   };


   // xsec for each process (in pb)
   double Xsec(SampleTag s) {
      switch (s) {
         // MCFM NLO CT14 pPb
         case DYMuMu1030_PbP:return 6.361e3*208.; break; // Powheg
         case DYMuMu30_PbP:  return 1.357e3*208.; break; // Powheg
         default:        return -1;
      }
   };

   // Nevts for each process
   int Nevts(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:  return 1e6; break; // was 1e6 but contains 10-600 // 795824
         case DYMuMu30_PbP:    return 971000; break;
         default:          return -1;
      }
   };

   // Is it pPb or PbP? (aka do we need to switch the eta sign?)
   int switcheta(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:  return -1; break;
         case DYMuMu30_PbP:    return -1; break;
         default:          return 1;
      }
   };

   // path to the ntuple
   const char* PathTuple(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/Powheg_DY_M_10_30_mumu_PbP_CT14_GENonly/crab_DYtoMuMu_M-10_30_PbP_powheg_CT14_genonly_20180222_2/180222_132125/0000/ntuple_skim_*.root"; break;
         case DYMuMu30_PbP:  return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/Powheg_DY_M_30_mumu_PbP_CT14_GENonly/crab_DYtoMuMu_M-30_PbP_powheg_CT14_genonly_20180222/180222_102155/0000/ntuple_skim_*root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to the histos for emu
   const char* PathHistos(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYMuMu1030_PbP.root"; break;
         case DYMuMu30_PbP:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYMuMu30_PbP.root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to the histos for fake rate
   const char* PathFRHistos(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:return "FIXME"; break;
         case DYMuMu30_PbP:  return "FIXME"; break;
         default:        return "Unknown sample";
      }
   };
   const char* PathFRHistos2(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYMuMu1030_PbP.root"; break;
         case DYMuMu30_PbP:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYMuMu30_PbP.root"; break;
         default:        return "Unknown sample";
      }
   };
   const char* PathFRHistosEmi(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:return "FIXME"; break;
         case DYMuMu30_PbP:  return "FIXME"; break;
         default:        return "Unknown sample";
      }
   };

   // path to ntuple in EOS
   const char* NtupleDir(SampleTag s) {
      switch(s) {
         case DYMuMu1030_PbP:return "Powheg_DY_M_10_30_mumu_PbP_CT14_GENonly/crab_DYtoMuMu_M-10_30_PbP_powheg_CT14_genonly_20180222_2/180222_132125/0000/"; break;
         case DYMuMu30_PbP:  return "Powheg_DY_M_30_mumu_PbP_CT14_GENonly/crab_DYtoMuMu_M-30_PbP_powheg_CT14_genonly_20180222/180222_102155/0000/"; break;
         default:        return "Unknown sample";
      }
   };

   // sample names
   const char* Name(SampleTag s) {
      switch (s) {
         case DYMuMu1030_PbP:return "DYMuMu1030_PbP"; break;
         case DYMuMu30_PbP:  return "DYMuMu30_PbP"; break;
         default:        return "UnknownSample";
      }
   };

   bool IsDY(SampleTag s) {
      if (s==DYMuMu1030_PbP || s==DYMuMu30_PbP)
         return true;
      else return false;
   };

   bool IsDYMuMu(SampleTag s) {
      if (s==DYMuMu1030_PbP || s==DYMuMu30_PbP) return true;
      else return false;
   };
};

#endif // #ifndef samples_v20180814_CT14_h
