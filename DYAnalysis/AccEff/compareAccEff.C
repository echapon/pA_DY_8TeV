#include "TFile.h"
#include "TCanvas.h"
#include "TEfficiency.h"

void setStyle(TEfficiency *e, Color_t color=kBlack, Style_t markerstyle=kFullCircle);

void compareAccEff() {
   TFile *fnom = TFile::Open("ROOTFile_Histogram_Acc_Eff_Powheg_HLT_PAL3Mu12_v*_0_rewboth.root");
   TFile *fnorew = TFile::Open("ROOTFile_Histogram_Acc_Eff_Powheg_HLT_PAL3Mu12_v*_0_norew.root");
   TFile *frewminus = TFile::Open("ROOTFile_Histogram_Acc_Eff_Powheg_HLT_PAL3Mu12_v*_0_rewminus.root");
   TFile *frewplus = TFile::Open("ROOTFile_Histogram_Acc_Eff_Powheg_HLT_PAL3Mu12_v*_0_rewplus.root");
   TFile *fpPb = TFile::Open("ROOTFile_Histogram_Acc_Eff_Powheg_HLT_PAL3Mu12_v*_1_rewboth.root");
   TFile *fPbp = TFile::Open("ROOTFile_Histogram_Acc_Eff_Powheg_HLT_PAL3Mu12_v*_2_rewboth.root");
   TFile *fPyquen = TFile::Open("ROOTFile_Histogram_Acc_Eff_Pyquen_HLT_PAL3Mu12_v*_0_rewboth.root");

   TEfficiency *acc_nom = (TEfficiency*) fnom->Get("TEff_Acc_Mass");
   TEfficiency *eff_nom = (TEfficiency*) fnom->Get("TEff_Eff_Mass");
   TEfficiency *acc_norew = (TEfficiency*) fnorew->Get("TEff_Acc_Mass");
   TEfficiency *eff_norew = (TEfficiency*) fnorew->Get("TEff_Eff_Mass");
   TEfficiency *acc_rewminus = (TEfficiency*) frewminus->Get("TEff_Acc_Mass");
   TEfficiency *eff_rewminus = (TEfficiency*) frewminus->Get("TEff_Eff_Mass");
   TEfficiency *acc_rewplus = (TEfficiency*) frewplus->Get("TEff_Acc_Mass");
   TEfficiency *eff_rewplus = (TEfficiency*) frewplus->Get("TEff_Eff_Mass");
   TEfficiency *acc_pPb = (TEfficiency*) fpPb->Get("TEff_Acc_Mass");
   TEfficiency *eff_pPb = (TEfficiency*) fpPb->Get("TEff_Eff_Mass");
   TEfficiency *acc_Pbp = (TEfficiency*) fPbp->Get("TEff_Acc_Mass");
   TEfficiency *eff_Pbp = (TEfficiency*) fPbp->Get("TEff_Eff_Mass");
   TEfficiency *acc_Pyquen = (TEfficiency*) fPyquen->Get("TEff_Acc_Mass");
   TEfficiency *eff_Pyquen = (TEfficiency*) fPyquen->Get("TEff_Eff_Mass");
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
   TH1F *haxes_acc = new TH1F("haxes_acc",";Gen-Level Dimuon Mass (post-FSR) [GeV];Acceptance",1,15,600);
   haxes_acc->GetYaxis()->SetRangeUser(1e-3,1.1);
   TH1F *haxes_eff = new TH1F("haxes_eff",";Gen-Level Dimuon Mass (post-FSR) [GeV];Efficiency",1,15,600);
   haxes_eff->GetYaxis()->SetRangeUser(0.5,1.05);

   // set styles
   setStyle(acc_nom);
   setStyle(eff_nom);
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
   
   // reweighting effect
   haxes_acc->Draw();
   acc_nom->Draw("same");
   gPad->SetLogx();
   gPad->SetLogy();
   // acc_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(1e-3,1.1);
   acc_norew->Draw("same");
   acc_rewplus->Draw("same");
   acc_rewminus->Draw("same");
   tleg = new TLegend(0.6,0.2,0.9,0.5);
   tleg->SetBorderSize(0);
   tleg->AddEntry(acc_nom,"HF both","lp");
   tleg->AddEntry(acc_norew,"No HF rew","lp");
   tleg->AddEntry(acc_rewplus,"HF+ rew","lp");
   tleg->AddEntry(acc_rewminus,"HF+ rew","lp");
   tleg->Draw();
   c1->SaveAs("comp_acc_rew.pdf");

   haxes_eff->Draw();
   eff_nom->Draw("same");
   gPad->SetLogy(kFALSE);
   // eff_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(0.5,1.1);
   eff_norew->Draw("same");
   eff_rewplus->Draw("same");
   eff_rewminus->Draw("same");
   tleg = new TLegend(0.6,0.2,0.9,0.5);
   tleg->SetBorderSize(0);
   tleg->AddEntry(eff_nom,"HF both","lp");
   tleg->AddEntry(eff_norew,"No HF rew","lp");
   tleg->AddEntry(eff_rewplus,"HF+ rew","lp");
   tleg->AddEntry(eff_rewminus,"HF+ rew","lp");
   tleg->Draw();
   c1->SaveAs("comp_eff_rew.pdf");
   
   // pPb vs Pbp
   haxes_acc->Draw();
   acc_nom->Draw("same");
   gPad->SetLogy();
   // acc_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(1e-3,1.1);
   acc_pPb->Draw("same");
   acc_Pbp->Draw("same");
   tleg = new TLegend(0.6,0.2,0.9,0.5);
   tleg->SetBorderSize(0);
   tleg->AddEntry(acc_nom,"pPb+Pbp","lp");
   tleg->AddEntry(acc_pPb,"pPb","lp");
   tleg->AddEntry(acc_Pbp,"Pbp","lp");
   tleg->Draw();
   c1->SaveAs("comp_acc_beamdir.pdf");

   haxes_eff->Draw();
   eff_nom->Draw("same");
   gPad->SetLogy(kFALSE);
   // eff_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(0.5,1.1);
   eff_pPb->Draw("same");
   eff_Pbp->Draw("same");
   tleg = new TLegend(0.6,0.2,0.9,0.5);
   tleg->SetBorderSize(0);
   tleg->AddEntry(eff_nom,"pPb+Pbp","lp");
   tleg->AddEntry(eff_pPb,"pPb","lp");
   tleg->AddEntry(eff_Pbp,"Pbp","lp");
   tleg->Draw();
   c1->SaveAs("comp_eff_beamdir.pdf");
   
   // Powheg vs Pyquen
   haxes_acc->Draw();
   acc_nom->Draw("same");
   gPad->SetLogy();
   // acc_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(1e-3,1.1);
   acc_Pyquen->Draw("same");
   tleg = new TLegend(0.6,0.2,0.9,0.5);
   tleg->SetBorderSize(0);
   tleg->AddEntry(acc_nom,"Powheg","lp");
   tleg->AddEntry(acc_Pyquen,"Pyquen","lp");
   tleg->Draw();
   c1->SaveAs("comp_acc_generator.pdf");

   haxes_eff->Draw();
   eff_nom->Draw("same");
   gPad->SetLogy(kFALSE);
   // eff_nom->GetPaintedGraph()->GetHistogram()->GetYaxis()->SetRangeUser(0.5,1.1);
   eff_Pyquen->Draw("same");
   tleg = new TLegend(0.6,0.2,0.9,0.5);
   tleg->SetBorderSize(0);
   tleg->AddEntry(eff_nom,"Powheg","lp");
   tleg->AddEntry(eff_Pyquen,"Pyquen","lp");
   tleg->Draw();
   c1->SaveAs("comp_eff_generator.pdf");
}

void setStyle(TEfficiency *e, Color_t color=kBlack, Style_t markerstyle=kFullCircle) {
   e->SetMarkerColor(color);
   e->SetLineColor(color);
   e->SetMarkerStyle(markerstyle);
}
