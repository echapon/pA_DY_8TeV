#include "../../interface/defs.h"

using namespace RooFit;
using namespace DYana;

const double lumi_sf = 1.;//0.92;

void fit_wjets(int iFR=1, TString vartag = "") 
// iFR = 1 (template) or 2 (data-MC)
// vartag = "" (mass) or "massMET" or "Mt"
{
	Double_t Lumi = lumi_all;

   //Get ROOT Files
   TFile* f[NSamples+2];
   for (int i=0; i<ALL; i++) f[i] = new TFile(PathFRHistos2(static_cast<SampleTag>(i)));
   f[QCD] = new TFile(PathFRHistos2(QCD));

	//Get Histograms
   TH1D* wjets_fit[NSamples+2]; 
   TH1D* dijet_fit[NSamples+2];
   TH1D* wjetsSS_fit[NSamples+2];
   TH1D* dijetSS_fit[NSamples+2];
   double norm[NSamples+2];
   for (int i=0; i<ALL; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      double lumi = lumi_all;
      if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
         lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
      }
      norm[i] = IsData(tag) ? 1. : (Xsec(tag)*lumi*lumi_sf)/Nevts(tag);
      cout<< "norm[" << i << "] = " << norm[i]<<endl;

      wjets_fit[i] = (TH1D*)f[i]->Get(Form("WJets/fit%sWJets%d",vartag.Data(),iFR));
      wjets_fit[i]->Scale(norm[i]);
      dijet_fit[i] = (TH1D*)f[i]->Get(Form("Dijet/fit%sDijet%d",vartag.Data(),iFR));
      dijet_fit[i]->Scale(norm[i]);
      wjetsSS_fit[i] = (TH1D*)f[i]->Get(Form("WJets/fit%sSameWJets%d",vartag.Data(),iFR));
      wjetsSS_fit[i]->Scale(norm[i]);
      dijetSS_fit[i] = (TH1D*)f[i]->Get(Form("Dijet/fit%sSameDijet%d",vartag.Data(),iFR));
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
       if (IsData(tag)) {
          if (tag != DataFirst) {
             toadd = true;
             tagtoadd = DataFirst;
          }
       }

       if (toadd) {
          dijet_fit[tagtoadd]->Add(dijet_fit[i]);
          dijetSS_fit[tagtoadd]->Add(dijetSS_fit[i]);
          wjets_fit[tagtoadd]->Add(wjets_fit[i]);
          wjetsSS_fit[tagtoadd]->Add(wjetsSS_fit[i]);
       }
   } // sample loop

   // ttbar template: MC, 1P1F, OS
	TH1D *h_ttbar = wjets_fit[TT];

   // DY template: MC, 1P1F, OS
   TH1D *h_DYJets = wjets_fit[DYFirst];

   // W+jets template: data-MC, 1P1F, SS
   // // TH1D *h_WJets = wjetsSS_fit[DataFirst];
   // TH1D *h_DYJets_SS = dijetSS_fit[DYFirst];
   // TH1D *h_DYJets_SS2 = wjetsSS_fit[DYFirst];
   // TH1D *h_QCD_SS = dijetSS_fit[DataFirst];
   // TH1D *h_QCD_SS2 = wjetsSS_fit[DataFirst];
   // TH1D *h_ttbar_SS = wjetsSS_fit[TT];
   // // // h_WJets->Add(h_QCD_SS,-2.0); // the -2 is probably because QCD is estimated from the 2noniso sample: either muon1 is propagated to low iso, either muon2 is
   // // h_WJets->Add(h_DYJets_SS,-1.0);
   // // h_WJets->Add(h_ttbar_SS,-1.0);

   // temporarily take the Wjets template from MC
   TH1D *h_WJets = wjets_fit[WFirst];

   // QCD template: data-MC, 0P2F, OS
   TH1D *h_QCD = dijet_fit[DataFirst];
   TH1D *h_DYJets_Dijet = dijet_fit[DYFirst];
   TH1D *h_WJets_Dijet = dijet_fit[WFirst];
   TH1D *h_ttbar_Dijet = dijet_fit[TT];
   h_QCD->Add(h_DYJets_Dijet,-1.0);
   h_QCD->Add(h_ttbar_Dijet,-1.0);

   // data template: data, 1P1F, OS
	TH1D *h_data = wjets_fit[DataFirst];

   // save all this to a file
   TFile *tf = TFile::Open("fit_wjets_inputs.root","RECREATE");
   h_ttbar->Write("h_ttbar");
   h_DYJets->Write("h_DYJets");
   h_WJets->Write("h_WJets");
   // wjetsSS_fit[WFirst]->Write("hh_WJets_SS_MC");
   // h_QCD_SS->Write("hh_QCD_SS");
   // h_QCD_SS2->Write("hh_QCD_SS2");
   // h_DYJets_SS->Write("hh_DYJets_SS");
   // h_DYJets_SS2->Write("hh_DYJets_SS2");
   // h_ttbar_SS->Write("hh_ttbar_SS");
   h_QCD->Write("h_QCD");
   h_DYJets_Dijet->Write("hh_DYJets_Dijet");
   h_WJets_Dijet->Write("hh_WJets_Dijet");
   h_ttbar_Dijet->Write("hh_ttbar_Dijet");
   h_data->Write("h_data");
   tf->Close();
   // return;

	//Convert TH1D to RooDataHist
   RooRealVar* mass = NULL;
   if (vartag=="") mass = new RooRealVar("mass", "Dimuon mass [GeV]", 15,200);
   else if (vartag=="massMET") mass = new RooRealVar("massMET", "Dimuon mass [GeV] in 2 MET bins", 0,400);
   // RooRealVar mass("vtxchi2", "dimuon chi2", 0,20);
   // RooRealVar mass("dphi", "dimuon Delta phi", 0,3.2);
   // RooRealVar mass("mt", "Mt", 0,200);

	RooDataHist *RooHist_ttbar = new RooDataHist("RooHist_ttbar", "RooHistogram_ttbar", *mass, h_ttbar);
	RooDataHist *RooHist_DYJets = new RooDataHist("RooHist_DYJets", "RooHistogram_DYJets", *mass, h_DYJets);
	RooDataHist *RooHist_WJets = new RooDataHist("RooHist_WJets", "RooHistogram_WJets", *mass, h_WJets);
	RooDataHist *RooHist_QCD = new RooDataHist("RooHist_QCD", "RooHistogram_QCD", *mass, h_QCD);
	RooDataHist *RooHist_data = new RooDataHist("RooHist_data", "RooHistogram_data", *mass, h_data);

	//Convert RooDataHist to RooHistPdf
	RooHistPdf *pdf_ttbar = new RooHistPdf("pdf_ttbar", "Template from ttbar MC", *mass, *RooHist_ttbar, 0);
	RooHistPdf *pdf_DYJets = new RooHistPdf("pdf_DYJets", "Template from DYJets MC", *mass, *RooHist_DYJets, 0);
	RooHistPdf *pdf_WJets = new RooHistPdf("pdf_WJets", "Template from same-sign WJets", *mass, *RooHist_WJets, 0);
	RooHistPdf *pdf_QCD = new RooHistPdf("pdf_QCD", "Template from data-driven dijet", *mass, *RooHist_QCD, 0);

	// Construct model = n_ttbar * ttbar + n_WJets * WJets
	double N_ttbar = h_ttbar->Integral(); 
	double N_DYJets = h_DYJets->Integral(); 
	double N_WJets = h_WJets->Integral(); 
	double N_QCD = h_QCD->Integral();

   RooRealVar sf_MC("sf_MC", "sf_MC", 0.5, 0.1, 3.);
   RooRealVar sf_QCD("sf_QCD", "sf_QCD", 1, 0.1, 10.);
   RooRealVar sf_WJets("sf_WJets", "sf_WJets", 0.5, 0.1, 10.);
   RooConstVar n_ttbar0("n_ttbar0","n_ttbar0",N_ttbar);
	RooFormulaVar n_ttbar("n_ttbar", "@0*@1", RooArgSet(sf_MC,n_ttbar0));
   RooConstVar n_DYJets0("n_DYJets0","n_DYJets0",N_DYJets);
	RooFormulaVar n_DYJets("n_DYJets", "@0*@1", RooArgSet(sf_MC,n_DYJets0));
   RooConstVar n_QCD0("n_QCD0","n_QCD0",N_QCD);
	RooFormulaVar n_QCD("n_QCD", "@0*@1", RooArgSet(sf_QCD,n_QCD0));
   RooConstVar n_WJets0("n_WJets0","n_WJets0",N_WJets);
	RooFormulaVar n_WJets("n_WJets", "@0*@1", RooArgSet(sf_WJets,n_WJets0));
	RooAddPdf model( "model","model",RooArgList(*pdf_WJets, *pdf_QCD, *pdf_DYJets, *pdf_ttbar), RooArgList(n_WJets, n_QCD, n_DYJets, n_ttbar) );

	// Fit to data
	RooFitResult* r = model.fitTo( *RooHist_data, Save(), Extended() );
	r->Print();
   // RooAbsReal *chi2 = model.createChi2(*RooHist_data);
   // cout << "chi2ndof: " << chi2->getVal() / ((Double_t)h_data->GetNbinsX()) << endl;

   TCanvas *c1 = new TCanvas();
   c1->SetLogy();
   RooPlot *frame = mass->frame(Name("massframe"));
   // pdf_ttbar->plotOn(frame,LineColor(kRed));
   // pdf_DYJets->plotOn(frame,LineColor(kGreen));
   // pdf_WJets->plotOn(frame,LineColor(kBlue));
   // pdf_QCD->plotOn(frame,LineColor(kMagenta));
   RooHist_data->plotOn(frame);
   model.plotOn(frame,LineColor(kRed),FillColor(kRed));
   model.plotOn(frame,Components(RooArgSet(*pdf_ttbar,*pdf_WJets,*pdf_QCD)),LineColor(kCyan),FillColor(kCyan));
   model.plotOn(frame,Components(RooArgSet(*pdf_ttbar,*pdf_WJets)),LineColor(kBlue),FillColor(kBlue));
   model.plotOn(frame,Components(*pdf_ttbar),LineColor(kGreen+2),FillColor(kGreen+2));
   RooHist_data->plotOn(frame);
   frame->Draw();

   TLegend *tleg = new TLegend(0.6,0.6,0.9,0.92);
   tleg->SetBorderSize(0);
   TLegendEntry *tlegent;
   tlegent = tleg->AddEntry(frame->FindObject("h_RooHist_data"),"Data","LP"); 
   tlegent = tleg->AddEntry(frame->FindObject("model_Norm[mass]_Comp[pdf_ttbar]"),"Top","L"); tlegent->SetLineColor(kGreen+2);
   tlegent = tleg->AddEntry(frame->FindObject("model_Norm[mass]_Comp[pdf_ttbar,pdf_WJets] "),"W+jets","L"); tlegent->SetLineColor(kBlue);
   tlegent = tleg->AddEntry(frame->FindObject("model_Norm[mass]_Comp[pdf_ttbar,pdf_WJets,pdf_QCD] "),"Dijet","L"); tlegent->SetLineColor(kCyan);
   tlegent = tleg->AddEntry(frame->FindObject("model_Norm[mass]"),"Drell-Yan","L"); tlegent->SetLineColor(kRed);
   tleg->Draw();
   c1->SaveAs("fit_wjets_" + vartag + Form("%d.pdf",iFR));
}
