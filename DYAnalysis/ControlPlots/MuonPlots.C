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
#include <Include/ControlPlots.h>

static inline void loadBar(int x, int n, int r, int w);
void MuonPlots(Bool_t isCorrected = kFALSE, TString Type = "MC", TString HLTname = "PAL3Mu12")
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
   bool doData = Type.Contains("Data");
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

	TFile *f = new TFile("ROOTFile_Histogram_InvMass_" + HLTname + "_" + Type + "_" + isApplyMomCorr + ".root", "RECREATE");

   // TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
   // TString BaseLocation = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/trees_20170518/";
	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";

	//Each ntuple directory & corresponding Tags
	vector<TString> ntupleDirectory; vector<TString> Tag; vector<Double_t> Xsec; vector<Double_t> nEvents; vector< DYana::SampleTag > STags;

	if( !doData )
	{
		analyzer->SetupMCsamples_v20170830("", &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);
	}
	else
	{
		ntupleDirectory.push_back( "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20170518/170517_220343/0000/" ); Tag.push_back( "Data1" );
      ntupleDirectory.push_back( "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20170518/170517_220714/0000/" ); Tag.push_back( "Data2" );
	}

	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

      Bool_t doflip = (switcheta(STags[i_tup])<0);

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation + "/" + ntupleDirectory[i_tup]+"/ntuple_*.root");
      // if( Tag[i_tup].Contains("Data") && doData )
      // {
      //    // -- Run2015D -- // 
      //    chain->Add(BaseLocation+"/80X/v20170519/*.root");
      // }
		NtupleHandle *ntuple = new NtupleHandle( chain, doflip );
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_HI();
		
		rochcor2015 *rmcor = new rochcor2015();

		Bool_t isMC;
		Tag[i_tup].Contains("Data") ? isMC = kFALSE : isMC = kTRUE;
		analyzer->SetupPileUpReWeighting( isMC );

		ControlPlots *Plots = new ControlPlots( Tag[i_tup], analyzer );

		TH1D *h_PU = new TH1D("h_PU_"+Tag[i_tup], "", 50, 0, 50);
		TH1D *h_nVertices_before = new TH1D("h_nVertices_before_"+Tag[i_tup], "", 50, 0, 50);
		TH1D *h_nVertices_after = new TH1D("h_nVertices_after_"+Tag[i_tup], "", 50, 0, 50);

      // HI stuff
		TH1D *h_hiHF = new TH1D("h_hiHF_"+Tag[i_tup], "", 110, 0, 550);
		TH1D *h_hiHFplus = new TH1D("h_hiHFplus_"+Tag[i_tup], "", 110, 0, 550);
		TH1D *h_hiHFminus = new TH1D("h_hiHFminus_"+Tag[i_tup], "", 110, 0, 550);
		TH1D *h_hiNtracks = new TH1D("h_hiNtracks_"+Tag[i_tup], "", 90, 0, 450);


		Bool_t isNLO = 0;
      // NO NLO in pA
      // if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
      // {
      //    isNLO = 1;
      //    cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
      // }

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
			Double_t PUWeight = analyzer->PileUpWeightValue( ntuple->nPileUp );

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == 1 )
				SumWeight_Separated += GenWeight;

			// -- Fill the histogram for gen-level information (signal sample) -- //
			if( GenFlag == 1 && Tag[i_tup].Contains("DYMuMu") )
			{
				vector<GenLepton> GenLeptonCollection;
				Int_t NGenLeptons = ntuple->gnpair;
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( genlep.isMuon() && genlep.isHardProcess )
					{
						GenLeptonCollection.push_back( genlep );

						if( GenLeptonCollection.size() ==  2 )
							break;
					}
				}
				Plots->FillHistograms_GenDoubleMu(ntuple, GenLeptonCollection[0], GenLeptonCollection[1], GenWeight);
			}

			if( ntuple->isTriggered( analyzer->HLT )  && GenFlag) 
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
						
						if( doData )
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
					Plots->FillHistograms_DoubleMu(ntuple, mu1, mu2, GenWeight*PUWeight);

					Int_t PU = ntuple->nPileUp;
					h_PU->Fill( PU, PUWeight );

					Int_t nVertices = ntuple->nVertices;
					h_nVertices_before->Fill(nVertices, GenWeight);
					h_nVertices_after->Fill(nVertices, GenWeight*PUWeight);

               h_hiHF->Fill(ntuple->hiHF,GenWeight*PUWeight);
               h_hiHFplus->Fill(ntuple->hiHFplus,GenWeight*PUWeight);
               h_hiHFminus->Fill(ntuple->hiHFminus,GenWeight*PUWeight);
               h_hiNtracks->Fill(ntuple->hiNtracks,GenWeight*PUWeight);
				}
				
			} //End of if( isTriggered )

		} //End of event iteration

		Plots->WriteHistograms( f );
		h_PU->Write();
		h_nVertices_before->Write();
		h_nVertices_after->Write();
      h_hiHF->Write();
      h_hiHFplus->Write();
      h_hiHFminus->Write();
      h_hiNtracks->Write();

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

