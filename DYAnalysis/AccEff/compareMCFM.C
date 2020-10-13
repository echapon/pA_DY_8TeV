#include "../Include/MyCanvas.C"
#include "../BkgEst/interface/defs.h"

TH1D *compRFB(TH1D* hist); 

void compareMCFM() {
   TFile *fPowheg = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_Eff_weights_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root");
   TFile *fPowhegrewt = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_CT14_rewt_CT14nlo_EPPS16nlo_CT14nlo_Pb208.root");
   TFile *fMG5 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_MG5_rewt_CT14nlo_EPPS16nlo_CT14nlo_Pb208.root");
   TFile *fMCFM0 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/nominal_1560/Z_only_nlo_CT14nlo_1.00_1.00_DY15600_full_NLO_muM34_EPPS.DA.root");
   TFile *fMCFM1 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/nominal_1560/Z_only_nlo_CT14nlo_90___90___DY15600_full_NLO_mu90_EPPS.DAT.root");
   TFile *fMCFM2 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/nominal_1560/Z_only_nlo_CT14nlo_1.00_1.00_DY15600_full_NLO_muM34_nCTEQ.D.root");
   TFile *fMCFM_NNLO = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/NNLO/all.root");

   const int nhists = 12+2;
   const char* histnamesPowheg[nhists] = {
      "MCFM/h_m34_1_0",
      "MCFM/h_m34_2_0",
      "MCFM/h_y34lM_0",
      "MCFM/h_y34lM_0",
      "MCFM/h_pt34lM_0",
      "MCFM/h_phist34lM_1_0",
      "MCFM/h_phist34lM_2_0",
      "MCFM/h_y34hM_0",
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
      "id24fb",
      "id25",
      "id26",
      "id27",
      "id28",
      "id28fb",
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
      "|y| (15<M<60 GeV)",
      "p_{T} (15<M<60 GeV)",
      "#phi^{*} (15<M<60 GeV)",
      "#phi^{*} (15<M<60 GeV)",
      "y (60<M<120 GeV)",
      "|y| (60<M<120 GeV)",
      "p_{T} (60<M<120 GeV)",
      "#phi^{*} (60<M<120 GeV)",
      "#phi^{*} (60<M<120 GeV)",
      "mass [GeV]",
      "mass [GeV]"
   };

   // do the comparisons
   for (int i=0; i<nhists; i++) {
      TString histnameMCFM = TString(histnamesMCFM[i]).ReplaceAll("fb","");
      TH1D *hPowheg = (TH1D*) fPowheg->Get(histnamesPowheg[i]);
      TH1D *hPowhegrewt = (TH1D*) fPowhegrewt->Get(histnamesPowheg[i]);
      TH1D *hMG5 = (TH1D*) fMG5->Get(histnamesPowheg[i]);
      TH1D *hMCFM0 = (TH1D*) fMCFM0->Get(histnameMCFM);
      TH1D *hMCFM1 = (TH1D*) fMCFM1->Get(histnameMCFM);
      TH1D *hMCFM2 = (TH1D*) fMCFM2->Get(histnameMCFM);
      TH1D *hMCFM_NNLO = (TH1D*) fMCFM_NNLO->Get(histnameMCFM);

      // factor 208 in between MCFM and Powheg
      hMCFM0->Scale(hMCFM0->GetBinWidth(1)*208./1000.);
      hMCFM1->Scale(hMCFM1->GetBinWidth(1)*208./1000.);
      hMCFM2->Scale(hMCFM1->GetBinWidth(1)*208./1000.);
      hMCFM_NNLO->Scale(hMCFM1->GetBinWidth(1)*208./1000./100.);
      // POWHEG is a number of events: divide by lumi
      hPowheg->Scale(1./lumi_all);
      hPowhegrewt->Scale(1./lumi_all);
      hMG5->Scale(1.30/lumi_all); // ad hoc scale factor

      if (TString(histnamesMCFM[i]).Contains("fb")) {
         hPowheg = compRFB(hPowheg);
         hPowhegrewt = compRFB(hPowhegrewt);
         hMCFM0 = compRFB(hMCFM0);
         hMCFM1 = compRFB(hMCFM1);
         hMCFM2 = compRFB(hMCFM2);
         hMCFM_NNLO = compRFB(hMCFM_NNLO);
         hMG5 = compRFB(hMG5);
      }

      TString yaxistitle = (TString(histnamesMCFM[i]).Contains("fb")) ? "R_{FB}" : "Xsec [pb] / bin";
      MyCanvas c(histnamesMCFM[i],xtitles[i],yaxistitle,800,600);
      c.SetRatioRange(0.79,1.21);
      // c.CanvasWithThreeHistogramsRatioPlot(hMCFM0,hMCFM1,hPowheg,
      //       "MCFM (auto #mu)", "MCFM (#mu=90 GeV)", "Powheg", "Powheg/MCFM");
      // c.CanvasWithThreeHistogramsRatioPlot(hMCFM0,hMCFM2,hPowheg,
      //       "MCFM (EPPS16)", "MCFM (nCTEQ15)", "Powheg", "Powheg/MCFM");
      // c.CanvasWithThreeHistogramsRatioPlot(hMCFM0,hMCFM_NNLO,hPowheg,
      //       "MCFM (NLO)", "MCFM (NNLO)", "Powheg (NLO)", "Powheg/MCFM");
      // c.CanvasWithThreeHistogramsRatioPlot(hMCFM0,hMG5,hPowheg,
            // "MCFM (NLO)", "MG5 (NLO)", "Powheg (NLO)", "Powheg/X");
      c.CanvasWithThreeHistogramsRatioPlot(hPowhegrewt,hMG5,hPowheg,
            "Powheg (rewt)", "MG5 (NLO)", "Powheg (NLO)", "Powheg/X");
      c.PrintCanvas();
      c.PrintCanvas_PNG();
   }
}

void compareMCFM_CT14() {
   TFile *fPowhegEPPS = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_Eff_weights_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root");
   TFile *fPowheg = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_CT14.root");
   TFile *fPowhegrewt = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_CT14nlo.root");
   TFile *fPowhegrewt2 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_CT14nlo_rewisospin.root");
   TFile *fPowhegpp = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_CT14_rewt_CT14nlo_CT14nlo_rewisospin.root");
   TFile *fMCFM0 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/NLO_eigenvectors/Z_only_nlo_CT14nlo_1.00_1.00_DY15600_NLO_muM34_CT14_0_0.DAT.root");
   TFile *fMCFMpp = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/MCFM/MCFM-8.0/Bin/DYpA8TeV/NLO_eigenvectors/Z_only_nlo_CT14nlo_1.00_1.00_DY15600_NLO_muM34_CT14pp_0_0.D.root");
   // TFile *fMG5 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_MG5_rewt_CT14nlo_CT14nlo_rewisospin.root");
   TFile *fMG5 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_MG5_rewt_CT14nlo_CT14nlo.root");
   TFile *fMG5_isospin = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_MG5_rewt_CT14nlo_CT14nlo_rewisospin.root");
   TFile *fPowheg_NNPDF = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_NNPDF31_nnlo_as_0118_mc_hessian_pdfas_rewisospin.root");
   TFile *fPowheg_TUJUnlo = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_TUJU19_nlo_208_82.root");
   TFile *fPowheg_TUJUnnlo = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_TUJU19_nnlo_208_82.root");
   TFile *fPowheg_TUJUnlo_pp = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_TUJU19_nlo_1_1_rewisospin.root");
   TFile *fPowheg_nNNPDF1 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nNNPDF10_nlo_as_0118_Pb208.root");
   TFile *fPowheg_nNNPDF2 = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nNNPDF20_nlo_as_0118_Pb208.root");
   TFile *fPowheg_nCTEQ = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_nCTEQ15FullNuc_208_82.root");
   TFile *fPowheg_KSASG20nlo = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_KSASG20-NLO-Pb.root");
   TFile *fPowheg_KSASG20nnlo = TFile::Open("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/ROOTFile_Histogram_Acc_weights_genonly_EPPS16_rewt_CT14nlo_KSASG20-NNLO-Pb.root");

   gSystem->mkdir("./compareMCFM_CT14");
   gSystem->mkdir("./compareMCFM_CT14/MG5");
   gSystem->mkdir("./compareMCFM_CT14/rewCT14");
   gSystem->mkdir("./compareMCFM_CT14/rewNNPDF");
   gSystem->mkdir("./compareMCFM_CT14/rewTUJUnlo");
   gSystem->mkdir("./compareMCFM_CT14/rewTUJUnnlo");
   gSystem->mkdir("./compareMCFM_CT14/rewTUJUnlopp");
   gSystem->mkdir("./compareMCFM_CT14/rewnNNPDF1");
   gSystem->mkdir("./compareMCFM_CT14/rewnNNPDF2");
   gSystem->mkdir("./compareMCFM_CT14/rewnCTEQ");
   gSystem->mkdir("./compareMCFM_CT14/rewKSASG20nlo");
   gSystem->mkdir("./compareMCFM_CT14/rewKSASG20nnlo");

   const int nhists = 12+2;
   const char* histnamesPowheg[nhists] = {
      "MCFM/h_m34_1_0",
      "MCFM/h_m34_2_0",
      "MCFM/h_y34lM_0",
      "MCFM/h_y34lM_0",
      "MCFM/h_pt34lM_0",
      "MCFM/h_phist34lM_1_0",
      "MCFM/h_phist34lM_2_0",
      "MCFM/h_y34hM_0",
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
      "id24fb",
      "id25",
      "id26",
      "id27",
      "id28",
      "id28fb",
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
      "|y| (15<M<60 GeV)",
      "p_{T} (15<M<60 GeV)",
      "#phi^{*} (15<M<60 GeV)",
      "#phi^{*} (15<M<60 GeV)",
      "y (60<M<120 GeV)",
      "|y| (60<M<120 GeV)",
      "p_{T} (60<M<120 GeV)",
      "#phi^{*} (60<M<120 GeV)",
      "#phi^{*} (60<M<120 GeV)",
      "mass [GeV]",
      "mass [GeV]"
   };

   // do the comparisons
   for (int i=0; i<nhists; i++) {
      TString histnameMCFM = TString(histnamesMCFM[i]).ReplaceAll("fb","");
      TH1D *hPowhegEPPS = (TH1D*) fPowhegEPPS->Get(histnamesPowheg[i]);
      TH1D *hPowheg = (TH1D*) fPowheg->Get(histnamesPowheg[i]);
      TH1D *hPowhegrewt = (TH1D*) fPowhegrewt->Get(histnamesPowheg[i]);
      TH1D *hPowhegrewt2 = (TH1D*) fPowhegrewt2->Get(histnamesPowheg[i]);
      TH1D *hPowhegpp = (TH1D*) fPowhegpp->Get(histnamesPowheg[i]);
      TH1D *hMCFM0 = (TH1D*) fMCFM0->Get(histnameMCFM);
      TH1D *hMCFMpp = (TH1D*) fMCFMpp->Get(histnameMCFM);
      TH1D *hMG5 = (TH1D*) fMG5->Get(histnamesPowheg[i]);
      TH1D *hMG5_isospin = (TH1D*) fMG5_isospin->Get(histnamesPowheg[i]);
      TH1D *hPowheg_NNPDF = (TH1D*) fPowheg_NNPDF->Get(histnamesPowheg[i]);
      TH1D *hPowheg_TUJUnlo = (TH1D*) fPowheg_TUJUnlo->Get(histnamesPowheg[i]);
      TH1D *hPowheg_TUJUnnlo = (TH1D*) fPowheg_TUJUnnlo->Get(histnamesPowheg[i]);
      TH1D *hPowheg_TUJUnlo_pp = (TH1D*) fPowheg_TUJUnlo_pp->Get(histnamesPowheg[i]);
      TH1D *hPowheg_nNNPDF1 = (TH1D*) fPowheg_nNNPDF1->Get(histnamesPowheg[i]);
      TH1D *hPowheg_nNNPDF2 = (TH1D*) fPowheg_nNNPDF2->Get(histnamesPowheg[i]);
      TH1D *hPowheg_nCTEQ = (TH1D*) fPowheg_nCTEQ->Get(histnamesPowheg[i]);
      TH1D *hPowheg_KSASG20nlo = (TH1D*) fPowheg_KSASG20nlo->Get(histnamesPowheg[i]);
      TH1D *hPowheg_KSASG20nnlo = (TH1D*) fPowheg_KSASG20nnlo->Get(histnamesPowheg[i]);

      // factor 208 in between MCFM and Powheg
      hMCFM0->Scale(hMCFM0->GetBinWidth(1)*208./1000.);
      hMCFMpp->Scale(hMCFMpp->GetBinWidth(1)*208./1000.);
      // POWHEG is a number of events: divide by lumi
      hPowhegEPPS->Scale(1./lumi_all);
      hPowheg->Scale(1./lumi_all);
      hPowhegrewt->Scale(1./lumi_all);
      hPowhegrewt2->Scale(1./lumi_all);
      hPowhegpp->Scale(1./lumi_all);
      hMG5->Scale(1.30/lumi_all); //ad hoc scale factor
      hMG5_isospin->Scale(1.30/lumi_all); //ad hoc scale factor
      hPowheg_NNPDF->Scale(1./lumi_all);
      hPowheg_TUJUnlo->Scale(1./lumi_all);
      hPowheg_TUJUnnlo->Scale(1./lumi_all);
      hPowheg_TUJUnlo_pp->Scale(1./lumi_all);
      hPowheg_nNNPDF1->Scale(1./lumi_all);
      hPowheg_nNNPDF2->Scale(1./lumi_all);
      hPowheg_nCTEQ->Scale(1./lumi_all);
      hPowheg_KSASG20nlo->Scale(1./lumi_all);
      hPowheg_KSASG20nnlo->Scale(1./lumi_all);

      if (TString(histnamesMCFM[i]).Contains("fb")) {
         hPowhegEPPS = compRFB(hPowhegEPPS);
         hPowheg = compRFB(hPowheg);
         hPowhegrewt = compRFB(hPowhegrewt);
         hPowhegpp = compRFB(hPowhegpp);
         hMCFM0 = compRFB(hMCFM0);
         hMCFMpp = compRFB(hMCFMpp);
         hMG5 = compRFB(hMG5);
         hMG5_isospin = compRFB(hMG5_isospin);
         hPowheg_NNPDF = compRFB(hPowheg_NNPDF);
         hPowheg_TUJUnlo = compRFB(hPowheg_TUJUnlo);
         hPowheg_TUJUnnlo = compRFB(hPowheg_TUJUnnlo);
         hPowheg_TUJUnlo_pp = compRFB(hPowheg_TUJUnlo_pp);
         hPowheg_nNNPDF1 = compRFB(hPowheg_nNNPDF1);
         hPowheg_nNNPDF2 = compRFB(hPowheg_nNNPDF2);
         hPowheg_nCTEQ = compRFB(hPowheg_nCTEQ);
         hPowheg_KSASG20nlo = compRFB(hPowheg_KSASG20nlo);
         hPowheg_KSASG20nnlo = compRFB(hPowheg_KSASG20nnlo);
      }

      TString yaxistitle = (TString(histnamesMCFM[i]).Contains("fb")) ? "R_{FB}" : "Xsec [pb] / bin";
      MyCanvas c(TString("compareMCFM_CT14/")+histnamesMCFM[i],xtitles[i],yaxistitle,800,600);
      c.SetRatioRange(0.79,1.21);
      c.CanvasWithThreeHistogramsRatioPlot(hMCFM0,hMCFMpp,hPowheg,
            "MCFM (CT14 w/ isospin)", "MCFM (CT14 w/o isospin)", "Powheg (CT14 original)", "MCFM/Powheg");
      c.PrintCanvas();
      c.PrintCanvas_PNG();
      MyCanvas c2(TString("compareMCFM_CT14/MG5/")+TString(histnamesMCFM[i])+"_MG5",xtitles[i],yaxistitle,800,600);
      c2.SetRatioRange(0.79,1.21);
      c2.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hMG5_isospin,
            "Powheg (CT14)", "Powheg (EPPS)", "MG5 (rew CT14 w/ isospin)", "Powheg/MG5");
      c2.PrintCanvas();
      c2.PrintCanvas_PNG();
      MyCanvas c3(TString("compareMCFM_CT14/rewCT14/")+TString(histnamesMCFM[i])+"_rewCT14",xtitles[i],yaxistitle,800,600);
      c3.SetRatioRange(0.79,1.21);
      c3.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowhegrewt2,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew CT14 w/ isospin)", "Powheg/Powheg (rewt)");
      c3.PrintCanvas();
      c3.PrintCanvas_PNG();
      MyCanvas c4(TString("compareMCFM_CT14/rewNNPDF/")+TString(histnamesMCFM[i])+"_rewNNPDF",xtitles[i],yaxistitle,800,600);
      c4.SetRatioRange(0.79,1.21);
      c4.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_NNPDF,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew NNPDF3.0 w/ isospin)", "Powheg/Powheg (rewt)");
      c4.PrintCanvas();
      c4.PrintCanvas_PNG();
      MyCanvas c5(TString("compareMCFM_CT14/rewTUJUnlo/")+TString(histnamesMCFM[i])+"_rewTUJUnlo",xtitles[i],yaxistitle,800,600);
      c5.SetRatioRange(0.79,1.21);
      c5.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_TUJUnlo,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew TUJUnlo)", "Powheg/Powheg (rewt)");
      c5.PrintCanvas();
      c5.PrintCanvas_PNG();
      MyCanvas c6(TString("compareMCFM_CT14/rewTUJUnnlo/")+TString(histnamesMCFM[i])+"_rewTUJUnnlo",xtitles[i],yaxistitle,800,600);
      c6.SetRatioRange(0.79,1.21);
      c6.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_TUJUnnlo,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew TUJUnnlo)", "Powheg/Powheg (rewt)");
      c6.PrintCanvas();
      c6.PrintCanvas_PNG();
      MyCanvas c7(TString("compareMCFM_CT14/rewTUJUnlopp/")+TString(histnamesMCFM[i])+"_rewTUJUnlopp",xtitles[i],yaxistitle,800,600);
      c7.SetRatioRange(0.79,1.21);
      c7.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_TUJUnlo_pp,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew TUJUnlo pp w/ isospin)", "Powheg/Powheg (rewt)");
      c7.PrintCanvas();
      c7.PrintCanvas_PNG();
      MyCanvas c8(TString("compareMCFM_CT14/rewnNNPDF1/")+TString(histnamesMCFM[i])+"_rewnNNPDF1",xtitles[i],yaxistitle,800,600);
      c8.SetRatioRange(0.79,1.21);
      c8.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_nNNPDF1,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew nNNPDF 1.0)", "Powheg/Powheg (rewt)");
      c8.PrintCanvas();
      c8.PrintCanvas_PNG();
      MyCanvas c9(TString("compareMCFM_CT14/rewnNNPDF2/")+TString(histnamesMCFM[i])+"_rewnNNPDF2",xtitles[i],yaxistitle,800,600);
      c9.SetRatioRange(0.79,1.21);
      c9.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_nNNPDF2,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew nNNPDF 2.0)", "Powheg/Powheg (rewt)");
      c9.PrintCanvas();
      c9.PrintCanvas_PNG();
      MyCanvas c10(TString("compareMCFM_CT14/rewnCTEQ/")+TString(histnamesMCFM[i])+"_rewnCTEQ",xtitles[i],yaxistitle,800,600);
      c10.SetRatioRange(0.79,1.21);
      c10.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_nCTEQ,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew nCTEQ15)", "Powheg/Powheg (rewt)");
      c10.PrintCanvas();
      c10.PrintCanvas_PNG();
      MyCanvas c11(TString("compareMCFM_CT14/rewKSASG20nlo/")+TString(histnamesMCFM[i])+"_rewKSASG20nlo",xtitles[i],yaxistitle,800,600);
      c11.SetRatioRange(0.79,1.21);
      c11.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_KSASG20nlo,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew KSASG20nlo)", "Powheg/Powheg (rewt)");
      c11.PrintCanvas();
      c11.PrintCanvas_PNG();
      MyCanvas c12(TString("compareMCFM_CT14/rewKSASG20nnlo/")+TString(histnamesMCFM[i])+"_rewKSASG20nnlo",xtitles[i],yaxistitle,800,600);
      c12.SetRatioRange(0.79,1.21);
      c12.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowheg_KSASG20nnlo,
            "Powheg (CT14)", "Powheg (EPPS)", "Powheg (rew KSASG20nnlo)", "Powheg/Powheg (rewt)");
      c12.PrintCanvas();
      c12.PrintCanvas_PNG();


      // OLD STUFF
      // MyCanvas c2(TString(histnamesMCFM[i])+"_Powhegrew",xtitles[i],yaxistitle,800,600);
      // c2.SetRatioRange(0.79,1.21);
      // c2.CanvasWithThreeHistogramsRatioPlot(hMCFM0,hMCFMpp,hPowhegrewt,
      //       "MCFM (CT14 w/ isospin)", "MCFM (CT14 w/o isospin)", "Powheg (EPPS16->CT14)", "MCFM/Powheg");
      // c2.PrintCanvas();
      // MyCanvas c3(TString(histnamesMCFM[i])+"_MG5",xtitles[i],yaxistitle,800,600);
      // c3.SetRatioRange(0.79,1.21);
      // c3.CanvasWithThreeHistogramsRatioPlot(hMG5,hMG5_isospin,hMCFMpp,
      //       "MG5 (NNPDF->CT14)", "MG5 (NNPDF->CT14 + isospin)", "MCFM (CT14 w/o isospin)", "MG5/MCFM");
      // c3.PrintCanvas();

      // MyCanvas c4(TString(histnamesMCFM[i])+"_Powhegrew2",xtitles[i],"Xsec [pb] / bin",800,600);
      // c4.SetRatioRange(0.79,1.21);
      // // c4.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegpp,hMG5,
      // //       "Powheg (original)", "Powheg (isospin added)", "MG5 (isospin added)", "Powheg/MG5");
      // // c4.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegpp,hPowhegrewt,
      //       // "Powheg (original)", "Powheg (isospin added)", "Powheg (rewt EPPS #rightarrow  CT14)", "Powheg/Powhegrewt");
      // c4.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegrewt,hPowhegrewt2,
      //       "Powheg (original)", "Powheg (EPPS->CT14)", "Powheg (EPPS->CT14 + isospin)", "Powheg/Powhegrewt");
      // c4.PrintCanvas();

      // MyCanvas c5(TString(histnamesMCFM[i])+"_PDFcomp",xtitles[i],yaxistitle,800,600);
      // c5.SetRatioRange(0.79,1.21);
      // c5.CanvasWithThreeHistogramsRatioPlot(hPowheg,hPowhegEPPS,hPowhegrewt2,
      //       "Powheg (CT14)", "Powheg (EPPS)", "Powheg (EPPS->CT14 + isospin)", "Powheg/Powhegrewt");
      // c5.PrintCanvas();
   }
}

TH1D *compRFB(TH1D* hist) {
   int nbins = hist->GetNbinsX();
   TH1D *ans = new TH1D(TString(hist->GetName()) + "_fb", TString(hist->GetTitle()) + " (F/B ratio)", nbins/2, hist->GetBinLowEdge(nbins/2+1), hist->GetXaxis()->GetXmax());

   for (int i=1; i<=nbins/2; i++) {
      int im = nbins/2 - i + 1;
      int ip = nbins/2 + i;
      double ym = hist->GetBinContent(im);
      double yp = hist->GetBinContent(ip);
      double eym = hist->GetBinError(im);
      double eyp = hist->GetBinError(ip);
      double rfb = (ym>0) ? yp/ym : 1.;
      double erfb = (ym>0) ? rfb * sqrt(pow(eym/ym,2)+pow(eyp/yp,2)) : 0;
      ans->SetBinContent(i,rfb);
      ans->SetBinError(i,erfb);
   }

   ans->GetXaxis()->SetRangeUser(0,3.);
   ans->GetYaxis()->SetRangeUser(0.65,1.45);

   return ans;
}
