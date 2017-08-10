#include "../../interface/defs.h"

using namespace RooFit;
using namespace DYana;

void fit_wjets()
{
	Double_t Lumi = lumi_all;

   //Get ROOT Files
   TFile* f[NSamples+2];
   for (int i=0; i<ALL; i++) f[i] = new TFile(PathFRHistos2(static_cast<SampleTag>(i)));
   f[QCD] = new TFile(PathFRHistos2(QCD));

	//Get Histograms
   TH1D* wjets_fit[NSamples+2]; // just for draw MC histograms
   TH1D* dijet_fit[NSamples+2];
   TH1D* wjetsSS_fit[NSamples+2];
   TH1D* dijetSS_fit[NSamples+2];
   double norm[NSamples+2];
   for (int i=0; i<ALL; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      norm[i] = (Xsec(tag)*lumi_all)/Nevts(tag);
      cout<< "norm[" << i << "] = " << norm[i]<<endl;

      wjets_fit[i] = (TH1D*)f[i]->Get("fitWJets2");
      wjets_fit[i]->Scale(norm[i]);
      dijet_fit[i] = (TH1D*)f[i]->Get("fitDijet2");
      dijet_fit[i]->Scale(norm[i]);
      wjetsSS_fit[i] = (TH1D*)f[i]->Get("fitSameWJets2");
      wjetsSS_fit[i]->Scale(norm[i]);
      dijetSS_fit[i] = (TH1D*)f[i]->Get("fitSameDijet2");
      dijetSS_fit[i]->Scale(norm[i]);

       // add histos together
       bool toadd = false;
       SampleTag tagtoadd;
       if (IsDY(tag)) {
          if (tag != DYFirst) {
             toadd = true;
             tagtoadd = DYFirst;
          }
       }
       if (IsData(tag)) {
          if (tag != DataFirst) {
             toadd = true;
             tagtoadd = DataFirst;
          }
       }
       if (IsDiboson(tag)) {
          if (tag != VVFirst) {
             toadd = true;
             tagtoadd = VVFirst;
          }
       }
       if (IsWjets(tag)) {
          if (tag != WFirst) {
             toadd = true;
             tagtoadd = WFirst;
          }
       }

       if (toadd) {
          dijet_fit[tagtoadd]->Add(dijet_fit[i]);
          dijetSS_fit[tagtoadd]->Add(dijetSS_fit[i]);
          wjets_fit[tagtoadd]->Add(wjets_fit[i]);
          wjetsSS_fit[tagtoadd]->Add(wjetsSS_fit[i]);
       }
   } // sample loop

	TH1D *h_ttbar = wjets_fit[TT];

	TH1D *h_DYJets = wjets_fit[DYFirst];

	TH1D *h_WJets = wjetsSS_fit[DataFirst];
	TH1D *h_QCD_SS = dijetSS_fit[DataFirst];
	TH1D *h_ttbar_SS = wjetsSS_fit[TT];
	h_WJets->Add(h_QCD_SS,-2.0); // why -2 ???
	h_WJets->Add(h_ttbar_SS,-1.0);

	TH1D *h_QCD = dijet_fit[Data1];
	TH1D *h_DYJets_Dijet = dijet_fit[DYFirst];
	TH1D *h_ttbar_Dijet = dijet_fit[TT];

	h_QCD->Add(h_DYJets_Dijet,-1.0);
	h_QCD->Add(h_ttbar_Dijet,-1.0);

	TH1D *h_data = wjets_fit[DataFirst];

	//Convert TH1D to RooDataHist
	RooRealVar mass("mass", "Dimuon mass [GeV]", 15,200);

	RooDataHist *RooHist_ttbar = new RooDataHist("RooHist_ttbar", "RooHistogram_ttbar", mass, h_ttbar);
	RooDataHist *RooHist_DYJets = new RooDataHist("RooHist_DYJets", "RooHistogram_DYJets", mass, h_DYJets);
	RooDataHist *RooHist_WJets = new RooDataHist("RooHist_WJets", "RooHistogram_WJets", mass, h_WJets);
	RooDataHist *RooHist_QCD = new RooDataHist("RooHist_QCD", "RooHistogram_QCD", mass, h_QCD);
	RooDataHist *RooHist_data = new RooDataHist("RooHist_data", "RooHistogram_data", mass, h_data);

	//Convert RooDataHist to RooHistPdf
	RooHistPdf *pdf_ttbar = new RooHistPdf("pdf_ttbar", "Template from ttbar MC", mass, *RooHist_ttbar, 0);
	RooHistPdf *pdf_DYJets = new RooHistPdf("pdf_DYJets", "Template from DYJets MC", mass, *RooHist_DYJets, 0);
	RooHistPdf *pdf_WJets = new RooHistPdf("pdf_WJets", "Template from same-sign WJets", mass, *RooHist_WJets, 0);
	RooHistPdf *pdf_QCD = new RooHistPdf("pdf_QCD", "Template from data-driven dijet", mass, *RooHist_QCD, 0);

	// Construct model = n_ttbar * ttbar + n_WJets * WJets
	double N_ttbar = h_ttbar->Integral();
	double N_DYJets = h_DYJets->Integral();
	double N_WJets = h_WJets->Integral()*3; // Why???
	double N_QCD = h_QCD->Integral()*2; // Why???
	double ratio = h_data->Integral()/(N_ttbar + N_DYJets + N_WJets + N_QCD);
	N_ttbar *= ratio;
	N_DYJets *= ratio;
	N_WJets *= ratio;
	N_QCD *= ratio;
   cout << "ratio " << ratio << endl;

	RooRealVar n_ttbar("n_ttbar", "n_ttbar", N_ttbar, N_ttbar*0.5, N_ttbar*1.5);
	RooRealVar n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.5, N_DYJets*1.5);
	RooRealVar n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*100);
	RooRealVar n_QCD("n_QCD", "n_QCD", N_QCD, N_QCD*0.01, N_QCD*100);
	RooAddPdf model( "model","model",RooArgList(*pdf_WJets, *pdf_QCD, *pdf_DYJets, *pdf_ttbar), RooArgList(n_WJets, n_QCD, n_DYJets, n_ttbar) );

	// Fit to data
	RooFitResult* r = model.fitTo( *RooHist_data, Save() );
	r->Print();
   // RooAbsReal *chi2 = model.createChi2(*RooHist_data);
   // cout << "chi2ndof: " << chi2->getVal() / ((Double_t)h_data->GetNbinsX()) << endl;

   RooPlot *frame = mass.frame(Name("massframe"));
   RooHist_data->plotOn(frame);
   pdf_ttbar->plotOn(frame,LineColor(kRed));
   pdf_DYJets->plotOn(frame,LineColor(kGreen));
   pdf_WJets->plotOn(frame,LineColor(kBlue));
   pdf_QCD->plotOn(frame,LineColor(kMagenta));
   model.plotOn(frame,LineColor(kGray));
   frame->Draw();
}
