#include "../BkgEst/interface/defs.h"

const double logchi2min = -99;
const double logchi2max = log(20)/log(10);

double integrate(TH1 *hist, double xmin=logchi2min, double xmax=logchi2max);

void makeFRfile(TString histofile, TString fitresfile1234, TString outputfile, DYana::var thevar) { // give fitresfile with 1234 instead of 1314, 710 etc
   TFile *tfhist = TFile::Open(histofile);
   TFile *tfres1415 = TFile::Open(fitresfile1234.ReplaceAll("1234","1415"));
   TFile *tfres313= TFile::Open(fitresfile1234.ReplaceAll("1234","313"));
   TFile *tfres710= TFile::Open(fitresfile1234.ReplaceAll("1234","710"));
   TFile *tfout = TFile::Open(outputfile,"RECREATE");

   TString varname = DYana::varname(thevar);

   int nbins = nbinsvar(thevar);
   double* xbins = binsvar(thevar);

   tfout->cd();
   TH1D *hnom = new TH1D("QCDfakes", "QCD fakes (nominal);" + TString(xaxistitle(thevar)) + ";Entries", nbins, xbins);
   TH1D *hnom_stat = new TH1D("QCDfakes_stat", "QCD fakes (stat);" + TString(xaxistitle(thevar)) + ";Entries", nbins, xbins);
   TH1D *hnom_syst = new TH1D("QCDfakes_syst", "QCD fakes (syst);" + TString(xaxistitle(thevar)) + ";Entries", nbins, xbins);
   TH1D *h_1415 = new TH1D("QCDfakes_1415", "QCD fakes (1415);" + TString(xaxistitle(thevar)) + ";Entries", nbins, xbins);
   TH1D *h_313 = new TH1D("QCDfakes_313", "QCD fakes (313);" + TString(xaxistitle(thevar)) + ";Entries", nbins, xbins);
   TH1D *h_710 = new TH1D("QCDfakes_710", "QCD fakes (710);" + TString(xaxistitle(thevar)) + ";Entries", nbins, xbins);

   TH1D *hn1415 = (TH1D*) tfres1415->Get(varname + "/hdataSS");
   TH1D *hfrac1415 = (TH1D*) tfres1415->Get(varname + "/hfracSS1");
   TH1D *hn313 = (TH1D*) tfres313->Get(varname + "/hdataSS");
   TH1D *hfrac313 = (TH1D*) tfres313->Get(varname + "/hfracSS1");
   TH1D *hn710 = (TH1D*) tfres710->Get(varname + "/hdataSS");
   TH1D *hfrac710 = (TH1D*) tfres710->Get(varname + "/hfracSS1");


   // first fill the histos for 1415, 313, 710
   for (int ibin=1; ibin<=nbins; ibin++) {
     TH1D *h14 = (TH1D*) tfhist->Get(varname + "/" + Form("%.3f_%.3f",xbins[ibin-1],xbins[ibin]) + "/" + "DataSS14");
     TH1D *h15 = (TH1D*) tfhist->Get(varname + "/" + Form("%.3f_%.3f",xbins[ibin-1],xbins[ibin]) + "/" + "DataSS15");
     TH1D *h3 = (TH1D*) tfhist->Get(varname + "/" + Form("%.3f_%.3f",xbins[ibin-1],xbins[ibin]) + "/" + "DataSS3");
     TH1D *h13 = (TH1D*) tfhist->Get(varname + "/" + Form("%.3f_%.3f",xbins[ibin-1],xbins[ibin]) + "/" + "DataSS13");
     TH1D *h7 = (TH1D*) tfhist->Get(varname + "/" + Form("%.3f_%.3f",xbins[ibin-1],xbins[ibin]) + "/" + "DataSS7");
     TH1D *h10 = (TH1D*) tfhist->Get(varname + "/" + Form("%.3f_%.3f",xbins[ibin-1],xbins[ibin]) + "/" + "DataSS10");

     // estimate for 1415
     double frac14 = integrate(h14) / h14->Integral();
     double frac15 = integrate(h15) / h15->Integral();
     h_1415->SetBinContent(ibin, hn1415->GetBinContent(ibin) * (hfrac1415->GetBinContent(ibin)*frac14 + (1.-hfrac1415->GetBinContent(ibin))*frac15));
     h_1415->SetBinError(ibin, h_1415->GetBinContent(ibin)*hn1415->GetBinError(ibin)/hn1415->GetBinContent(ibin));

     // estimate for 313
     double frac3 = integrate(h3) / h3->Integral();
     double frac13 = integrate(h13) / h13->Integral();
     h_313->SetBinContent(ibin, hn313->GetBinContent(ibin) * (hfrac313->GetBinContent(ibin)*frac3 + (1.-hfrac313->GetBinContent(ibin))*frac13));
     cout << "h_313->SetBinContent(" << ibin << ", " <<hn313->GetBinContent(ibin) << " * (" <<hfrac313->GetBinContent(ibin) << "*" << frac3 << " + (1.-" << hfrac313->GetBinContent(ibin) << ")*" << frac13 << "))" << endl;
     h_313->SetBinError(ibin, h_313->GetBinContent(ibin)*hn313->GetBinError(ibin)/hn313->GetBinContent(ibin));

     // estimate for 710
     double frac7 = integrate(h7) / h7->Integral();
     double frac10 = integrate(h10) / h10->Integral();
     h_710->SetBinContent(ibin, hn710->GetBinContent(ibin) * (hfrac710->GetBinContent(ibin)*frac7 + (1.-hfrac710->GetBinContent(ibin))*frac10));
     h_710->SetBinError(ibin, h_710->GetBinContent(ibin)*hn710->GetBinError(ibin)/hn710->GetBinContent(ibin));
   }
   
   // then make the nominal, stat and syst ones
   for (int ibin=1; ibin<=nbins; ibin++) {
      hnom->SetBinContent(ibin,h_1415->GetBinContent(ibin));
      hnom->SetBinError(ibin,h_1415->GetBinError(ibin));
      hnom_stat->SetBinContent(ibin,h_1415->GetBinError(ibin));
      hnom_syst->SetBinContent(ibin,max(fabs(h_313->GetBinContent(ibin)-hnom->GetBinContent(ibin)),fabs(h_710->GetBinContent(ibin)-hnom->GetBinContent(ibin))));
   }

   tfout->Write();
   tfout->Close();
}

double integrate(TH1 *hist, double xmin, double xmax) {
   int ibin1 = hist->FindBin(xmin);
   int ibin2 = hist->FindBin(xmax);
   double int0 = hist->Integral(ibin1,ibin2);

   // now we need to remove the bits that we counted outside the range
   double x1a = hist->GetBinCenter(ibin1)-hist->GetBinWidth(ibin1)/2.;
   double x2b = hist->GetBinCenter(ibin2)+hist->GetBinWidth(ibin2)/2.;
   return int0  
      - (hist->GetBinContent(ibin1) * (xmin - x1a) / hist->GetBinWidth(ibin1))
      - (hist->GetBinContent(ibin2) * (x2b - xmax) / hist->GetBinWidth(ibin2));
}
