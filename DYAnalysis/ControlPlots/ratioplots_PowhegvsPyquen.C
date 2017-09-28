void ratioplots_PowhegvsPyquen() {
   TFile *fPowheg = TFile::Open("ROOTFile_Histogram_InvMass_PAL3Mu12_MC_MomUnCorr.root");
   TFile *fPyquen = TFile::Open("ROOTFile_Histogram_InvMass_PAL3Mu12_Pyquen_MomUnCorr.root");
   TCanvas *c1 = new TCanvas();

   const char* histname[97] = {
   	"h_Pt_DYMuMu30_PbP",	
   	"h_eta_DYMuMu30_PbP",	
   	"h_phi_DYMuMu30_PbP",	
   	"h_mass_DYMuMu30_PbP",	
   	"h_diPt_DYMuMu30_PbP",	
   	"h_diRap_DYMuMu30_PbP",	
   	"h_absdiRap_M20to30_DYMuMu30_PbP",	
   	"h_absdiRap_M30to45_DYMuMu30_PbP",	
   	"h_absdiRap_M45to60_DYMuMu30_PbP",	
   	"h_absdiRap_M60to120_DYMuMu30_PbP",	
   	"h_absdiRap_M120to200_DYMuMu30_PbP",	
   	"h_absdiRap_M200to1500_DYMuMu30_PbP",	
   	"h_lead_Pt_DYMuMu30_PbP",	
   	"h_sub_Pt_DYMuMu30_PbP",	
   	"h_lead_eta_DYMuMu30_PbP",	
   	"h_sub_eta_DYMuMu30_PbP",	
   	"h_lead_phi_DYMuMu30_PbP",	
   	"h_sub_phi_DYMuMu30_PbP",	
   	"h_barrel_Pt_DYMuMu30_PbP",	
   	"h_barrel_eta_DYMuMu30_PbP",	
   	"h_barrel_phi_DYMuMu30_PbP",	
   	"h_endcap_Pt_DYMuMu30_PbP",	
   	"h_endcap_eta_DYMuMu30_PbP",	
   	"h_endcap_phi_DYMuMu30_PbP",	
   	"h_mass_OS_DYMuMu30_PbP",	
   	"h_mass_SS_DYMuMu30_PbP",	
   	"h_Pt_minusCharge_DYMuMu30_PbP",	
   	"h_eta_minusCharge_DYMuMu30_PbP",	
   	"h_phi_minusCharge_DYMuMu30_PbP",	
   	"h_Pt_plusCharge_DYMuMu30_PbP",	
   	"h_eta_plusCharge_DYMuMu30_PbP",	
   	"h_phi_plusCharge_DYMuMu30_PbP",	
   	"h_Pt_M70_DYMuMu30_PbP",	
   	"h_Pt_M70to90_DYMuMu30_PbP",	
   	"h_Pt_M90to110_DYMuMu30_PbP",	
   	"h_Pt_M110toInf_DYMuMu30_PbP",	
   	"h_Pt_M70to111_DYMuMu30_PbP",	
   	"h_Pt_M15to60_DYMuMu30_PbP",	
   	"h_Pt_M60to120_DYMuMu30_PbP",	
   	"h_Pt_M120to3000_DYMuMu30_PbP",	
   	"h_Pt_M15to30_DYMuMu30_PbP",	
   	"h_Pt_M30to45_DYMuMu30_PbP",	
   	"h_Pt_M45to60_DYMuMu30_PbP",	
   	"h_Pt_M120to200_DYMuMu30_PbP",	
   	"h_Pt_M200to3000_DYMuMu30_PbP",	
   	"h_Angle_DYMuMu30_PbP",	
   	"h_Pt_TrigLeg_DYMuMu30_PbP",	
   	"h_eta_TrigLeg_DYMuMu30_PbP",	
   	"h_phi_TrigLeg_DYMuMu30_PbP",	
   	"h_Pt_OtherLeg_DYMuMu30_PbP",	
   	"h_eta_OtherLeg_DYMuMu30_PbP",	
   	"h_phi_OtherLeg_DYMuMu30_PbP",	
   	"h_GenMass_DYMuMu30_PbP",	
   	"h_GenPt_DYMuMu30_PbP",	
   	"h_GenEta_DYMuMu30_PbP",	
   	"h_GenPhi_DYMuMu30_PbP",	
   	"h_GenPt_Lead_DYMuMu30_PbP",	
   	"h_GenEta_Lead_DYMuMu30_PbP",	
   	"h_GenPhi_Lead_DYMuMu30_PbP",	
   	"h_GenPt_Sub_DYMuMu30_PbP",	
   	"h_GenEta_Sub_DYMuMu30_PbP",	
   	"h_GenPhi_Sub_DYMuMu30_PbP",	
   	"h_GenDiPt_DYMuMu30_PbP",	
   	"h_GenDiRap_DYMuMu30_PbP",	
   	"h_VtxProb_DYMuMu30_PbP",	
   	"h_VtxProb_belowM600_DYMuMu30_PbP",	
   	"h_VtxProb_overM600_DYMuMu30_PbP",	
   	"h_VtxNormChi2_DYMuMu30_PbP",	
   	"h_VtxNormChi2_belowM600_DYMuMu30_PbP",	
   	"h_VtxNormChi2_overM600_DYMuMu30_PbP",	
   	"h_mass_OS_BB_DYMuMu30_PbP",	
   	"h_mass_OS_BE_DYMuMu30_PbP",	
   	"h_mass_OS_EE_DYMuMu30_PbP",	
   	"h_mass_OS_part1_DYMuMu30_PbP",	
   	"h_mass_OS_part1_BB_DYMuMu30_PbP",	
   	"h_mass_OS_part1_BE_DYMuMu30_PbP",	
   	"h_mass_OS_part1_EE_DYMuMu30_PbP",	
   	"h_mass_OS_part2_DYMuMu30_PbP",	
   	"h_mass_OS_part2_BB_DYMuMu30_PbP",	
   	"h_mass_OS_part2_BE_DYMuMu30_PbP",	
   	"h_mass_OS_part2_EE_DYMuMu30_PbP",	
   	"h_muonHits_DYMuMu30_PbP",	
   	"h_nMatches_DYMuMu30_PbP",	
   	"h_RelPtError_DYMuMu30_PbP",	
   	"h_dxyVTX_DYMuMu30_PbP",	
   	"h_dzVTX_DYMuMu30_PbP",	
   	"h_pixelHits_DYMuMu30_PbP",	
   	"h_trackerLayers_DYMuMu30_PbP",	
   	"h_RelTrkIso_DYMuMu30_PbP",	
   	"h_RelPFIso_DYMuMu30_PbP",	
   	"h_PU_DYMuMu30_PbP",	
   	"h_nVertices_before_DYMuMu30_PbP",	
   	"h_nVertices_after_DYMuMu30_PbP",	
   	"h_hiHF_DYMuMu30_PbP",	
   	"h_hiHFplus_DYMuMu30_PbP",	
   	"h_hiHFminus_DYMuMu30_PbP",	
   	"h_hiNtracks_DYMuMu30_PbP"};

   for (int i=0; i<97; i++) {
      TH1D *hist1, *hist2;
      TRatioPlot *a=NULL;
      TLegend *tleg = new TLegend(0.67,0.8,0.87,0.9);

      // 30-infty
      hist1 = (TH1D*) fPyquen->Get(histname[i]);
      hist1->Scale(1./hist1->Integral());
      hist2 = (TH1D*) fPowheg->Get(histname[i]);
      hist2->Scale(1./hist2->Integral());
      a = new TRatioPlot(hist1,hist2);  
      a->Draw(); 
      a->GetLowerRefYaxis()->SetRangeUser(0.55,1.45); 
      if (TString(histname[i]).Contains("Mass")) a->GetUpperPad()->SetLogy();
      gPad->Update();
      TPaveText t2(0,0.93,0.4,1,"NDC"); t2.SetFillColor(0); t2.SetBorderSize(0); t2.AddText(histname[i]); t2.Draw();
         tleg->SetBorderSize(0);
         tleg->AddEntry(hist1,"Pyquen","l");
         tleg->AddEntry(hist2,"Powheg","p");
      tleg->Draw();
      c1->SaveAs(Form("%s_DYMuMu30_PbP.pdf",histname[i]));
      delete a;
      delete tleg;
   }
}
