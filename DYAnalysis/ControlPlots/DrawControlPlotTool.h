#include <vector>
#include <iostream>
#include <fstream>

#include <TPad.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TString.h>
#include <THStack.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TF1.h>
#include <TLatex.h>
#include <TPaveText.h>

#include "Include/tdrstyle.C"
#include "Include/CMS_lumi.C"
#include "Include/DYAnalyzer.h"
#include "../BkgEst/interface/defs.h"

using namespace std;
using DYana::binnum;
const int nMassBin = binnum;

class DrawControlPlotTool
{
public:
	Double_t MassBinEdges[nMassBin+1];
	Int_t DrawDataDriven;
	TString NormType;
   TString Rew;
   TString TnpRew;
   TString ZptRew;
   TString MomCor;
   TString HLTname;

   // TeX options
	Bool_t doTex;
   Bool_t doGroupTex;

	TString FileLocation;
	TFile *f_input;
	TFile *f_input_Data;
	TFile *f_input_bkg_emu;
	TFile *f_input_bkg_dijet;
	TFile *f_input_bkg_wjets;
	TFile *f_output;

	vector< TString > HistNames; vector< TString> Variables; vector< TString > XTitles;

	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< Int_t > color; vector< DYana::SampleTag > STags;

	double Nfactor_overall;
	
	DrawControlPlotTool(TString HLTname_arg, Int_t DrawDataDriven_arg, TString NormType_arg, TString MomCor_arg, TString Rew_arg, bool doTnpRew, bool doZptRew);
	virtual void SetupHistogramNames();
	virtual void GenLevelMassSpectrum();
	virtual void LoopForHistograms(Int_t nHist);
   virtual void SetTexOptions(bool groupTex=true);
   virtual void myDivide(TH1D* hnum, TH1D* hden);
   virtual void myMultiply(TH1D* h1, TH1D* h2);

// protected:
	virtual Double_t Entry_Zpeak(TH1D *h);
	virtual Double_t Calc_NormFactor();
	virtual void NormalizationToLumi( vector< TH1D* > h_MC, TString Variable );
	virtual void RebinHistograms( TH1D &h_data, vector< TH1D* > &h_MC, TString Variable );
	virtual void StoreYieldHistogram( TH1D* h_data, vector< TH1D* > h_bkgs, TString Type );
	virtual void DrawMassHistogram_DataDrivenBkg(TString Type, TH1D *h_data, vector< TH1D* > h_MC, const char* variable="mass");
	virtual void SetLegendPosition( TString Variable, Double_t &xlow, Double_t &ylow, Double_t &xhigh, Double_t &yhigh);
	virtual void DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_pred, THStack *hs, TLegend *legend, TString XTitle);
	virtual void PrintTex( TString Variable, TH1D* h_data, TH1D* h_pred, vector< TH1D* > h_MC, TString XTitle);
	virtual void SetAxisRange( TString Variable, TH1D* h_data);
	virtual void SetRatioRangeLabelSize( TString Variable, Double_t &ratio_min, Double_t &ratio_max, Double_t &LabelSize );
	virtual void ChangeHistError_StatOnlyError(TH1D *h_mass, TH1D *h_StatUnc);
	virtual void DrawBkgRatioPlot( TString Type, TH1D* h_data, vector<TH1D*> h_bkgs, vector<TString> Names, TString variable, bool isSS, bool isHichi2=false );
	virtual TH1D* MakeMassHistogram( TString HLTType, TString Type );
};

DrawControlPlotTool::DrawControlPlotTool(TString HLTname_arg, Int_t DrawDataDriven_arg, TString NormType_arg, TString MomCor_arg, TString Rew_arg, bool doTnpRew, bool doZptRew)
{
	if( !(NormType_arg == "Lumi" || NormType_arg == "Zpeak") )
	{
		cout << NormType_arg << " is not allowed type ... Possible NormType = Lumi, Zpeak" << endl;
		return;
	}

	DrawDataDriven = DrawDataDriven_arg;
	NormType = NormType_arg;
	MomCor = MomCor_arg;
   Rew = Rew_arg;
   HLTname = HLTname_arg;
   doTex = false;

	setTDRStyle();
	gROOT->SetStyle( "tdrStyle" );
	// tdrStyle->SetErrorX(0.5);

   // Double_t MassBinEdges_temp[nMassBin+1] =
   // {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
   //                             64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
   //                             110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
   //                             200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
   //                             830, 1000, 1500, 3000};

	for(Int_t i=0; i<nMassBin+1; i++)
		MassBinEdges[i] = DYana::bins[i]; // MassBinEdges_temp[i];

	// -- Get root file containing the histograms -- //
	// FileLocation = "/Users/KyeongPil_Lee/Research/ntupleMaking/13TeV/Results_ROOTFiles/" + HLTname; // -- 74X -- //
   // FileLocation = "/home/kplee/CommonCodes/DrellYanAnalysis/Results_ROOTFiles_76X/" + HLTname; // -- 76X -- //
	FileLocation = "/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/"; 

   // if( HLTname == "None" ) FileLocation = ".";

   TnpRew = "_notnprew";
   if (doTnpRew) TnpRew = "_tnprew";
   ZptRew = "_noZptrew";
   if (doZptRew) ZptRew = "";

	f_input = new TFile(FileLocation + "/ROOTFile_Histogram_InvMass_" + HLTname + "_Powheg_" + MomCor + "_" + Rew + TnpRew + ZptRew + ".root");
	f_input_Data = new TFile(FileLocation + "/ROOTFile_Histogram_InvMass_" + HLTname + "_Data_" + MomCor + "_noHFrew_notnprew.root");
   cout << f_input_Data->GetName() << endl;
   cout << f_input->GetName() << endl;
	
	// -- output file -- //
	f_output = new TFile("ROOTFile_YieldHistogram_" + MomCor + "_" + Rew + TnpRew + ZptRew + ".root", "RECREATE");

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );
   analyzer->SetupMCsamples_v20180814("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

	// -- Set the colors for each sample -- //
	for(Int_t i=0; i<(Int_t)Tag.size(); i++ )
	{
		if( Tag[i] == "ZZ" || Tag[i] == "WZ" || Tag[i] == "WW" )
			color.push_back( kGreen );
		else if( Tag[i].Contains("Wp") || Tag[i].Contains("Wm") )
			color.push_back( kBlue );
		else if( Tag[i].Contains("DYTauTau") )
			color.push_back( kBlue-9 );
		else if( Tag[i].Contains("DY") )
			color.push_back( kOrange );
		else if( Tag[i].Contains("TT") )
			color.push_back( kRed );
      else if( Tag[i].Contains("TW") || Tag[i].Contains("TbarW") ) 
         color.push_back( kRed+1 );
	}

	Nfactor_overall = 1;
	if( NormType == "Zpeak" )
		Nfactor_overall = this->Calc_NormFactor();

	TH1::AddDirectory(0);
}

void DrawControlPlotTool::SetupHistogramNames()
{
   // put the observables first
   HistNames.push_back( "h_mass2" );			Variables.push_back( "MassAnaBins" );				XTitles.push_back( "Invariant Mass(#mu#mu) [GeV]");
   HistNames.push_back( "h_mass3bins" );			Variables.push_back( "Mass3bins" );				XTitles.push_back( "Invariant Mass(#mu#mu) [GeV]");
   HistNames.push_back( "h_mass2" );			Variables.push_back( "MassAnaBins_part1" );				XTitles.push_back( "Invariant Mass(#mu#mu) (PbP) [GeV]");
   HistNames.push_back( "h_mass2" );			Variables.push_back( "MassAnaBins_part2" );				XTitles.push_back( "Invariant Mass(#mu#mu) (pPb) [GeV]");
	HistNames.push_back( "h_diPt2_M60to120" );			Variables.push_back( "diPtM60120" );				XTitles.push_back( "dimuon P_{T} (60<M_{#mu#mu}<120 GeV) [GeV]");
	HistNames.push_back( "h_diPt2_M60to120" );			Variables.push_back( "diPtM60120_part1" );				XTitles.push_back( "dimuon P_{T} (60<M_{#mu#mu}<120 GeV) (PbP) [GeV]");
	HistNames.push_back( "h_diPt2_M60to120" );			Variables.push_back( "diPtM60120_part2" );				XTitles.push_back( "dimuon P_{T} (60<M_{#mu#mu}<120 GeV) (pPb) [GeV]");
   HistNames.push_back( "h_diPt2_M15to60" );			Variables.push_back( "diPtM1560" );				XTitles.push_back( "dimuon P_{T} (15<M_{#mu#mu}<60 GeV) [GeV]");
	HistNames.push_back( "h_Phistar2_M60to120" );			Variables.push_back( "PhistarAnaBins" );		XTitles.push_back( "#phi^{*} (60<M_{#mu#mu}<120 GeV)");
	HistNames.push_back( "h_Phistar2_M60to120" );			Variables.push_back( "PhistarAnaBins_part1" );		XTitles.push_back( "#phi^{*} (60<M_{#mu#mu}<120 GeV) (PbP)");
	HistNames.push_back( "h_Phistar2_M60to120" );			Variables.push_back( "PhistarAnaBins_part2" );		XTitles.push_back( "#phi^{*} (60<M_{#mu#mu}<120 GeV) (pPb)");
   HistNames.push_back( "h_Phistar2_M15to60" );			Variables.push_back( "PhistarAnaBins1560" );		XTitles.push_back( "#phi^{*} (15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_M15to60" );			Variables.push_back( "diRapidityM1560AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_M15to60" );			Variables.push_back( "diRapidityM1560AnaBins_part1" );		XTitles.push_back( "dimuon Rapidity (CM) (15<M_{#mu#mu}<60 GeV) (PbP)");
	HistNames.push_back( "h_diRap2_M15to60" );			Variables.push_back( "diRapidityM1560AnaBins_part2" );		XTitles.push_back( "dimuon Rapidity (CM) (15<M_{#mu#mu}<60 GeV) (pPb)");
	HistNames.push_back( "h_diRap2_M60to120" );			Variables.push_back( "diRapidityM60120AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (60<M_{#mu#mu}<120 GeV)");
	HistNames.push_back( "h_diRap2_M60to120" );			Variables.push_back( "diRapidityM60120AnaBins_part1" );		XTitles.push_back( "dimuon Rapidity (CM) (60<M_{#mu#mu}<120 GeV) (PbP)");
	HistNames.push_back( "h_diRap2_M60to120" );			Variables.push_back( "diRapidityM60120AnaBins_part2" );		XTitles.push_back( "dimuon Rapidity (CM) (60<M_{#mu#mu}<120 GeV) (pPb)");

   // then the rest
   HistNames.push_back( "h_Pt" );				Variables.push_back( "Pt" );				XTitles.push_back( "Muon P_{T} [GeV]");
   HistNames.push_back( "h_eta" );				Variables.push_back( "Eta" );				XTitles.push_back( "Muon #eta_{lab}");
   HistNames.push_back( "h_phi" );				Variables.push_back( "Phi" );				XTitles.push_back( "Muon #phi");
   HistNames.push_back( "h_mass" );			Variables.push_back( "Mass" );				XTitles.push_back( "Invariant Mass(#mu#mu) [GeV]");
   HistNames.push_back( "h_mass_Higgs" );			Variables.push_back( "Mass_Higgs" );				XTitles.push_back( "Invariant Mass(#mu#mu) [GeV]");
   HistNames.push_back( "h_massZ" );			Variables.push_back( "MassZPeak" );				XTitles.push_back( "Invariant Mass(#mu#mu) [GeV]");
   HistNames.push_back( "h_diPt" );			Variables.push_back( "diPt" );				XTitles.push_back( "dimuon P_{T} [GeV]");
   HistNames.push_back( "h_diPt_M15to20" );			Variables.push_back( "diPt_M15to20" );				XTitles.push_back( "dimuon P_{T} [GeV] (15<M_{#mu#mu}<20 GeV)");
   HistNames.push_back( "h_diRap_M15to20" );			Variables.push_back( "diRapidity_M15to20" );				XTitles.push_back( "dimuon Rapidity (lab) (15<M_{#mu#mu}<20 GeV)");
   HistNames.push_back( "h_Phistar_M60to120" );			Variables.push_back( "Phistar" );				XTitles.push_back( "#phi^{*} (60<M_{#mu#mu}<120 GeV)");

	HistNames.push_back( "h_diRap_M15to60" );			Variables.push_back( "diRapidityM1560" );		XTitles.push_back( "dimuon Rapidity (lab) (15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap_M60to120" );			Variables.push_back( "diRapidityM60120" );		XTitles.push_back( "dimuon Rapidity (lab) (60<M_{#mu#mu}<120 GeV)");
	HistNames.push_back( "h_diRapCM_coarse_M60to120" );			Variables.push_back( "diRapidityCMcoarseM60120" );		XTitles.push_back( "dimuon Rapidity (CM) (60<M_{#mu#mu}<120 GeV)");
	HistNames.push_back( "h_diRapCM_coarse_M60to120" );			Variables.push_back( "diRapidityCMcoarseM60120_part1" );		XTitles.push_back( "dimuon Rapidity (CM) (60<M_{#mu#mu}<120 GeV) (PbP)");
	HistNames.push_back( "h_diRapCM_coarse_M60to120" );			Variables.push_back( "diRapidityCMcoarseM60120_part2" );		XTitles.push_back( "dimuon Rapidity (CM) (60<M_{#mu#mu}<120 GeV) (pPb)");
	HistNames.push_back( "h_diRap_M120to600" );			Variables.push_back( "diRapidityM120600" );		XTitles.push_back( "dimuon Rapidity (lab) (120<M_{#mu#mu}<600 GeV)");

	HistNames.push_back( "h_lead_Pt" );			Variables.push_back( "LeadPt" );			XTitles.push_back( "Leading Muon P_{T} [GeV]");
	HistNames.push_back( "h_sub_Pt" );			Variables.push_back( "SubPt" );				XTitles.push_back( "sub-leading Muon P_{T} [GeV]");
	HistNames.push_back( "h_lead_eta" );		Variables.push_back( "LeadEta" );			XTitles.push_back( "Leading Muon #eta_{lab}");
	HistNames.push_back( "h_sub_eta" );			Variables.push_back( "SubEta" );			XTitles.push_back( "sub-leading Muon #eta_{lab}");
	HistNames.push_back( "h_lead_eta_M80to100" );		Variables.push_back( "LeadEta_M80to100" );			XTitles.push_back( "Leading Muon #eta_{lab} (60<M<120 GeV)");
	HistNames.push_back( "h_sub_eta_M80to100" );			Variables.push_back( "SubEta_M80to100" );			XTitles.push_back( "sub-leading Muon #eta_{lab} (60<M<120 GeV)");
	HistNames.push_back( "h_lead_eta_M80to100" );		Variables.push_back( "LeadEta_M80to100_part1" );			XTitles.push_back( "Leading Muon #eta_{lab} (60<M<120 GeV) (PbP)");
	HistNames.push_back( "h_sub_eta_M80to100" );			Variables.push_back( "SubEta_M80to100_part1" );			XTitles.push_back( "sub-leading Muon #eta_{lab} (60<M<120 GeV) (PbP)");
	HistNames.push_back( "h_lead_eta_M80to100" );		Variables.push_back( "LeadEta_M80to100_part2" );			XTitles.push_back( "Leading Muon #eta_{lab} (60<M<120 GeV) (pPb)");
	HistNames.push_back( "h_sub_eta_M80to100" );			Variables.push_back( "SubEta_M80to100_part2" );			XTitles.push_back( "sub-leading Muon #eta_{lab} (60<M<120 GeV) (pPb)");
	HistNames.push_back( "h_lead_phi" );		Variables.push_back( "LeadPhi" );			XTitles.push_back( "Leading Muon #phi");
	HistNames.push_back( "h_sub_phi" );			Variables.push_back( "SubPhi" );			XTitles.push_back( "sub-leading Muon #phi");

	HistNames.push_back( "h_barrel_Pt" );		Variables.push_back( "BarrelPt" );			XTitles.push_back( "Muon(|#eta_{lab}|<0.9) P_{T} [GeV]");
	HistNames.push_back( "h_barrel_eta" );		Variables.push_back( "BarrelEta" );			XTitles.push_back( "Muon(|#eta_{lab}|<0.9) #eta");
	HistNames.push_back( "h_barrel_phi" );		Variables.push_back( "BarrelPhi" );			XTitles.push_back( "Muon(|#eta_{lab}|<0.9) #phi");
	HistNames.push_back( "h_endcap_Pt" );		Variables.push_back( "EndcapPt" );			XTitles.push_back( "Muon(|#eta_{lab}|>0.9) P_{T} [GeV]");
	HistNames.push_back( "h_endcap_eta" );		Variables.push_back( "EndcapEta" );			XTitles.push_back( "Muon(|#eta_{lab}|>0.9) #eta");
	HistNames.push_back( "h_endcap_phi" );		Variables.push_back( "EndcapPhi" );			XTitles.push_back( "Muon(|#eta_{lab}|>0.9) #phi");

	HistNames.push_back( "h_mass_OS" );			Variables.push_back( "OSMass_DYBin" );		XTitles.push_back( "Invariant Mass(Opposite Sign) [GeV]");
	HistNames.push_back( "h_mass_SS" );			Variables.push_back( "SSMass" );			XTitles.push_back( "Invariant Mass(Same Sign) [GeV]");
	HistNames.push_back( "h_mass2_SS" );			Variables.push_back( "SSMass_DYBin" );			XTitles.push_back( "Invariant Mass(Same Sign) [GeV]");
	HistNames.push_back( "h_mass3bins_SS" );			Variables.push_back( "SSMass3bins_DYBin" );			XTitles.push_back( "Invariant Mass(Same Sign) [GeV]");
	HistNames.push_back( "h_diPt2_SS_M60to120" );			Variables.push_back( "SSdiPtM60120" );				XTitles.push_back( "dimuon P_{T} (Same Sign, 60<M_{#mu#mu}<120 GeV) [GeV]");
   HistNames.push_back( "h_diPt2_SS_M15to60" );			Variables.push_back( "SSdiPtM1560" );				XTitles.push_back( "dimuon P_{T} (Same Sign, 15<M_{#mu#mu}<60 GeV) [GeV]");
	HistNames.push_back( "h_Phistar2_SS_M60to120" );			Variables.push_back( "SSPhistarAnaBins" );		XTitles.push_back( "#phi^{*} (Same Sign, 60<M_{#mu#mu}<120 GeV)");
   HistNames.push_back( "h_Phistar2_SS_M15to60" );			Variables.push_back( "SSPhistarAnaBins1560" );		XTitles.push_back( "#phi^{*} (Same Sign, 15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_SS_M15to60" );			Variables.push_back( "SSdiRapidityM1560AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (Same Sign, 15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_SS_M60to120" );			Variables.push_back( "SSdiRapidityM60120AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (Same Sign, 60<M_{#mu#mu}<120 GeV)");

	HistNames.push_back( "h_mass_Hichi2" );			Variables.push_back( "Hichi2Mass" );			XTitles.push_back( "Invariant Mass(Opposite Sign, high dimu #chi^{2}) [GeV]");
	HistNames.push_back( "h_mass2_Hichi2" );			Variables.push_back( "Hichi2Mass_DYBin" );			XTitles.push_back( "Invariant Mass(Opposite Sign, high dimu #chi^{2}) [GeV]");
	HistNames.push_back( "h_mass3bins_Hichi2" );			Variables.push_back( "Hichi2Mass3bins_DYBin" );			XTitles.push_back( "Invariant Mass(Opposite Sign, high dimu #chi^{2}) [GeV]");
	HistNames.push_back( "h_diPt2_Hichi2_M60to120" );			Variables.push_back( "Hichi2diPtM60120" );				XTitles.push_back( "dimuon P_{T} (Opposite Sign, high dimu #chi^{2}, 60<M_{#mu#mu}<120 GeV) [GeV]");
   HistNames.push_back( "h_diPt2_Hichi2_M15to60" );			Variables.push_back( "Hichi2diPtM1560" );				XTitles.push_back( "dimuon P_{T} (Opposite Sign, high dimu #chi^{2}, 15<M_{#mu#mu}<60 GeV) [GeV]");
	HistNames.push_back( "h_Phistar2_Hichi2_M60to120" );			Variables.push_back( "Hichi2PhistarAnaBins" );		XTitles.push_back( "#phi^{*} (Opposite Sign, high dimu #chi^{2}, 60<M_{#mu#mu}<120 GeV)");
   HistNames.push_back( "h_Phistar2_Hichi2_M15to60" );			Variables.push_back( "Hichi2PhistarAnaBins1560" );		XTitles.push_back( "#phi^{*} (Opposite Sign, high dimu #chi^{2}, 15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_Hichi2_M15to60" );			Variables.push_back( "Hichi2diRapidityM1560AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (Opposite Sign, high dimu #chi^{2}, 15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_Hichi2_M60to120" );			Variables.push_back( "Hichi2diRapidityM60120AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (Opposite Sign, high dimu #chi^{2}, 60<M_{#mu#mu}<120 GeV)");

	HistNames.push_back( "h_mass_SS_Hichi2" );			Variables.push_back( "SS_Hichi2Mass" );			XTitles.push_back( "Invariant Mass(Same Sign, high dimu #chi^{2}) [GeV]");
	HistNames.push_back( "h_mass2_SS_Hichi2" );			Variables.push_back( "SS_Hichi2Mass_DYBin" );			XTitles.push_back( "Invariant Mass(Same Sign, high dimu #chi^{2}) [GeV]");
	HistNames.push_back( "h_mass3bins_SS_Hichi2" );			Variables.push_back( "SS_Hichi2Mass3bins_DYBin" );			XTitles.push_back( "Invariant Mass(Same Sign, high dimu #chi^{2}) [GeV]");
	HistNames.push_back( "h_diPt2_SS_Hichi2_M60to120" );			Variables.push_back( "SS_Hichi2diPtM60120" );				XTitles.push_back( "dimuon P_{T} (Same Sign, high dimu #chi^{2}, 60<M_{#mu#mu}<120 GeV) [GeV]");
   HistNames.push_back( "h_diPt2_SS_Hichi2_M15to60" );			Variables.push_back( "SS_Hichi2diPtM1560" );				XTitles.push_back( "dimuon P_{T} (Same Sign, high dimu #chi^{2}, 15<M_{#mu#mu}<60 GeV) [GeV]");
	HistNames.push_back( "h_Phistar2_SS_Hichi2_M60to120" );			Variables.push_back( "SS_Hichi2PhistarAnaBins" );		XTitles.push_back( "#phi^{*} (Same Sign, high dimu #chi^{2}, 60<M_{#mu#mu}<120 GeV)");
   HistNames.push_back( "h_Phistar2_SS_Hichi2_M15to60" );			Variables.push_back( "SS_Hichi2PhistarAnaBins1560" );		XTitles.push_back( "#phi^{*} (Same Sign, high dimu #chi^{2}, 15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_SS_Hichi2_M15to60" );			Variables.push_back( "SS_Hichi2diRapidityM1560AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (Same Sign, high dimu #chi^{2}, 15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_diRap2_SS_Hichi2_M60to120" );			Variables.push_back( "SS_Hichi2diRapidityM60120AnaBins" );		XTitles.push_back( "dimuon Rapidity (CM) (Same Sign, high dimu #chi^{2}, 60<M_{#mu#mu}<120 GeV)");

	HistNames.push_back( "h_Pt_minusCharge" );	Variables.push_back( "MinusChargePt" );		XTitles.push_back( "Muon(mu^{-}) P_{T} [GeV]");
	HistNames.push_back( "h_Pt_plusCharge" );	Variables.push_back( "PlusChargePt" );		XTitles.push_back( "Muon(mu^{+}) P_{T} [GeV]");

	HistNames.push_back( "h_Pt_M15to60" );				Variables.push_back( "Pt1560" );				XTitles.push_back( "Muon P_{T} [GeV] (15<M_{#mu#mu}<60 GeV)");
	HistNames.push_back( "h_Pt_M15to600" );				Variables.push_back( "Pt15600" );				XTitles.push_back( "Muon P_{T} [GeV] (15<M_{#mu#mu}<600 GeV)");
	HistNames.push_back( "h_Pt_M120to600" );				Variables.push_back( "Pt120600" );				XTitles.push_back( "Muon P_{T} [GeV] (120<M_{#mu#mu}<600 GeV)");

	HistNames.push_back( "h_Angle" );			Variables.push_back( "Angle" );				XTitles.push_back( "Angle between two muons");

	HistNames.push_back( "h_Pt_TrigLeg" );		Variables.push_back( "TrigMatchedPt" );		XTitles.push_back( "Muon(Matched with Trigger) P_{T} [GeV]");
	HistNames.push_back( "h_eta_TrigLeg" );		Variables.push_back( "TrigMatchedEta" );	XTitles.push_back( "Muon(Matched with Trigger) #eta_{lab}");
	HistNames.push_back( "h_phi_TrigLeg" );		Variables.push_back( "TrigMatchedPhi" );	XTitles.push_back( "Muon(Matched with Trigger) #phi");

	HistNames.push_back( "h_Pt_OtherLeg" );		Variables.push_back( "TrigNOTMatchedPt" );	XTitles.push_back( "Muon(NOT Matched with Trigger) P_{T} [GeV]");
	HistNames.push_back( "h_eta_OtherLeg" );	Variables.push_back( "TrigNOTMatchedEta" );	XTitles.push_back( "Muon(NOT Matched with Trigger) #eta_{lab}");
	HistNames.push_back( "h_phi_OtherLeg" );	Variables.push_back( "TrigNOTMatchedPhi" );	XTitles.push_back( "Muon(NOT Matched with Trigger) #phi");

	HistNames.push_back( "h_VtxProb" );						Variables.push_back( "VtxProb" );					XTitles.push_back( "Vertex Probability");
	HistNames.push_back( "h_VtxNormChi2" );						Variables.push_back( "VtxNormChi2" );					XTitles.push_back( "Vertex Normalized #chi^{2}");
	HistNames.push_back( "h_VtxNormChi2_fullrange" );						Variables.push_back( "VtxNormChi2_fullrange" );					XTitles.push_back( "Vertex Normalized #chi^{2}");

	HistNames.push_back( "h_mass_OS_BB" );				Variables.push_back( "OSMass_DYBin_BB" );		XTitles.push_back( "Invariant Mass(Opposite Sign, BB) [GeV]");
	HistNames.push_back( "h_mass_OS_BE" );				Variables.push_back( "OSMass_DYBin_BE" );		XTitles.push_back( "Invariant Mass(Opposite Sign, BE) [GeV]");
	HistNames.push_back( "h_mass_OS_EE" );				Variables.push_back( "OSMass_DYBin_EE" );		XTitles.push_back( "Invariant Mass(Opposite Sign, EE) [GeV]");

	HistNames.push_back( "h_muonHits" );			Variables.push_back( "MuonHits" );			XTitles.push_back( "# Muon Hits");
	HistNames.push_back( "h_nMatches" );			Variables.push_back( "nMatches" );			XTitles.push_back( "# Matched Stations");
	HistNames.push_back( "h_RelPtError" );			Variables.push_back( "RelPtError" );		XTitles.push_back( "P_{T} Error / P_{T}");
	HistNames.push_back( "h_dxyVTX" );				Variables.push_back( "dxyVTX" );			XTitles.push_back( "dxy(PV) [cm]");
	HistNames.push_back( "h_dxyVTX_zoom" );				Variables.push_back( "dxyVTX_zoom" );			XTitles.push_back( "dxy(PV) [cm]");
	HistNames.push_back( "h_dzVTX" );				Variables.push_back( "dzVTX" );				XTitles.push_back( "dz(PV) [cm]");
	HistNames.push_back( "h_pixelHits" );			Variables.push_back( "PixelHits" );			XTitles.push_back( "# muon Pixel Hits");
	HistNames.push_back( "h_trackerLayers" );		Variables.push_back( "TrackerLayers" );		XTitles.push_back( "# Tracker Layers");
	HistNames.push_back( "h_RelTrkIso" );			Variables.push_back( "RelTrkIso" );			XTitles.push_back( "TrkIso / P_{T}");
	HistNames.push_back( "h_RelTrkIso_fullrange" );			Variables.push_back( "RelTrkIso_fullrange" );			XTitles.push_back( "TrkIso / P_{T}");
	HistNames.push_back( "h_RelPFIso" );			Variables.push_back( "RelPFIso" );			XTitles.push_back( "PFIso / P_{T}");

	HistNames.push_back( "h_pfMET_pT" );			Variables.push_back( "pfMETpt" );			XTitles.push_back( "PF MET [GeV]");
	HistNames.push_back( "h_pfMET_phi" );			Variables.push_back( "pfMETphi" );			XTitles.push_back( "PF MET #phi");
	HistNames.push_back( "h_pfMET_px" );			Variables.push_back( "pfMETpx" );			XTitles.push_back( "PF METx [GeV]");
	HistNames.push_back( "h_pfMET_py" );			Variables.push_back( "pfMETpy" );			XTitles.push_back( "PF METy [GeV]");
	HistNames.push_back( "h_pfMET_SumEt" );			Variables.push_back( "pfMETsumet" );			XTitles.push_back( "PF MET SumET [GeV]");

   HistNames.push_back( "h_pfMET_Type1_pT" );			Variables.push_back( "pfMETType1pt" );			XTitles.push_back( "PF Type1 MET [GeV]");
   HistNames.push_back( "h_pfMET_Type1_phi" );			Variables.push_back( "pfMETType1phi" );			XTitles.push_back( "PF Type1 MET #phi");
   HistNames.push_back( "h_pfMET_Type1_px" );			Variables.push_back( "pfMETType1px" );			XTitles.push_back( "PF Type1 METx [GeV]");
   HistNames.push_back( "h_pfMET_Type1_py" );			Variables.push_back( "pfMETType1py" );			XTitles.push_back( "PF Type1 METy [GeV]");
   HistNames.push_back( "h_pfMET_Type1_SumEt" );			Variables.push_back( "pfMETType1sumet" );			XTitles.push_back( "PF Type1 MET SumET [GeV]");
	
	HistNames.push_back( "h_nVertices_before" );			Variables.push_back( "nVertices" );			XTitles.push_back( "# vertices");
	HistNames.push_back( "h_hiHF" );			Variables.push_back( "hiHF" );			XTitles.push_back( "HF energy [GeV]");
	HistNames.push_back( "h_hiHFplus" );			Variables.push_back( "hiHFplus" );			XTitles.push_back( "HF (p-going) energy [GeV]");
	HistNames.push_back( "h_hiHFminus" );			Variables.push_back( "hiHFminus" );			XTitles.push_back( "HF (Pb-going) energy [GeV]");
	HistNames.push_back( "h_hiHFplusEta4" );			Variables.push_back( "hiHFplusEta4" );			XTitles.push_back( "HF (p-going, |#eta_{lab}|>4) energy [GeV]");
	HistNames.push_back( "h_hiHFminusEta4" );			Variables.push_back( "hiHFminusEta4" );			XTitles.push_back( "HF (Pb-going, |#eta_{lab}|>4) energy [GeV]");
	HistNames.push_back( "h_hiHFhit" );			Variables.push_back( "hiHFhit" );			XTitles.push_back( "HF hits");
	HistNames.push_back( "h_hiHFhitplus" );			Variables.push_back( "hiHFhitplus" );			XTitles.push_back( "HF (p-going) hits");
	HistNames.push_back( "h_hiHFhitminus" );			Variables.push_back( "hiHFhitminus" );			XTitles.push_back( "HF (Pb-going) hits");
	HistNames.push_back( "h_hiET" );			Variables.push_back( "hiET" );			XTitles.push_back( "E_{T} [GeV]");
	HistNames.push_back( "h_hiEE" );			Variables.push_back( "hiEE" );			XTitles.push_back( "E endcap [GeV]");
	HistNames.push_back( "h_hiEB" );			Variables.push_back( "hiEB" );			XTitles.push_back( "E barrel [GeV]");
	HistNames.push_back( "h_hiEEplus" );			Variables.push_back( "hiEEplus" );			XTitles.push_back( "E endcap (p-going) [GeV]");
	HistNames.push_back( "h_hiEEminus" );			Variables.push_back( "hiEEminus" );			XTitles.push_back( "E endcap (Pb-going) [GeV]");
	HistNames.push_back( "h_hiNpix" );			Variables.push_back( "hiNpix" );			XTitles.push_back( "# pixel hits");
	HistNames.push_back( "h_hiNtracks" );			Variables.push_back( "hiNtracks" );			XTitles.push_back( "# tracks");
	HistNames.push_back( "h_hiNtracks_M1560" );			Variables.push_back( "hiNtracks_M1560" );			XTitles.push_back( "# tracks (15<M<60)");
	HistNames.push_back( "h_hiNtracks_M60120" );			Variables.push_back( "hiNtracks_M60120" );			XTitles.push_back( "# tracks (60<M<120)");
	HistNames.push_back( "h_hiNtracksPtCut" );			Variables.push_back( "hiNtracksPtCut" );			XTitles.push_back( "# tracks with p_{T} cut");

   // HistNames.push_back( "h_maxRelTrkIso_OS" );			Variables.push_back( "maxRelTrkIso_OS" );			XTitles.push_back( "max rel trk iso (OS)");
   // HistNames.push_back( "h_maxRelTrkIso_OS_M1560" );			Variables.push_back( "maxRelTrkIso_OS_M1560" );			XTitles.push_back( "max rel trk iso (OS_M1560)");
   // HistNames.push_back( "h_maxRelTrkIso_OS_M60120" );			Variables.push_back( "maxRelTrkIso_OS_M60120" );			XTitles.push_back( "max rel trk iso (OS_M60120)");
   // HistNames.push_back( "h_maxRelTrkIso_OS_M120600" );			Variables.push_back( "maxRelTrkIso_OS_M120600" );			XTitles.push_back( "max rel trk iso (OS_M120600)");
   // HistNames.push_back( "h_maxRelTrkIso_SS" );			Variables.push_back( "maxRelTrkIso_SS" );			XTitles.push_back( "max rel trk iso (SS)");
   // HistNames.push_back( "h_maxRelTrkIso_SS_M1560" );			Variables.push_back( "maxRelTrkIso_SS_M1560" );			XTitles.push_back( "max rel trk iso (SS_M1560)");
   // HistNames.push_back( "h_maxRelTrkIso_SS_M60120" );			Variables.push_back( "maxRelTrkIso_SS_M60120" );			XTitles.push_back( "max rel trk iso (SS_M60120)");
   // HistNames.push_back( "h_maxRelTrkIso_SS_M120600" );			Variables.push_back( "maxRelTrkIso_SS_M120600" );			XTitles.push_back( "max rel trk iso (SS_M120600)");
   // HistNames.push_back( "h_maxRelPFIso_OS" );			Variables.push_back( "maxRelPFIso_OS" );			XTitles.push_back( "max rel PF iso (OS)");
   // HistNames.push_back( "h_maxRelPFIso_OS_M1560" );			Variables.push_back( "maxRelPFIso_OS_M1560" );			XTitles.push_back( "max rel PF iso (OS_M1560)");
   // HistNames.push_back( "h_maxRelPFIso_OS_M60120" );			Variables.push_back( "maxRelPFIso_OS_M60120" );			XTitles.push_back( "max rel PF iso (OS_M60120)");
   // HistNames.push_back( "h_maxRelPFIso_OS_M120600" );			Variables.push_back( "maxRelPFIso_OS_M120600" );			XTitles.push_back( "max rel PF iso (OS_M120600)");
   // HistNames.push_back( "h_maxRelPFIso_SS" );			Variables.push_back( "maxRelPFIso_SS" );			XTitles.push_back( "max rel PF iso (SS)");
   // HistNames.push_back( "h_maxRelPFIso_SS_M1560" );			Variables.push_back( "maxRelPFIso_SS_M1560" );			XTitles.push_back( "max rel PF iso (SS_M1560)");
   // HistNames.push_back( "h_maxRelPFIso_SS_M60120" );			Variables.push_back( "maxRelPFIso_SS_M60120" );			XTitles.push_back( "max rel PF iso (SS_M60120)");
   // HistNames.push_back( "h_maxRelPFIso_SS_M120600" );			Variables.push_back( "maxRelPFIso_SS_M120600" );			XTitles.push_back( "max rel PF iso (SS_M120600)");
}

Double_t DrawControlPlotTool::Calc_NormFactor()
{
	// -- Get histograms: data -- //
	f_input_Data->cd();
	TH1D *h_data = (TH1D*)f_input_Data->Get( "h_mass_OS_Data1" )->Clone();
   h_data->Add((TH1D*)f_input_Data->Get( "h_mass_OS_Data2" ));

	// -- Get histograms: MC -- //
	vector< TH1D* > h_MC;
	f_input->cd();
	Int_t nTag = Tag.size();
	for(Int_t i_tag=0; i_tag<nTag; i_tag++)
		h_MC.push_back( (TH1D*)f_input->Get( "h_mass_OS_"+Tag[i_tag] )->Clone() );

	NormalizationToLumi( h_MC, "h_mass_OS");

	Double_t EntryZ_data = Entry_Zpeak( h_data );
	Double_t EntryZ_MC = 0;
	Int_t nMC = h_MC.size();
	for(Int_t i_MC=0; i_MC<nMC; i_MC++)
	{
		Double_t EntryZ = Entry_Zpeak( h_MC[i_MC] );
		cout << "[" << Tag[i_MC] << "] " <<"\tEntryZ(overall Nfactor is not applied): " << EntryZ << endl;

		EntryZ_MC += EntryZ;
	}

	Double_t Nfactor = EntryZ_data / EntryZ_MC;

	printf("(EntryZ_MC, EntryZ_Data, Nfactor) = (%12.1lf, %12.1lf, %.6lf)\n", EntryZ_MC, EntryZ_data, Nfactor);

	return Nfactor;
}

void DrawControlPlotTool::NormalizationToLumi( vector< TH1D* > h_MC, TString Variable )
{
	//////////////////////////////
	// -- Set the luminosity -- //
	//////////////////////////////
	Double_t Luminosity = lumi_all;

	if( Variable.Contains("part1") )
		Luminosity = lumi_part1;
	else if( Variable.Contains("part2") )
	{
		Luminosity = lumi_part2;
	}
	cout << "Variable: " << Variable << ", Luminosity: " << Luminosity << endl;

	/////////////////////////
	// -- Normalization -- //
	/////////////////////////
	Int_t nMC = h_MC.size();
	for(Int_t i=0; i<nMC; i++)
	{
		Double_t Norm = (Luminosity * Xsec[i]) / nEvents[i];
      if (IsDYMuMu(STags[i])) {
         // combine pPb and PbP for DYMuMu
         Bool_t doflip = (switcheta(STags[i])<0);
         if (doflip)
            Norm = (Variable.Contains("part2")) ? 0 : ( Xsec[i] * lumi_part1 ) / (Double_t)nEvents[i];
         else 
            Norm = (Variable.Contains("part1")) ? 0 : ( Xsec[i] * lumi_part2 ) / (Double_t)nEvents[i];
      }
      cout << "[Sample: " << Tag[i] << "] Normalization factor to Integrated Luminosity " << Luminosity << "/pb: " <<
         Norm <<  " = (" << Luminosity << " * " << Xsec[i] << ") / " << nEvents[i] <<  endl;
		h_MC[i]->Scale( Norm );
	}
}

void DrawControlPlotTool::LoopForHistograms(Int_t nHist)
{
	Int_t nLoopHist = nHist;
	Int_t nTotalHist = (Int_t)HistNames.size();
	if( nHist == -1 ) nLoopHist = nTotalHist;
	if( nHist > nTotalHist )
	{
		cout << "nHist > nTotalHist! ... " << nHist << " > " << nTotalHist << endl;
      return;
	}

	for(Int_t i_hist=0; i_hist<nLoopHist; i_hist++)
	{
      cout << i_hist << ": " << HistNames[i_hist] << endl;
		// -- Get a histogram: Data -- //
		f_input_Data->cd();
      bool found1, found2;
      found1 = !Variables[i_hist].Contains("part2") && f_input_Data->Get( HistNames[i_hist]+"_Data1" );
      found2 = !Variables[i_hist].Contains("part1") && f_input_Data->Get( HistNames[i_hist]+"_Data2" );
      TH1D *h_data = NULL, *h_data1 = NULL, *h_data2 = NULL;
		if (found1) h_data1 = (TH1D*)f_input_Data->Get( HistNames[i_hist]+"_Data1" )->Clone(); 
		if (found2) h_data2 = (TH1D*)f_input_Data->Get( HistNames[i_hist]+"_Data2" )->Clone(); 

      if (h_data1 && !h_data2) h_data = h_data1;
      else if (h_data2 && !h_data1) h_data = h_data2;
      else if (h_data1 && h_data2) {
         h_data = h_data1;
         h_data->Add(h_data2);
      } else {
         continue;
      }

		// -- Get histograms: MC -- //
		vector< TH1D* > h_MC;
		f_input->cd();
		Int_t nTag = Tag.size();
		for(Int_t i_tag=0; i_tag<nTag; i_tag++) {
         cout << HistNames[i_hist]+"_"+Tag[i_tag]<< endl;
			h_MC.push_back( (TH1D*)f_input->Get( HistNames[i_hist]+"_"+Tag[i_tag] )->Clone() );
      }

		this->NormalizationToLumi( h_MC, Variables[i_hist] );

		this->RebinHistograms( *h_data, h_MC, Variables[i_hist] );

		// -- Check the underflow & overflow -- //
		Double_t UnderFlow = h_data->GetBinContent(0);
		Double_t OverFlow = h_data->GetBinContent( h_data->GetNbinsX() + 1 );
		if( UnderFlow != 0 || OverFlow != 0 )
		{
			cout << endl;
			cout << "\tVariable: " << Variables[i_hist] << endl;
			cout << "\t(UnderFlow, OverFlow) = " << "(" << UnderFlow << ", " << OverFlow << ")" << endl;
			cout << endl;
		}

		/////////////////////////////////////////////////////////////////////////////////////////
		// -- Store yield histogram && Draw mass distribution using data-driven backgrounds -- //
		/////////////////////////////////////////////////////////////////////////////////////////
		if( Variables[i_hist].Contains("MassAnaBins") 
            || Variables[i_hist].Contains("Mass3bins")
            || Variables[i_hist].Contains("SSMass_DYBin")
            || Variables[i_hist].Contains("Hichi2Mass_DYBin")
            || Variables[i_hist].Contains("diPtM60120")
            || Variables[i_hist].Contains("diPtM1560")
            || Variables[i_hist].Contains("diRapidityM1560AnaBins") || Variables[i_hist].Contains("diRapidityM60120AnaBins")
            || (Variables[i_hist].Contains("PhistarAnaBins")))
		{
			vector< TH1D* > h_bkgs;
			for(Int_t i_tag=0; i_tag<nTag; i_tag++)
			{
				if( !Tag[i_tag].Contains("DYMuMu") ) 
					h_bkgs.push_back( h_MC[i_tag] );
			}
			this->StoreYieldHistogram( h_data, h_bkgs, "MCBasedBkg" );

			if( DrawDataDriven >0 )
         {
            TString Type = "All";
            if( Variables[i_hist].Contains("part1") ) Type = "part1";
            else if( Variables[i_hist].Contains("part2") ) Type = "part2";
            TString variable = "mass";
            if (Variables[i_hist].Contains("diPtM60120")) variable = "pt";
            else if (Variables[i_hist].Contains("3bins")) variable = "mass3bins";
            else if (Variables[i_hist].Contains("diPtM1560")) variable = "pt1560";
            else if (Variables[i_hist].Contains("PhistarAnaBins1560")) variable = "phistar1560";
            else if (Variables[i_hist].Contains("PhistarAnaBins")) variable = "phistar";
            else if (Variables[i_hist].Contains("diRapidityM60120AnaBins")) variable = "rap60120";
            else if (Variables[i_hist].Contains("diRapidityM1560AnaBins")) variable = "rap1560";
            this->DrawMassHistogram_DataDrivenBkg(Type, h_data, h_MC, variable);
         }
		}


		//////////////////////////////////////////
		// -- Set attributes: Data Histogram -- //
		//////////////////////////////////////////
		h_data->SetStats(kFALSE);
		h_data->SetLineColor(kBlack);
		h_data->SetMarkerStyle(20);
		h_data->SetMarkerColor(kBlack);
		h_data->SetMarkerSize(0.5);
		h_data->SetFillColorAlpha(kWhite, 0);

		/////////////////////////////////////////////////////////
		// -- Make MC HStack & Set attributes: MC Histogram -- //
		/////////////////////////////////////////////////////////u
		THStack *hs = new THStack("hs_"+Variables[i_hist], "");

		Int_t nMC = h_MC.size();
		for(Int_t i_MC=0; i_MC<nMC; i_MC++)
		{
			if( NormType == "Zpeak" )
				h_MC[i_MC]->Scale( Nfactor_overall );

			h_MC[i_MC]->SetLineColor( color[i_MC] );
			h_MC[i_MC]->SetFillColor( color[i_MC] );
			h_MC[i_MC]->SetMarkerColor( color[i_MC] );

			hs->Add( h_MC[i_MC] );
		}


		//////////////////////////
		// -- Set the legend -- //
		//////////////////////////
		Double_t xlow = 0.75, ylow = 0.70, xhigh = 0.99, yhigh = 0.94;
		this->SetLegendPosition( Variables[i_hist], xlow, ylow, xhigh, yhigh );

		TLegend *legend = new TLegend(xlow, ylow, xhigh, yhigh);
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->AddEntry(h_data, "Data");
		for(Int_t i_MC=nMC-1; i_MC>=0; i_MC--)
		{
         // cout << Tag[i_MC] << endl;
         if( STags[i_MC] == DYana::TW )
            legend->AddEntry(h_MC[i_MC], "tW" );
         else if( STags[i_MC] == DYana::TT )
				legend->AddEntry(h_MC[i_MC], "ttbar" );
			else if( STags[i_MC] == DYana::VVFirst )
				legend->AddEntry(h_MC[i_MC], "Diboson" );
			else if( STags[i_MC] == DYana::WFirst )
				legend->AddEntry(h_MC[i_MC], "WJets" );
			else if( STags[i_MC] == DYana::DYFirst )
				legend->AddEntry(h_MC[i_MC], "DYTauTau" );
			else if( STags[i_MC] == DYana::DYLast) 
				legend->AddEntry(h_MC[i_MC], "DYMuMu" );
		}

		// -- Sum of all prediction: for ratio plot -- //
		TH1D *h_totMC = NULL;
		for(Int_t i_MC=0; i_MC<nMC; i_MC++)
      {
			if( h_totMC == NULL )
				h_totMC = (TH1D*)h_MC[i_MC]->Clone();
			else
				h_totMC->Add( h_MC[i_MC] );
		}

		this->DrawCanvas( Variables[i_hist], h_data, h_totMC, hs, legend, XTitles[i_hist]);
      if (doTex) this->PrintTex( Variables[i_hist], h_data, h_totMC, h_MC, XTitles[i_hist]);
	}
}

void DrawControlPlotTool::RebinHistograms( TH1D& h_data, vector< TH1D* > &h_MC, TString Variable )
{
	Int_t nMC = (Int_t)h_MC.size();
	if( Variable == "OSMass") // -- Mass Bin size: 5GeV -- //
	{
		h_data.Rebin(5);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i]->Rebin(5);
	}
	else if( Variable.Contains("OSMass_M60to120") ) // -- Mass Bin size: 2GeV -- //
	{
		h_data.Rebin(2);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i]->Rebin(2);
	}
	else if( Variable.Contains("MassAnaBins") ) // -- Mass Bin: Follow DY Mass binning @ 13TeV -- //
	{
		h_data = *(TH1D*)h_data.Rebin(nMassBin, h_data.GetName(), MassBinEdges);
		for(Int_t i=0; i<nMC; i++)
			h_MC[i] = (TH1D*)h_MC[i]->Rebin(nMassBin, h_MC[i]->GetName(), MassBinEdges);
	}
   // else if( Variable.Contains("Pt") && !Variable.Contains("M60120") ) // -- Mass Bin: Follow DY Mass binning @ 13TeV -- //
   // {
   //    h_data.Rebin(2);
   //    for(Int_t i=0; i<nMC; i++)
   //       h_MC[i]->Rebin(2);
   // }
   // else if( Variable == "VtxNormChi2" )
   // {
   //    h_data.Rebin(5);
   //    for(Int_t i=0; i<nMC; i++)
   //       h_MC[i]->Rebin(5);
   // }
}

void DrawControlPlotTool::DrawBkgRatioPlot( TString Type, TH1D* h_data, vector<TH1D*> h_bkgs, vector<TString> Names, TString variable, bool isSS, bool isHichi2 )
{
	///////////////////////////////////////////
	// -- Make total background histogram -- //
	///////////////////////////////////////////
	TH1D *h_totBkg = NULL;
	TH1D* h_FR = NULL;
	TH1D* h_ttbarlike = NULL;
	TH1D* h_ZZWZ = NULL;
	TH1D* h_DYTauTau = NULL;

   cout << "Data: " << h_data->Integral() << endl;
	Int_t nBkg = (Int_t)h_bkgs.size();
	for(Int_t i_bkg=0; i_bkg<nBkg; i_bkg++)
   {
      if( Names[i_bkg] == "QCD" || Names[i_bkg].Contains("Wm") || Names[i_bkg].Contains("Wp")  ) continue; // skip those processes here that are replaced with data-driven

		TH1D* h_temp = h_bkgs[i_bkg];
      // cout << Names[i_bkg] << " " << h_temp->Integral() << endl;

		// -- total backgrounds -- //
		if( h_totBkg == NULL )
			h_totBkg = (TH1D*)h_temp->Clone();
		else
			h_totBkg->Add( h_temp );

		// -- fake rate -- //
      // if( Names[i_bkg] == "QCD" || Names[i_bkg].Contains("Wm") || Names[i_bkg].Contains("Wp")  )
		if( Names[i_bkg] == "DiJet" || Names[i_bkg] == "WJets"  )
		{
			if( h_FR == NULL )
				h_FR = (TH1D*)h_temp->Clone();
			else
				h_FR->Add( h_temp );
		}

		// -- ttbar-like bkg -- //
      else if( Names[i_bkg] == "TT" || Names[i_bkg] == "TW" || Names[i_bkg] == "WW" || Names[i_bkg] == "ttbar" )
		{
			if( h_ttbarlike == NULL )
				h_ttbarlike = (TH1D*)h_temp->Clone();
			else
				h_ttbarlike->Add( h_temp );
		}

		// -- WZ and ZZ -- //
		else if( Names[i_bkg] == "WZ" || Names[i_bkg] == "ZZ" )
		{
			if( h_ZZWZ == NULL )
				h_ZZWZ = (TH1D*)h_temp->Clone();
			else
				h_ZZWZ->Add( h_temp );
		}

		// -- DYtautau -- //
		else if( Names[i_bkg].Contains("DYTauTau") )
      {
			if( h_DYTauTau == NULL )
				h_DYTauTau = (TH1D*)h_temp->Clone();
			else
				h_DYTauTau->Add( h_temp );
      }
      else cout << "unknown bkg: " << Names[i_bkg] << endl;


	}
	h_totBkg->Sumw2();
	h_FR->Sumw2();
	h_ttbarlike->Sumw2();
	h_ZZWZ->Sumw2();
	h_DYTauTau->Sumw2();

	////////////////////////////////
	// -- Make the ratio plots -- //
	////////////////////////////////
	TH1D* h_BkgRatio_totBkg = (TH1D*)h_data->Clone(); h_BkgRatio_totBkg->Sumw2();
	h_BkgRatio_totBkg->Divide(h_totBkg, h_data);

	TH1D* h_BkgRatio_FR = (TH1D*)h_data->Clone(); h_BkgRatio_FR->Sumw2();
	h_BkgRatio_FR->Divide(h_FR, h_data);

	TH1D* h_BkgRatio_ttbarlike = (TH1D*)h_data->Clone(); h_BkgRatio_ttbarlike->Sumw2();
	h_BkgRatio_ttbarlike->Divide(h_ttbarlike, h_data);

	TH1D* h_BkgRatio_ZZWZ = (TH1D*)h_data->Clone(); h_BkgRatio_ZZWZ->Sumw2();
	h_BkgRatio_ZZWZ->Divide(h_ZZWZ, h_data);

	TH1D* h_BkgRatio_DYTauTau = (TH1D*)h_data->Clone(); h_BkgRatio_DYTauTau->Sumw2();
	h_BkgRatio_DYTauTau->Divide(h_DYTauTau, h_data);

	///////////////////////
	// -- make canvas -- //
	///////////////////////
	TString CanvasName = "c_BkgRatio_DataDrivenBkg_"+Type+"_"+variable + ((isSS) ? "_SS" : "") + ((isHichi2) ? "_Hichi2" : "");
	TCanvas *c = new TCanvas(CanvasName, "", 800, 600);
	c->cd();
	if (!variable.Contains("rap")) gPad->SetLogx();
	if (!isSS && !isHichi2) gPad->SetLogy();
	gPad->SetGridx(kFALSE);
	gPad->SetGridy(kFALSE);
	gPad->SetLeftMargin(0.11);
	gPad->SetRightMargin(0.02);
	gPad->SetTopMargin(0.07);
	gPad->SetBottomMargin(0.13);

	// -- draw -- //
	h_BkgRatio_totBkg->Draw("EP");
	h_BkgRatio_FR->Draw("EPSAME");
	h_BkgRatio_ttbarlike->Draw("EPSAME");
	h_BkgRatio_ZZWZ->Draw("EPSAME");
	h_BkgRatio_DYTauTau->Draw("EPSAME");

	// -- make legend & add the histograms -- //
	TLegend *legend = new TLegend(0.7,0.15,0.95,0.4);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	legend->AddEntry( h_BkgRatio_totBkg, "Total");
	legend->AddEntry( h_BkgRatio_FR, "QCD + WJets");
	legend->AddEntry( h_BkgRatio_ttbarlike, "t#bar{t} + WW");
	legend->AddEntry( h_BkgRatio_ZZWZ, "ZZ + WZ");
	legend->AddEntry( h_BkgRatio_DYTauTau, "DY#rightarrow#tau#tau");
	legend->Draw();

	// -- axis settings -- //
   if (!isSS && !isHichi2) h_BkgRatio_totBkg->GetYaxis()->SetRangeUser(1e-5,1.);
   else h_BkgRatio_totBkg->GetYaxis()->SetRangeUser(0.,2.);
	h_BkgRatio_totBkg->GetYaxis()->SetLabelSize(0.05);
	h_BkgRatio_totBkg->GetYaxis()->SetTitle("Fraction of backgrounds");
	h_BkgRatio_totBkg->GetYaxis()->SetTitleSize(0.045);
	h_BkgRatio_totBkg->GetYaxis()->SetTitleOffset(1.);
	

	h_BkgRatio_totBkg->GetXaxis()->SetMoreLogLabels();
	h_BkgRatio_totBkg->GetXaxis()->SetNoExponent();
   h_BkgRatio_totBkg->GetXaxis()->SetTitle( DYana::xaxistitle(variable) );
	h_BkgRatio_totBkg->GetXaxis()->SetTitleOffset( 1.2 );
	h_BkgRatio_totBkg->GetXaxis()->SetTitleSize( 0.05 );
	h_BkgRatio_totBkg->GetXaxis()->SetLabelColor(1);
	h_BkgRatio_totBkg->GetXaxis()->SetLabelFont(42);
	h_BkgRatio_totBkg->GetXaxis()->SetLabelOffset(0.007);
	h_BkgRatio_totBkg->GetXaxis()->SetLabelSize(0.05);
   // h_BkgRatio_totBkg->GetXaxis()->SetRangeUser(15, 5000);
	// 



	// -- attributes for each histogram -- //
	h_BkgRatio_totBkg->SetMarkerStyle(20);
	h_BkgRatio_totBkg->SetMarkerSize(1);
	h_BkgRatio_totBkg->SetMarkerColor(kBlack);
	h_BkgRatio_totBkg->SetLineColor(kBlack);
	h_BkgRatio_totBkg->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_FR->SetMarkerStyle(21);
	h_BkgRatio_FR->SetMarkerSize(1);
	h_BkgRatio_FR->SetMarkerColor(kMagenta+2);
	h_BkgRatio_FR->SetLineColor(kMagenta+2);
	h_BkgRatio_FR->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_ttbarlike->SetMarkerStyle(22);
	h_BkgRatio_ttbarlike->SetMarkerSize(1);
	h_BkgRatio_ttbarlike->SetMarkerColor(kRed);
	h_BkgRatio_ttbarlike->SetLineColor(kRed);
	h_BkgRatio_ttbarlike->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_ZZWZ->SetMarkerStyle(23);
	h_BkgRatio_ZZWZ->SetMarkerSize(1);
	h_BkgRatio_ZZWZ->SetMarkerColor(kGreen+2);
	h_BkgRatio_ZZWZ->SetLineColor(kGreen+2);
	h_BkgRatio_ZZWZ->SetFillColorAlpha(kWhite, 0);

	h_BkgRatio_DYTauTau->SetMarkerStyle(29);
	h_BkgRatio_DYTauTau->SetMarkerSize(1);
	h_BkgRatio_DYTauTau->SetMarkerColor(kBlue-9);
	h_BkgRatio_DYTauTau->SetLineColor(kBlue-9);
	h_BkgRatio_DYTauTau->SetFillColorAlpha(kWhite, 0);

   CMS_lumi( c, 111, 0 );

   c->Update();
	c->SaveAs("ControlPlots/pdf/"+CanvasName+".pdf");
   c->SaveAs("ControlPlots/root/"+CanvasName+".root");

   // compute by how much FR would need to be scaled for perfect closure, and save the result
   TH1D *h_nonclosure_scale = (TH1D*) h_data->Clone("h_nonclosure_scale");
   h_nonclosure_scale->Add(h_ttbarlike,-1);
   h_nonclosure_scale->Add(h_ZZWZ,-1);
   h_nonclosure_scale->Add(h_DYTauTau,-1);
   myDivide(h_nonclosure_scale,h_FR);

   // protection: if there is no data, then set the nonclosure ratio to 1 +/- 1
   for (int i=0; i<=h_nonclosure_scale->GetNbinsX()+1; i++) {
      if (h_data->GetBinContent(i)<0.1) {
         h_nonclosure_scale->SetBinContent(i,1);
         h_nonclosure_scale->SetBinError(i,0.5);
      }
   }

   if (isSS && !isHichi2) h_nonclosure_scale->SaveAs("ControlPlots/SSnonclosure/"+variable+".root");
}

void DrawControlPlotTool::StoreYieldHistogram( TH1D* h_data, vector< TH1D* > h_bkgs, TString Type )
{
	TH1D *h_totBkg = NULL;

	Int_t nBkg = (Int_t)h_bkgs.size();
	for(Int_t i_bkg=0; i_bkg<nBkg; i_bkg++)
	{
		if( h_totBkg == NULL )
			h_totBkg = (TH1D*)h_bkgs[i_bkg]->Clone();
		else
			h_totBkg->Add( h_bkgs[i_bkg] );
	}
	h_totBkg->Sumw2();

	TString HistoName = h_data->GetName();
	HistoName.ReplaceAll("mass2", "mass_bkgsub");
	HistoName.ReplaceAll("mass3bins", "mass3bins_bkgsub");
	HistoName.ReplaceAll("diPt2_M60to120", "pt_bkgsub");
	HistoName.ReplaceAll("diPt2_M15to60", "pt1560_bkgsub");
	HistoName.ReplaceAll("Phistar2_M60to120", "phistar_bkgsub");
	HistoName.ReplaceAll("Phistar2_M15to60", "phistar1560_bkgsub");
	HistoName.ReplaceAll("diRap2_M15to60", "rap1560_bkgsub");
	HistoName.ReplaceAll("diRap2_M60to120", "rap60120_bkgsub");
	HistoName.ReplaceAll("_Data", "_"+Type);

	TH1D *h_yield = (TH1D*)h_data->Clone();
	h_yield->SetName( HistoName );
	h_yield->Sumw2();

	// -- Background subtraction -- //
	h_yield->Add( h_totBkg, -1 );

	// -- Remove negative bins -- //
	for(Int_t i=0; i<h_yield->GetNbinsX(); i++)
	{
		Double_t content = h_yield->GetBinContent(i+1);

		if( content < 0 )
			h_yield->SetBinContent(i+1, 0);
	}

	f_output->cd();
	h_yield->Write();
}

void DrawControlPlotTool::DrawMassHistogram_DataDrivenBkg(TString Type, TH1D *h_data, vector< TH1D* > h_MC, const char* variable)
{
	///////////////////////////////////////////////
	// -- Get the histograms obtained from MC -- //
	///////////////////////////////////////////////
	TH1D *h_SignalMC = NULL;
   TH1D *h_ttbar_emu = NULL;
   TH1D *h_DYTauTau_emu = NULL;
   TH1D *h_WW_emu = NULL;
   TH1D *h_WZ_emu = NULL;
   TH1D *h_ZZ_emu = NULL;

	Int_t nMC = (Int_t)h_MC.size();
	for(Int_t i_MC=0; i_MC<nMC; i_MC++)
	{
		if( Tag[i_MC].Contains("DYMuMu") ) // -- Signal MC samples -- //
		{
			if( h_SignalMC == NULL )
				h_SignalMC = (TH1D*)h_MC[i_MC]->Clone(Form("h_%s_DYMC_",variable)+Type);
			else
				h_SignalMC->Add( h_MC[i_MC] );
		}
		if( Tag[i_MC].Contains("DYTauTau") ) // -- TauTau MC samples -- //
		{
			if( h_DYTauTau_emu == NULL )
				h_DYTauTau_emu = (TH1D*)h_MC[i_MC]->Clone(Form("h_%s_DYTauTauMC_",variable)+Type);
			else
				h_DYTauTau_emu->Add( h_MC[i_MC] );
		}
		if( Tag[i_MC] == "TT" || Tag[i_MC] == "TW" || Tag[i_MC] == "TbarW" ) {
			if( h_ttbar_emu == NULL )
				h_ttbar_emu = (TH1D*)h_MC[i_MC]->Clone(Form("h_%s_TTMC_",variable)+Type);
			else
				h_ttbar_emu->Add( h_MC[i_MC] );
      }
		if( Tag[i_MC] == "WW" ) h_WW_emu = (TH1D*)h_MC[i_MC]->Clone(Form("h_%s_WWMC_",variable)+Type);
		if( Tag[i_MC] == "WZ" ) h_WZ_emu = (TH1D*)h_MC[i_MC]->Clone(Form("h_%s_WZMC_",variable)+Type);
		if( Tag[i_MC] == "ZZ" ) h_ZZ_emu = (TH1D*)h_MC[i_MC]->Clone(Form("h_%s_ZZMC_",variable)+Type);
	}

	///////////////////////////////////////////////////////
	// -- Undo the shape change due to HF reweighting -- //
	///////////////////////////////////////////////////////
   TFile *fae = TFile::Open("ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth_noZptrew.root");
   TH1D *h_AccPass = (TH1D*) fae->Get(Form("h_%s_AccPass",variable));
   TH1D *h_EffTotal_HFrew = (TH1D*) fae->Get(Form("h_%s_EffTotal",variable));
   TH1D *hratio = h_AccPass;
   hratio->Divide(h_EffTotal_HFrew);

   h_SignalMC->Multiply(hratio);
   h_DYTauTau_emu->Multiply(hratio);
   h_ttbar_emu->Multiply(hratio);
   h_WW_emu->Multiply(hratio);
   h_WZ_emu->Multiply(hratio);
   h_ZZ_emu->Multiply(hratio);

	////////////////////////////////////////////////////////////////
	// -- Bring the histograms estimated by data-driven method -- //
	////////////////////////////////////////////////////////////////
	f_input_bkg_emu = TFile::Open(FileLocation + Form("/BkgEst/emu/result/emu_%s.root",variable));
   

   // EC
   f_input_bkg_dijet = TFile::Open(Form("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/result/dijet_%s.root",variable));
   f_input_bkg_wjets = TFile::Open(Form("/afs/cern.ch/user/e/echapon/workspace/private/2016_pPb/DY/tree_ana/PADrellYan8TeV/DYAnalysis/BkgEst/fakerate/applyFR/result/wjets_%s.root",variable));
   // file for nonclosure
   TFile *f_nonclosure = TFile::Open(Form("/afs/cern.ch/work/e/echapon/public/DY_pA_2016/SSnonclosure/%s.root",variable));


   // default
   // f_input_bkg_dijet = new TFile(Form("/afs/cern.ch/work/h/hckim/public/ForEmilien_20190506/dijet_%s_opt1050_QCDopt2_reltrkisoR03muptlt10isomax0p2_L3Mu12_beforeCor_FRopt1.root",variable));
   // f_input_bkg_wjets = new TFile(Form("/afs/cern.ch/work/h/hckim/public/ForEmilien_20190506/wjets_%s_opt1050_QCDopt2_reltrkisoR03muptlt10isomax0p2_L3Mu12_beforeCor_FRopt1.root",variable));
   // // Different FR formula(modified):
   // f_input_bkg_dijet = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/dijet_%s_opt1050_QCDopt1_reltrkisoR03muptlt10isomax0p2_L3Mu12_FRopt2.root",variable));
   // f_input_bkg_wjets = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/wjets_%s_opt1050_QCDopt1_reltrkisoR03muptlt10isomax0p2_L3Mu12_FRopt2.root",variable));
   // // QCD MC varied:
   // f_input_bkg_dijet = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/dijet_%s_opt1050_QCDopt1_reltrkisoR03muptlt10isomax0p2_L3Mu12_FRopt2.root",variable));
   // f_input_bkg_wjets = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/wjets_%s_opt1050_QCDopt1_reltrkisoR03muptlt10isomax0p2_L3Mu12_FRopt2.root",variable));
   // // SF uncertainties varied:
   // f_input_bkg_dijet = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/dijet_%s_opt1005_QCDopt2_reltrkisoR03muptlt10isomax0p2_L3Mu12_FRopt2.root",variable));
   // f_input_bkg_wjets = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/wjets_%s_opt1005_QCDopt2_reltrkisoR03muptlt10isomax0p2_L3Mu12_FRopt2.root",variable));
   // // iso max varied:
   // f_input_bkg_dijet = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/dijet_%s_opt1050_QCDopt2_reltrkisoR03muptlt10isomax0p5_L3Mu12_FRopt2.root",variable));
   // f_input_bkg_wjets = new TFile(Form("/afs/cern.ch/work/h/hckim/public/DYFRfiles_w20190430/wjets_%s_opt1050_QCDopt2_reltrkisoR03muptlt10isomax0p5_L3Mu12_FRopt2.root",variable));

   bool isSS = TString(h_data->GetName()).Contains("SS");
   bool isHichi2 = TString(h_data->GetName()).Contains("Hichi2");
   TString tagSS = isSS ? "_SS" : "";
   TString tagHichi2 = isHichi2 ? "_Hichi2" : "";
   TString dijettag;
   TString wjetstag;
   if (isSS) {
      if (isHichi2) {
         dijettag = "dijetSSHichi2_template"; 
         wjetstag = "wjetsSSHichi2_template"; 
         // dijettag = "dijetSS_template";
         // wjetstag = "wjetsSS_template";
      } else {
         dijettag = "dijetSS_template";
         wjetstag = "wjetsSS_template";
      }
   } else {
      if (isHichi2) {
         dijettag = "dijetHichi2_template";
         wjetstag = "wjetsHichi2_template";
      } else {
         dijettag = "dijet_template";
         wjetstag = "wjets_template";
      }
   }
	TH1D *h_diJet_FR = (TH1D*)f_input_bkg_dijet->Get(dijettag)->Clone();
	TH1D *h_WJets_FR = (TH1D*)f_input_bkg_wjets->Get(wjetstag)->Clone();
   TH1D *h_emu_ratio = (TH1D*)f_input_bkg_emu->Get("emu_ratio")->Clone();
   TH1D *h_nonclosure = (TH1D*)f_nonclosure->Get("h_nonclosure_scale");
   
   // apply the emu correction here
   h_DYTauTau_emu->Multiply(h_emu_ratio);
   h_ttbar_emu->Multiply(h_emu_ratio);
   h_WW_emu->Multiply(h_emu_ratio);
   // NB: originally the WZ and ZZ were not emu-corrected, but now they are https://indico.cern.ch/event/967578/note/
   h_WZ_emu->Multiply(h_emu_ratio);
   h_ZZ_emu->Multiply(h_emu_ratio);

	f_input_bkg_dijet->cd();
	TH1D *h_StatUnc_diJet_FR = (TH1D*)f_input_bkg_dijet->Get("dijet_stat")->Clone();
	f_input_bkg_wjets->cd();
	TH1D *h_StatUnc_WJets_FR = (TH1D*)f_input_bkg_wjets->Get("wjets_stat")->Clone();

	ChangeHistError_StatOnlyError(h_diJet_FR, h_StatUnc_diJet_FR);
	ChangeHistError_StatOnlyError(h_WJets_FR, h_StatUnc_WJets_FR);

   // and account for nonclosure
   // EDIT Oct 26, 2020: We do not do this anymore https://indico.cern.ch/event/967578/note/
   // myMultiply(h_diJet_FR,h_nonclosure);
   // myMultiply(h_WJets_FR,h_nonclosure);

	if( Type == "part1" || Type == "part2" )
	{
		Double_t NormFactor = 1;
      if( Type == "part1" )
         NormFactor = lumi_part1 / lumi_all;
      else if( Type == "part2" )
         NormFactor = lumi_part2 / lumi_all;

		h_diJet_FR->Scale( NormFactor );
		h_WJets_FR->Scale( NormFactor );
		h_ttbar_emu->Scale( NormFactor );
		h_DYTauTau_emu->Scale( NormFactor );
		h_WW_emu->Scale( NormFactor );
		h_WZ_emu->Scale( NormFactor );
		h_ZZ_emu->Scale( NormFactor );
	}

	vector< TH1D* > StackHistos; vector< TString > LegendNames; vector< Int_t > colors;
	StackHistos.push_back( h_WW_emu ); LegendNames.push_back( "WW (e#mu)" ); colors.push_back(kGreen);
	StackHistos.push_back( h_WZ_emu ); LegendNames.push_back( "WZ (e#mu)" ); colors.push_back(kGreen+2);
	StackHistos.push_back( h_ZZ_emu ); LegendNames.push_back( "ZZ (e#mu)" ); colors.push_back(kGreen+4);
	StackHistos.push_back( h_DYTauTau_emu ); LegendNames.push_back( "DYTauTau (e#mu)" ); colors.push_back(kBlue-9);
	StackHistos.push_back( h_ttbar_emu ); LegendNames.push_back( "ttbar (e#mu)" ); colors.push_back(kRed);
   if (DrawDataDriven==1) {StackHistos.push_back( h_diJet_FR ); LegendNames.push_back( "QCD (FR)" ); colors.push_back(kMagenta+2);}
	if (DrawDataDriven==1) {StackHistos.push_back( h_WJets_FR ); LegendNames.push_back( "WJets (FR)" ); colors.push_back(kBlue);}
	StackHistos.push_back( h_SignalMC ); LegendNames.push_back( "DYMuMu" ); colors.push_back(kOrange);

	//////////////////////////////////////////
	// -- Set attributes: Data Histogram -- //
	//////////////////////////////////////////
	h_data->SetStats(kFALSE);
	h_data->SetLineColor(kBlack);
	h_data->SetMarkerStyle(20);
	h_data->SetMarkerColor(kBlack);
	h_data->SetMarkerSize(0.5);
	h_data->SetFillColorAlpha(kWhite, 0);

	/////////////////////////////////////////////////////////
	// -- Make MC HStack & Set attributes: MC Histogram -- //
	/////////////////////////////////////////////////////////
	THStack *hs = new THStack(Form("hs_%s_DYBin_DataDrivenBkg_",variable)+tagSS+tagHichi2+Type, "");

	Int_t nStackHists = (Int_t)StackHistos.size();
	for(Int_t iter=0; iter<nStackHists; iter++)
	{
		StackHistos[iter]->SetLineColor( colors[iter] );
		StackHistos[iter]->SetFillColor( colors[iter] );
		StackHistos[iter]->SetMarkerColor( colors[iter] );
		StackHistos[iter]->SetMarkerSize( 0 );

		hs->Add( StackHistos[iter] );
	}

	//////////////////////////
	// -- Set the legend -- //
	//////////////////////////
	Double_t xlow = 0.75, ylow = 0.70, xhigh = 0.99, yhigh = 0.94;
	this->SetLegendPosition( Form("%s_DYBin_DataDrivenBkg",variable)+tagSS+tagHichi2, xlow, ylow, xhigh, yhigh );

	TLegend *legend = new TLegend(xlow, ylow, xhigh, yhigh);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	legend->AddEntry(h_data, "Data");
	for(Int_t iter=nStackHists-1; iter>=0; iter--)
		legend->AddEntry(StackHistos[iter], LegendNames[iter] );

	// -- Sum of all prediction: for ratio plot -- //
	TH1D *h_pred = NULL;
	for(Int_t iter=0; iter<nStackHists; iter++)
	{
		if( h_pred == NULL )
			h_pred = (TH1D*)StackHistos[iter]->Clone();
		else
			h_pred->Add( StackHistos[iter] );
	}

	this->DrawCanvas( Form("%s_DYBin_",variable)+tagSS+tagHichi2+Type+"_DataDrivenBkg", h_data, h_pred, hs, legend, Form("%s (",variable)+Type+")" + tagSS + tagHichi2);

	// -- Store yield histogram -- //
	vector< TH1D* > h_bkgs; vector< TString > Names;

	h_bkgs.push_back( h_WW_emu ); Names.push_back("WW");
	h_bkgs.push_back( h_WZ_emu ); Names.push_back("WZ");
	h_bkgs.push_back( h_ZZ_emu ); Names.push_back("ZZ");
	h_bkgs.push_back( h_DYTauTau_emu ); Names.push_back("DYTauTau");
	h_bkgs.push_back( h_ttbar_emu ); Names.push_back("ttbar");
   if (DrawDataDriven==1) {
      h_bkgs.push_back( h_diJet_FR ); Names.push_back("DiJet");
      h_bkgs.push_back( h_WJets_FR ); Names.push_back("WJets");
   // } else if (DrawDataDriven==2) {
   }
	
	this->StoreYieldHistogram( h_data, h_bkgs, "DataDrivenBkg_"+Type );
	this->DrawBkgRatioPlot( Type, h_data, h_bkgs, Names, variable, isSS, isHichi2 );

	f_output->cd();
	h_SignalMC->Write();

	TFile *f_output2 = TFile::Open(Form("ControlPlots/root/ROOTFile_Histograms_%s",variable) + tagSS + tagHichi2 + "_" + MomCor + "_" + Rew + TnpRew + ZptRew + "_" + Type + ".root", "RECREATE");
	f_output2->cd();

	h_data->SetName("h_data");
	h_data->Write();

	h_SignalMC->SetName("h_SignalMC");
	h_SignalMC->Write();

	h_diJet_FR->SetName("h_diJet_FR");
	h_diJet_FR->Write();

	h_WJets_FR->SetName("h_WJets_FR");
	h_WJets_FR->Write();

	h_WZ_emu->SetName("h_WZ_emu");
	h_WZ_emu->Write();

	h_ZZ_emu->SetName("h_ZZ_emu");
	h_ZZ_emu->Write();

	h_WW_emu->SetName("h_WW_emu");
	h_WW_emu->Write();

	h_DYTauTau_emu->SetName("h_DYTauTau_emu");
	h_DYTauTau_emu->Write();

	h_ttbar_emu->SetName("h_ttbar_emu");
	h_ttbar_emu->Write();

	delete f_output2;
}

TH1D* DrawControlPlotTool::MakeMassHistogram( TString HLTType, TString Type )
{
	TString FileName = FileLocation + "/ROOTFile_Histogram_InvMass_IsoMu20_OR_IsoTkMu20_MCBkg_MomCorr.root";
	TFile *f_MCBkg = TFile::Open( FileName ); f_MCBkg->cd();

	TString HistName = "";
	if( (HLTType == "part1") || (HLTType == "part2") )
		HistName = "h_mass_OS_"+HLTType+"_Norm_"+Type;
	else
		HistName = "h_mass_OS_Norm_"+Type;

	TH1D* h_mass = (TH1D*)f_MCBkg->Get( HistName )->Clone();
	h_mass = (TH1D*)h_mass->Rebin(nMassBin, h_mass->GetName(), MassBinEdges);

	delete f_MCBkg;

	// Int_t nTag = (Int_t)Tag.size();
	// for(Int_t i_tag=0; i_tag<nTag; i_tag++)
	// {
	// 	printf("(Tag, Type) = (%s, %s)\n", Tag[i_tag].Data(), Type.Data() );

	// 	if( Tag[i_tag] == Type )
	// 	{
	// 		Double_t Luminosity = Lumi;
	// 		if( HLTType == "part1" )
	// 			Luminosity = Lumi_part1;
	// 		else if( HLTType == "part2" )
	// 			Luminosity = Lumi - Lumi_part1;

	// 		Double_t Norm = (Lumi * Xsec[i_tag] ) / nEvents[i_tag];
	// 		printf( "[Normalization factor of %s (HLT Type = %s)] = %lf\n", Tag[i_tag].Data(), HLTType.Data(), Norm );
	// 		h_mass->Scale( Norm );
	// 		break;
	// 	}
	// }

	return h_mass;
}

void DrawControlPlotTool::SetLegendPosition( TString Variable, Double_t &xlow, Double_t &ylow, Double_t &xhigh, Double_t &yhigh)
{
	if( Variable == "Pt" )
		ylow = 0.60;

	else if( Variable == "absdiRap_M60to120" || Variable == "OSMass_M60to120" || Variable.Contains("Eta") || Variable.Contains("Phi") || Variable.Contains("Rapidity"))
	{
		xlow = 0.85; ylow = 0.77; 
		xhigh = 0.99; yhigh = 0.94;
	}
	else if( Variable.Contains("OSMass_DYBin") )
	{
		ylow = 0.60;
		xhigh = 0.95;
	}
}

void DrawControlPlotTool::DrawCanvas( TString Variable, TH1D* h_data, TH1D* h_pred, THStack *hs, TLegend *legend, TString XTitle)
{
	TCanvas *c = new TCanvas( "c_"+Variable, "", 800, 600 );

    c->cd(1);
    ///////////////////
    // -- Top Pad -- //
    ///////////////////
    TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
    c1_1->Draw();
    c1_1->cd();
    c1_1->UseCurrentStyle();
    c1_1->SetRightMargin(0.045);
    c1_1->SetBottomMargin(0.3);
    bool setlogy = false;
    if (!(Variable.Contains("Rap") || Variable.Contains("rap") || Variable.Contains("Eta") || Variable.Contains("eta"))) {
       c1_1->SetLogy();
       setlogy = true;
    }

    h_data->GetXaxis()->SetLabelSize(0.000);
    h_data->GetYaxis()->SetTitle("Entry");
    // -- I don't know why TDR style is not applied to Y-axis Title size & Offiset
    h_data->GetYaxis()->SetTitleSize(0.06);
    h_data->GetYaxis()->SetTitleOffset(1.25);
    h_data->GetYaxis()->SetLabelSize(0.05);

    // -- Adjust Y-axis range automatically by calculating maximum value of the histogram -- //
    Double_t MaxValue_Data = h_data->GetBinContent(h_data->GetMaximumBin());
    Double_t MaxValue_totMC = h_pred->GetBinContent(h_pred->GetMaximumBin());
    Double_t MaxValue = 0;
    if( MaxValue_Data > MaxValue_totMC ) MaxValue = MaxValue_Data;
    else MaxValue = MaxValue_totMC;

    Double_t UpperEdge = MaxValue * 4;
    if (!setlogy) UpperEdge = MaxValue * 1.2;
    h_data->GetYaxis()->SetRangeUser(0.05, UpperEdge);

    // -- Set the axis range by hand -- //
    this->SetAxisRange( Variable, h_data );
   	
    h_data->SetTitle("");

    TH1D* h_format = (TH1D*)h_data->Clone();
    h_format->Reset("ICES");
    h_format->Draw("");
    hs->Draw("histsame");
    h_data->Draw("EPsame");
    h_format->Draw("axissame");
    legend->Draw();

    // UF / OF
    TPaveText t1(0.15,0.975,0.3,1,"NDC"); 
    t1.SetFillColor(0); 
    t1.SetBorderSize(0); 
    t1.SetTextSize(0.02);
    t1.SetTextAlign(12);
    t1.AddText(Form("Data: UF=%.0f,OF=%.0f",h_data->GetBinContent(0),h_data->GetBinContent(h_data->GetNbinsX()+1)));
    t1.Draw();
    TPaveText t2(0.15,0.95,0.3,0.975,"NDC"); 
    t2.SetFillColor(0); 
    t2.SetBorderSize(0); 
    t2.SetTextSize(0.02);
    t2.SetTextAlign(12);
    t2.AddText(Form("MC: UF=%.1f,OF=%.1f",h_pred->GetBinContent(0),h_pred->GetBinContent(h_pred->GetNbinsX()+1)));
    t2.Draw();

    // Run some goodness-of-fit tests
    TPaveText t3(0.3,0.95,0.78,1,"NDC"); 
    t3.SetFillColor(0); 
    t3.SetBorderSize(0); 
    t3.SetTextSize(0.05);
    t3.SetTextAlign(32);
    TString opt = "UW";
    t3.AddText(Form("#chi^{2}/ndf=%.2f (%.1f%s)",
             h_data->Chi2Test(h_pred,opt + " CHI2/NDF"),
             100.*h_data->Chi2Test(h_pred,opt),
             "%"
             )); 
    t3.Draw();

    TPaveText t4(0.8,0.95,0.95,1,"NDC"); 
    t4.SetFillColor(0); 
    t4.SetBorderSize(0); 
    t4.SetTextSize(0.05);
    t4.SetTextAlign(32);
    t4.AddText(Form("KS %.1f%s", 100.*h_data->KolmogorovTest(h_pred),"%")); 
    t4.Draw();


    //////////////////////
    // -- Bottom Pad -- //
    //////////////////////
    TPad *c1_2 = new TPad("padc1_2","padc1_2",0.01,0.01,0.99,0.3);
    c1_2->Draw();
    c1_2->cd();

    c1_2->SetBottomMargin(0.4);
    c1_2->SetRightMargin(0.035);
    c1_2->SetLeftMargin(0.15);	

    c1_2->SetFillStyle(0);
    c1_2->SetGrid();

    //Make ratio plot
    TH1D *h_ratio = (TH1D*)h_data->Clone();
    h_data->Sumw2(); //h_pred->Sumw2();
    h_ratio->Divide(h_data, h_pred);
    h_ratio->SetTitle("");
    h_ratio->GetXaxis()->SetMoreLogLabels();
    h_ratio->GetXaxis()->SetNoExponent();
    h_ratio->GetXaxis()->SetTitle( XTitle );
    h_ratio->GetXaxis()->SetTitleOffset( 0.9 );
    h_ratio->GetXaxis()->SetTitleSize( 0.2 );
    h_ratio->GetXaxis()->SetLabelColor(1);
    h_ratio->GetXaxis()->SetLabelFont(42);
    h_ratio->GetXaxis()->SetLabelOffset(0.007);
    h_ratio->GetXaxis()->SetLabelSize(0.15);

    h_ratio->GetYaxis()->SetTitle( "data/Pred." );
    h_ratio->GetYaxis()->SetTitleOffset( 0.4 );
    h_ratio->GetYaxis()->SetTitleSize( 0.17);
    
    
    h_ratio->SetMarkerStyle(20);
    h_ratio->SetMarkerSize(0.7);
    h_ratio->SetMarkerColor(kBlack);

    Double_t ratio_max = 1.5;
    Double_t ratio_min = 0.5;
    Double_t LabelSize = 0.11;
    this->SetRatioRangeLabelSize( Variable, ratio_min, ratio_max, LabelSize );

    h_ratio->SetMaximum( ratio_max );
    h_ratio->SetMinimum( ratio_min );
    h_ratio->GetYaxis()->SetLabelSize( LabelSize );

    h_ratio->Draw("e1p");

    // -- flat line = 1.00 -- //
    TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
    f_line->SetLineColor(kRed);
    f_line->SetLineWidth(1);
    f_line->Draw("SAME");	    

    if( Variable.Contains("diPt")  ) { c1_1->SetLogx(); c1_2->SetLogx(); }
    // if( Variable == "OSMass" || Variable == "OSMass_DYBin" ) { c1_1->SetLogx(); c1_2->SetLogx(); }
    if( Variable.Contains("OSMass_DYBin") ) { c1_1->SetLogx(); c1_2->SetLogx(); }

	// f_output->cd();
	// c->Write();
	c->SaveAs("ControlPlots/pdf/c_"+Variable+".pdf");
	c->SaveAs("ControlPlots/root/c_"+Variable+".root");

}

void DrawControlPlotTool::SetAxisRange( TString Variable, TH1D* h_data)
{
	if( Variable.Contains("OSMass_M60to120") ) { h_data->GetXaxis()->SetRangeUser(60, 120); }
	if( Variable == "Angle" ) { h_data->GetXaxis()->SetRangeUser(0, 4); }
   // if( Variable.Contains("Pt") ) { h_data->GetXaxis()->SetRangeUser(0, 1000); }
	// if( Variable.Contains("VtxProb") ||  Variable.Contains("VtxNormChi2") ) { h_data->GetYaxis()->SetRangeUser(5e-3, 1e7); }
	// if( Variable.Contains("Pt")  ) { h_data->GetXaxis()->SetRangeUser(0, 250); h_data->GetYaxis()->SetRangeUser(0.5, 1000000);}
	// if( Variable.Contains("Eta")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 5000000); }
	// if( Variable.Contains("Phi")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 5000000); }
	// if( Variable.Contains("Mass") ) { h_data->GetXaxis()->SetRangeUser(10, 3000); h_data->GetYaxis()->SetRangeUser(0.005, 10000000);}
	
	// if( Variable.Contains("DYBin") ) { h_data->GetXaxis()->SetRangeUser(15, 3000); h_data->GetYaxis()->SetRangeUser(0.005, 10000000);}
	// if( Variable == "OSMass" ) { h_data->GetXaxis()->SetRangeUser(15, 3000); h_data->GetYaxis()->SetRangeUser(0.005, 10000000);}
	// if( Variable.Contains("Angle")  ) { h_data->GetXaxis()->SetRangeUser(0, 4); h_data->GetYaxis()->SetRangeUser(0.5, 1000000); }
	// if( Variable.Contains("Rapidity")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 300000); }
	// if( Variable.Contains("absdiRap")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 500000); }
	// if( Variable.Contains("absdiRap_M60to120")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 2000000); }
	// if( Variable.Contains("SSMass")  ) { h_data->GetYaxis()->SetRangeUser(0.5, 10); }
}

void DrawControlPlotTool::SetRatioRangeLabelSize( TString Variable, Double_t &ratio_min, Double_t &ratio_max, Double_t &LabelSize )
{
	if( Variable.Contains("OSMass_M60to120") || Variable == "diRapidity" || Variable.Contains("Eta") || Variable.Contains("Phi") || Variable == "diPt" )
	{
		ratio_max = 1.2; ratio_min = 0.8; LabelSize = 0.09;
	}
	else if ( Variable.Contains("Pt") )
	{
		ratio_max = 1.2; ratio_min = 0.8;
	}
	else if( Variable.Contains("OSMass_DYBin") || Variable.Contains("absdiRap") ||  Variable == "OSMass" || Variable.Contains("VtxProb") || Variable.Contains("VtxNormChi2") )
	{
		ratio_max = 1.3; ratio_min = 0.7;
	}
}

Double_t DrawControlPlotTool::Entry_Zpeak(TH1D *h)
{
	Double_t Entry_Z = 0;
	Int_t nBins = h->GetNbinsX();
	for(Int_t i=0; i<nBins; i++)
	{
		Double_t i_bin = i+1;
		Double_t BinCenter = h->GetBinCenter(i_bin);
		if( BinCenter > 60 && BinCenter < 120 )
			Entry_Z += h->GetBinContent(i_bin);
	}

	return Entry_Z;
}

void DrawControlPlotTool::GenLevelMassSpectrum()
{
	f_input->cd();
	// -- Setting colors for each histogram -- //
	vector< Int_t > Colors;
	for(Int_t i=1; i<=9; i++)
		Colors.push_back( i );
	Colors.push_back( kOrange );
	Colors.push_back( kViolet );

	// -- Get Dimuon mass distribution from each sample -- //
	vector< TH1D* > h_MC;
	vector< TString > HistTag;
	Int_t nTag = Tag.size();
	for(Int_t i_tag=0; i_tag<nTag; i_tag++)
	{
		
		if( Tag[i_tag].Contains("DYMuMu") && !Tag[i_tag].Contains("_PbP") ) // -- signal sample, remove PbP -- //
		{
			TH1D *h_temp = NULL;
			h_temp = (TH1D*)f_input->Get( "h_GenMass_"+Tag[i_tag] )->Clone();

			if( h_temp == NULL )
			{
				cout << "No Histograms corresponding to " << Tag[i_tag] << endl;
				return;
			}

			h_temp->Rebin(2);
			Double_t Norm = (lumi_all * Xsec[i_tag]) / nEvents[i_tag];
			h_temp->Scale( Norm );

			h_MC.push_back( h_temp );
			HistTag.push_back( Tag[i_tag] );

			// cout << "[Sample: " << Tag[i] << "] Normalization factor to Integrated Luminosity " << Lumi << "/pb: " << Norm << endl;
			
		}
	}

	Int_t nMC = (Int_t)h_MC.size();
	Int_t nColors = (Int_t)Colors.size();
	if( nMC > nColors )
	{
		printf("[ERROR! ... # histograms > # colors!. (nMC, nColors) = (%d, %d)\n", nMC, nColors);
		return;
	}

	THStack *hs = new THStack("hs", "");
	TLegend *legend = new TLegend(0.55, 0.8, 0.95, 0.95);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);

	for(Int_t i_MC=nMC-1; i_MC>=0; i_MC--)
	{
		h_MC[i_MC]->SetFillColor( Colors[i_MC] );
		h_MC[i_MC]->SetLineColor( Colors[i_MC] );
		h_MC[i_MC]->SetMarkerColor( Colors[i_MC] );
		legend->AddEntry( h_MC[i_MC], HistTag[i_MC] );

		hs->Add( h_MC[i_MC] );
	}

	TString CanvasName = "c_GenMass_SignalMC";
	TCanvas *c_hs = new TCanvas(CanvasName, "", 800, 800);
	c_hs->cd();
	gPad->SetLogy();
	gPad->SetLogx();
	gPad->SetTopMargin(0.03);
	gPad->SetRightMargin(0.05);

	TH1D* h_format = (TH1D*)h_MC[0]->Clone();
	h_format->Reset("ICES");
	h_format->GetXaxis()->SetRangeUser(10, 600);
	h_format->GetYaxis()->SetRangeUser(1e-2, 1e7);
	h_format->GetXaxis()->SetNoExponent();
	h_format->GetXaxis()->SetMoreLogLabels();
	h_format->GetXaxis()->SetTitle("Gen-Level Dimuon Mass (isHardProcess) [GeV]");
	h_format->GetYaxis()->SetTitle("# Events");
	h_format->GetYaxis()->SetTitleOffset(1.4);

	h_format->Draw("");
	hs->Draw("histsame");
	h_format->Draw("axissame");
	legend->Draw();

	c_hs->SaveAs("ControlPlots/pdf/"+CanvasName+".pdf");
	c_hs->SaveAs("ControlPlots/root/"+CanvasName+".root");
}

void DrawControlPlotTool::ChangeHistError_StatOnlyError(TH1D *h_mass, TH1D *h_StatUnc)
{
	for(Int_t i=0; i<nMassBin; i++)
	{
		Int_t i_bin = i+1;
		Double_t StatUnc = h_StatUnc->GetBinContent(i_bin);
		h_mass->SetBinError(i_bin, StatUnc);
	}
}

void DrawControlPlotTool::SetTexOptions(bool groupTex) {
   doTex = true;
   doGroupTex = groupTex;
}

void DrawControlPlotTool::PrintTex( TString Variable, TH1D* h_data, TH1D* h_pred, vector< TH1D* > h_MC, TString XTitle) {
   using namespace DYana;

   ofstream file("ControlPlots/tex/"+Variable+".tex");

   // header
   file << "\\begin{tabular}{|cc|"; 
   int nbkg = doGroupTex ? 5 : h_MC.size();
   for (int i=0; i<nbkg; i++) file << "c";
   file << "|c|c|}" << endl; 

   // column labels
   file << "\\hline" << endl;
   file << "\\multicolumn{2}{c}{" << XTitle << "} & ";
   if (doGroupTex) {
      file << "ttbar & diboson & Wjets & DYtautau & DYmumu & ";
   } else {
      for (int i=0; i<nbkg; i++) file << Name(static_cast<SampleTag>(i)) << " & ";
   }
   file << " Tot. bkg & Data \\\\" << endl;
   file << "\\hline" << endl;

   // print the bin contents
   file.precision(1); file.setf(ios::fixed);
   for (int i=1; i<=h_data->GetNbinsX(); i++) {
      file << "[" << h_data->GetBinLowEdge(i) << ", & " << h_data->GetBinLowEdge(i)+h_data->GetBinWidth(i) << "] & ";

      if (doGroupTex) {
         double val=0, err2=0;
         // TT
         file << h_MC[0]->GetBinContent(i) << "$\\pm$" << h_MC[0]->GetBinError(i) << " & ";
         // Other bkgs
         for (unsigned int j=1; j<h_MC.size(); j++) {
            val += h_MC[j]->GetBinContent(i);
            err2 += pow(h_MC[j]->GetBinError(i),2);
            if (j==VVLast || j==WLast || j==DYLast || j==DYTauTau30 || j==DYLast) {
               file << val << "$\\pm$" << sqrt(err2) << " & ";
               val=0; err2=0;
            }
         }
      } else {
         for (int j=0; j<nbkg; j++) file << h_MC[j]->GetBinContent(i) << "$\\pm$" << h_MC[j]->GetBinError(i) << " & ";
      }

      file << h_pred->GetBinContent(i) << "$\\pm$" << h_pred->GetBinError(i) << " & ";
      file << h_data->GetBinContent(i) << "\\\\" << endl;
   }

   // close file
   file << "\\hline" << endl;
   file << "\\end{tabular}" << endl;
   file.close();
}

void DrawControlPlotTool::myDivide(TH1D* hnum, TH1D* hden) {
   if (hnum->GetNbinsX() != hden->GetNbinsX()) {
      cerr << "ERROR in DrawControlPlotTool::myDivide: numerator and denominator have different numbers of bins" << endl;
      return;
   }

   const unsigned int nbins = hnum->GetNbinsX();
   for (unsigned int i=0; i<=nbins+1; i++) {
      double binc = hden->GetBinContent(i);
      if (binc<1e-1) binc=1; // do not correct if the correction is larger than a factor 10
      hnum->SetBinContent(i,hnum->GetBinContent(i)/binc);
      hnum->SetBinError(i,hnum->GetBinError(i)/binc);
   }
}

void DrawControlPlotTool::myMultiply(TH1D* h1, TH1D* h2) {
   if (h1->GetNbinsX() != h2->GetNbinsX()) {
      cerr << "ERROR in DrawControlPlotTool::myMultiply: histograms have different numbers of bins" << endl;
      return;
   }

   const unsigned int nbins = h1->GetNbinsX();
   for (unsigned int i=0; i<=nbins+1; i++) {
      double binc = h2->GetBinContent(i);
      if (fabs(binc)<1e-1 || fabs(binc)>10) binc=1; // do not correct if the correction is larger than a factor 10
      h1->SetBinContent(i,h1->GetBinContent(i)*binc);
      h1->SetBinError(i,h1->GetBinError(i)*binc);
   }
}
