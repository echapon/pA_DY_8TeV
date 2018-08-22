#include "../../Include/MyCanvas.C"
#include "../../Include/tdrstyle.C"
#include "../interface/defs.h"

using namespace DYana;

void plotABCD(TString trigger="PAL3Mu12", TString tag="DataMinusMC") {
   setTDRStyle();

   TFile *f = TFile::Open("output_ABCD_" + trigger + "_" + tag + ".root");

   TString varnames[5] = {"Mass","Rap1560","Rap60120","Pt","Phistar"};

   TString legend = "Data - #sum MC";
   if (tag=="Wjets") legend = "W+jet MC";
   if (tag=="QCD") legend = "QCD MC";

   for (int i=0; i<5; i++) {
      // compare the different methods
      TH1D *hdata = (TH1D*) f->Get("inputs/h_" + varnames[i] + "_2iso_OS_Data");
      TH1D *hdataMC = (TH1D*) f->Get("inputs/h_" + varnames[i] + "_2iso_OS_" + tag);
      TH1D *h0 = (TH1D*) f->Get("outputs_0isoTo2iso/hbkg_OS_2iso_" + varnames[i]); h0->SetLineColor(kRed); h0->SetMarkerColor(kRed);
      TH1D *h1 = (TH1D*) f->Get("outputs_1isoTo2iso/hbkg_OS_2iso_" + varnames[i]); h1->SetLineColor(kBlue); h1->SetMarkerColor(kBlue);
      TH1D *h2 = (TH1D*) f->Get("outputs_0and1isoTo2iso/hbkg_OS_2iso_" + varnames[i]); h2->SetLineColor(kMagenta); h2->SetMarkerColor(kMagenta);

      vector<TH1D*> histos;
      vector<TString> hnames;
      if (tag=="DataMinusMC") {histos.push_back(hdata); hnames.push_back("Data");}
      histos.push_back(hdataMC); hnames.push_back(legend);
      histos.push_back(h0); hnames.push_back("ABCD (0iso to 2iso)");
      histos.push_back(h1); hnames.push_back("ABCD (1iso to 2iso)");
      histos.push_back(h2); hnames.push_back("ABCD ((0+1)iso to 2iso)");

      MyCanvas ccomp1("c_comp1_" + varnames[i] + "_" + trigger + "_" + tag,xaxistitle(varnames[i]),"Events",800,800);
      if (varnames[i] == "Mass" || varnames[i] == "Pt" || varnames[i] == "Phistar") {
         ccomp1.SetLogx();
         ccomp1.SetLogy();
         ccomp1.SetYRange(0.1,2.*histos[0]->GetMaximum());
      }

      ccomp1.CanvasWithMultipleHistograms(histos,hnames);
      ccomp1.PrintCanvas();

      MyCanvas ccomp2("c_comp2_" + varnames[i] + "_" + trigger + "_" + tag,xaxistitle(varnames[i]),"Events",800,800);
      if (varnames[i] == "Mass" || varnames[i] == "Pt" || varnames[i] == "Phistar") {
         ccomp2.SetLogx();
         ccomp2.SetLogy();
         ccomp2.SetYRange(0.1,2.*h0->GetMaximum());
      }
      ccomp2.CanvasWithThreeHistogramsRatioPlot(h0,h1,h2,
            "ABCD (0iso to 2iso)","ABCD (1iso to 2iso)","ABCD ((0+1)iso to 2iso)","var / nom",
            kBlue,kRed,kBlack);
      ccomp2.PrintCanvas();


      // closure test on 2 to 1iso
      MyCanvas cclosure("c_closure_" + varnames[i] + "_" + trigger + "_" + tag,xaxistitle(varnames[i]),"Events",800,800);
      TH1D *hdata1 = (TH1D*) f->Get("inputs/h_" + varnames[i] + "_1iso_OS_" + tag);
      TH1D *h01 = (TH1D*) f->Get("outputs_0isoTo1iso/hbkg_OS_1iso_" + varnames[i]); 
      if (varnames[i] == "Mass" || varnames[i] == "Pt" || varnames[i] == "Phistar") {
         cclosure.SetLogx();
      }
      cclosure.CanvasWithHistogramsRatioPlot(h01,hdata1,
            "ABCD (0iso to 1iso)",legend + " (1iso)","ABCD/Data",
            kRed,kBlack);
      cclosure.PrintCanvas();
   }
}
