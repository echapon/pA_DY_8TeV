#ifndef samples_v20200901_MG5_h
#define samples_v20200901_MG5_h

#include <vector>
#include "analysis.h"

namespace DYana_v20200901_MG5 {

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
      // v.push_back(DYMuMu1030_PbP);
      v.push_back(DYMuMu30_PbP);
      return v;
   };


   // xsec for each process (in pb)
   double Xsec(SampleTag s) {
      switch (s) {
         case DYMuMu30_PbP:  return 3958*208.; break; // 6570.3892586*208.; break; 
         default:        return -1;
      }
   };

   // Nevts for each process
   int Nevts(SampleTag s) {
      switch (s) {
         // case DYMuMu1030_PbP:  return 1980000; break; // was 1e6 but contains 10-600 // 795824
         case DYMuMu30_PbP:    return 1989442; break; //5000000; break;
         default:          return -1;
      }
   };

   // Is it pPb or PbP? (aka do we need to switch the eta sign?)
   int switcheta(SampleTag s) {
      switch (s) {
         // case DYMuMu1030_PbP:  return -1; break;
         case DYMuMu30_PbP:    return -1; break;
         default:          return 1;
      }
   };

   // path to the ntuple
   const char* PathTuple(SampleTag s) {
      switch (s) {
         // case DYMuMu1030_PbP:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DY_M_10_30_mumu_8p16TeV_Pbp_CT14_noEWK_Z/crab_DYtoMuMu_M-10_30_PbP_powheg_CT14_noEWK_genonly_20190928_2/190928_173808/0000/ntuple_skim_*.root"; break;
         case DYMuMu30_PbP:  return "/eos/cms/store/group/phys_heavyions/echapon/HIN18003/MG5/dyellell012j_5f_NLO_FXFX_8p16TeV_Pythia8Hadronizer/ntuple/ntuple_all.root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to the histos for emu
   const char* PathHistos(SampleTag s) {
      switch (s) {
         // case DYMuMu1030_PbP:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYMuMu1030_PbP.root"; break;
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
         // case DYMuMu1030_PbP:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYMuMu1030_PbP.root"; break;
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
         // case DYMuMu1030_PbP:return "DY_M_10_30_mumu_8p16TeV_Pbp_CT14_noEWK_Z/crab_DYtoMuMu_M-10_30_PbP_powheg_CT14_noEWK_genonly_20190928_2/190928_173808/0000"; break;
         case DYMuMu30_PbP:  return "dyellell012j_5f_NLO_FXFX_8p16TeV_Pythia8Hadronizer/"; break;
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

#endif // #ifndef samples_v20200901_MG5_h
