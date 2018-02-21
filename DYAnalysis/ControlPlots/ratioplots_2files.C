#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TRatioPlot.h"

void ratioplots_2files(const char* file1, const char* file2, const char* legend1, const char* legend2, const char* tags1, const char* tags2, double maxdiff=0.45) {
   TFile *f1 = TFile::Open(file1);
   if (!f1 || !f1->IsOpen()) return;
   TFile *f2 = TFile::Open(file2);
   if (!f2 || !f2->IsOpen()) return;
   TCanvas *c1 = new TCanvas();
   c1->Print("ratioplots.pdf[");

   const int nhist = 92;
   const char* histname[nhist] = {
      "h_Angle",
      "h_barrel_eta",
      "h_barrel_phi",
      "h_barrel_Pt",
      "h_diPt",
      "h_diPt2_M60to120",
      "h_diRap2_M15to60",
      "h_diRap2_M60to120",
      "h_diRap_M120to600",
      "h_diRap_M15to60",
      "h_diRap_M60to120",
      "h_dxyVTX",
      "h_dzVTX",
      "h_endcap_eta",
      "h_endcap_phi",
      "h_endcap_Pt",
      "h_eta",
      "h_eta_OtherLeg",
      "h_eta_TrigLeg",
      "h_hiEB",
      "h_hiEE",
      "h_hiEEminus",
      "h_hiEEplus",
      "h_hiET",
      "h_hiHF",
      "h_hiHFhit",
      "h_hiHFhitminus",
      "h_hiHFhitplus",
      "h_hiHFminus",
      "h_hiHFminusEta4",
      "h_hiHFplus",
      "h_hiHFplusEta4",
      "h_hiNpix",
      "h_hiNtracks",
      "h_hiNtracksPtCut",
      "h_lead_eta",
      "h_lead_phi",
      "h_lead_Pt",
      "h_mass",
      "h_massZ",
      "h_mass2",
      "h_mass3",
      "h_mass_OS",
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
      "h_mass_SS",
      "h_muonHits",
      "h_nMatches",
      "h_nVertices_before",
      "h_nVertices_before",
      "h_pfMET_phi",
      "h_pfMET_pT",
      "h_pfMET_px",
      "h_pfMET_py",
      "h_pfMET_SumEt",
      "h_pfMET_Type1_phi",
      "h_pfMET_Type1_pT",
      "h_pfMET_Type1_px",
      "h_pfMET_Type1_py",
      "h_pfMET_Type1_SumEt",
      "h_phi",
      "h_phi_OtherLeg",
      "h_Phistar2_M60to120",
      "h_Phistar_M60to120",
      "h_phi_TrigLeg",
      "h_pixelHits",
      "h_Pt",
      "h_Pt_M120to600",
      "h_Pt_M15to60",
      "h_Pt_M15to600",
      "h_Pt_minusCharge",
      "h_Pt_OtherLeg",
      "h_Pt_plusCharge",
      "h_Pt_TrigLeg",
      "h_RelPFIso",
      "h_RelPtError",
      "h_RelTrkIso",
      "h_sub_eta",
      "h_sub_phi",
      "h_sub_Pt",
      "h_trackerLayers",
      "h_VtxNormChi2",
      "h_VtxProb"
   };

   for (int i=0; i<nhist; i++) {
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
      a->GetLowerRefYaxis()->SetRangeUser(1.-maxdiff,1.+maxdiff); 
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
      c1->Print("ratioplots.pdf","pdf");
      delete a;
      delete tleg;
   }
   c1->Print("ratioplots.pdf]");
}
