#include "../interface/defs.h"

#include "TFile.h"
#include "TH1.h"
#include "TVirtualFitter.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFitResult.h"

const TString isotag="0.0-0.2";
const TString nonisotag="0.2-10.0";

using namespace std;
using namespace DYana;
using namespace DYana_v20180814;

void runABCD_improved2(var thevar, const char* fdata="ROOTFile_Histogram_InvMass_isos_PAL3Mu12_Data_MomCorr00_noHFrew_notnprew.root", const char* fMC="ROOTFile_Histogram_InvMass_isos_PAL3Mu12_Powheg_MomCorr00_rewboth_tnprew.root", double SSfactor=2.15824, double SSfactor_err=0.1) {
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
         h_OS_0iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_" + nonisotag + "_" + nonisotag + "_" + Name(tag));
         h_OS_1iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_" + isotag + "_" + nonisotag + "_" + Name(tag));
         h_OS_2iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_" + isotag + "_" + isotag + "_" + Name(tag));
         h_SS_0iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_SS_" + nonisotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_1iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_SS_" + isotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_2iso[i] = (TH1D*) tfdata->Get("h_" + thevart + "_SS_" + isotag + "_" + isotag + "_" + Name(tag));
      } else {
         h_OS_0iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_" + nonisotag + "_" + nonisotag + "_" + Name(tag));
         h_OS_1iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_" + isotag + "_" + nonisotag + "_" + Name(tag));
         h_OS_2iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_" + isotag + "_" + isotag + "_" + Name(tag));
         h_SS_0iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + nonisotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_1iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + isotag + "_" + nonisotag + "_" + Name(tag));
         h_SS_2iso[i] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + isotag + "_" + isotag + "_" + Name(tag));

         double lumi = lumi_all;
         if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
            lumi = switcheta(tag)>0 ? lumi_part1 : lumi_part2;
         }
         double norm = (Xsec(tag)*lumi)/Nevts(tag) ;
         cout << Name(tag) << " " << lumi << " " << norm << endl;
         h_OS_0iso[i]->Scale(norm);
         h_OS_1iso[i]->Scale(norm);
         h_OS_2iso[i]->Scale(norm);
         h_SS_0iso[i]->Scale(norm);
         h_SS_1iso[i]->Scale(norm);
         h_SS_2iso[i]->Scale(norm);
      }
   }

   h_OS_0iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_" + nonisotag + "_" + nonisotag + "_" + Name(QCD));
   h_OS_1iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_" + isotag + "_" + nonisotag + "_" + Name(QCD));
   h_OS_2iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_" + isotag + "_" + isotag + "_" + Name(QCD));
   h_SS_0iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + nonisotag + "_" + nonisotag + "_" + Name(QCD));
   h_SS_1iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + isotag + "_" + nonisotag + "_" + Name(QCD));
   h_SS_2iso[ALL] = (TH1D*) tfMC->Get("h_" + thevart + "_SS_" + isotag + "_" + isotag + "_" + Name(QCD));

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

   h_OS_0iso[Data1]->Draw();
   h_OS_0iso[0]->SetMarkerColor(kRed);
   h_OS_0iso[0]->SetLineColor(kRed);
   h_OS_0iso[0]->Draw("same");
   h_OS_0iso[WFirst]->SetMarkerColor(kBlue);
   h_OS_0iso[WFirst]->SetLineColor(kBlue);
   h_OS_0iso[WFirst]->Draw("same");
   h_OS_0iso[ALL]->SetMarkerColor(kGreen+1);
   h_OS_0iso[ALL]->SetLineColor(kGreen+1);
   h_OS_0iso[ALL]->Draw("same");
   c1.SaveAs("test_OS_0iso.pdf");
   c1.SetLogy(); c1.Update(); c1.SaveAs("test_OS_0iso_log.pdf"); c1.SetLogy(kFALSE);

   h_OS_1iso[Data1]->Draw();
   h_OS_1iso[0]->SetMarkerColor(kRed);
   h_OS_1iso[0]->SetLineColor(kRed);
   h_OS_1iso[0]->Draw("same");
   h_OS_1iso[WFirst]->SetMarkerColor(kBlue);
   h_OS_1iso[WFirst]->SetLineColor(kBlue);
   h_OS_1iso[WFirst]->Draw("same");
   h_OS_1iso[ALL]->SetMarkerColor(kGreen+1);
   h_OS_1iso[ALL]->SetLineColor(kGreen+1);
   h_OS_1iso[ALL]->Draw("same");
   c1.SaveAs("test_OS_1iso.pdf");
   c1.SetLogy(); c1.Update(); c1.SaveAs("test_OS_1iso_log.pdf"); c1.SetLogy(kFALSE);

   h_OS_2iso[Data1]->Draw();
   h_OS_2iso[0]->SetMarkerColor(kRed);
   h_OS_2iso[0]->SetLineColor(kRed);
   h_OS_2iso[0]->Draw("same");
   h_OS_2iso[WFirst]->SetMarkerColor(kBlue);
   h_OS_2iso[WFirst]->SetLineColor(kBlue);
   h_OS_2iso[WFirst]->Draw("same");
   h_OS_2iso[ALL]->SetMarkerColor(kGreen+1);
   h_OS_2iso[ALL]->SetLineColor(kGreen+1);
   h_OS_2iso[ALL]->Draw("same");
   c1.SaveAs("test_OS_2iso.pdf");
   c1.SetLogy(); c1.Update(); c1.SaveAs("test_OS_2iso_log.pdf"); c1.SetLogy(kFALSE);

   h_SS_0iso[Data1]->Draw();
   h_SS_0iso[0]->SetMarkerColor(kRed);
   h_SS_0iso[0]->SetLineColor(kRed);
   h_SS_0iso[0]->Draw("same");
   h_SS_0iso[WFirst]->SetMarkerColor(kBlue);
   h_SS_0iso[WFirst]->SetLineColor(kBlue);
   h_SS_0iso[WFirst]->Draw("same");
   h_SS_0iso[ALL]->SetMarkerColor(kGreen+1);
   h_SS_0iso[ALL]->SetLineColor(kGreen+1);
   h_SS_0iso[ALL]->Draw("same");
   c1.SaveAs("test_SS_0iso.pdf");
   c1.SetLogy(); c1.Update(); c1.SaveAs("test_SS_0iso_log.pdf"); c1.SetLogy(kFALSE);

   h_SS_1iso[Data1]->Draw();
   h_SS_1iso[0]->SetMarkerColor(kRed);
   h_SS_1iso[0]->SetLineColor(kRed);
   h_SS_1iso[0]->Draw("same");
   h_SS_1iso[WFirst]->SetMarkerColor(kBlue);
   h_SS_1iso[WFirst]->SetLineColor(kBlue);
   h_SS_1iso[WFirst]->Draw("same");
   h_SS_1iso[ALL]->SetMarkerColor(kGreen+1);
   h_SS_1iso[ALL]->SetLineColor(kGreen+1);
   h_SS_1iso[ALL]->Draw("same");
   c1.SaveAs("test_SS_1iso.pdf");
   c1.SetLogy(); c1.Update(); c1.SaveAs("test_SS_1iso_log.pdf"); c1.SetLogy(kFALSE);

   h_SS_2iso[Data1]->Draw();
   h_SS_2iso[0]->SetMarkerColor(kRed);
   h_SS_2iso[0]->SetLineColor(kRed);
   h_SS_2iso[0]->Draw("same");
   h_SS_2iso[WFirst]->SetMarkerColor(kBlue);
   h_SS_2iso[WFirst]->SetLineColor(kBlue);
   h_SS_2iso[WFirst]->Draw("same");
   h_SS_2iso[ALL]->SetMarkerColor(kGreen+1);
   h_SS_2iso[ALL]->SetLineColor(kGreen+1);
   h_SS_2iso[ALL]->Draw("same");
   c1.SaveAs("test_SS_2iso.pdf");
   c1.SetLogy(); c1.Update(); c1.SaveAs("test_SS_2iso_log.pdf"); c1.SetLogy(kFALSE);

   // write histos to a file to play with them
   TFile *fout = TFile::Open("test_abcd.root","RECREATE");
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

   fout->Close();
}
