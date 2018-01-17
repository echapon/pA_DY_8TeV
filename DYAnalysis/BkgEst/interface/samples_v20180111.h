#ifndef samples_v20180111_h
#define samples_v20180111_h

#include <vector>
#include "analysis.h"

namespace DYana_v20180111 {

   // list of available processes (MC + data)
   enum SampleTag : int {
      TT=0,
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
         case DYMuMu1030:return 1180.94e3; break; // Powheg
         case DYMuMu30:  return 266.271e3; break; // Powheg
         case DYMuMu1030_PbP:return 1180.94e3; break; // Powheg
         case DYMuMu30_PbP:  return 266.271e3; break; // Powheg
         case DYTauTau1030:return 1171.91e3; break; // Powheg
         case DYTauTau30:  return 265.810e3; break; // Powheg
         case WW:        return 208.*32.93; break; // PYTHIA8, inclusive
         case WZ:        return 208.*12.26; break; // PYTHIA8, inclusive
         case ZZ:        return 208.*4.755; break; // PYTHIA8, inclusive
         case TT:        return 59.0e3; break; // according to DDE: NNLO (MCFM + Top++, CT14+EPPS16)
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
         case DYMuMu1030:  return 997120; break; // was 997120 but seems to contain 10-600 // 789275
         case DYMuMu30:    return 1e6; break;
         case DYMuMu1030_PbP:  return 1e6; break; // was 1e6 but contains 10-600 // 795824
         case DYMuMu30_PbP:    return 1e6; break;
         case DYTauTau1030:return 464494; break;
         case DYTauTau30:  return 498444; break;
         case WW:          return 195810; break;
         case WZ:          return 99759; break;
         case ZZ:          return 100000; break;
         case TT:          return 100e3; break;
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
         case DYMuMu1030:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-10to30_pPb-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-10to30_pPb_Powheg_20170905/170905_103354/0000/ntuple_skim*root"; break;
         case DYMuMu30:  return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-30_pPb-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-30_pPb_Powheg_20170831/170831_093521/0000/ntuple_skim*root"; break;
         case DYMuMu1030_PbP:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-10to30_PbP_Powheg_20170817/170817_151934/0000/ntuple_skim*root"; break;
         case DYMuMu30_PbP:  return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-30_PbP-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-30_PbP_Powheg_20170817/170817_152017/0000/ntuple_skim*root"; break;
         case DYTauTau1030:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_PowhegTauola/crab_DYtoTauTau_M-10to30_20180117_2/180117_132637/0000/ntuple_skim*root"; break;
         case DYTauTau30:return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-30_PbP-EmbEPOS_8p16_PowhegTauola/crab_DYtoTauTau_M-30_20180117/180117_132803/0000/ntuple_skim*root"; break;
         case WW:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WW_PbP-EmbEPOS_8p16_Pythia8/crab_WW_20180117/180117_132840/0000/ntuple_skim*root"; break;
         case WZ:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WZ_PbP-EmbEPOS_8p16_Pythia8/crab_WZ_20180117/180117_132905/0000/ntuple_skim*root"; break;
         case ZZ:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/ZZ_PbP-EmbEPOS_8p16_Pythia8/crab_ZZ_20180117/180117_132927/0000/ntuple_skim*root"; break;
         case TT:        return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/Ttbar_PbP-EmbEPOS_8p16_Powheg/crab_Ttbar_PbP_Powheg_20170817/170817_152112/0000/ntuple_skim*root"; break;
         case WpMu:      return "eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WpToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToMuNu_PbP_Powheg_20170831/170831_093747/0000/ntuple_skim*root"; break;
         case WmMu:      return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WmToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToMuNu_PbP_Powheg_20170817/170817_151834/0000/ntuple_skim*root"; break;
         case WpTau:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WpToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToTauNu_PbP_Powheg_20170817/170817_152338/0000/ntuple_skim*.root"; break;
         case WmTau:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/WmToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToTauNu_PbP_Powheg_20170817/170817_152252/0000/ntuple_skim*.root"; break;
         case Data1:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20170518/170517_220343/0000/ntuple_skim*root"; break;
         case Data2:     return "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20170518/170517_220714/0000/ntuple_skim*root"; break;
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
         case DYMuMu1030:return "FIXME"; break;
         case DYMuMu30:  return "FIXME"; break;
         case DYMuMu1030_PbP:return "FIXME"; break;
         case DYMuMu30_PbP:  return "FIXME"; break;
         case DYTauTau1030:return "FIXME"; break;
         case DYTauTau30:return "FIXME"; break;
         case WW:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeWW.root"; break;
         case WZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeWZ.root"; break;
         case ZZ:        return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeZZ.root"; break;
         case TT:        return "FIXME"; break;
         case QCD:       return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeQCD.root"; break;
         case WpMu:      return "FIXME"; break;
         case WmMu:      return "FIXME"; break;
         case WpTau:     return "FIXME"; break;
         case WmTau:     return "FIXME"; break;
         case Data1:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeData1.root"; break;
         case Data2:     return "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/histograms/fakeData2.root"; break;
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
         case DYMuMu1030:return "DYtoMuMu_M-10to30_pPb-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-10to30_pPb_Powheg_20170905/170905_103354/0000/"; break;
         case DYMuMu30:  return "DYtoMuMu_M-30_pPb-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-30_pPb_Powheg_20170831/170831_093521/0000/"; break;
         case DYMuMu1030_PbP:return "DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-10to30_PbP_Powheg_20170817/170817_151934/0000/"; break;
         case DYMuMu30_PbP:  return "DYtoMuMu_M-30_PbP-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-30_PbP_Powheg_20170817/170817_152017/0000/"; break;
         case DYTauTau1030:return "DYtoMuMu_M-10to30_PbP-EmbEPOS_8p16_PowhegTauola/crab_DYtoTauTau_M-10to30_20180117_2/180117_132637/0000/"; break;
         case DYTauTau30:return "DYtoMuMu_M-30_PbP-EmbEPOS_8p16_PowhegTauola/crab_DYtoTauTau_M-30_20180117/180117_132803/0000/"; break;
         case WW:        return "WW_PbP-EmbEPOS_8p16_Pythia8/crab_WW_20180117/180117_132840/0000/"; break;
         case WZ:        return "WZ_PbP-EmbEPOS_8p16_Pythia8/crab_WZ_20180117/180117_132905/0000/"; break;
         case ZZ:        return "ZZ_PbP-EmbEPOS_8p16_Pythia8/crab_ZZ_20180117/180117_132927/0000/"; break;
         case TT:        return "Ttbar_PbP-EmbEPOS_8p16_Powheg/crab_Ttbar_PbP_Powheg_20170817/170817_152112/0000/"; break;
         case WpMu:      return "WpToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToMuNu_PbP_Powheg_20170831/170831_093747/0000/"; break;
         case WmMu:      return "WmToMuNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToMuNu_PbP_Powheg_20170817/170817_151834/0000/"; break;
         case WpTau:     return "WpToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WpToTauNu_PbP_Powheg_20170817/170817_152338/0000/"; break;
         case WmTau:     return "WmToTauNu_PbP-EmbEPOS_8p16_Powheg/crab_WmToTauNu_PbP_Powheg_20170817/170817_152252/0000/"; break;
         case QCD:       return "QCDtoMu_pThat-20_PbP-EmbEPOS_8p16_Pythia8/crab_QCDtoMu_20180117/180117_132953/0000/"; break;
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

#endif // #ifndef samples_v20180111_h
