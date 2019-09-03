void compareFR();

void compareFR(TString etaregion) { // "", "_barrel", "_endcap"
   TFile *f1mu = TFile::Open("FRhistos_1mu.root");
   TFile *f2muSS = TFile::Open("FRhistos_2muSS.root");
   TFile *f2muSS_lochi2 = TFile::Open("FRhistos_2muSS_lochi2.root");
   TFile *f2muSS_hichi2 = TFile::Open("FRhistos_2muSS_hichi2.root");
   TFile *ftemplate = TFile::Open("/afs/cern.ch/work/h/hckim/public/ForEmilien_20190506_v2/input/estimateFRinput/fakerate_reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_beforeCor.root");

   TH1D *h1mu = (TH1D*) f1mu->Get("hFR" + etaregion);
   TH1D *h2muSS = (TH1D*) f2muSS->Get("hFR" + etaregion);
   TH1D *h2muSS_lochi2 = (TH1D*) f2muSS_lochi2->Get("hFR" + etaregion);
   TH1D *h2muSS_hichi2 = (TH1D*) f2muSS_hichi2->Get("hFR" + etaregion);
   TH1D *hQCD1mu = (TH1D*) f1mu->Get("hFR_QCD" + etaregion);
   TH1D *hQCD2muSS = (TH1D*) f2muSS->Get("hFR_QCD" + etaregion);
   TH1D *hQCD2muSS_lochi2 = (TH1D*) f2muSS_lochi2->Get("hFR_QCD" + etaregion);
   TH1D *hQCD2muSS_hichi2 = (TH1D*) f2muSS_hichi2->Get("hFR_QCD" + etaregion);

   TH1D *htemplate = (TH1D*) ftemplate->Get("FR_template" + etaregion);

   // set styles
   if (htemplate) {
      htemplate->SetMarkerSize(1);
      htemplate->SetMarkerStyle(20);
   }

   h1mu->SetMarkerStyle(21);
   h1mu->SetMarkerColor(kRed);
   h1mu->SetLineColor(kRed);
   h2muSS->SetMarkerStyle(33);
   h2muSS->SetMarkerColor(kBlue);
   h2muSS->SetLineColor(kBlue);
   h2muSS_lochi2->SetMarkerStyle(22);
   h2muSS_lochi2->SetMarkerColor(kBlue+2);
   h2muSS_lochi2->SetLineColor(kBlue+2);
   h2muSS_hichi2->SetMarkerStyle(23);
   h2muSS_hichi2->SetMarkerColor(kBlue-7);
   h2muSS_hichi2->SetLineColor(kBlue-7);

   hQCD1mu->SetMarkerStyle(24);
   hQCD1mu->SetMarkerColor(kMagenta);
   hQCD1mu->SetLineColor(kMagenta);
   hQCD2muSS->SetMarkerStyle(27);
   hQCD2muSS->SetMarkerColor(kCyan);
   hQCD2muSS->SetLineColor(kCyan);
   hQCD2muSS_lochi2->SetMarkerStyle(26);
   hQCD2muSS_lochi2->SetMarkerColor(kCyan+2);
   hQCD2muSS_lochi2->SetLineColor(kCyan+2);
   hQCD2muSS_hichi2->SetMarkerStyle(32);
   hQCD2muSS_hichi2->SetMarkerColor(kCyan-7);
   hQCD2muSS_hichi2->SetLineColor(kCyan-7);

   TCanvas c1;
   c1.SetLogx();
   h1mu->Draw();
   h1mu->GetYaxis()->SetRangeUser(0,1);
   c1.Update();

   hQCD1mu->Draw("same");
   h2muSS->Draw("same");
   hQCD2muSS->Draw("same");
   if (htemplate) htemplate->Draw("same");

   TLegend *tleg = new TLegend(0.2,0.65,0.5,0.95);
   tleg->SetBorderSize(0);
   if (etaregion=="_barrel") tleg->SetHeader("|#eta|<1.2");
   else if (etaregion=="_endcap") tleg->SetHeader("|#eta|>1.2");
   else tleg->SetHeader("|#eta|<2.4");
   tleg->AddEntry(htemplate, "Data, template", "lp");
   tleg->AddEntry(h1mu, "Data-MC, 1#mu", "lp");
   tleg->AddEntry(h2muSS, "Data-MC, 2#mu SS", "lp");
   tleg->AddEntry(hQCD1mu, "QCD MC, 1#mu", "lp");
   tleg->AddEntry(hQCD2muSS, "QCD MC, 2#mu SS", "lp");
   tleg->Draw();
   c1.SaveAs("compareFR" + etaregion + ".pdf");

   TCanvas c2;
   c2.SetLogx();
   h2muSS_lochi2->Draw();
   h2muSS_lochi2->GetYaxis()->SetRangeUser(0,1);
   c2.Update();

   h2muSS_hichi2->Draw("same");
   hQCD2muSS_lochi2->Draw("same");
   hQCD2muSS_hichi2->Draw("same");

   TLegend *tleg2 = new TLegend(0.2,0.65,0.5,0.95);
   tleg2->SetBorderSize(0);
   if (etaregion=="_barrel") tleg2->SetHeader("|#eta|<1.2");
   else if (etaregion=="_endcap") tleg2->SetHeader("|#eta|>1.2");
   else tleg2->SetHeader("|#eta|<2.4");
   tleg2->AddEntry(h2muSS_lochi2, "Data-MC, 2#mu SS, low dimuon #chi^{2}", "lp");
   tleg2->AddEntry(h2muSS_hichi2, "Data-MC, 2#mu SS, high dimuon #chi^{2}", "lp");
   tleg2->AddEntry(hQCD2muSS_lochi2, "QCD MC, 2#mu SS, low dimuon #chi^{2}", "lp");
   tleg2->AddEntry(hQCD2muSS_hichi2, "QCD MC, 2#mu SS, high dimuon #chi^{2}", "lp");
   tleg2->Draw();
   c2.SaveAs("compareFR_2muSS" + etaregion + ".pdf");
}

void compareFR() {
   compareFR("");
   compareFR("_barrel");
   compareFR("_endcap");
}