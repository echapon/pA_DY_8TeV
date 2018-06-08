const Double_t isoval[10] = { 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1, 1e99};
const int binnum = 16;
const double bins[17] = {4,8,11,15,20,30,40,50,60,76,86,96,106,120,150,200,600};

const int colors[12] = {kBlack,kRed+1,kBlue+2,kGreen+2,kMagenta+2,kCyan+2,kYellow+2,kViolet+2,kTeal+2,kOrange+2,kSpring+2,kGray};
const int styles[12] = {20,21,22,23,24,25,26,27,28,29,30,31};

void optimise(const char* dyfile="ROOTFile_Histogram_Acc_Eff_isostudy_Powheg_PAL1DoubleMu0_0.root", 
      const char* dataOSfile="ROOTFile_Histogram_Acc_Eff_isostudy_Powheg_PAL1DoubleMu0_0_data.root", 
      const char* dataSSfile="ROOTFile_Histogram_Acc_Eff_isostudy_Powheg_PAL1DoubleMu0_0_dataSS.root") {
   TFile *fdy = TFile::Open(dyfile);
   TFile *fdOS = TFile::Open(dataOSfile);
   TFile *fdSS = TFile::Open(dataSSfile);
   TFile *fout = TFile::Open("histos.root","RECREATE");

   TH1D *hdy[12][10];
   TH1D *hdOS[12][10];
   TH1D *hdSS[12][10];
   for (int i=0; i<12; i++) {
      for (int j=0; j<10; j++) {
         hdy[i][j] = (TH1D*) fdy->Get(Form("h_mass_EffPass_%d_%d",i,j));
         hdOS[i][j] = (TH1D*) fdOS->Get(Form("h_mass_EffPass_%d_%d",i,j));
         hdSS[i][j] = (TH1D*) fdSS->Get(Form("h_mass_EffPass_%d_%d",i,j));
      }
   }

   // make the optimisation histo for all cases
   for (int imass=0; imass<binnum; imass++) { // loop on mass bins
      double maxfom = -1;
      TCanvas *c1 = new TCanvas(Form("c_%d_%f_M_%f",imass,bins[imass],bins[imass+1]), Form("%f<M<%f GeV",bins[imass],bins[imass+1]));
      TLegend *tleg = new TLegend(0.6,0.6,0.95,0.95);
      tleg->SetBorderSize(0);
      tleg->SetHeader(Form("%f<M<%f GeV",bins[imass],bins[imass+1]));

      for (int i=0; i<12; i++) { // loop on iso types
          double y[9] = {0};

          // first get the SS/OS transfer factor k
          double xdy = hdy[i][9]->GetBinContent(imass+1) - hdy[i][8]->GetBinContent(imass+1);
          double xdOS = hdOS[i][9]->GetBinContent(imass+1) - hdOS[i][8]->GetBinContent(imass+1);
          double xdSS = hdSS[i][9]->GetBinContent(imass+1) - hdSS[i][8]->GetBinContent(imass+1);
          double k = (xdOS-xdy)/xdSS;
          if (i<=6) cout << bins[imass] << "<M<" << bins[imass+1] << " (i=" << i << "): k = " << k << endl;

          // now build the graph
          for (int j=0; j<9; j++) {
             double S = hdy[i][j]->GetBinContent(imass+1);
             double B = hdSS[i][j]->GetBinContent(imass+1)*k;
             y[j] = S / sqrt(S+B);
          }

          TGraph *g = new TGraph(9,isoval,y);
          g->SetName(Form("g_%d_%d",imass,i));
          g->SetMarkerStyle(styles[i]);
          g->SetMarkerColor(colors[i]);
          g->SetLineColor(colors[i]);
          if (i==0) g->Draw("ALP");
          else g->Draw("LP");
          tleg->AddEntry(g,Form("isotype = %d", i),"LP");
          fout->cd();
          g->Write();
      }
      tleg->Draw();
      fout->cd();
      c1->Write();
   }

   fout->Close();
}
