void compare_fits(TString basename, TString var) {
   TFile *f12 = TFile::Open(basename + "_bkg12.root");
   TFile *f34 = TFile::Open(basename + "_bkg34.root");
   TFile *f56 = TFile::Open(basename + "_bkg56.root");
   TFile *f89 = TFile::Open(basename + "_bkg89.root");
   TFile *f710 = TFile::Open(basename + "_bkg710.root");
   TFile *f313 = TFile::Open(basename + "_bkg313.root");
   TFile *f1415 = TFile::Open(basename + "_bkg1415.root");

   bool do12 = false; // SS iso only, ignore
   bool do34 = true; // for L3Mu12
   bool do56 = true; // for L3Mu12
   bool do89 = false; // OS noniso only, ignore
   bool do710 = false;
   bool do313 = false;
   bool do1415 = false;

   TCanvas *c1 = new TCanvas("c1","c1",800,600);
   TCanvas *c2 = new TCanvas("c2","c2",800,600);

   if (var=="mass" || var=="pt" || var=="phistar") {
      c1->SetLogx(); c1->SetLogy();
   }

   c1->cd();
   TLegend *tleg = new TLegend(0.8,0.8,1,1);
   tleg->SetBorderSize(0);
   tleg->SetHeader(var);
   
   TH1D* h = (TH1D*) f12->Get(var + "/hndy_ini");
   h->GetXaxis()->SetTitle(var);
   h->GetYaxis()->SetTitle("N(DY)");
   h->Draw();
   h->GetYaxis()->SetRangeUser(0.5*h->GetMinimum(),1.5*h->GetMaximum());
   h->Draw();
   tleg->AddEntry(h,"Input","l");
   if (do12) {
      h = (TH1D*) f12->Get(var + "/hndy");
      h->Draw("same");
      tleg->AddEntry(h,"Fit bkg12","l");
   }
   if (do34) {
      h = (TH1D*) f34->Get(var + "/hndy");
      h->SetLineColor(kBlue);
      h->SetMarkerColor(kBlue);
      h->Draw("same");
      tleg->AddEntry(h,"Fit bkg34","l");
   }
   if (do56) {
      h = (TH1D*) f56->Get(var + "/hndy");
      h->SetLineColor(kRed);
      h->SetMarkerColor(kRed);
      h->Draw("same");
      tleg->AddEntry(h,"Fit bkg56","l");
   }
   if (do89) {
      h = (TH1D*) f89->Get(var + "/hndy");
      h->SetLineColor(kGreen);
      h->SetMarkerColor(kGreen);
      h->Draw("same");
      tleg->AddEntry(h,"Fit bkg89","l");
   }
   if (do710) {
      h = (TH1D*) f710->Get(var + "/hndy");
      h->SetLineColor(kMagenta);
      h->SetMarkerColor(kMagenta);
      h->Draw("same");
      tleg->AddEntry(h,"Fit bkg710","l");
   }
   if (do313) {
      h = (TH1D*) f313->Get(var + "/hndy");
      h->SetLineColor(kCyan);
      h->SetMarkerColor(kCyan);
      h->Draw("same");
      tleg->AddEntry(h,"Fit bkg313","l");
   }
   if (do1415) {
      h = (TH1D*) f1415->Get(var + "/hndy");
      h->SetLineColor(kOrange);
      h->SetMarkerColor(kOrange);
      h->Draw("same");
      tleg->AddEntry(h,"Fit bkg1415","l");
   }
   tleg->Draw();
   c1->SaveAs(basename + "_" + var + "_compare.pdf");

   c2->cd();
   tleg = new TLegend(0.44,0.72,0.61,0.91);
   tleg->SetBorderSize(0);
   tleg->SetHeader(var);

   TString cut;
   if (var=="mass") cut = "rapbin1<-2&&rapbin2>1&&ptbin1<0.1&&ptbin2>100";
   else if (var=="rap1560") cut = "massbin1<20&&massbin2>55";
   else if (var=="rap60120") cut = "massbin1>55&&massbin2<125&&ptbin1<1&&ptbin2>100&&phistarbin1<0.01&&phistarbin2>1";
   else if (var=="pt") cut = "massbin1>55&&massbin2<125&&rapbin1<-2&&rapbin2>1&&phistarbin1<0.01&&phistarbin2>1";
   else if (var=="phistar") cut = "massbin1>55&&massbin2<125&&rapbin1<-2&&rapbin2>1&&ptbin1<0.01&&ptbin2>100";

   TString opt = "";
   TTree *tr = NULL;
   if (do12) {
      TH1D *h12 = new TH1D("h12",";p-value (#chi^{2},ndf)",20,0,1);
      tr = (TTree*) f12->Get("fit_results");
      tr->Draw("TMath::Prob(chi2,ndf)>>+h12",cut,"hist" + opt);
      tleg->AddEntry(h12,"bkg12","l");
      cout << "bkg12: <pval> = " << h12->GetMean() << ", " << h12->GetBinContent(1) << " fits with pval<0.05" << endl;
      if (opt=="") opt = " same";
   }
   if (do34) {
      TH1D *h34 = new TH1D("h34",";p-value (#chi^{2},ndf)",20,0,1);
      h34->SetLineColor(kBlue);
      tr = (TTree*) f34->Get("fit_results");
      tr->Draw("TMath::Prob(chi2,ndf)>>+h34",cut,"hist" + opt);
      tleg->AddEntry(h34,"bkg34","l");
      cout << "bkg34: <pval> = " << h34->GetMean() << ", " << h34->GetBinContent(1) << " fits with pval<0.05" << endl;
      if (opt=="") opt = " same";
   }
   if (do56) {
      TH1D *h56 = new TH1D("h56",";p-value (#chi^{2},ndf)",20,0,1);
      h56->SetLineColor(kRed);
      tr = (TTree*) f56->Get("fit_results");
      tr->Draw("TMath::Prob(chi2,ndf)>>+h56",cut,"hist" + opt);
      tleg->AddEntry(h56,"bkg56","l");
      cout << "bkg56: <pval> = " << h56->GetMean() << ", " << h56->GetBinContent(1) << " fits with pval<0.05" << endl;
      if (opt=="") opt = " same";
   }
   if (do89) {
      TH1D *h89 = new TH1D("h89",";p-value (#chi^{2},ndf)",20,0,1);
      h89->SetLineColor(kGreen);
      tr = (TTree*) f89->Get("fit_results");
      tr->Draw("TMath::Prob(chi2,ndf)>>+h89",cut,"hist" + opt);
      tleg->AddEntry(h89,"bkg89","l");
      cout << "bkg89: <pval> = " << h89->GetMean() << ", " << h89->GetBinContent(1) << " fits with pval<0.05" << endl;
      if (opt=="") opt = " same";
   }
   if (do710) {
      TH1D *h710 = new TH1D("h710",";p-value (#chi^{2},ndf)",20,0,1);
      h710->SetLineColor(kMagenta);
      tr = (TTree*) f710->Get("fit_results");
      tr->Draw("TMath::Prob(chi2,ndf)>>+h710",cut,"hist" + opt);
      tleg->AddEntry(h710,"bkg710","l");
      cout << "bkg710: <pval> = " << h710->GetMean() << ", " << h710->GetBinContent(1) << " fits with pval<0.05" << endl;
      if (opt=="") opt = " same";
   }
   if (do313) {
      TH1D *h313 = new TH1D("h313",";p-value (#chi^{2},ndf)",20,0,1);
      h313->SetLineColor(kCyan);
      tr = (TTree*) f313->Get("fit_results");
      tr->Draw("TMath::Prob(chi2,ndf)>>+h313",cut,"hist" + opt);
      tleg->AddEntry(h313,"bkg313","l");
      cout << "bkg313: <pval> = " << h313->GetMean() << ", " << h313->GetBinContent(1) << " fits with pval<0.05" << endl;
      if (opt=="") opt = " same";
   }
   if (do1415) {
      TH1D *h1415 = new TH1D("h1415",";p-value (#chi^{2},ndf)",20,0,1);
      h1415->SetLineColor(kOrange);
      tr = (TTree*) f1415->Get("fit_results");
      tr->Draw("TMath::Prob(chi2,ndf)>>+h1415",cut,"hist" + opt);
      tleg->AddEntry(h1415,"bkg1415","l");
      cout << "bkg1415: <pval> = " << h1415->GetMean() << ", " << h1415->GetBinContent(1) << " fits with pval<0.05" << endl;
      if (opt=="") opt = " same";
   }
   tleg->Draw();
   c2->SaveAs(basename + "_" + var + "_chi2.pdf");

}

void compare_fits(TString basename) {
   compare_fits(basename,"mass");
   compare_fits(basename,"rap1560");
   compare_fits(basename,"rap60120");
   compare_fits(basename,"pt");
   compare_fits(basename,"phistar");
}
