// -- Class for common functions used in DY differential cross section measurement analysis @ 13 TeV -- //
// -- Author: KyoengPil Lee, 05 Dec. 2015 -- //
// -- 2016.10.24 (v01): add SeparateDYLLSample_LHEInfo & Include Object_v01.h and NtupleHandle_v01.h
// -- 2016.11.04 (v02): include v02 headers, SeparateDYLLSample_LHEInfo -> add DYEE case
#pragma once

#include "Object.h"
#include "NtupleHandle.h"
#include "tnp_weight.h"

#include <TSystem.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "BkgEst/interface/defs.h"

using namespace std;

// #define Lumi 2832.673 // -- Up to Run260627 (Full 2015 Data), MuonPhys_v2 JSON. unit: /pb, Updated at 2016.02.24 -- //
// #define Lumi_part1 865.919 // -- integrated luminosity before Run 257933 -- //
// #define nMassBin 43


TString GetBasePath()
{
	TString BasePath = gSystem->Getenv("KP_ANALYZER_PATH");
	return BasePath + "/";
}

class DYAnalyzer
{
public:

	TString Path_CommonCodes;
	TString HLT;
	Double_t LeadPtCut;
	Double_t SubPtCut;
	Double_t LeadEtaCut;
	Double_t SubEtaCut;
   bool gNoIso;

   Int_t sign; // 1 for pPb, -1 for Pbp

	Double_t PileUpWeight[52];

	Double_t Eff_RecoID_data[5][4];
	Double_t Eff_RecoID_MC[5][4];

	Double_t Eff_Iso_data[5][4];
	Double_t Eff_Iso_MC[5][4];

	Double_t Eff_part1_data[5][4];
	Double_t Eff_part1_MC[5][4];

	Double_t Eff_part2_data[5][4];
	Double_t Eff_part2_MC[5][4];

	// -- Constructor -- //
	DYAnalyzer(TString HLTname);

	// -- Setup accetpance cuts -- //
	void AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut);

	////////////////////////////
	// -- Setup MC samples -- //
	////////////////////////////
	void SetupMCsamples_v20170519( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *Xsec, vector<Double_t> *nEvents );
   void SetupMCsamples_v20170830( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *xsec, vector<Double_t> *nEvents, vector<DYana_v20170830::SampleTag> *STags );
   void SetupMCsamples_v20180111( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *xsec, vector<Double_t> *nEvents, vector<DYana_v20180111::SampleTag> *STags );
   void SetupMCsamples_v20180814( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *xsec, vector<Double_t> *nEvents, vector<DYana_v20180814::SampleTag> *STags );
	Bool_t SeparateDYLLSample_isHardProcess(TString Tag, NtupleHandle *ntuple);
	Bool_t SeparateDYLLSample_LHEInfo(TString Tag, NtupleHandle *ntuple);

	// -- outdated -- //
	Bool_t SeparateDYLLSample(TString Tag, NtupleHandle *ntuple);

	//////////////////////////////////
	// -- Setup pileup weighting -- //
	//////////////////////////////////
	void SetupPileUpReWeighting( Bool_t isMC );
	Double_t PileUpWeightValue(Int_t PileUp_MC);


	/////////////////////////////////////////
	// -- Setup Efficiency scale factor -- //
	/////////////////////////////////////////
	Double_t EfficiencySF_EventWeight(Muon mu1, Muon mu2, int idx1=0, int idx2=0);
	Int_t FindPtBin(Double_t Pt);
	Int_t FindEtaBin(Double_t eta);
	
	////////////////////////////
	// -- Event Selections -- //
	////////////////////////////
   Bool_t EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection, bool noiso=false, double vtxChi2Cut=20, bool dotight=true); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_minusDimuonVtxCut(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Dijet(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_Wjet(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_CheckMoreThanOneDimuonCand(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection, Bool_t& isMoreThanOneCand); // -- output: 2 muons passing event selection conditions -- //
	Bool_t EventSelection_generic(vector< Muon > MuonCollection, NtupleHandle *ntuple, vector< Muon >* SelectedMuonCollection,
         TString trigger, int isotype, double isocut); // -- output: 2 muons passing event selection conditions -- //

	Bool_t isPassAccCondition_Muon(Muon Mu1, Muon Mu2);
	Bool_t isPassAccCondition_GenLepton(GenLepton genlep1, GenLepton genlep2);
	void CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu);
	void CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep);
	void DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2);

	// -- for electron channel - //
	Bool_t EventSelection_Electron(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel_NminusPFIso(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t EventSelection_ElectronChannel(vector< Electron > ElectronCollection, NtupleHandle *ntuple, vector< Electron >* SelectedElectronCollection); // -- output: 2 electrons passing event selection conditions -- //
	Bool_t isPassAccCondition_Electron(Electron Elec1, Electron Elec2);
	Bool_t isPassAccCondition_GenLepton_ECALGAP(GenLepton genlep1, GenLepton genlep2);
	void CompareElectron(Electron *Elec1, Electron *Elec2, Electron *leadElec, Electron *subElec);

	// -- pre-FSR functions -- //
	void PostToPreFSR_byDressedLepton(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	void PostToPreFSR_byDressedLepton_AllPhotons(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection);
	TString DecideFSRType(GenLepton preFSR1, GenLepton preFSR2, GenLepton postFSR1, GenLepton postFSR2);
	Double_t Calc_dR_GenLeptons( GenLepton genlep1, GenLepton genlep2 );
	Double_t Calc_dR_GenLepton_GenOthers( GenLepton genlep1, GenOthers genlep2 );

	// -- miscellaneous -- //
	void GenMatching(TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection);
	void ConvertToTunePInfo( Muon &mu );
	void PrintOutDoubleMuInfo( Muon mu1, Muon mu2 );

	void MakeTChain_fromTextFile(TChain *chain, TString FileName);
};

DYAnalyzer::DYAnalyzer(TString HLTname) : sign(1)
{
	if( HLTname == "None" )
	{
		cout << "===================================================" << endl;
		cout << "[No specific trigger setting ... basic constructor]" << endl;
		cout << "===================================================" << endl;
		
		HLT = "None";
		LeadPtCut = 9999;
		SubPtCut = 9999;
		LeadEtaCut = 9999;
		SubEtaCut = 9999;
	}
	else
	{
      this->AssignAccThreshold(HLTname, &HLT, &LeadPtCut, &SubPtCut, &LeadEtaCut, &SubEtaCut);
      gNoIso = HLTname.Contains("noiso");
		cout << "===========================================================" << endl;
		cout << "Trigger: " << HLT << endl;
		cout << "leading lepton pT Cut: " << LeadPtCut << endl;
		cout << "Sub-leading lepton pT Cut: " << SubPtCut << endl;
		cout << "leading lepton Eta Cut: " << LeadEtaCut << endl;
		cout << "sub-leading lepton Eta Cut: " << SubEtaCut << endl;
      if (gNoIso) cout << "NOT ";
      cout << "using isolation" << endl;
		cout << "===========================================================" << endl;
	}


	this->Path_CommonCodes = GetBasePath() + TString::Format("%s", "Include/");
	printf("[Path_CommonCodes = %s]\n", Path_CommonCodes.Data() );
}

void DYAnalyzer::MakeTChain_fromTextFile( TChain *chain, TString FileName)
{
	ifstream openFile( FileName.Data() );
	cout << "===============================" << endl;
	cout << "Read " << FileName << endl;
	cout << "===============================" << endl;
	if( openFile.is_open() )
	{
		string line;
		while(getline(openFile, line))
		{
			cout << line << endl;
			chain->Add(line.data());
		}
		openFile.close();
	}

	cout << "==================================" << endl;
	cout << "All ROOT Files are put into TChain" << endl;
	cout << "==================================" << endl;
}

void DYAnalyzer::AssignAccThreshold(TString HLTname, TString *HLT, Double_t *LeadPtCut, Double_t *SubPtCut, Double_t *LeadEtaCut, Double_t *SubEtaCut)
{
	if( HLTname.Contains("L3Mu12") )
	{
		*HLT = "HLT_PAL3Mu12_v*"; 
		*LeadPtCut = 15;
		*SubPtCut = 7;//10;//15;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
   else if( HLTname.Contains("L1DoubleMu0") )
	{
		*HLT = "HLT_PAL1DoubleMu0_v*"; 
		*LeadPtCut = 7;
		*SubPtCut = 7;
		*LeadEtaCut = 2.4;
		*SubEtaCut = 2.4;
	}
	else
	{ 
		cout << "Wrong HLT name!: " << HLTname << endl;
		return; 
	}

}

void DYAnalyzer::SetupMCsamples_v20170519( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *xsec, vector<Double_t> *nEvents )
{
   using namespace DYana_v20170519;
   for (int i=0; i<DataFirst; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      ntupleDirectory->push_back(NtupleDir(tag));
      Tag->push_back(Name(tag));
      xsec->push_back(Xsec(tag));
      nEvents->push_back(Nevts(tag));
      if (IsDY(tag)) { // separate mumu and tautau
         Tag->back().ReplaceAll("DY","DYTauTau");
      }
   }
   // and add DYMuMu
   for (int i=DYFirst; i<DataFirst; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      ntupleDirectory->push_back(NtupleDir(tag));
      Tag->push_back(Name(tag));
      xsec->push_back(Xsec(tag));
      nEvents->push_back(Nevts(tag));
      Tag->back().ReplaceAll("DY","DYMuMu");
   }
}

void DYAnalyzer::SetupMCsamples_v20170830( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *xsec, vector<Double_t> *nEvents, vector<DYana_v20170830::SampleTag> *STags )
{
   if (Type=="Pyquen") { // Pyquen
      using namespace DYana_v20170830_Pyquen;
      cout << "Using samples from v20170830 for Type " << Type.Data() << endl;
      for (int i=0; i<DataFirst; i++) {
         SampleTag tag = static_cast<SampleTag>(i);
         // if (!IsDYMuMu(tag)) continue;
         ntupleDirectory->push_back(NtupleDir(tag));
         Tag->push_back(Name(tag));
         xsec->push_back(Xsec(tag));
         nEvents->push_back(Nevts(tag));
         DYana_v20170830::SampleTag tag_Powheg = static_cast<DYana_v20170830::SampleTag>(i);
         STags->push_back(tag_Powheg);
      }
   } else { // Powheg
      using namespace DYana_v20170830;
      cout << "Using samples from v20170830 for Type " << Type.Data() << endl;
      for (int i=0; i<DataFirst; i++) {
         SampleTag tag = static_cast<SampleTag>(i);
         // if (!IsDYMuMu(tag)) continue;
         ntupleDirectory->push_back(NtupleDir(tag));
         Tag->push_back(Name(tag));
         xsec->push_back(Xsec(tag));
         nEvents->push_back(Nevts(tag));
         STags->push_back(tag);
      }
   }
}

void DYAnalyzer::SetupMCsamples_v20180111( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *xsec, vector<Double_t> *nEvents, vector<DYana_v20180111::SampleTag> *STags )
{
   if (Type=="Pyquen") { // Pyquen
      using namespace DYana_v20180111_Pyquen;
      cout << "Using samples from v20180111 for Type " << Type.Data() << endl;
      for (int i=0; i<DataFirst; i++) {
         SampleTag tag = static_cast<SampleTag>(i);
         // if (!IsDYMuMu(tag)) continue;
         ntupleDirectory->push_back(NtupleDir(tag));
         Tag->push_back(Name(tag));
         xsec->push_back(Xsec(tag));
         nEvents->push_back(Nevts(tag));
         DYana_v20180111::SampleTag tag_Powheg = static_cast<DYana_v20180111::SampleTag>(i);
         STags->push_back(tag_Powheg);
      }
   } else if (Type=="CT14") { // CT14, no EPPS16
      using namespace DYana_v20180111_CT14;
      cout << "Using samples from v20180111_CT14 for Type " << Type.Data() << endl;
      for (int i=DYMuMu1030_PbP; i<=DYMuMu30_PbP; i++) {
         SampleTag tag = static_cast<SampleTag>(i);
         // if (!IsDYMuMu(tag)) continue;
         ntupleDirectory->push_back(NtupleDir(tag));
         Tag->push_back(Name(tag));
         xsec->push_back(Xsec(tag));
         nEvents->push_back(Nevts(tag));
         DYana_v20180111::SampleTag tag_Powheg = static_cast<DYana_v20180111::SampleTag>(i);
         STags->push_back(tag_Powheg);
      }
   } else if (Type=="negweights") { // include negative weights
      using namespace DYana_v20180111_negweights;
      cout << "Using samples from v20180111_negweights for Type " << Type.Data() << endl;
      for (int i=DYMuMu1030_PbP; i<=DYMuMu30_PbP; i++) {
         SampleTag tag = static_cast<SampleTag>(i);
         // if (!IsDYMuMu(tag)) continue;
         ntupleDirectory->push_back(NtupleDir(tag));
         Tag->push_back(Name(tag));
         xsec->push_back(Xsec(tag));
         nEvents->push_back(Nevts(tag));
         DYana_v20180111::SampleTag tag_Powheg = static_cast<DYana_v20180111::SampleTag>(i);
         STags->push_back(tag_Powheg);
      }
   } else if (Type=="noEWK") { // no EWK corrections
      using namespace DYana_v20180111_noEWK;
      cout << "Using samples from v20180111_noEWK for Type " << Type.Data() << endl;
      for (int i=DYMuMu1030_PbP; i<=DYMuMu30_PbP; i++) {
         SampleTag tag = static_cast<SampleTag>(i);
         // if (!IsDYMuMu(tag)) continue;
         ntupleDirectory->push_back(NtupleDir(tag));
         Tag->push_back(Name(tag));
         xsec->push_back(Xsec(tag));
         nEvents->push_back(Nevts(tag));
         DYana_v20180111::SampleTag tag_Powheg = static_cast<DYana_v20180111::SampleTag>(i);
         STags->push_back(tag_Powheg);
      }
   } else { // Powheg
      using namespace DYana_v20180111;
      cout << "Using samples from v20180111 for Type " << Type.Data() << endl;
      for (int i=0; i<DataFirst; i++) {
         SampleTag tag = static_cast<SampleTag>(i);
         // if (!IsDYMuMu(tag)) continue;
         ntupleDirectory->push_back(NtupleDir(tag));
         Tag->push_back(Name(tag));
         xsec->push_back(Xsec(tag));
         nEvents->push_back(Nevts(tag));
         STags->push_back(tag);
      }
   }
}

void DYAnalyzer::SetupMCsamples_v20180814( TString Type, vector<TString> *ntupleDirectory, vector<TString> *Tag, vector<Double_t> *xsec, vector<Double_t> *nEvents, vector<DYana_v20180814::SampleTag> *STags )
{
   using namespace DYana_v20180814;
   cout << "Using samples from v20180814 for Type " << Type.Data() << endl;
   for (int i=0; i<DataFirst; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      // if (!IsDYMuMu(tag)) continue;
      ntupleDirectory->push_back(NtupleDir(tag));
      Tag->push_back(Name(tag));
      xsec->push_back(Xsec(tag));
      nEvents->push_back(Nevts(tag));
      STags->push_back(tag);
   }
}

Bool_t DYAnalyzer::SeparateDYLLSample_isHardProcess(TString Tag, NtupleHandle *ntuple)
{
	Bool_t GenFlag = kFALSE;

	// -- Seperate DYMuMu events from DYTauTau  -- //
	if( Tag.Contains("DYMuMu") || Tag.Contains("PIBkg") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
         TLorentzVector v1 = GenLeptonCollection[0].Momentum;
         TLorentzVector v2 = GenLeptonCollection[1].Momentum;
         Double_t reco_M = (v1 + v2).M();
			if( Tag == "DYMuMu1050" ) // -- Select only evetns withtin 10 < M < 50 -- //
			{
				if( reco_M >= 10 && reco_M <= 50 )
					GenFlag = kTRUE;
			} else if ( Tag == "DYMuMu50100" ) // -- Select only evetns withtin 50 < M < 100 -- //
			{
				if( reco_M >= 50 && reco_M <= 100 )
					GenFlag = kTRUE;
			} else if( Tag == "DYMuMu100200" ) // -- Select only evetns withtin 100 < M < 200 -- //
			{
				if( reco_M >= 100 && reco_M <= 200 )
					GenFlag = kTRUE;
			} else if( Tag == "DYMuMu200400" ) // -- Select only evetns withtin 200 < M < 400 -- //
			{
				if( reco_M >= 200 && reco_M <= 400 )
					GenFlag = kTRUE;
			} else if( Tag == "DYMuMu4001000" ) // -- Select only evetns withtin 400 < M < 1000 -- //
			{
				if( reco_M >= 400 && reco_M <= 1000 )
					GenFlag = kTRUE;
			} else if ( Tag.Contains("DYMuMu1030") ) // -- Select only evetns withtin 10 < M < 30 -- //
			{
				if( reco_M >= 10 && reco_M <= 30 )
					GenFlag = kTRUE;
			} else if ( Tag.Contains("DYMuMu30") ) // -- Select only evetns withtin M > 30 -- //
			{
				if( reco_M >= 30 )
					GenFlag = kTRUE;
			} else GenFlag = kTRUE;
		}
	}
	else if( Tag.Contains("DYEE") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isElectron() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 electrons from hard-process -- //
		{
			if( Tag == "DYEE_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = kTRUE;
			}
			if( Tag.Contains("M50to100") ) // -- Select only evetns withtin 50 < M < 100 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 100 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	// -- Separate DYTauTau events from MuMu events -- //
	else if( Tag.Contains("DYTauTau") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( abs(genlep.ID) == 15 && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 taus from hard-process -- //
		{
         TLorentzVector v1 = GenLeptonCollection[0].Momentum;
         TLorentzVector v2 = GenLeptonCollection[1].Momentum;
         Double_t reco_M = (v1 + v2).M();
			if ( Tag.Contains("DYTauTau1030") ) // -- Select only evetns withtin 10 < M < 30 -- //
         {
            if( reco_M >= 10 && reco_M <= 30 )
               GenFlag = kTRUE;
         } else if ( Tag.Contains("DYTauTau30") ) // -- Select only evetns withtin M > 30 -- //
         {
            if( reco_M >= 30 )
               GenFlag = kTRUE;
         } else GenFlag = kTRUE;
		}
	}
	// -- Madgraph sample -- //
	else if( Tag.Contains("Madgraph") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isHardProcess )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
			if( Tag == "Madgraph_M50to150" ) // -- Select only evetns withtin 50 < M < 150 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 150 )
					GenFlag = kTRUE;
			}
			else if( Tag == "Madgraph_M10to50" )
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M > 10 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	// -- other cases(e.g. ttbar, WJets, Diboson...): pass
	else
		GenFlag = kTRUE; 

	return GenFlag;
}

Bool_t DYAnalyzer::SeparateDYLLSample_LHEInfo(TString Tag, NtupleHandle *ntuple)
{
	Bool_t GenFlag = kFALSE;

	// -- Seperate DYMuMu events -- //
	if( Tag.Contains("DYMuMu") || Tag.Contains("DYEE") )
	{
		Double_t LeptonID = 0;
		if( Tag.Contains("DYMuMu") ) LeptonID = 13;
		else if( Tag.Contains("DYEE") ) LeptonID = 11;
		
		vector<LHELepton> LHELeptonCollection;
		Int_t nLHELepton = ntuple->nLHEParticle;
		for(Int_t i_lhe=0; i_lhe<nLHELepton; i_lhe++)
		{
			LHELepton lhelep;
			lhelep.FillFromNtuple(ntuple, i_lhe);
			if( fabs(lhelep.ID) == LeptonID )
				LHELeptonCollection.push_back( lhelep );

			if( LHELeptonCollection.size() == 2 ) // -- Select the events containing 2 LHE muons -- //
			{
				if( Tag.Contains("M50to100") ) // -- Select only evetns withtin 50 < M < 100 -- //
				{
					TLorentzVector v1 = LHELeptonCollection[0].Momentum;
					TLorentzVector v2 = LHELeptonCollection[1].Momentum;
					Double_t M = (v1 + v2).M();
					if( M < 100 )
						GenFlag = kTRUE;
				}
				else
					GenFlag = kTRUE;
			}
		}
	}
	else
		GenFlag = kTRUE;
	
	return GenFlag;
}

Bool_t DYAnalyzer::SeparateDYLLSample(TString Tag, NtupleHandle *ntuple)
{
	Bool_t GenFlag = kFALSE;

	// -- Seperate DYMuMu events from DYTauTau  -- //
	if( Tag.Contains("DYMuMu") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 muons from hard-process -- //
		{
			if( Tag == "DYMuMu1050" ) // -- Select only evetns withtin 10 < M < 50 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M >= 10 && reco_M <= 50 )
					GenFlag = kTRUE;
			} else if ( Tag == "DYMuMu50100" ) // -- Select only evetns withtin 50 < M < 100 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M >= 50 && reco_M <= 100 )
					GenFlag = kTRUE;
			} else if( Tag == "DYMuMu100200" ) // -- Select only evetns withtin 100 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M >= 100 && reco_M <= 200 )
					GenFlag = kTRUE;
			} else if( Tag == "DYMuMu200400" ) // -- Select only evetns withtin 200 < M < 400 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M >= 200 && reco_M <= 400 )
					GenFlag = kTRUE;
			} else if( Tag == "DYMuMu4001000" ) // -- Select only evetns withtin 400 < M < 1000 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M >= 400 && reco_M <= 1000 )
					GenFlag = kTRUE;
			} else GenFlag = kTRUE;
		}
	}
	else if( Tag.Contains("DYEE") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isElectron() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 electrons from hard-process -- //
		{
			if( Tag == "DYEE_M50to200" ) // -- Select only evetns withtin 50 < M < 200 -- //
			{
				TLorentzVector v1 = GenLeptonCollection[0].Momentum;
				TLorentzVector v2 = GenLeptonCollection[1].Momentum;
				Double_t reco_M = (v1 + v2).M();
				if( reco_M < 200 )
					GenFlag = kTRUE;
			}
			else
				GenFlag = kTRUE;
		}
	}
	// -- Separate DYTauTau events from MuMu events -- //
	else if( Tag.Contains("DYTauTau") )
	{
		vector<GenLepton> GenLeptonCollection;
		Int_t NGenLeptons = ntuple->gnpair;
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( abs(genlep.ID) == 15 && genlep.fromHardProcessDecayed )
				GenLeptonCollection.push_back( genlep );
		}

		if( GenLeptonCollection.size() == 2 ) // -- Select the events containing 2 taus from hard-process -- //
		{
			GenFlag = kTRUE;
		}
	}
	// -- other cases(e.g. ttbar, WJets, Diboson...): pass
	else
		GenFlag = kTRUE; 

	return GenFlag;
}

void DYAnalyzer::SetupPileUpReWeighting( Bool_t isMC )
{
	if( isMC == kFALSE ) // -- for data -- //
	{
		for(Int_t i=0; i<50; i++)
			PileUpWeight[i] = 1;

		return;
	}
	
   // // -- Only for the MC -- //
   // TString FileLocation = Path_CommonCodes + "ROOTFile_PUReWeight_76X_v20160404_71mb.root";
   // TFile *f = new TFile(FileLocation);
   // f->cd();
   // TH1D *h_weight = (TH1D*)f->Get("h_PUReWeights");
   // if( h_weight == NULL )
   // {
   //    cout << "ERROR! ... No Weight histogram!"<< endl;
   //    return;
   // }

   // NO PU weight available yet (FIXME)
	for(Int_t i=0; i<50; i++)
	{
      // Int_t i_bin = i+1;
		PileUpWeight[i] = 1.; // h_weight->GetBinContent(i_bin);
	}
}

Double_t DYAnalyzer::PileUpWeightValue(Int_t PileUp_MC)
{
	if( PileUp_MC < 0 || PileUp_MC > 49 )
	{
		cout << "[PileUp_MC = " << PileUp_MC << "]: NO CORRESPONDING PU Weight! ... it returns 0" << endl;
		return 0;
	}
	return PileUpWeight[PileUp_MC];
}


Double_t DYAnalyzer::EfficiencySF_EventWeight(Muon mu1, Muon mu2, int idx1, int idx2)
{
	Double_t weight = 1;
   // FIXME implement the tnp weight factors

	return weight;
}


Int_t DYAnalyzer::FindPtBin(Double_t Pt)
{
	const Int_t nPtBins = 4;
	Double_t PtBinEdges[nPtBins+1] = {10, 22, 40, 70, 250};

	Int_t ptbin = 9999;

	// -- if Pt is larger than the largest Pt bin edge, SF is same with the value for the last bin -- // 
	if( Pt > PtBinEdges[nPtBins] )
		ptbin = nPtBins-1;
	else
	{
		for(Int_t i=0; i<nPtBins; i++)
		{
			if( Pt > PtBinEdges[i] && Pt < PtBinEdges[i+1] )
			{
				ptbin = i;
				break;
			}
		}
	}

	return ptbin;
}

Int_t DYAnalyzer::FindEtaBin(Double_t eta)
{
	const Int_t nEtaBins = 5;
	Double_t EtaBinEdges[nEtaBins+1] = {-2.4, -1.2, -0.3, 0.3, 1.2, 2.4};

	Int_t etabin = 9999;

	for(Int_t i=0; i<nEtaBins; i++)
	{
		if( eta > EtaBinEdges[i] && eta < EtaBinEdges[i+1] )
		{
			etabin = i;
			break;
		}
	}

	return etabin;
}

Bool_t DYAnalyzer::EventSelection(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection, bool noiso, double vtxChi2Cut, bool dotight) // -- output: 2 muons passing event selection conditions -- //
{
   using namespace DYana;

	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( (dotight && MuonCollection[j].isTightMuon()) 
             || (!dotight && MuonCollection[j].isLoose) ) {
          bool passOK=false;
          // for noiso or L3Mu12: no additional cut
          if (noiso || gNoIso) 
             passOK=true;
          // for L1DoubleMu0: need also tk iso, dxy and dz cuts
          if (!noiso && // HLT.Contains("L1DoubleMu0") && 
                   // (MuonCollection[j].trkiso*MuonCollection[j].Pt < 2.5 && fabs(MuonCollection[j].dzVTX)<0.1 && fabs(MuonCollection[j].dxyVTX)<0.01))
                   (MuonCollection[j].trkiso < 0.2))
             passOK = true;

          if (passOK)
             QMuonCollection.push_back( MuonCollection[j] );
       }
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	// -- It has to be above 15GeV -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, HLT) && mu.Pt > LeadPtCut )
		{
			isExistHLTMatchedMuon = kTRUE;
			break;
		}
	}

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons >= 2) // if there are more than 2 OK muons, just look at the first 2
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			Bool_t isOS = kFALSE;
			if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
         if( reco_M > bins[0] && reco_M < bins[binnum] && isPassAcc == kTRUE && VtxNormChi2 < vtxChi2Cut && Angle < TMath::Pi() - 0.005 )
         // if( reco_M > 30 && reco_M < 600 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
      else if( nQMuons > 2 )
      {
         Double_t VtxProb_BestPair = -1;
         Double_t VtxNormChi2_BestPair = 999;
         Muon mu1_BestPair;
         Muon mu2_BestPair;

         for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
         {
            Muon Mu = QMuonCollection[i_mu];

            // -- at least 1 muon should be matched with HLT objects in best pair -- //
            if( Mu.isTrigMatched(ntuple, HLT) && Mu.Pt > LeadPtCut )
            {
               // -- Mu in this loop: QMuon Matched with HLT object -- //

               // -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
               for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
               {
                  Muon Mu_jth = QMuonCollection[j_mu];

                  if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
                  {
                     // -- Check that this pair is within acceptance -- //
                     Bool_t isPassAcc = kFALSE;
                     isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

                     if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
                     {
                        Double_t VtxProb_temp = -999;
                        Double_t VtxNormChi2_temp = 2.*vtxChi2Cut;
                        DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

                        // -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
                        if( VtxNormChi2_temp < VtxNormChi2_BestPair )
                        {
                           VtxNormChi2_BestPair = VtxNormChi2_temp;
                           mu1_BestPair = Mu;
                           mu2_BestPair = Mu_jth;
                        }
                     }
                  }
               } // -- end of the loop for j_mu (finding for second muon)
            }
         } // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

         if( VtxNormChi2_BestPair < 2.*vtxChi2Cut ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
         {
            TLorentzVector reco_v1 = mu1_BestPair.Momentum;
            TLorentzVector reco_v2 = mu2_BestPair.Momentum;
            Double_t reco_M = (reco_v1 + reco_v2).M();

            // -- 3D open angle is calculated using inner track information -- //
            // -- 3D open angle -- //
            Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

            Bool_t isOS = kFALSE;
            if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

            if( reco_M > bins[0] && reco_M < bins[binnum] && VtxNormChi2_BestPair < vtxChi2Cut && Angle < TMath::Pi() - 0.005 )
               // if( reco_M > 30 && reco_M < 600 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
            {
               isPassEventSelection = kTRUE;
               SelectedMuonCollection->push_back( mu1_BestPair );
               SelectedMuonCollection->push_back( mu2_BestPair );
            }
         }

      } // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}

Bool_t DYAnalyzer::EventSelection_minusDimuonVtxCut(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
   using namespace DYana;

	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isTightMuon() ) {
          // for L1DoubleMu0 / low mass: use PF iso
          if (HLT.Contains("L1DoubleMu0") && MuonCollection[j].relPFiso < 0.3)
	        QMuonCollection.push_back( MuonCollection[j] );
          // for L1DoubleMu0 / low mass: use trkiso
          if (HLT.Contains("L3Mu12") && MuonCollection[j].trkiso < 0.3)
	        QMuonCollection.push_back( MuonCollection[j] );
       }
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, "HLT_PAL3Mu12_v*") )
		{
			isExistHLTMatchedMuon = kTRUE;
			break;
		}
	}

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons == 2)
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			// if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && Angle < TMath::Pi() - 0.005 )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
		else if( nQMuons > 2 )
		{
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
			{
				Muon Mu = QMuonCollection[i_mu];

				// -- at least 1 muon should be matched with HLT objects in best pair -- //
				if( Mu.isTrigMatched(ntuple, "HLT_PAL3Mu12_v*") )
				{
					// -- Mu in this loop: QMuon Matched with HLT object -- //

					// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
					for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
					{
						Muon Mu_jth = QMuonCollection[j_mu];

						if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
						{
							// -- Check that this pair is within acceptance -- //
							Bool_t isPassAcc = kFALSE;
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

							if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
							{
								Double_t VtxProb_temp = -999;
								Double_t VtxNormChi2_temp = 999;
								DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

								// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
								if( VtxNormChi2_temp < VtxNormChi2_BestPair )
								{
									VtxNormChi2_BestPair = VtxNormChi2_temp;
									mu1_BestPair = Mu;
									mu2_BestPair = Mu_jth;
								}
							}
						}
					} // -- end of the loop for j_mu (finding for second muon)
				}
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				// if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 )
				if( reco_M > bins[0] && Angle < TMath::Pi() - 0.005 )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}

// generic event selection function for any trigger and iso cut
Bool_t DYAnalyzer::EventSelection_generic(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection,  // -- output: 2 muons passing event selection conditions -- //
                  TString trigger, int isotype, double isocut )
{
   using namespace DYana;

	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
	    if( MuonCollection[j].isTightMuon()) {
          Muon mu = MuonCollection[j];
          // now check iso
          double isoval=999;
          if (isotype==0)
             isoval = mu.relPFiso;
          else if (isotype==1)
             isoval = (mu.PfChargedHadronIsoR03+mu.PfNeutralHadronIsoR03)/mu.Pt;
          else if (isotype==2)
             isoval = (mu.PfChargedHadronIsoR03)/mu.Pt;
          else if (isotype==3)
             isoval = (mu.PfChargedHadronIsoR04+mu.PfNeutralHadronIsoR04+mu.PfGammaIsoR04)/mu.Pt;
          else if (isotype==4)
             isoval = (mu.PfChargedHadronIsoR04+mu.PfNeutralHadronIsoR04)/mu.Pt;
          else if (isotype==5)
             isoval = (mu.PfChargedHadronIsoR04)/mu.Pt;
          else if (isotype==6)
             isoval = mu.trkiso;
          else if (isotype==7)
             isoval = mu.PfChargedHadronIsoR03+mu.PfNeutralHadronIsoR03+mu.PfGammaIsoR03;
          else if (isotype==8)
             isoval = (mu.PfChargedHadronIsoR03+mu.PfNeutralHadronIsoR03);
          else if (isotype==9)
             isoval = (mu.PfChargedHadronIsoR03);
          else if (isotype==10)
             isoval = (mu.PfChargedHadronIsoR04+mu.PfNeutralHadronIsoR04+mu.PfGammaIsoR04);
          else if (isotype==11)
             isoval = (mu.PfChargedHadronIsoR04+mu.PfNeutralHadronIsoR04);
          else if (isotype==12)
             isoval = (mu.PfChargedHadronIsoR04);
          else if (isotype==13)
             isoval = mu.trkiso*mu.Pt;

             if (isoval<isocut) {
	        QMuonCollection.push_back( MuonCollection[j] );
             }
       }
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	// -- It has to be above 15GeV -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	Int_t nHLTMatchedMuon = 0;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, trigger) && (trigger.Contains("DoubleMu") || mu.Pt > LeadPtCut) )
		{
			nHLTMatchedMuon++;
		}
	}
   isExistHLTMatchedMuon = ((!trigger.Contains("DoubleMu") && nHLTMatchedMuon>=1) || nHLTMatchedMuon>=2);

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons == 2)
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			Bool_t isOS = kFALSE;
			if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
         if( reco_M > bins[0] && reco_M < bins[binnum] && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 )
         // if( reco_M > 30 && reco_M < 600 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
      else if( nQMuons > 2 )
      {
         Double_t VtxProb_BestPair = -1;
         Double_t VtxNormChi2_BestPair = 999;
         Muon mu1_BestPair;
         Muon mu2_BestPair;

         for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
         {
            Muon Mu = QMuonCollection[i_mu];

            // -- at least 1 muon should be matched with HLT objects in best pair -- //
            if( Mu.isTrigMatched(ntuple, trigger) && Mu.Pt > LeadPtCut )
            {
               // -- Mu in this loop: QMuon Matched with HLT object -- //

               // -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
               for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
               {
                  Muon Mu_jth = QMuonCollection[j_mu];

                  if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
                  {
                     // -- Check that this pair is within acceptance -- //
                     Bool_t isPassAcc = kFALSE;
                     isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

                     if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
                     {
                        Double_t VtxProb_temp = -999;
                        Double_t VtxNormChi2_temp = 999;
                        DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

                        // -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
                        if( VtxNormChi2_temp < VtxNormChi2_BestPair )
                        {
                           VtxNormChi2_BestPair = VtxNormChi2_temp;
                           mu1_BestPair = Mu;
                           mu2_BestPair = Mu_jth;
                        }
                     }
                  }
               } // -- end of the loop for j_mu (finding for second muon)
            }
         } // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

         if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
         {
            TLorentzVector reco_v1 = mu1_BestPair.Momentum;
            TLorentzVector reco_v2 = mu2_BestPair.Momentum;
            Double_t reco_M = (reco_v1 + reco_v2).M();

            // -- 3D open angle is calculated using inner track information -- //
            // -- 3D open angle -- //
            Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

            Bool_t isOS = kFALSE;
            if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

            if( reco_M > bins[0] && reco_M < bins[binnum] && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 )
            // if( reco_M > 30 && reco_M < 600 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
            {
               isPassEventSelection = kTRUE;
               SelectedMuonCollection->push_back( mu1_BestPair );
               SelectedMuonCollection->push_back( mu2_BestPair );
            }
         }

      } // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}

Bool_t DYAnalyzer::isPassAccCondition_Muon(Muon Mu1, Muon Mu2)
{
   using namespace DYana;
	Bool_t isPassAcc = kFALSE;
	Muon leadMu, subMu;
   double diRap = sign*(Mu1.Momentum+Mu2.Momentum).Rapidity()-rapshift;
   double diMass = (Mu1.Momentum+Mu2.Momentum).M();
	CompareMuon(&Mu1, &Mu2, &leadMu, &subMu);
	if( leadMu.Pt > LeadPtCut && fabs(leadMu.eta) < LeadEtaCut && 
		subMu.Pt  > SubPtCut  && fabs(subMu.eta)  < SubEtaCut &&
      diRap>rapbin_60120[0] && diRap<rapbin_60120[rapbinnum_60120] &&
      diMass>bins[0] && diMass<bins[binnum] )
		isPassAcc = kTRUE;

	return isPassAcc;
}

Bool_t DYAnalyzer::isPassAccCondition_GenLepton(GenLepton genlep1, GenLepton genlep2)
{
   using namespace DYana;

	Bool_t isPassAcc = kFALSE;

	GenLepton leadGenLep, subGenLep;
   double diRap = sign*(genlep1.Momentum+genlep2.Momentum).Rapidity()-rapshift;
   double diMass = (genlep1.Momentum+genlep2.Momentum).M();
	CompareGenLepton(&genlep1, &genlep2, &leadGenLep, &subGenLep);
	
	if( leadGenLep.Pt > LeadPtCut && fabs(leadGenLep.eta) < LeadEtaCut &&
		subGenLep.Pt  > SubPtCut  && fabs(subGenLep.eta) < SubEtaCut  &&
      diRap>rapbin_60120[0] && diRap<rapbin_60120[rapbinnum_60120] &&
      diMass>bins[0] && diMass<bins[binnum] )
		isPassAcc = 1;

	return isPassAcc;
}

void DYAnalyzer::CompareMuon(Muon *Mu1, Muon *Mu2, Muon *leadMu, Muon *subMu)
{
    if( Mu1->Pt > Mu2->Pt )
    {
        *leadMu = *Mu1;
        *subMu = *Mu2;
    }
    else
    {
        *leadMu = *Mu2;
        *subMu = *Mu1;
    }
}

void DYAnalyzer::CompareGenLepton(GenLepton *genlep1, GenLepton *genlep2, GenLepton *leadgenlep, GenLepton *subgenlep)
{
	if( genlep1->Pt > genlep2->Pt )
	{
		*leadgenlep = *genlep1;
		*subgenlep = *genlep2;
	}
	else
	{
		*leadgenlep = *genlep2;
		*subgenlep = *genlep1;
	}
}

void DYAnalyzer::DimuonVertexProbNormChi2(NtupleHandle *ntuple, Double_t Pt1, Double_t Pt2, Double_t *VtxProb, Double_t *VtxNormChi2)
{
	vector<double> *PtCollection1 = ntuple->vtxTrkCkt1Pt;
	vector<double> *PtCollection2 = ntuple->vtxTrkCkt2Pt;
	vector<double> *VtxProbCollection = ntuple->vtxTrkProb;

	Int_t NPt1 = (Int_t)PtCollection1->size();
	Int_t NPt2 = (Int_t)PtCollection2->size();
	Int_t NProb = (Int_t)VtxProbCollection->size();

	if( NPt1 != NPt2 || NPt2 != NProb || NPt1 != NProb ) 
		cout << "NPt1: " << NPt1 << " NPt2: " << NPt2 << " Nprob: " << NProb << endl;

	// cout << "Pt1: " << Pt1 << " Pt2: " << Pt2 << endl;

	for(Int_t i=0; i<NProb; i++)
	{
		// cout << "\tPtCollection1->at("<< i << "): " << PtCollection1->at(i) << " PtCollection2->at("<< i << "): " << PtCollection2->at(i) << endl;
		if( ( PtCollection1->at(i) == Pt1 && PtCollection2->at(i) == Pt2 )  || ( PtCollection1->at(i) == Pt2 && PtCollection2->at(i) == Pt1 ) )
		{
			*VtxProb = VtxProbCollection->at(i);
			*VtxNormChi2 = ntuple->vtxTrkChi2->at(i) / ntuple->vtxTrkNdof->at(i);
			break;
		}
	}

	return;
}

// -- Event selecton for the electron channel (test) -- //
Bool_t DYAnalyzer::EventSelection_Electron(vector< Electron > ElectronCollection, NtupleHandle *ntuple, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns() && elec.ecalDriven == 1 && elec.Pt > 15 )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}

// -- Event selecton for the electron channel (2016.02.11) -- //
Bool_t DYAnalyzer::EventSelection_ElectronChannel(vector< Electron > ElectronCollection, NtupleHandle *ntuple, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns() && elec.ecalDriven == 1 
			&& elec.Pt > SubPtCut && fabs(elec.etaSC) < SubEtaCut && !( fabs(elec.etaSC) > 1.4442 && fabs(elec.etaSC) < 1.566 ) )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}

// -- Event selecton for the electron channel (2016.02.11) -- //
Bool_t DYAnalyzer::EventSelection_ElectronChannel_NminusPFIso(vector< Electron > ElectronCollection, NtupleHandle *ntuple, // -- input: All electrons in a event & NtupleHandle -- //
						vector< Electron >* SelectedElectronCollection) // -- output: 2 electrons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	// -- Electron ID -- //
	vector< Electron > QElectronCollection;
	for(Int_t j=0; j<(int)ElectronCollection.size(); j++)
	{
		Electron elec = ElectronCollection[j];
		// cout << "elec.passConvVeto: " << elec.passConvVeto << endl;
		if( elec.isMediumElectron_Spring25ns_minus_PFIso() && elec.ecalDriven == 1 
			&& elec.Pt > SubPtCut && fabs(elec.etaSC) < SubEtaCut && !( fabs(elec.etaSC) > 1.4442 && fabs(elec.etaSC) < 1.566 ) )
			QElectronCollection.push_back( ElectronCollection[j] );
	}

	Int_t nQElectrons = (Int_t)QElectronCollection.size();

	// cout << "# qualified electrons: " << nQElectrons << endl;

	if( nQElectrons == 2 )
	{
		Electron recolep1 = QElectronCollection[0];
		Electron recolep2 = QElectronCollection[1];

		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Electron(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		if( reco_M > 10 && isPassAcc == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedElectronCollection->push_back( recolep1 );
			SelectedElectronCollection->push_back( recolep2 );
		}
	}
	return isPassEventSelection;

}

Bool_t DYAnalyzer::isPassAccCondition_Electron(Electron Elec1, Electron Elec2)
{
	Bool_t isPassAcc = kFALSE;
	Electron leadElec, subElec;
	CompareElectron(&Elec1, &Elec2, &leadElec, &subElec);
	if( leadElec.Pt > LeadPtCut && fabs(leadElec.etaSC) < LeadEtaCut && !( fabs(leadElec.etaSC) > 1.4442 && fabs(leadElec.etaSC) < 1.566 ) &&
		subElec.Pt  > SubPtCut  && fabs(subElec.etaSC)  < SubEtaCut && !( fabs(subElec.etaSC) > 1.4442 && fabs(subElec.etaSC) < 1.566 ) )
		isPassAcc = kTRUE;

	return isPassAcc;
}


Bool_t DYAnalyzer::isPassAccCondition_GenLepton_ECALGAP(GenLepton genlep1, GenLepton genlep2)
{
	Bool_t isPassAcc = kFALSE;

	GenLepton leadGenLep, subGenLep;
	CompareGenLepton(&genlep1, &genlep2, &leadGenLep, &subGenLep);
	
	if( leadGenLep.Pt > LeadPtCut && fabs(leadGenLep.eta) < LeadEtaCut && !( fabs(leadGenLep.eta) > 1.4442 && fabs(leadGenLep.eta) < 1.566 ) &&
		subGenLep.Pt  > SubPtCut  && fabs(subGenLep.eta) < SubEtaCut && !( fabs(subGenLep.eta) > 1.4442 && fabs(subGenLep.eta) < 1.566 ) )
		isPassAcc = 1;

	return isPassAcc;
}

void DYAnalyzer::CompareElectron(Electron *Elec1, Electron *Elec2, Electron *leadElec, Electron *subElec)
{
    if( Elec1->Pt > Elec2->Pt )
    {
        *leadElec = *Elec1;
        *subElec = *Elec2;
    }
    else
    {
        *leadElec = *Elec2;
        *subElec = *Elec1;
    }
}

void DYAnalyzer::PostToPreFSR_byDressedLepton(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection)
{
	TLorentzVector genlep_Mom_postFSR = genlep_postFSR->Momentum;

	TLorentzVector SumPhotonMom;
	SumPhotonMom.SetPxPyPzE(0,0,0,0);

	Int_t NGenOthers = ntuple->nGenOthers;
	for(Int_t i_gen=0; i_gen<NGenOthers; i_gen++)
	{
		GenOthers genlep;
		genlep.FillFromNtuple(ntuple, i_gen);

		// -- Only for the photons whose mother is muon or anti-muon -- //
		if( fabs(genlep.ID) == 22 && fabs(genlep.Mother) == 13)
		{
			
			Double_t dR = Calc_dR_GenLepton_GenOthers(*genlep_postFSR, genlep);

			// -- Sum of all photon's momentum near the post-FSR muon -- //
			if( dR < dRCut )
			{
				SumPhotonMom  = SumPhotonMom + genlep.Momentum;
				GenPhotonCollection->push_back( genlep );
			}
		}
	}

	// -- Momentum(pre-FSR) = Momentum(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
	genlep_preFSR->Momentum = genlep_Mom_postFSR + SumPhotonMom;
	genlep_preFSR->Et = genlep_preFSR->Momentum.Et();
	genlep_preFSR->Pt = genlep_preFSR->Momentum.Pt();
	genlep_preFSR->eta = genlep_preFSR->Momentum.Eta();
	genlep_preFSR->phi = genlep_preFSR->Momentum.Phi();
	genlep_preFSR->Px = genlep_preFSR->Momentum.Px();
	genlep_preFSR->Py = genlep_preFSR->Momentum.Py();
	genlep_preFSR->Pz = genlep_preFSR->Momentum.Pz();
}

void DYAnalyzer::PostToPreFSR_byDressedLepton_AllPhotons(NtupleHandle *ntuple, GenLepton *genlep_postFSR, Double_t dRCut, GenLepton *genlep_preFSR, vector< GenOthers >* GenPhotonCollection)
{
	TLorentzVector genlep_Mom_postFSR = genlep_postFSR->Momentum;

	TLorentzVector SumPhotonMom;
	SumPhotonMom.SetPxPyPzE(0,0,0,0);

	Int_t NGenOthers = ntuple->nGenOthers;
	for(Int_t i_gen=0; i_gen<NGenOthers; i_gen++)
	{
		GenOthers genlep;
		genlep.FillFromNtuple(ntuple, i_gen);

		// -- all photons within dR < 0.1 -- //
		// if( fabs(genlep.ID) == 22 && fabs(genlep.Mother) == 13)
		if( fabs(genlep.ID) == 22 )
		{
			
			Double_t dR = Calc_dR_GenLepton_GenOthers(*genlep_postFSR, genlep);

			// -- Sum of all photon's momentum near the post-FSR muon -- //
			if( dR < dRCut )
			{
				SumPhotonMom  = SumPhotonMom + genlep.Momentum;
				GenPhotonCollection->push_back( genlep );
			}
		}
	}

	// -- Momentum(pre-FSR) = Momentum(post-FSR) + Sum of all Photon's momentum near the post-FSR muon -- //
	genlep_preFSR->Momentum = genlep_Mom_postFSR + SumPhotonMom;
	genlep_preFSR->Et = genlep_preFSR->Momentum.Et();
	genlep_preFSR->Pt = genlep_preFSR->Momentum.Pt();
	genlep_preFSR->eta = genlep_preFSR->Momentum.Eta();
	genlep_preFSR->phi = genlep_preFSR->Momentum.Phi();
	genlep_preFSR->Px = genlep_preFSR->Momentum.Px();
	genlep_preFSR->Py = genlep_preFSR->Momentum.Py();
	genlep_preFSR->Pz = genlep_preFSR->Momentum.Pz();
}

TString DYAnalyzer::DecideFSRType(GenLepton preFSR1, GenLepton preFSR2, GenLepton postFSR1, GenLepton postFSR2)
{
	TString FSRType = "";

	Bool_t isPassAcc_preFSREvent = kFALSE;
	isPassAcc_preFSREvent = isPassAccCondition_GenLepton(preFSR1, preFSR2);

	Bool_t isPassAcc_postFSREvent = kFALSE;
	isPassAcc_postFSREvent = isPassAccCondition_GenLepton(postFSR1, postFSR2);


	if( isPassAcc_preFSREvent == kFALSE && isPassAcc_postFSREvent == kTRUE )
		FSRType = "A";

	else if( isPassAcc_preFSREvent == kTRUE && isPassAcc_postFSREvent == kTRUE)
		FSRType = "B";
	
	else if( isPassAcc_preFSREvent == kTRUE && isPassAcc_postFSREvent == kFALSE)
		FSRType = "C";

	else if( isPassAcc_preFSREvent == kFALSE && isPassAcc_postFSREvent == kFALSE)
		FSRType = "D";
	else
	{
		cout << "ERROR: NO FSR TYPE CORRESPONDING TO THIS EVENT" << endl;
		FSRType = "NOTAssigned";
	}

	return FSRType;
}

Double_t DYAnalyzer::Calc_dR_GenLeptons( GenLepton genlep1, GenLepton genlep2 )
{
	Double_t eta1 = genlep1.eta;
	Double_t phi1 = genlep1.phi;

	Double_t eta2 = genlep2.eta;
	Double_t phi2 = genlep2.phi;

	Double_t diff_eta = eta1 - eta2;
	Double_t diff_phi = phi1 - phi2;

	Double_t dR = sqrt( diff_eta * diff_eta + diff_phi * diff_phi );
	return dR;
}

Double_t DYAnalyzer::Calc_dR_GenLepton_GenOthers( GenLepton genlep1, GenOthers genlep2 )
{
	Double_t eta1 = genlep1.eta;
	Double_t phi1 = genlep1.phi;

	Double_t eta2 = genlep2.eta;
	Double_t phi2 = genlep2.phi;

	Double_t diff_eta = eta1 - eta2;
	Double_t diff_phi = phi1 - phi2;

	Double_t dR = sqrt( diff_eta * diff_eta + diff_phi * diff_phi );
	return dR;
}

void DYAnalyzer::GenMatching(TString MuonType, NtupleHandle* ntuple, vector<Muon>* MuonCollection)
{
	vector<GenLepton> GenLeptonCollection;
	Int_t NGenLeptons = ntuple->gnpair;

	if( MuonType == "PromptFinalState" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isPromptFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else if( MuonType == "fromTau")
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.isDirectPromptTauDecayProductFinalState )
				GenLeptonCollection.push_back( genlep );
		}

	}
	else if( MuonType == "fromHardProcess" )
	{
		for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
		{
			GenLepton genlep;
			genlep.FillFromNtuple(ntuple, i_gen);
			if( genlep.isMuon() && genlep.fromHardProcessFinalState )
				GenLeptonCollection.push_back( genlep );
		}
	}
	else
	{
		cout << "Incorrect MuonType!" << endl;
		return;
	}

	//Give Acceptance Cuts
	if( GenLeptonCollection.size() >= 2 )
	{
      if (!isPassAccCondition_GenLepton(GenLeptonCollection[0], GenLeptonCollection[1]))
			GenLeptonCollection.clear();
	}


	
	Int_t NMuons = (Int_t)MuonCollection->size();
	vector<Muon> RecoMuonCollection;
	//Copy all muons in MuonCollection into RecoMuonCollection
	for(Int_t i_mu=0; i_mu<NMuons; i_mu++)
		RecoMuonCollection.push_back( MuonCollection->at(i_mu) );

	MuonCollection->clear();

	Int_t NGen = (Int_t)GenLeptonCollection.size();
	for(Int_t i_gen=0; i_gen<NGen; i_gen++)
	{
		GenLepton genlep = GenLeptonCollection[i_gen];
		Double_t gen_Pt = genlep.Pt;
		Double_t gen_eta = genlep.eta;
		Double_t gen_phi = genlep.phi;

		Int_t i_matched = -1;
		Double_t dPtMin = 1e10;
		for(Int_t i_reco=0; i_reco<NMuons; i_reco++)
		{
			Muon mu = RecoMuonCollection[i_reco];
			Double_t reco_Pt = mu.Pt;
			Double_t reco_eta = mu.eta;
			Double_t reco_phi = mu.phi;

			Double_t dR = sqrt( (gen_eta-reco_eta)*(gen_eta-reco_eta) + (gen_phi-reco_phi)*(gen_phi-reco_phi) );
			Double_t dPt = fabs(gen_Pt - reco_Pt);
			if( dR < 0.3 )
			{
				if( dPt < dPtMin )
				{
					i_matched = i_reco;
					dPtMin = dPt;
				}
			}
		}

		if( i_matched != -1 )
			MuonCollection->push_back( RecoMuonCollection[i_matched] );
	}

	return;
}

void DYAnalyzer::ConvertToTunePInfo( Muon &mu )
{
	// -- Use TuneP information -- //
	mu.Pt = mu.TuneP_pT;
	mu.eta = mu.TuneP_eta;
	mu.phi = mu.TuneP_phi;

	Double_t Px = mu.TuneP_Px;
	Double_t Py = mu.TuneP_Py;
	Double_t Pz = mu.TuneP_Pz;
	Double_t E = sqrt( Px*Px + Py*Py + Pz*Pz + M_Mu*M_Mu );
	mu.Momentum.SetPxPyPzE( Px, Py, Pz, E );
}

void DYAnalyzer::PrintOutDoubleMuInfo( Muon mu1, Muon mu2 )
{
	printf("\t[Muon1] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu1.Pt, mu1.eta, mu1.phi, mu1.charge);
	printf("\t[Muon2] (pT, eta, phi, charge) = (%10.5lf, %10.5lf, %10.5lf, %.1d)\n", mu2.Pt, mu2.eta, mu2.phi, mu2.charge);
	Double_t reco_M = ( mu1.Momentum + mu2.Momentum ).M();
	printf("\t\tDilepton Mass: %10.5lf\n", reco_M);

}

Bool_t DYAnalyzer::EventSelection_Dijet(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > PassingMuonCollection;
	vector< Muon > FailingMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
      if( MuonCollection[j].isTightMuon() ) {
         // for L1DoubleMu0 / low mass: use PF iso
         if (HLT.Contains("L1DoubleMu0") && MuonCollection[j].relPFiso < 0.3)
            PassingMuonCollection.push_back( MuonCollection[j] );
         else
            FailingMuonCollection.push_back( MuonCollection[j] );
         // for L1DoubleMu0 / low mass: use trkiso
         if (HLT.Contains("L3Mu12") && MuonCollection[j].trkiso < 0.3)
            PassingMuonCollection.push_back( MuonCollection[j] );
         else
            FailingMuonCollection.push_back( MuonCollection[j] );
      }
	}

	Int_t nFailMuon = (Int_t)FailingMuonCollection.size();

	if( nFailMuon >= 2 ) // -- Dijet events: contains more than 2 failing muons regardless of # passing muons -- // 
	{
		if( nFailMuon == 2 )
		{
			Muon recolep1 = FailingMuonCollection[0];
			Muon recolep2 = FailingMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			Bool_t isOS = kFALSE;
			if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}

		} // -- end of if( nFailMuon == 2 ) -- //
		else // -- # failing muons > 2 -- // 
		{
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nFailMuon; i_mu++)
			{
				Muon Mu = FailingMuonCollection[i_mu];

				// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
				for(Int_t j_mu=0; j_mu<nFailMuon; j_mu++)
				{
					Muon Mu_jth = FailingMuonCollection[j_mu];

					if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
					{
						// -- Check that this pair is within acceptance -- //
						Bool_t isPassAcc = kFALSE;
						isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

						if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
						{
							Double_t VtxProb_temp = -999;
							Double_t VtxNormChi2_temp = 999;
							DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

							// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
							if( VtxNormChi2_temp < VtxNormChi2_BestPair )
							{
								VtxNormChi2_BestPair = VtxNormChi2_temp;
								mu1_BestPair = Mu;
								mu2_BestPair = Mu_jth;
							}
						}
					}
				} // -- end of the loop for j_mu (finding for second muon)
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				Bool_t isOS = kFALSE;
				if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

				if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- end of (# failing muons > 2) case -- //

	} // -- end of if( nFailMuon >= 2 ) -- //

	return isPassEventSelection;
}

Bool_t DYAnalyzer::EventSelection_Wjet(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > PassingMuonCollection;
	vector< Muon > FailingMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
      if( MuonCollection[j].isTightMuon() ) {
         // for L1DoubleMu0 / low mass: use PF iso
         if (HLT.Contains("L1DoubleMu0") && MuonCollection[j].relPFiso < 0.3)
            PassingMuonCollection.push_back( MuonCollection[j] );
         else
            FailingMuonCollection.push_back( MuonCollection[j] );
         // for L1DoubleMu0 / low mass: use trkiso
         if (HLT.Contains("L3Mu12") && MuonCollection[j].trkiso < 0.3)
            PassingMuonCollection.push_back( MuonCollection[j] );
         else
            FailingMuonCollection.push_back( MuonCollection[j] );
      }
	}

	Int_t nFailMuon = (Int_t)FailingMuonCollection.size();
	Int_t nPassMuon = (Int_t)PassingMuonCollection.size();

	if( nFailMuon == 1 && nPassMuon == 1) // -- W+Jets events: exactly (# pass muon , # fail muon ) = (1, 1) -- //
	{
		Muon recolep1 = PassingMuonCollection[0]; // -- first one: passing muon -- //
		Muon recolep2 = FailingMuonCollection[1]; // -- second one: failing muon -- //

		// -- Check the Accpetance -- //
		Bool_t isPassAcc = kFALSE;
		isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

		Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

		Double_t VtxProb = -999;
		Double_t VtxNormChi2 = 999;
		DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

		TLorentzVector inner_v1 = recolep1.Momentum_Inner;
		TLorentzVector inner_v2 = recolep2.Momentum_Inner;

		// -- 3D open angle -- //
		Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

		Bool_t isOS = kFALSE;
		if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

		// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
		if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
		{
			isPassEventSelection = kTRUE;
			SelectedMuonCollection->push_back( recolep1 ); // -- first one: passing muon -- //
			SelectedMuonCollection->push_back( recolep2 ); // -- second one: failing muon -- //
		}
	}

	return isPassEventSelection;
}

Bool_t DYAnalyzer::EventSelection_CheckMoreThanOneDimuonCand(vector< Muon > MuonCollection, NtupleHandle *ntuple, // -- input: All muons in a event & NtupleHandle -- //
						vector< Muon >* SelectedMuonCollection, Bool_t& isMoreThanOneCand) // -- output: 2 muons passing event selection conditions -- //
{
	Bool_t isPassEventSelection = kFALSE;
	isMoreThanOneCand = kFALSE;

	//Collect qualified muons among muons
	vector< Muon > QMuonCollection;
	for(Int_t j=0; j<(int)MuonCollection.size(); j++)
	{
      if( MuonCollection[j].isTightMuon() ) {
         // for L1DoubleMu0 / low mass: use PF iso
         if (HLT.Contains("L1DoubleMu0") && MuonCollection[j].relPFiso < 0.3)
            QMuonCollection.push_back( MuonCollection[j] );
         // for L1DoubleMu0 / low mass: use trkiso
         if (HLT.Contains("L3Mu12") && MuonCollection[j].trkiso < 0.3)
            QMuonCollection.push_back( MuonCollection[j] );
      }
	}

	// -- Check the existence of at least one muon matched with HLT-object -- //
	Bool_t isExistHLTMatchedMuon = kFALSE;
	for(Int_t i_mu=0; i_mu<(Int_t)QMuonCollection.size(); i_mu++)
	{
		Muon mu = QMuonCollection[i_mu];
		if( mu.isTrigMatched(ntuple, "HLT_PAL3Mu12_v*") )
		{
			isExistHLTMatchedMuon = kTRUE;
			break;
		}
	}

	if( isExistHLTMatchedMuon == kTRUE )
	{
		Int_t nQMuons = (Int_t)QMuonCollection.size();
		if( nQMuons == 2)
		{
			Muon recolep1 = QMuonCollection[0];
			Muon recolep2 = QMuonCollection[1];

			// -- Check the Accpetance -- //
			Bool_t isPassAcc = kFALSE;
			isPassAcc = isPassAccCondition_Muon(recolep1, recolep2);

			Double_t reco_M = (recolep1.Momentum + recolep2.Momentum).M();

			Double_t VtxProb = -999;
			Double_t VtxNormChi2 = 999;
			DimuonVertexProbNormChi2(ntuple, recolep1.Inner_pT, recolep2.Inner_pT, &VtxProb, &VtxNormChi2);

			TLorentzVector inner_v1 = recolep1.Momentum_Inner;
			TLorentzVector inner_v2 = recolep2.Momentum_Inner;

			// -- 3D open angle -- //
			Double_t Angle = recolep1.Momentum.Angle( recolep2.Momentum.Vect() );

			Bool_t isOS = kFALSE;
			if( recolep1.charge != recolep2.charge ) isOS = kTRUE;

			// if( reco_M > 10 && isPassAcc == kTRUE && Chi2/ndof(VTX) < 20 && Angle < TMath::Pi() - 0.005 )
			if( reco_M > 10 && isPassAcc == kTRUE && VtxNormChi2 < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
			{
				isPassEventSelection = kTRUE;
				SelectedMuonCollection->push_back( recolep1 );
				SelectedMuonCollection->push_back( recolep2 );
			}
		}
		else if( nQMuons > 2 )
		{
			isMoreThanOneCand = kTRUE;
			Double_t VtxProb_BestPair = -1;
			Double_t VtxNormChi2_BestPair = 999;
			Muon mu1_BestPair;
			Muon mu2_BestPair;

			for(Int_t i_mu=0; i_mu<nQMuons; i_mu++)
			{
				Muon Mu = QMuonCollection[i_mu];

				// -- at least 1 muon should be matched with HLT objects in best pair -- //
				if( Mu.isTrigMatched(ntuple, "HLT_PAL3Mu12_v*") )
				{
					// -- Mu in this loop: QMuon Matched with HLT object -- //

					// -- Start another loop for finding second muon (for second muon, we don't need to check trigger matching) -- //
					for(Int_t j_mu=0; j_mu<nQMuons; j_mu++)
					{
						Muon Mu_jth = QMuonCollection[j_mu];

						if( j_mu != i_mu ) // -- do not calculate vertex variables(prob, chi2). with itself -- //
						{
							// -- Check that this pair is within acceptance -- //
							Bool_t isPassAcc = kFALSE;
							isPassAcc = isPassAccCondition_Muon(Mu, Mu_jth);

							if( isPassAcc == kTRUE ) // -- Find best pair ONLY for the pairs within acceptance -- //
							{
								Double_t VtxProb_temp = -999;
								Double_t VtxNormChi2_temp = 999;
								DimuonVertexProbNormChi2(ntuple, Mu.Inner_pT, Mu_jth.Inner_pT, &VtxProb_temp, &VtxNormChi2_temp);

								// -- Find best pair by selecting smallest Chi2/dnof(VTX) value -- // 
								if( VtxNormChi2_temp < VtxNormChi2_BestPair )
								{
									VtxNormChi2_BestPair = VtxNormChi2_temp;
									mu1_BestPair = Mu;
									mu2_BestPair = Mu_jth;
								}
							}
						}
					} // -- end of the loop for j_mu (finding for second muon)
				}
			} // -- end of the loop for i_mu (finding for the first muon matched with HLT matching)

			if( VtxNormChi2_BestPair < 999 ) // -- If at least one pair within acceptance & with at least one muon matched with HLT object exists -- //
			{
				TLorentzVector reco_v1 = mu1_BestPair.Momentum;
				TLorentzVector reco_v2 = mu2_BestPair.Momentum;
				Double_t reco_M = (reco_v1 + reco_v2).M();

				// -- 3D open angle is calculated using inner track information -- //
				// -- 3D open angle -- //
				Double_t Angle = reco_v1.Angle( reco_v2.Vect() );

				Bool_t isOS = kFALSE;
				if( mu1_BestPair.charge != mu2_BestPair.charge ) isOS = kTRUE;

				if( reco_M > 10 && VtxNormChi2_BestPair < 20 && Angle < TMath::Pi() - 0.005 && isOS == kTRUE )
				{
					isPassEventSelection = kTRUE;
					SelectedMuonCollection->push_back( mu1_BestPair );
					SelectedMuonCollection->push_back( mu2_BestPair );
				}
			}

		} // -- End of else if( nQMuons > 2 ) -- //

	} // -- End of if( isExistHLTMatchedMuon == kTRUE ) -- //

	return isPassEventSelection;
}





