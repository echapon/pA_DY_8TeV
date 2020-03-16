#ifndef samples_v20180814_h
#define samples_v20180814_h

#include <vector>
#include "analysis.h"

namespace DYana_v20180814 {

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
      v.push_back(TT);
      v.push_back(TW);
      v.push_back(TbarW);
      v.push_back(WW);
      v.push_back(WZ);
      v.push_back(ZZ);
      v.push_back(WpMu);
      v.push_back(WmMu);
      v.push_back(WpTau);
      v.push_back(WmTau);
      v.push_back(DYMuMu1030);
      v.push_back(DYMuMu30);
      v.push_back(DYMuMu1030_PbP);
      v.push_back(DYMuMu30_PbP);
      v.push_back(DYTauTau1030);
      v.push_back(DYTauTau30);
      v.push_back(Data1);
      v.push_back(Data2);
      return v;
   };


   // xsec for each process (in pb)
   double Xsec(SampleTag s) {
      switch (s) {
         // MCFM NLO CT14 pPb
         case DYMuMu1030:return 3622.08*208; break; // Powheg
         case DYMuMu30:  return 1353.47*208; break; // Powheg
         case DYMuMu1030_PbP:return 3622.08*208; break; // Powheg
         case DYMuMu30_PbP:  return 1353.47*208; break; // Powheg
         case DYTauTau1030:return 3622.08*208; break; // Powheg
         case DYTauTau30:  return 1353.47*208; break; // Powheg
         case WW:        return 208.*60.1; break; // CMS pp 8TeV: SMP-14-016
         case WZ:        return 208.*24.09; break; // CMS pp 8TeV: SMP-14-014
         case ZZ:        return 208.*7.7; break; // CMS pp 8TeV: SMP-13-005
         case TT:        return 45e3; break; // CMS pPb 8.16TeV: HIN-17-002
         case TW:        return 12.2704*208; break; // Powheg NLO
         case TbarW:     return 12.20*208; break; // Powheg NLO
         // // PYTHIA
         // case WW:        return 208.*3.728; break;
         // case WZ:        return 208.*1.339; break;
         // case ZZ:        return 208.*0.5422; break;
         // case TT:        return 208.*14.42; break;
         case WpMu:      return 1213.72e3; break; // Powheg, from AN-17-058
         case WpTau:     return 1146.47e3; break;
         case WmMu:      return 1082.81e3; break;
         case WmTau:     return 1022.86e3; break;
         case QCD:       return 208.*3.68e11*0.00039; break;
         default:        return -1;
      }
   };

   // Nevts for each process
   int Nevts(SampleTag s) {
      switch (s) {
         case DYMuMu1030:  return 998000; break; // was 997120 but seems to contain 10-600 // 789275
         case DYMuMu30:    return 993700; break;
         case DYMuMu1030_PbP:  return 998900; break; // was 1e6 but contains 10-600 // 795824
         case DYMuMu30_PbP:    return 999900; break;
         case DYTauTau1030:return 498400; break;
         case DYTauTau30:  return 478800; break;
         case WW:          return 195810; break;
         case WZ:          return 99759; break;
         case ZZ:          return 100000; break;
         case TT:          return 100e3; break;
         case TW:          return 98400; break;
         case TbarW:       return 48400; break;
         case WpMu:        return 981874; break;
         case WmMu:        return 998908; break;
         case WpTau:       return 498212; break;
         case WmTau:       return 430894; break;
         case QCD:         return 968753; break;
         default:          return -1;
      }
   };

   // Is it pPb or PbP? (aka do we need to switch the eta sign?)
   int switcheta(SampleTag s) {
      switch (s) {
         case Data1:       return -1; break;
         case Data2:       return 1; break;
         case DYMuMu1030:  return 1; break;
         case DYMuMu30:    return 1; break;
         case DYMuMu1030_PbP:  return -1; break;
         case DYMuMu30_PbP:    return -1; break;
         case DYTauTau1030:return -1; break;
         case DYTauTau30:  return -1; break;
         case WW:          return -1; break;
         case WZ:          return -1; break;
         case ZZ:          return -1; break;
         case TT:          return -1; break;
         case TW:          return -1; break;
         case TbarW:       return -1; break;
         case WpMu:        return -1; break;
         case WmMu:        return -1; break;
         case WpTau:       return -1; break;
         case WmTau:       return -1; break;
         case QCD:         return -1; break;
         default:          return 1;
      }
   };

   // path to the ntuple
   const char* PathTuple(SampleTag s) {
      switch (s) {
         case DYMuMu1030:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-10to30_pPb-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-10to30_pPb-EmbEPOS_8p16_PowhegNoEW_moretrees_20180727/180727_131846/0000/ntuple_skim*root"; break;
         case DYMuMu30:  return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-30_pPb-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-30_pPb-EmbEPOS_8p16_PowhegNoEW_moretrees_20180727/180727_131926/0000/ntuple_skim*root"; break;
         case DYMuMu1030_PbP:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEW_moretrees_20180803/180803_092637/0000/ntuple_skim*root"; break;
         case DYMuMu30_PbP:  return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-30_PbP-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-30_PbP-EmbEPOS_8p16_PowhegNoEW_moretrees_20180803/180803_092814/0000/ntuple_skim*root"; break;
         case DYTauTau1030:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoTauTau_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEWTauola/crab_DYtoTauTau_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEWTauola_moretrees_20180727/180727_131548/0000/ntuple_skim*root"; break;
         case DYTauTau30:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoTauTau_M-30_PbP-EmbEPOS_8p16_PowhegNoEWTauola/crab_DYtoTauTau_M-30_PbP-EmbEPOS_8p16_PowhegNoEWTauola_moretrees_20180727/180727_131702/0000/ntuple_skim*root"; break;
         case WW:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WW_PbP-EmbEPOS_8p16_Pythia8/crab_WW_20180117/180117_132840/0000/ntuple_skim*root"; break;
         case WZ:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WZ_PbP-EmbEPOS_8p16_Pythia8/crab_WZ_20180117/180117_132905/0000/ntuple_skim*root"; break;
         case ZZ:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/ZZ_PbP-EmbEPOS_8p16_Pythia8/crab_ZZ_20180117/180117_132927/0000/ntuple_skim*root"; break;
         case TT:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/Ttbar_PbP-EmbEPOS_8p16_Powheg/crab_Ttbar_PbP_Powheg_20170817/170817_152112/0000/ntuple_skim*root"; break;
         case TW:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/TW_PbP-EmbEPOS_8p16_Powheg/crab_TW_PbP-EmbEPOS_8p16_Powheg_moretrees_20180727/180727_131747/0000/ntuple_skim*root"; break;
         case TbarW:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/TbarW_PbP-EmbEPOS_8p16_Powheg/crab_TbarW_PbP-EmbEPOS_8p16_Powheg_moretrees_20180827/180827_150711/0000/ntuple_skim*root"; break;
         case WpMu:      return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WpToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToMuNu_PbP_Powheg_20170831/170831_093747/0000/ntuple_skim*root"; break;
         case WmMu:      return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WmToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToMuNu_PbP_Powheg_20170817/170817_151834/0000/ntuple_skim*root"; break;
         case WpTau:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WpToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToTauNu_PbP_Powheg_20170817/170817_152338/0000/ntuple_skim*.root"; break;
         case WmTau:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WmToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToTauNu_PbP_Powheg_20170817/170817_152252/0000/ntuple_skim*.root"; break;
         case Data1:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20191211/191211_172332/0000/ntuple_skim*root"; break;
         case Data2:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20191217/191217_083528/0000/ntuple_skim*root"; break;
         case QCD:       return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/QCDtoMu_pThat-20_PbP-EmbEPOS_8p16_Pythia8/crab_QCDtoMu_20180117/180117_132953/0000/ntuple_skim*root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to the histos for emu
   const char* PathHistos(SampleTag s) {
      switch (s) {
         case DYMuMu1030:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYMuMu1030.root"; break;
         case DYMuMu30:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYMuMu30.root"; break;
         case DYMuMu1030_PbP:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYMuMu1030_PbP.root"; break;
         case DYMuMu30_PbP:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYMuMu30_PbP.root"; break;
         case DYTauTau1030:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYTauTau1030.root"; break;
         case DYTauTau30:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histDYTauTau30.root"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histZZ.root"; break;
         case TT:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histTT.root"; break;
         case TW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histTW.root"; break;
         case TbarW:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histTW.root"; break;
         case WpMu:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWpMu.root"; break;
         case WmMu:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWmMu.root"; break;
         case WpTau:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWpTau.root"; break;
         case WmTau:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histWmTau.root"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histQCD.root"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histData2.root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to the histos for fake rate
   const char* PathFRHistos(SampleTag s) {
      switch (s) {
         case DYMuMu1030:return "FIXME"; break;
         case DYMuMu30:  return "FIXME"; break;
         case DYMuMu1030_PbP:return "FIXME"; break;
         case DYMuMu30_PbP:  return "FIXME"; break;
         case DYTauTau1030:return "FIXME"; break;
         case DYTauTau30:return "FIXME"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRZZ.root"; break;
         case TT:        return "FIXME"; break;
         case TW:        return "FIXME"; break;
         case TbarW:     return "FIXME"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRQCD.root"; break;
         case WpMu:      return "FIXME"; break;
         case WmMu:      return "FIXME"; break;
         case WpTau:     return "FIXME"; break;
         case WmTau:     return "FIXME"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFRData2.root"; break;
         default:        return "Unknown sample";
      }
   };
   const char* PathFRHistos2(SampleTag s) {
      switch (s) {
         case DYMuMu1030:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYMuMu1030.root"; break;
         case DYMuMu30:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYMuMu30.root"; break;
         case DYMuMu1030_PbP:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYMuMu1030_PbP.root"; break;
         case DYMuMu30_PbP:  return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYMuMu30_PbP.root"; break;
         case DYTauTau1030:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYTauTau1030.root"; break;
         case DYTauTau30:return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeDYTauTau30.root"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeZZ.root"; break;
         case TT:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeTT.root"; break;
         case TW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeTW.root"; break;
         case TbarW:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeTbarW.root"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeQCD.root"; break;
         case WpMu:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeWpMu.root"; break;
         case WmMu:      return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeWmMu.root"; break;
         case WpTau:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeWpTau.root"; break;
         case WmTau:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeWmTau.root"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/histograms/fakeData2.root"; break;
         default:        return "Unknown sample";
      }
   };
   const char* PathFRHistosEmi(SampleTag s) {
      switch (s) {
         case DYMuMu1030:return "FIXME"; break;
         case DYMuMu30:  return "FIXME"; break;
         case DYMuMu1030_PbP:return "FIXME"; break;
         case DYMuMu30_PbP:  return "FIXME"; break;
         case DYTauTau1030:return "FIXME"; break;
         case DYTauTau30:return "FIXME"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiZZ.root"; break;
         case TT:        return "FIXME"; break;
         case TW:        return "FIXME"; break;
         case TbarW:     return "FIXME"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiQCD.root"; break;
         case WpMu:      return "FIXME"; break;
         case WmMu:      return "FIXME"; break;
         case WpTau:     return "FIXME"; break;
         case WmTau:     return "FIXME"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/histFREmiData2.root"; break;
         default:        return "Unknown sample";
      }
   };

   // path to ntuple in EOS
   const char* NtupleDir(SampleTag s) {
      switch(s) {
         case DYMuMu1030:return "DYtoMuMu_M-10to30_pPb-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-10to30_pPb-EmbEPOS_8p16_PowhegNoEW_moretrees_20180727/180727_131846/0000"; break;
         case DYMuMu30:  return "DYtoMuMu_M-30_pPb-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-30_pPb-EmbEPOS_8p16_PowhegNoEW_moretrees_20180727/180727_131926/0000/"; break;
         case DYMuMu1030_PbP:return "DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEW_moretrees_20180803/180803_092637/0000/"; break;
         case DYMuMu30_PbP:  return "DYtoMuMu_M-30_PbP-EmbEPOS_8p16_PowhegNoEW/crab_DYtoMuMu_M-30_PbP-EmbEPOS_8p16_PowhegNoEW_moretrees_20180803/180803_092814/0000/"; break;
         case DYTauTau1030:return "DYtoTauTau_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEWTauola/crab_DYtoTauTau_M-10to30_PbP-EmbEPOS_8p16_PowhegNoEWTauola_moretrees_20180727/180727_131548/0000/"; break;
         case DYTauTau30:return "DYtoTauTau_M-30_PbP-EmbEPOS_8p16_PowhegNoEWTauola/crab_DYtoTauTau_M-30_PbP-EmbEPOS_8p16_PowhegNoEWTauola_moretrees_20180727/180727_131702/0000/"; break;
         case WW:        return "WW_PbP-EmbEPOS_8p16_Pythia8/crab_WW_20180117/180117_132840/0000/"; break;
         case WZ:        return "WZ_PbP-EmbEPOS_8p16_Pythia8/crab_WZ_20180117/180117_132905/0000/"; break;
         case ZZ:        return "ZZ_PbP-EmbEPOS_8p16_Pythia8/crab_ZZ_20180117/180117_132927/0000/"; break;
         case TT:        return "Ttbar_PbP-EmbEPOS_8p16_Powheg/crab_Ttbar_PbP_Powheg_20170817/170817_152112/0000/"; break;
         case TW:        return "TW_PbP-EmbEPOS_8p16_Powheg/crab_TW_PbP-EmbEPOS_8p16_Powheg_moretrees_20180727/180727_131747/0000/"; break;
         case TbarW:     return "TbarW_PbP-EmbEPOS_8p16_Powheg/crab_TbarW_PbP-EmbEPOS_8p16_Powheg_moretrees_20180827/180827_150711/0000/"; break;
         case WpMu:      return "WpToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToMuNu_PbP_Powheg_20170831/170831_093747/0000/"; break;
         case WmMu:      return "WmToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToMuNu_PbP_Powheg_20170817/170817_151834/0000/"; break;
         case WpTau:     return "WpToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToTauNu_PbP_Powheg_20170817/170817_152338/0000/"; break;
         case WmTau:     return "WmToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToTauNu_PbP_Powheg_20170817/170817_152252/0000/"; break;
         case QCD:       return "QCDtoMu_pThat-20_PbP-EmbEPOS_8p16_Pythia8/crab_QCDtoMu_20180117/180117_132953/0000/"; break;
         case Data1:     return "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20191211/191211_172332/0000/"; break;
         case Data2:     return "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20191217/191217_083528/0000/"; break;
         default:        return "Unknown sample";
      }
   };

   // sample names
   const char* Name(SampleTag s) {
      switch (s) {
         case DYMuMu1030:return "DYMuMu1030"; break;
         case DYMuMu30:  return "DYMuMu30"; break;
         case DYMuMu1030_PbP:return "DYMuMu1030_PbP"; break;
         case DYMuMu30_PbP:  return "DYMuMu30_PbP"; break;
         case DYTauTau1030:return "DYTauTau1030"; break;
         case DYTauTau30:return "DYTauTau30"; break;
         case WW:        return "WW"; break;   
         case WZ:        return "WZ"; break;   
         case ZZ:        return "ZZ"; break;   
         case TT:        return "TT"; break;   
         case TW:        return "TW"; break;   
         case TbarW:     return "TbarW"; break;   
         case WpMu:      return "WpMu"; break;
         case WmMu:      return "WmMu"; break;
         case WpTau:     return "WpTau"; break;
         case WmTau:     return "WmTau"; break;
         case QCD:       return "QCD"; break;
         case Data1:     return "Data1"; break;
         case Data2:     return "Data2"; break;
         default:        return "UnknownSample";
      }
   };

   bool IsData(SampleTag s) {
      if (s==Data1 || s==Data2) return true;
      else return false;
   };

   bool IsDY(SampleTag s) {
      if (s==DYMuMu1030 || s==DYMuMu30
            || s==DYMuMu1030_PbP || s==DYMuMu30_PbP
            || s==DYTauTau1030 || s==DYTauTau30) 
         return true;
      else return false;
   };

   bool IsDYMuMu(SampleTag s) {
      if (s==DYMuMu1030 || s==DYMuMu30 || s==DYMuMu1030_PbP || s==DYMuMu30_PbP) return true;
      else return false;
   };

   bool IsDYTauTau(SampleTag s) {
      if (s==DYTauTau1030 || s==DYTauTau30) return true;
      else return false;
   };

   bool IsDiboson(SampleTag s) {
      if (s==WW || s==WZ || s==ZZ) return true;
      else return false;
   };

   bool IsWjets(SampleTag s) {
      if (s==WpMu || s==WpTau || s==WmMu || s==WmTau) return true;
      else return false;
   };
};

#endif // #ifndef samples_v20180814_h
