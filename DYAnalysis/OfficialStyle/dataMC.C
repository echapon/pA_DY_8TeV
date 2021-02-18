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
#include "../Include/lhapdf_utils.h"
#include "../SysUncEstimation/syst.C"

using namespace DYana;

void normBinWidth(TH1D *hist);

const double fontsize = 0.10;
const int font = 42;

void dataMC(var thevar)
{
   setTDRStyle();

   const char* thevarname = varname(thevar);
   TString thexaxistitle = xaxistitle(thevar);

   // general multiplicator to enlarge or reduce the size of everything
   const float sizemod = (thevar==var::mass) ? 0.85 : 1.15;
   float lumiTextSize0 = lumiTextSize;
   float lumiTextOffset0 = lumiTextSize;
   float cmsTextSize0 = cmsTextSize;
   float cmsTextOffset0 = cmsTextSize;
   lumiTextSize *= sizemod;
   if (sizemod>1) lumiTextOffset *= 0.5;
   else lumiTextOffset *= 0.8;
   cmsTextSize *= sizemod;
   if (sizemod>1) cmsTextOffset *= 0.5;
   else cmsTextOffset *= 0.8;

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
   c1->SetLeftMargin(0.16*sizemod);
   c1->SetRightMargin(0.02);
   c1->SetTopMargin(0.08);//0.08
   c1->SetBottomMargin(0.1);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: TopPad
   TPad *TopPad = new TPad("TopPad", "TopPad",0.01,0.01,0.99,0.99);
//###   TPad *TopPad = new TPad("TopPad", "TopPad",0.01,0.30,0.99,0.99);


   TopPad->Draw();
   TopPad->cd();
//   TopPad->Range(0.7100599,-11.71078,3.622756,3.443786);
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
   TopPad->SetLeftMargin(0.142*sizemod);
   if (sizemod>1) TopPad->SetLeftMargin(0.152*sizemod);
   TopPad->SetRightMargin(0.04);
   TopPad->SetTopMargin(0.08);//0.05
   TopPad->SetBottomMargin(0.32);//0.32
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);
   TopPad->SetFrameFillStyle(0);
   TopPad->SetFrameBorderMode(0);

   // This is default: no Zpt rew
   TFile* f1 = new TFile(Form("ControlPlots/root/ROOTFile_Histograms_%s_MomCorr00_rewboth_tnprew_noZptrew_All.root",thevarname));
   // // This is for test: with Zpt rew
   // TFile* f1 = new TFile(Form("ControlPlots/root/ROOTFile_Histograms_%s_MomCorr00_rewboth_tnprew_All.root",thevarname));
   f1->cd();

   TH1D* h_data = (TH1D*) f1->Get("h_data");
   TH1D* h_SignalMC = (TH1D*) f1->Get("h_SignalMC");
   TH1D* h_ttbar_emu = (TH1D*) f1->Get("h_ttbar_emu");
   TH1D* h_DYTauTau_emu = (TH1D*) f1->Get("h_DYTauTau_emu");
   TH1D* h_WW_emu = (TH1D*) f1->Get("h_WW_emu");
   TH1D* h_WZ_emu = (TH1D*) f1->Get("h_WZ_emu");
   TH1D* h_ZZ_emu = (TH1D*) f1->Get("h_ZZ_emu");
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
   normBinWidth(h_WZ_emu);
   normBinWidth(h_ZZ_emu);
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
      fixXaxis(h_WZ_emu);
      fixXaxis(h_ZZ_emu);
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
   TH1D *hWZ = (TH1D*) h_WZ_emu->Clone(); hWZ->Sumw2();
   TH1D *hZZ = (TH1D*) h_ZZ_emu->Clone(); hZZ->Sumw2();
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

   // for the ratio, set all uncertainties to 0
   for (int i=0; i<htotal->GetNbinsX()+2; i++) htotal->SetBinError(i,0);

   hratio->Divide(hdata, htotal);

   // now the histogram with the systs
   TH1D *hsyst = (TH1D*) htotal->Clone("hsyst");

   vector< map<bin, syst> > systmap_all;

   vector<TString> tags;
   tags.push_back("rewNtracks");
   tags.push_back("MomCorr_smooth");
   tags.push_back("tnp_tot");
   tags.push_back("effstat_up");
   tags.push_back("bkg");
   tags.push_back("Eff_theory");
   tags.push_back("lumi");

   for (vector<TString>::const_iterator it=tags.begin(); it!=tags.end(); it++) {
      map<bin,syst> systmap;
      TString systfilename = "SysUncEstimation/csv/" + TString(*it) + "_" + TString(varname(thevar)) + ".csv";
      cout << systfilename << endl;
      systmap = readSyst(systfilename.Data());
      systmap_all.push_back(systmap);
   }

   map<bin, syst> ans = combineSyst(systmap_all, "Total");

   // one last thing: add theory uncertainties
   TFile *fth_EPPS16 = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/ROOTFile_Histogram_Acc_weights_genonly_EPPS16.root");
   vector<TH1D*> hth_EPPS16;
   int i=0;
   const char* acceffstr = "AccPass";

   TGraphAsymmErrors *gth_EPPS16 = NULL;
   if (fth_EPPS16->IsOpen()) { // skip the theory part if we don't want dsigma/dX
      hth_EPPS16.push_back((TH1D*) fth_EPPS16->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
      for (i=285; i<=324; i++) {
         hth_EPPS16.push_back((TH1D*) fth_EPPS16->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
      }
      for (i=112; i<=167; i++) {
         hth_EPPS16.push_back((TH1D*) fth_EPPS16->Get(Form("h_%s_%s%d",varname(thevar),acceffstr,i)));
      }
      gth_EPPS16 = pdfuncert(hth_EPPS16, "EPPS16nlo_CT14nlo_Pb208");
   }

   // combine syst with theory uncertainty
   int isyst=1;
   for (auto it = ans.begin(); it != ans.end(); it++) {
      // relative syst value
      double systval = it->second.value;

      // theory uncertainty
      double th_up = gth_EPPS16->GetEYhigh()[isyst-1];
      double th_down = gth_EPPS16->GetEYlow()[isyst-1];
      double thval = gth_EPPS16->GetY()[isyst-1];
      double therr = (th_up+th_down)/(2.*thval);

      cout << isyst << " " << systval << " " << therr << endl;
      double totunc = sqrt(pow(systval,2)+pow(therr,2));

      hsyst->SetBinError(isyst, totunc * hsyst->GetBinContent(isyst));
      isyst++;
   }

   // now the ratio for syst
   TH1D *hratio_syst = (TH1D*) htotal->Clone("hratio_syst");
   hratio_syst->Divide(hsyst,htotal);

   double st = 0.35/0.6;
   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = 932;
   color = new TColor(ci, 1, 1, 1, " ", 0);
   h_data->SetFillColor(ci);
   h_data->SetMarkerStyle(20);
   h_data->SetMarkerSize(1.5*sizemod);
   h_data->GetXaxis()->SetTitle(thexaxistitle);
   // h_data->GetXaxis()->SetRange(1,43);
   // if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::pt1560) {
      // h_data->GetXaxis()->SetMoreLogLabels();
   // }
   if (thevar==var::mass) {
      h_data->GetXaxis()->SetMoreLogLabels(); 
      h_data->GetXaxis()->SetNoExponent();
   }
   if (thevar==var::phistar || thevar == var::phistar1560) {
      // h_data->GetXaxis()->SetMoreLogLabels();
      // h_data->GetXaxis()->SetNdivisions(0,4,3,kTRUE);
   }
   h_data->GetXaxis()->SetNoExponent();
   h_data->GetXaxis()->SetLabelFont(font);
   h_data->GetXaxis()->SetLabelSize(0);
   h_data->GetXaxis()->SetTitleSize(0);
   h_data->GetXaxis()->SetTitleFont(font);
   if (thevar==var::mass || thevar==var::mass3bins) h_data->GetYaxis()->SetTitle("Entries / GeV");
   else if (thevar==var::pt || thevar==var::pt1560) h_data->GetYaxis()->SetTitle("Entries / GeV");
   else if (thevar==var::phistar || thevar==var::phistar1560) h_data->GetYaxis()->SetTitle("Entries / unit #lower[0.1]{#phi}#lower[-0.1]{#scale[0.8]{*}}");
   else h_data->GetYaxis()->SetTitle("Entries / unit y");
   // else h_data->GetYaxis()->SetTitle("Entries / unit 1");

   h_data->GetYaxis()->SetLabelFont(font);
   h_data->GetYaxis()->SetLabelSize(fontsize*sizemod*st);
   //h_data->GetYaxis()->SetLabelOffset(1.1);
   h_data->GetYaxis()->SetTitleSize(fontsize*sizemod*st);
   h_data->GetYaxis()->SetTitleOffset(1.1*sizemod);//0.9
   if (thevar==var::rap60120 || thevar==var::rap1560) {
      h_data->GetYaxis()->SetLabelSize(0.7*fontsize*st);//*sizemod);
   }
   if (thevar==var::mass) {
      h_data->GetYaxis()->SetTitleOffset(1.5*sizemod);//0.9
   }
   h_data->GetYaxis()->SetTitleFont(font);
   h_data->GetZaxis()->SetLabelFont(font);
   h_data->GetZaxis()->SetLabelSize(fontsize*sizemod*st);
   h_data->GetZaxis()->SetTitleSize(fontsize*sizemod*st);
   h_data->GetZaxis()->SetTitleFont(font);
   double yaxismin = 0.001;
   if (thevar == var::pt) {
      yaxismin = 0.11;
      h_data->SetMaximum(2e5);//2e4
   } else if (thevar == var::pt1560) {
      yaxismin = 0.08;
      h_data->SetMaximum(20000);//1900
   } else if (thevar == var::rap1560) {
      yaxismin = 0;
      h_data->SetMaximum(1750);
   } else if (thevar == var::rap60120) {
      yaxismin = 0;
      h_data->SetMaximum(12250);//8150
   } else if (thevar == var::phistar) {
      yaxismin = 9;
      h_data->SetMaximum(5e7);//3e6
   } else if (thevar == var::phistar1560) {
      yaxismin = 40;
      h_data->SetMaximum(9e5);
   }
   h_data->SetMinimum(yaxismin);
   h_data->Draw("E1P");
   // if (thevar==var::pt) h_data->GetXaxis()->SetRangeUser(0.5,ptbin_meas[ptbinnum_meas]);
   // if (thevar==var::pt1560) h_data->GetXaxis()->SetRangeUser(0.5,ptbin_meas_1560[ptbinnum_meas_1560]);
   // if (thevar==var::phistar) h_data->GetXaxis()->SetRangeUser(0.001,phistarbin[phistarnum-1]);
   // if (thevar==var::phistar1560) h_data->GetXaxis()->SetRangeUser(0.001,phistarbin_1560[phistarnum_1560-1]);
   h_data->Draw("E1P");
   hstack->Draw("histsame");
   h_data->Draw("E1Psame");
   hsyst->SetFillColorAlpha(kBlack, 0.5);
   hsyst->SetMarkerSize(0);
   // hsyst->SetFillStyle(3003);
   hsyst->Draw("E2same");
   //
   TLine grid_;
   grid_.SetLineColor(kGray+2);
   grid_.SetLineStyle(kSolid);
   const double *xAxis = hdata->GetXaxis()->GetXbins()->GetArray();
   for( unsigned int ii=0; ii<hdata->GetNbinsX()+1; ii++ ) {
     grid_.DrawLine(xAxis[ii],yaxismin,xAxis[ii],htotal->GetBinContent(ii+1));
   }

   double dx = (0.92 - 0.70) * sizemod;
   double dy = (0.90 - 0.70) * sizemod;
   TLegend *leg = new TLegend(0.92-dx,0.90-dy,0.92,0.90,NULL,"brNDC");//0.92
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   hdata->SetMarkerSize(1.5*sizemod);
   TLegendEntry *entry=leg->AddEntry(hdata,"Data","pl");
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
   latex.SetTextSize(0.034*sizemod);
   double xlatex=.15, ylatex=0.855;//0.89
   if (thevar != var::mass) xlatex += 0.05;
	double dylatex=0.045*sizemod;
   if (thevar==var::rap1560 || thevar==rap60120) ylatex=0.87;//0.9
   latex.SetTextAlign(12);  //centered
   if (thevar!=rap1560 && thevar!=rap60120) {
      latex.DrawLatex(xlatex,ylatex,"-2.87 < |y_{CM}| < 1.93");
      ylatex -= dylatex;
   }
   if (thevar==pt1560 || thevar==phistar1560 || thevar==rap1560) {
      latex.DrawLatex(xlatex,ylatex,"15 < m_{#mu#mu} < 60 GeV");
      ylatex -= dylatex;
   }
   if (thevar==pt || thevar==phistar || thevar==rap60120) {
      latex.DrawLatex(xlatex,ylatex,"60 < m_{#mu#mu} < 120 GeV");
      ylatex -= dylatex;
   }
   latex.DrawLatex(xlatex,ylatex,"|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
   ylatex -= dylatex;

   // ------------>Primitives in pad: bottomPad
   c1->cd();
   TPad *bottomPad = new TPad("bottomPad", "bottomPad",0.01,0.01,0.99,0.3);
   double sb = 1./st;
   bottomPad->Draw();
   bottomPad->cd();
   // bottomPad->Range(0.7499746,-0.2272727,3.590752,1.590909);
   bottomPad->SetFillColor(0);
   bottomPad->SetBorderMode(0);
   bottomPad->SetBorderSize(2);
   if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar == var::phistar || thevar==var::pt1560 || thevar==var::phistar1560)  bottomPad->SetLogx();
   //bottomPad->SetGridx();
   //bottomPad->SetGridy();
   bottomPad->SetTickx(1);
   bottomPad->SetTicky(1);
   bottomPad->SetLeftMargin(0.142*sizemod);
   if (sizemod>1) bottomPad->SetLeftMargin(0.152*sizemod);
   bottomPad->SetRightMargin(0.04);
   bottomPad->SetTopMargin(0.05);//0.05
   bottomPad->SetBottomMargin(0.35*sizemod);
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
   hratio->SetMarkerSize(1.5*sizemod);
   hratio->GetXaxis()->SetTitle(thexaxistitle);
   // hratio->GetXaxis()->SetRange(1,43);
   // if (thevar==var::mass || thevar==var::mass3bins || thevar==var::pt || thevar==var::pt1560) hratio->GetXaxis()->SetMoreLogLabels();
   if (thevar==var::mass) {
      hratio->GetXaxis()->SetMoreLogLabels(); 
      hratio->GetXaxis()->SetNoExponent();
   } else {
      hratio->GetYaxis()->SetNdivisions(5,5,0);//,kTRUE);
   }
   if (!(thevar==var::phistar || thevar==var::phistar1560)) {
      hratio->GetXaxis()->SetNoExponent();
      // hratio->GetXaxis()->SetMoreLogLabels();
      // hratio->GetXaxis()->SetNdivisions(5,5,0);//,kTRUE);
   }
   hratio->GetXaxis()->SetLabelFont(font);
   hratio->GetXaxis()->SetLabelOffset(0.007);
   if (sizemod>1) hratio->GetXaxis()->SetLabelOffset(-0.003);
   hratio->GetXaxis()->SetLabelSize(fontsize*sizemod*sb);
   if (thevar==var::rap1560 || thevar==var::rap60120) hratio->GetXaxis()->SetLabelSize(fontsize*sizemod*sb*0.9);
   hratio->GetXaxis()->SetTitleSize(fontsize*sizemod*sb);
   if (thevar==var::mass) hratio->GetXaxis()->SetTitleOffset(1.1*sizemod);
   else hratio->GetXaxis()->SetTitleOffset(0.8*sizemod);
   hratio->GetXaxis()->SetTitleFont(font);
   hratio->GetYaxis()->SetTitle("Data/Pred."); // "Data/(DY+Bkg)");
   hratio->GetYaxis()->SetLabelFont(font);
   hratio->GetYaxis()->SetLabelSize(fontsize*sizemod*sb);
   if (thevar==var::rap1560 || thevar==var::rap60120) hratio->GetYaxis()->SetLabelSize(fontsize*sizemod*sb*0.9);
   hratio->GetYaxis()->SetTitleSize(fontsize*sizemod*sb);
   if (thevar==var::mass) hratio->GetYaxis()->SetTitleOffset(0.5*sizemod);
   else hratio->GetYaxis()->SetTitleOffset(0.35*sizemod);
   hratio->GetYaxis()->SetTitleFont(font);
   // hratio->GetYaxis()->SetRangeUser(42);
   hratio->GetZaxis()->SetLabelFont(font);
   hratio->GetZaxis()->SetLabelSize(fontsize*sizemod*sb);
   hratio->GetZaxis()->SetTitleSize(fontsize*sizemod*sb);
   hratio->GetZaxis()->SetTitleFont(font);
   hratio->Draw("E1PL");
   
   // if (thevar==var::pt) hratio->GetXaxis()->SetRangeUser(0.5,ptbin_meas[ptbinnum_meas-1]);
   // if (thevar==var::pt1560) hratio->GetXaxis()->SetRangeUser(0.5,ptbin_meas_1560[ptbinnum_meas_1560-1]);
   // if (thevar==var::phistar) hratio->GetXaxis()->SetRangeUser(0.001,phistarbin[phistarnum-1]);
   // if (thevar==var::phistar1560) hratio->GetXaxis()->SetRangeUser(0.001,phistarbin_1560[phistarnum_1560-1]);
   // hratio->Draw("E1PL");
   
   // TF1 *f_line1 = new TF1("f_line","1",-10000,10000);
   // f_line1->SetFillColor(19);
   // f_line1->SetFillStyle(0);
   // f_line1->SetMarkerStyle(20);

   // f_line1->SetLineColor(kSpring-1);
   // f_line1->SetLineStyle(2);
   // f_line1->SetLineWidth(1*sizemod/sb);
   // f_line1->GetXaxis()->SetLabelFont(font);
   // f_line1->GetXaxis()->SetLabelOffset(0.007);
   // f_line1->GetXaxis()->SetLabelSize(fontsize*sizemod);
   // f_line1->GetXaxis()->SetTitleSize(fontsize*sizemod);
   // f_line1->GetXaxis()->SetTitleOffset(0.9);//*sizemod);
   // f_line1->GetXaxis()->SetTitleFont(font);
   // f_line1->GetYaxis()->SetLabelFont(font);
   // f_line1->GetYaxis()->SetLabelOffset(0.007*sizemod/sb);
   // f_line1->GetYaxis()->SetLabelSize(fontsize*sizemod);
   // f_line1->GetYaxis()->SetTitleSize(fontsize*sizemod);
   // f_line1->GetYaxis()->SetTitleOffset(1.25);//*sizemod);
   // f_line1->GetYaxis()->SetTitleFont(font);
   // f_line1->Draw("SAME");

   // fix the X axis labels...
   // case of mass
   if (thevar==var::mass) {
      for (int i=1; i<13; i++) {
         if (i>4 && i<9) hratio->GetXaxis()->ChangeLabel(i,-1,0);
      }
   } else if (thevar==var::pt || thevar==var::pt1560) {
      for (int i=0; i<30; i++) {
         if (i > 5 && i<10) hratio->GetXaxis()->ChangeLabel(i,-1,0);
         if (i > 30 && i<100) hratio->GetXaxis()->ChangeLabel(i,-1,0);
         if (i > 200) hratio->GetXaxis()->ChangeLabel(i,-1,0);
      }
   }
   hratio->Draw("E1PL");

   hratio_syst->SetFillColorAlpha(kBlack, 0.5);
   hratio_syst->SetMarkerSize(0);
   // hratio_syst->SetFillStyle(3003);
   hratio_syst->Draw("E2same");

   // TLine gridRatio;
   // gridRatio.SetLineColor(kGray+2);
   // gridRatio.SetLineStyle(2);
   // for( unsigned int ii=0; ii<hdata->GetNbinsX()+1; ii++ ) {
   //   gridRatio.DrawLine(xAxis[ii],0.45,xAxis[ii],1.55);
   // }

   TLine gridRatio;
   gridRatio.SetLineColor(kRed);
   gridRatio.SetLineStyle(2);
   // if (thevar==var::pt) gridRatio.DrawLine(0.5,1.0,ptbin_meas[ptbinnum_meas-1],1.0);
   // else if (thevar==var::pt1560) gridRatio.DrawLine(0.5,1.0,ptbin_meas_1560[ptbinnum_meas_1560-1],1.0);
   //  else gridRatio.DrawLine(xAxis[0],1.0,xAxis[hdata->GetNbinsX()],1.0);
   double linemin = hratio->GetBinCenter(1)-hratio->GetBinWidth(1)/2.;
   double linemax = hratio->GetBinCenter(hratio->GetNbinsX())+hratio->GetBinWidth(hratio->GetNbinsX())/2.;
   gridRatio.DrawLine(linemin,1.0,linemax,1.0);
	
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

   // restore CMS parameters
   lumiTextSize = lumiTextSize0;
   lumiTextOffset = lumiTextSize0;
   cmsTextSize = cmsTextSize0;
   cmsTextOffset = cmsTextSize0;
}

void dataMC() {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      dataMC(thevar_i);
   }
}
