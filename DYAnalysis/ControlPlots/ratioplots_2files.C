#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TRatioPlot.h"
#include "../Include/MyCanvas.C"
#include "DrawControlPlotTool.h"

void ratioplots_2files(const char* file1, const char* file2, TString legend1, TString legend2, const char* tags1, const char* tags2, double maxdiff=0.45) {
   TFile *f1 = TFile::Open(file1);
   if (!f1 || !f1->IsOpen()) return;
   TFile *f2 = TFile::Open(file2);
   if (!f2 || !f2->IsOpen()) return;
   TCanvas *c1 = new TCanvas();
   c1->Print("ratioplots.pdf[");

   DrawControlPlotTool tool("None",0,"Lumi","MomCorr00",true,false);
   tool.SetupHistogramNames(); 

   int nhist = tool.HistNames.size();

   for (int i=0; i<nhist; i++) {
      TH1D *hist1=NULL, *hist2=NULL;
      TRatioPlot *a=NULL;

      // get the histos
      TString myl(tags1);
      TString tok;
      Ssiz_t from = 0;
      while (myl.Tokenize(tok, from, ",")) {
         TH1D *histtmp = (TH1D*) f1->Get(tool.HistNames[i] + "_" + tok);
         if (!histtmp) break;
         if (!hist1) hist1 = histtmp;
         else hist1->Add(histtmp);
      }
      if (!hist1) continue;
      if (hist1->Integral()==0) continue;

      myl = TString(tags2);
      from = 0;
      while (myl.Tokenize(tok, from, ",")) {
         TH1D *histtmp = (TH1D*) f2->Get(tool.HistNames[i] + "_" + tok);
         if (!histtmp) break;
         if (!hist2) hist2 = histtmp;
         else hist2->Add(histtmp);
      }
      if (!hist2) continue;
      if (hist2->Integral()==0) continue;

      hist1->Scale(1./hist1->Integral());
      hist2->Scale(1./hist2->Integral());

      cout << hist1->GetXaxis()->GetTitle() << endl;
      cout << hist1->GetYaxis()->GetTitle() << endl;

      MyCanvas myc("compare_" + tool.Variables[i],tool.XTitles[i],"Entries per bin (norm. to 1)");
      myc.SetRatioRange(0.88,1.12);
      myc.SetLegendPosition(0.57,0.7,0.87,0.9);
      myc.CanvasWithHistogramsRatioPlot(hist1,hist2,legend1,legend2,"ratio");
      myc.c->cd();
      
      TPaveText t3(0.3,0.96,0.7,1,"NDC"); 
      t3.SetFillColor(0); 
      t3.SetBorderSize(0); 
      t3.SetTextSize(0.04);
      TString opt = "WW";
      if (TString(tags1).Contains("Data")) opt="UU NORM";
      // else if (TString(tags2).Contains("Data")) opt="WW";
      t3.AddText(Form("#chi^{2}/ndf=%.2f (%.1f%s)",
               hist2->Chi2Test(hist1,opt + " CHI2/NDF"),
               100.*hist2->Chi2Test(hist1,opt),
               "%"
               )); 
      t3.Draw();
      
      TPaveText t4(0.75,0.96,0.95,1,"NDC"); 
      t4.SetFillColor(0); 
      t4.SetBorderSize(0); 
      t4.SetTextSize(0.04);
      t4.AddText(Form("KS %.1f%s", 100.*hist2->KolmogorovTest(hist1),"%")); 
      t4.Draw();

      myc.PrintCanvas();
   }
}
