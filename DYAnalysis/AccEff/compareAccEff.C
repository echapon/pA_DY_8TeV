#include "TFile.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "../Include/MyCanvas.C"

void setStyle(TEfficiency *e, Color_t color=kBlack, Style_t markerstyle=kFullCircle);

void compareAccEff(TString variable="Mass") { // Mass | Pt | Rap1560 | Rap60120 | Phistar
   TFile *fnom = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root");
   TFile *fmomcor = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomUnCorr_Powheg_PAL3Mu12_0_rewboth.root");
   TFile *fnorew = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_norew.root");
   TFile *frewminus = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewminus.root");
   TFile *frewplus = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewplus.root");
   TFile *fpPb = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_1_rewboth.root");
   TFile *fPbp = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_2_rewboth.root");
   TFile *fPyquen = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Pyquen_PAL3Mu12_0_rewboth.root");

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
   // // we only have Pyquen for M>30: set lower masses to 0
   // TH1 *htmp = (TH1*) acc_Pyquen->GetPassedHistogram()->Clone();
   // for (int i=0; i<=htmp->GetNbinsX(); i++) 
   //    if (htmp->GetBinCenter(i)<30)
   //       htmp->SetBinContent(i,0);
   // acc_Pyquen->SetPassedHistogram(*htmp,"");
   // htmp = (TH1*) eff_Pyquen->GetPassedHistogram()->Clone();
   // for (int i=0; i<=htmp->GetNbinsX(); i++) 
   //    if (htmp->GetBinCenter(i)<30)
   //       htmp->SetBinContent(i,0);
   // eff_Pyquen->SetPassedHistogram(*htmp,"");

   // axes
   TString xtitle; double xmin, xmax;
   if (variable=="Mass") {
      xtitle="Gen-Level Dimuon Mass (post-FSR) [GeV]";
      xmin=15.;
      xmax=600.;
   } else if (variable.Contains("Pt")) {
      xtitle="Gen-Level Dimuon p_{T} (post-FSR) [GeV]";
      xmin=0.3;
      xmax=600.;
   } else if (variable.Contains("Rap")) {
      xtitle="Gen-Level Dimuon y_{CM} (post-FSR)";
      xmin=-2.87;
      xmax=1.93;
   } else if (variable.Contains("Phistar")) {
      xtitle="Gen-Level Dimuon #phi^{*} (post-FSR)";
      xmin=0.002;
      xmax=3.277;
   }

   double xl1=0.6,yl1=0.35,xl2=0.9,yl2=0.55;
   if (variable.Contains("Rap")) {xl1=0.35;xl2=0.7;}
   if (variable == "Rap1560") {yl1=0.6;yl2=0.95;}
   
   // Rochester corrections
   MyCanvas cEffRoch("comp_eff_momcor_"+variable,xtitle,"Efficiency");
   if (!variable.Contains("Rap")) cEffRoch.SetLogx();
   cEffRoch.SetLegendPosition(xl1,yl1,xl2,yl2);
   cEffRoch.SetXRange(xmin,xmax);
   cEffRoch.SetYRange(0.5,1.05);
   cEffRoch.SetRatioRange(0.95,1.05);
   cEffRoch.CanvasWithGraphRatioPlot(eff_momcor->CreateGraph(),eff_nom->CreateGraph(),"No mom. corr.","Mom. corr. B-H","No mom. corr. / corr. B-H");
   cEffRoch.PrintCanvas();

   
   // reweighting effect
   MyCanvas cAccrew("comp_acc_rew_"+variable,xtitle,"Acceptance");
   if (!variable.Contains("Rap")) cAccrew.SetLogx();
   cAccrew.SetLegendPosition(xl1,yl1,xl2,yl2);
   cAccrew.SetXRange(xmin,xmax);
   cAccrew.SetYRange(1e-3,1.1);
   cAccrew.SetRatioRange(0.95,1.05);
   cAccrew.CanvasWithGraphRatioPlot(acc_norew->CreateGraph(),acc_nom->CreateGraph(),"No HF rew","HF both","No HF rew / HF both");
   cAccrew.PrintCanvas();

   MyCanvas cEffrew("comp_eff_rew_"+variable,xtitle,"Efficiency");
   if (!variable.Contains("Rap")) cEffrew.SetLogx();
   cEffrew.SetLegendPosition(xl1,yl1,xl2,yl2);
   cEffrew.SetXRange(xmin,xmax);
   cEffrew.SetYRange(0.5,1.05);
   cEffrew.SetRatioRange(0.95,1.05);
   cEffrew.CanvasWithGraphRatioPlot(eff_norew->CreateGraph(),eff_nom->CreateGraph(),"No HF rew","HF both","No HF rew / HF both");
   cEffrew.PrintCanvas();

   // pPb vs Pbp
   MyCanvas cAccbeam("comp_acc_beamdir_"+variable,xtitle,"Acceptance");
   if (!variable.Contains("Rap")) cAccbeam.SetLogx();
   cAccbeam.SetLegendPosition(xl1,yl1,xl2,yl2);
   cAccbeam.SetXRange(xmin,xmax);
   cAccbeam.SetYRange(1e-3,1.1);
   cAccbeam.SetRatioRange(0.95,1.05);
   cAccbeam.CanvasWithThreeGraphsRatioPlot(acc_pPb->CreateGraph(),acc_Pbp->CreateGraph(),acc_nom->CreateGraph(),"pPb","Pbp","pPb+Pbp","pPb or Pbp / pPb+Pbp");
   cAccbeam.PrintCanvas();

   MyCanvas cEffbeam("comp_eff_beamdir_"+variable,xtitle,"Efficiency");
   if (!variable.Contains("Rap")) cEffbeam.SetLogx();
   cEffbeam.SetLegendPosition(xl1,yl1,xl2,yl2);
   cEffbeam.SetXRange(xmin,xmax);
   cEffbeam.SetYRange(0.5,1.05);
   cEffbeam.SetRatioRange(0.95,1.05);
   cEffbeam.CanvasWithThreeGraphsRatioPlot(eff_pPb->CreateGraph(),eff_Pbp->CreateGraph(),eff_nom->CreateGraph(),"pPb","Pbp","pPb+Pbp","pPb or Pbp / pPb+Pbp");
   cEffbeam.PrintCanvas();
   
   // Powheg vs Pyquen
   MyCanvas cAccgenerator("comp_acc_generator_"+variable,xtitle,"Acceptance");
   if (!variable.Contains("Rap")) cAccgenerator.SetLogx();
   cAccgenerator.SetLegendPosition(xl1,yl1,xl2,yl2);
   cAccgenerator.SetXRange(xmin,xmax);
   cAccgenerator.SetYRange(1e-3,1.1);
   cAccgenerator.SetRatioRange(0.95,1.05);
   cAccgenerator.CanvasWithGraphRatioPlot(acc_Pyquen->CreateGraph(),acc_nom->CreateGraph(),"Pyquen","Powheg","Pyquen / Powheg");
   cAccgenerator.PrintCanvas();

   MyCanvas cEffgenerator("comp_eff_generator_"+variable,xtitle,"Efficiency");
   if (!variable.Contains("Rap")) cEffgenerator.SetLogx();
   cEffgenerator.SetLegendPosition(xl1,yl1,xl2,yl2);
   cEffgenerator.SetXRange(xmin,xmax);
   cEffgenerator.SetYRange(0.5,1.05);
   cEffgenerator.SetRatioRange(0.95,1.05);
   cEffgenerator.CanvasWithGraphRatioPlot(eff_Pyquen->CreateGraph(),eff_nom->CreateGraph(),"Pyquen","Powheg","Pyquen / Powheg");
   cEffgenerator.PrintCanvas();
}

void setStyle(TEfficiency *e, Color_t color, Style_t markerstyle) {
   e->SetMarkerColor(color);
   e->SetLineColor(color);
   e->SetMarkerStyle(markerstyle);
}
