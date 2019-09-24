#include "../interface/defs.h"

#include "TFile.h"
#include "TH1.h"
#include "TVirtualFitter.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TString.h"
#include "THStack.h"

const TString isotag="0.0-0.2";
const TString nonisotag="0.2-100.0";

using namespace std;
using namespace DYana;
using namespace DYana_v20180814;

void DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_EW, TH1D* h_Wj, TH1D *h_QCD, TString XTitle);
void DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_pred, THStack *hs, TLegend *legend, TString XTitle);

void runABCD_improved2(var thevar, TString hivtxtag = "", const char* fdata="ROOTFile_Histogram_InvMass_isos_PAL3Mu12_Data_MomCorr00_noHFrew_notnprew.root", const char* fMC="ROOTFile_Histogram_InvMass_isos_PAL3Mu12_Powheg_MomCorr00_rewboth_tnprew.root", double SSfactor=2.15824, double SSfactor_err=0.1) {
   TFile *tfdata = TFile::Open(fdata);
   TFile *tfMC = TFile::Open(fMC);

   TString thevart(Varname(thevar));

   TH1D* h_OS_0iso[NSamples+1];
   TH1D* h_OS_1iso[NSamples+1];
   TH1D* h_OS_2iso[NSamples+1];
   TH1D* h_SS_0iso[NSamples+1];
   TH1D* h_SS_1iso[NSamples+1];
   TH1D* h_SS_2iso[NSamples+1];

   for(int i=0;i<ALL;i++) {
      SampleTag tag = static_cast<SampleTag>(i);

      if (IsData(tag)) {
         h_OS_0iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_" + hivtxtag + nonisotag + "_" + nonisotag + "_" + Name(tag));
         h_OS_1iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_" + hivtxtag + isotag + "_" + nonisotag + "_" + Name(tag));
         h_OS_2iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_" + hivtxtag + isotag + "_" + isotag + "_" + Name(tag));
         h_SS_0iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_SS_" + hivtxtag + nonisotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_1iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_SS_" + hivtxtag + isotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_2iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_SS_" + hivtxtag + isotag + "_" + isotag + "_" + Name(tag));
      } else {
         h_OS_0iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_" + hivtxtag + nonisotag + "_" + nonisotag + "_" + Name(tag));
         cout << "h_" + thevart + "_" + hivtxtag + nonisotag + "_" + nonisotag + "_" + Name(tag) << endl;
         h_OS_1iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_" + hivtxtag + isotag + "_" + nonisotag + "_" + Name(tag));
         h_OS_2iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_" + hivtxtag + isotag + "_" + isotag + "_" + Name(tag));
         h_SS_0iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + hivtxtag + nonisotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_1iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + hivtxtag + isotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_2iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + hivtxtag + isotag + "_" + isotag + "_" + Name(tag));

         double lumi = lumi_all;
         if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
            lumi = switcheta(tag)>0 ? lumi_part1 : lumi_part2;
         }
         double norm = (Xsec(tag)*lumi)/Nevts(tag) ;
         // cout << Name(tag) << " " << lumi << " " << norm << endl;
         h_OS_0iso[i]->Scale(norm);
         h_OS_1iso[i]->Scale(norm);
         h_OS_2iso[i]->Scale(norm);
         h_SS_0iso[i]->Scale(norm);
         h_SS_1iso[i]->Scale(norm);
         h_SS_2iso[i]->Scale(norm);
      }
   }

   h_OS_0iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_" + hivtxtag + nonisotag + "_" + nonisotag + "_" + Name(QCD));
   h_OS_1iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_" + hivtxtag + isotag + "_" + nonisotag + "_" + Name(QCD));
   h_OS_2iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_" + hivtxtag + isotag + "_" + isotag + "_" + Name(QCD));
   h_SS_0iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + hivtxtag + nonisotag + "_" + nonisotag + "_" + Name(QCD));
   h_SS_1iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + hivtxtag + isotag + "_" + nonisotag + "_" + Name(QCD));
   h_SS_2iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + hivtxtag + isotag + "_" + isotag + "_" + Name(QCD));

   // add data together
   h_OS_0iso[Data1]->Add(h_OS_0iso[Data2]);
   h_OS_1iso[Data1]->Add(h_OS_1iso[Data2]);
   h_OS_2iso[Data1]->Add(h_OS_2iso[Data2]);
   h_SS_0iso[Data1]->Add(h_SS_0iso[Data2]);
   h_SS_1iso[Data1]->Add(h_SS_1iso[Data2]);
   h_SS_2iso[Data1]->Add(h_SS_2iso[Data2]);

   // add EWK processes together
   for (int i=1; i<=DYLast; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      if (IsWjets(tag)) continue;

      h_OS_0iso[0]->Add(h_OS_0iso[i]);
      h_OS_1iso[0]->Add(h_OS_1iso[i]);
      h_OS_2iso[0]->Add(h_OS_2iso[i]);
      h_SS_0iso[0]->Add(h_SS_0iso[i]);
      h_SS_1iso[0]->Add(h_SS_1iso[i]);
      h_SS_2iso[0]->Add(h_SS_2iso[i]);
   }

   // add Wjets together
   for (int i=WFirst+1; i<=WLast; i++) {
      h_OS_0iso[WFirst]->Add(h_OS_0iso[i]);
      h_OS_1iso[WFirst]->Add(h_OS_1iso[i]);
      h_OS_2iso[WFirst]->Add(h_OS_2iso[i]);
      h_SS_0iso[WFirst]->Add(h_SS_0iso[i]);
      h_SS_1iso[WFirst]->Add(h_SS_1iso[i]);
      h_SS_2iso[WFirst]->Add(h_SS_2iso[i]);
   }

   // normalise QCD to SS 0iso data
   double norm = h_SS_0iso[Data1]->Integral() / h_SS_0iso[ALL]->Integral();
   h_OS_0iso[ALL]->Scale(norm);
   h_OS_1iso[ALL]->Scale(norm);
   h_OS_2iso[ALL]->Scale(norm);
   h_SS_0iso[ALL]->Scale(norm);
   h_SS_1iso[ALL]->Scale(norm);
   h_SS_2iso[ALL]->Scale(norm);

   // start with a few control plots
   TCanvas c1;

   DrawCanvas("prefit_OS_0iso_" + thevart, h_OS_0iso[Data1], h_OS_0iso[0], h_OS_0iso[WFirst], h_OS_0iso[ALL], xaxistitle(thevar));
   DrawCanvas("prefit_OS_1iso_" + thevart, h_OS_1iso[Data1], h_OS_1iso[0], h_OS_1iso[WFirst], h_OS_1iso[ALL], xaxistitle(thevar));
   DrawCanvas("prefit_OS_2iso_" + thevart, h_OS_2iso[Data1], h_OS_2iso[0], h_OS_2iso[WFirst], h_OS_2iso[ALL], xaxistitle(thevar));
   DrawCanvas("prefit_SS_0iso_" + thevart, h_SS_0iso[Data1], h_SS_0iso[0], h_SS_0iso[WFirst], h_SS_0iso[ALL], xaxistitle(thevar));
   DrawCanvas("prefit_SS_1iso_" + thevart, h_SS_1iso[Data1], h_SS_1iso[0], h_SS_1iso[WFirst], h_SS_1iso[ALL], xaxistitle(thevar));
   DrawCanvas("prefit_SS_2iso_" + thevart, h_SS_2iso[Data1], h_SS_2iso[0], h_SS_2iso[WFirst], h_SS_2iso[ALL], xaxistitle(thevar));

   // write histos to a file to play with them
   TFile *fout = TFile::Open("BkgEst/abcd/histos_abcd.root","RECREATE");
   h_OS_0iso[Data1]->Write("h_OS_0iso_Data");
   h_OS_0iso[0]->Write("h_OS_0iso_EW");
   h_OS_0iso[WFirst]->Write("h_OS_0iso_WJets");
   h_OS_0iso[ALL]->Write("h_OS_0iso_QCDMC");
   h_OS_1iso[Data1]->Write("h_OS_1iso_Data");
   h_OS_1iso[0]->Write("h_OS_1iso_EW");
   h_OS_1iso[WFirst]->Write("h_OS_1iso_WJets");
   h_OS_1iso[ALL]->Write("h_OS_1iso_QCDMC");
   h_OS_2iso[Data1]->Write("h_OS_2iso_Data");
   h_OS_2iso[0]->Write("h_OS_2iso_EW");
   h_OS_2iso[WFirst]->Write("h_OS_2iso_WJets");
   h_OS_2iso[ALL]->Write("h_OS_2iso_QCDMC");
   h_SS_0iso[Data1]->Write("h_SS_0iso_Data");
   h_SS_0iso[0]->Write("h_SS_0iso_EW");
   h_SS_0iso[WFirst]->Write("h_SS_0iso_WJets");
   h_SS_0iso[ALL]->Write("h_SS_0iso_QCDMC");
   h_SS_1iso[Data1]->Write("h_SS_1iso_Data");
   h_SS_1iso[0]->Write("h_SS_1iso_EW");
   h_SS_1iso[WFirst]->Write("h_SS_1iso_WJets");
   h_SS_1iso[ALL]->Write("h_SS_1iso_QCDMC");
   h_SS_2iso[Data1]->Write("h_SS_2iso_Data");
   h_SS_2iso[0]->Write("h_SS_2iso_EW");
   h_SS_2iso[WFirst]->Write("h_SS_2iso_WJets");
   h_SS_2iso[ALL]->Write("h_SS_2iso_QCDMC");

   // use 0iso and 1iso to determine SS scaling factors (assuming Wjets is OK -- it's sub-dominant anyway), then use this to go to 2iso
   ///// 0iso
   // start by subtracting Wjets and EW from SS data
   h_SS_0iso[Data1]->Add(h_SS_0iso[0],-1);
   h_SS_0iso[Data1]->Add(h_SS_0iso[WFirst],-1);

   // compute initial guesses for the fractions
   double int0_SS_0iso = h_SS_0iso[Data1]->Integral();
   double int0_EW_0iso = h_OS_0iso[0]->Integral();
   double int0_Wj_0iso = h_OS_0iso[WFirst]->Integral();
   double fracSS_0iso = int0_SS_0iso/h_OS_0iso[Data1]->Integral();
   double fracEW_0iso = int0_EW_0iso/h_OS_0iso[Data1]->Integral();
   double fracWj_0iso = int0_Wj_0iso/h_OS_0iso[Data1]->Integral();

   // scale SS data template so that the sum is equal to the data integral
   double iOS, iSS, eOS, eSS;
   iOS = h_OS_0iso[Data1]->IntegralAndError(1,nbinsvar(thevar),eOS);
   iSS = h_SS_0iso[Data1]->IntegralAndError(1,nbinsvar(thevar),eSS);
   h_SS_0iso[Data1]->Scale((1.-fracEW_0iso-fracWj_0iso) * h_OS_0iso[Data1]->Integral() / h_SS_0iso[Data1]->Integral());
   double int1_SS_0iso = h_SS_0iso[Data1]->Integral();
   double fact_0iso = int1_SS_0iso / int0_SS_0iso;
   double fact_err_0iso = fact_0iso * sqrt(pow(eOS/iOS,2) + pow(eSS/iSS,2));
   cout << "SS scaling factor for 0iso: " << fact_0iso << " +/- " << fact_err_0iso << endl;
   DrawCanvas("postfit_OS_0iso_" + thevart, h_OS_0iso[Data1], h_OS_0iso[0], h_OS_0iso[WFirst], h_SS_0iso[Data1], xaxistitle(thevar));

   ///// 1iso
   // start by subtracting Wjets and EW from SS data
   h_SS_1iso[Data1]->Add(h_SS_1iso[0],-1);
   h_SS_1iso[Data1]->Add(h_SS_1iso[WFirst],-1);

   // compute initial guesses for the fractions
   double int0_SS_1iso = h_SS_1iso[Data1]->Integral();
   double int0_EW_1iso = h_OS_1iso[0]->Integral();
   double int0_Wj_1iso = h_OS_1iso[WFirst]->Integral();
   double fracSS_1iso = int0_SS_1iso/h_OS_1iso[Data1]->Integral();
   double fracEW_1iso = int0_EW_1iso/h_OS_1iso[Data1]->Integral();
   double fracWj_1iso = int0_Wj_1iso/h_OS_1iso[Data1]->Integral();

   // scale SS data template so that the sum is equal to the data integral
   iOS = h_OS_1iso[Data1]->IntegralAndError(1,nbinsvar(thevar),eOS);
   iSS = h_SS_1iso[Data1]->IntegralAndError(1,nbinsvar(thevar),eSS);
   h_SS_1iso[Data1]->Scale((1.-fracEW_1iso-fracWj_1iso) * h_OS_1iso[Data1]->Integral() / h_SS_1iso[Data1]->Integral());
   double int1_SS_1iso = h_SS_1iso[Data1]->Integral();
   double fact_1iso = int1_SS_1iso / int0_SS_1iso;
   double fact_err_1iso = fact_1iso * sqrt(pow(eOS/iOS,2) + pow(eSS/iSS,2));
   cout << "SS scaling factor for 1iso: " << fact_1iso << " +/- " << fact_err_1iso << endl;
   DrawCanvas("postfit_OS_1iso_" + thevart, h_OS_1iso[Data1], h_OS_1iso[0], h_OS_1iso[WFirst], h_SS_1iso[Data1], xaxistitle(thevar));

   // average 0iso and 1iso
   // 2 versions: simple weighted average, or extrapolation to 2iso for possible dependence
   double xx[2] = {0,1};
   double ex[2] = {0,0};
   double yy[2] = {fact_0iso,fact_1iso};
   double ey[2] = {fact_err_0iso,fact_err_1iso};
   TGraphErrors tg(2,xx,yy,ex,ey);

   TFitResultPtr resa = tg.Fit("pol0","MENS EX0");
   double fact_a = resa->Value(0);
   double fact_err_a = resa->ParError(0);

   TFitResultPtr resb = tg.Fit("pol1","MENS EX0");
   const double xxb[1] = {2};
   double yyb[1] = {0};
   resb->GetConfidenceIntervals(1,1,1,xxb,yyb,0.683);
   double fact_b = resb->Value(0) + 2.*resb->Value(1);
   double fact_err_b = yyb[0];

   cout << "combined factor from averaging: " << fact_a << " +/- " << fact_err_a << endl;
   cout << "combined factor from extrapolation to 2iso: " << fact_b << " +/- " << fact_err_b << endl;

   // plot option a
   TH1D *h_SS_2iso_a = (TH1D*) h_SS_2iso[Data1]->Clone();
   h_SS_2iso_a->Add(h_SS_2iso[0],-1);
   h_SS_2iso_a->Add(h_SS_2iso[WFirst],-1);
   h_SS_2iso_a->Scale(fact_a);
   DrawCanvas("postfit_avg_OS_2iso_" + thevart, h_OS_2iso[Data1], h_OS_2iso[0], h_OS_2iso[WFirst], h_SS_2iso_a, xaxistitle(thevar));

   // plot option b
   TH1D *h_SS_2iso_b = (TH1D*) h_SS_2iso[Data1]->Clone();
   h_SS_2iso_b->Add(h_SS_2iso[0],-1);
   h_SS_2iso_b->Add(h_SS_2iso[WFirst],-1);
   h_SS_2iso_b->Scale(fact_b);
   DrawCanvas("postfit_extrapol_OS_2iso_" + thevart, h_OS_2iso[Data1], h_OS_2iso[0], h_OS_2iso[WFirst], h_SS_2iso_b, xaxistitle(thevar));

   fout->Close();
}

void DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_EW, TH1D* h_Wj, TH1D *h_QCD, TString XTitle) {
   THStack *hs = new THStack("hs_"+Variable, "");
   TH1D *h_pred = NULL;

   h_pred = (TH1D*) h_EW->Clone();
   h_EW->SetLineColor(kRed);
   h_EW->SetFillColor(kRed);
   hs->Add(h_EW);
   h_Wj->SetLineColor(kBlue);
   h_Wj->SetFillColor(kBlue);
   hs->Add(h_Wj);
   h_pred->Add(h_Wj);
   h_QCD->SetLineColor(kViolet);
   h_QCD->SetFillColor(kViolet);
   hs->Add(h_QCD);
   h_pred->Add(h_QCD);

   Double_t xlow = 0.75, ylow = 0.70, xhigh = 0.99, yhigh = 0.94;
   TLegend *legend = new TLegend(xlow, ylow, xhigh, yhigh);
   legend->SetFillStyle(0);
   legend->SetBorderSize(0);
   legend->AddEntry(h_data, "Data", "lp");
   legend->AddEntry(h_QCD, "QCD", "f");
   legend->AddEntry(h_Wj, "W+jets (MC)", "f");
   legend->AddEntry(h_EW, "TW, TT, VV, DY", "f");

   DrawCanvas(Variable, h_data, h_pred, hs, legend, XTitle);
}

void DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_pred, THStack *hs, TLegend *legend, TString XTitle) {
   TCanvas *c = new TCanvas( "c_"+Variable, "", 800, 600 );

   c->cd(1);
   ///////////////////
   // -- Top Pad -- //
   ///////////////////
   TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
   c1_1->Draw();
   c1_1->cd();
   c1_1->UseCurrentStyle();
   c1_1->SetRightMargin(0.045);
   c1_1->SetBottomMargin(0.3);
   // c1_1->SetLogy();

   h_data->GetXaxis()->SetLabelSize(0.000);
   h_data->GetYaxis()->SetTitle("Entry");
   // -- I don't know why TDR style is not applied to Y-axis Title size & Offiset
   h_data->GetYaxis()->SetTitleSize(0.06);
   h_data->GetYaxis()->SetTitleOffset(1.25);
   h_data->GetYaxis()->SetLabelSize(0.05);

   // -- Adjust Y-axis range automatically by calculating maximum value of the histogram -- //
   Double_t MaxValue_Data = h_data->GetBinContent(h_data->GetMaximumBin());
   Double_t MaxValue_totMC = h_pred->GetBinContent(h_pred->GetMaximumBin());
   Double_t MaxValue = 0;
   if( MaxValue_Data > MaxValue_totMC ) MaxValue = MaxValue_Data;
   else MaxValue = MaxValue_totMC;

   Double_t UpperEdge = MaxValue * 1.3;
   h_data->GetYaxis()->SetRangeUser(0.05, UpperEdge);

   // -- Set the axis range by hand -- //
   // this->SetAxisRange( Variable, h_data );

   h_data->SetTitle("");

   TH1D* h_format = (TH1D*)h_data->Clone();
   h_format->Reset("ICES");
   h_format->Draw("");
   hs->Draw("histsame");
   h_data->Draw("EPsame");
   h_format->Draw("axissame");
   legend->Draw();

   // UF / OF
   TPaveText t1(0.15,0.975,0.3,1,"NDC"); 
   t1.SetFillColor(0); 
   t1.SetBorderSize(0); 
   t1.SetTextSize(0.02);
   t1.SetTextAlign(12);
   t1.AddText(Form("Data: UF=%.0f,OF=%.0f",h_data->GetBinContent(0),h_data->GetBinContent(h_data->GetNbinsX()+1)));
   t1.Draw();
   TPaveText t2(0.15,0.95,0.3,0.975,"NDC"); 
   t2.SetFillColor(0); 
   t2.SetBorderSize(0); 
   t2.SetTextSize(0.02);
   t2.SetTextAlign(12);
   t2.AddText(Form("MC: UF=%.1f,OF=%.1f",h_pred->GetBinContent(0),h_pred->GetBinContent(h_pred->GetNbinsX()+1)));
   t2.Draw();

   // Run some goodness-of-fit tests
   TPaveText t3(0.3,0.95,0.78,1,"NDC"); 
   t3.SetFillColor(0); 
   t3.SetBorderSize(0); 
   t3.SetTextSize(0.05);
   t3.SetTextAlign(32);
   TString opt = "UW";
   t3.AddText(Form("#chi^{2}/ndf=%.2f (%.1f%s)",
            h_data->Chi2Test(h_pred,opt + " CHI2/NDF"),
            100.*h_data->Chi2Test(h_pred,opt),
            "%"
            )); 
   t3.Draw();

   TPaveText t4(0.8,0.95,0.95,1,"NDC"); 
   t4.SetFillColor(0); 
   t4.SetBorderSize(0); 
   t4.SetTextSize(0.05);
   t4.SetTextAlign(32);
   t4.AddText(Form("KS %.1f%s", 100.*h_data->KolmogorovTest(h_pred),"%")); 
   t4.Draw();


   //////////////////////
   // -- Bottom Pad -- //
   //////////////////////
   TPad *c1_2 = new TPad("padc1_2","padc1_2",0.01,0.01,0.99,0.3);
   c1_2->Draw();
   c1_2->cd();

   c1_2->SetBottomMargin(0.4);
   c1_2->SetRightMargin(0.035);
   c1_2->SetLeftMargin(0.15);   

   c1_2->SetFillStyle(0);
   c1_2->SetGrid();

   //Make ratio plot
   TH1D *h_ratio = (TH1D*)h_data->Clone();
   h_data->Sumw2(); //h_pred->Sumw2();
   h_ratio->Divide(h_data, h_pred);
   h_ratio->SetTitle("");
   h_ratio->GetXaxis()->SetMoreLogLabels();
   h_ratio->GetXaxis()->SetNoExponent();
   h_ratio->GetXaxis()->SetTitle( XTitle );
   h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
   h_ratio->GetXaxis()->SetTitleSize( 0.2 );
   h_ratio->GetXaxis()->SetLabelColor(1);
   h_ratio->GetXaxis()->SetLabelFont(42);
   h_ratio->GetXaxis()->SetLabelOffset(0.007);
   h_ratio->GetXaxis()->SetLabelSize(0.15);

   h_ratio->GetYaxis()->SetTitle( "data/Pred." );
   h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
   h_ratio->GetYaxis()->SetTitleSize( 0.17);


   h_ratio->SetMarkerStyle(20);
   h_ratio->SetMarkerSize(0.7);
   h_ratio->SetMarkerColor(kBlack);

   Double_t ratio_max = 1.5;
   Double_t ratio_min = 0.5;
   Double_t LabelSize = 0.11;
   // this->SetRatioRangeLabelSize( Variable, ratio_min, ratio_max, LabelSize );

   h_ratio->SetMaximum( ratio_max );
   h_ratio->SetMinimum( ratio_min );
   h_ratio->GetYaxis()->SetLabelSize( LabelSize );

   h_ratio->Draw("e1p");

   // -- flat line = 1.00 -- //
   TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
   f_line->SetLineColor(kRed);
   f_line->SetLineWidth(1);
   f_line->Draw("SAME");     

   if( Variable.Contains("Pt")  ) { c1_1->SetLogx(); c1_2->SetLogx(); }
   if( Variable.Contains("Phistar")  ) { c1_1->SetLogx(); c1_2->SetLogx(); }
   // if( Variable == "OSMass" || Variable == "OSMass_DYBin" ) { c1_1->SetLogx(); c1_2->SetLogx(); }
   if( Variable.Contains("Mass") ) { c1_1->SetLogx(); c1_2->SetLogx(); }

   // f_output->cd();
   // c->Write();
   c->SaveAs("BkgEst/abcd/pdf/c_"+Variable+"_lin.pdf");
   c->SaveAs("BkgEst/abcd/root/c_"+Variable+"_lin.root");

   c1_1->SetLogy();
   c1_1->Update();

   c->SaveAs("BkgEst/abcd/pdf/c_"+Variable+"_log.pdf");
   c->SaveAs("BkgEst/abcd/root/c_"+Variable+"_log.root");
}
