#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <THStack.h>
#include <TMath.h>
#include <TText.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TColor.h>
#include <TLatex.h>
#include <TEfficiency.h>
#include <TFrame.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include "../../interface/tdrstyle.C"
#include "../../interface/CMS_lumi.C"
#include "../../interface/defs.h"
using namespace std;
using namespace DYana;

void setDataHist(TH1D* hist);
void setMCHist(TH1D* hist, const int& color);
void setFRHist(TH1D* hist);
TH1D* FRByTemplate(TH1D** numerator, TH1D** denominator);
TH1D* FRByRatio(TH1D** numerator, TH1D** denominator);
void FRStore(TFile* g, TH1D** numerator, TH1D** denominator);

TH1D* FRCheck(TH1D** numerator, TH1D** denominator);

TH1D* tight_QCD;
TH1D* loose_QCD;
TH1D* ratio_QCD;

TH1D* tight_allch;
TH1D* loose_allch;
TH1D* ratio_allch;

TH1D* tight_Data;
TH1D* loose_Data;
TH1D* ratio_Data;

TH1D* test_barrel_denominator_QCDtemplate;
TH1D* test_barrel_numerator_QCDtemplate;


//void estimateFR_opttest_v20190901(TString indir, TString rmk,TString rmk1,TString rmk2,int QCDopt=2, TString indir2="20190306v9_iso0_1_HFminus", TString indir3="20190402v11_iso0_1_HFminus", TString rmkf="rmkf", int FRopt=1) {
void estimateFR_opttest_v20190918_ptincl(TString indir="20190906v62_60ptincl_iso0_0p2_nominalfixed",TString rmk="opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12",TString rmk1="reltrkisoR03muptlt10",TString rmk2="L3Mu12",int QCDopt=2,TString indir2="20190901v50_iso0_1_nominal",TString indir3="20190906v62_60ptincl_iso0_0p2_nominalfixed",TString rmkf="reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_FRopt101",int FRopt=101) {

	TFile* file[NSamples+2];
	TFile* fileSS1;
	TFile* fileSS2;

	//TFile* fin_fitNch_barrel = new TFile(Form("histogramsFit_%s/histFRFit_barrel_%s_QCDin0p875Data.root",indir.Data(),rmk.Data()),"read");
	//TFile* fin_fitNch_endcap = new TFile(Form("histogramsFit_%s/histFRFit_endcap_%s_QCDin0p875Data.root",indir.Data(),rmk.Data()),"read");

	TFile* fin_fitNch_barrel = new TFile(Form("histogramsFit_%s/histFRFit_barrel_%s.root",indir.Data(),rmk.Data()),"read");
	TFile* fin_fitNch_endcap = new TFile(Form("histogramsFit_%s/histFRFit_endcap_%s.root",indir.Data(),rmk.Data()),"read");


	//histogramsFit_20190128/histFRFit_endcap_opt15_QCDopt2_reltrkisoR03_highPtMuonID_L1DoubleMu0_QCDin0p875Data.root

	TH1D* h_fitNch_barrel = (TH1D*)fin_fitNch_barrel->Get("h_fitNch_barrel");
	TH1D* h_fitNch_endcap = (TH1D*)fin_fitNch_endcap->Get("h_fitNch_endcap");
	/*
		for (int i=0; i<ALL; i++) file[i] = new TFile(PathFRHistos(static_cast<SampleTag>(i)));
		file[QCD] = new TFile(PathFRHistos(QCD));
		*/


	TH1D* denominator_pt_fit_barrel[NSamples+2];
	TH1D* denominator_pt_fit_endcap[NSamples+2];
	TH1D* denominator_pt_xsec_barrel[NSamples+2];
	TH1D* denominator_pt_xsec_endcap[NSamples+2];

	TH1D* numerator_pt_barrel[NSamples+2];
	TH1D* numerator_pt_endcap[NSamples+2];

	TH1D* denominator_barrel[NSamples+2];
	TH1D* denominator_endcap[NSamples+2];

	double norm_all[NSamples+2];
	double norm_fit_barrel[NSamples+2];
	double norm_fit_endcap[NSamples+2];

	double norm_pt_all[NSamples+2];
	double norm_pt_fit_barrel[NSamples+2];
	double norm_pt_fit_endcap[NSamples+2];


	// Stack is not working yet..need to solve
	THStack *hs_fit = new THStack("hs_fit","fit stacked");
	THStack *hs_xsec = new THStack("hs_xsec","xsec stacked");
	THStack *hs_data = new THStack("hs_data","data stacked");

	for (int i=0; i<=QCD; i++) {
		if (i==ALL) continue;
		SampleTag tag = static_cast<SampleTag>(i);
		file[i] = new TFile(Form("histograms_%s/histFR_%s_%s_%s.root",indir2.Data(),rmk1.Data(),Name(tag),rmk2.Data()));
		//histograms_20190128/histFR_reltrkisoR03_ZZ_highPtMuonID_L1DoubleMu0.root

		if (i==QCD && QCDopt==2) {
			fileSS1 = new TFile(Form("histograms_%s/histFR_SS%s_Data1_%s.root",indir2.Data(),rmk1.Data(),rmk2.Data()));
			fileSS2 = new TFile(Form("histograms_%s/histFR_SS%s_Data2_%s.root",indir2.Data(),rmk1.Data(),rmk2.Data()));
			denominator_pt_fit_barrel[i] = (TH1D*)fileSS1->Get("denominator_pt_barrel")->Clone("denominator_pt_fit_barrel"+TString(Name(tag)));
			denominator_pt_xsec_barrel[i] = (TH1D*)fileSS1->Get("denominator_pt_barrel")->Clone("denominator_pt_xsec_barrel"+TString(Name(tag)));
			denominator_barrel[i] = (TH1D*)fileSS1->Get("denominator_barrel")->Clone("denominator_barrel"+TString(Name(tag)));
			numerator_pt_barrel[i] = (TH1D*)fileSS1->Get("numerator_pt_barrel")->Clone("numerator_pt_barrel"+TString(Name(tag)));
			denominator_pt_fit_endcap[i] = (TH1D*)fileSS1->Get("denominator_pt_endcap")->Clone("denominator_pt_fit_endcap"+TString(Name(tag)));
			denominator_pt_xsec_endcap[i] = (TH1D*)fileSS1->Get("denominator_pt_endcap")->Clone("denominator_pt_xsec_endcap"+TString(Name(tag)));
			denominator_endcap[i] = (TH1D*)fileSS1->Get("denominator_endcap")->Clone("denominator_endcap"+TString(Name(tag)));
			numerator_pt_endcap[i] = (TH1D*)fileSS1->Get("numerator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(tag)));

			denominator_pt_fit_barrel[i]->Add((TH1D*)fileSS2->Get("denominator_pt_barrel"));
			denominator_pt_xsec_barrel[i]->Add((TH1D*)fileSS2->Get("denominator_pt_barrel"));
			denominator_barrel[i]->Add((TH1D*)fileSS2->Get("denominator_barrel"));
			numerator_pt_barrel[i]->Add((TH1D*)fileSS2->Get("numerator_pt_barrel"));
			denominator_pt_fit_endcap[i]->Add((TH1D*)fileSS2->Get("denominator_pt_endcap"));
			denominator_pt_xsec_endcap[i]->Add((TH1D*)fileSS2->Get("denominator_pt_endcap"));
			denominator_endcap[i]->Add((TH1D*)fileSS2->Get("denominator_endcap"));
			numerator_pt_endcap[i]->Add((TH1D*)fileSS2->Get("numerator_pt_endcap"));
		}
		else {
			denominator_pt_fit_barrel[i] = (TH1D*)file[i]->Get("denominator_pt_barrel")->Clone("denominator_pt_fit_barrel"+TString(Name(tag)));
			denominator_pt_xsec_barrel[i] = (TH1D*)file[i]->Get("denominator_pt_barrel")->Clone("denominator_pt_xsec_barrel"+TString(Name(tag)));
			denominator_barrel[i] = (TH1D*)file[i]->Get("denominator_barrel")->Clone("denominator_barrel"+TString(Name(tag)));
			numerator_pt_barrel[i] = (TH1D*)file[i]->Get("numerator_pt_barrel")->Clone("numerator_pt_barrel"+TString(Name(tag)));
			denominator_pt_fit_endcap[i] = (TH1D*)file[i]->Get("denominator_pt_endcap")->Clone("denominator_pt_fit_endcap"+TString(Name(tag)));
			denominator_pt_xsec_endcap[i] = (TH1D*)file[i]->Get("denominator_pt_endcap")->Clone("denominator_pt_xsec_endcap"+TString(Name(tag)));
			denominator_endcap[i] = (TH1D*)file[i]->Get("denominator_endcap")->Clone("denominator_endcap"+TString(Name(tag)));
			numerator_pt_endcap[i] = (TH1D*)file[i]->Get("numerator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(tag)));
		}

		if(IsData(tag)) {
			setDataHist( denominator_pt_fit_barrel[i] );
			setDataHist( denominator_pt_xsec_barrel[i] );
			setDataHist( denominator_barrel[i] );
			setDataHist( numerator_pt_barrel[i] );
			setDataHist( denominator_pt_fit_endcap[i] );
			setDataHist( denominator_pt_xsec_endcap[i] );
			setDataHist( denominator_endcap[i] );
			setDataHist( numerator_pt_endcap[i] );
		}
		else {
			setMCHist( denominator_pt_fit_barrel[i], i );
			setMCHist( denominator_pt_xsec_barrel[i], i );
			setMCHist( denominator_barrel[i], i );
			setMCHist( numerator_pt_barrel[i], i );
			setMCHist( denominator_pt_fit_endcap[i], i );
			setMCHist( denominator_pt_xsec_endcap[i], i );
			setMCHist( denominator_endcap[i], i );
			setMCHist( numerator_pt_endcap[i], i );

			if ((i==DYMuMu1030_PbP || i==DYMuMu30_PbP)) {
				norm_all[i] = (Xsec(tag)*lumi_part1)/Nevts(tag);
			}
			else if ((i==DYMuMu1030 || i==DYMuMu30)){
				norm_all[i] = (Xsec(tag)*lumi_part2)/Nevts(tag);
			}
			else if (i==QCD && QCDopt==2) {
				//norm_all[i] = 1.0;
				norm_all[i] = (Xsec(tag)*lumi_all)/Nevts(tag);
			}
			else {
				norm_all[i] = (Xsec(tag)*lumi_all)/Nevts(tag);
			}
			denominator_pt_fit_barrel[i]->Scale(norm_all[i]);
			denominator_pt_xsec_barrel[i]->Scale(norm_all[i]);
			denominator_barrel[i]->Scale(norm_all[i]);
			numerator_pt_barrel[i]->Scale(norm_all[i]);
			denominator_pt_fit_endcap[i]->Scale(norm_all[i]);
			denominator_pt_xsec_endcap[i]->Scale(norm_all[i]);
			denominator_endcap[i]->Scale(norm_all[i]);
			numerator_pt_endcap[i]->Scale(norm_all[i]);
		}
	}

	test_barrel_denominator_QCDtemplate = (TH1D*)denominator_pt_fit_barrel[QCD]->Clone();
   test_barrel_numerator_QCDtemplate = (TH1D*)numerator_pt_barrel[QCD]->Clone();


	// NB: the numbers below are the output of fitTemplate.cc

	double intDY_barrel=0, intDY_endcap=0;
	TH1D* denominator_barrel_DYJets;
	TH1D* denominator_endcap_DYJets;
	TH1D* tmp_barrel=(TH1D*)denominator_barrel[DYTauTau1030]->Clone();
	TH1D* tmp_endcap=(TH1D*)denominator_endcap[DYTauTau1030]->Clone();
	//       tmp_barrel->Scale(norm_all[DYTauTau1030]);
	//       tmp_endcap->Scale(norm_all[DYTauTau1030]);
	denominator_barrel_DYJets=(TH1D*)tmp_barrel->Clone();
	denominator_endcap_DYJets=(TH1D*)tmp_endcap->Clone();
	intDY_barrel+=denominator_barrel_DYJets->Integral();
	intDY_endcap+=denominator_endcap_DYJets->Integral();
	for (int i=DYTauTau30; i<=DYMuMu30_PbP; i++) {
		SampleTag tag = static_cast<SampleTag>(i);

		//		 if (i==DYMuMu1030_PbP || i==DYMuMu30_PbP) continue;
		tmp_barrel=(TH1D*)denominator_barrel[i]->Clone();
		tmp_endcap=(TH1D*)denominator_endcap[i]->Clone();
		/*
			if ((i==DYMuMu1030_PbP || i==DYMuMu30_PbP)) {
			tmp_barrel->Scale((Xsec(tag)*lumi_part1)/Nevts(tag));
			tmp_endcap->Scale((Xsec(tag)*lumi_part1)/Nevts(tag));
			}
			else if ((i==DYMuMu1030 || i==DYMuMu30)){
			tmp_barrel->Scale((Xsec(tag)*lumi_part2)/Nevts(tag));
			tmp_endcap->Scale((Xsec(tag)*lumi_part2)/Nevts(tag));
			}
			else {
			tmp_barrel->Scale((Xsec(tag)*lumi_all)/Nevts(tag));
			tmp_endcap->Scale((Xsec(tag)*lumi_all)/Nevts(tag));
			}
			*/
		//tmp_barrel->Scale(norm_all[i]);
		//tmp_endcap->Scale(norm_all[i]);
		denominator_barrel_DYJets->Add(tmp_barrel);
		denominator_endcap_DYJets->Add(tmp_endcap);
		//	    intDY_barrel+=denominator_barrel_DYJets->Integral();
		//     intDY_endcap+=denominator_endcap_DYJets->Integral();
		intDY_barrel+=tmp_barrel->Integral();
		intDY_endcap+=tmp_endcap->Integral();
	}
	double intDY_barrel_pt=0, intDY_endcap_pt=0;
	TH1D* denominator_barrel_DYJets_pt;
	TH1D* denominator_endcap_DYJets_pt;
	TH1D* tmp_barrel_pt=(TH1D*)denominator_pt_fit_barrel[DYTauTau1030]->Clone();
	TH1D* tmp_endcap_pt=(TH1D*)denominator_pt_fit_endcap[DYTauTau1030]->Clone();
	//       tmp_barrel_pt->Scale(norm_all[DYTauTau1030]);
	//       tmp_endcap_pt->Scale(norm_all[DYTauTau1030]);
	denominator_barrel_DYJets_pt=(TH1D*)tmp_barrel_pt->Clone();
	denominator_endcap_DYJets_pt=(TH1D*)tmp_endcap_pt->Clone();
	intDY_barrel_pt+=denominator_barrel_DYJets_pt->Integral();
	intDY_endcap_pt+=denominator_endcap_DYJets_pt->Integral();
	for (int i=DYTauTau30; i<=DYMuMu30_PbP; i++) {
		SampleTag tag = static_cast<SampleTag>(i);

		//		 if (i==DYMuMu1030_PbP || i==DYMuMu30_PbP) continue;
		tmp_barrel_pt=(TH1D*)denominator_pt_fit_barrel[i]->Clone();
		tmp_endcap_pt=(TH1D*)denominator_pt_fit_endcap[i]->Clone();
		/*
			if ((i==DYMuMu1030_PbP || i==DYMuMu30_PbP)) {
			tmp_barrel_pt->Scale((Xsec(tag)*lumi_part1)/Nevts(tag));
			tmp_endcap_pt->Scale((Xsec(tag)*lumi_part1)/Nevts(tag));
			}
			else if ((i==DYMuMu1030 || i==DYMuMu30)){
			tmp_barrel_pt->Scale((Xsec(tag)*lumi_part2)/Nevts(tag));
			tmp_endcap_pt->Scale((Xsec(tag)*lumi_part2)/Nevts(tag));
			}
			else {
			tmp_barrel_pt->Scale((Xsec(tag)*lumi_all)/Nevts(tag));
			tmp_endcap_pt->Scale((Xsec(tag)*lumi_all)/Nevts(tag));
			}
			*/
		//tmp_barrel->Scale(norm_all[i]);
		//tmp_endcap->Scale(norm_all[i]);
		denominator_barrel_DYJets_pt->Add(tmp_barrel_pt);
		denominator_endcap_DYJets_pt->Add(tmp_endcap_pt);
		//	    intDY_barrel+=denominator_barrel_DYJets->Integral();
		//     intDY_endcap+=denominator_endcap_DYJets->Integral();
		intDY_barrel_pt+=tmp_barrel_pt->Integral();
		intDY_endcap_pt+=tmp_endcap_pt->Integral();
	}

	double intWJets_barrel=0, intWJets_endcap=0;
	TH1D* denominator_barrel_WJets;
	TH1D* denominator_endcap_WJets;
	TH1D* tmp_barrel_WJets=(TH1D*)denominator_barrel[WpMu]->Clone();
	TH1D* tmp_endcap_WJets=(TH1D*)denominator_endcap[WpMu]->Clone();
	//       tmp_barrel_WJets->Scale(norm_all[WpMu]);
	//       tmp_endcap_WJets->Scale(norm_all[WpMu]);
	denominator_barrel_WJets=(TH1D*)tmp_barrel_WJets->Clone();
	denominator_endcap_WJets=(TH1D*)tmp_endcap_WJets->Clone();
	//	    intDY_barrel+=denominator_barrel_WJets->Integral();
	//     intDY_endcap+=denominator_endcap_WJets->Integral();
	intWJets_barrel+=denominator_barrel_WJets->Integral();
	intWJets_endcap+=denominator_endcap_WJets->Integral();

	for (int i=WmMu; i<=WmTau; i++) {
		tmp_barrel_WJets=(TH1D*)denominator_barrel[i]->Clone();
		tmp_endcap_WJets=(TH1D*)denominator_endcap[i]->Clone();
		//       tmp_barrel_WJets->Scale(norm_all[i]);
		//       tmp_endcap_WJets->Scale(norm_all[i]);
		denominator_barrel_WJets->Add(tmp_barrel_WJets);
		denominator_endcap_WJets->Add(tmp_endcap_WJets);
		//	    intDY_barrel+=denominator_barrel_WJets->Integral();
		//     intDY_endcap+=denominator_endcap_WJets->Integral();
		intWJets_barrel+=tmp_barrel_WJets->Integral();
		intWJets_endcap+=tmp_endcap_WJets->Integral();
	}

	double intWJets_barrel_pt=0, intWJets_endcap_pt=0;
	TH1D* denominator_barrel_WJets_pt;
	TH1D* denominator_endcap_WJets_pt;
	TH1D* tmp_barrel_WJets_pt=(TH1D*)denominator_pt_fit_barrel[WpMu]->Clone();
	TH1D* tmp_endcap_WJets_pt=(TH1D*)denominator_pt_fit_endcap[WpMu]->Clone();
	//       tmp_barrel_WJets_pt->Scale(norm_all[WpMu]);
	//       tmp_endcap_WJets_pt->Scale(norm_all[WpMu]);
	denominator_barrel_WJets_pt=(TH1D*)tmp_barrel_WJets_pt->Clone();
	denominator_endcap_WJets_pt=(TH1D*)tmp_endcap_WJets_pt->Clone();
	//	    intDY_barrel+=denominator_barrel_WJets->Integral();
	//     intDY_endcap+=denominator_endcap_WJets->Integral();
	intWJets_barrel_pt+=denominator_barrel_WJets_pt->Integral();
	intWJets_endcap_pt+=denominator_endcap_WJets_pt->Integral();

	for (int i=WmMu; i<=WmTau; i++) {
		tmp_barrel_WJets_pt=(TH1D*)denominator_pt_fit_barrel[i]->Clone();
		tmp_endcap_WJets_pt=(TH1D*)denominator_pt_fit_endcap[i]->Clone();
		//       tmp_barrel_WJets_pt->Scale(norm_all[i]);
		//       tmp_endcap_WJets_pt->Scale(norm_all[i]);
		denominator_barrel_WJets_pt->Add(tmp_barrel_WJets_pt);
		denominator_endcap_WJets_pt->Add(tmp_endcap_WJets_pt);
		//	    intDY_barrel+=denominator_barrel_WJets->Integral();
		//     intDY_endcap+=denominator_endcap_WJets->Integral();
		intWJets_barrel_pt+=tmp_barrel_WJets_pt->Integral();
		intWJets_endcap_pt+=tmp_endcap_WJets_pt->Integral();
	}

	double dataall_barrel_integ=denominator_barrel[Data1]->Integral()+denominator_barrel[Data2]->Integral();
	double dataall_pt_barrel_integ=denominator_pt_fit_barrel[Data1]->Integral()+denominator_pt_fit_barrel[Data2]->Integral();
	double dataall_endcap_integ=denominator_endcap[Data1]->Integral()+denominator_endcap[Data2]->Integral();
	double dataall_pt_endcap_integ=denominator_pt_fit_endcap[Data1]->Integral()+denominator_pt_fit_endcap[Data2]->Integral();


	double sum_of_fitNch_barrel = 0.0;
	double sum_of_fitNch_endcap = 0.0;

	for (int i=1;i<9;i++) {
		sum_of_fitNch_barrel+=h_fitNch_barrel->GetBinContent(i);
		sum_of_fitNch_endcap+=h_fitNch_endcap->GetBinContent(i);
		std::cout << "################OK?######################"<< std::endl;

	}

	//norm_fit_barrel[TT] = h_fitNch_barrel->GetBinContent(8)/denominator_barrel[TT]->Integral();
	//###	norm_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/denominator_barrel[TT]->Integral();
	//###	norm_pt_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/denominator_pt_fit_barrel[TT]->Integral();
	norm_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/sum_of_fitNch_barrel)*dataall_barrel_integ/denominator_barrel[TT]->Integral();
	norm_pt_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/denominator_pt_fit_barrel[TT]->Integral();


	std::cout << "--- h_fitNch_barrel->GetBinContent(8): " << h_fitNch_barrel->GetBinContent(8) << std::endl;
	std::cout << "--- h_fitNch_barrel->GetBinContent(9) : " << h_fitNch_barrel->GetBinContent(9) << std::endl;
	std::cout << "dataall_barrel_integ : " << dataall_barrel_integ << std::endl;
	std::cout << "denominator_barrel[TT]->Integral() : " << denominator_barrel[TT]->Integral() << std::endl;
	std::cout << "norm_fit_barrel[TT] : " << norm_fit_barrel[TT] << std::endl;

	std::cout << "--- h_fitNch_barrel->GetBinContent(8): " << h_fitNch_barrel->GetBinContent(8) << std::endl;
	std::cout << "--- h_fitNch_barrel->GetBinContent(9) : " << h_fitNch_barrel->GetBinContent(9) << std::endl;
	std::cout << "dataall_pt_barrel_integ : " << dataall_pt_barrel_integ << std::endl;
	std::cout << "denominator_pt_fit_barrel[TT]->Integral() : " << denominator_pt_fit_barrel[TT]->Integral() << std::endl;
	std::cout << "norm_pt_fit_barrel[TT] : " << norm_pt_fit_barrel[TT] << std::endl;


	//for TW+TbarW
	double intTWs_barrel=0, intTWs_endcap=0;
	TH1D* denominator_barrel_TWs;
	TH1D* denominator_endcap_TWs;
	TH1D* tmp_barrel_TWs=(TH1D*)denominator_barrel[TW]->Clone();
	TH1D* tmp_endcap_TWs=(TH1D*)denominator_endcap[TW]->Clone();
	//       tmp_barrel_TWs->Scale(norm_all[TW]);
	//       tmp_endcap_TWs->Scale(norm_all[TW]);
	denominator_barrel_TWs=(TH1D*)tmp_barrel_TWs->Clone();
	denominator_endcap_TWs=(TH1D*)tmp_endcap_TWs->Clone();
	intTWs_barrel+=denominator_barrel_TWs->Integral();
	intTWs_endcap+=denominator_endcap_TWs->Integral();
	tmp_barrel_TWs=(TH1D*)denominator_barrel[TbarW]->Clone();
	tmp_endcap_TWs=(TH1D*)denominator_endcap[TbarW]->Clone();
	//       tmp_barrel_TWs->Scale(norm_all[TbarW]);
	//       tmp_endcap_TWs->Scale(norm_all[TbarW]);
	denominator_barrel_TWs->Add(tmp_barrel_TWs);
	denominator_endcap_TWs->Add(tmp_endcap_TWs);
	intTWs_barrel+=denominator_barrel_TWs->Integral();
	intTWs_endcap+=denominator_endcap_TWs->Integral();

	double intTWs_barrel_pt=0, intTWs_endcap_pt=0;
	TH1D* denominator_barrel_TWs_pt;
	TH1D* denominator_endcap_TWs_pt;
	TH1D* tmp_barrel_TWs_pt=(TH1D*)denominator_pt_fit_barrel[TW]->Clone();
	TH1D* tmp_endcap_TWs_pt=(TH1D*)denominator_pt_fit_endcap[TW]->Clone();
	//       tmp_barrel_TWs_pt->Scale(norm_all[TW]);
	//       tmp_endcap_TWs_pt->Scale(norm_all[TW]);
	denominator_barrel_TWs_pt=(TH1D*)tmp_barrel_TWs_pt->Clone();
	denominator_endcap_TWs_pt=(TH1D*)tmp_endcap_TWs_pt->Clone();
	intTWs_barrel_pt+=denominator_barrel_TWs_pt->Integral();
	intTWs_endcap_pt+=denominator_endcap_TWs_pt->Integral();
	tmp_barrel_TWs_pt=(TH1D*)denominator_pt_fit_barrel[TbarW]->Clone();
	tmp_endcap_TWs_pt=(TH1D*)denominator_pt_fit_endcap[TbarW]->Clone();
	//       tmp_barrel_TWs_pt->Scale(norm_all[TbarW]);
	//       tmp_endcap_TWs_pt->Scale(norm_all[TbarW]);
	denominator_barrel_TWs_pt->Add(tmp_barrel_TWs_pt);
	denominator_endcap_TWs_pt->Add(tmp_endcap_TWs_pt);
	intTWs_barrel_pt+=denominator_barrel_TWs_pt->Integral();
	intTWs_endcap_pt+=denominator_endcap_TWs_pt->Integral();



	for (int i=DYTauTau1030; i<=DYMuMu30_PbP; i++) {
		//		 if (i==DYMuMu1030_PbP || i==DYMuMu30_PbP) continue;
		//       norm_fit_barrel[i] = h_fitNch_barrel->GetBinContent(1)/intDY_barrel;
		//###			norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(1)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/intDY_barrel;
		//###			norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(1)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/intDY_barrel_pt;
		norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(1)/sum_of_fitNch_barrel)*dataall_barrel_integ/intDY_barrel;
		norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(1)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/intDY_barrel_pt;



		std::cout << std::endl;
		std::cout << "--- h_fitNch_barrel->GetBinContent(1): " << h_fitNch_barrel->GetBinContent(1) << std::endl;
		std::cout << "--- h_fitNch_barrel->GetBinContent(9) : " << h_fitNch_barrel->GetBinContent(9) << std::endl;
		std::cout << "dataall_barrel_integ : " << dataall_barrel_integ << std::endl;
		std::cout << "intDY_barrel : " << intDY_barrel << std::endl;
		std::cout << "norm_fit_barrel[" << i << "] : " << norm_fit_barrel[i] << std::endl;

		std::cout << "--- h_fitNch_barrel->GetBinContent(1): " << h_fitNch_barrel->GetBinContent(1) << std::endl;
		std::cout << "--- h_fitNch_barrel->GetBinContent(9) : " << h_fitNch_barrel->GetBinContent(9) << std::endl;
		std::cout << "dataall_pt_barrel_integ : " << dataall_pt_barrel_integ << std::endl;
		std::cout << "intDY_barrel_pt : " << intDY_barrel_pt << std::endl;
		std::cout << "norm_pt_fit_barrel[" << i << "] : " << norm_pt_fit_barrel[i] << std::endl;



	}
	for (int i=WpMu; i<=WmTau; i++) {
		// norm_fit_barrel[i] = h_fitNch_barrel->GetBinContent(3)/intWJets_barrel;
		//###		norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(3)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/intWJets_barrel;
		//###		norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(3)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/intWJets_barrel_pt;
		norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(3)/sum_of_fitNch_barrel)*dataall_barrel_integ/intWJets_barrel;
		norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(3)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/intWJets_barrel_pt;



		std::cout << std::endl;
		std::cout << "--- h_fitNch_barrel->GetBinContent(3): " << h_fitNch_barrel->GetBinContent(3) << std::endl;
		std::cout << "--- h_fitNch_barrel->GetBinContent(9) : " << h_fitNch_barrel->GetBinContent(9) << std::endl;
		std::cout << "dataall_barrel_integ : " << dataall_barrel_integ << std::endl;
		std::cout << "intWJets_barrel : " << intWJets_barrel << std::endl;
		std::cout << "norm_fit_barrel[" << i << "] : " << norm_fit_barrel[i] << std::endl;

		std::cout << "--- h_fitNch_barrel->GetBinContent(3): " << h_fitNch_barrel->GetBinContent(3) << std::endl;
		std::cout << "--- h_fitNch_barrel->GetBinContent(9) : " << h_fitNch_barrel->GetBinContent(9) << std::endl;
		std::cout << "dataall_pt_barrel_integ : " << dataall_pt_barrel_integ << std::endl;
		std::cout << "intWJets_barrel_pt : " << intWJets_barrel_pt << std::endl;
		std::cout << "norm_pt_fit_barrel[" << i << "] : " << norm_pt_fit_barrel[i] << std::endl;



	}
	for (int i=TW; i<=TbarW; i++) {
		//norm_fit_barrel[i] = h_fitNch_barrel->GetBinContent(4)/intTWs_barrel;
		//###	norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(4)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/intTWs_barrel;
		//###	norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(4)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/intTWs_barrel_pt;
		norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(4)/sum_of_fitNch_barrel)*dataall_barrel_integ/intTWs_barrel;
		norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(4)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/intTWs_barrel_pt;



	}
	/*
		norm_fit_barrel[TT] = h_fitNch_barrel->GetBinContent(8)/denominator_barrel[TT]->Integral();
		norm_fit_barrel[QCD] = h_fitNch_barrel->GetBinContent(2)/denominator_barrel[QCD]->Integral();
	//	 norm_fit_barrel[TW] = h_fitNch_barrel->GetBinContent(4)/denominator_barrel[TW]->Integral();
	norm_fit_barrel[WW] = h_fitNch_barrel->GetBinContent(5)/denominator_barrel[WW]->Integral();
	norm_fit_barrel[WZ] = h_fitNch_barrel->GetBinContent(6)/denominator_barrel[WZ]->Integral();
	norm_fit_barrel[ZZ] = h_fitNch_barrel->GetBinContent(7)/denominator_barrel[ZZ]->Integral();
	*/
	//######
	/*
		norm_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/denominator_barrel[TT]->Integral();
		norm_pt_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/denominator_pt_fit_barrel[TT]->Integral();
		norm_fit_barrel[QCD] = (h_fitNch_barrel->GetBinContent(2)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/denominator_barrel[QCD]->Integral();
		norm_pt_fit_barrel[QCD] = (h_fitNch_barrel->GetBinContent(2)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/denominator_pt_fit_barrel[QCD]->Integral();
		norm_fit_barrel[WW] = (h_fitNch_barrel->GetBinContent(5)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/denominator_barrel[WW]->Integral();
		norm_pt_fit_barrel[WW] = (h_fitNch_barrel->GetBinContent(5)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/denominator_pt_fit_barrel[WW]->Integral();
		norm_fit_barrel[WZ] = (h_fitNch_barrel->GetBinContent(6)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/denominator_barrel[WZ]->Integral();
		norm_pt_fit_barrel[WZ] = (h_fitNch_barrel->GetBinContent(6)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/denominator_pt_fit_barrel[WZ]->Integral();
		norm_fit_barrel[ZZ] = (h_fitNch_barrel->GetBinContent(7)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/denominator_barrel[ZZ]->Integral();
		norm_pt_fit_barrel[ZZ] = (h_fitNch_barrel->GetBinContent(7)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/denominator_pt_fit_barrel[ZZ]->Integral();
		*/
	//#######
	norm_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/sum_of_fitNch_barrel)*dataall_barrel_integ/denominator_barrel[TT]->Integral();
	norm_pt_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/denominator_pt_fit_barrel[TT]->Integral();
	norm_fit_barrel[QCD] = (h_fitNch_barrel->GetBinContent(2)/sum_of_fitNch_barrel)*dataall_barrel_integ/denominator_barrel[QCD]->Integral();
	norm_pt_fit_barrel[QCD] = (h_fitNch_barrel->GetBinContent(2)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/denominator_pt_fit_barrel[QCD]->Integral();
	norm_fit_barrel[WW] = (h_fitNch_barrel->GetBinContent(5)/sum_of_fitNch_barrel)*dataall_barrel_integ/denominator_barrel[WW]->Integral();
	norm_pt_fit_barrel[WW] = (h_fitNch_barrel->GetBinContent(5)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/denominator_pt_fit_barrel[WW]->Integral();
	norm_fit_barrel[WZ] = (h_fitNch_barrel->GetBinContent(6)/sum_of_fitNch_barrel)*dataall_barrel_integ/denominator_barrel[WZ]->Integral();
	norm_pt_fit_barrel[WZ] = (h_fitNch_barrel->GetBinContent(6)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/denominator_pt_fit_barrel[WZ]->Integral();
	norm_fit_barrel[ZZ] = (h_fitNch_barrel->GetBinContent(7)/sum_of_fitNch_barrel)*dataall_barrel_integ/denominator_barrel[ZZ]->Integral();
	norm_pt_fit_barrel[ZZ] = (h_fitNch_barrel->GetBinContent(7)/sum_of_fitNch_barrel)*dataall_pt_barrel_integ/denominator_pt_fit_barrel[ZZ]->Integral();






	std::cout << "#######################" << std::endl;
	std::cout << "norm_fit_barrel[QCD]: "<< norm_fit_barrel[QCD] << std::endl;

	for (int i=DYTauTau1030; i<=DYMuMu30_PbP; i++) {
		//   		 if (i==DYMuMu1030_PbP || i==DYMuMu30_PbP) continue;
		//	   norm_fit_endcap[i] = h_fitNch_endcap->GetBinContent(1)/intDY_endcap;
		//###	norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(1)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/intDY_endcap;
		//###	norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(1)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/intDY_endcap_pt;
		norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(1)/sum_of_fitNch_endcap)*dataall_endcap_integ/intDY_endcap;
		norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(1)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/intDY_endcap_pt;


	}
	//    norm_fit_endcap[TT] = h_fitNch_endcap->GetBinContent(8)/denominator_endcap[TT]->Integral();
	//###	norm_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[TT]->Integral();
	//###	norm_pt_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[TT]->Integral();
	norm_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/sum_of_fitNch_endcap)*dataall_endcap_integ/denominator_endcap[TT]->Integral();
	norm_pt_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/denominator_pt_fit_endcap[TT]->Integral();



	for (int i=WpMu; i<=WmTau; i++) {
		//       norm_fit_endcap[i] = h_fitNch_endcap->GetBinContent(3)/intWJets_endcap;
		//### 	norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(3)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/intWJets_endcap;
		//###	norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(3)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/intWJets_endcap_pt;
		norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(3)/sum_of_fitNch_endcap)*dataall_endcap_integ/intWJets_endcap;
		norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(3)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/intWJets_endcap_pt;



	}
	for (int i=TW; i<=TbarW; i++) {
		//       norm_fit_endcap[i] = h_fitNch_endcap->GetBinContent(4)/intTWs_endcap;
		//###	norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(4)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/intTWs_endcap;
		//###	norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(4)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/intTWs_endcap_pt;
		norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(4)/sum_of_fitNch_endcap)*dataall_endcap_integ/intTWs_endcap;
		norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(4)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/intTWs_endcap_pt;



	}

	/*
		norm_fit_endcap[QCD] = h_fitNch_endcap->GetBinContent(2)/denominator_endcap[QCD]->Integral();
	//	 norm_fit_endcap[TW] = h_fitNch_endcap->GetBinContent(4)/denominator_endcap[TW]->Integral();
	norm_fit_endcap[WW] = h_fitNch_endcap->GetBinContent(5)/denominator_endcap[WW]->Integral();
	norm_fit_endcap[WZ] = h_fitNch_endcap->GetBinContent(6)/denominator_endcap[WZ]->Integral();
	norm_fit_endcap[ZZ] = h_fitNch_endcap->GetBinContent(7)/denominator_endcap[ZZ]->Integral();
	*/


	//###########
	/*
		norm_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[TT]->Integral();
		norm_pt_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[TT]->Integral();
		norm_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[QCD]->Integral();
		norm_pt_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[QCD]->Integral();
		norm_fit_endcap[WW] = (h_fitNch_endcap->GetBinContent(5)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[WW]->Integral();
		norm_pt_fit_endcap[WW] = (h_fitNch_endcap->GetBinContent(5)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[WW]->Integral();
		norm_fit_endcap[WZ] = (h_fitNch_endcap->GetBinContent(6)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[WZ]->Integral();
		norm_pt_fit_endcap[WZ] = (h_fitNch_endcap->GetBinContent(6)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[WZ]->Integral();
		norm_fit_endcap[ZZ] = (h_fitNch_endcap->GetBinContent(7)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[ZZ]->Integral();
		norm_pt_fit_endcap[ZZ] = (h_fitNch_endcap->GetBinContent(7)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[ZZ]->Integral();
		*/
	//###################
	norm_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/sum_of_fitNch_endcap)*dataall_endcap_integ/denominator_endcap[TT]->Integral();
	norm_pt_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/denominator_pt_fit_endcap[TT]->Integral();
	norm_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/sum_of_fitNch_endcap)*dataall_endcap_integ/denominator_endcap[QCD]->Integral();
	norm_pt_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/denominator_pt_fit_endcap[QCD]->Integral();
	norm_fit_endcap[WW] = (h_fitNch_endcap->GetBinContent(5)/sum_of_fitNch_endcap)*dataall_endcap_integ/denominator_endcap[WW]->Integral();
	norm_pt_fit_endcap[WW] = (h_fitNch_endcap->GetBinContent(5)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/denominator_pt_fit_endcap[WW]->Integral();
	norm_fit_endcap[WZ] = (h_fitNch_endcap->GetBinContent(6)/sum_of_fitNch_endcap)*dataall_endcap_integ/denominator_endcap[WZ]->Integral();
	norm_pt_fit_endcap[WZ] = (h_fitNch_endcap->GetBinContent(6)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/denominator_pt_fit_endcap[WZ]->Integral();
	norm_fit_endcap[ZZ] = (h_fitNch_endcap->GetBinContent(7)/sum_of_fitNch_endcap)*dataall_endcap_integ/denominator_endcap[ZZ]->Integral();
	norm_pt_fit_endcap[ZZ] = (h_fitNch_endcap->GetBinContent(7)/sum_of_fitNch_endcap)*dataall_pt_endcap_integ/denominator_pt_fit_endcap[ZZ]->Integral();


	norm_fit_endcap[Data1] = 1.0;
	norm_fit_endcap[Data2] = 1.0;
	norm_fit_barrel[Data1] = 1.0;
	norm_fit_barrel[Data2] = 1.0;

	norm_pt_fit_endcap[Data1] = 1.0;
	norm_pt_fit_endcap[Data2] = 1.0;
	norm_pt_fit_barrel[Data1] = 1.0;
	norm_pt_fit_barrel[Data2] = 1.0;

	//    norm_all[Data1] = (Xsec(tag)*lumi_all)/Nevts(tag);
	TH1D* FR_QCDonly_barrel;
	TH1D* FR_QCDonly_endcap;

	FR_QCDonly_barrel = (TH1D*)numerator_pt_barrel[QCD]->Clone();
	FR_QCDonly_endcap = (TH1D*)numerator_pt_endcap[QCD]->Clone();

	if (QCDopt==1 && FRopt==101) {
		FR_QCDonly_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_QCDonly_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
	}
	else if (QCDopt==2 && FRopt==102) {
		FR_QCDonly_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_QCDonly_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
	}
	else {
		std::cout << "QCDopt : " << QCDopt << " - FRopt : " << FRopt << std::endl;
		std::cout << "QCDopt and FRopt is not corresponed, just use present QCDopt files" << std::endl;
		FR_QCDonly_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_QCDonly_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
	}
	FR_QCDonly_barrel->SetName("FR_QCDonly_barrel");
	FR_QCDonly_endcap->SetName("FR_QCDonly_endcap");

	TH1D* FR_QCDonly_barrel_beffit = (TH1D*)FR_QCDonly_barrel->Clone("FR_QCDonly_barrel_beffit");
	TH1D* FR_QCDonly_endcap_beffit = (TH1D*)FR_QCDonly_endcap->Clone("FR_QCDonly_endcap_beffit");



	for(int i=0;i<=QCD;i++) {
		if (i==ALL) continue;
		SampleTag tag = static_cast<SampleTag>(i);
		denominator_barrel[i]->Scale(norm_fit_barrel[i]);
		//       denominator_pt_fit_barrel[i]->Scale(norm_fit_barrel[i]);
		denominator_pt_fit_barrel[i]->Scale(norm_pt_fit_barrel[i]);
		//       denominator_pt_xsec_barrel[i]->Scale(norm_all[i]);
		//       numerator_pt_barrel[i]->Scale(norm_all[i]);

		//###L672		if (i==QCD) numerator_pt_barrel[i]->Scale(norm_pt_fit_barrel[i]);
		numerator_pt_barrel[i]->Scale(norm_pt_fit_barrel[i]);
		denominator_endcap[i]->Scale(norm_fit_endcap[i]);
		//       denominator_pt_fit_endcap[i]->Scale(norm_fit_endcap[i]);
		denominator_pt_fit_endcap[i]->Scale(norm_pt_fit_endcap[i]);
		//       denominator_pt_xsec_endcap[i]->Scale(norm_all[i]);
		//       numerator_pt_endcap[i]->Scale(norm_all[i]);
		//###L679		if (i==QCD) numerator_pt_endcap[i]->Scale(norm_pt_fit_endcap[i]);
		//	norm_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[QCD]->Integral();
		//	norm_pt_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[QCD]->Integral();
		numerator_pt_endcap[i]->Scale(norm_pt_fit_endcap[i]);
		std::cout << "#######################" << std::endl;
		std::cout << "scaled denominator_pt_fit_barrel[" << Name(tag) << "(" << i << ")]: " << denominator_pt_fit_barrel[i]->Integral() << std::endl;
		std::cout << "scaled denominator_pt_fit_endcap[" << Name(tag) << "(" << i << ")]: " << denominator_pt_fit_endcap[i]->Integral() << std::endl;
		std::cout << "scaled numerator_pt_barrel[" << Name(tag) << "(" << i << ")]: " << numerator_pt_barrel[i]->Integral() << std::endl;
		std::cout << "scaled numerator_pt_endcap[" << Name(tag) << "(" << i << ")]: " << numerator_pt_endcap[i]->Integral() << std::endl;
		std::cout << "#######################" << std::endl;

		if(IsData(tag)) {
			setDataHist( denominator_pt_fit_barrel[i] );
			setDataHist( denominator_pt_xsec_barrel[i] );
			setDataHist( denominator_barrel[i] );
			setDataHist( numerator_pt_barrel[i] );
			setDataHist( denominator_pt_fit_endcap[i] );
			setDataHist( denominator_pt_xsec_endcap[i] );
			setDataHist( denominator_endcap[i] );
			setDataHist( numerator_pt_endcap[i] );
		}
		else {
			setMCHist( denominator_pt_fit_barrel[i], i );
			setMCHist( denominator_pt_xsec_barrel[i], i );
			setMCHist( denominator_barrel[i], i );
			setMCHist( numerator_pt_barrel[i], i );
			setMCHist( denominator_pt_fit_endcap[i], i );
			setMCHist( denominator_pt_xsec_endcap[i], i );
			setMCHist( denominator_endcap[i], i );
			setMCHist( numerator_pt_endcap[i], i );
		}

		if (IsData(tag)) {
			hs_data->Add(denominator_pt_xsec_barrel[i]);
		}
		else {
			hs_xsec->Add(denominator_pt_xsec_barrel[i]);
			hs_fit->Add(denominator_pt_fit_barrel[i]);
		}

		TH1D* hratio_xsecvsfit;
		hratio_xsecvsfit=(TH1D*)denominator_pt_fit_barrel[i]->Clone();
		hratio_xsecvsfit->Sumw2();
		hratio_xsecvsfit->Divide(denominator_pt_xsec_barrel[i],denominator_pt_fit_barrel[i],1,1,"B");

		TCanvas* chs = new TCanvas("chs","",600,800);
		chs->SetFillColor(0);

		TPad* pad1 = new TPad("pad1","",0.0,0.30,1.0,1.0,21);
		TPad* pad2 = new TPad("pad2","",0.0,0.0,1.0,0.30,21);

		pad1->SetFillColor(0);
		pad2->SetFillColor(0);

		pad1->SetTopMargin(0.05);
		pad1->SetBottomMargin(0.0);
		pad1->SetLeftMargin(0.15);
		pad1->SetRightMargin(0.05);

		pad2->SetTopMargin(0.0);
		pad2->SetBottomMargin(0.3);
		pad2->SetLeftMargin(0.15);
		pad2->SetRightMargin(0.05);

		pad1->Draw();
		pad2->Draw();

		pad1->cd();
		pad1->SetLogy(1);
		denominator_pt_xsec_barrel[i]->GetYaxis()->SetRangeUser(0.1,10*denominator_pt_xsec_barrel[i]->GetMaximum());
		denominator_pt_xsec_barrel[i]->Draw("hist");
		denominator_pt_fit_barrel[i]->Draw("same pe");

		pad2->cd();
		pad2->SetLogy(0);
		hratio_xsecvsfit->SetMarkerStyle(20);
		hratio_xsecvsfit->SetMarkerColor(kBlack);

		hratio_xsecvsfit->GetXaxis()->SetTitle("muon's p_{T} (GeV)");

		hratio_xsecvsfit->GetXaxis()->SetTitleSize(0.140);
		hratio_xsecvsfit->GetXaxis()->SetLabelSize(0.100);
		hratio_xsecvsfit->GetYaxis()->SetTitle("Fitting/Xsection");
		hratio_xsecvsfit->GetYaxis()->SetTitleSize(0.140);
		hratio_xsecvsfit->GetYaxis()->SetLabelSize(0.070);
		hratio_xsecvsfit->GetYaxis()->SetTitleOffset(0.4);
		hratio_xsecvsfit->GetXaxis()->SetTitleOffset(1.0);


		hratio_xsecvsfit->Draw("pe");
		//chs->SaveAs(Form("print/Comp_Xsec_vs_Fit_%s_opt7_QCDDATASS2.pdf",Name(tag)));	
		//###chs->SaveAs(Form("print/Comp_Xsec_vs_Fit_%s_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.pdf",Name(tag)));	
		//	chs->SaveAs(Form("printEst_20190128/Comp_Xsec_vs_Fit_%s_%s.pdf",Name(tag),rmk.Data()));	
		chs->SaveAs(Form("printEst_%s/Comp_Xsec_vs_Fit_%s_%s.pdf",indir3.Data(),Name(tag),rmkf.Data()));  

	}
	TCanvas* chs1 = new TCanvas("chs1","stacked hists",600,600);
	chs1->SetLogy(1);
	hs_data->Draw("");
	hs_xsec->Draw("same");
	//	 chs1->SaveAs("print/Comp_Data_vs_Xsec_stacked_opt7_QCDDATASS2.pdf");
	//	 chs1->SaveAs("print/Comp_Data_vs_Xsec_stacked_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.pdf");
	//	 chs1->SaveAs(Form("printEst_20190128/Comp_Data_vs_Xsec_stacked_%s.pdf",rmk.Data()));
	chs1->SaveAs(Form("printEst_%s/Comp_Data_vs_Xsec_stacked_%s.pdf",indir3.Data(),rmkf.Data()));


	hs_data->Draw("");
	hs_fit->Draw("same");
	//	 chs1->SaveAs("print/Comp_Data_vs_Fit_stacked_opt7_QCDDATASS2.pdf");
	//	 chs1->SaveAs("print/Comp_Data_vs_Fit_stacked_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.pdf");
	//	 chs1->SaveAs(Form("printEst_20190128/Comp_Data_vs_Fit_stacked_%s.pdf",rmk.Data()));
	chs1->SaveAs(Form("printEst_%s/Comp_Data_vs_Fit_stacked_%s.pdf",indir3.Data(),rmkf.Data()));

	/*
		TH1D* FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
		TH1D* FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);

		TH1D* FR_xsec_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_xsec_barrel);
		TH1D* FR_xsec_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_xsec_endcap);
		*/
	//    TH1D* FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
	//    TH1D* FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);

	//    TH1D* FR_template_barrel = (TH1D*)FRByTemplate(denominator_pt_fit_barrel, denominator_pt_fit_barrel);
	//    TH1D* FR_template_endcap = (TH1D*)FRByTemplate(denominator_pt_fit_endcap, denominator_pt_fit_endcap);

	TH1D* FR_template_barrel_tmp = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
	TH1D* FR_template_endcap_tmp = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);


	TH1D* FR_xsec_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
	TH1D* FR_xsec_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);
	/*
		TH1D* FRCheck_numerator_pt_barrel = (TH1D*)FRCheck(numerator_pt_barrel, numerator_pt_barrel);
		TH1D* FRCheck_numerator_pt_endcap = (TH1D*)FRCheck(numerator_pt_endcap, numerator_pt_endcap);
		TH1D* FRCheck_denominator_pt_fit_barrel = (TH1D*)FRCheck(denominator_pt_fit_barrel, denominator_pt_fit_barrel);
		TH1D* FRCheck_denominator_pt_fit_endcap = (TH1D*)FRCheck(denominator_pt_fit_endcap, denominator_pt_fit_endcap);
		*/
	TH1D* FRCheck_numerator_pt_barrel;
	TH1D* FRCheck_numerator_pt_endcap;
	TH1D* FRCheck_denominator_pt_fit_barrel;
	TH1D* FRCheck_denominator_pt_fit_endcap;
	//	 FRCheck_numerator_pt_barrel->Sumw2();
	//	 FRCheck_numerator_pt_endcap->Sumw2();
	//	 FRCheck_denominator_pt_fit_barrel->Sumw2();
	//	 FRCheck_denominator_pt_fit_endcap->Sumw2();
	FRCheck_numerator_pt_barrel = (TH1D*)FRCheck(numerator_pt_barrel, numerator_pt_barrel);
	FRCheck_numerator_pt_endcap = (TH1D*)FRCheck(numerator_pt_endcap, numerator_pt_endcap);
	FRCheck_denominator_pt_fit_barrel = (TH1D*)FRCheck(denominator_pt_fit_barrel, denominator_pt_fit_barrel);
	FRCheck_denominator_pt_fit_endcap = (TH1D*)FRCheck(denominator_pt_fit_endcap, denominator_pt_fit_endcap);

	FRCheck_numerator_pt_barrel->SetName("FRCheck_numerator_pt_barrel");
	FRCheck_numerator_pt_endcap->SetName("FRCheck_numerator_pt_endcap");
	FRCheck_denominator_pt_fit_barrel->SetName("FRCheck_denominator_pt_fit_barrel");
	FRCheck_denominator_pt_fit_endcap->SetName("FRCheck_denominator_pt_fit_endcap");

	TH1D* Ratio_barrel = (TH1D*)FRCheck_numerator_pt_barrel->Clone("Ratio_barrel");
	TH1D* Ratio_endcap = (TH1D*)FRCheck_numerator_pt_endcap->Clone("Ratio_endcap");

	Ratio_barrel->Divide(Ratio_barrel,FRCheck_denominator_pt_fit_barrel,1,1,"B");
	Ratio_endcap->Divide(Ratio_endcap,FRCheck_denominator_pt_fit_endcap,1,1,"B");



	TH1D* Ratio_tot_barrel;
	TH1D* Ratio_tot_endcap;
	TH1D* Ratio_num_barrel;
	TH1D* Ratio_num_endcap;
	TH1D* Ratio_den_barrel;
	TH1D* Ratio_den_endcap;

	Ratio_num_barrel = (TH1D*)FRCheck(numerator_pt_barrel, numerator_pt_barrel);
	Ratio_num_endcap = (TH1D*)FRCheck(numerator_pt_endcap, numerator_pt_endcap);
	Ratio_num_barrel->Sumw2();
	Ratio_num_endcap->Sumw2();
	Ratio_num_barrel->SetName("Ratio_num_barrel");
	Ratio_num_endcap->SetName("Ratio_num_endcap");

	Ratio_den_barrel = (TH1D*)FRCheck(denominator_pt_fit_barrel,denominator_pt_fit_barrel);
	Ratio_den_endcap = (TH1D*)FRCheck(denominator_pt_fit_endcap,denominator_pt_fit_endcap);
	Ratio_den_barrel->Sumw2();
	Ratio_den_endcap->Sumw2();
	Ratio_den_barrel->SetName("Ratio_den_barrel");
	Ratio_den_endcap->SetName("Ratio_den_endcap");

	Ratio_tot_barrel = (TH1D*)Ratio_num_barrel->Clone();
	Ratio_tot_endcap = (TH1D*)Ratio_num_endcap->Clone();
	Ratio_tot_barrel->Sumw2();
	Ratio_tot_endcap->Sumw2();
	Ratio_tot_barrel->Divide(Ratio_num_barrel,Ratio_den_barrel,1,1,"B");
	Ratio_tot_endcap->Divide(Ratio_num_endcap,Ratio_den_endcap,1,1,"B");
	Ratio_tot_barrel->SetName("Ratio_tot_barrel");
	Ratio_tot_endcap->SetName("Ratio_tot_endcap");

	//###	 Ratio_barrel->Divide(Ratio_barrel,FRCheck_denominator_pt_fit_barrel,1,1,"B");
	//###	 Ratio_endcap->Divide(Ratio_endcap,FRCheck_denominator_pt_fit_endcap,1,1,"B");

	//    TH1D* FR_xsec_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
	//    TH1D* FR_xsec_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);

	//###	 FR_xsec_barrel->Divide(FR_xsec_barrel,Ratio_barrel,1,1,"B");
	//###	 FR_xsec_endcap->Divide(FR_xsec_endcap,Ratio_endcap,1,1,"B");

	//	 FR_template_barrel->Divide(FR_template_barrel,FRCheck_denominator_pt_fit_barrel,1,1,"B");
	//	 FR_template_endcap->Divide(FR_template_endcap,FRCheck_denominator_pt_fit_endcap,1,1,"B");

	if (FRopt==101 || FRopt==102) {
		std::cout << "FRopt == 101 or 102 , PASSED" << std::endl;
	}
	else if (QCDopt==1 && FRopt==103) {
		FR_QCDonly_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_QCDonly_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
	}
	else if (QCDopt==2 && FRopt==104) {
		FR_QCDonly_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_QCDonly_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
	}
	else {
		std::cout << "QCDopt : " << QCDopt << " - FRopt : " << FRopt << std::endl;
		std::cout << "QCDopt and FRopt is not corresponed, just use present QCDopt files" << std::endl;
		FR_QCDonly_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_QCDonly_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
	}
	FR_QCDonly_barrel->SetName("FR_QCDonly_barrel");
	FR_QCDonly_endcap->SetName("FR_QCDonly_endcap");

	TH1D* FR_QCDonly_barrel_aftfit = (TH1D*)FR_QCDonly_barrel->Clone("FR_QCDonly_barrel_aftfit");
	TH1D* FR_QCDonly_endcap_aftfit = (TH1D*)FR_QCDonly_endcap->Clone("FR_QCDonly_endcap_aftfit");


	TH1D* FR_template_barrel;
	TH1D* FR_template_endcap;

	/*
		if (FRopt==10) {
		FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);
		}
		else if (FRopt==11) {
		FR_template_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);
		}
		else if (FRopt==12) {
		FR_template_barrel = (TH1D*)numerator_pt_barrel[QCD]->Clone();
		FR_template_endcap = (TH1D*)numerator_pt_barrel[QCD]->Clone();
		FR_template_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_template_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
		}
		*/

	if (FRopt==200) {
		FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);
	}
	else if (FRopt==201) {
		FR_template_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);
	}
	else if (FRopt==202) {
		FR_template_barrel = (TH1D*)numerator_pt_barrel[QCD]->Clone();
		FR_template_endcap = (TH1D*)numerator_pt_barrel[QCD]->Clone();
		FR_template_barrel->Divide(numerator_pt_barrel[QCD],denominator_pt_fit_barrel[QCD],1,1,"B");
		FR_template_endcap->Divide(numerator_pt_endcap[QCD],denominator_pt_fit_endcap[QCD],1,1,"B");
	}
	else if (FRopt==203) {
		FR_template_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);
		FR_template_barrel->Divide(FR_template_barrel,Ratio_num_barrel,1,1,"B");
		FR_template_endcap->Divide(FR_template_endcap,Ratio_num_endcap,1,1,"B");
	}
	else if (FRopt==204) {
		FR_template_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);
		FR_template_barrel->Multiply(Ratio_den_barrel);
		FR_template_endcap->Multiply(Ratio_den_endcap);
	}
	else if (FRopt==205) {
		FR_template_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);
		FR_template_barrel->Divide(FR_template_barrel,Ratio_tot_barrel,1,1,"B");
		FR_template_endcap->Divide(FR_template_endcap,Ratio_tot_endcap,1,1,"B");
	}
	else {
		std::cout << "FRopt is not 200-205, 205 will be stored" << std::endl;
		FR_template_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
		FR_template_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);
		FR_template_barrel->Divide(FR_template_barrel,Ratio_tot_barrel,1,1,"B");
		FR_template_endcap->Divide(FR_template_endcap,Ratio_tot_endcap,1,1,"B");

	}
	FR_template_barrel->SetName("FR_template_barrel");
	FR_template_endcap->SetName("FR_template_endcap");

	//###	 Ratio_barrel->Divide(Ratio_barrel,FRCheck_denominator_pt_fit_barrel,1,1,"B");
	//###	 Ratio_endcap->Divide(Ratio_endcap,FRCheck_denominator_pt_fit_endcap,1,1,"B");

	//    TH1D* FR_xsec_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
	//    TH1D* FR_xsec_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);

	//###	 FR_xsec_barrel->Divide(FR_xsec_barrel,Ratio_barrel,1,1,"B");
	//###	 FR_xsec_endcap->Divide(FR_xsec_endcap,Ratio_endcap,1,1,"B");

	//	 FR_template_barrel->Divide(FR_template_barrel,FRCheck_denominator_pt_fit_barrel,1,1,"B");
	//	 FR_template_endcap->Divide(FR_template_endcap,FRCheck_denominator_pt_fit_endcap,1,1,"B");




	std::cout << "### CHECK UNITY ###" << std::endl;
	double num_uni = denominator_pt_fit_barrel[Data1]->Integral()+denominator_pt_fit_barrel[Data2]->Integral();
	double den_uni = 0.0;
	for (int i=0; i<=QCD; i++) {
		if (i==ALL || i==Data1 || i==Data2) continue;
		den_uni+=denominator_pt_fit_barrel[i]->Integral();
	}
	std::cout << "num_uni : " << num_uni << std::endl;
	std::cout << "den_uni : " << den_uni << std::endl;


	//    TH1D* FR_template_barrel = (TH1D*)FRByRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
	//    TH1D* FR_template_endcap = (TH1D*)FRByRatio(numerator_pt_endcap, denominator_pt_fit_endcap);


	int W = 1200;
	int H = 1200;

	int H_ref = 1200;
	int W_ref = 1200;

	// references for T, B, L, R
	float T = 0.08*H_ref;
	float B = 0.12*H_ref;
	float L = 0.12*W_ref;
	float R = 0.04*W_ref;

	lumi_13TeV = "2759 pb^{-1}";
	writeExtraText = true;
	extraText = "Preliminary";
	drawLogo = false;
	tdrGrid(true);
	lumiTextSize = 0.5;
	cmsTextSize = 0.75;
	/*
		TH1D* ptFrame = new TH1D("ptFrame","",8,47,500);
		ptFrame->SetStats(kFALSE);
		ptFrame->GetXaxis()->SetTitle("p_{T}[GeV]");
		ptFrame->GetYaxis()->SetTitle("Fake Rate");

		ptFrame->SetMinimum(0);
		ptFrame->SetMaximum(1.0); 
		ptFrame->GetXaxis()->SetTitleOffset(1);
		ptFrame->GetYaxis()->SetTitleOffset(1.1);
		ptFrame->GetXaxis()->SetTitleSize(0.05);
		ptFrame->GetYaxis()->SetTitleSize(0.05);  
		ptFrame->GetXaxis()->SetLabelSize(0.035);
		ptFrame->GetYaxis()->SetLabelSize(0.035); 
		ptFrame->GetXaxis()->SetMoreLogLabels(); 
		*/
	TH2D* ptFrame = new TH2D("ptFrame","",8,47,500,105,-0.05,1.00);
	ptFrame->SetStats(kFALSE);
	ptFrame->GetXaxis()->SetTitle("p_{T}[GeV]");
	ptFrame->GetYaxis()->SetTitle("Fake Rate");

	ptFrame->SetMinimum(0);
	ptFrame->SetMaximum(1.0); 
	ptFrame->GetXaxis()->SetTitleOffset(1);
	ptFrame->GetYaxis()->SetTitleOffset(1.1);
	ptFrame->GetXaxis()->SetTitleSize(0.05);
	ptFrame->GetYaxis()->SetTitleSize(0.05);  
	ptFrame->GetXaxis()->SetLabelSize(0.035);
	ptFrame->GetYaxis()->SetLabelSize(0.035); 
	ptFrame->GetXaxis()->SetMoreLogLabels(); 

	TCanvas* canv = new TCanvas("canv","",1200,1200);
	canv->SetFillColor(0);
	canv->SetLeftMargin( L/W );
	canv->SetRightMargin( R/W );
	canv->SetTopMargin( T/H );
	canv->SetBottomMargin( B/H );

	FR_template_barrel->SetMarkerSize(3);
	FR_template_endcap->SetMarkerSize(3);
	FR_xsec_barrel->SetMarkerSize(3);
	FR_xsec_endcap->SetMarkerSize(3);

	FR_template_barrel->SetLineColor(1);
	FR_template_barrel->SetLineWidth(2);
	FR_template_barrel->SetMarkerStyle(20);
	FR_template_barrel->SetMarkerColor(1);

	FR_xsec_barrel->SetLineColor(2);
	FR_xsec_barrel->SetLineWidth(2);
	FR_xsec_barrel->SetMarkerStyle(21);
	FR_xsec_barrel->SetMarkerColor(2);

	FR_template_endcap->SetLineColor(1);
	FR_template_endcap->SetLineWidth(2);
	FR_template_endcap->SetMarkerStyle(20);
	FR_template_endcap->SetMarkerColor(1);

	FR_xsec_endcap->SetLineColor(2);
	FR_xsec_endcap->SetLineWidth(2);
	FR_xsec_endcap->SetMarkerStyle(21);
	FR_xsec_endcap->SetMarkerColor(2);

	canv->cd();
	canv->SetLogx();


	TLegend* legend2 = new TLegend(.60,.70,.90,.89);
	legend2->AddEntry(FR_template_barrel,"Template fitting","lp");
	legend2->AddEntry(FR_xsec_barrel,"Ratio method","lp");
	legend2->SetBorderSize(0);

	ptFrame->Draw();
	CMS_lumi(canv,4,11);
	canv->Update();
	canv->RedrawAxis();
	canv->GetFrame()->Draw();
	FR_template_barrel->Draw("");
	FR_xsec_barrel->Draw("same");
	legend2->Draw("SAME");
	//canv->Print("print/FR_Barrel_opt7_QCDDATASS2.pdf");
	//canv->Print("print/FR_Barrel_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.pdf");
	//canv->Print(Form("printEst_20190128/FR_Barrel_%s.pdf",rmk.Data()));
	canv->Print(Form("printEst_%s/FR_Barrel_%s.pdf",indir3.Data(),rmkf.Data()));

	canv->Clear();
	ptFrame->Draw();
	CMS_lumi(canv,4,11);
	canv->Update();
	canv->RedrawAxis();
	canv->GetFrame()->Draw();
	FR_template_endcap->Draw("");
	FR_xsec_endcap->Draw("same");
	legend2->Draw("SAME");
	//canv->Print("print/FR_Endcap_opt7_QCDDATASS2.pdf");
	//canv->Print("print/FR_Endcap_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.pdf");
	//canv->Print(Form("printEst_20190128/FR_Endcap_%s.pdf",rmk.Data()));
	canv->Print(Form("printEst_%s/FR_Endcap_%s.pdf",indir3.Data(),rmkf.Data()));


	//TFile* g = new TFile("result/fakerate_opt7_QCDDATASS2.root","RECREATE");
	//TFile* g = new TFile("result/fakerate_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.root","RECREATE");
	//TFile* g = new TFile(Form("resultEst_20190128/fakerate_%s.root",rmk.Data()),"RECREATE");
	TFile* g = new TFile(Form("resultEst_%s/fakerate_%s.root",indir3.Data(),rmkf.Data()),"RECREATE");


	//    FRStore(g,numerator_pt_barrel, denominator_pt_fit_barrel);
	//    FRStore(g,numerator_pt_endcap, denominator_pt_fit_endcap);

	g->cd();
	test_barrel_denominator_QCDtemplate->Write();
   test_barrel_numerator_QCDtemplate->Write();


	//FR_template_barrel->Write();
	//FR_template_endcap->Write();
	FR_xsec_barrel->Write();
	FR_xsec_endcap->Write();
	FRCheck_numerator_pt_barrel->Write();
	FRCheck_numerator_pt_endcap->Write();
	FRCheck_denominator_pt_fit_barrel->Write();
	FRCheck_denominator_pt_fit_endcap->Write();
	Ratio_barrel->Write();
	Ratio_endcap->Write();

	//#################################################################

	FRStore(g,numerator_pt_barrel, denominator_pt_fit_barrel);
	FRStore(g,numerator_pt_endcap, denominator_pt_fit_endcap);

	FR_QCDonly_barrel_beffit->Write();
	FR_QCDonly_endcap_beffit->Write();
	FR_QCDonly_barrel_aftfit->Write();
	FR_QCDonly_endcap_aftfit->Write();



	Ratio_num_barrel->Write();
	Ratio_num_endcap->Write();
	Ratio_den_barrel->Write();
	Ratio_den_endcap->Write();
	Ratio_tot_barrel->Write();
	Ratio_tot_endcap->Write();

	FR_template_barrel->Write();
	FR_template_endcap->Write();





	//##################################################################
	g->Close();

	std::cout << "numerator unity check ------------------" << std::endl;
	cout<<"FRCheck_numerator_pt_barrel"<<endl;
	for(int i=1; i<ptbinnum+2; i++) {
		if(i!=1) cout<<",";
		cout<<FRCheck_numerator_pt_barrel->GetBinContent(i);
	}
	cout<<endl;
	cout<<"FRCheck_numerator_pt_endcap"<<endl;
	for(int i=1; i<ptbinnum_endcap+2; i++) {
		if(i!=1) cout<<",";
		cout<<FRCheck_numerator_pt_endcap->GetBinContent(i);
	}
	cout<<endl;
	std::cout << "denominator_pt_fit unity check ------------------" << std::endl;
	cout<<"FRCheck_denominator_pt_fit_barrel"<<endl;
	for(int i=1; i<ptbinnum+2; i++) {
		if(i!=1) cout<<",";
		cout<<FRCheck_denominator_pt_fit_barrel->GetBinContent(i);
	}
	cout<<endl;
	cout<<"FRCheck_denominator_pt_fit_endcap"<<endl;
	for(int i=1; i<ptbinnum_endcap+2; i++) {
		if(i!=1) cout<<",";
		cout<<FRCheck_denominator_pt_fit_endcap->GetBinContent(i);
	}
	cout<<endl;
	std::cout << "Ratio (barrel) ------------------" << std::endl;
	for(int i=1; i<ptbinnum+2; i++) {
		if(i!=1) cout<<",";
		cout<<Ratio_barrel->GetBinContent(i)<<" #pm " << Ratio_barrel->GetBinError(i) << endl;
	}
	cout<<endl;
	std::cout << "Ratio (endcap) ------------------" << std::endl;
	for(int i=1; i<ptbinnum_endcap+2; i++) {
		if(i!=1) cout<<",";
		cout<<Ratio_endcap->GetBinContent(i)<<" #pm " << Ratio_endcap->GetBinError(i) << endl;
	}
	cout<<endl;

	std::cout << "######## FR ---------------" << std::endl;
	cout<<"Template Barrel"<<endl;
	for(int i=1; i<ptbinnum+2; i++) {
		if(i!=1) cout<<",";
		cout<<FR_template_barrel->GetBinContent(i);
	}
	cout<<endl;
	cout<<"Template Endcap"<<endl;
	for(int i=1; i<ptbinnum_endcap+2; i++) {
		if(i!=1) cout<<",";
		cout<<FR_template_endcap->GetBinContent(i);
	}
	cout<<endl;
	cout<<"Ratio Barrel"<<endl;
	for(int i=1; i<ptbinnum+2; i++) {
		if(i!=1) cout<<",";
		cout<<FR_xsec_barrel->GetBinContent(i);
	}
	cout<<endl;
	cout<<"Ratio Endcap"<<endl;
	for(int i=1; i<ptbinnum_endcap+2; i++) {
		if(i!=1) cout<<",";
		cout<<FR_xsec_endcap->GetBinContent(i);
	}
	cout<<endl;
	cout<<"Template/Ratio Barrel"<<endl;
	for(int i=1; i<ptbinnum+2; i++) {
		if(i!=1) cout<<",";
		cout<<(FR_template_barrel->GetBinContent(i))/(FR_xsec_barrel->GetBinContent(i));
	}
	cout<<endl;
	cout<<"Template/Ratio Endcap"<<endl;
	for(int i=1; i<ptbinnum_endcap+2; i++) {
		if(i!=1) cout<<",";
		cout<<(FR_template_endcap->GetBinContent(i))/(FR_xsec_endcap->GetBinContent(i));
	}
	cout<<endl;

}

void setDataHist(TH1D* hist) {
	hist->SetLineWidth(2);
	hist->SetMarkerStyle(33);
	hist->SetMarkerSize(3);
	hist->SetStats(kFALSE);
	hist->Sumw2();
}

void setMCHist(TH1D* hist, const int& color) {
	hist->SetFillColor(color+2);
	hist->SetStats(kFALSE);
	hist->Sumw2();
}

TH1D* FRByTemplate(TH1D** numerator, TH1D** denominator) {

	TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_template_barrel" : "FR_template_endcap";

	TH1D* num = (TH1D*)numerator[Data1]->Clone(name);
	num->Add(numerator[Data2]);

	TH1D* den = (TH1D*)numerator[DYTauTau1030]->Clone(name+"_");
	den->Add(numerator[DYTauTau30]);
	den->Add(numerator[DYMuMu1030]);
	den->Add(numerator[DYMuMu30]);
	den->Add(numerator[DYMuMu1030_PbP]);
	den->Add(numerator[DYMuMu30_PbP]);

	num->Multiply(numerator[QCD]);

	den->Add(numerator[TT]);
	den->Add(numerator[WpMu]);
	den->Add(numerator[WmMu]);
	den->Add(numerator[WpTau]);
	den->Add(numerator[WmTau]);

	den->Add(numerator[QCD]);
	den->Add(numerator[TW]);
	den->Add(numerator[TbarW]);
	den->Add(numerator[WW]);
	den->Add(numerator[WZ]);
	den->Add(numerator[ZZ]);
	den->Multiply(denominator[QCD]);

	num->Divide(den);

	delete den;
	return num;
}

TH1D* FRByRatio(TH1D** numerator, TH1D** denominator) {

	TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_xsec_barrel" : "FR_xsec_endcap";

	TH1D* num = (TH1D*)denominator[DYTauTau1030]->Clone(name);
	num->Add(denominator[DYTauTau30]);
	num->Add(denominator[DYMuMu1030]);
	num->Add(denominator[DYMuMu30]);
	num->Add(denominator[DYMuMu1030_PbP]);
	num->Add(denominator[DYMuMu30_PbP]);

	TH1D* den = (TH1D*)numerator[DYTauTau1030]->Clone(name+"_");
	den->Add(numerator[DYTauTau30]);
	den->Add(numerator[DYMuMu1030]);
	den->Add(numerator[DYMuMu30]);
	den->Add(numerator[DYMuMu1030_PbP]);
	den->Add(numerator[DYMuMu30_PbP]);

	num->Add(denominator[TT]);
	num->Add(denominator[WpMu]);
	num->Add(denominator[WmMu]);
	num->Add(denominator[WpTau]);
	num->Add(denominator[WmTau]);

	num->Add(denominator[QCD]);
	num->Add(denominator[TW]);
	num->Add(denominator[TbarW]);
	num->Add(denominator[WW]);
	num->Add(denominator[WZ]);
	num->Add(denominator[ZZ]);
	num->Multiply(numerator[QCD]);

	TH1D *num_dataAll = (TH1D*) numerator[Data1]->Clone(name+"_tmp");
	num_dataAll->Add(numerator[Data2]);
	num->Multiply(num_dataAll);

	den->Add(numerator[TT]);
	den->Add(numerator[WpMu]);
	den->Add(numerator[WmMu]);
	den->Add(numerator[WpTau]);
	den->Add(numerator[WmTau]);

	den->Add(numerator[QCD]);
	den->Add(numerator[TW]);
	den->Add(numerator[TbarW]);
	den->Add(numerator[WW]);
	den->Add(numerator[WZ]);
	den->Add(numerator[ZZ]);
	den->Multiply(denominator[QCD]);
	TH1D *den_dataAll = (TH1D*) denominator[Data1]->Clone(name+"_tmp");
	den_dataAll->Add(denominator[Data2]);
	den->Multiply(den_dataAll);

	num->Write();
	den->Write();
	num->Divide(den);

	delete den;
	return num;
}



TH1D* FRCheck(TH1D** numerator, TH1D** denominator) {

	TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_check_barrel" : "FR_check_endcap";

	TH1D* den = (TH1D*)denominator[DYTauTau1030]->Clone(name);
	den->Add(denominator[DYTauTau30]);
	den->Add(denominator[DYMuMu1030]);
	den->Add(denominator[DYMuMu30]);
	den->Add(denominator[DYMuMu1030_PbP]);
	den->Add(denominator[DYMuMu30_PbP]);

	den->Add(denominator[TT]);
	den->Add(denominator[WpMu]);
	den->Add(denominator[WmMu]);
	den->Add(denominator[WpTau]);
	den->Add(denominator[WmTau]);

	den->Add(denominator[QCD]);
	den->Add(denominator[TW]);
	den->Add(denominator[TbarW]);
	den->Add(denominator[WW]);
	den->Add(denominator[WZ]);
	den->Add(denominator[ZZ]);
	TH1D *num = (TH1D*) denominator[Data1]->Clone(name+"_tmp");
	num->Add(denominator[Data2]);
	//num->Divide(den);
	den->Sumw2();
	num->Sumw2();
	TH1D* num2 = (TH1D*)num->Clone("num2");
	num->Divide(num,den,1,1,"B");

	int imax = (name.Contains("barrel")) ? ptbinnum : ptbinnum_endcap;
	for(int i=1; i<imax+2; i++) {
		double deninteg = den->GetBinContent(i);
		double numinteg = num2->GetBinContent(i);
		double Rinteg = numinteg/deninteg;
		double Rerrinteg = Rinteg*sqrt(1/deninteg+1/numinteg);
		cout<<num->GetBinContent(i)<<" #pm " << num->GetBinError(i) << endl;
		cout << "CONFIRM : " << Rinteg << " #pm " << Rerrinteg << endl;
	}
	cout<<endl;


	delete den;
	return num;
}


void FRStore(TFile* g, TH1D** numerator, TH1D** denominator) {

	TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "barrel" : "endcap";

	tight_Data = (TH1D*)numerator[Data1]->Clone("numerator_pt_"+name);
	tight_Data->Add(numerator[Data2]);

	loose_Data = (TH1D*)denominator[Data1]->Clone("denominator_pt_fit_"+name);
	loose_Data->Add(denominator[Data2]);

	tight_QCD = (TH1D*)numerator[QCD]->Clone("numerator_pt_"+name);
	loose_QCD = (TH1D*)denominator[QCD]->Clone("denominator_pt_fit_"+name);

//	tight_allch = (TH1D*)numerator[QCD]->Clone("numerator_pt_"+name);
	tight_allch = (TH1D*)numerator[DYTauTau1030]->Clone("numerator_pt_"+name);


//	tight_allch->Add(numerator[DYTauTau1030]);
	tight_allch->Add(numerator[DYTauTau30]);
	tight_allch->Add(numerator[DYMuMu1030]);
	tight_allch->Add(numerator[DYMuMu30]);
	tight_allch->Add(numerator[DYMuMu1030_PbP]);
	tight_allch->Add(numerator[DYMuMu30_PbP]);

	tight_allch->Add(numerator[TT]);
	tight_allch->Add(numerator[WpMu]);
	tight_allch->Add(numerator[WmMu]);
	tight_allch->Add(numerator[WpTau]);
	tight_allch->Add(numerator[WmTau]);

	tight_allch->Add(numerator[TW]);
	tight_allch->Add(numerator[TbarW]);
	tight_allch->Add(numerator[WW]);
	tight_allch->Add(numerator[WZ]);
	tight_allch->Add(numerator[ZZ]);


//	loose_allch = (TH1D*)denominator[QCD]->Clone("denominator_pt_fit_"+name);
	loose_allch = (TH1D*)denominator[DYTauTau1030]->Clone("denominator_pt_fit_"+name);


//	loose_allch->Add(denominator[DYTauTau1030]);
	loose_allch->Add(denominator[DYTauTau30]);
	loose_allch->Add(denominator[DYMuMu1030]);
	loose_allch->Add(denominator[DYMuMu30]);
	loose_allch->Add(denominator[DYMuMu1030_PbP]);
	loose_allch->Add(denominator[DYMuMu30_PbP]);

	loose_allch->Add(denominator[TT]);
	loose_allch->Add(denominator[WpMu]);
	loose_allch->Add(denominator[WmMu]);
	loose_allch->Add(denominator[WpTau]);
	loose_allch->Add(denominator[WmTau]);

	loose_allch->Add(denominator[TW]);
	loose_allch->Add(denominator[TbarW]);
	loose_allch->Add(denominator[WW]);
	loose_allch->Add(denominator[WZ]);
	loose_allch->Add(denominator[ZZ]);

	//////////////////
	ratio_Data = (TH1D*)tight_Data->Clone("denominator_pt_fit_"+name);
	ratio_Data->Divide(tight_Data,loose_Data,1,1,"B");

	ratio_QCD = (TH1D*)tight_QCD->Clone("denominator_pt_fit_"+name);
	ratio_QCD->Divide(tight_QCD,loose_QCD,1,1,"B");

	ratio_allch = (TH1D*)tight_allch->Clone("denominator_pt_fit_"+name);
	ratio_allch->Divide(tight_allch,loose_allch,1,1,"B");

	g->cd();

	tight_Data->SetName("tight_Data_"+name);
	loose_Data->SetName("loose_Data_"+name);
	ratio_Data->SetName("ratio_Data_"+name);

	tight_QCD->SetName("tight_QCD_"+name);
	loose_QCD->SetName("loose_QCD_"+name);
	ratio_QCD->SetName("ratio_QCD_"+name);

	tight_allch->SetName("tight_allch_"+name);
	loose_allch->SetName("loose_allch_"+name);
	ratio_allch->SetName("ratio_allch_"+name);

	tight_Data->Write();
	loose_Data->Write();
	ratio_Data->Write();
	tight_QCD->Write();
	loose_QCD->Write();
	ratio_QCD->Write();
	tight_allch->Write();
	loose_allch->Write();
	ratio_allch->Write();




	return;
}


