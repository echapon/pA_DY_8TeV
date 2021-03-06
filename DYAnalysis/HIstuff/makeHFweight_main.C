#include "MakeHFweight.C"

void makeHFweight_main() {
   TFile *f = new TFile("HFweight.root","RECREATE");
   // data
   TChain *chdata = new TChain("recoTree/DYTree");
   chdata->Add("/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20170518/170517_220343/0000/ntuple_skim_*.root");
   chdata->Add("/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20170518/170517_220714/0000/ntuple_skim_*.root");
   MakeHFweight data(chdata);
   HistPack hists_data;
   hists_data.label = "data";
   data.Loop(hists_data);

   // MC
   TChain *chMC = new TChain("recoTree/DYTree");
   chMC->Add("/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/DYtoMuMu_M-30_pPb-EmbEPOS_8p16_Powheg/crab_DYtoMuMu_M-30_pPb_Powheg_20170831/170831_093521/0000/ntuple_skim*root");
   MakeHFweight MC(chMC);
   HistPack hists_MC;
   hists_MC.label = "MC";
   MC.Loop(hists_MC);


   // make data/MC ratios
   TH1F *h_hiHF_ratio = (TH1F*) hists_data.h_hiHF->Clone("h_hiHF_ratio");
   h_hiHF_ratio->Divide(hists_MC.h_hiHF);
   TH1F *h_hiNtracks_ratio = (TH1F*) hists_data.h_hiNtracks->Clone("h_hiNtracks_ratio");
   h_hiNtracks_ratio->Divide(hists_MC.h_hiNtracks);
   TH1F *h_hiHFplus_ratio = (TH1F*) hists_data.h_hiHFplus->Clone("h_hiHFplus_ratio");
   h_hiHFplus_ratio->Divide(hists_MC.h_hiHFplus);
   TH1F *h_hiHFminus_ratio = (TH1F*) hists_data.h_hiHFminus->Clone("h_hiHFminus_ratio");
   h_hiHFminus_ratio->Divide(hists_MC.h_hiHFminus);
   TH1F *h_hiHF_PV1_ratio = (TH1F*) hists_data.h_hiHF_PV1->Clone("h_hiHF_PV1_ratio");
   h_hiHF_PV1_ratio->Divide(hists_MC.h_hiHF_PV1);
   TH1F *h_hiNtracks_PV1_ratio = (TH1F*) hists_data.h_hiNtracks_PV1->Clone("h_hiNtracks_PV1_ratio");
   h_hiNtracks_PV1_ratio->Divide(hists_MC.h_hiNtracks_PV1);
   TH1F *h_hiHFplus_PV1_ratio = (TH1F*) hists_data.h_hiHFplus_PV1->Clone("h_hiHFplus_PV1_ratio");
   h_hiHFplus_PV1_ratio->Divide(hists_MC.h_hiHFplus_PV1);
   TH1F *h_hiHFminus_PV1_ratio = (TH1F*) hists_data.h_hiHFminus_PV1->Clone("h_hiHFminus_PV1_ratio");
   h_hiHFminus_PV1_ratio->Divide(hists_MC.h_hiHFminus_PV1);

   f->Write();
   f->Close();
}
