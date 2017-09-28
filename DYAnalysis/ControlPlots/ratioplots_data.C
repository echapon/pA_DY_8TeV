void ratioplots_data(const char* filename="ROOTFile_Histogram_InvMass_PAL3Mu12_Data_MomUnCorr.root") {
   TFile *f = TFile::Open(filename);
   TCanvas *c1 = new TCanvas();

   const char* histname[85] = {
   	"h_Pt",	
   	"h_eta",	
   	"h_phi",	
   	"h_mass",	
   	"h_diPt",	
   	"h_diRap",	
   	"h_absdiRap_M20to30",	
   	"h_absdiRap_M30to45",	
   	"h_absdiRap_M45to60",	
   	"h_absdiRap_M60to120",	
   	"h_absdiRap_M120to200",	
   	"h_absdiRap_M200to1500",	
   	"h_lead_Pt",	
   	"h_sub_Pt",	
   	"h_lead_eta",	
   	"h_sub_eta",	
   	"h_lead_phi",	
   	"h_sub_phi",	
   	"h_barrel_Pt",	
   	"h_barrel_eta",	
   	"h_barrel_phi",	
   	"h_endcap_Pt",	
   	"h_endcap_eta",	
   	"h_endcap_phi",	
   	"h_mass_OS",	
   	"h_mass_SS",	
   	"h_Pt_minusCharge",	
   	"h_eta_minusCharge",	
   	"h_phi_minusCharge",	
   	"h_Pt_plusCharge",	
   	"h_eta_plusCharge",	
   	"h_phi_plusCharge",	
   	"h_Pt_M70",	
   	"h_Pt_M70to90",	
   	"h_Pt_M90to110",	
   	"h_Pt_M110toInf",	
   	"h_Pt_M70to111",	
   	"h_Pt_M15to60",	
   	"h_Pt_M60to120",	
   	"h_Pt_M120to3000",	
   	"h_Pt_M15to30",	
   	"h_Pt_M30to45",	
   	"h_Pt_M45to60",	
   	"h_Pt_M120to200",	
   	"h_Pt_M200to3000",	
   	"h_Angle",	
   	"h_Pt_TrigLeg",	
   	"h_eta_TrigLeg",	
   	"h_phi_TrigLeg",	
   	"h_Pt_OtherLeg",	
   	"h_eta_OtherLeg",	
   	"h_phi_OtherLeg",	
   	"h_VtxProb",	
   	"h_VtxProb_belowM600",	
   	"h_VtxProb_overM600",	
   	"h_VtxNormChi2",	
   	"h_VtxNormChi2_belowM600",	
   	"h_VtxNormChi2_overM600",	
   	"h_mass_OS_BB",	
   	"h_mass_OS_BE",	
   	"h_mass_OS_EE",	
   	"h_mass_OS_part1",	
   	"h_mass_OS_part1_BB",	
   	"h_mass_OS_part1_BE",	
   	"h_mass_OS_part1_EE",	
   	"h_mass_OS_part2",	
   	"h_mass_OS_part2_BB",	
   	"h_mass_OS_part2_BE",	
   	"h_mass_OS_part2_EE",	
   	"h_muonHits",	
   	"h_nMatches",	
   	"h_RelPtError",	
   	"h_dxyVTX",	
   	"h_dzVTX",	
   	"h_pixelHits",	
   	"h_trackerLayers",	
   	"h_RelTrkIso",	
   	"h_RelPFIso",	
   	"h_PU",	
   	"h_nVertices_before",	
   	"h_nVertices_after",	
   	"h_hiHF",	
   	"h_hiHFplus",	
   	"h_hiHFminus",	
   	"h_hiNtracks"};

   for (int i=0; i<85; i++) {
      TH1D *hist1, *hist2;
      hist1 = (TH1D*) f->Get(Form("%s_Data1",histname[i]));
      hist1->Scale(1./hist1->Integral());
      hist2 = (TH1D*) f->Get(Form("%s_Data2",histname[i]));
      hist2->Scale(1./hist2->Integral());
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
