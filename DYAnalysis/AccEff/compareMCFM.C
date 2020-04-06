#include "../Include/MyCanvas.C"
#include "../BkgEst/interface/defs.h"

void compareMCFM() {
   TFile *fPowheg = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_Eff_weights_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root");
   TFile *fMCFM0 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/nominal_1560/Z_only_nlo_CT14nlo_1.00_1.00_DY15600_full_NLO_muM34_EPPS.DA.root");
   TFile *fMCFM1 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/nominal_1560/Z_only_nlo_CT14nlo_90___90___DY15600_full_NLO_mu90_EPPS.DAT.root");

   const int nhists = 12;
   const char* histnamesPowheg[nhists] = {
      "MCFM/h_m34_1_0",
      "MCFM/h_m34_2_0",
      "MCFM/h_y34lM_0",
      "MCFM/h_pt34lM_0",
      "MCFM/h_phist34lM_1_0",
      "MCFM/h_phist34lM_2_0",
      "MCFM/h_y34hM_0",
      "MCFM/h_pt34hM_0",
      "MCFM/h_phist34hM_1_0",
      "MCFM/h_phist34hM_2_0",
      "MCFM/h_m34cut_1_0",
      "MCFM/h_m34cut_2_0"
   };
   const char* histnamesMCFM[nhists] = {
      "id20",
      "id21",
      "id24",
      "id25",
      "id26",
      "id27",
      "id28",
      "id29",
      "id30",
      "id31",
      "id32",
      "id33",
   };
   const char* xtitles[nhists] = {
      "mass [GeV]",
      "mass [GeV]",
      "y (15<M<60 GeV)",
      "p_{T} (15<M<60 GeV)",
      "#phi^{*} (15<M<60 GeV)",
      "#phi^{*} (15<M<60 GeV)",
      "y (60<M<120 GeV)",
      "p_{T} (60<M<120 GeV)",
      "#phi^{*} (60<M<120 GeV)",
      "#phi^{*} (60<M<120 GeV)",
      "mass [GeV]",
      "mass [GeV]"
   };

   // do the comparisons
   for (int i=0; i<nhists; i++) {
      TH1D *hPowheg = (TH1D*) fPowheg->Get(histnamesPowheg[i]);
      TH1D *hMCFM0 = (TH1D*) fMCFM0->Get(histnamesMCFM[i]);
      TH1D *hMCFM1 = (TH1D*) fMCFM1->Get(histnamesMCFM[i]);

      // factor 208 in between MCFM and Powheg
      hMCFM0->Scale(hMCFM0->GetBinWidth(1)*208./1000.);
      hMCFM1->Scale(hMCFM1->GetBinWidth(1)*208./1000.);
      // POWHEG is a number of events: divide by lumi
      hPowheg->Scale(1./lumi_all);

      MyCanvas c(histnamesMCFM[i],xtitles[i],"Xsec [pb] / bin",800,600);
      c.CanvasWithThreeHistogramsRatioPlot(hMCFM0,hMCFM1,hPowheg,
            "MCFM (auto #mu)", "MCFM (#mu=90 GeV)", "Powheg", "Powheg/MCFM");
      c.PrintCanvas();
   }
}
