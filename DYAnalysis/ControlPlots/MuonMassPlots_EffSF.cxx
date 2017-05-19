#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TApplication.h>
#include <vector>
#include <TMath.h>

// -- for Rochester Muon momentum correction -- //
#include <Include/RochesterMomCorr_76X/RoccoR.cc>
#include <Include/RochesterMomCorr_76X/rochcor2015.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>

static inline void loadBar(int x, int n, int r, int w);
void MuonMassPlots_EffSF(Bool_t isCorrected = kTRUE, TString Type = "MCBkg", TString HLTname = "IsoMu20_OR_IsoTkMu20")
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Type: " << Type << endl;
	TString isApplyMomCorr = "";
	if( isCorrected == kTRUE )
	{
		cout << "Apply Roochester Muon Momentum Correction..." << endl;
		isApplyMomCorr = "MomCorr";
	}
	else
	{
		cout << "DO *NOT* Apply Roochester Muon Momentum Correction..." << endl;
		isApplyMomCorr = "MomUnCorr";
	}

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );
	analyzer->SetupEfficiencyScaleFactor( "ROOTFile_TagProbeEfficiency_76X_v20160502.root" );

	TFile *f = new TFile("ROOTFile_Histogram_InvMass_" + HLTname + "_" + Type + "_" + isApplyMomCorr + ".root", "RECREATE");

	TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
	//Each ntuple directory & corresponding Tags
	vector<TString> ntupleDirectory; vector<TString> Tag; vector<Double_t> Xsec; vector<Double_t> nEvents;

	if( Type == "MC" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("Full_AdditionalSF", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}
	else if( Type == "Golden" )
	{
		// ntupleDirectory.push_back( "Run2015C/GoldenJSON/SingleMuon_v3_Run246908to256869" ); Tag.push_back( "Data" ); // -- Run2015C -- //
	}
	else if( Type == "MuonPhys" )
	{
		ntupleDirectory.push_back( "76X/v20160205_SingleMuon_RunC_Rereco_MuonPhys" ); Tag.push_back( "Data" ); // -- Run2015C -- //
	}
	else if( Type == "MCBkg" )
	{
		analyzer->SetupMCsamples_v20160309_76X_MiniAODv2("MCBkg", &ntupleDirectory, &Tag, &Xsec, &nEvents);
	}
	else
	{
		cout << "ERROR: Possible Type: Golden, MuonPhys, and MC" << endl;
		return;
	}

	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation + "/" + ntupleDirectory[i_tup] + "/ntuple_*.root");
		if( Tag[i_tup] == "Data" && Type == "Golden" )
		{
			// -- Run2015D -- // 
		}
		else if( Tag[i_tup] == "Data" && Type == "MuonPhys" )
		{
			// -- Run2015D -- // 
			chain->Add(BaseLocation+"/76X/v20160303_SingleMuon_RunD_Rereco_MuonPhys/*.root");
		}
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_Muon();
		
		rochcor2015 *rmcor = new rochcor2015();

		Bool_t isMC;
		Tag[i_tup] == "Data" ? isMC = kFALSE : isMC = kTRUE;
		analyzer->SetupPileUpReWeighting_76X( isMC );

		TH1D* h_mass_HLTv4p2 = new TH1D("h_mass_OS_HLTV4p2_"+Tag[i_tup], "", 10000, 0, 10000);
		TH1D* h_mass_HLTv4p3 = new TH1D("h_mass_OS_HLTV4p3_"+Tag[i_tup], "", 10000, 0, 10000);

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Double_t SumWeight = 0;
		Double_t SumWeight_Separated = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
		// NEvents = 100000;
		for(Int_t i=0; i<NEvents; i++)
		{
			loadBar(i+1, NEvents, 100, 100);
			
			ntuple->GetEvent(i);

			/////////////////////////////
			// -- Bring the weights -- //
			/////////////////////////////
				// -- Positive/Negative Gen-weights -- //
			Double_t GenWeight;
			if( isNLO == 1 )
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
			else
				GenWeight = 1;

			SumWeight += GenWeight;

			// -- Pileup-Reweighting -- //
			Double_t PUWeight = analyzer->PileUpWeightValue_76X( ntuple->nPileUp );

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == 1 )
				SumWeight_Separated += GenWeight;

			if( ntuple->isTriggered( analyzer->HLT ) && GenFlag)
			{
				//Collect Reconstruction level information
				vector< Muon > MuonCollection;
				Int_t NLeptons = ntuple->nMuon;
				for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
				{
					Muon mu;
					mu.FillFromNtuple(ntuple, i_reco);

					// -- Apply Rochester momentum scale correction -- //
					if( isCorrected == kTRUE )
					{
						float qter = 1.0;
						
						if( Tag[i_tup] == "Data" )
							rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
						else
							rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

						// -- Change Muon pT, eta and phi with updated(corrected) one -- //
						mu.Pt = mu.Momentum.Pt();
						mu.eta = mu.Momentum.Eta();
						mu.phi = mu.Momentum.Phi();
					}
					
					MuonCollection.push_back( mu );
				}

				// -- Event Selection -- //
				vector< Muon > SelectedMuonCollection;
				Bool_t isPassEventSelection = kFALSE;
				isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

				if( isPassEventSelection == kTRUE )
				{
					Muon mu1 = SelectedMuonCollection[0];
					Muon mu2 = SelectedMuonCollection[1];
					Double_t reco_M = (mu1.Momentum + mu2.Momentum).M();

					Double_t Eff_SF_HLTv4p2 = analyzer->EfficiencySF_EventWeight_HLTv4p2( SelectedMuonCollection[0], SelectedMuonCollection[1] );
					Double_t Eff_SF_HLTv4p3 = analyzer->EfficiencySF_EventWeight_HLTv4p3( SelectedMuonCollection[0], SelectedMuonCollection[1] );

					h_mass_HLTv4p2->Fill( reco_M, GenWeight*PUWeight*Eff_SF_HLTv4p2	);
					h_mass_HLTv4p3->Fill( reco_M, GenWeight*PUWeight*Eff_SF_HLTv4p3	);
				}
				
			} //End of if( isTriggered )

		} //End of event iteration

		f->cd();
		h_mass_HLTv4p2->Write();
		h_mass_HLTv4p3->Write();

		h_mass_HLTv4p2->Sumw2();
		h_mass_HLTv4p3->Sumw2();

		// -- Combine -- //
		Double_t Norm_HLTv4p2 = (Lumi_HLTv4p2 * Xsec[i_tup] ) / nEvents[i_tup];
		TH1D* h_mass_HLTv4p2_Norm = (TH1D*)h_mass_HLTv4p2->Clone( "h_mass_OS_HLTv4p2_Norm_"+Tag[i_tup] );
		h_mass_HLTv4p2_Norm->Scale( Norm_HLTv4p2 );

		Double_t Norm_HLTv4p3 = ( (Lumi - Lumi_HLTv4p2) * Xsec[i_tup] ) / nEvents[i_tup];
		TH1D* h_mass_HLTv4p3_Norm = (TH1D*)h_mass_HLTv4p3->Clone( "h_mass_OS_HLTv4p3_Norm_"+Tag[i_tup] );
		h_mass_HLTv4p3_Norm->Scale( Norm_HLTv4p3 );

		TH1D* h_mass = (TH1D*)h_mass_HLTv4p2_Norm->Clone( "h_mass_OS_Norm_"+Tag[i_tup] );
		h_mass->Add( h_mass_HLTv4p3_Norm );

		h_mass_HLTv4p2_Norm->Write();
		h_mass_HLTv4p3_Norm->Write();
		h_mass->Write(); // -- it is already normalized -- //

		if(isNLO == 1)
		{
			printf("\tTotal sum of weights: %.1lf\n", SumWeight);
			printf("\tSum of weights of Seperated events: %.1lf\n", SumWeight_Separated);
		}

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	Double_t TotalRunTime = totaltime.CpuTime();
	cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

	TTimeStamp ts_end;
	cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;
}

static inline void loadBar(int x, int n, int r, int w)
{
    // Only update r times.
    if( x == n )
    	cout << endl;

    if ( x % (n/r +1) != 0 ) return;

 
    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;
 
    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );
 
    // Show the load bar.
    for (int x=0; x<c; x++) cout << "=";
 
    for (int x=c; x<w; x++) cout << " ";
 
    // ANSI Control codes to go back to the
    // previous line and clear it.
	cout << "]\r" << flush;

}