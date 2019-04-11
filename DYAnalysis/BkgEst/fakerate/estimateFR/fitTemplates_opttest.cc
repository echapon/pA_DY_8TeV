#include "TFile.h"
#include "TH1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "TPaveText.h"

#include "../../interface/tdrstyle.C"
#include "../../interface/CMS_lumi.C"
#include "../../interface/defs.h"
using namespace RooFit;
using namespace DYana;


void fitTemplates_opttest(TString indirname, TString outdirname, const TString& category, const TString& xtitle, double xmin, double xmax, int opt, int QCDopt, TString opttest, TString opt2)
{
	//Get ROOT Files
	TFile* file[NSamples+2];

	for (int i=0; i<=QCD; i++) {
		if (i==ALL) continue;
		SampleTag tag = static_cast<SampleTag>(i);	
		file[i] = new TFile(Form("%s/histFR_%s_%s%s.root",indirname.Data(),opttest.Data(),Name(tag),opt2.Data()),"READ");
		std::cout << TString(Name(tag)) << " - " << Form("%s/histFR_%s_%s%s.root",indirname.Data(),opttest.Data(),Name(tag),opt2.Data()) << std::endl;
	}

	//Get Histograms
	TH1D *h_WpMu	= (TH1D*)file[WpMu]->Get("denominator" + category );
	TH1D *h_WmMu	= (TH1D*)file[WmMu]->Get("denominator" + category );
	TH1D *h_WpTau	= (TH1D*)file[WpTau]->Get("denominator" + category );
	TH1D *h_WmTau	= (TH1D*)file[WmTau]->Get("denominator" + category );

	TH1D *h_DYMuMu1030		= (TH1D*)file[DYMuMu1030]->Get("denominator" + category );
	TH1D *h_DYMuMu30			= (TH1D*)file[DYMuMu30]->Get("denominator" + category );
	TH1D *h_DYMuMu1030_PbP	= (TH1D*)file[DYMuMu1030_PbP]->Get("denominator" + category );
	TH1D *h_DYMuMu30_PbP		= (TH1D*)file[DYMuMu30_PbP]->Get("denominator" + category );
	TH1D *h_DYTauTau1030		= (TH1D*)file[DYTauTau1030]->Get("denominator" + category );
	TH1D *h_DYTauTau30		= (TH1D*)file[DYTauTau30]->Get("denominator" + category );

	TH1D *h_ttbar = (TH1D*)file[TT]->Get( "denominator" + category );

	// For QCD - QCDopt = 1 (QCD MC), 2(Data SS) 
	TH1D *h_QCD;
	TFile* fileQCDSS1= new TFile(Form("%s/histFR_SS%s_%s%s.root",indirname.Data(),opttest.Data(),Name(static_cast<SampleTag>(Data1)),opt2.Data()),"READ");
	TFile* fileQCDSS2= new TFile(Form("%s/histFR_SS%s_%s%s.root",indirname.Data(),opttest.Data(),Name(static_cast<SampleTag>(Data2)),opt2.Data()),"READ");

	std::cout << Form("%s/histFR_SS%s_%s%s.root",indirname.Data(),opttest.Data(),Name(static_cast<SampleTag>(Data1)),opt2.Data()) << std::endl;
	std::cout << Form("%s/histFR_SS%s_%s%s.root",indirname.Data(),opttest.Data(),Name(static_cast<SampleTag>(Data2)),opt2.Data()) << std::endl;

	Double_t Lumi = lumi_all;
	Double_t Lumi1=lumi_part1;
	Double_t Lumi2=lumi_part2;

	TH1D *h_QCDMC = (TH1D*)file[QCD]->Get( "denominator" + category );
	h_QCDMC->Scale((Xsec(QCD)*Lumi)/Nevts(QCD));

	double Npass_QCDMC = h_QCDMC->Integral();


	if (QCDopt==1) {
		h_QCD = (TH1D*)file[QCD]->Get( "denominator" + category );
		h_QCD->Scale((Xsec(QCD)*Lumi)/Nevts(QCD));
	}
	else if (QCDopt==2) {
		h_QCD = (TH1D*)fileQCDSS1->Get( "denominator" + category );
		h_QCD->Add((TH1D*)fileQCDSS2->Get( "denominator" + category ));
	}

	TH1D *h_TW = (TH1D*)file[TW]->Get( "denominator" + category );
	TH1D *h_TbarW = (TH1D*)file[TbarW]->Get( "denominator" + category );
	TH1D *h_WW = (TH1D*)file[WW]->Get( "denominator" + category );
	TH1D *h_WZ = (TH1D*)file[WZ]->Get( "denominator" + category );
	TH1D *h_ZZ = (TH1D*)file[ZZ]->Get( "denominator" + category );

	TH1D *h_data = (TH1D*)file[Data1]->Get( "denominator" + category );
	h_data->Add((TH1D*)file[Data2]->Get( "denominator" + category ));

	// MC scaled(multiplied) (Xsec(channel)*Lumi)/Nevts(channel)
	std::cout << "########## Xsec * Lumi (before scale)##############" << std::endl;
	std::cout << "QCD(total) - xsec: " << Xsec(QCD) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(QCD)*Lumi << std::endl;
	std::cout << "WpMu(total) - xsec: " << Xsec(WpMu) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(WpMu)*Lumi << std::endl;
	std::cout << "WmMu(total) - xsec: " << Xsec(WmMu) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(WmMu)*Lumi << std::endl;
	std::cout << "WpTau(total) - xsec: " << Xsec(WpTau) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(WpTau)*Lumi << std::endl;
	std::cout << "WmTau(total) - xsec: " << Xsec(WmTau) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(WmTau)*Lumi << std::endl;
	std::cout << "DYMuMu1030(Lumi2) - xsec: " << Xsec(DYMuMu1030) << " * Lumi2: " << Lumi2 << " -> xsec*Lumi: " << Xsec(DYMuMu1030)*Lumi2 << std::endl;
	std::cout << "DYMuMu30(Lumi2) - xsec: " << Xsec(DYMuMu30) << " * Lumi2: " << Lumi2 << " -> xsec*Lumi: " << Xsec(DYMuMu30)*Lumi2 << std::endl;
	std::cout << "DYMuMu1030_PbP(Lumi1) - xsec: " << Xsec(DYMuMu1030_PbP) << " * Lumi1: " << Lumi1 << " -> xsec*Lumi: " << Xsec(DYMuMu1030_PbP)*Lumi1 << std::endl;
	std::cout << "DYMuMu30_PbP(Lumi1) - xsec: " << Xsec(DYMuMu30_PbP) << " * Lumi1: " << Lumi1 << " -> xsec*Lumi: " << Xsec(DYMuMu30_PbP)*Lumi1 << std::endl;
	std::cout << "DYTauTau1030(Lumi) - xsec: " << Xsec(DYTauTau1030) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(DYTauTau1030)*Lumi << std::endl;
	std::cout << "DYTauTau30(Lumi) - xsec: " << Xsec(DYTauTau30) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(DYTauTau30)*Lumi << std::endl;
	std::cout << "TW(total) - xsec: " << Xsec(TW) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(TW)*Lumi << std::endl;
	std::cout << "TbarW(total) - xsec: " << Xsec(TbarW) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(TbarW)*Lumi << std::endl;
	std::cout << "WW(total) - xsec: " << Xsec(WW) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(WW)*Lumi << std::endl;
	std::cout << "WZ(total) - xsec: " << Xsec(WZ) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(WZ)*Lumi << std::endl;
	std::cout << "ZZ(total) - xsec: " << Xsec(ZZ) << " * Lumi: " << Lumi << " -> xsec*Lumi: " << Xsec(ZZ)*Lumi << std::endl;
	std::cout << std::endl;	

	std::cout << "########## Xsec * Lumi (before scale)##############" << std::endl;
	std::cout << "Data(total) - Lumiall: " << Lumi << " pb-1, Integral: " << h_data->Integral() << std::endl;
	std::cout << "QCD(total) - xsec: " << Xsec(QCD) << " * Lumi: " << Lumi << " -> Integral: " << h_QCD->Integral() << std::endl;
	std::cout << "WpMu(total) - xsec: " << Xsec(WpMu) << " * Lumi: " << Lumi << " -> Integral: " << h_WpMu->Integral() << std::endl;
	std::cout << "WmMu(total) - xsec: " << Xsec(WmMu) << " * Lumi: " << Lumi << " -> Integral: " << h_WmMu->Integral() << std::endl;
	std::cout << "WpTau(total) - xsec: " << Xsec(WpTau) << " * Lumi: " << Lumi << " -> Integral: " << h_WpTau->Integral() << std::endl;
	std::cout << "WmTau(total) - xsec: " << Xsec(WmTau) << " * Lumi: " << Lumi << " -> Integral: " << h_WmTau->Integral() << std::endl;
	std::cout << "DYMuMu1030(Lumi2) - xsec: " << Xsec(DYMuMu1030) << " * Lumi2: " << Lumi2 << " -> Integral: " << h_DYMuMu1030->Integral() << std::endl;
	std::cout << "DYMuMu30(Lumi2) - xsec: " << Xsec(DYMuMu30) << " * Lumi2: " << Lumi2 << " -> Integral: " << h_DYMuMu30->Integral() << std::endl;
	std::cout << "DYMuMu1030_PbP(Lumi1) - xsec: " << Xsec(DYMuMu1030_PbP) << " * Lumi1: " << Lumi1 << " -> Integral: " << h_DYMuMu1030_PbP->Integral() << std::endl;
	std::cout << "DYMuMu30_PbP(Lumi1) - xsec: " << Xsec(DYMuMu30_PbP) << " * Lumi1: " << Lumi1 << " -> Integral: " << h_DYMuMu30_PbP->Integral() << std::endl;
	std::cout << "DYTauTau1030(Lumi) - xsec: " << Xsec(DYTauTau1030) << " * Lumi: " << Lumi << " -> Integral: " << h_DYTauTau1030->Integral() << std::endl;
	std::cout << "DYTauTau30(Lumi) - xsec: " << Xsec(DYTauTau30) << " * Lumi: " << Lumi << " -> Integral: " << h_DYTauTau30->Integral() << std::endl;
	std::cout << "TW(total) - xsec: " << Xsec(TW) << " * Lumi: " << Lumi << " -> Integral: " << h_TW->Integral() << std::endl;
	std::cout << "TbarW(total) - xsec: " << Xsec(TbarW) << " * Lumi: " << Lumi << " -> Integral: " << h_TbarW->Integral() << std::endl;
	std::cout << "WW(total) - xsec: " << Xsec(WW) << " * Lumi: " << Lumi << " -> Integral: " << h_WW->Integral() << std::endl;
	std::cout << "WZ(total) - xsec: " << Xsec(WZ) << " * Lumi: " << Lumi << " -> Integral: " << h_WZ->Integral() << std::endl;
	std::cout << "ZZ(total) - xsec: " << Xsec(ZZ) << " * Lumi: " << Lumi << " -> Integral: " << h_ZZ->Integral() << std::endl;
	std::cout << std::endl;	


	h_WpMu->Scale((Xsec(WpMu)*Lumi)/Nevts(WpMu));
	h_WmMu->Scale((Xsec(WmMu)*Lumi)/Nevts(WmMu));
	h_WpTau->Scale((Xsec(WpTau)*Lumi)/Nevts(WpTau));
	h_WmTau->Scale((Xsec(WmTau)*Lumi)/Nevts(WmTau));

	h_DYMuMu1030->Scale((Xsec(DYMuMu1030)*Lumi2)/Nevts(DYMuMu1030));
	h_DYMuMu30->Scale((Xsec(DYMuMu30)*Lumi2)/Nevts(DYMuMu30));
	h_DYMuMu1030_PbP->Scale((Xsec(DYMuMu1030_PbP)*Lumi1)/Nevts(DYMuMu1030_PbP));
	h_DYMuMu30_PbP->Scale((Xsec(DYMuMu30_PbP)*Lumi1)/Nevts(DYMuMu30_PbP));
	h_DYTauTau1030->Scale((Xsec(DYTauTau1030)*Lumi)/Nevts(DYTauTau1030));
	h_DYTauTau30->Scale((Xsec(DYTauTau30)*Lumi)/Nevts(DYTauTau30));

	h_ttbar->Scale((Xsec(TT)*Lumi)/Nevts(TT));
	h_TW->Scale((Xsec(TW)*Lumi)/Nevts(TW));
	h_TbarW->Scale((Xsec(TbarW)*Lumi)/Nevts(TbarW));
	h_WW->Scale((Xsec(WW)*Lumi)/Nevts(WW));
	h_WZ->Scale((Xsec(WZ)*Lumi)/Nevts(WZ));
	h_ZZ->Scale((Xsec(ZZ)*Lumi)/Nevts(ZZ));

	// for DYJets, WJets, merge histrograms after normalization
	TH1D *h_DYJets = (TH1D*)h_DYMuMu1030->Clone();
	h_DYJets->Add(h_DYMuMu30);
   h_DYJets->Add(h_DYMuMu1030_PbP);
   h_DYJets->Add(h_DYMuMu30_PbP);
	h_DYJets->Add(h_DYTauTau1030);
	h_DYJets->Add(h_DYTauTau30);

	TH1D *h_WJets = (TH1D*)h_WpMu->Clone();
	h_WJets->Add(h_WmMu);
	h_WJets->Add(h_WpTau);
	h_WJets->Add(h_WmTau);

	h_TW->Add(h_TbarW);   

	std::cout << "########## Xsec * Lumi ##############" << std::endl;
	std::cout << "Data(total) - Lumiall: " << Lumi << " pb-1, Integral: " << h_data->Integral() << std::endl;
	std::cout << "QCD(total) - xsec: " << Xsec(QCD) << " * Lumi: " << Lumi << " -> Integral: " << h_QCD->Integral() << std::endl;
	std::cout << "WpMu(total) - xsec: " << Xsec(WpMu) << " * Lumi: " << Lumi << " -> Integral: " << h_WpMu->Integral() << std::endl;
	std::cout << "WmMu(total) - xsec: " << Xsec(WmMu) << " * Lumi: " << Lumi << " -> Integral: " << h_WmMu->Integral() << std::endl;
	std::cout << "WpTau(total) - xsec: " << Xsec(WpTau) << " * Lumi: " << Lumi << " -> Integral: " << h_WpTau->Integral() << std::endl;
	std::cout << "WmTau(total) - xsec: " << Xsec(WmTau) << " * Lumi: " << Lumi << " -> Integral: " << h_WmTau->Integral() << std::endl;
	std::cout << "DYMuMu1030(Lumi2) - xsec: " << Xsec(DYMuMu1030) << " * Lumi2: " << Lumi2 << " -> Integral: " << h_DYMuMu1030->Integral() << std::endl;
	std::cout << "DYMuMu30(Lumi2) - xsec: " << Xsec(DYMuMu30) << " * Lumi2: " << Lumi2 << " -> Integral: " << h_DYMuMu30->Integral() << std::endl;
	std::cout << "DYMuMu1030_PbP(Lumi1) - xsec: " << Xsec(DYMuMu1030_PbP) << " * Lumi1: " << Lumi1 << " -> Integral: " << h_DYMuMu1030_PbP->Integral() << std::endl;
	std::cout << "DYMuMu30_PbP(Lumi1) - xsec: " << Xsec(DYMuMu30_PbP) << " * Lumi1: " << Lumi1 << " -> Integral: " << h_DYMuMu30_PbP->Integral() << std::endl;
	std::cout << "DYTauTau1030(Lumi) - xsec: " << Xsec(DYTauTau1030) << " * Lumi: " << Lumi << " -> Integral: " << h_DYTauTau1030->Integral() << std::endl;
	std::cout << "DYTauTau30(Lumi) - xsec: " << Xsec(DYTauTau30) << " * Lumi: " << Lumi << " -> Integral: " << h_DYTauTau30->Integral() << std::endl;
	std::cout << "TW(total) - xsec: " << Xsec(TW) << " * Lumi: " << Lumi << " -> Integral: " << h_TW->Integral() << std::endl;
	std::cout << "TbarW(total) - xsec: " << Xsec(TbarW) << " * Lumi: " << Lumi << " -> Integral: " << h_TbarW->Integral() << std::endl;
	std::cout << "WW(total) - xsec: " << Xsec(WW) << " * Lumi: " << Lumi << " -> Integral: " << h_WW->Integral() << std::endl;
	std::cout << "WZ(total) - xsec: " << Xsec(WZ) << " * Lumi: " << Lumi << " -> Integral: " << h_WZ->Integral() << std::endl;
	std::cout << "ZZ(total) - xsec: " << Xsec(ZZ) << " * Lumi: " << Lumi << " -> Integral: " << h_ZZ->Integral() << std::endl;
	std::cout << std::endl;	

	////////////////////////////////

	// get N_channel : normalized intrgral of each signal
	Double_t Npass_ttbar = h_ttbar->Integral();
	Double_t NN_ttbar = Npass_ttbar;
	cout << "N_ttbar: "<< NN_ttbar << endl;

	Double_t Npass_DYJets = h_DYJets->Integral();
	Double_t NN_DYJets = Npass_DYJets;
	cout << "N_DYJets: "<< NN_DYJets << endl;

	Double_t Npass_WJets = h_WJets->Integral();
	Double_t NN_WJets = Npass_WJets;
	cout << "N_WJets: "<< NN_WJets << endl;

	Double_t Npass_TW = h_TW->Integral();
	Double_t NN_TW = Npass_TW;
	cout << "N_TW(TW+TbarW): "<< NN_TW << endl;

	Double_t Npass_WW = h_WW->Integral();
	Double_t NN_WW = Npass_WW;
	cout << "N_WW: "<< NN_WW << endl;

	Double_t Npass_WZ = h_WZ->Integral();
	Double_t NN_WZ = Npass_WZ;
	cout << "N_WZ: "<< NN_WZ << endl;

	Double_t Npass_ZZ = h_ZZ->Integral();
	Double_t NN_ZZ = Npass_ZZ;
	cout << "N_ZZ: "<< NN_ZZ << endl;

	Double_t Npass_QCD = h_QCD->Integral();
	Double_t NN_QCD;
	if (QCDopt==1) NN_QCD = Npass_QCDMC;
	else if (QCDopt==2) NN_QCD= Npass_QCD;

	cout << "N_QCD: "<< NN_QCD << endl;

	/*
		Double_t NN_QCD = Lumi * h_QCD->Integral();
		*/	
	cout << "#############################" << endl;

	double N_total = NN_ttbar + NN_DYJets + NN_WJets + NN_QCD + NN_TW + NN_WW + NN_WZ + NN_ZZ;
	double h_data_int = h_data->Integral();
	double N_ttbar = h_data->Integral()*NN_ttbar/N_total;
	double N_DYJets = h_data->Integral()*NN_DYJets/N_total;
	double N_WJets = h_data->Integral()*NN_WJets/N_total;
	double N_QCD = h_data->Integral()*NN_QCD/N_total;
	double N_TW = h_data->Integral()*NN_TW/N_total;
	double N_WW = h_data->Integral()*NN_WW/N_total;
	double N_WZ = h_data->Integral()*NN_WZ/N_total;
	double N_ZZ = h_data->Integral()*NN_ZZ/N_total;

	//Convert TH1D to RooDataHist
	RooRealVar obs("obs", xtitle, xmin, xmax);

	RooDataHist *RooHist_ttbar = new RooDataHist("RooHist_ttbar", "RooHistogram_ttbar", obs, h_ttbar);
	RooDataHist *RooHist_WJets = new RooDataHist("RooHist_WJets", "RooHistogram_WJets", obs, h_WJets);
	RooDataHist *RooHist_DYJets = new RooDataHist("RooHist_DYJets", "RooHistogram_DYJets", obs, h_DYJets);
	RooDataHist *RooHist_QCD = new RooDataHist("RooHist_QCD", "RooHistogram_QCD", obs, h_QCD);
	RooDataHist *RooHist_data = new RooDataHist("RooHist_data", "RooHistogram_data", obs, h_data);
	RooDataHist *RooHist_TW = new RooDataHist("RooHist_TW", "RooHistogram_TW", obs, h_TW);
	RooDataHist *RooHist_WW = new RooDataHist("RooHist_WW", "RooHistogram_WW", obs, h_WW);
	RooDataHist *RooHist_WZ = new RooDataHist("RooHist_WZ", "RooHistogram_WZ", obs, h_WZ);
	RooDataHist *RooHist_ZZ = new RooDataHist("RooHist_ZZ", "RooHistogram_ZZ", obs, h_ZZ);

	//Convert RooDataHist to RooHistPdf
	RooHistPdf *pdf_ttbar = new RooHistPdf("pdf_ttbar", "Template from ttbar MC", obs, *RooHist_ttbar, 0);
	RooHistPdf *pdf_WJets = new RooHistPdf("pdf_WJets", "Template from WJets MC", obs, *RooHist_WJets, 0);
	RooHistPdf *pdf_DYJets = new RooHistPdf("pdf_DYJets", "Template from DYJets MC", obs, *RooHist_DYJets, 0);
	RooHistPdf *pdf_QCD = new RooHistPdf("pdf_QCD", "Template from QCD MC", obs, *RooHist_QCD, 0);
	RooHistPdf *pdf_TW = new RooHistPdf("pdf_TW", "Template from TW MC", obs, *RooHist_TW, 0);
	RooHistPdf *pdf_WW = new RooHistPdf("pdf_WW", "Template from WW MC", obs, *RooHist_WW, 0);
	RooHistPdf *pdf_WZ = new RooHistPdf("pdf_WZ", "Template from WZ MC", obs, *RooHist_WZ, 0);
	RooHistPdf *pdf_ZZ = new RooHistPdf("pdf_ZZ", "Template from ZZ MC", obs, *RooHist_ZZ, 0);

	RooRealVar n_ttbar("n_ttbar", "n_ttbar", NN_ttbar, NN_ttbar*0.5, NN_ttbar*1.5);
	RooRealVar n_WJets("n_WJets", "n_WJets", NN_WJets, N_WJets*0.5, N_WJets*1.5);
	RooRealVar n_DYJets("n_DYJets", "n_DYJets", NN_DYJets, NN_DYJets*0.5, NN_DYJets*1.5);
	RooRealVar n_QCD("n_QCD", "n_QCD", 0.875*h_data->Integral(), 10, h_data->Integral());
	RooRealVar n_TW("n_TW", "n_TW", NN_TW, 0.5*NN_TW, NN_TW*1.5);
	RooRealVar n_WW("n_WW", "n_WW", NN_WW, 0.5*NN_WW, NN_WW*1.5);
	RooRealVar n_WZ("n_WZ", "n_WZ", NN_WZ, 0.5*NN_WZ, NN_WZ*1.5);
	RooRealVar n_ZZ("n_ZZ", "n_ZZ", NN_ZZ, 0.5*NN_ZZ, NN_ZZ*1.5);

	std::cout << "############################################" << std::endl;
	std::cout << "    N_QCD     : " << N_QCD << std::endl;
	std::cout << "    NN_QCD    : " << NN_QCD << std::endl;
	std::cout << "############################################" << std::endl;
	std::cout << "    N_DYJets  : " << N_DYJets << std::endl;
	std::cout << "    NN_DYJets : " << NN_DYJets << std::endl;
	std::cout << "############################################" << std::endl;
	std::cout << "    N_WJets   : " << N_WJets << std::endl;
	std::cout << "    NN_WJets  : " << NN_WJets << std::endl;
	std::cout << "############################################" << std::endl;
	std::cout << "    N_ttbar   : " << N_ttbar << std::endl;
	std::cout << "    NN_ttbar  : " << NN_ttbar << std::endl;
	std::cout << "############################################" << std::endl;
	std::cout << "    N_TW      : " << N_TW << std::endl;
	std::cout << "    NN_TW     : " << NN_TW << std::endl;
	std::cout << "############################################" << std::endl;
	std::cout << "    N_WW      : " << N_WW << std::endl;
	std::cout << "    NN_WW     : " << NN_WW << std::endl;
	std::cout << "############################################" << std::endl;
	std::cout << "    N_WZ      : " << N_WZ << std::endl;
	std::cout << "    NN_WZ     : " << NN_WZ << std::endl;
	std::cout << "############################################" << std::endl;
	std::cout << "    N_ZZ      : " << N_ZZ << std::endl;
	std::cout << "    NN_ZZ     : " << NN_ZZ << std::endl;
	std::cout << "############################################" << std::endl;

//NN : after scale, hitogram
//N : consideration with ratio

	double lumiSF_min=0.95;
	double lumiSF_max=1.05;

	switch (opt) {

		case 1005:
			std::cout << "##### opt 1005, SF uncertainties 5% #####" << std::endl;
			lumiSF_min=0.95;
			lumiSF_max=1.05;
			break;

		case 1010:
			std::cout << "##### opt 1010, SF uncertainties 10% #####" << std::endl;
			lumiSF_min=0.90;
			lumiSF_max=1.10;
			break;

		case 1020:
			std::cout << "##### opt 1020, SF uncertainties 20% #####" << std::endl;
			lumiSF_min=0.80;
			lumiSF_max=1.20;
			break;

		case 1030:
			std::cout << "##### opt 1030, SF uncertainties 30% #####" << std::endl;
			lumiSF_min=0.70;
			lumiSF_max=1.30;
			break;

		case 1040:
			std::cout << "##### opt 1040, SF uncertainties 40% #####" << std::endl;
			lumiSF_min=0.60;
			lumiSF_max=1.40;
			break;

		case 1050:
			std::cout << "##### opt 1050, SF uncertainties 50% #####" << std::endl;
			lumiSF_min=0.50;
			lumiSF_max=1.50;
			break;

	}
	RooRealVar n_ttbar0("n_ttbar0", "n_ttbar0", NN_ttbar);
	RooRealVar n_WJets0("n_WJets0", "n_WJets0", NN_WJets);
	RooRealVar n_DYJets0("n_DYJets0", "n_DYJets0", NN_DYJets);
	RooRealVar n_QCD0("n_QCD0", "n_QCD0", 0.875*h_data->Integral());
	RooRealVar n_TW0("n_TW0", "n_TW0", NN_TW);
	RooRealVar n_WW0("n_WW0", "n_WW0", NN_WW);
	RooRealVar n_WZ0("n_WZ0", "n_WZ0", NN_WZ);
	RooRealVar n_ZZ0("n_ZZ0", "n_ZZ0", NN_ZZ);
	RooRealVar lumiSF("lumiSF","lumiSF",1,lumiSF_min,lumiSF_max);

	RooFormulaVar n_ttbar1("n_ttbar1", "@0*@1", RooArgSet(n_ttbar0,lumiSF));
	RooFormulaVar n_WJets1("n_WJets1", "@0*@1", RooArgSet(n_WJets0,lumiSF));
	RooFormulaVar n_DYJets1("n_DYJets1", "@0*@1", RooArgSet(n_DYJets0,lumiSF));
	RooFormulaVar n_TW1("n_TW1", "@0*@1", RooArgSet(n_TW0,lumiSF));
	RooFormulaVar n_WW1("n_WW1", "@0*@1", RooArgSet(n_WW0,lumiSF));
	RooFormulaVar n_WZ1("n_WZ1", "@0*@1", RooArgSet(n_WZ0,lumiSF));
	RooFormulaVar n_ZZ1("n_ZZ1", "@0*@1", RooArgSet(n_ZZ0,lumiSF));
	RooRealVar n_QCD1("n_QCD1", "n_QCD1", 0.875*h_data->Integral(), 0.1*h_data->Integral(), 10*h_data->Integral());

	RooAddPdf model = RooAddPdf( "model","model",RooArgList(*pdf_QCD, *pdf_WJets, *pdf_DYJets, *pdf_ttbar, *pdf_TW, *pdf_WW, *pdf_WZ, *pdf_ZZ), RooArgList(n_QCD1, n_WJets1, n_DYJets1, n_ttbar1, n_TW1, n_WW1, n_WZ1, n_ZZ1) ); 

	RooFitResult* r = model.fitTo( *RooHist_data, Save() );

	std::cout << "########################" << std::endl;
	std::cout << "n_ttbar/N_ttbar: " << n_ttbar1.getVal()/N_ttbar << std::endl;
	std::cout << "n_DYJets/N_DYJets: " << n_DYJets1.getVal()/N_DYJets << std::endl;
	std::cout << "n_WJets/N_WJets: " << n_WJets1.getVal()/N_WJets << std::endl;
	std::cout << "n_QCD/N_QCD: " << n_QCD1.getVal()/N_QCD << std::endl;
	std::cout << "n_TW/N_TW: " << n_TW1.getVal()/N_TW << std::endl;
	std::cout << "n_WW/N_WW: " << n_WW1.getVal()/N_WW << std::endl;
	std::cout << "n_WZ/N_WZ: " << n_WZ1.getVal()/N_WZ << std::endl;
	std::cout << "n_ZZ/N_ZZ: " << n_ZZ1.getVal()/N_ZZ << std::endl;
	std::cout << "########################" << std::endl;
	std::cout << "n_ttbar/NN_ttbar: " << n_ttbar1.getVal()/NN_ttbar << std::endl;
	std::cout << "n_DYJets/NN_DYJets: " << n_DYJets1.getVal()/NN_DYJets << std::endl;
	std::cout << "n_WJets/NN_WJets: " << n_WJets1.getVal()/NN_WJets << std::endl;
	std::cout << "n_QCD/NN_QCD: " << n_QCD1.getVal()/NN_QCD << std::endl;
	std::cout << "n_TW/NN_TW: " << n_TW1.getVal()/NN_TW << std::endl;
	std::cout << "n_WW/NN_WW: " << n_WW1.getVal()/NN_WW << std::endl;
	std::cout << "n_WZ/NN_WZ: " << n_WZ1.getVal()/NN_WZ << std::endl;
	std::cout << "n_ZZ/NN_ZZ: " << n_ZZ1.getVal()/NN_ZZ << std::endl;
	std::cout << "########################" << std::endl;

	TCanvas *c_fit = new TCanvas("c_fit", "", 800, 800);
	c_fit->cd();

	//Top Pad
	TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
	c1_1->Draw();
	c1_1->cd();
	c1_1->SetTopMargin(0.01);
	c1_1->SetBottomMargin(0.25);
	c1_1->SetRightMargin(0.03);
	c1_1->SetLeftMargin(0.09);
	c1_1->SetFillStyle(0);
	c1_1->SetLogy(1);

	RooPlot* frame1 = obs.frame( Title(" ") ) ;
	RooHist_data->plotOn(frame1, DataError(RooAbsData::SumW2));
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar,pdf_DYJets,pdf_WJets,pdf_QCD"), LineColor(0), FillColor(7), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar,pdf_DYJets,pdf_WJets"), LineColor(0), FillColor(4), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar,pdf_DYJets"), LineColor(0), FillColor(2), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar"), LineColor(0), FillColor(3), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW"), LineColor(0), FillColor(13), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW"), LineColor(0), FillColor(15), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ"), LineColor(0), FillColor(14), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ"), LineColor(0), FillColor(15), DrawOption("F") );
	RooHist_data->plotOn(frame1, DataError(RooAbsData::SumW2));
	frame1->Draw();
	r->Print();

	std::cout << "########################" << std::endl;

	TLegend *leg1 = new TLegend(0.58,0.7,.96,.98);
	leg1->SetFillColor(kWhite);
	leg1->SetLineColor(kWhite);
	leg1->AddEntry(frame1->nameOf(0),Form("Data : %.2f",h_data->Integral()), "EP");
	leg1->AddEntry(frame1->nameOf(1),Form("QCD : %.2f (%.2f)",n_QCD1.getVal(),n_QCD1.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(2),Form("WJets : %.2f (%.2f)",n_WJets1.getVal(),n_WJets1.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(3),Form("DYJets : %.2f (%.2f)",n_DYJets1.getVal(),n_DYJets1.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(4),Form("ttbar : %.2f (%.2f)",n_ttbar1.getVal(),n_ttbar1.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(5),Form("TW : %.2f (%.2f)",n_TW1.getVal(),n_TW1.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(5),Form("WW : %.2f (%.2f)",n_WW1.getVal(),n_WW1.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(6),Form("WZ : %.2f (%.2f)",n_WZ1.getVal(),n_WZ1.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(7),Form("ZZ : %.2f (%.2f)",n_ZZ1.getVal(),n_ZZ1.getVal()/h_data->Integral()),"F");
	leg1->Draw();

	frame1->GetYaxis()->SetTitle("Entry");
	frame1->GetXaxis()->SetLabelSize(0);
	frame1->SetMinimum(0.5);
	frame1->SetMaximum(50000000.0);


	TH1D *h_MC = (TH1D*)model.createHistogram("h_MC", obs);
	h_data->GetXaxis()->SetRangeUser(xmin,xmax);
	h_MC->GetXaxis()->SetRangeUser(xmin,xmax);

	Double_t Ndata = h_data->Integral();
	Double_t NMC = h_MC->Integral();
	h_MC->Scale(Ndata / NMC );
	RooAbsReal *chi2 = model.createChi2(*RooHist_data);
	cout << "chi2: " << chi2->getVal() << endl;
	cout << "Normalized chi2: " << chi2->getVal() / ((Double_t)h_data->GetNbinsX()) << endl;
	cout << "#### UNTIL NOW : " << category << endl;

	TPaveText *pt_norchi2 = new TPaveText(0.05*(xmax-xmin),1000000,0.53*(xmax-xmin),30000000);
	pt_norchi2->AddText(Form("chi2/ndf : %f",chi2->getVal() / ((Double_t)h_data->GetNbinsX())));
	pt_norchi2->SetBorderSize(1);
	pt_norchi2->Draw();

	//Bottom Pad
	TPad *c1_2 = new TPad("padc1_2","padc1_2",0.01,0.01,0.99,0.25);
	c1_2->Draw();
	c1_2->cd();
	c1_2->SetTopMargin(0.1);
	c1_2->SetBottomMargin(0.30);
	c1_2->SetRightMargin(0.02);
	c1_2->SetLeftMargin(0.08);
	c1_2->SetFillStyle(0);
	c1_2->SetGrid();

	//Make ratio plot
	TH1D *h_ratio = (TH1D*)h_data->Clone();
	h_data->GetXaxis()->SetRangeUser(xmin,xmax);
	h_MC->GetXaxis()->SetRangeUser(xmin,xmax);
	h_ratio->GetXaxis()->SetRangeUser(xmin,xmax);

	h_data->Sumw2(); h_MC->Sumw2();
	h_ratio->Divide(h_data, h_MC);
	h_ratio->SetTitle("");
	h_ratio->GetXaxis()->SetMoreLogLabels();
	h_ratio->GetXaxis()->SetNoExponent();
	//	h_ratio->GetXaxis()->SetTitle( "PFIso/p_{T}" );
	h_ratio->GetXaxis()->SetTitle(xtitle.Data());

	h_ratio->GetYaxis()->SetTitle("data/MC");
	h_ratio->GetXaxis()->SetTitleSize(0.13);
	h_ratio->GetYaxis()->SetTitleSize(0.09);
	h_ratio->GetYaxis()->SetTitleOffset(0.4);
	h_ratio->GetXaxis()->SetLabelSize(0.11);
	h_ratio->GetYaxis()->SetLabelSize(0.07);
	h_ratio->GetYaxis()->SetTickLength(0.015);
	h_ratio->SetMaximum( 1.3 );
	h_ratio->SetMinimum( 0.7 );
	h_ratio->SetMarkerSize(0.3);
	h_ratio->SetStats(kFALSE);

	h_ratio->Draw("e1p");

	TH1D *h_line = (TH1D*)h_data->Clone();
	h_line->Reset("ICES");
	Int_t Nbins = h_line->GetNbinsX();
	for(Int_t i_bin=0; i_bin< Nbins; i_bin++)
		h_line->SetBinContent(i_bin+1, 1);

	h_line->SetLineColor(kRed);
	h_line->Draw("LSAME");

	TString optst=Form("opt%d",opt);
	std::cout << "########### optst : " << optst <<std::endl;
	TString QCDoptst=Form("QCDopt%d",QCDopt);
	std::cout << "########### QCDoptst : " << QCDoptst <<std::endl;

	c_fit->SaveAs(Form("printFit_%s/Fit",outdirname.Data())+category+"_"+optst+"_"+QCDoptst+"_"+opttest.Data()+opt2.Data()+"_QCDin0p875Data.pdf");

	c_fit->Close();
	TFile* fout = new TFile(Form("histogramsFit_%s/histFRFit",outdirname.Data())+category+"_"+optst+"_"+QCDoptst+"_"+opttest.Data()+opt2.Data()+"_QCDin0p875Data.root","recreate");

	fout->cd();	
	TH1D* h_fitNch = new TH1D("h_fitNch","",12,0,12);

	h_fitNch->SetBinContent(1,n_DYJets1.getVal());
	h_fitNch->SetBinContent(2,n_QCD1.getVal());
	h_fitNch->SetBinContent(3,n_WJets1.getVal());
	h_fitNch->SetBinContent(4,n_TW1.getVal());
	h_fitNch->SetBinContent(5,n_WW1.getVal());
	h_fitNch->SetBinContent(6,n_WZ1.getVal());
	h_fitNch->SetBinContent(7,n_ZZ1.getVal());
	h_fitNch->SetBinContent(8,n_ttbar1.getVal());
	h_fitNch->SetBinContent(9,h_data->Integral());
	h_fitNch->SetBinContent(10,chi2->getVal());
	h_fitNch->SetBinContent(11,h_data->GetNbinsX());
	h_fitNch->SetBinContent(12,chi2->getVal() / ((Double_t)h_data->GetNbinsX()));
	for (int g=1;g<13;g++) std::cout << h_fitNch->GetBinContent(g) << " ";
	std::cout << std::endl;
	h_fitNch->Write();
	std::cout << "##### CLOSE #####" << std::endl;
	return;
}
