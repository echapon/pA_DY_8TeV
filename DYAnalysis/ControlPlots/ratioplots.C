void ratioplots(const char* filename) {
   TFile *f = TFile::Open(filename);
   TCanvas *c1 = new TCanvas();

   const char* histname[97] = {
   	"h_Pt_DYMuMu30",	
   	"h_eta_DYMuMu30",	
   	"h_phi_DYMuMu30",	
   	"h_mass_DYMuMu30",	
   	"h_diPt_DYMuMu30",	
   	"h_diRap_DYMuMu30",	
   	"h_absdiRap_M20to30_DYMuMu30",	
   	"h_absdiRap_M30to45_DYMuMu30",	
   	"h_absdiRap_M45to60_DYMuMu30",	
   	"h_absdiRap_M60to120_DYMuMu30",	
   	"h_absdiRap_M120to200_DYMuMu30",	
   	"h_absdiRap_M200to1500_DYMuMu30",	
   	"h_lead_Pt_DYMuMu30",	
   	"h_sub_Pt_DYMuMu30",	
   	"h_lead_eta_DYMuMu30",	
   	"h_sub_eta_DYMuMu30",	
   	"h_lead_phi_DYMuMu30",	
   	"h_sub_phi_DYMuMu30",	
   	"h_barrel_Pt_DYMuMu30",	
   	"h_barrel_eta_DYMuMu30",	
   	"h_barrel_phi_DYMuMu30",	
   	"h_endcap_Pt_DYMuMu30",	
   	"h_endcap_eta_DYMuMu30",	
   	"h_endcap_phi_DYMuMu30",	
   	"h_mass_OS_DYMuMu30",	
   	"h_mass_SS_DYMuMu30",	
   	"h_Pt_minusCharge_DYMuMu30",	
   	"h_eta_minusCharge_DYMuMu30",	
   	"h_phi_minusCharge_DYMuMu30",	
   	"h_Pt_plusCharge_DYMuMu30",	
   	"h_eta_plusCharge_DYMuMu30",	
   	"h_phi_plusCharge_DYMuMu30",	
   	"h_Pt_M70_DYMuMu30",	
   	"h_Pt_M70to90_DYMuMu30",	
   	"h_Pt_M90to110_DYMuMu30",	
   	"h_Pt_M110toInf_DYMuMu30",	
   	"h_Pt_M70to111_DYMuMu30",	
   	"h_Pt_M15to60_DYMuMu30",	
   	"h_Pt_M60to120_DYMuMu30",	
   	"h_Pt_M120to3000_DYMuMu30",	
   	"h_Pt_M15to30_DYMuMu30",	
   	"h_Pt_M30to45_DYMuMu30",	
   	"h_Pt_M45to60_DYMuMu30",	
   	"h_Pt_M120to200_DYMuMu30",	
   	"h_Pt_M200to3000_DYMuMu30",	
   	"h_Angle_DYMuMu30",	
   	"h_Pt_TrigLeg_DYMuMu30",	
   	"h_eta_TrigLeg_DYMuMu30",	
   	"h_phi_TrigLeg_DYMuMu30",	
   	"h_Pt_OtherLeg_DYMuMu30",	
   	"h_eta_OtherLeg_DYMuMu30",	
   	"h_phi_OtherLeg_DYMuMu30",	
   	"h_GenMass_DYMuMu30",	
   	"h_GenPt_DYMuMu30",	
   	"h_GenEta_DYMuMu30",	
   	"h_GenPhi_DYMuMu30",	
   	"h_GenPt_Lead_DYMuMu30",	
   	"h_GenEta_Lead_DYMuMu30",	
   	"h_GenPhi_Lead_DYMuMu30",	
   	"h_GenPt_Sub_DYMuMu30",	
   	"h_GenEta_Sub_DYMuMu30",	
   	"h_GenPhi_Sub_DYMuMu30",	
   	"h_GenDiPt_DYMuMu30",	
   	"h_GenDiRap_DYMuMu30",	
   	"h_VtxProb_DYMuMu30",	
   	"h_VtxProb_belowM600_DYMuMu30",	
   	"h_VtxProb_overM600_DYMuMu30",	
   	"h_VtxNormChi2_DYMuMu30",	
   	"h_VtxNormChi2_belowM600_DYMuMu30",	
   	"h_VtxNormChi2_overM600_DYMuMu30",	
   	"h_mass_OS_BB_DYMuMu30",	
   	"h_mass_OS_BE_DYMuMu30",	
   	"h_mass_OS_EE_DYMuMu30",	
   	"h_mass_OS_part1_DYMuMu30",	
   	"h_mass_OS_part1_BB_DYMuMu30",	
   	"h_mass_OS_part1_BE_DYMuMu30",	
   	"h_mass_OS_part1_EE_DYMuMu30",	
   	"h_mass_OS_part2_DYMuMu30",	
   	"h_mass_OS_part2_BB_DYMuMu30",	
   	"h_mass_OS_part2_BE_DYMuMu30",	
   	"h_mass_OS_part2_EE_DYMuMu30",	
   	"h_muonHits_DYMuMu30",	
   	"h_nMatches_DYMuMu30",	
   	"h_RelPtError_DYMuMu30",	
   	"h_dxyVTX_DYMuMu30",	
   	"h_dzVTX_DYMuMu30",	
   	"h_pixelHits_DYMuMu30",	
   	"h_trackerLayers_DYMuMu30",	
   	"h_RelTrkIso_DYMuMu30",	
   	"h_RelPFIso_DYMuMu30",	
   	"h_PU_DYMuMu30",	
   	"h_nVertices_before_DYMuMu30",	
   	"h_nVertices_after_DYMuMu30",	
   	"h_hiHF_DYMuMu30",	
   	"h_hiHFplus_DYMuMu30",	
   	"h_hiHFminus_DYMuMu30",	
   	"h_hiNtracks_DYMuMu30"};

   for (int i=0; i<97; i++) {
      TH1D *hist1, *hist2;
      hist1 = (TH1D*) f->Get(histname[i]);
      hist2 = (TH1D*) f->Get(Form("%s_PbP",histname[i]));
      TRatioPlot *a = new TRatioPlot(hist1,hist2);  
      a->Draw(); 
      a->GetLowerRefYaxis()->SetRangeUser(0.55,1.45); 
      gPad->Update();
      TPaveText t(0,0.93,0.4,1,"NDC"); t.SetFillColor(0); t.SetBorderSize(0); t.AddText(histname[i]); t.Draw();
      TLegend *tleg = new TLegend(0.67,0.8,0.87,0.9);
      tleg->SetBorderSize(0);
      tleg->AddEntry(hist1,"1st part (pPb)","l");
      tleg->AddEntry(hist2,"2nd part (PbP)","p");
      tleg->Draw();
      c1->SaveAs(Form("%s.pdf",histname[i]));
      delete a;
      delete tleg;
   }
}
