#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TPad.h"
#include "TStyle.h"
#include "TColor.h"
#include "TF1.h"
#include "TLine.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "THStack.h"

#include "../BkgEst/interface/defs.h"
#include "../Include/tdrstyle.C"
#include "../Include/CMS_lumi.C"
#include "../Include/PlotTools.h"

using namespace DYana;

void normBinWidth(TH1D *hist);

void dataMC(var thevar)
{
   setTDRStyle();

   const char* thevarname = varname(thevar);
   TString thexaxistitle = xaxistitle(thevar);

   TCanvas *c1 = new TCanvas("c1", "",0,0, 800, 800);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(0,0,1,1);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   //c1->SetGridx();
   //c1->SetGridy();
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.16);
   c1->SetRightMargin(0.02);
   c1->SetTopMargin(0.08);
   c1->SetBottomMargin(0.14);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: TopPad
   TPad *TopPad = new TPad("TopPad", "TopPad",0.01,0.01,0.99,0.99);
   TopPad->Draw();
   TopPad->cd();
   TopPad->Range(0.7100599,-11.71078,3.622756,3.443786);
   TopPad->SetFillColor(0);
   TopPad->SetBorderMode(0);
   TopPad->SetBorderSize(2);
   if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
      TopPad->SetLogx();
      TopPad->SetLogy();
   }
   //TopPad->SetGridx();
   //TopPad->SetGridy();
   TopPad->SetTickx(1);
   TopPad->SetTicky(1);
   TopPad->SetLeftMargin(0.13);
   TopPad->SetRightMargin(0.05);
   TopPad->SetTopMargin(0.05);
   TopPad->SetBottomMargin(0.32);
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);

   TFile* f1 = new TFile(Form("ControlPlots/root/ROOTFile_Histograms_%s_MomCorr00_rewboth_tnprew_noZptrew_All.root",thevarname));
   // TFile* f1 = new TFile(Form("ControlPlots/root/ROOTFile_Histograms_%s_MomCorr00_rewboth_tnprew_All.root",thevarname));
   f1->cd();

   TH1D* h_data = (TH1D*) f1->Get("h_data");
   TH1D* h_SignalMC = (TH1D*) f1->Get("h_SignalMC");
   TH1D* h_ttbar_emu = (TH1D*) f1->Get("h_ttbar_emu");
   TH1D* h_DYTauTau_emu = (TH1D*) f1->Get("h_DYTauTau_emu");
   TH1D* h_WW_emu = (TH1D*) f1->Get("h_WW_emu");
   TH1D* h_WZ = (TH1D*) f1->Get("h_WZ_MC");
   TH1D* h_ZZ = (TH1D*) f1->Get("h_ZZ_MC");
   TH1D* h_WJets_FR = (TH1D*) f1->Get("h_WJets_FR");
   TH1D* h_diJet_FR = (TH1D*) f1->Get("h_diJet_FR");
   TH1D* htotal = (TH1D*) h_SignalMC->Clone("htotal");
   htotal->Reset();
   TH1D* hratio = (TH1D*) h_data->Clone("hratio");

   // need to normalise all this by bin width
   normBinWidth(h_data);
   normBinWidth(h_SignalMC);
   normBinWidth(h_ttbar_emu);
   normBinWidth(h_DYTauTau_emu);
   normBinWidth(h_WW_emu);
   normBinWidth(h_WZ);
   normBinWidth(h_ZZ);
   normBinWidth(h_WJets_FR);
   normBinWidth(h_diJet_FR);

   // if pt or phistar: need to fix the X axis so that we can see the first bin
   if (TString(thevarname).Contains("pt") || TString(thevarname).Contains("phistar")
         || TString(thevarname).Contains("pt1560") || TString(thevarname).Contains("phistar1560")) {
      fixXaxis(h_data);
      fixXaxis(h_SignalMC);
      fixXaxis(h_ttbar_emu);
      fixXaxis(h_DYTauTau_emu);
      fixXaxis(h_WW_emu);
      fixXaxis(h_WZ);
      fixXaxis(h_ZZ);
      fixXaxis(h_WJets_FR);
      fixXaxis(h_diJet_FR);
      fixXaxis(htotal);
      fixXaxis(hratio);
   }

   TH1D *hdata = (TH1D*) h_data->Clone();
   TH1D *hDY = (TH1D*) h_SignalMC->Clone();
   TH1D *httbar = (TH1D*) h_ttbar_emu->Clone();
   TH1D *hDYtautau = (TH1D*) h_DYTauTau_emu->Clone();
   TH1D *hWW = (TH1D*) h_WW_emu->Clone(); hWW->Sumw2();
   TH1D *hWZ = (TH1D*) h_WZ->Clone(); hWZ->Sumw2();
   TH1D *hZZ = (TH1D*) h_ZZ->Clone(); hZZ->Sumw2();
   TH1D *hWJets = (TH1D*) h_WJets_FR->Clone();
   TH1D *hdijet = (TH1D*) h_diJet_FR->Clone();
   TH1D *hdiboson = (TH1D*) hWW->Clone();
   hdiboson->Add( hWZ );
   hdiboson->Add( hZZ );

   TH1D* hTop = (TH1D*) httbar->Clone();
   TH1D* hEW = (TH1D*) hdiboson->Clone();
   hEW->Add(hDYtautau);
   hEW->Add(hWJets);

   hDY->SetFillColor(kOrange-2);
   hDY->SetLineColor(kOrange+3);
   hTop->SetFillColor(kRed+2);
   hTop->SetLineColor(kRed+4);
   hEW->SetFillColor(kOrange+10);
   hEW->SetLineColor(kOrange+3);
   hdijet->SetFillColor(kViolet-5);
   hdijet->SetLineColor(kOrange+3);

   THStack* hstack = new THStack("hstack", "hstack");
   hstack->Add(hdijet, "hist");
   hstack->Add(hEW, "hist");
   hstack->Add(hTop, "hist");
   hstack->Add(hDY, "hist");

   htotal->Add(hdijet);
   htotal->Add(hEW);
   htotal->Add(hTop);
   htotal->Add(hDY);
   htotal->Sumw2();
   hratio->Divide(hdata, htotal);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 932;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   h_data->SetFillColor(ci);
   h_data->SetMarkerStyle(20);
   h_data->SetMarkerSize(1.0);
   h_data->GetXaxis()->SetTitle(thexaxistitle);
   // h_data->GetXaxis()->SetRange(1,43);
   if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::phistar || thevar == var::pt1560 || thevar == var::phistar1560) {
      h_data->GetXaxis()->SetMoreLogLabels();
   }
   h_data->GetXaxis()->SetNoExponent();
   h_data->GetXaxis()->SetLabelFont(42);
   h_data->GetXaxis()->SetLabelSize(0);
   h_data->GetXaxis()->SetTitleSize(0);
   h_data->GetXaxis()->SetTitleFont(42);
   if (thevar==var::mass || thevar==var::mass3bins) h_data->GetYaxis()->SetTitle("Entries / GeV/c^{2}");
   else if (thevar==var::pt || thevar==var::pt1560) h_data->GetYaxis()->SetTitle("Entries / GeV/c");
   else h_data->GetYaxis()->SetTitle("Entries / 1");
   h_data->GetYaxis()->SetLabelFont(42);
   h_data->GetYaxis()->SetLabelSize(0.035);
   h_data->GetYaxis()->SetTitleSize(0.06);
   h_data->GetYaxis()->SetTitleOffset(0.9);
   h_data->GetYaxis()->SetTitleFont(42);
   h_data->GetZaxis()->SetLabelFont(42);
   h_data->GetZaxis()->SetLabelSize(0.035);
   h_data->GetZaxis()->SetTitleSize(0.035);
   h_data->GetZaxis()->SetTitleFont(42);
   double yaxismin = 0.001;
   if (thevar == var::pt) {
      yaxismin = 0.1;
      h_data->SetMaximum(2e4);
   } else if (thevar == var::pt1560) {
      yaxismin = 0.1;
      h_data->SetMaximum(1900);
   } else if (thevar == var::rap1560) {
      yaxismin = 0;
      h_data->SetMaximum(1550);
   } else if (thevar == var::rap60120) {
      yaxismin = 0;
      h_data->SetMaximum(8150);
   } else if (thevar == var::phistar) {
      yaxismin = 90;
      h_data->SetMaximum(3e6);
   } else if (thevar == var::phistar1560) {
      yaxismin = 10;
      h_data->SetMaximum(5e5);
   }
   h_data->SetMinimum(yaxismin);
   h_data->Draw("E1P");
   if (thevar==var::pt) h_data->GetXaxis()->SetRangeUser(0.5,ptbin_meas[ptbinnum_meas-1]);
   if (thevar==var::pt1560) h_data->GetXaxis()->SetRangeUser(0.5,ptbin_meas_1560[ptbinnum_meas_1560-1]);
   if (thevar==var::phistar) h_data->GetXaxis()->SetRangeUser(0.001,phistarbin[phistarnum-1]);
   if (thevar==var::phistar1560) h_data->GetXaxis()->SetRangeUser(0.001,phistarbin_1560[phistarnum_1560-1]);
   h_data->Draw("E1P");
   hstack->Draw("histsame");
   h_data->Draw("E1Psame");
   //
   TLine grid_;
   grid_.SetLineColor(kGray+2);
   grid_.SetLineStyle(kSolid);
   const double *xAxis = hdata->GetXaxis()->GetXbins()->GetArray();
   for( unsigned int ii=0; ii<hdata->GetNbinsX()+1; ii++ ) {
     grid_.DrawLine(xAxis[ii],yaxismin,xAxis[ii],htotal->GetBinContent(ii+1));
   }

   TLegend *leg = new TLegend(0.70,0.70,0.90,0.90,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   hdata->SetMarkerSize(1);
   TLegendEntry *entry=leg->AddEntry(hdata,"data","pl");
   entry=leg->AddEntry(hDY,"#gamma* /#font[122]{Z} #rightarrow #mu#mu","f");
   entry=leg->AddEntry(hTop,"t#bar{t}","f");
   entry=leg->AddEntry(hEW,"EW","f");
   entry=leg->AddEntry(hdijet,"QCD","f");
   leg->Draw();

   // TPaveText *pave = new TPaveText(1200,5000000,2000,7000000); // CHANGE
   // pave->SetFillColor(0);
   // TText* text = pave->AddText("2.8 fb^{#font[122]{\55}1} (13 TeV)");
   // text->SetTextSize(0.03);
   // pave->Draw("0");
   // TPaveText *ptitle = new TPaveText(20,900000,60,1020000); // CHANGE
   // ptitle->SetFillColor(0);
   // TText* ttitle = ptitle->AddText("CMS Preliminary");
   // ttitle->SetTextSize(0.035);
   // ptitle->Draw("0");
   CMS_lumi( TopPad, 111, 0 );
   TopPad->RedrawAxis();

   // add label
   TLatex latex;
   latex.SetNDC();
   latex.SetTextSize(0.03);
   double xlatex=.17, ylatex=0.9, dylatex=0.04;
   if (thevar==var::rap1560 || thevar==rap60120) ylatex=0.9;
   latex.SetTextAlign(12);  //centered
   if (thevar!=rap1560 && thevar!=rap60120) {
      latex.DrawLatex(xlatex,ylatex,"-2.87 < |y_{CM}| < 1.93");
      ylatex -= dylatex;
   }
   if (thevar==pt1560 || thevar==phistar1560 || thevar==rap1560) {
      latex.DrawLatex(xlatex,ylatex,"15 < M < 60 GeV/c^{2}");
      ylatex -= dylatex;
   }
   if (thevar==pt || thevar==phistar || thevar==rap60120) {
      latex.DrawLatex(xlatex,ylatex,"60 < M < 120 GeV/c^{2}");
      ylatex -= dylatex;
   }
   latex.DrawLatex(xlatex,ylatex,"|#eta_{lab}^{#mu}|<2.4, p_{T}^{#mu} > 15 (10) GeV/c");
   ylatex -= dylatex;

// ------------>Primitives in pad: bottomPad
   TPad *bottomPad = new TPad("bottomPad", "bottomPad",0.01,0.01,0.99,0.3);
   bottomPad->Draw();
   bottomPad->cd();
   bottomPad->Range(0.7499746,-0.2272727,3.590752,1.590909);
   bottomPad->SetFillColor(0);
   bottomPad->SetBorderMode(0);
   bottomPad->SetBorderSize(2);
   if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::phistar || thevar==var::pt1560 || thevar==var::phistar1560)  bottomPad->SetLogx();
   //bottomPad->SetGridx();
   //bottomPad->SetGridy();
   bottomPad->SetTickx(1);
   bottomPad->SetTicky(1);
   bottomPad->SetLeftMargin(0.122);
   bottomPad->SetRightMargin(0.04);
   bottomPad->SetTopMargin(0.05);
   bottomPad->SetBottomMargin(0.4);
   bottomPad->SetFrameFillStyle(0);
   bottomPad->SetFrameBorderMode(0);
   bottomPad->SetFrameFillStyle(0);
   bottomPad->SetFrameBorderMode(0);

   hratio->SetMinimum(0.45);
   hratio->SetMaximum(1.55);
   hratio->SetEntries(129);
   hratio->SetDirectory(0);
   hratio->SetStats(0);

   hratio->SetFillColor(kBlack);
   hratio->SetFillStyle(1001);
   hratio->SetMarkerStyle(20);
   hratio->GetXaxis()->SetTitle(thexaxistitle);
   // hratio->GetXaxis()->SetRange(1,43);
   if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) hratio->GetXaxis()->SetMoreLogLabels();
   hratio->GetXaxis()->SetNoExponent();
   hratio->GetXaxis()->SetLabelFont(42);
   hratio->GetXaxis()->SetLabelOffset(0.007);
   hratio->GetXaxis()->SetLabelSize(0.15);
   hratio->GetXaxis()->SetTitleSize(0.2);
   hratio->GetXaxis()->SetTitleOffset(0.9);
   hratio->GetXaxis()->SetTitleFont(42);
   hratio->GetYaxis()->SetTitle("Data/(DY+Bkg)");
   hratio->GetYaxis()->SetLabelFont(42);
   hratio->GetYaxis()->SetLabelSize(0.07);
   hratio->GetYaxis()->SetTitleSize(0.1);
   hratio->GetYaxis()->SetTitleOffset(0.4);
   hratio->GetYaxis()->SetTitleFont(42);
   // hratio->GetYaxis()->SetRangeUser(42);
   hratio->GetZaxis()->SetLabelFont(42);
   hratio->GetZaxis()->SetLabelSize(0.035);
   hratio->GetZaxis()->SetTitleSize(0.035);
   hratio->GetZaxis()->SetTitleFont(42);
   hratio->Draw("E1PL");
   if (thevar==var::pt) hratio->GetXaxis()->SetRangeUser(0.5,ptbin_meas[ptbinnum_meas-1]);
   if (thevar==var::pt1560) hratio->GetXaxis()->SetRangeUser(0.5,ptbin_meas_1560[ptbinnum_meas_1560-1]);
   if (thevar==var::phistar) hratio->GetXaxis()->SetRangeUser(0.001,phistarbin[phistarnum-1]);
   if (thevar==var::phistar1560) hratio->GetXaxis()->SetRangeUser(0.001,phistarbin_1560[phistarnum_1560-1]);
   hratio->Draw("E1PL");
   
   TF1 *f_line1 = new TF1("f_line","1",-10000,10000);
   f_line1->SetFillColor(19);
   f_line1->SetFillStyle(0);
   f_line1->SetMarkerStyle(20);

   f_line1->SetLineColor(kSpring-1);
   f_line1->SetLineStyle(2);
   f_line1->SetLineWidth(1);
   f_line1->GetXaxis()->SetLabelFont(42);
   f_line1->GetXaxis()->SetLabelOffset(0.007);
   f_line1->GetXaxis()->SetLabelSize(0.05);
   f_line1->GetXaxis()->SetTitleSize(0.06);
   f_line1->GetXaxis()->SetTitleOffset(0.9);
   f_line1->GetXaxis()->SetTitleFont(42);
   f_line1->GetYaxis()->SetLabelFont(42);
   f_line1->GetYaxis()->SetLabelOffset(0.007);
   f_line1->GetYaxis()->SetLabelSize(0.05);
   f_line1->GetYaxis()->SetTitleSize(0.06);
   f_line1->GetYaxis()->SetTitleOffset(1.25);
   f_line1->GetYaxis()->SetTitleFont(42);
   f_line1->Draw("SAME");
   hratio->Draw("E1PLsame");

   TLine gridRatio;
   gridRatio.SetLineColor(kGray+2);
   gridRatio.SetLineStyle(2);
   for( unsigned int ii=0; ii<hdata->GetNbinsX()+1; ii++ ) {
     gridRatio.DrawLine(xAxis[ii],0.45,xAxis[ii],1.55);
   }

   c1->cd();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->SaveAs(Form("OfficialStyle/%s.C",thevarname));
   c1->SaveAs(Form("OfficialStyle/%s.pdf",thevarname));

   // save the ratio
   TFile *fratio = TFile::Open("OfficialStyle/dataMCratios.root","UPDATE");
   hratio->Write(thevarname);
   fratio->Close();
}

void dataMC() {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      dataMC(thevar_i);
   }
}
