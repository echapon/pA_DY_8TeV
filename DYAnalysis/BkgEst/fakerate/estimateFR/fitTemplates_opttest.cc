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


//void fitTemplates_opttest(const TString& category, const TString& xtitle, double xmin, double xmax, int opt, int QCDopt, TString opttest)
//###void fitTemplates_opttest(const TString& category, const TString& xtitle, double xmin, double xmax, int opt, int QCDopt, TString opttest, TString MuonIDopt)
void fitTemplates_opttest(const TString& category="", const TString& xtitle="PFIso/p_{T}", double xmin=0.0, double xmax=1.0, int opt=16, int QCDopt=1, TString opttest="ZVETO_SMUwJET_MuPtlt10", TString MuonIDopt="")
{
   //Get ROOT Files
   TFile* file[NSamples+2];

   //for (int i=0; i<ALL; i++) file[i] = new TFile(PathFRHistos(static_cast<SampleTag>(i)));
	//file[QCD] = new TFile(PathFRHistos(QCD));

	//###histFR_ZVETO_SMUwoJET_MuPtlt10_QCD_opttest.root
	for (int i=0; i<=QCD; i++) {
		if (i==ALL) continue;
		SampleTag tag = static_cast<SampleTag>(i);	
		//file[i] = new TFile(Form("histograms/histFR_%s_%s_opttest.root",opttest.Data(),Name(tag)),"READ");
		//std::cout << TString(Name(tag)) << " - " << Form("histograms/histFR_%s_%s_opttest.root",opttest.Data(),Name(tag)) << std::endl;
/*
		file[i] = new TFile(Form("histograms/histZVETO_%s%s.root",opttest.Data(),Name(tag)),"READ");
		std::cout << TString(Name(tag)) << " - " << Form("histograms/histZVETO_%s%s.root",opttest.Data(),Name(tag)) << std::endl;
*/
//		file[i] = new TFile(Form("histograms/hist%s%s.root",opttest.Data(),Name(tag)),"READ");
//		std::cout << TString(Name(tag)) << " - " << Form("histograms/hist%s%s.root",opttest.Data(),Name(tag)) << std::endl;

/*
		file[i] = new TFile(Form("histograms_test/histFR_%s_%s%s.root",opttest.Data(),Name(tag),MuonIDopt.Data()),"READ");
		std::cout << TString(Name(tag)) << " - " << Form("histograms_test/histFR_%s_%s%s.root",opttest.Data(),Name(tag),MuonIDopt.Data()) << std::endl;
*/

		file[i] = new TFile(Form("histograms_20180814/histFR_%s_%s%s.root",opttest.Data(),Name(tag),MuonIDopt.Data()),"READ");
		std::cout << TString(Name(tag)) << " - " << Form("histograms_20180814/histFR_%s_%s%s.root",opttest.Data(),Name(tag),MuonIDopt.Data()) << std::endl;

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
/*
	TFile* fileQCDSS1=new TFile("histograms/histFRQCDData1.root");
	TFile* fileQCDSS2=new TFile("histograms/histFRQCDData2.root");
*/
/*
	TFile* fileQCDSS1=new TFile("histograms/histZVETOData1.root");
   TFile* fileQCDSS2=new TFile("histograms/histZVETOData2.root");
*/
/*
   TFile* fileQCDSS1= new TFile("histograms/histZVETO_SMUwJET_MuPtlt15Data1.root","read");
   TFile* fileQCDSS2= new TFile("histograms/histZVETO_SMUwJET_MuPtlt15Data2.root","read");
*/

/*
	TFile* fileQCDSS1= new TFile(Form("histograms/histZVETO_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1))),"READ");
	TFile* fileQCDSS2= new TFile(Form("histograms/histZVETO_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2))),"READ");

	std::cout << Form("histograms/histZVETO_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1))) << std::endl;
	std::cout << Form("histograms/histZVETO_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2))) << std::endl;
*/
/*
	TFile* fileQCDSS1= new TFile(Form("histograms/hist%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1))),"READ");
	TFile* fileQCDSS2= new TFile(Form("histograms/hist%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2))),"READ");

	std::cout << Form("histograms/hist%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1))) << std::endl;
	std::cout << Form("histograms/hist%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2))) << std::endl;
*/
/*
	TFile* fileQCDSS1= new TFile(Form("histograms_test/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1)),MuonIDopt.Data()),"READ");
	TFile* fileQCDSS2= new TFile(Form("histograms_test/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2)),MuonIDopt.Data()),"READ");

	std::cout << Form("histograms_test/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1)),MuonIDopt.Data()) << std::endl;
	std::cout << Form("histograms_test/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2)),MuonIDopt.Data()) << std::endl;
*/

	TFile* fileQCDSS1= new TFile(Form("histograms_20180814/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1)),MuonIDopt.Data()),"READ");
	TFile* fileQCDSS2= new TFile(Form("histograms_20180814/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2)),MuonIDopt.Data()),"READ");

	std::cout << Form("histograms_20180814/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data1)),MuonIDopt.Data()) << std::endl;
	std::cout << Form("histograms_20180814/histFR_%s_%s%s.root",opttest.Data(),Name(static_cast<SampleTag>(Data2)),MuonIDopt.Data()) << std::endl;


	Double_t Lumi = lumi_all;


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
	TH1D *h_WW = (TH1D*)file[WW]->Get( "denominator" + category );
	TH1D *h_WZ = (TH1D*)file[WZ]->Get( "denominator" + category );
	TH1D *h_ZZ = (TH1D*)file[ZZ]->Get( "denominator" + category );

   TH1D *h_data = (TH1D*)file[Data1]->Get( "denominator" + category );
   h_data->Add((TH1D*)file[Data2]->Get( "denominator" + category ));

	// MC scaled(multiplied) (Xsec(channel)*Lumi)/Nevts(channel)

	h_WpMu->Scale((Xsec(WpMu)*Lumi)/Nevts(WpMu));
	h_WmMu->Scale((Xsec(WmMu)*Lumi)/Nevts(WmMu));
	h_WpTau->Scale((Xsec(WpTau)*Lumi)/Nevts(WpTau));
	h_WmTau->Scale((Xsec(WmTau)*Lumi)/Nevts(WmTau));

	h_DYMuMu1030->Scale((Xsec(DYMuMu1030)*Lumi)/Nevts(DYMuMu1030));
	h_DYMuMu30->Scale((Xsec(DYMuMu30)*Lumi)/Nevts(DYMuMu30));
	h_DYMuMu1030_PbP->Scale((Xsec(DYMuMu1030_PbP)*Lumi)/Nevts(DYMuMu1030_PbP));
	h_DYMuMu30_PbP->Scale((Xsec(DYMuMu30_PbP)*Lumi)/Nevts(DYMuMu30_PbP));
	h_DYTauTau1030->Scale((Xsec(DYTauTau1030)*Lumi)/Nevts(DYTauTau1030));
	h_DYTauTau30->Scale((Xsec(DYTauTau30)*Lumi)/Nevts(DYTauTau30));

	h_ttbar->Scale((Xsec(TT)*Lumi)/Nevts(TT));
	h_TW->Scale((Xsec(TW)*Lumi)/Nevts(TW));
	h_WW->Scale((Xsec(WW)*Lumi)/Nevts(WW));
	h_WZ->Scale((Xsec(WZ)*Lumi)/Nevts(WZ));
	h_ZZ->Scale((Xsec(ZZ)*Lumi)/Nevts(ZZ));

	// for DYJets, WJets, merge histrograms after normalization
	TH1D *h_DYJets = (TH1D*)h_DYMuMu1030->Clone();
   h_DYJets->Add(h_DYMuMu30);
//   h_DYJets->Add(h_DYMuMu1030_PbP);
//   h_DYJets->Add(h_DYMuMu30_PbP);
   h_DYJets->Add(h_DYTauTau1030);
   h_DYJets->Add(h_DYTauTau30);

	TH1D *h_WJets = (TH1D*)h_WpMu->Clone();
   h_WJets->Add(h_WmMu);
   h_WJets->Add(h_WpTau);
   h_WJets->Add(h_WmTau);




/*
	std::cout << "Integral(Data) : " << h_data->Integral() << std::endl;
 
	std::cout << "Xsec(TT) : " << Xsec(TT) << " , Lumi : " << Lumi << " , Nevts(TT): " << Nevts(TT) << " --- Scale factor : " << (Xsec(TT)*Lumi)/Nevts(TT) << std::endl;
	std::cout << "Xsec(QCD) : " << Xsec(QCD) << " , Lumi : " << Lumi << " , Nevts(QCD): " << Nevts(QCD) << " --- Scale factor : " << (Xsec(QCD)*Lumi)/Nevts(QCD) << std::endl;
	std::cout << "Xsec(WW) : " << Xsec(WW) << " , Lumi : " << Lumi << " , Nevts(WW): " << Nevts(WW) << " --- Scale factor : " << (Xsec(WW)*Lumi)/Nevts(WW) << std::endl;
	std::cout << "Xsec(WZ) : " << Xsec(WZ) << " , Lumi : " << Lumi << " , Nevts(WZ): " << Nevts(WZ) << " --- Scale factor : " << (Xsec(WZ)*Lumi)/Nevts(WZ) << std::endl;
	std::cout << "Xsec(ZZ) : " << Xsec(ZZ) << " , Lumi : " << Lumi << " , Nevts(ZZ): " << Nevts(ZZ) << " --- Scale factor : " << (Xsec(ZZ)*Lumi)/Nevts(ZZ) << std::endl;
	
	std::cout << "Xsec(WpMu) : " << Xsec(WpMu) << " , Lumi : " << Lumi << " , Nevts(WpMu): " << Nevts(WpMu) << " --- Scale factor : " << (Xsec(WpMu)*Lumi)/Nevts(WpMu) << std::endl;
	std::cout << "Xsec(WmMu) : " << Xsec(WmMu) << " , Lumi : " << Lumi << " , Nevts(WmMu): " << Nevts(WmMu) << " --- Scale factor : " << (Xsec(WmMu)*Lumi)/Nevts(WmMu) << std::endl;
	std::cout << "Xsec(WpTau) : " << Xsec(WpTau) << " , Lumi : " << Lumi << " , Nevts(WpTau): " << Nevts(WpTau) << " --- Scale factor : " << (Xsec(WpTau)*Lumi)/Nevts(WpTau) << std::endl;
	std::cout << "Xsec(WmTau) : " << Xsec(WmTau) << " , Lumi : " << Lumi << " , Nevts(WmTau): " << Nevts(WmTau) << " --- Scale factor : " << (Xsec(WmTau)*Lumi)/Nevts(WmTau) << std::endl;

	std::cout << "Xsec(DYMuMu1030) : " << Xsec(DYMuMu1030) << " , Lumi : " << Lumi << " , Nevts(DYMuMu1030): " << Nevts(DYMuMu1030) << " --- Scale factor : " << (Xsec(DYMuMu1030)*Lumi)/Nevts(DYMuMu1030) << std::endl;
	std::cout << "Xsec(DYMuMu30) : " << Xsec(DYMuMu30) << " , Lumi : " << Lumi << " , Nevts(DYMuMu30): " << Nevts(DYMuMu30) << " --- Scale factor : " << (Xsec(DYMuMu30)*Lumi)/Nevts(DYMuMu30) << std::endl;
	std::cout << "Xsec(DYMuMu1030_PbP) : " << Xsec(DYMuMu1030_PbP) << " , Lumi : " << Lumi << " , Nevts(DYMuMu1030_PbP): " << Nevts(DYMuMu1030_PbP) << " --- Scale factor : " << (Xsec(DYMuMu1030_PbP)*Lumi)/Nevts(DYMuMu1030_PbP) << std::endl;
	std::cout << "Xsec(DYMuMu30_PbP) : " << Xsec(DYMuMu30_PbP) << " , Lumi : " << Lumi << " , Nevts(DYMuMu30_PbP): " << Nevts(DYMuMu30_PbP) << " --- Scale factor : " << (Xsec(DYMuMu30_PbP)*Lumi)/Nevts(DYMuMu30_PbP) << std::endl;
	std::cout << "Xsec(DYTauTau1030) : " << Xsec(DYTauTau1030) << " , Lumi : " << Lumi << " , Nevts(DYTauTau1030): " << Nevts(DYTauTau1030) << " --- Scale factor : " << (Xsec(DYTauTau1030)*Lumi)/Nevts(DYTauTau1030) << std::endl;
	std::cout << "Xsec(DYTauTau30) : " << Xsec(DYTauTau30) << " , Lumi : " << Lumi << " , Nevts(DYTauTau30): " << Nevts(DYTauTau30) << " --- Scale factor : " << (Xsec(DYTauTau30)*Lumi)/Nevts(DYTauTau30) << std::endl;

	std::cout << h_ttbar->Integral() << " + " << h_QCD->Integral() << " + " << h_WW->Integral() << " + " << h_WZ->Integral() << " + " << h_ZZ->Integral() << " + " << h_WJets->Integral() << " + " << h_DYJets->Integral() << std::endl;
	std::cout << "Integral(MC) : " << h_ttbar->Integral()+h_QCD->Integral()+h_WW->Integral()+h_WZ->Integral()+h_ZZ->Integral()+h_WJets->Integral()+h_DYJets->Integral() << std::endl;
	std::cout << "Integral(MC-QCD) : " << h_ttbar->Integral()+h_WW->Integral()+h_WZ->Integral()+h_ZZ->Integral()+h_WJets->Integral()+h_DYJets->Integral() << std::endl;
*/





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
	cout << "N_TW: "<< NN_TW << endl;

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
	if (QCDopt==1) NN_QCD = Npass_QCD;
	else if (QCDopt==2) NN_QCD= Npass_QCDMC;

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
/*
   std::cout << N_total << " " <<  NN_ttbar << " " << NN_DYJets << " " << NN_WJets << " " << NN_QCD << " " << NN_WW << " " << NN_WZ << " " << NN_ZZ << std::endl;
   std::cout << N_total/N_total << " " <<  NN_ttbar/N_total << " " << NN_DYJets/N_total << " " << NN_WJets/N_total << " " << NN_QCD/N_total << " " << NN_WW/N_total << " " << NN_WZ/N_total << " " << NN_ZZ/N_total << std::endl;
*/
	// Double_t xsec_DYJets = 6025.2; Double_t Nprocessed_DYJets = 2000000; Double_t Npass_DYJets = h_DYJets->Integral();
	// Double_t N_DYJets = ((xsec_DYJets * Lumi) / Nprocessed_DYJets) * Npass_DYJets;
	// cout << "N_DYJets: "<< N_DYJets << " Range: " << N_DYJets*0.5 << " " << N_DYJets*1.5 << endl;
	

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
/*
 	RooRealVar n_ttbar("n_ttbar", "n_ttbar", N_ttbar, N_ttbar*1.0, N_ttbar*1.0);
	RooRealVar n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*1.0, N_WJets*1.00);
	RooRealVar n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*1.0, N_DYJets*1.0);
	RooRealVar n_QCD("n_QCD", "n_QCD", N_QCD, 1.0*N_QCD, N_QCD*1.0);
	RooRealVar n_WW("n_WW", "n_WW", N_WW, 1.0*N_WW, N_WW*1.0);
	RooRealVar n_WZ("n_WZ", "n_WZ", N_WZ, 1.0*N_WZ, N_WZ*1.0);
	RooRealVar n_ZZ("n_ZZ", "n_ZZ", N_ZZ, 1.0*N_ZZ, N_ZZ*1.0);
*/
/*
 	RooRealVar n_ttbar("n_ttbar", "n_ttbar", N_ttbar, NN_ttbar*0.1, NN_ttbar*100.0);
	RooRealVar n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*1000.00);
	RooRealVar n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.01, N_DYJets*1000.0);
	RooRealVar n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	RooRealVar n_WW("n_WW", "n_WW", N_WW, 0.1*NN_WW, NN_WW*100.0);
	RooRealVar n_WZ("n_WZ", "n_WZ", N_WZ, 0.1*NN_WZ, NN_WZ*100.0);
	RooRealVar n_ZZ("n_ZZ", "n_ZZ", N_ZZ, 0.1*NN_ZZ, NN_ZZ*100.0);
*/
/*
 	RooRealVar n_ttbar("n_ttbar", "n_ttbar", NN_ttbar, NN_ttbar*0.1, NN_ttbar*100.0);
	RooRealVar n_WJets("n_WJets", "n_WJets", NN_WJets, N_WJets*0.01, N_WJets*1000.00);
	RooRealVar n_DYJets("n_DYJets", "n_DYJets", NN_DYJets, N_DYJets*0.01, N_DYJets*1000.0);
	RooRealVar n_QCD("n_QCD", "n_QCD", NN_QCD, 10, h_data->Integral());
	RooRealVar n_WW("n_WW", "n_WW", NN_WW, 0.1*NN_WW, NN_WW*100.0);
	RooRealVar n_WZ("n_WZ", "n_WZ", NN_WZ, 0.1*NN_WZ, NN_WZ*100.0);
	RooRealVar n_ZZ("n_ZZ", "n_ZZ", NN_ZZ, 0.1*NN_ZZ, NN_ZZ*100.0);
*/
/*
 	RooRealVar n_ttbar("n_ttbar", "n_ttbar", NN_ttbar, NN_ttbar*0.1, NN_ttbar*100.0);
	RooRealVar n_WJets("n_WJets", "n_WJets", NN_WJets, N_WJets*0.01, N_WJets*1000.00);
	RooRealVar n_DYJets("n_DYJets", "n_DYJets", NN_DYJets, N_DYJets*0.01, N_DYJets*1000.0);
	RooRealVar n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	RooRealVar n_WW("n_WW", "n_WW", NN_WW, 0.1*NN_WW, NN_WW*100.0);
	RooRealVar n_WZ("n_WZ", "n_WZ", NN_WZ, 0.1*NN_WZ, NN_WZ*100.0);
	RooRealVar n_ZZ("n_ZZ", "n_ZZ", NN_ZZ, 0.1*NN_ZZ, NN_ZZ*100.0);
*/

 	RooRealVar n_ttbar("n_ttbar", "n_ttbar", NN_ttbar, NN_ttbar*0.1, NN_ttbar*100.0);
	RooRealVar n_WJets("n_WJets", "n_WJets", NN_WJets, N_WJets*0.01, N_WJets*1000.00);
//###	RooRealVar n_DYJets("n_DYJets", "n_DYJets", NN_DYJets, N_DYJets*0.01, N_DYJets*1000.0);
	RooRealVar n_DYJets("n_DYJets", "n_DYJets", 0.1*NN_DYJets, NN_DYJets*0.00005, 10.0*h_data->Integral());
	std::cout << "******BABO******** " << NN_DYJets*0.00005 << " --- " << NN_DYJets << " --- " << 10.0*h_data->Integral() << std::endl;
	RooRealVar n_QCD("n_QCD", "n_QCD", 0.875*h_data->Integral(), 10, h_data->Integral());
	RooRealVar n_TW("n_TW", "n_TW", NN_TW, 0.1*NN_TW, NN_TW*100.0);
	RooRealVar n_WW("n_WW", "n_WW", NN_WW, 0.1*NN_WW, NN_WW*100.0);
	RooRealVar n_WZ("n_WZ", "n_WZ", NN_WZ, 0.1*NN_WZ, NN_WZ*100.0);
	RooRealVar n_ZZ("n_ZZ", "n_ZZ", NN_ZZ, 0.1*NN_ZZ, NN_ZZ*100.0);



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








	switch (opt) {

		case 0:
//opt0
	std::cout << "##### opt 0 #####" << std::endl;
 	n_ttbar.setRange(N_ttbar*1.0, N_ttbar*1.0);
	n_WJets.setRange(N_WJets*1.0, N_WJets*1.00);
	n_DYJets.setRange(N_DYJets*1.0, N_DYJets*1.0);
	n_QCD.setRange(1.0*N_QCD, N_QCD*1.0);
	n_WW.setRange(1.0*N_WW, N_WW*1.0);
	n_WZ.setRange(1.0*N_WZ, N_WZ*1.0);
	n_ZZ.setRange(1.0*N_ZZ, N_ZZ*1.0);
	break;

		case 1:

//opt1
 	std::cout << "##### opt 1 #####" << std::endl;
	n_ttbar.setRange(N_ttbar*0.1, N_ttbar*10);
	n_WJets.setRange(N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange(N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.1*N_WW, N_WW*10);
	n_WZ.setRange(0.1*N_WZ, N_WZ*10);
	n_ZZ.setRange(0.1*N_ZZ, N_ZZ*10);
	break;

	case 2:

//opt2
	std::cout << "##### opt 2 #####" << std::endl;

 	n_ttbar.setRange(N_ttbar*0.1, N_ttbar*10);
	n_WJets.setRange(N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange(N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.95*N_WW, N_WW*1.05);
	n_WZ.setRange(0.95*N_WZ, N_WZ*1.05);
	n_ZZ.setRange(0.95*N_ZZ, N_ZZ*1.05);
	break;

	case 3:

 //opt3
 	std::cout << "##### opt 3 #####" << std::endl;
	n_ttbar.setRange(N_ttbar*0.95, N_ttbar*1.05);
	n_WJets.setRange(N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange(N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.95*N_WW, N_WW*1.05);
	n_WZ.setRange(0.95*N_WZ, N_WZ*1.05);
	n_ZZ.setRange(0.95*N_ZZ, N_ZZ*1.05);
	break;

	case 4:
 //opt4
	std::cout << "##### opt 4 #####" << std::endl;

 	n_ttbar.setRange(NN_ttbar*0.95, NN_ttbar*1.05);
	n_WJets.setRange(N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange(N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.95*NN_WW, NN_WW*1.05);
	n_WZ.setRange(0.95*NN_WZ, NN_WZ*1.05);
	n_ZZ.setRange(0.95*NN_ZZ, NN_ZZ*1.05);
	break;

	case 5:
//opt5
 	std::cout << "##### opt 5 #####" << std::endl;
	n_ttbar.setRange(NN_ttbar*0.1, NN_ttbar*10);
	n_WJets.setRange(N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange(N_DYJets*0.01, N_DYJets*100);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.1*NN_WW, NN_WW*10);
	n_WZ.setRange(0.1*NN_WZ, NN_WZ*10);
	n_ZZ.setRange(0.1*NN_ZZ, NN_ZZ*10);
	break;

	case 6:

//opt6
 	std::cout << "##### opt 6 #####" << std::endl;
	n_ttbar.setRange(NN_ttbar*0.1, NN_ttbar*100);
	n_WJets.setRange(N_WJets*0.01, N_WJets*1000);
	n_DYJets.setRange(N_DYJets*0.01, N_DYJets*1000);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.1*NN_WW, NN_WW*100);
	n_WZ.setRange(0.1*NN_WZ, NN_WZ*100);
	n_ZZ.setRange(0.1*NN_ZZ, NN_ZZ*100);
	break;

	case 7:

//opt7
 	std::cout << "##### opt 7 #####" << std::endl;
	n_ttbar.setRange(NN_ttbar*0.1, NN_ttbar*100);
	n_WJets.setRange(NN_WJets*0.01, NN_WJets*1000);
	n_DYJets.setRange(NN_DYJets*0.01, NN_DYJets*1000);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.1*NN_WW, NN_WW*100);
	n_WZ.setRange(0.1*NN_WZ, NN_WZ*100);
	n_ZZ.setRange(0.1*NN_ZZ, NN_ZZ*100);
	break;

	case 8:
	//opt8
 	std::cout << "##### opt 8 #####" << std::endl;
	n_ttbar.setRange(NN_ttbar*0.95, NN_ttbar*1.05);
	n_WJets.setRange(NN_WJets*0.001, NN_WJets*10000);
	n_DYJets.setRange(NN_DYJets*0.001, NN_DYJets*10000);
	n_QCD.setRange(10, h_data->Integral());
	n_WW.setRange(0.95*NN_WW, NN_WW*1.05);
	n_WZ.setRange(0.95*NN_WZ, NN_WZ*1.05);
	n_ZZ.setRange(0.95*NN_ZZ, NN_ZZ*1.05);
	break;

		case 9:
//opt9
	std::cout << "##### opt 9 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*1.0, NN_ttbar*1.0);
	n_WJets.setRange(NN_WJets*1.0, NN_WJets*1.00);
	n_DYJets.setRange(NN_DYJets*1.0, NN_DYJets*1.0);
	n_QCD.setRange(1, h_data->Integral());
	n_WW.setRange(1.0*NN_WW, NN_WW*1.0);
	n_WZ.setRange(1.0*NN_WZ, NN_WZ*1.0);
	n_ZZ.setRange(1.0*NN_ZZ, NN_ZZ*1.0);
	break;

		case 10:
//opt10
	std::cout << "##### opt 10 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*1.0, NN_ttbar*1.0);
	n_WJets.setRange(NN_WJets*1.0, NN_WJets*1.00);
	n_DYJets.setRange(NN_DYJets*1.0, NN_DYJets*1.0);
	n_QCD.setRange(NN_QCD*1.0, NN_QCD*1.0);
	n_WW.setRange(1.0*NN_WW, NN_WW*1.0);
	n_WZ.setRange(1.0*NN_WZ, NN_WZ*1.0);
	n_ZZ.setRange(1.0*NN_ZZ, NN_ZZ*1.0);
	break;

		case 11:
//opt11, optA
	std::cout << "##### opt 11 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*1.0, NN_ttbar*1.0);
	n_WJets.setRange(NN_WJets*1.0, NN_WJets*1.00);
	n_DYJets.setRange(NN_DYJets*1.0, NN_DYJets*1.0);
	n_QCD.setRange(1, h_data->Integral());
	n_WW.setRange(1.0*NN_WW, NN_WW*1.0);
	n_WZ.setRange(1.0*NN_WZ, NN_WZ*1.0);
	n_ZZ.setRange(1.0*NN_ZZ, NN_ZZ*1.0);
	break;

		case 12:
//opt12
	std::cout << "##### opt 12 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*1.0, NN_ttbar*1.0);
	n_WJets.setRange(NN_WJets*0.1, NN_WJets*10.0);
	n_DYJets.setRange(NN_DYJets*0.1, NN_DYJets*10.0);
	n_QCD.setRange(1, h_data->Integral());
	n_WW.setRange(1.0*NN_WW, NN_WW*1.0);
	n_WZ.setRange(1.0*NN_WZ, NN_WZ*1.0);
	n_ZZ.setRange(1.0*NN_ZZ, NN_ZZ*1.0);
	break;

		case 13:
//opt13
	std::cout << "##### opt 13 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*0.95, NN_ttbar*1.05);
	n_WJets.setRange(NN_WJets*0.1, NN_WJets*10.0);
	n_DYJets.setRange(NN_DYJets*0.1, NN_DYJets*10.0);
	n_QCD.setRange(1, h_data->Integral());
	n_WW.setRange(0.95*NN_WW, NN_WW*1.05);
	n_WZ.setRange(0.95*NN_WZ, NN_WZ*1.05);
	n_ZZ.setRange(0.95*NN_ZZ, NN_ZZ*1.05);
	break;

		case 14:
//opt14
	std::cout << "##### opt 14 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*0.1, NN_ttbar*10.0);
	n_WJets.setRange(NN_WJets*0.01, NN_WJets*1000.0);
	n_DYJets.setRange(NN_DYJets*0.01, NN_DYJets*1000.0);
	n_QCD.setRange(1, h_data->Integral());
	n_WW.setRange(0.1*NN_WW, NN_WW*10.0);
	n_WZ.setRange(0.1*NN_WZ, NN_WZ*10.0);
	n_ZZ.setRange(0.1*NN_ZZ, NN_ZZ*10.0);
	break;

		case 15:
//opt15, optB
	std::cout << "##### opt 15 #####" << std::endl;
 	n_ttbar.setRange(1, h_data->Integral());
	n_WJets.setRange(1, h_data->Integral());
	n_DYJets.setRange(1, h_data->Integral());
	n_QCD.setRange(1, h_data->Integral());
	n_WW.setRange(1, h_data->Integral());
	n_WZ.setRange(1, h_data->Integral());
	n_ZZ.setRange(1, h_data->Integral());
	break;

		case 16:
//opt11, optA
	std::cout << "##### opt 16 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*0.9, NN_ttbar*1.1);
	n_WJets.setRange(NN_WJets*0.9, NN_WJets*1.1);
	n_DYJets.setRange(NN_DYJets*0.9, NN_DYJets*1.1);
	n_QCD.setRange(1, h_data->Integral());
	n_WW.setRange(0.9*NN_WW, NN_WW*1.1);
	n_WZ.setRange(0.9*NN_WZ, NN_WZ*1.1);
	n_ZZ.setRange(0.9*NN_ZZ, NN_ZZ*1.1);
	break;

		case 20:
//
	std::cout << "##### opt 20 #####" << std::endl;
 	n_ttbar.setRange(NN_ttbar*0.9, NN_ttbar*1.1);
	n_WJets.setRange(NN_WJets*0.9, NN_WJets*1.1);
	//###n_DYJets.setRange(NN_DYJets*0.00005, 10.0*h_data->Integral());
	n_DYJets.setRange(NN_DYJets*0.00005, 10.0*h_data->Integral());
	n_QCD.setRange(1, h_data->Integral());
	n_TW.setRange(0.9*NN_TW, NN_TW*1.1);
	n_WW.setRange(0.9*NN_WW, NN_WW*1.1);
	n_WZ.setRange(0.9*NN_WZ, NN_WZ*1.1);
	n_ZZ.setRange(0.9*NN_ZZ, NN_ZZ*1.1);
	std::cout << "******BABO2******** " << NN_DYJets*0.00005 << " --- " << NN_DYJets << " --- " << 0.1*h_data->Integral() << std::endl;
	break;

/*
		case 0:
//opt0
	std::cout << "##### opt 0 #####" << std::endl;
 	n_ttbar.setRange("n_ttbar", N_ttbar*1.0, N_ttbar*1.0);
	n_WJets.setRange("n_WJets", N_WJets*1.0, N_WJets*1.00);
	n_DYJets.setRange("n_DYJets", N_DYJets*1.0, N_DYJets*1.0);
	n_QCD.setRange("n_QCD", 1.0*N_QCD, N_QCD*1.0);
	n_WW.setRange("n_WW", 1.0*N_WW, N_WW*1.0);
	n_WZ.setRange("n_WZ", 1.0*N_WZ, N_WZ*1.0);
	n_ZZ.setRange("n_ZZ", 1.0*N_ZZ, N_ZZ*1.0);
	break;

		case 1:

//opt1
 	std::cout << "##### opt 1 #####" << std::endl;
	n_ttbar.setRange("n_ttbar", N_ttbar*0.1, N_ttbar*10);
	n_WJets.setRange("n_WJets", N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange("n_DYJets", N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.1*N_WW, N_WW*10);
	n_WZ.setRange("n_WZ", 0.1*N_WZ, N_WZ*10);
	n_ZZ.setRange("n_ZZ", 0.1*N_ZZ, N_ZZ*10);
	break;

	case 2:

//opt2
	std::cout << "##### opt 2 #####" << std::endl;

 	n_ttbar.setRange("n_ttbar", N_ttbar*0.1, N_ttbar*10);
	n_WJets.setRange("n_WJets", N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange("n_DYJets", N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.95*N_WW, N_WW*1.05);
	n_WZ.setRange("n_WZ", 0.95*N_WZ, N_WZ*1.05);
	n_ZZ.setRange("n_ZZ", 0.95*N_ZZ, N_ZZ*1.05);
	break;

	case 3:

 //opt3
 	std::cout << "##### opt 3 #####" << std::endl;
	n_ttbar.setRange("n_ttbar", N_ttbar*0.95, N_ttbar*1.05);
	n_WJets.setRange("n_WJets", N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange("n_DYJets", N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.95*N_WW, N_WW*1.05);
	n_WZ.setRange("n_WZ", 0.95*N_WZ, N_WZ*1.05);
	n_ZZ.setRange("n_ZZ", 0.95*N_ZZ, N_ZZ*1.05);
	break;

	case 4:
 //opt4
	std::cout << "##### opt 4 #####" << std::endl;

 	n_ttbar.setRange("n_ttbar", NN_ttbar*0.95, NN_ttbar*1.05);
	n_WJets.setRange("n_WJets", N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange("n_DYJets", N_DYJets*0.1, N_DYJets*10);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.95*NN_WW, NN_WW*1.05);
	n_WZ.setRange("n_WZ", 0.95*NN_WZ, NN_WZ*1.05);
	n_ZZ.setRange("n_ZZ", 0.95*NN_ZZ, NN_ZZ*1.05);
	break;

	case 5:
//opt5
 	std::cout << "##### opt 5 #####" << std::endl;
	n_ttbar.setRange("n_ttbar", NN_ttbar*0.1, NN_ttbar*10);
	n_WJets.setRange("n_WJets", N_WJets*0.01, N_WJets*100);
	n_DYJets.setRange("n_DYJets", N_DYJets*0.01, N_DYJets*100);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.1*NN_WW, NN_WW*10);
	n_WZ.setRange("n_WZ", 0.1*NN_WZ, NN_WZ*10);
	n_ZZ.setRange("n_ZZ", 0.1*NN_ZZ, NN_ZZ*10);
	break;

	case 6:

//opt6
 	std::cout << "##### opt 6 #####" << std::endl;
	n_ttbar.setRange("n_ttbar", NN_ttbar*0.1, NN_ttbar*100);
	n_WJets.setRange("n_WJets", N_WJets*0.01, N_WJets*1000);
	n_DYJets.setRange("n_DYJets", N_DYJets*0.01, N_DYJets*1000);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.1*NN_WW, NN_WW*100);
	n_WZ.setRange("n_WZ", 0.1*NN_WZ, NN_WZ*100);
	n_ZZ.setRange("n_ZZ", 0.1*NN_ZZ, NN_ZZ*100);
	break;

	case 7:

//opt7
 	std::cout << "##### opt 7 #####" << std::endl;
	n_ttbar.setRange("n_ttbar", NN_ttbar*0.1, NN_ttbar*100);
	n_WJets.setRange("n_WJets", NN_WJets*0.01, NN_WJets*1000);
	n_DYJets.setRange("n_DYJets", NN_DYJets*0.01, NN_DYJets*1000);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.1*NN_WW, NN_WW*100);
	n_WZ.setRange("n_WZ", 0.1*NN_WZ, NN_WZ*100);
	n_ZZ.setRange("n_ZZ", 0.1*NN_ZZ, NN_ZZ*100);
	break;

	//opt8
 	std::cout << "##### opt 8 #####" << std::endl;
	n_ttbar.setRange("n_ttbar", NN_ttbar*0.95, NN_ttbar*1.05);
	n_WJets.setRange("n_WJets", NN_WJets*0.001, NN_WJets*10000);
	n_DYJets.setRange("n_DYJets", NN_DYJets*0.001, NN_DYJets*10000);
	n_QCD.setRange("n_QCD", 10, h_data->Integral());
	n_WW.setRange("n_WW", 0.95*NN_WW, NN_WW*1.05);
	n_WZ.setRange("n_WZ", 0.95*NN_WZ, NN_WZ*1.05);
	n_ZZ.setRange("n_ZZ", 0.95*NN_ZZ, NN_ZZ*1.05);
	break;
*/

	}



/*
	switch (opt) {

		case 0:
//opt0
	std::cout << "##### opt 0 #####" << std::endl;
 	n_ttbar("n_ttbar", "n_ttbar", N_ttbar, N_ttbar*1.0, N_ttbar*1.0);
	n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*1.0, N_WJets*1.00);
	n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*1.0, N_DYJets*1.0);
	n_QCD("n_QCD", "n_QCD", N_QCD, 1.0*N_QCD, N_QCD*1.0);
	n_WW("n_WW", "n_WW", N_WW, 1.0*N_WW, N_WW*1.0);
	n_WZ("n_WZ", "n_WZ", N_WZ, 1.0*N_WZ, N_WZ*1.0);
	n_ZZ("n_ZZ", "n_ZZ", N_ZZ, 1.0*N_ZZ, N_ZZ*1.0);
	break;

		case 1:

//opt1
 	std::cout << "##### opt 1 #####" << std::endl;
	n_ttbar("n_ttbar", "n_ttbar", N_ttbar, N_ttbar*0.1, N_ttbar*10);
	n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*100);
	n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.1, N_DYJets*10);
	n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	n_WW("n_WW", "n_WW", N_WW, 0.1*N_WW, N_WW*10);
	n_WZ("n_WZ", "n_WZ", N_WZ, 0.1*N_WZ, N_WZ*10);
	n_ZZ("n_ZZ", "n_ZZ", N_ZZ, 0.1*N_ZZ, N_ZZ*10);
	break;

	case 2:

//opt2
	std::cout << "##### opt 2 #####" << std::endl;

 	n_ttbar("n_ttbar", "n_ttbar", N_ttbar, N_ttbar*0.1, N_ttbar*10);
	n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*100);
	n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.1, N_DYJets*10);
	n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	n_WW("n_WW", "n_WW", N_WW, 0.95*N_WW, N_WW*1.05);
	n_WZ("n_WZ", "n_WZ", N_WZ, 0.95*N_WZ, N_WZ*1.05);
	n_ZZ("n_ZZ", "n_ZZ", N_ZZ, 0.95*N_ZZ, N_ZZ*1.05);
	break;

	case 3:

 //opt3
 	std::cout << "##### opt 3 #####" << std::endl;
	n_ttbar("n_ttbar", "n_ttbar", N_ttbar, N_ttbar*0.95, N_ttbar*1.05);
	n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*100);
	n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.1, N_DYJets*10);
	n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	n_WW("n_WW", "n_WW", N_WW, 0.95*N_WW, N_WW*1.05);
	n_WZ("n_WZ", "n_WZ", N_WZ, 0.95*N_WZ, N_WZ*1.05);
	n_ZZ("n_ZZ", "n_ZZ", N_ZZ, 0.95*N_ZZ, N_ZZ*1.05);
	break;

	case 4:
 //opt4
	std::cout << "##### opt 4 #####" << std::endl;

 	n_ttbar("n_ttbar", "n_ttbar", NN_ttbar, NN_ttbar*0.95, NN_ttbar*1.05);
	n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*100);
	n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.1, N_DYJets*10);
	n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	n_WW("n_WW", "n_WW", NN_WW, 0.95*NN_WW, NN_WW*1.05);
	n_WZ("n_WZ", "n_WZ", NN_WZ, 0.95*NN_WZ, NN_WZ*1.05);
	n_ZZ("n_ZZ", "n_ZZ", NN_ZZ, 0.95*NN_ZZ, NN_ZZ*1.05);
	break;

	case 5:
//opt5
 	std::cout << "##### opt 5 #####" << std::endl;
	n_ttbar("n_ttbar", "n_ttbar", NN_ttbar, NN_ttbar*0.1, NN_ttbar*10);
	n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*100);
	n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.01, N_DYJets*100);
	n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	n_WW("n_WW", "n_WW", NN_WW, 0.1*NN_WW, NN_WW*10);
	n_WZ("n_WZ", "n_WZ", NN_WZ, 0.1*NN_WZ, NN_WZ*10);
	n_ZZ("n_ZZ", "n_ZZ", NN_ZZ, 0.1*NN_ZZ, NN_ZZ*10);
	break;

	case 6:

//opt6
 	std::cout << "##### opt 6 #####" << std::endl;
	n_ttbar("n_ttbar", "n_ttbar", NN_ttbar, NN_ttbar*0.1, NN_ttbar*100);
	n_WJets("n_WJets", "n_WJets", N_WJets, N_WJets*0.01, N_WJets*1000);
	n_DYJets("n_DYJets", "n_DYJets", N_DYJets, N_DYJets*0.01, N_DYJets*1000);
	n_QCD("n_QCD", "n_QCD", N_QCD, 10, h_data->Integral());
	n_WW("n_WW", "n_WW", NN_WW, 0.1*NN_WW, NN_WW*100);
	n_WZ("n_WZ", "n_WZ", NN_WZ, 0.1*NN_WZ, NN_WZ*100);
	n_ZZ("n_ZZ", "n_ZZ", NN_ZZ, 0.1*NN_ZZ, NN_ZZ*100);
	break;
	
	}
*/
	RooAddPdf model( "model","model",RooArgList(*pdf_QCD, *pdf_WJets, *pdf_DYJets, *pdf_ttbar, *pdf_TW, *pdf_WW, *pdf_WZ, *pdf_ZZ), RooArgList(n_QCD, n_WJets, n_DYJets, n_ttbar, n_TW, n_WW, n_WZ, n_ZZ) );
   // RooAddPdf model( "model","model", RooArgList(*pdf_ttbar, *pdf_WJets, *pdf_QCD), RooArgList(n_ttbar, n_WJets, n_QCD) );

   // RooFitResult* r = pdf_WJets->fitTo( *RooHist_data, Save() );
   RooFitResult* r = model.fitTo( *RooHist_data, Save() );



   std::cout << "########################" << std::endl;
   std::cout << "n_ttbar/N_ttbar: " << n_ttbar.getVal()/N_ttbar << std::endl;
   std::cout << "n_DYJets/N_DYJets: " << n_DYJets.getVal()/N_DYJets << std::endl;
   std::cout << "n_WJets/N_WJets: " << n_WJets.getVal()/N_WJets << std::endl;
   std::cout << "n_QCD/N_QCD: " << n_QCD.getVal()/N_QCD << std::endl;
	std::cout << "n_TW/N_TW: " << n_TW.getVal()/N_TW << std::endl;
   std::cout << "n_WW/N_WW: " << n_WW.getVal()/N_WW << std::endl;
   std::cout << "n_WZ/N_WZ: " << n_WZ.getVal()/N_WZ << std::endl;
   std::cout << "n_ZZ/N_ZZ: " << n_ZZ.getVal()/N_ZZ << std::endl;
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
	// pdf_ttbar->plotOn( frame1, LineColor(kOrange) );
	// pdf_WJets->plotOn(frame1, LineColor(kGreen) );
	RooHist_data->plotOn(frame1, DataError(RooAbsData::SumW2));
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar,pdf_DYJets,pdf_WJets,pdf_QCD"), LineColor(0), FillColor(7), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar,pdf_DYJets,pdf_WJets"), LineColor(0), FillColor(4), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar,pdf_DYJets"), LineColor(0), FillColor(2), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW,pdf_ttbar"), LineColor(0), FillColor(3), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW,pdf_WW"), LineColor(0), FillColor(13), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ,pdf_TW"), LineColor(0), FillColor(15), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ,pdf_WZ"), LineColor(0), FillColor(14), DrawOption("F") );
	model.plotOn(frame1, Components("pdf_ZZ"), LineColor(0), FillColor(15), DrawOption("F") );
	//model.plotOn(frame1, Components("pdf_WJets"), LineColor(0), FillColor(kGreen), DrawOption("F") );
	//model.plotOn(frame1, Components("pdf_QCD,pdf_WJets,pdf_ttbar"), LineColor(kRed), FillColor(kRed), DrawOption("F") );
	//model.plotOn(frame1, Components("pdf_QCD,pdf_WJets"), LineColor(kBlue), FillColor(kBlue), DrawOption("F") );
	//model.plotOn(frame1, Components("pdf_QCD"), LineColor(kGreen), FillColor(kGreen), DrawOption("F") );
	//model.plotOn(frame1, LineColor(1) );
	RooHist_data->plotOn(frame1, DataError(RooAbsData::SumW2));
	//model.paramOn(frame1, Layout(0.65,0.9,0.9) );
	frame1->Draw();
   r->Print();

	std::cout << "########################" << std::endl;
/*
	std::cout << "n_ttbar*N_ttbar: " << n_ttbar.getVal()*N_ttbar << std::endl;
	std::cout << "n_DYJets*N_DYJets: " << n_DYJets.getVal()*N_DYJets << std::endl;
	std::cout << "n_WJets*N_WJets: " << n_WJets.getVal()*N_WJets << std::endl;
	std::cout << "n_QCD*N_QCD: " << n_QCD.getVal()*N_QCD << std::endl;
	std::cout << "n_WW*N_WW: " << n_WW.getVal()*N_WW << std::endl;
	std::cout << "n_WZ*N_WZ: " << n_WZ.getVal()*N_WZ << std::endl;
	std::cout << "n_ZZ*N_ZZ: " << n_ZZ.getVal()*N_ZZ << std::endl;
*/
	std::cout << "########################" << std::endl;

	TLegend *leg1 = new TLegend(0.58,0.7,.96,.98);
	leg1->SetFillColor(kWhite);
	leg1->SetLineColor(kWhite);
	leg1->AddEntry(frame1->nameOf(0),Form("Data : %.2f",h_data->Integral()), "EP");
	leg1->AddEntry(frame1->nameOf(1),Form("QCD : %.2f (%.2f)",n_QCD.getVal(),n_QCD.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(2),Form("WJets : %.2f (%.2f)",n_WJets.getVal(),n_WJets.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(3),Form("DYJets : %.2f (%.2f)",n_DYJets.getVal(),n_DYJets.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(4),Form("ttbar : %.2f (%.2f)",n_ttbar.getVal(),n_ttbar.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(5),Form("TW : %.2f (%.2f)",n_TW.getVal(),n_TW.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(5),Form("WW : %.2f (%.2f)",n_WW.getVal(),n_WW.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(6),Form("WZ : %.2f (%.2f)",n_WZ.getVal(),n_WZ.getVal()/h_data->Integral()),"F");
	leg1->AddEntry(frame1->nameOf(7),Form("ZZ : %.2f (%.2f)",n_ZZ.getVal(),n_ZZ.getVal()/h_data->Integral()),"F");
	leg1->Draw();


	frame1->GetYaxis()->SetTitle("Entry");
	frame1->GetXaxis()->SetLabelSize(0);
	frame1->SetMinimum(0.5);
	frame1->SetMaximum(50000000.0);


	TH1D *h_MC = (TH1D*)model.createHistogram("h_MC", obs);
	// h_MC->Sumw2();
	// TCanvas *c_MC = new TCanvas("c_MC", "", 700, 700);
	
	Double_t Ndata = h_data->Integral();
	Double_t NMC = h_MC->Integral();
	h_MC->Scale(Ndata / NMC );
	// cout << "# data: " << Ndata << endl;
	// h_MC->Draw(); 
	// h_data->Draw("SAMEEP");
	RooAbsReal *chi2 = model.createChi2(*RooHist_data);
	cout << "chi2: " << chi2->getVal() << endl;
	cout << "Normalized chi2: " << chi2->getVal() / ((Double_t)h_data->GetNbinsX()) << endl;
	cout << "#### UNTIL NOW : " << category << endl;

	TPaveText *pt_norchi2 = new TPaveText(0.05,1000000,0.53,30000000);
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

	//leg1->Draw();

	//###c_fit->Print("print/fit"+category+".pdf");
/*
   c_fit->SaveAs("print/fit"+category+"_opt0_wPbPDYJets.pdf");
	TFile* fout = new TFile("print/fitresult_"+category+"_opt0_wPbPDYJets.root","recreate");
*/
	TString optst=Form("opt%d",opt);
	std::cout << "########### optst : " << optst <<std::endl;
 	TString QCDoptst=Form("QCDopt%d",QCDopt);
	std::cout << "########### QCDoptst : " << QCDoptst <<std::endl;
    
	//###c_fit->SaveAs("print/fit_v2"+category+"_"+optst+"_"+QCDoptst+"_QCDDATASS2_woPbPDYJets.pdf");
	//######c_fit->SaveAs("print/fit_v2"+category+"_"+optst+"_"+QCDoptst+"_QCDDATASS2_QCDin0p875Data.pdf");
	//##########c_fit->SaveAs("print/fit_v2"+category+"_"+optst+"_"+QCDoptst+"_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.pdf");
	//############c_fit->SaveAs("print_opttest/Fit"+category+"_"+optst+"_"+QCDoptst+"_hist"+opttest.Data()+MuonIDopt.Data()+"_QCDin0p875Data.pdf");
	c_fit->SaveAs("print_20180814/Fit"+category+"_"+optst+"_"+QCDoptst+"_hist"+opttest.Data()+MuonIDopt.Data()+"_QCDin0p875Data.pdf");


	//c_fit->SaveAs("print/fit"+category+"_opt0_woPbPDYJets_nology.pdf");
	c_fit->Close();
	//###TFile* fout = new TFile("histograms/histFRFit_v2_"+category+"_"+optst+"_"+QCDoptst+"_QCDDATASS2_woPbPDYJets.root","recreate");
	//#########TFile* fout = new TFile("histograms/histFRFit_v2_"+category+"_"+optst+"_"+QCDoptst+"_QCDDATASS2_QCDin0p875Data.root","recreate");
	//##############TFile* fout = new TFile("histograms/histFRFit_v2_"+category+"_"+optst+"_"+QCDoptst+"_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.root","recreate");
	//##################TFile* fout = new TFile("histograms_opttest/histFRFit"+category+"_"+optst+"_"+QCDoptst+"_hist"+opttest.Data()+MuonIDopt.Data()+"_QCDin0p875Data.root","recreate");

	TFile* fout = new TFile("histograms_20180814/histFRFit"+category+"_"+optst+"_"+QCDoptst+"_hist"+opttest.Data()+MuonIDopt.Data()+"_QCDin0p875Data.root","recreate");
	fout->cd();	
	TH1D* h_fitNch = new TH1D("h_fitNch","",12,0,12);
	h_fitNch->SetBinContent(1,n_DYJets.getVal());
 	h_fitNch->SetBinContent(2,n_QCD.getVal());
  	h_fitNch->SetBinContent(3,n_WJets.getVal());
	h_fitNch->SetBinContent(4,n_TW.getVal());
   h_fitNch->SetBinContent(5,n_WW.getVal());
	h_fitNch->SetBinContent(6,n_WZ.getVal());
	h_fitNch->SetBinContent(7,n_ZZ.getVal());
	h_fitNch->SetBinContent(8,n_ttbar.getVal());
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

