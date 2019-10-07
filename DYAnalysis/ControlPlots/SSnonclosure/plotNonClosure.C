void plotNonClosure(TString variable) {
   TFile *f_nonclosure = TFile::Open("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/SSnonclosure/"+variable+".root");
   TH1D *h_nonclosure = (TH1D*)f_nonclosure->Get("h_nonclosure_scale");

   TCanvas c1;

   if (variable.Contains("mass")) {
      c1.SetLogx();
      h_nonclosure->GetXaxis()->SetTitle("M [GeV/c^{2}]");
   } else if (variable.Contains("phistar")) {
      c1.SetLogx();
      h_nonclosure->GetXaxis()->SetTitle("#phi^{*}");
   } else if (variable.Contains("pt")) {
      c1.SetLogx();
      h_nonclosure->GetXaxis()->SetTitle("p_{T} [GeV/c]");
   } else {
      h_nonclosure->GetXaxis()->SetTitle("y_{CM}");
   }

   h_nonclosure->GetYaxis()->SetTitle("Nonclosure");
   h_nonclosure->GetYaxis()->SetRangeUser(0,5);
   h_nonclosure->GetXaxis()->SetLabelSize(0.05);
   h_nonclosure->GetXaxis()->SetMoreLogLabels();

   h_nonclosure->Draw();

   TF1 tf("tf","1",-1e4,1e4);
   tf.SetLineStyle(3);
   tf.Draw("same");

   c1.SaveAs(variable+".pdf");
}

void plotNonClosure() {
   plotNonClosure("mass");
   plotNonClosure("mass3bins");
   plotNonClosure("rap1560");
   plotNonClosure("pt1560");
   plotNonClosure("phistar1560");
   plotNonClosure("rap60120");
   plotNonClosure("pt");
   plotNonClosure("phistar");
}
