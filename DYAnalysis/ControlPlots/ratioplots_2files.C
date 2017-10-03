void ratioplots_2files(const char* file1, const char* file2, const char* legend1, const char* legend2, const char* tags1, const char* tags2) {
   TFile *f1 = TFile::Open(file1);
   if (!f1 || !f1->IsOpen()) return;
   TFile *f2 = TFile::Open(file2);
   if (!f2 || !f2->IsOpen()) return;
   TCanvas *c1 = new TCanvas();

   const char* histname[99] = {
   	"h_Pt",	
   	"h_eta",	
   	"h_phi",	
   	"h_mass",	
   	"h_mass2",	
   	"h_mass3",	
   	"h_diPt",	
   	"h_diRap",	
      "h_diRap_M15to60",
      "h_diRap_M60to120",
      "h_diRap_M120to600",
      "h_diRap2_M15to60",
      "h_diRap2_M60to120",
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
      "h_Pt_M15to60",
      "h_Pt_M60to120",
      "h_Pt_M120to600",
   	"h_Angle",	
   	"h_Pt_TrigLeg",	
   	"h_eta_TrigLeg",	
   	"h_phi_TrigLeg",	
   	"h_Pt_OtherLeg",	
   	"h_eta_OtherLeg",	
   	"h_phi_OtherLeg",	
   	"h_GenMass",	
   	"h_GenMass_preFSR",	
   	"h_GenPt",	
   	"h_GenEta",	
   	"h_GenPhi",	
   	"h_GenPt_Lead",	
   	"h_GenEta_Lead",	
   	"h_GenPhi_Lead",	
   	"h_GenPt_Sub",	
   	"h_GenEta_Sub",	
   	"h_GenPhi_Sub",	
   	"h_GenDiPt",	
   	"h_GenDiRap",	
   	"h_VtxProb",	
   	"h_VtxProb_belowM600",	
   	"h_VtxNormChi2",	
   	"h_VtxNormChi2_belowM600",	
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
   	"h_hiHFplusEta4",	
   	"h_hiHFminusEta4",	
   	"h_hiHFhit",	
   	"h_hiHFhitPlus",	
   	"h_hiHFhitMinus",	
   	"h_hiET",	
   	"h_hiEE",	
   	"h_hiEB",	
   	"h_hiEEplus",	
   	"h_hiEEminus",	
   	"h_hiNpix",
   	"h_hiNtracks",
   	"h_hiNtracksPtCut"
   };

   for (int i=0; i<99; i++) {
      TH1D *hist1=NULL, *hist2=NULL;
      TRatioPlot *a=NULL;

      // get the histos
      TString myl(tags1);
      TString tok;
      Ssiz_t from = 0;
      while (myl.Tokenize(tok, from, ",")) {
         TH1D *histtmp = (TH1D*) f1->Get(Form("%s_%s",histname[i],tok.Data()));
         if (!histtmp) break;
         if (!hist1) hist1 = histtmp;
         else hist1->Add(histtmp);
      }
      if (!hist1) continue;
      if (hist1->Integral()==0) continue;

      myl = TString(tags2);
      from = 0;
      while (myl.Tokenize(tok, from, ",")) {
         TH1D *histtmp = (TH1D*) f2->Get(Form("%s_%s",histname[i],tok.Data()));
         if (!histtmp) break;
         if (!hist2) hist2 = histtmp;
         else hist2->Add(histtmp);
      }
      if (!hist2) continue;
      if (hist2->Integral()==0) continue;

      hist1->Scale(1./hist1->Integral());
      hist2->Scale(1./hist2->Integral());
      a = new TRatioPlot(hist1,hist2);  
      a->Draw(); 
      a->GetLowerRefYaxis()->SetRangeUser(0.55,1.45); 
      if (TString(histname[i]).Contains("Mass")) a->GetUpperPad()->SetLogy();
      gPad->Update();

      TPaveText t2(0,0.93,0.4,1,"NDC"); 
      t2.SetFillColor(0); 
      t2.SetBorderSize(0); 
      t2.SetTextSize(0.05);
      t2.AddText(histname[i]); 
      t2.Draw();
      
      TPaveText t3(0.5,0.93,0.7,1,"NDC"); 
      t3.SetFillColor(0); 
      t3.SetBorderSize(0); 
      t3.SetTextSize(0.05);
      TString opt = "WW";
      if (TString(tags1).Contains("Data")) opt="UU NORM";
      // else if (TString(tags2).Contains("Data")) opt="WW";
      t3.AddText(Form("#chi^{2}/ndf=%.2f (%.1f%s)",
               hist2->Chi2Test(hist1,opt + " CHI2/NDF"),
               100.*hist2->Chi2Test(hist1,opt),
               "%"
               )); 
      t3.Draw();
      
      TPaveText t4(0.75,0.93,0.95,1,"NDC"); 
      t4.SetFillColor(0); 
      t4.SetBorderSize(0); 
      t4.SetTextSize(0.05);
      t4.AddText(Form("KS %.1f%s", 100.*hist2->KolmogorovTest(hist1),"%")); 
      t4.Draw();
      
      TLegend *tleg = new TLegend(0.67,0.8,0.87,0.9);
      tleg->SetBorderSize(0);
      tleg->AddEntry(hist1,legend1,"l");
      tleg->AddEntry(hist2,legend2,"p");
      tleg->Draw();
      c1->SaveAs(Form("%s.pdf",histname[i]));
      delete a;
      delete tleg;
   }
}
