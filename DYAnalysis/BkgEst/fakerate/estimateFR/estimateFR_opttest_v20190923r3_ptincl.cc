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

TH1D* tight_QCD_barrel;
TH1D* loose_QCD_barrel;
TH1D* ratio_QCD_barrel;

TH1D* denominator_pt_barrel[NSamples+2];
TH1D* denominator_pt_endcap[NSamples+2];
TH1D* numerator_pt_barrel[NSamples+2];
TH1D* numerator_pt_endcap[NSamples+2];


TH1D* test_barrel_denominator_QCDtemplate;
TH1D* test_barrel_numerator_QCDtemplate;

//void estimateFR_opttest_v20190909_ptdif(TString indir="20190909v60_iso0_0p2_nominalfixed_test",TString indiriso="20190909v60_iso0_1_nominalfixed_test",TString rmk="opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12",TString rmk1="reltrkisoR03muptlt10",TString rmk2="L3Mu12",int QCDopt=2,TString indir2="20190901v50_iso0_1_nominal",TString indir3="20190909v60_iso0_0p2_nominalfixed_test",TString rmkf="reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_FRopt205",int FRopt=205) {
//void estimateFR_opttest_v20190909_ptdif(TString indir="20190909v60_iso0_0p2_nominalfixed_test",TString indiriso="20190909v60_iso0_1_nominalfixed_test",TString rmk="opt1050_QCDopt1_reltrkisoR03muptlt10_L3Mu12",TString rmk1="reltrkisoR03muptlt10",TString rmk2="L3Mu12",int QCDopt=1,TString indir2="20190901v50_iso0_1_nominal",TString indir3="20190909v60_iso0_0p2_nominalfixed_test",TString rmkf="reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt1_L3Mu12_FRopt205",int FRopt=205) {
void estimateFR_opttest_v20190923r3_ptincl(TString indir="20190909v60_iso0_0p2_nominalfixed_test",TString indiriso="20190909v60_iso0_1_nominalfixed_test",TString rmk="opt1050_QCDopt2_reltrkisoR03muptlt10_L3Mu12",TString rmk1="reltrkisoR03muptlt10",TString rmk2="L3Mu12",int QCDopt=2,TString indir2="20190901v50_iso0_1_nominal",TString indir3="20190909v60_iso0_0p2_nominalfixed_test",TString rmkf="reltrkisoR03muptlt10isomax0p2_opt1050_QCDopt2_L3Mu12_FRopt201",int FRopt=201) {



	TFile* file[NSamples+2];
	TFile* fileSS1;
	TFile* fileSS2;

	TFile* fin_fitNch_barrel = new TFile(Form("histogramsFit_%s/histFRFit_barrel_%s.root",indir.Data(),rmk.Data()),"read");
	TFile* fin_fitNch_endcap = new TFile(Form("histogramsFit_%s/histFRFit_endcap_%s.root",indir.Data(),rmk.Data()),"read");

	TFile* fin_fitNch_barrel_iso = new TFile(Form("histogramsFit_%s/histFRFit_barrel_%s.root",indiriso.Data(),rmk.Data()),"read");
	TFile* fin_fitNch_endcap_iso = new TFile(Form("histogramsFit_%s/histFRFit_endcap_%s.root",indiriso.Data(),rmk.Data()),"read");

	TFile* g = new TFile(Form("resultEst_%s/fakerate_%s.root",indir3.Data(),rmkf.Data()),"RECREATE");


	for (int k=0; k<=QCD; k++) {
		if (k==ALL) continue;
		SampleTag tag = static_cast<SampleTag>(k);
		file[k] = new TFile(Form("histograms_%s/histFR_%s_%s_%s.root",indir2.Data(),rmk1.Data(),Name(tag),rmk2.Data()),"read");
		fileSS1 = new TFile(Form("histograms_%s/histFR_SS%s_Data1_%s.root",indir2.Data(),rmk1.Data(),rmk2.Data()));
		fileSS2 = new TFile(Form("histograms_%s/histFR_SS%s_Data2_%s.root",indir2.Data(),rmk1.Data(),rmk2.Data()));
	}

	TH1D* tight_allch_barrel = new TH1D("tight_allch_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* loose_allch_barrel = new TH1D("loose_allch_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* ratio_allch_barrel = new TH1D("ratio_allch_barrel",";Single muon's p{T} (GeV/c);Ratio",ptbinnum,ptbin);

	TH1D* tight_Data_barrel = new TH1D("tight_Data_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* loose_Data_barrel = new TH1D("loose_Data_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* ratio_Data_barrel = new TH1D("ratio_Data_barrel",";Single muon's p{T} (GeV/c);Ratio",ptbinnum,ptbin);

	TH1D* tight_QCD_barrel = new TH1D("tight_QCD_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* loose_QCD_barrel = new TH1D("loose_QCD_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* ratio_QCD_barrel = new TH1D("ratio_QCD_barrel",";Single muon's p{T} (GeV/c);Ratio",ptbinnum,ptbin);

	TH1D* tight_QCDori_barrel = new TH1D("tight_QCDori_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* loose_QCDori_barrel = new TH1D("loose_QCDori_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* ratio_QCDori_barrel = new TH1D("ratio_QCDori_barrel",";Single muon's p{T} (GeV/c);Ratio",ptbinnum,ptbin);

	TH1D* tight_QCDSS_barrel = new TH1D("tight_QCDSS_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* loose_QCDSS_barrel = new TH1D("loose_QCDSS_barrel",";Single muon's p{T} (GeV/c);Entries",ptbinnum,ptbin);
	TH1D* ratio_QCDSS_barrel = new TH1D("ratio_QCDSS_barrel",";Single muon's p{T} (GeV/c);Ratio",ptbinnum,ptbin);



	TH1D* tight_allch_endcap = new TH1D("tight_allch_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* loose_allch_endcap = new TH1D("loose_allch_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* ratio_allch_endcap = new TH1D("ratio_allch_endcap",";Single muon's p{T} (GeV/c);Ratio",ptbinnum_endcap,ptbin_endcap);

	TH1D* tight_Data_endcap = new TH1D("tight_Data_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* loose_Data_endcap = new TH1D("loose_Data_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* ratio_Data_endcap = new TH1D("ratio_Data_endcap",";Single muon's p{T} (GeV/c);Ratio",ptbinnum_endcap,ptbin_endcap);

	TH1D* tight_QCD_endcap = new TH1D("tight_QCD_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* loose_QCD_endcap = new TH1D("loose_QCD_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* ratio_QCD_endcap = new TH1D("ratio_QCD_endcap",";Single muon's p{T} (GeV/c);Ratio",ptbinnum_endcap,ptbin_endcap);

	TH1D* tight_QCDori_endcap = new TH1D("tight_QCDori_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* loose_QCDori_endcap = new TH1D("loose_QCDori_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* ratio_QCDori_endcap = new TH1D("ratio_QCDori_endcap",";Single muon's p{T} (GeV/c);Ratio",ptbinnum_endcap,ptbin_endcap);

	TH1D* tight_QCDSS_endcap = new TH1D("tight_QCDSS_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* loose_QCDSS_endcap = new TH1D("loose_QCDSS_endcap",";Single muon's p{T} (GeV/c);Entries",ptbinnum_endcap,ptbin_endcap);
	TH1D* ratio_QCDSS_endcap = new TH1D("ratio_QCDSS_endcap",";Single muon's p{T} (GeV/c);Ratio",ptbinnum_endcap,ptbin_endcap);


	tight_allch_barrel->Sumw2();
	loose_allch_barrel->Sumw2();
	ratio_allch_barrel->Sumw2();

	tight_Data_barrel->Sumw2();
	loose_Data_barrel->Sumw2();
	ratio_Data_barrel->Sumw2();

	tight_QCD_barrel->Sumw2();
	loose_QCD_barrel->Sumw2();
	ratio_QCD_barrel->Sumw2();

	tight_QCDori_barrel->Sumw2();
	loose_QCDori_barrel->Sumw2();
	ratio_QCDori_barrel->Sumw2();

	tight_QCDSS_barrel->Sumw2();
	loose_QCDSS_barrel->Sumw2();
	ratio_QCDSS_barrel->Sumw2();


	tight_allch_endcap->Sumw2();
	loose_allch_endcap->Sumw2();
	ratio_allch_endcap->Sumw2();

	tight_Data_endcap->Sumw2();
	loose_Data_endcap->Sumw2();
	ratio_Data_endcap->Sumw2();

	tight_QCD_endcap->Sumw2();
	loose_QCD_endcap->Sumw2();
	ratio_QCD_endcap->Sumw2();

	tight_QCDori_endcap->Sumw2();
	loose_QCDori_endcap->Sumw2();
	ratio_QCDori_endcap->Sumw2();

	tight_QCDSS_endcap->Sumw2();
	loose_QCDSS_endcap->Sumw2();
	ratio_QCDSS_endcap->Sumw2();


	TH1D* FR_template_barrel = new TH1D("FR_template_barrel",";Single muon's p{T} (GeV/c);Ratio",ptbinnum,ptbin);
	TH1D* FR_template_endcap = new TH1D("FR_template_endcap",";Single muon's p{T} (GeV/c);Ratio",ptbinnum_endcap,ptbin_endcap);

	FR_template_barrel->Sumw2();
	FR_template_endcap->Sumw2();





	for (int i=0;i<ptbinnum;i++) {
//		TH1D* h_fitNch_barrel = (TH1D*)fin_fitNch_barrel->Get(Form("h_fitNch_barrel_ptbin%d",i));
//		TH1D* h_fitNch_barrel_iso = (TH1D*)fin_fitNch_barrel_iso->Get(Form("h_fitNch_barrel_ptbin%d",i));
		TH1D* h_fitNch_barrel = (TH1D*)fin_fitNch_barrel->Get("h_fitNch_barrel");
		TH1D* h_fitNch_barrel_iso = (TH1D*)fin_fitNch_barrel_iso->Get("h_fitNch_barrel");


		std::cout << "ptbin : " << i << std::endl;
		double tmp_tight_allch_barrel=0.0;
		double tmp_tight_allch_barrel_error=0.0;

		for (int j=1;j<9;j++) {
			if (j==2) continue;
			tmp_tight_allch_barrel+=h_fitNch_barrel->GetBinContent(j);
			tmp_tight_allch_barrel_error+=h_fitNch_barrel->GetBinContent(j+15);
		//std::cout << "OK?" << std::endl;
	
		}

		double tmp_loose_allch_barrel=0.0;
		double tmp_loose_allch_barrel_error=0.0;
		for (int j=TT;j<=QCD;j++) {
			SampleTag tag = static_cast<SampleTag>(j);
			std::cout << "tag : " << Name(tag) << std::endl;
			if (j==ALL) continue;
			std::cout << tag << std::endl;
			double norm_tmp;
			if ((j==DYMuMu1030_PbP || j==DYMuMu30_PbP)) {
				norm_tmp = (Xsec(tag)*lumi_part1);
			}
			else if ((j==DYMuMu1030 || j==DYMuMu30)){
				norm_tmp = (Xsec(tag)*lumi_part2);
			}
			else if ((j==Data1 || j==Data2)){
				norm_tmp = 1.0;
			}
			else norm_tmp = (Xsec(tag)*lumi_all);
			denominator_pt_barrel[j] = (TH1D*)file[j]->Get("denominator_pt_barrel")->Clone("denominator_pt_fit_barrel"+TString(Name(tag)));
			numerator_pt_barrel[j] = (TH1D*)file[j]->Get("numerator_pt_barrel")->Clone("numerator_pt_barrel"+TString(Name(tag)));

			if (j==QCD) {
						loose_QCDori_barrel = (TH1D*)file[QCD]->Get("denominator_pt_barrel")->Clone("denominator_pt_fit_barrel"+TString(Name(tag)));
				tight_QCDori_barrel = (TH1D*)file[QCD]->Get("numerator_pt_barrel")->Clone("numerator_pt_barrel"+TString(Name(tag)));

				loose_QCDSS_barrel = (TH1D*)fileSS1->Get("denominator_pt_barrel")->Clone("denominator_pt_fit_barrel"+TString(Name(Data1)));
				loose_QCDSS_barrel->Add((TH1D*)fileSS2->Get("denominator_pt_barrel"));
				tight_QCDSS_barrel = (TH1D*)fileSS1->Get("numerator_pt_barrel")->Clone("denominator_pt_barrel"+TString(Name(Data1)));
				tight_QCDSS_barrel->Add((TH1D*)fileSS2->Get("numerator_pt_barrel"));
			}

			if (j!=Data1 && j!=Data2 && j!=QCD) {
				if (denominator_pt_barrel[j]->GetBinContent(i+1)!=0) {
					tmp_loose_allch_barrel=tmp_loose_allch_barrel+h_fitNch_barrel->GetBinContent(13)*norm_tmp*denominator_pt_barrel[j]->GetBinContent(i+1)/denominator_pt_barrel[j]->Integral();
					std::cout << "error " << (TMath::Sqrt(pow(denominator_pt_barrel[j]->GetBinError(i+1)/denominator_pt_barrel[j]->GetBinContent(i+1),2)+1/norm_tmp+1/denominator_pt_barrel[j]->Integral())) << std::endl;
					tmp_loose_allch_barrel_error=tmp_loose_allch_barrel_error+((h_fitNch_barrel->GetBinContent(13)*norm_tmp*denominator_pt_barrel[j]->GetBinContent(i+1)/denominator_pt_barrel[j]->Integral())*TMath::Sqrt(pow(h_fitNch_barrel->GetBinContent(14)/h_fitNch_barrel->GetBinContent(13),2)+pow(denominator_pt_barrel[j]->GetBinError(i+1)/denominator_pt_barrel[j]->GetBinContent(i+1),2)+1/norm_tmp+1/denominator_pt_barrel[j]->Integral()));
				}
				else {std::cout << "NONE entry - j : " << j << std::endl;}
				//std::cout << "tmp_loose_allch_barrel: " << tmp_loose_allch_barrel << std::endl;
				//std::cout << "tmp_loose_allch_barrel_error: " << tmp_loose_allch_barrel_error << std::endl;


			}
		}//j
		std::cout << "##########################################" << std::endl;
		//std::cout << "tmp_loose_allch_barrel: " << tmp_loose_allch_barrel << std::endl;
		//std::cout << "tmp_loose_allch_barrel_error: " << tmp_loose_allch_barrel_error << std::endl;


		loose_allch_barrel->SetBinContent(i+1,tmp_loose_allch_barrel);
		loose_allch_barrel->SetBinError(i+1,tmp_loose_allch_barrel_error);

		std::cout << " QCD fraction " << h_fitNch_barrel->GetBinContent(2)/(numerator_pt_barrel[Data1]->GetBinContent(i+1)+numerator_pt_barrel[Data2]->GetBinContent(i+1)) << std::endl; 
		std::cout << h_fitNch_barrel->GetBinContent(2)/h_fitNch_barrel->GetBinContent(9)*(denominator_pt_barrel[Data1]->GetBinContent(i+1)+denominator_pt_barrel[Data2]->GetBinContent(i+1)) << " - " << h_fitNch_barrel_iso->GetBinContent(2) << std::endl;
		loose_QCD_barrel->SetBinContent(i+1,h_fitNch_barrel->GetBinContent(2)/h_fitNch_barrel->GetBinContent(9)*(denominator_pt_barrel[Data1]->GetBinContent(i+1)+denominator_pt_barrel[Data2]->GetBinContent(i+1)));
		loose_QCD_barrel->SetBinError(i+1,(h_fitNch_barrel->GetBinContent(2)/h_fitNch_barrel->GetBinContent(9)*(denominator_pt_barrel[Data1]->GetBinContent(i+1)+denominator_pt_barrel[Data2]->GetBinContent(i+1)))*TMath::Sqrt(pow(h_fitNch_barrel->GetBinContent(17)/h_fitNch_barrel->GetBinContent(2),2)+1/h_fitNch_barrel->GetBinContent(9)+pow(((denominator_pt_barrel[Data1]->GetBinError(i+1)+denominator_pt_barrel[Data2]->GetBinError(i+1)))/(denominator_pt_barrel[Data1]->GetBinContent(i+1)+denominator_pt_barrel[Data2]->GetBinContent(i+1)),2)));

		loose_Data_barrel->SetBinContent(i+1,denominator_pt_barrel[Data1]->GetBinContent(i+1)+denominator_pt_barrel[Data2]->GetBinContent(i+1));
		loose_Data_barrel->SetBinError(i+1,denominator_pt_barrel[Data1]->GetBinError(i+1)+denominator_pt_barrel[Data2]->GetBinError(i+1));

		tight_allch_barrel->SetBinContent(i+1,tmp_tight_allch_barrel);
		tight_allch_barrel->SetBinError(i+1,tmp_tight_allch_barrel_error);
		tight_QCD_barrel->SetBinContent(i+1,h_fitNch_barrel->GetBinContent(2));
		tight_QCD_barrel->SetBinError(i+1,h_fitNch_barrel->GetBinContent(17));
		tight_Data_barrel->SetBinContent(i+1,h_fitNch_barrel->GetBinContent(9));
		tight_Data_barrel->SetBinError(i+1,h_fitNch_barrel->GetBinContent(24));

		std::cout << "loose h_data : " << h_fitNch_barrel->GetBinContent(9) << std::endl;
		std::cout << "loose data1+data2 : " << denominator_pt_barrel[Data1]->GetBinContent(i+1)+denominator_pt_barrel[Data2]->GetBinContent(i+1) << std::endl;
		std::cout << "tight data1+data2 : " << numerator_pt_barrel[Data1]->GetBinContent(i+1)+numerator_pt_barrel[Data2]->GetBinContent(i+1) << std::endl;
		double tmpratio = (numerator_pt_barrel[Data1]->GetBinContent(i+1)+numerator_pt_barrel[Data2]->GetBinContent(i+1))/(denominator_pt_barrel[Data1]->GetBinContent(i+1)+denominator_pt_barrel[Data2]->GetBinContent(i+1));
		std::cout << "data ratio : " << tmpratio << std::endl;


	}//barrel

	ratio_allch_barrel=(TH1D*)tight_allch_barrel->Clone("ratio_allch_barrel");
	ratio_allch_barrel->Divide(tight_allch_barrel,loose_allch_barrel,1,1,"B");
	ratio_QCD_barrel=(TH1D*)tight_QCD_barrel->Clone("ratio_QCD_barrel");
	ratio_QCD_barrel->Divide(tight_QCD_barrel,loose_QCD_barrel,1,1,"B");
	ratio_Data_barrel=(TH1D*)tight_Data_barrel->Clone("ratio_Data_barrel");
	ratio_Data_barrel->Divide(tight_Data_barrel,loose_Data_barrel,1,1,"B");



	for (int i=0;i<ptbinnum_endcap;i++) {

		TH1D* h_fitNch_endcap = (TH1D*)fin_fitNch_endcap->Get("h_fitNch_endcap");
		TH1D* h_fitNch_endcap_iso = (TH1D*)fin_fitNch_endcap_iso->Get("h_fitNch_endcap");


		std::cout << "ptbin : " << i << std::endl;
		double tmp_tight_allch_endcap=0.0;
		double tmp_tight_allch_endcap_error=0.0;
		for (int j=1;j<9;j++) {
			if (j==2) continue;
			tmp_tight_allch_endcap+=h_fitNch_endcap->GetBinContent(j);

			tmp_tight_allch_endcap_error+=h_fitNch_endcap->GetBinContent(j+15);

		}

		double tmp_loose_allch_endcap=0.0;
		double tmp_loose_allch_endcap_error=0.0;
		for (int j=TT;j<=QCD;j++) {
			SampleTag tag = static_cast<SampleTag>(j);

			if (j==ALL) continue;
			std::cout << tag << std::endl;
			double norm_tmp;
			if ((j==DYMuMu1030_PbP || j==DYMuMu30_PbP)) {
				norm_tmp = (Xsec(tag)*lumi_part1);
			}
			else if ((j==DYMuMu1030 || j==DYMuMu30)){
				norm_tmp = (Xsec(tag)*lumi_part2);
			}
			else if ((j==Data1 || j==Data2)){
				norm_tmp = 1.0;
			}
			else norm_tmp = (Xsec(tag)*lumi_all);
			denominator_pt_endcap[j] = (TH1D*)file[j]->Get("denominator_pt_endcap")->Clone("denominator_pt_fit_endcap"+TString(Name(tag)));
			numerator_pt_endcap[j] = (TH1D*)file[j]->Get("numerator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(tag)));

			if (j==QCD) {
			loose_QCDori_endcap = (TH1D*)file[QCD]->Get("denominator_pt_endcap")->Clone("denominator_pt_fit_endcap"+TString(Name(tag)));
				tight_QCDori_endcap = (TH1D*)file[QCD]->Get("numerator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(tag)));


				loose_QCDSS_endcap = (TH1D*)fileSS1->Get("denominator_pt_endcap")->Clone("denominator_pt_fit_endcap"+TString(Name(Data1)));
				loose_QCDSS_endcap->Add((TH1D*)fileSS2->Get("denominator_pt_endcap"));
				tight_QCDSS_endcap = (TH1D*)fileSS1->Get("numerator_pt_endcap")->Clone("denominator_pt_endcap"+TString(Name(Data1)));
				tight_QCDSS_endcap->Add((TH1D*)fileSS2->Get("numerator_pt_endcap"));
	
			}


			if (j!=Data1 && j!=Data2 && j!=QCD) {
				if (denominator_pt_endcap[j]->GetBinContent(i+1)!=0) {
					tmp_loose_allch_endcap=tmp_loose_allch_endcap+h_fitNch_endcap->GetBinContent(13)*norm_tmp*denominator_pt_endcap[j]->GetBinContent(i+1)/denominator_pt_endcap[j]->Integral();
					std::cout << "error " << (TMath::Sqrt(pow(denominator_pt_endcap[j]->GetBinError(i+1)/denominator_pt_endcap[j]->GetBinContent(i+1),2)+1/norm_tmp+1/denominator_pt_endcap[j]->Integral())) << std::endl;
					tmp_loose_allch_endcap_error=tmp_loose_allch_endcap_error+((h_fitNch_endcap->GetBinContent(13)*norm_tmp*denominator_pt_endcap[j]->GetBinContent(i+1)/denominator_pt_endcap[j]->Integral())*TMath::Sqrt(pow(h_fitNch_endcap->GetBinContent(14)/h_fitNch_endcap->GetBinContent(13),2)+pow(denominator_pt_endcap[j]->GetBinError(i+1)/denominator_pt_endcap[j]->GetBinContent(i+1),2)+1/norm_tmp+1/denominator_pt_endcap[j]->Integral()));
				}
				else {std::cout << "NONE entry - j : " << j << std::endl;}
				std::cout << "tmp_loose_allch_endcap: " << tmp_loose_allch_endcap << std::endl;
				std::cout << "tmp_loose_allch_endcap_error: " << tmp_loose_allch_endcap_error << std::endl;


			}
		}//j
		std::cout << "##########################################" << std::endl;
		std::cout << "tmp_loose_allch_endcap: " << tmp_loose_allch_endcap << std::endl;
		std::cout << "tmp_loose_allch_endcap_error: " << tmp_loose_allch_endcap_error << std::endl;


		loose_allch_endcap->SetBinContent(i+1,tmp_loose_allch_endcap);
		loose_allch_endcap->SetBinError(i+1,tmp_loose_allch_endcap_error);

		loose_QCD_endcap->SetBinContent(i+1,h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9)*(denominator_pt_endcap[Data1]->GetBinContent(i+1)+denominator_pt_endcap[Data2]->GetBinContent(i+1)));
		std::cout << "loose h_data : " << h_fitNch_endcap->GetBinContent(9) << std::endl;
		std::cout << "loose data1+data2 : " << denominator_pt_endcap[Data1]->GetBinContent(i+1)+denominator_pt_endcap[Data2]->GetBinContent(i+1) << std::endl;
		std::cout << "tight data1+data2 : " << numerator_pt_endcap[Data1]->GetBinContent(i+1)+numerator_pt_endcap[Data2]->GetBinContent(i+1) << std::endl;
		double tmpratio = (numerator_pt_endcap[Data1]->GetBinContent(i+1)+numerator_pt_endcap[Data2]->GetBinContent(i+1))/(denominator_pt_endcap[Data1]->GetBinContent(i+1)+denominator_pt_endcap[Data2]->GetBinContent(i+1));
		std::cout << "ratio : " << tmpratio << std::endl;

		loose_QCD_endcap->SetBinError(i+1,(h_fitNch_endcap->GetBinContent(2)/h_fitNch_endcap->GetBinContent(9)*(denominator_pt_endcap[Data1]->GetBinContent(i+1)+denominator_pt_endcap[Data2]->GetBinContent(i+1)))*TMath::Sqrt(pow(h_fitNch_endcap->GetBinContent(17)/h_fitNch_endcap->GetBinContent(2),2)+1/h_fitNch_endcap->GetBinContent(9)+pow(((denominator_pt_endcap[Data1]->GetBinError(i+1)+denominator_pt_endcap[Data2]->GetBinError(i+1)))/(denominator_pt_endcap[Data1]->GetBinContent(i+1)+denominator_pt_endcap[Data2]->GetBinContent(i+1)),2)));

		loose_Data_endcap->SetBinContent(i+1,denominator_pt_endcap[Data1]->GetBinContent(i+1)+denominator_pt_endcap[Data2]->GetBinContent(i+1));
		loose_Data_endcap->SetBinError(i+1,denominator_pt_endcap[Data1]->GetBinError(i+1)+denominator_pt_endcap[Data2]->GetBinError(i+1));

		tight_allch_endcap->SetBinContent(i+1,tmp_tight_allch_endcap);
		tight_allch_endcap->SetBinError(i+1,tmp_tight_allch_endcap_error);
		tight_QCD_endcap->SetBinContent(i+1,h_fitNch_endcap->GetBinContent(2));
		tight_QCD_endcap->SetBinError(i+1,h_fitNch_endcap->GetBinContent(17));
		tight_Data_endcap->SetBinContent(i+1,h_fitNch_endcap->GetBinContent(9));
		tight_Data_endcap->SetBinError(i+1,h_fitNch_endcap->GetBinContent(24));

	}//endcap
	ratio_allch_endcap=(TH1D*)tight_allch_endcap->Clone("ratio_allch_endcap");
	ratio_allch_endcap->Divide(tight_allch_endcap,loose_allch_endcap,1,1,"B");
	ratio_QCD_endcap=(TH1D*)tight_QCD_endcap->Clone("ratio_QCD_endcap");
	ratio_QCD_endcap->Divide(tight_QCD_endcap,loose_QCD_endcap,1,1,"B");
	ratio_Data_endcap=(TH1D*)tight_Data_endcap->Clone("ratio_Data_endcap");
	ratio_Data_endcap->Divide(tight_Data_endcap,loose_Data_endcap,1,1,"B");


	//////////////////////////////////////////////////

	TH1D* Ratio_tot_barrel;
	TH1D* Ratio_tot_endcap;
	TH1D* Ratio_num_barrel;
	TH1D* Ratio_num_endcap;
	TH1D* Ratio_den_barrel;
	TH1D* Ratio_den_endcap;

	Ratio_num_barrel = (TH1D*)tight_Data_barrel->Clone("Ratio_num_barrel");
	TH1D* tight_allch_barrel_tmp2=(TH1D*)tight_allch_barrel->Clone();
	tight_allch_barrel_tmp2->Add(tight_QCD_barrel);
	Ratio_num_barrel->Divide(tight_Data_barrel,tight_allch_barrel_tmp2,1,1,"B");
	Ratio_num_endcap = (TH1D*)tight_Data_endcap->Clone("Ratio_num_endcap");
	TH1D* tight_allch_endcap_tmp2=(TH1D*)tight_allch_endcap->Clone();
	tight_allch_endcap_tmp2->Add(tight_QCD_endcap);
	Ratio_num_endcap->Divide(tight_Data_endcap,tight_allch_endcap_tmp2,1,1,"B");
	Ratio_num_barrel->Sumw2();
	Ratio_num_endcap->Sumw2();
	Ratio_num_barrel->SetName("Ratio_num_barrel");
	Ratio_num_endcap->SetName("Ratio_num_endcap");

	Ratio_den_barrel = (TH1D*)loose_Data_barrel->Clone("Ratio_den_barrel");
	TH1D* loose_allch_barrel_tmp2=(TH1D*)loose_allch_barrel->Clone();
	loose_allch_barrel_tmp2->Add(loose_QCD_barrel);
	Ratio_den_barrel->Divide(loose_Data_barrel,loose_allch_barrel_tmp2,1,1,"B");
	Ratio_den_endcap = (TH1D*)loose_Data_endcap->Clone("Ratio_den_endcap");
	TH1D* loose_allch_endcap_tmp2=(TH1D*)loose_allch_endcap->Clone();
	loose_allch_endcap_tmp2->Add(loose_QCD_endcap);
	Ratio_den_endcap->Divide(loose_Data_endcap,loose_allch_endcap_tmp2,1,1,"B");
	Ratio_den_barrel->Sumw2();
	Ratio_den_endcap->Sumw2();
	Ratio_den_barrel->SetName("Ratio_den_barrel");
	Ratio_den_endcap->SetName("Ratio_den_endcap");

	Ratio_tot_barrel = (TH1D*)Ratio_num_barrel->Clone("Ratio_tot_barrel");
	Ratio_tot_endcap = (TH1D*)Ratio_num_endcap->Clone("Ratio_tot_endcap");
	Ratio_tot_barrel->Sumw2();
	Ratio_tot_endcap->Sumw2();
	Ratio_tot_barrel->Divide(Ratio_num_barrel,Ratio_den_barrel,1,1,"B");
	Ratio_tot_endcap->Divide(Ratio_num_endcap,Ratio_den_endcap,1,1,"B");
	Ratio_tot_barrel->SetName("Ratio_tot_barrel");
	Ratio_tot_endcap->SetName("Ratio_tot_endcap");

	ratio_QCDori_barrel = (TH1D*)tight_QCDori_barrel->Clone("ratio_QCDori_barrel");
	ratio_QCDori_endcap = (TH1D*)tight_QCDori_endcap->Clone("ratio_QCDori_endcap");
	ratio_QCDori_barrel->Sumw2();
	ratio_QCDori_endcap->Sumw2();
	ratio_QCDori_barrel->Divide(tight_QCDori_barrel,loose_QCDori_barrel,1,1,"B");
	ratio_QCDori_endcap->Divide(tight_QCDori_endcap,loose_QCDori_endcap,1,1,"B");
	ratio_QCDori_barrel->SetName("ratio_QCDori_barrel");
	ratio_QCDori_endcap->SetName("ratio_QCDori_endcap");

	ratio_QCDSS_barrel = (TH1D*)tight_QCDSS_barrel->Clone("ratio_QCDSS_barrel");
	ratio_QCDSS_endcap = (TH1D*)tight_QCDSS_endcap->Clone("ratio_QCDSS_endcap");
	ratio_QCDSS_barrel->Sumw2();
	ratio_QCDSS_endcap->Sumw2();
	ratio_QCDSS_barrel->Divide(tight_QCDSS_barrel,loose_QCDSS_barrel,1,1,"B");
	ratio_QCDSS_endcap->Divide(tight_QCDSS_endcap,loose_QCDSS_endcap,1,1,"B");
	ratio_QCDSS_barrel->SetName("ratio_QCDSS_barrel");
	ratio_QCDSS_endcap->SetName("ratio_QCDSS_endcap");

	if (FRopt==101) {
		FR_template_barrel = (TH1D*)ratio_QCDori_barrel->Clone();
		FR_template_endcap = (TH1D*)ratio_QCDori_endcap->Clone();
	}
	else if (FRopt==102) {
		FR_template_barrel = (TH1D*)ratio_QCDSS_barrel->Clone();
		FR_template_endcap = (TH1D*)ratio_QCDSS_endcap->Clone();
	}


	else if (FRopt==200) {
		FR_template_barrel = (TH1D*)Ratio_num_barrel->Clone();
		FR_template_barrel->Multiply(tight_QCD_barrel);
		FR_template_barrel->Divide(loose_QCD_barrel);

		FR_template_endcap = (TH1D*)Ratio_num_endcap->Clone();
		FR_template_endcap->Multiply(tight_QCD_endcap);
		FR_template_endcap->Divide(loose_QCD_endcap);
	}
	else if (FRopt==201) {
		FR_template_barrel = (TH1D*)Ratio_tot_barrel->Clone();
		FR_template_barrel->Multiply(tight_QCD_barrel);
		FR_template_barrel->Divide(loose_QCD_barrel);

		FR_template_endcap = (TH1D*)Ratio_tot_endcap->Clone();
		FR_template_endcap->Multiply(tight_QCD_endcap);
		FR_template_endcap->Divide(loose_QCD_endcap);
	}
	else if (FRopt==202) {
		FR_template_barrel = (TH1D*)tight_QCD_barrel->Clone();
		FR_template_barrel->Divide(loose_QCD_barrel);

		FR_template_endcap = (TH1D*)tight_QCD_endcap->Clone();
		FR_template_endcap->Divide(loose_QCD_endcap);
	}
	else if (FRopt==203) {
		FR_template_barrel = (TH1D*)tight_QCD_barrel->Clone();
		FR_template_barrel->Divide(loose_QCD_barrel);
		FR_template_barrel->Divide(Ratio_den_barrel);

		FR_template_endcap = (TH1D*)tight_QCD_endcap->Clone();
		FR_template_endcap->Divide(loose_QCD_endcap);
		FR_template_endcap->Divide(Ratio_den_endcap);
	}
	else if (FRopt==204) {
		FR_template_barrel = (TH1D*)tight_QCD_barrel->Clone();
		FR_template_barrel->Divide(loose_QCD_barrel);
		FR_template_barrel->Multiply(Ratio_num_barrel);

		FR_template_endcap = (TH1D*)tight_QCD_endcap->Clone();
		FR_template_endcap->Divide(loose_QCD_endcap);
		FR_template_endcap->Multiply(Ratio_num_endcap);
	}
	else if (FRopt==205) {
		FR_template_barrel = (TH1D*)tight_QCD_barrel->Clone();
		FR_template_barrel->Divide(loose_QCD_barrel);

		FR_template_endcap = (TH1D*)tight_QCD_endcap->Clone();
		FR_template_endcap->Divide(loose_QCD_endcap);
	}
	FR_template_barrel->SetName("FR_template_barrel");
	FR_template_endcap->SetName("FR_template_endcap");



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
	//	FR_xsec_barrel->SetMarkerSize(3);
	//	FR_xsec_endcap->SetMarkerSize(3);

	FR_template_barrel->SetLineColor(1);
	FR_template_barrel->SetLineWidth(2);
	FR_template_barrel->SetMarkerStyle(20);
	FR_template_barrel->SetMarkerColor(1);

	//	FR_xsec_barrel->SetLineColor(2);
	//	FR_xsec_barrel->SetLineWidth(2);
	//	FR_xsec_barrel->SetMarkerStyle(21);
	//	FR_xsec_barrel->SetMarkerColor(2);

	FR_template_endcap->SetLineColor(1);
	FR_template_endcap->SetLineWidth(2);
	FR_template_endcap->SetMarkerStyle(20);
	FR_template_endcap->SetMarkerColor(1);

	//	FR_xsec_endcap->SetLineColor(2);
	//	FR_xsec_endcap->SetLineWidth(2);
	//	FR_xsec_endcap->SetMarkerStyle(21);
	//	FR_xsec_endcap->SetMarkerColor(2);

	canv->cd();
	canv->SetLogx();


	TLegend* legend2 = new TLegend(.60,.70,.90,.89);
	legend2->AddEntry(FR_template_barrel,"Template fitting","lp");
	//	legend2->AddEntry(FR_xsec_barrel,"Ratio method","lp");
	legend2->SetBorderSize(0);

	ptFrame->Draw();
	CMS_lumi(canv,4,11);
	canv->Update();
	canv->RedrawAxis();
	canv->GetFrame()->Draw();
	FR_template_barrel->Draw("");
	//	FR_xsec_barrel->Draw("same");
	legend2->Draw("SAME");
	canv->Print(Form("printEst_%s/FR_Barrel_%s.pdf",indir3.Data(),rmkf.Data()));

	canv->Clear();
	ptFrame->Draw();
	CMS_lumi(canv,4,11);
	canv->Update();
	canv->RedrawAxis();
	canv->GetFrame()->Draw();
	FR_template_endcap->Draw("");
	//	FR_xsec_endcap->Draw("same");
	legend2->Draw("SAME");
	canv->Print(Form("printEst_%s/FR_Endcap_%s.pdf",indir3.Data(),rmkf.Data()));

	//test_barrel_denominator_QCDtemplate->Write();
	//test_barrel_numerator_QCDtemplate->Write();
	g->cd();

	loose_allch_barrel->Write();
	tight_allch_barrel->Write();
	ratio_allch_barrel->Write();

	loose_allch_endcap->Write();
	tight_allch_endcap->Write();
	ratio_allch_endcap->Write();

	//FR_xsec_barrel->Write();
	//FR_xsec_endcap->Write();
	//loose_allch_barrel->Write();
	//tight_allch_barrel->Write();
	//ratio_allch_barrel->Write();
	loose_QCD_barrel->Write();
	tight_QCD_barrel->Write();
	ratio_QCD_barrel->Write();
	loose_Data_barrel->Write();
	tight_Data_barrel->Write();
	ratio_Data_barrel->Write();

	loose_QCD_endcap->Write();
	tight_QCD_endcap->Write();
	ratio_QCD_endcap->Write();
	loose_Data_endcap->Write();
	tight_Data_endcap->Write();
	ratio_Data_endcap->Write();

	loose_QCDori_barrel->Write();
	tight_QCDori_barrel->Write();
	ratio_QCDori_barrel->Write();
	loose_QCDSS_barrel->Write();
	tight_QCDSS_barrel->Write();
	ratio_QCDSS_barrel->Write();
	
	loose_QCDori_endcap->Write();
	tight_QCDori_endcap->Write();
	ratio_QCDori_endcap->Write();
	loose_QCDSS_endcap->Write();
	tight_QCDSS_endcap->Write();
	ratio_QCDSS_endcap->Write();
	



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


