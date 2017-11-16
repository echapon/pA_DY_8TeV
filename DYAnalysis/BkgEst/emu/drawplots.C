#include "../../Include/MyCanvas.C"

void drawplots(const char* var="mass") {
   TFile *f = TFile::Open(Form("result/emu_%s.root",var));
   TString xaxis;
   TString tvar(var);
   if (tvar=="mass") xaxis = "M_{e#mu} [GeV/c^{2}]";
   else if (tvar=="pt") xaxis = "p_{T,e#mu} [GeV/c]";
   else if (tvar=="phistar") xaxis = "#phi^{*}_{e#mu}";
   else xaxis = "y_{e#mu}";

   MyCanvas c1("result/plot_" + tvar + "_ttbar",xaxis,"Entries");
   c1.CanvasWithHistogramsRatioPlot((TH1D*) gDirectory->Get("ttbar"),(TH1D*) gDirectory->Get("ttbar_MC"),"t#bar{t} (data-driven)","t#bar{t} (MC)","ratio",kBlack,kRed,false,true,"EP","hist same");
   c1.PrintCanvas();
   
   MyCanvas c2("result/plot_" + tvar + "_WW",xaxis,"Entries");
   c2.CanvasWithHistogramsRatioPlot((TH1D*) gDirectory->Get("WW"),(TH1D*) gDirectory->Get("WW_MC"),"WW (data-driven)","WW (MC)","ratio",kBlack,kGreen,false,true,"EP","hist same");
   c2.PrintCanvas();

   MyCanvas c3("result/plot_" + tvar + "_DYtautau",xaxis,"Entries");
   c3.CanvasWithHistogramsRatioPlot((TH1D*) gDirectory->Get("DYtautau"),(TH1D*) gDirectory->Get("DYtautau_MC"),"DY (data-driven)","DY (MC)","ratio",kBlack,kBlue,false,true,"EP","hist same");
   c3.PrintCanvas();
}
