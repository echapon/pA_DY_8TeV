#include "TFile.h"
#include "TCanvas.h"
#include "TEfficiency.h"

void setStyle(TEfficiency *e, Color_t color=kBlack, Style_t markerstyle=kFullCircle);

void compareAccEff(TString variable="Mass") { // Mass | Pt | Rap1560 | Rap60120 | Phistar
   TFile *fnom = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_rewboth.root");
   TFile *fmomcor = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr_Powheg_PAL3Mu12_0_rewboth.root");
   TFile *fnorew = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_norew.root");
   TFile *frewminus = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_rewminus.root");
   TFile *frewplus = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_rewplus.root");
   TFile *fpPb = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_1_rewboth.root");
   TFile *fPbp = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_2_rewboth.root");
   TFile *fPyquen = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Pyquen_PAL3Mu12_0_rewboth.root");

   TEfficiency *acc_nom = (TEfficiency*) fnom->Get("TEff_Acc_" + variable);
   TEfficiency *eff_nom = (TEfficiency*) fnom->Get("TEff_Eff_" + variable);
   TEfficiency *acc_momcor = (TEfficiency*) fmomcor->Get("TEff_Acc_" + variable);
   TEfficiency *eff_momcor = (TEfficiency*) fmomcor->Get("TEff_Eff_" + variable);
   TEfficiency *acc_norew = (TEfficiency*) fnorew->Get("TEff_Acc_" + variable);
   TEfficiency *eff_norew = (TEfficiency*) fnorew->Get("TEff_Eff_" + variable);
   TEfficiency *acc_rewminus = (TEfficiency*) frewminus->Get("TEff_Acc_" + variable);
   TEfficiency *eff_rewminus = (TEfficiency*) frewminus->Get("TEff_Eff_" + variable);
   TEfficiency *acc_rewplus = (TEfficiency*) frewplus->Get("TEff_Acc_" + variable);
   TEfficiency *eff_rewplus = (TEfficiency*) frewplus->Get("TEff_Eff_" + variable);
   TEfficiency *acc_pPb = (TEfficiency*) fpPb->Get("TEff_Acc_" + variable);
   TEfficiency *eff_pPb = (TEfficiency*) fpPb->Get("TEff_Eff_" + variable);
   TEfficiency *acc_Pbp = (TEfficiency*) fPbp->Get("TEff_Acc_" + variable);
   TEfficiency *eff_Pbp = (TEfficiency*) fPbp->Get("TEff_Eff_" + variable);
   TEfficiency *acc_Pyquen = (TEfficiency*) fPyquen->Get("TEff_Acc_" + variable);
   TEfficiency *eff_Pyquen = (TEfficiency*) fPyquen->Get("TEff_Eff_" + variable);
   // we only have Pyquen for M>30: set lower masses to 0
   TH1 *htmp = (TH1*) acc_Pyquen->GetPassedHistogram()->Clone();
   for (int i=0; i<=htmp->GetNbinsX(); i++) 
      if (htmp->GetBinCenter(i)<30)
         htmp->SetBinContent(i,0);
   acc_Pyquen->SetPassedHistogram(*htmp,"");
   htmp = (TH1*) eff_Pyquen->GetPassedHistogram()->Clone();
   for (int i=0; i<=htmp->GetNbinsX(); i++) 
      if (htmp->GetBinCenter(i)<30)
         htmp->SetBinContent(i,0);
   eff_Pyquen->SetPassedHistogram(*htmp,"");

   // axes
   TString xtitle; double xmin, xmax;
   if (variable=="Mass") {
      xtitle="Gen-Level Dimuon Mass (post-FSR) [GeV]";
      xmin=15.;
      xmax=600.;
   } else if (variable=="Pt") {
      xtitle="Gen-Level Dimuon p_{T} (post-FSR) [GeV]";
      xmin=0.3;
      xmax=200.;
   } else if (variable=="Rap1560") {
      xtitle="Gen-Level Dimuon y_{CM} (post-FSR)";
      xmin=-3.;
      xmax=2.;
   } else if (variable=="Rap60120") {
      xtitle="Gen-Level Dimuon y_{CM} (post-FSR)";
      xmin=-3.;
      xmax=2.;
   } else if (variable=="Phistar") {
      xtitle="Gen-Level Dimuon #phi^{*} (post-FSR)";
      xmin=0.002;
      xmax=3;
   }
   TH1F *haxes_acc = new TH1F("haxes_acc",";"+xtitle+";Acceptance",1,xmin,xmax);
   haxes_acc->GetYaxis()->SetRangeUser(1e-3,1.1);
   TH1F *haxes_eff = new TH1F("haxes_eff",";"+xtitle+";Efficiency",1,xmin,xmax);
   haxes_eff->GetYaxis()->SetRangeUser(0.5,1.05);

   // set styles
   setStyle(acc_nom);
   setStyle(eff_nom);
   setStyle(acc_momcor,kRed,kFullSquare);
   setStyle(eff_momcor,kRed,kFullSquare);
   setStyle(acc_norew,kRed,kFullSquare);
   setStyle(eff_norew,kRed,kFullSquare);
   setStyle(acc_rewplus,kBlue,kFullTriangleUp);
   setStyle(eff_rewplus,kBlue,kFullTriangleUp);
   setStyle(acc_rewminus,kGreen,kFullTriangleDown);
   setStyle(eff_rewminus,kGreen,kFullTriangleDown);
   setStyle(acc_pPb,kRed,kFullTriangleUp);
   setStyle(eff_pPb,kRed,kFullTriangleUp);
   setStyle(acc_Pbp,kBlue,kFullTriangleDown);
   setStyle(eff_Pbp,kBlue,kFullTriangleDown);
   setStyle(acc_Pyquen,kRed,kFullSquare);
   setStyle(eff_Pyquen,kRed,kFullSquare);

   TCanvas *c1 = new TCanvas("c1","c1",600,600);
   TLegend *tleg = NULL;
   double xl1=0.6,yl1=0.2,xl2=0.9,yl2=0.5;
   if (variable.Contains("Rap")) {xl1=0.35;xl2=0.65;}
   if (variable == "Rap1560") {yl1=0.6;yl2=0.9;}

   // Rochester corrections
   haxes_eff->Draw();
   eff_nom->Draw("same");
   if (!variable.Contains("Rap")) gPad->SetLogx();
   gPad->SetLogy(kFALSE);
   // eff_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(0.5,1.1);
   eff_momcor->Draw("same");
   tleg = new TLegend(xl1,yl1,xl2,yl2);
   tleg->SetBorderSize(0);
   tleg->AddEntry(eff_nom,"No mom. corr.","lp");
   tleg->AddEntry(eff_momcor,"Incl. mom. corr.","lp");
   tleg->Draw();
   c1->SaveAs("comp_eff_momcor_"+variable+".pdf");
   
   // reweighting effect
   haxes_acc->Draw();
   acc_nom->Draw("same");
   if (!variable.Contains("Rap")) gPad->SetLogx();
   if (variable=="Mass") gPad->SetLogy();
   // acc_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(1e-3,1.1);
   acc_norew->Draw("same");
   // acc_rewplus->Draw("same");
   // acc_rewminus->Draw("same");
   tleg = new TLegend(xl1,yl1,xl2,yl2);
   tleg->SetBorderSize(0);
   tleg->AddEntry(acc_nom,"HF both","lp");
   tleg->AddEntry(acc_norew,"No HF rew","lp");
   // tleg->AddEntry(acc_rewplus,"HF+ rew","lp");
   // tleg->AddEntry(acc_rewminus,"HF- rew","lp");
   tleg->Draw();
   c1->SaveAs("comp_acc_rew_"+variable+".pdf");

   haxes_eff->Draw();
   eff_nom->Draw("same");
   gPad->SetLogy(kFALSE);
   // eff_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(0.5,1.1);
   eff_norew->Draw("same");
   // eff_rewplus->Draw("same");
   // eff_rewminus->Draw("same");
   tleg = new TLegend(xl1,yl1,xl2,yl2);
   tleg->SetBorderSize(0);
   tleg->AddEntry(eff_nom,"HF both","lp");
   tleg->AddEntry(eff_norew,"No HF rew","lp");
   // tleg->AddEntry(eff_rewplus,"HF+ rew","lp");
   // tleg->AddEntry(eff_rewminus,"HF- rew","lp");
   tleg->Draw();
   c1->SaveAs("comp_eff_rew_"+variable+".pdf");
   
   // pPb vs Pbp
   haxes_acc->Draw();
   acc_nom->Draw("same");
   if (variable=="Mass") gPad->SetLogy();
   // acc_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(1e-3,1.1);
   acc_pPb->Draw("same");
   acc_Pbp->Draw("same");
   tleg = new TLegend(xl1,yl1,xl2,yl2);
   tleg->SetBorderSize(0);
   tleg->AddEntry(acc_nom,"pPb+Pbp","lp");
   tleg->AddEntry(acc_pPb,"pPb","lp");
   tleg->AddEntry(acc_Pbp,"Pbp","lp");
   tleg->Draw();
   c1->SaveAs("comp_acc_beamdir_"+variable+".pdf");

   haxes_eff->Draw();
   eff_nom->Draw("same");
   gPad->SetLogy(kFALSE);
   // eff_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(0.5,1.1);
   eff_pPb->Draw("same");
   eff_Pbp->Draw("same");
   tleg = new TLegend(xl1,yl1,xl2,yl2);
   tleg->SetBorderSize(0);
   tleg->AddEntry(eff_nom,"pPb+Pbp","lp");
   tleg->AddEntry(eff_pPb,"pPb","lp");
   tleg->AddEntry(eff_Pbp,"Pbp","lp");
   tleg->Draw();
   c1->SaveAs("comp_eff_beamdir_"+variable+".pdf");
   
   // Powheg vs Pyquen
   haxes_acc->Draw();
   acc_nom->Draw("same");
   if (variable=="Mass") gPad->SetLogy();
   // acc_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(1e-3,1.1);
   acc_Pyquen->Draw("same");
   tleg = new TLegend(xl1,yl1,xl2,yl2);
   tleg->SetBorderSize(0);
   tleg->AddEntry(acc_nom,"Powheg","lp");
   tleg->AddEntry(acc_Pyquen,"Pyquen","lp");
   tleg->Draw();
   c1->SaveAs("comp_acc_generator_"+variable+".pdf");

   haxes_eff->Draw();
   eff_nom->Draw("same");
   gPad->SetLogy(kFALSE);
   // eff_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(0.5,1.1);
   eff_Pyquen->Draw("same");
   tleg = new TLegend(xl1,yl1,xl2,yl2);
   tleg->SetBorderSize(0);
   tleg->AddEntry(eff_nom,"Powheg","lp");
   tleg->AddEntry(eff_Pyquen,"Pyquen","lp");
   tleg->Draw();
   c1->SaveAs("comp_eff_generator_"+variable+".pdf");
}

void setStyle(TEfficiency *e, Color_t color=kBlack, Style_t markerstyle=kFullCircle) {
   e->SetMarkerColor(color);
   e->SetLineColor(color);
   e->SetMarkerStyle(markerstyle);
}
