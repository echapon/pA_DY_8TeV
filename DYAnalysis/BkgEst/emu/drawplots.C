#include "../../Include/MyCanvas.C"

void drawplots(const char* var="mass") {
   TFile *f = TFile::Open(Form("result/emu_%s.root",var));
   TString xaxis;
   TString tvar(var);
   if (tvar=="mass") xaxis = "M_{e#mu} [GeV/c^{2}]";
   else if (tvar.Contains("pt")) xaxis = "p_{T,e#mu} [GeV/c]";
   else if (tvar.Contains("phistar")) xaxis = "#phi^{*}_{e#mu}";
   else xaxis = "y_{e#mu}";

   bool logx = (tvar=="mass" || tvar.Contains("pt") || tvar.Contains("phistar"));

   MyCanvas c1("result/plot_" + tvar + "_ttbar",xaxis,"Entries");
   if (logx) c1.SetLogx();
   c1.CanvasWithHistogramsRatioPlot((TH1D*) gDirectory->Get("ttbar"),(TH1D*) gDirectory->Get("ttbar_MC"),"t#bar{t} (data-driven)","t#bar{t} (MC)","ratio",kBlack,kRed,false,true,"EP","hist same");
   c1.PrintCanvas();
   
   MyCanvas c2("result/plot_" + tvar + "_WW",xaxis,"Entries");
   if (logx) c2.SetLogx();
   c2.CanvasWithHistogramsRatioPlot((TH1D*) gDirectory->Get("WW"),(TH1D*) gDirectory->Get("WW_MC"),"WW (data-driven)","WW (MC)","ratio",kBlack,kGreen,false,true,"EP","hist same");
   c2.PrintCanvas();

   MyCanvas c3("result/plot_" + tvar + "_DYtautau",xaxis,"Entries");
   if (logx) c3.SetLogx();
   c3.CanvasWithHistogramsRatioPlot((TH1D*) gDirectory->Get("DYtautau"),(TH1D*) gDirectory->Get("DYtautau_MC"),"DY (data-driven)","DY (MC)","ratio",kBlack,kBlue,false,true,"EP","hist same");
   c3.PrintCanvas();

   // SS
   // MyCanvas c4("result/plot_" + tvar + "_SS",xaxis,"Entries");
   // c4.CanvasWithHistogramsRatioPlot((TH1D*) gDirectory->Get("emuSS_data"),(TH1D*) gDirectory->Get("emuSS_sumMC"),"Data (SS)","MC (SS)","ratio",kBlack,kGray,false,true,"EP","hist same");
   // c4.PrintCanvas();
   TCanvas *c4 = new TCanvas();
   TH1D* emuSS_data = (TH1D*) gDirectory->Get("emuSS_data");
   TH1D* emuSS_sumMC = (TH1D*) gDirectory->Get("emuSS_sumMC");
   if (logx) c4->SetLogx();
   emuSS_data->GetYaxis()->SetTitle("Entries");
   emuSS_data->GetXaxis()->SetTitle(xaxis);
   emuSS_data->Draw();
   emuSS_data->GetYaxis()->SetRangeUser(0,2.5*emuSS_data->GetBinContent(emuSS_data->GetMaximumBin()));
   emuSS_data->Draw();
   emuSS_sumMC->SetFillColor(kGray);
   emuSS_sumMC->SetFillStyle(3002);
   emuSS_sumMC->Draw("hist same");
   TLegend *tleg = new TLegend(0.55,0.77,0.95,0.92);
   tleg->SetBorderSize(0);
   tleg->SetHeader("SS e#mu");
   tleg->AddEntry(emuSS_data, "Data (SS)", "lp");
   tleg->AddEntry(emuSS_sumMC, "MC (SS)", "f");
   tleg->Draw();
   c4->SaveAs("result/plot_" + tvar + "_emuSS.pdf");
}
