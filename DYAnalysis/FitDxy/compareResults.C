void compareResult(TString file1, TString file2, TString tag1, TString tag2, TString varname, TString vartitle, bool logx, bool logy);

void compareResults(TString file1="output_L1DoubleMu0_extended.root", TString file2="output_L3Mu12_extended.root", TString tag1="L1DoubleMu0", TString tag2="L3Mu12") {
   compareResult(file1, file2, tag1, tag2, "mass", "mass [GeV]", true, true);
   compareResult(file1, file2, tag1, tag2, "rap1560", "rapidity (15<M<60 GeV)", false, false);
   compareResult(file1, file2, tag1, tag2, "rap60120", "rapidity (60<M<120 GeV)", false, false);
   compareResult(file1, file2, tag1, tag2, "pt", "p_{T} [GeV] (60<M<120 GeV)", true, true);
   compareResult(file1, file2, tag1, tag2, "phistar", "#phi^{*} (60<M<120 GeV)", true, true);
}

void compareResult(TString file1, TString file2, TString tag1, TString tag2, TString varname, TString vartitle, bool logx, bool logy) {
   TCanvas c1;
   TH1D *h1 = (TH1D*) TFile::Open(file1)->Get(varname + "/hndy");
   TH1D *h2 = (TH1D*) TFile::Open(file2)->Get(varname + "/hndy");
   h1->GetXaxis()->SetTitle(vartitle);
   h1->GetYaxis()->SetTitle("Fitted DYMuMu yields");
   h1->SetLineColor(kRed);
   h2->SetLineColor(kBlue);
   if (logx) c1.SetLogx();
   if (logy) c1.SetLogy();
   h1->Draw();
   h2->Draw("same");
   TLegend *tleg = new TLegend(0.3,0.13,0.6,0.3);
   tleg->SetBorderSize(0);
   tleg->AddEntry(h1,tag1,"lp");
   tleg->AddEntry(h2,tag2,"lp");
   tleg->Draw();
   c1.SaveAs("yields_fit_" + varname + ".pdf");

   for (int i=1; i<=h1->GetNbinsX(); i++) {
      double c = h1->GetBinContent(i);
      double e = h1->GetBinError(i);
      if (e>0) {
         h1->SetBinError(i,0);
         h1->SetBinContent(i,c/e);
      }
      c = h2->GetBinContent(i);
      e = h2->GetBinError(i);
      if (e>0) {
         h2->SetBinError(i,0);
         h2->SetBinContent(i,c/e);
      }
   }
   h1->GetYaxis()->SetTitle("N/#DeltaN");
   h1->Draw();
   h2->Draw("same");
   tleg->Draw();
   c1.SaveAs("signif_fit_" + varname + ".pdf");
}
