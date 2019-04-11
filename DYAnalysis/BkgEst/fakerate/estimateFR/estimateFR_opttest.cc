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
TH1D* FRBytRatio(TH1D** numerator, TH1D** denominator);

TH1D* FRCheck(TH1D** numerator, TH1D** denominator);


void estimateFR_opttest_v20190328(TString indir, TString rmk,TString rmk1,TString rmk2,int QCDopt=2, TString indir2="20190306v9_iso0_1_HFminus", TString indir3="20190402v11_iso0_1_HFminus", TString rmkf="rmkf") {
//void estimateFR_opttest_v20190328(TString indir, TString rmk,TString rmk1,TString rmk2) {



    TFile* file[NSamples+2];
	 TFile* fileSS1;
 	 TFile* fileSS2;
 

/*
	 TFile* fin_fitNch_barrel = new TFile("histograms/histFRFit_barrel_opt6_woPbPDYJets.root","read");
	 TFile* fin_fitNch_endcap = new TFile("histograms/histFRFit_endcap_opt6_woPbPDYJets.root","read");
*/
/*
	 TFile* fin_fitNch_barrel = new TFile("histograms/histFRFit_barrel_opt7_QCDDATASS2_woPbPDYJets.root","read");
	 TFile* fin_fitNch_endcap = new TFile("histograms/histFRFit_endcap_opt7_QCDDATASS2_woPbPDYJets.root","read");
*/
/*
	 TFile* fin_fitNch_barrel = new TFile("histograms/histFRFit_v2__barrel_opt16_QCDopt1_ZVETODATA_QCDin0p875Data.root","read");
	 TFile* fin_fitNch_endcap = new TFile("histograms/histFRFit_v2__endcap_opt16_QCDopt1_ZVETODATA_QCDin0p875Data.root","read");
*/
/*
	 TFile* fin_fitNch_barrel = new TFile("histograms/histFRFit_v2__barrel_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.root","read");
	 TFile* fin_fitNch_endcap = new TFile("histograms/histFRFit_v2__endcap_opt16_QCDopt2_histZVETO_SMUwJET_MuPtlt15_QCDin0p875Data.root","read");
*/
//###
//	TFile* fin_fitNch_barrel = new TFile(Form("histograms_opttest/histFRFit_barrel_%s.root",rmk.Data()),"read");
//	TFile* fin_fitNch_endcap = new TFile(Form("histograms_opttest/histFRFit_endcap_%s.root",rmk.Data()),"read");

//	TFile* fin_fitNch_barrel = new TFile(Form("histogramsFit_20190128/histFRFit_barrel_%s_QCDin0p875Data.root",rmk.Data()),"read");
//	TFile* fin_fitNch_endcap = new TFile(Form("histogramsFit_20190128/histFRFit_endcap_%s_QCDin0p875Data.root",rmk.Data()),"read");

	TFile* fin_fitNch_barrel = new TFile(Form("histogramsFit_%s/histFRFit_barrel_%s_QCDin0p875Data.root",indir.Data(),rmk.Data()),"read");
	TFile* fin_fitNch_endcap = new TFile(Form("histogramsFit_%s/histFRFit_endcap_%s_QCDin0p875Data.root",indir.Data(),rmk.Data()),"read");


//histogramsFit_20190128/histFRFit_endcap_opt15_QCDopt2_reltrkisoR03_highPtMuonID_L1DoubleMu0_QCDin0p875Data.root

	 TH1D* h_fitNch_barrel = (TH1D*)fin_fitNch_barrel->Get("h_fitNch");
	 TH1D* h_fitNch_endcap = (TH1D*)fin_fitNch_endcap->Get("h_fitNch");

/*
    for (int i=0; i<ALL; i++) file[i] = new TFile(PathFRHistos(static_cast<SampleTag>(i)));
    file[QCD] = new TFile(PathFRHistos(QCD));
*/

	 for (int i=0; i<=QCD; i++) {
		if (i==ALL) continue;
		SampleTag tag = static_cast<SampleTag>(i);
//		file[i] = new TFile(Form("histograms_20190128/histFR_%s%s.root",ntrmk.Data(),Name(tag)));
//		file[i] = new TFile(Form("histograms_20190128/histFR_%s_%s%s.root",rmk1.Data(),Name(tag),rmk2.Data()));
		file[i] = new TFile(Form("histograms_%s/histFR_%s_%s_%s.root",indir2.Data(),rmk1.Data(),Name(tag),rmk2.Data()));
//histograms_20190128/histFR_reltrkisoR03_ZZ_highPtMuonID_L1DoubleMu0.root
	 }

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
	 
    for(int i=0;i<=QCD;i++) {
       if (i==ALL) continue;
       SampleTag tag = static_cast<SampleTag>(i);
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
//       numerator_pt_endcap[i] = (TH1D*)fileSS1->Get("denominator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(tag)));


	      denominator_pt_fit_barrel[i]->Add((TH1D*)fileSS2->Get("denominator_pt_barrel"));
       denominator_pt_xsec_barrel[i]->Add((TH1D*)fileSS2->Get("denominator_pt_barrel"));
       denominator_barrel[i]->Add((TH1D*)fileSS2->Get("denominator_barrel"));
       numerator_pt_barrel[i]->Add((TH1D*)fileSS2->Get("numerator_pt_barrel"));
       denominator_pt_fit_endcap[i]->Add((TH1D*)fileSS2->Get("denominator_pt_endcap"));
       denominator_pt_xsec_endcap[i]->Add((TH1D*)fileSS2->Get("denominator_pt_endcap"));
       denominator_endcap[i]->Add((TH1D*)fileSS2->Get("denominator_endcap"));
       numerator_pt_endcap[i]->Add((TH1D*)fileSS2->Get("numerator_pt_endcap"));
//       numerator_pt_endcap[i]->Add((TH1D*)fileSS2->Get("denominator_pt_endcap"));


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
//       numerator_pt_endcap[i] = (TH1D*)file[i]->Get("denominator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(tag)));


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



    //norm_fit_barrel[TT] = h_fitNch_barrel->GetBinContent(8)/denominator_barrel[TT]->Integral();
	norm_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/denominator_barrel[TT]->Integral();
	norm_pt_fit_barrel[TT] = (h_fitNch_barrel->GetBinContent(8)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/denominator_pt_fit_barrel[TT]->Integral();

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
			norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(1)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/intDY_barrel;
			norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(1)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/intDY_barrel_pt;
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
		norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(3)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/intWJets_barrel;
		norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(3)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/intWJets_barrel_pt;
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
	norm_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(4)/h_fitNch_barrel->GetBinContent(9))*dataall_barrel_integ/intTWs_barrel;
	norm_pt_fit_barrel[i] = (h_fitNch_barrel->GetBinContent(4)/h_fitNch_barrel->GetBinContent(9))*dataall_pt_barrel_integ/intTWs_barrel_pt;


    }
/*
    norm_fit_barrel[TT] = h_fitNch_barrel->GetBinContent(8)/denominator_barrel[TT]->Integral();
	 norm_fit_barrel[QCD] = h_fitNch_barrel->GetBinContent(2)/denominator_barrel[QCD]->Integral();
//	 norm_fit_barrel[TW] = h_fitNch_barrel->GetBinContent(4)/denominator_barrel[TW]->Integral();
    norm_fit_barrel[WW] = h_fitNch_barrel->GetBinContent(5)/denominator_barrel[WW]->Integral();
    norm_fit_barrel[WZ] = h_fitNch_barrel->GetBinContent(6)/denominator_barrel[WZ]->Integral();
    norm_fit_barrel[ZZ] = h_fitNch_barrel->GetBinContent(7)/denominator_barrel[ZZ]->Integral();
*/
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







	 std::cout << "#######################" << std::endl;
	 std::cout << "norm_fit_barrel[QCD]: "<< norm_fit_barrel[QCD] << std::endl;

    for (int i=DYTauTau1030; i<=DYMuMu30_PbP; i++) {
//   		 if (i==DYMuMu1030_PbP || i==DYMuMu30_PbP) continue;
//	   norm_fit_endcap[i] = h_fitNch_endcap->GetBinContent(1)/intDY_endcap;
	norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(1)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/intDY_endcap;
	norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(1)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/intDY_endcap_pt;
    }
//    norm_fit_endcap[TT] = h_fitNch_endcap->GetBinContent(8)/denominator_endcap[TT]->Integral();
	norm_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[TT]->Integral();
	norm_pt_fit_endcap[TT] = (h_fitNch_endcap->GetBinContent(8)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[TT]->Integral();


    for (int i=WpMu; i<=WmTau; i++) {
//       norm_fit_endcap[i] = h_fitNch_endcap->GetBinContent(3)/intWJets_endcap;
 	norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(3)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/intWJets_endcap;
	norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(3)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/intWJets_endcap_pt;

 
  }
    for (int i=TW; i<=TbarW; i++) {
//       norm_fit_endcap[i] = h_fitNch_endcap->GetBinContent(4)/intTWs_endcap;
	norm_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(4)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/intTWs_endcap;
	norm_pt_fit_endcap[i] = (h_fitNch_endcap->GetBinContent(4)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/intTWs_endcap_pt;


    }

/*
    norm_fit_endcap[QCD] = h_fitNch_endcap->GetBinContent(2)/denominator_endcap[QCD]->Integral();
//	 norm_fit_endcap[TW] = h_fitNch_endcap->GetBinContent(4)/denominator_endcap[TW]->Integral();
    norm_fit_endcap[WW] = h_fitNch_endcap->GetBinContent(5)/denominator_endcap[WW]->Integral();
    norm_fit_endcap[WZ] = h_fitNch_endcap->GetBinContent(6)/denominator_endcap[WZ]->Integral();
    norm_fit_endcap[ZZ] = h_fitNch_endcap->GetBinContent(7)/denominator_endcap[ZZ]->Integral();
*/
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


    norm_fit_endcap[Data1] = 1.0;
    norm_fit_endcap[Data2] = 1.0;
    norm_fit_barrel[Data1] = 1.0;
    norm_fit_barrel[Data2] = 1.0;

    norm_pt_fit_endcap[Data1] = 1.0;
    norm_pt_fit_endcap[Data2] = 1.0;
    norm_pt_fit_barrel[Data1] = 1.0;
    norm_pt_fit_barrel[Data2] = 1.0;



//    norm_all[Data1] = (Xsec(tag)*lumi_all)/Nevts(tag);



	  for(int i=0;i<=QCD;i++) {
       if (i==ALL) continue;
       SampleTag tag = static_cast<SampleTag>(i);
       denominator_barrel[i]->Scale(norm_fit_barrel[i]);
//       denominator_pt_fit_barrel[i]->Scale(norm_fit_barrel[i]);
		 denominator_pt_fit_barrel[i]->Scale(norm_pt_fit_barrel[i]);
//       denominator_pt_xsec_barrel[i]->Scale(norm_all[i]);
//       numerator_pt_barrel[i]->Scale(norm_all[i]);

		if (i==QCD) numerator_pt_barrel[i]->Scale(norm_pt_fit_barrel[i]);

       denominator_endcap[i]->Scale(norm_fit_endcap[i]);
//       denominator_pt_fit_endcap[i]->Scale(norm_fit_endcap[i]);
		 denominator_pt_fit_endcap[i]->Scale(norm_pt_fit_endcap[i]);
//       denominator_pt_xsec_endcap[i]->Scale(norm_all[i]);
//       numerator_pt_endcap[i]->Scale(norm_all[i]);
		if (i==QCD) numerator_pt_endcap[i]->Scale(norm_pt_fit_endcap[i]);
//	norm_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9))*dataall_endcap_integ/denominator_endcap[QCD]->Integral();
//	norm_pt_fit_endcap[QCD] = (h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9))*dataall_pt_endcap_integ/denominator_pt_fit_endcap[QCD]->Integral();

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

    TH1D* FR_xsec_barrel = (TH1D*)FRBytRatio(numerator_pt_barrel, denominator_pt_xsec_barrel);
    TH1D* FR_xsec_endcap = (TH1D*)FRBytRatio(numerator_pt_endcap, denominator_pt_xsec_endcap);
*/
//    TH1D* FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
//    TH1D* FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);

//    TH1D* FR_template_barrel = (TH1D*)FRByTemplate(denominator_pt_fit_barrel, denominator_pt_fit_barrel);
//    TH1D* FR_template_endcap = (TH1D*)FRByTemplate(denominator_pt_fit_endcap, denominator_pt_fit_endcap);

    TH1D* FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
    TH1D* FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);


    TH1D* FR_xsec_barrel = (TH1D*)FRBytRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
    TH1D* FR_xsec_endcap = (TH1D*)FRBytRatio(numerator_pt_endcap, denominator_pt_fit_endcap);

	 std::cout << "### CHECK UNITY ###" << std::endl;
	 double num_uni = denominator_pt_fit_barrel[Data1]->Integral()+denominator_pt_fit_barrel[Data2]->Integral();
	 double den_uni = 0.0;
	 for (int i=0; i<=QCD; i++) {
		if (i==ALL || i==Data1 || i==Data2) continue;
		den_uni+=denominator_pt_fit_barrel[i]->Integral();
	}
	std::cout << "num_uni : " << num_uni << std::endl;
	std::cout << "den_uni : " << den_uni << std::endl;


//    TH1D* FR_template_barrel = (TH1D*)FRBytRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
//    TH1D* FR_template_endcap = (TH1D*)FRBytRatio(numerator_pt_endcap, denominator_pt_fit_endcap);


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
	 
    FR_template_barrel->Write();
    FR_template_endcap->Write();
    FR_xsec_barrel->Write();
    FR_xsec_endcap->Write();

    g->Close();

    cout<<"Template Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_template_barrel->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Template Endcap"<<endl;
    for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_template_endcap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Ratio Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_xsec_barrel->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Ratio Endcap"<<endl;
        for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_xsec_endcap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Template/Ratio Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<(FR_template_barrel->GetBinContent(i))/(FR_xsec_barrel->GetBinContent(i));
    }
    cout<<endl;
    cout<<"Template/Ratio Endcap"<<endl;
        for(int i=1; i<ptbinnum_endcap+1; i++) {
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

TH1D* FRBytRatio(TH1D** numerator, TH1D** denominator) {

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

    num->Divide(den);

    delete den;
    return num;
}

TH1D* FRCheck(TH1D** numerator, TH1D** denominator) {

    TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_check_barrel" : "FR_check_endcap";

    TH1D* num = (TH1D*)denominator[DYTauTau1030]->Clone(name);
    num->Add(denominator[DYTauTau30]);
    num->Add(denominator[DYMuMu1030]);
    num->Add(denominator[DYMuMu30]);
    num->Add(denominator[DYMuMu1030_PbP]);
    num->Add(denominator[DYMuMu30_PbP]);

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

    TH1D *den = (TH1D*) denominator[Data1]->Clone(name+"_tmp");
    den->Add(denominator[Data2]);

    num->Divide(den);

    delete den;
    return num;
}
