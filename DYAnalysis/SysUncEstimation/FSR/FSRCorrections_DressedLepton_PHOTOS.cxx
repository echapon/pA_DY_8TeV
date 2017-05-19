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
#include <TEfficiency.h>
#include <stdio.h>

#include <vector>

//Customized header files
#include <DYAnalysis_76X/CommonCodes/DYAnalyzer_v01.h>

// -- RooUnfold Package -- //
#include <Unfolding/src/RooUnfoldResponse.h>
#include <Unfolding/src/RooUnfoldBayes.h>
#include <Unfolding/src/RooUnfoldInvert.h>

static inline void loadBar(int x, int n, int r, int w);
void FillNtupleInformation_PHOTOS( vector<TString> &ntupleDirectory, vector<TString> &Tag, vector<Double_t> &Xsec, vector<Double_t> &nEvents );
void GenLevelMassSpectrum(TFile *f_output);

void FSRCorrections_DressedLepton_PHOTOS( TString HLTname = "IsoMu20_OR_IsoTkMu20" )
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

	// const Int_t nMassBin = 43;
	Double_t MassBinEdges[nMassBin+1] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60,
										 64, 68, 72, 76, 81, 86, 91, 96, 101, 106,
										 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,
										 200, 220, 243, 273, 320, 380, 440, 510, 600, 700,
										 830, 1000, 1500, 3000};

	TString BaseLocation = "/home/kplee/Physics/DYAnalysis_76X/Investigation/PHOTOS/v20161018_1st_ChangeMassScale_RepM/FlatNtupleMaker";

	// -- Each ntuple directory & corresponding Tags -- //
	// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
	FillNtupleInformation_PHOTOS( ntupleDirectory, Tag, Xsec, nEvents );

	TFile *f = new TFile("ROOTFile_FSRCorrections_DressedLepton_PHOTOS.root", "RECREATE");
	TH1D *h_mass_preFSR_tot = new TH1D("h_mass_preFSR", "", nMassBin, MassBinEdges);
	TH1D *h_mass_postFSR_tot = new TH1D("h_mass_postFSR", "", nMassBin, MassBinEdges);
	TH1D *h_mass_ratio_tot = new TH1D("h_mass_ratio", "", 100, -1, 1);

	RooUnfoldResponse *UnfoldRes = new RooUnfoldResponse(h_mass_postFSR_tot, h_mass_preFSR_tot);
	UnfoldRes->SetName("UnfoldRes");

	// const Int_t ndRCuts = 10;
	// Double_t dRCuts[ndRCuts] = {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5};
	// TH1D *h_nPhotons[ndRCuts];
	// TH1D *h_RatioE[ndRCuts];
	// TH1D *h_SumE[ndRCuts];
	// for(Int_t i=0; i<ndRCuts; i++)
	// {
	// 	h_nPhotons[i] = new TH1D("h_nPhotons_"+TString::Format("dR%.2f", dRCuts[i]), "", 10, 0, 10);
	// 	h_RatioE[i] = new TH1D("h_RatioE_"+TString::Format("dR%.2f", dRCuts[i]), "", 1000, -1, 1);
	// 	h_SumE[i] = new TH1D("h_SumE_"+TString::Format("dR%.2f", dRCuts[i]), "", 20000, 0, 200);
	// }

	vector <TH1D* > GammaHisto;
	TH1D* h_GammaE = new TH1D("h_GammaE", "", 20000, 0, 200); GammaHisto.push_back( h_GammaE );
	TH1D* h_GammaEta = new TH1D("h_GammaEta", "", 400, -20, 20); GammaHisto.push_back( h_GammaEta );
	TH1D* h_GammaPhi = new TH1D("h_GammaPhi", "", 80, -4, 4); GammaHisto.push_back( h_GammaPhi );
	TH1D* h_GammadR = new TH1D("h_GammadR", "", 3000, 0, 30); GammaHisto.push_back( h_GammadR );
	TH1D* h_GammaN = new TH1D("h_GammaN", "", 20, 0, 20); GammaHisto.push_back( h_GammaN );
	TH1D* h_GammaSumE = new TH1D("h_GammaSumE", "", 20000, 0, 200); GammaHisto.push_back( h_GammaSumE );

	// TH1D* h_GammaMother = new TH1D("h_GammaMother", "", 202000, -101000, 101000); GammaHisto.push_back( h_GammaMother );
	// TH1D* h_GammaStatus = new TH1D("h_GammaStatus", "", 3000, -1500, 1500); GammaHisto.push_back( h_GammaStatus );
	// TH2D* h_dR_E = new TH2D("h_dR_E", "", 60, 0, 30, 400, 0, 200);

	// const Int_t nStatus = 5;
	// Int_t Statuses[nStatus] = {1, 51, 44, 43, 33};
	// TH1D *h_dR_Status[nStatus];
	// for(Int_t i=0; i<nStatus; i++)
	// {
	// 	h_dR_Status[i] = new TH1D("h_dR_Status_"+TString::Format("%d", Statuses[i]), "", 3000, 0, 30);
	// }

	// const Int_t nMother = 6;
	// Int_t Mothers[nMother] = {111, 221, 22, 223, 13, 3212};
	// TH1D *h_dR_Mother[nMother];
	// for(Int_t i=0; i<nMother; i++)
	// {
	// 	h_dR_Mother[i] = new TH1D("h_dR_Mother_"+TString::Format("%d", Mothers[i]), "", 3000, 0, 30);
	// }

	// TH2D *h_Mother_Status = new TH2D("h_Mother_Status", "", nMother, 0, nMother, nStatus, 0, nStatus);

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TH1D *h_mass_preFSR = new TH1D("h_mass_preFSR_"+Tag[i_tup], "", nMassBin, MassBinEdges);
		TH1D *h_mass_postFSR = new TH1D("h_mass_postFSR_"+Tag[i_tup], "", nMassBin, MassBinEdges);
		TH1D *h_mass_ratio = new TH1D("h_mass_ratio_"+Tag[i_tup], "", 100, -1, 1);

		TH1D *h_mass_LHE = new TH1D("h_mass_LHE_"+Tag[i_tup], "", 10000, 0, 10000);

		// -- ntuple Setting -- //
		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		if( Tag[i_tup] == "DYMuMu_Photos_M50to100" )
			chain->Add(BaseLocation+"/M50toInf_part2/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_LHEInfo();
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_GenOthers();

		// analyzer->SetupPileUpReWeighting( Tag[i_tup] );

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Double_t SumWeights = 0;
		Double_t SumWeights_Separated = 0;
		Int_t SumEvents_Separated = 0;

		Int_t NEvents = chain->GetEntries();

		// NEvents = 10000;
		cout << "\t[Total Events: " << NEvents << "]" << endl; 

		Double_t norm = ( Xsec[i_tup] * Lumi ) / (Double_t)nEvents[i_tup];
		cout << "\t[Normalization factor: " << norm << "]" << endl;

		// -- Event loop starts -- //
		for(Int_t i=0; i<NEvents; i++)
		{
			// printf("[%dth Event]\n", i);

			loadBar(i+1, NEvents, 100, 100);

			ntuple->GetEvent(i);

			//Bring weights for NLO MC events
			Double_t GenWeight;
			if( isNLO == 1 )
				ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
			else
				GenWeight = 1;

			SumWeights += GenWeight;

			// Int_t PU = ntuple->nPileUp;
			// Double_t PUWeight = analyzer->PileUpWeightValue( PU );

			// Double_t TotWeight = norm * GenWeight * PUWeight;
			Double_t TotWeight = norm * GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_LHEInfo(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
				SumWeights_Separated += GenWeight;
				SumEvents_Separated++;

				// -- fill the mass histogram from LHE -- //
				vector<LHELepton> LHELeptonCollection;
				Int_t nLHELepton = ntuple->nLHEParticle;
				for(Int_t i_lhe=0; i_lhe<nLHELepton; i_lhe++)
				{
					LHELepton lhelep;
					lhelep.FillFromNtuple(ntuple, i_lhe);
					if( fabs(lhelep.ID) == 13 )
						LHELeptonCollection.push_back( lhelep );
				}
				TLorentzVector v1 = LHELeptonCollection[0].Momentum;
				TLorentzVector v2 = LHELeptonCollection[1].Momentum;
				Double_t M_LHE = (v1 + v2).M();
				h_mass_LHE->Fill( M_LHE, GenWeight );


				vector<GenLepton> GenLeptonCollection_FinalState;
				Int_t NGenLeptons = ntuple->gnpair;
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( genlep.isMuon() && genlep.fromHardProcessFinalState )
					{
						GenLeptonCollection_FinalState.push_back( genlep );

						if( (Int_t)GenLeptonCollection_FinalState.size() == 2 )
							break;
					}
				}

				Double_t dRCut = 0.1;

				GenLepton genlep_postFSR1 = GenLeptonCollection_FinalState[0];
				GenLepton genlep_preFSR1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
				vector< GenOthers > GenPhotonCollection1;
				analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_preFSR1, &GenPhotonCollection1);

				GenLepton genlep_postFSR2 = GenLeptonCollection_FinalState[1];
				GenLepton genlep_preFSR2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
				vector< GenOthers > GenPhotonCollection2;
				analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_preFSR2, &GenPhotonCollection2);

				// -- Fill the histograms for the photons near post-FSR muon1 -- //
				Int_t nPhotons1 = (Int_t)GenPhotonCollection1.size();
				GammaHisto[4]->Fill( nPhotons1, TotWeight );
				TLorentzVector SumPhotonMom1; SumPhotonMom1.SetPxPyPzE(0, 0, 0, 0);
				for(Int_t i_photon=0; i_photon<nPhotons1; i_photon++)
				{
					GenOthers photon = GenPhotonCollection1[i_photon];

					GammaHisto[0]->Fill(photon.Momentum.E(), TotWeight);
					GammaHisto[1]->Fill(photon.eta, TotWeight);
					GammaHisto[2]->Fill(photon.phi, TotWeight);

					Double_t dR = analyzer->Calc_dR_GenLepton_GenOthers( genlep_postFSR1, photon);
					GammaHisto[3]->Fill(dR, TotWeight);

					SumPhotonMom1 = SumPhotonMom1 + photon.Momentum;
				}
				h_GammaSumE->Fill( SumPhotonMom1.E(), TotWeight );

				// -- Fill the histograms for the photons near post-FSR muon2 -- //
				Int_t nPhotons2 = (Int_t)GenPhotonCollection2.size();
				GammaHisto[4]->Fill( nPhotons2, TotWeight );
				TLorentzVector SumPhotonMom2; SumPhotonMom2.SetPxPyPzE(0, 0, 0, 0);
				for(Int_t i_photon=0; i_photon<nPhotons2; i_photon++)
				{
					GenOthers photon = GenPhotonCollection2[i_photon];
					
					GammaHisto[0]->Fill(photon.Momentum.E(), TotWeight);
					GammaHisto[1]->Fill(photon.eta, TotWeight);
					GammaHisto[2]->Fill(photon.phi, TotWeight);

					Double_t dR = analyzer->Calc_dR_GenLepton_GenOthers( genlep_postFSR2, photon);
					GammaHisto[3]->Fill(dR, TotWeight);

					SumPhotonMom2 = SumPhotonMom2 + photon.Momentum;
				}
				h_GammaSumE->Fill( SumPhotonMom2.E(), TotWeight );

				// -- Mass Calculation -- //
				Double_t M_preFSR = ( genlep_preFSR1.Momentum + genlep_preFSR2.Momentum ).M();
				Double_t M_postFSR = ( genlep_postFSR1.Momentum + genlep_postFSR2.Momentum ).M();

				h_mass_preFSR->Fill( M_preFSR, TotWeight );
				h_mass_postFSR->Fill( M_postFSR, TotWeight );

				Double_t ratio = (M_preFSR - M_postFSR ) / M_preFSR;
				h_mass_ratio->Fill( ratio, TotWeight );

				h_mass_preFSR_tot->Fill( M_preFSR, TotWeight );
				h_mass_postFSR_tot->Fill( M_postFSR, TotWeight );
				h_mass_ratio_tot->Fill( ratio, TotWeight );

				UnfoldRes->Fill( M_postFSR, M_preFSR, TotWeight );

			} // -- End of if( GenFlag == kTRUE ) -- //

		} //End of event iteration

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;
		cout << "\tSum of Events of Separated Events: " << SumEvents_Separated << endl;

		f->cd();
		h_mass_preFSR->Write();
		h_mass_postFSR->Write();
		h_mass_ratio->Write();
		h_mass_LHE->Write();

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();
	h_mass_preFSR_tot->Write();
	h_mass_postFSR_tot->Write();
	h_mass_ratio_tot->Write();

	// for(Int_t i_dr=0; i_dr < ndRCuts; i_dr++)
	// {
	// 	h_nPhotons[i_dr]->Write();
	// 	h_RatioE[i_dr]->Write();
	// 	h_SumE[i_dr]->Write();
	// }

	Int_t nHisto = (Int_t)GammaHisto.size();
	for(Int_t i=0; i<nHisto; i++)
		GammaHisto[i]->Write();

	UnfoldRes->Write();

	// -- Response Matrix -- //
	TH2* h_RespM = UnfoldRes->Hresponse();
	h_RespM->SetName("h_RespM_RooUnfold");
	h_RespM->Write();
	TCanvas *c_RespM = new TCanvas("c_RespM", "", 800, 800);
	c_RespM->cd();
	h_RespM->SetStats(kFALSE);
	h_RespM->Draw("COLZ");
	h_RespM->SetMinimum(1e-3);
	h_RespM->SetMaximum(1);
	h_RespM->GetXaxis()->SetTitle("Invariant Mass (post-FSR)");
	h_RespM->GetYaxis()->SetTitle("Invariant Mass (pre-FSR)");
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetLogz();
	c_RespM->Write();

	TH1* h_Measured_RooUnfold = UnfoldRes->Hmeasured();
	h_Measured_RooUnfold->SetName("h_Measured_RooUnfold");
	h_Measured_RooUnfold->Write();

	TH1* h_Truth_RooUnfold = UnfoldRes->Htruth();
	h_Truth_RooUnfold->SetName("h_Truth_RooUnfold");
	h_Truth_RooUnfold->Write();

	GenLevelMassSpectrum( f );

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

void GenLevelMassSpectrum(TFile *f_output)
{
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;
	FillNtupleInformation_PHOTOS( ntupleDirectory, Tag, Xsec, nEvents );

	f_output->cd();
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
		
		if( Tag[i_tag].Contains("DYMuMu") ) // -- signal sample -- //
		{
			TH1D *h_temp = NULL;
			h_temp = (TH1D*)f_output->Get( "h_mass_LHE_"+Tag[i_tag] )->Clone();

			if( h_temp == NULL )
			{
				cout << "No Histograms corresponding to " << Tag[i_tag] << endl;
				return;
			}

			h_temp->Rebin(5);
			Double_t Norm = (Lumi * Xsec[i_tag]) / nEvents[i_tag];
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
	TLegend *legend = new TLegend(0.55, 0.63, 0.95, 0.95);
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
	h_format->SetStats(kFALSE);
	h_format->GetXaxis()->SetRangeUser(10, 4000);
	h_format->GetYaxis()->SetRangeUser(1e-4, 1e8);
	h_format->GetXaxis()->SetNoExponent();
	h_format->GetXaxis()->SetMoreLogLabels();
	h_format->GetXaxis()->SetTitle("M (#mu#mu) (LHE info) [GeV]");
	h_format->GetYaxis()->SetTitle("Entries/5 GeV");
	h_format->GetYaxis()->SetTitleOffset(1.4);

	h_format->Draw("");
	hs->Draw("histsame");
	h_format->Draw("axissame");
	legend->Draw();

	c_hs->SaveAs(CanvasName+".pdf");
}

void FillNtupleInformation_PHOTOS( vector<TString> &ntupleDirectory, vector<TString> &Tag, vector<Double_t> &Xsec, vector<Double_t> &nEvents )
{
	ntupleDirectory.push_back( "M10to50" ); Tag.push_back( "DYMuMu_Photos_M10to50" ); Xsec.push_back( 18610.0/3.0 ); nEvents.push_back( 2652513.0 ); 
	ntupleDirectory.push_back( "M50toInf_part1" ); Tag.push_back( "DYMuMu_Photos_M50to100" ); Xsec.push_back( 5869.58346/3.0 ); nEvents.push_back( 2183297.0 ); 
	ntupleDirectory.push_back( "M100to200" ); Tag.push_back( "DYMuMu_Photos_M100to200" ); Xsec.push_back( 226/3.0 ); nEvents.push_back( 186771.0 ); 
	ntupleDirectory.push_back( "M200to400" ); Tag.push_back( "DYMuMu_Photos_M200to400" ); Xsec.push_back( 7.67/3.0 ); nEvents.push_back( 35100.0 ); 
	ntupleDirectory.push_back( "M400to500" ); Tag.push_back( "DYMuMu_Photos_M400to500" ); Xsec.push_back( 0.423/3.0 ); nEvents.push_back( 36087.0 ); 
	ntupleDirectory.push_back( "M500to700" ); Tag.push_back( "DYMuMu_Photos_M500to700" ); Xsec.push_back( 0.24/3.0 ); nEvents.push_back( 28796.0 ); 
	ntupleDirectory.push_back( "M700to800" ); Tag.push_back( "DYMuMu_Photos_M700to800" ); Xsec.push_back( 0.035/3.0 ); nEvents.push_back( 38193.0 ); 
	ntupleDirectory.push_back( "M800to1000" ); Tag.push_back( "DYMuMu_Photos_M800to1000" ); Xsec.push_back( 0.03/3.0 ); nEvents.push_back( 20492.0 );
	ntupleDirectory.push_back( "M1000to1500" ); Tag.push_back( "DYMuMu_Photos_M1000to1500" ); Xsec.push_back( 0.016/3.0 ); nEvents.push_back( 32358.0 ); 
	ntupleDirectory.push_back( "M1500to2000" ); Tag.push_back( "DYMuMu_Photos_M1500to2000" ); Xsec.push_back( 0.002/3.0 ); nEvents.push_back( 25789.0 ); 
	ntupleDirectory.push_back( "M2000to3000" ); Tag.push_back( "DYMuMu_Photos_M2000to3000" ); Xsec.push_back( 0.00054/3.0 ); nEvents.push_back( 28922.0 );

	// -- apply the additional SF -- //
	Double_t SF = 1.033504;
	Int_t nTag = (Int_t)Tag.size();
	for(Int_t i_tag=0; i_tag<nTag; i_tag++)
	{
		if( Tag.at(i_tag).Contains("DYMuMu") && !Tag.at(i_tag).Contains("DYMuMu_Photos_M10to50") && !Tag.at(i_tag).Contains("DYMuMu_Photos_M50to100") )
		{
			Double_t xSec_before = Xsec.at(i_tag);

			Xsec.at(i_tag) = Xsec.at(i_tag) * SF;

			printf("[Cross seciton of %s] %lf -> %lf\n", Tag.at(i_tag).Data(), xSec_before, Xsec.at(i_tag) );
		}
	}
	cout << endl;
}


