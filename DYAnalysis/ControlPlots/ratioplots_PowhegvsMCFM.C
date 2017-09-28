void ratioplots_PowhegvsMCFM() {
   TFile *fPowheg = TFile::Open("ROOTFile_Histogram_InvMass_PAL3Mu12_MC_MomUnCorr_MCFMbinning.root");
   TFile *fMCFM1030 = TFile::Open("Z_only_nlo_CT14nlo_1.00_1.00_DY1030_nlo_EPPS16nlo_CT14nlo_P.root");
   TFile *fMCFM30 = TFile::Open("Z_only_nlo_CT14nlo_1.00_1.00_DY308000_nlo_EPPS16nlo_CT14nlo.root");
   TCanvas *c1 = new TCanvas();

   const char* histnamePowheg[4] = {"h_GenMass","h_GenDiPt","h_GenDiRap2", "h_GenMass2"};
   const char* histnameMCFM[4] = {"id1","id8","id5", "id19"};

   for (int i=0; i<4; i++) {
      TH1D *hist1, *hist2;
      TRatioPlot *a=NULL;
      TLegend *tleg = new TLegend(0.67,0.8,0.87,0.9);

      // 30-infty
      hist1 = (TH1D*) fMCFM30->Get(histnameMCFM[i]);
      hist1->Scale(1./hist1->Integral());
      hist2 = (TH1D*) fPowheg->Get(Form("%s_DYMuMu30",histnamePowheg[i]));
      hist2->Scale(1./hist2->Integral());
      a = new TRatioPlot(hist1,hist2);  
      a->Draw(); 
      a->GetLowerRefYaxis()->SetRangeUser(0.55,1.45); 
      if (TString(histnamePowheg[i]).Contains("Mass")) a->GetUpperPad()->SetLogy();
      gPad->Update();
      TPaveText t2(0,0.93,0.4,1,"NDC"); t2.SetFillColor(0); t2.SetBorderSize(0); t2.AddText(histnamePowheg[i]); t2.Draw();
         tleg->SetBorderSize(0);
         tleg->AddEntry(hist1,"MCFM","l");
         tleg->AddEntry(hist2,"Powheg","p");
      tleg->Draw();
      c1->SaveAs(Form("%s_DYMuMu30.pdf",histnamePowheg[i]));
      delete a;

      if (i<3) {
         // 10-30
         hist1 = (TH1D*) fMCFM1030->Get(histnameMCFM[i]);
         hist1->Scale(1./hist1->Integral());
         hist2 = (TH1D*) fPowheg->Get(Form("%s_DYMuMu1030",histnamePowheg[i]));
         hist2->Scale(1./hist2->Integral());
         a = new TRatioPlot(hist1,hist2);  
         a->Draw(); 
         a->GetLowerRefYaxis()->SetRangeUser(0.55,1.45); 
         if (TString(histnamePowheg[i]).Contains("Mass")) a->GetUpperPad()->SetLogy();
         gPad->Update();
         TPaveText t(0,0.93,0.4,1,"NDC"); t.SetFillColor(0); t.SetBorderSize(0); t.AddText(histnamePowheg[i]); t.Draw();
         tleg->Draw();
         c1->SaveAs(Form("%s_DYMuMu1030.pdf",histnamePowheg[i]));
         delete a;
      }

      delete tleg;
   }
}
