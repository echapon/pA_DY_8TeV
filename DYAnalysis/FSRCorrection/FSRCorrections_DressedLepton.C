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
#include <Include/DYAnalyzer.h>

//TUnfold
#include "TUnfoldDensity.h"
// #define VERBOSE_LCURVE_SCAN

using namespace DYana;

static inline void loadBar(int x, int n, int r, int w);
// TH1F* unfold_MLE(TH1F *hin, TH2F *hresponse, TH2F *hcov);
// TH1F* fold_MLE(TH1F *hin, TH1F *hresponse);
void FSRCorrections_DressedLepton( TString Sample = "Powheg", TString HLTname = "PAL3Mu12" )
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

   // const int binnum = 13;
   // double bins[14] = {15,20,30,40,50,60,76,86,96,106,120,150,200,600};
   // let's define a finer binning for gen
   const int rebin = 2;
   const int binnum_rebin = rebin*binnum;
   double *bins_rebin = new double[binnum_rebin+1];
   for (int i=0; i<binnum_rebin+1; i++) {
      int i_orig = i/rebin;
      double dm = (bins[i_orig+1]-bins[i_orig])/((double) rebin);
      bins_rebin[i] = bins[i_orig]+(i%rebin)*dm;
   }

	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";

	// -- Each ntuple directory & corresponding Tags -- //
	// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< SampleTag > STags;

   analyzer->SetupMCsamples_v20170830(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

	TFile *f = new TFile("ROOTFile_FSRCorrections_DressedLepton_" + Sample + ".root", "RECREATE");

	TH1D *h_mass_preFSR_tot = new TH1D("h_mass_preFSR", "", binnum, bins);
	TH1D *h_mass_preFSR_tot_fine = new TH1D("h_mass_preFSR_fine", "", 585,15,600);
	TH1D *h_mass_postFSR_tot = new TH1D("h_mass_postFSR", "", binnum, bins);
	TH1D *h_mass_postFSR_tot_fine = new TH1D("h_mass_postFSR_fine", "", 585,15,600);
	TH1D *h_mass_ratio_tot = new TH1D("h_mass_ratio", "", 100, -1, 1);
   TH2D *h_mass_postpreFSR_tot = new TH2D("h_mass_postpreFSR_tot", ";mass(post-FSR);mass(pre-FSR)", binnum, bins, binnum, bins);
   TH2D *h_mass_postpreFSR_tot_fine = new TH2D("h_mass_postpreFSR_tot_fine", ";mass(post-FSR);mass(pre-FSR)", 585,15,600,585,15,600);

	TH1D *h_pt_preFSR_tot = new TH1D("h_pt_preFSR", "", ptbinnum_meas, ptbin_meas);
	TH1D *h_pt_preFSR_tot_fine = new TH1D("h_pt_preFSR_fine", "", 200,0,200);
	TH1D *h_pt_postFSR_tot = new TH1D("h_pt_postFSR", "", ptbinnum_meas, ptbin_meas);
	TH1D *h_pt_postFSR_tot_fine = new TH1D("h_pt_postFSR_fine", "", 200,0,200);
	TH1D *h_pt_ratio_tot = new TH1D("h_pt_ratio", "", 100, -1, 1);
   TH2D *h_pt_postpreFSR_tot = new TH2D("h_pt_postpreFSR_tot", ";pt(post-FSR);pt(pre-FSR)", ptbinnum_meas, ptbin_meas, ptbinnum_meas, ptbin_meas);
   TH2D *h_pt_postpreFSR_tot_fine = new TH2D("h_pt_postpreFSR_tot_fine", ";pt(post-FSR);pt(pre-FSR)", 200,0,200,200,0,200);

	TH1D *h_rap1560_preFSR_tot = new TH1D("h_rap1560_preFSR", "", rapbinnum_1560, rapbin_1560);
	TH1D *h_rap1560_preFSR_tot_fine = new TH1D("h_rap1560_preFSR_fine", "", 500,-3,2);
	TH1D *h_rap1560_postFSR_tot = new TH1D("h_rap1560_postFSR", "", rapbinnum_1560, rapbin_1560);
	TH1D *h_rap1560_postFSR_tot_fine = new TH1D("h_rap1560_postFSR_fine", "", 500,-3,2);
	TH1D *h_rap1560_ratio_tot = new TH1D("h_rap1560_ratio", "", 100, -1, 1);
   TH2D *h_rap1560_postpreFSR_tot = new TH2D("h_rap1560_postpreFSR_tot", ";rap1560(post-FSR);rap1560(pre-FSR)", rapbinnum_1560, rapbin_1560, rapbinnum_1560, rapbin_1560);
   TH2D *h_rap1560_postpreFSR_tot_fine = new TH2D("h_rap1560_postpreFSR_tot_fine", ";rap1560(post-FSR);rap1560(pre-FSR)", 500,-3,2,500,-3,2);

	TH1D *h_rap60120_preFSR_tot = new TH1D("h_rap60120_preFSR", "", rapbinnum_60120, rapbin_60120);
	TH1D *h_rap60120_preFSR_tot_fine = new TH1D("h_rap60120_preFSR_fine", "", 500,-3,2);
	TH1D *h_rap60120_postFSR_tot = new TH1D("h_rap60120_postFSR", "", rapbinnum_60120, rapbin_60120);
	TH1D *h_rap60120_postFSR_tot_fine = new TH1D("h_rap60120_postFSR_fine", "", 500,-3,2);
	TH1D *h_rap60120_ratio_tot = new TH1D("h_rap60120_ratio", "", 100, -1, 1);
   TH2D *h_rap60120_postpreFSR_tot = new TH2D("h_rap60120_postpreFSR_tot", ";rap60120(post-FSR);rap60120(pre-FSR)", rapbinnum_60120, rapbin_60120, rapbinnum_60120, rapbin_60120);
   TH2D *h_rap60120_postpreFSR_tot_fine = new TH2D("h_rap60120_postpreFSR_tot_fine", ";rap60120(post-FSR);rap60120(pre-FSR)", 500,-3,2,500,-3,2);

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
      // loop only on DYMuMu!
      if (!Tag[i_tup].Contains("DYMuMu")) continue;

		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TH1D *h_mass_preFSR = new TH1D("h_mass_preFSR_"+Tag[i_tup], "", binnum, bins);
		TH1D *h_mass_postFSR = new TH1D("h_mass_postFSR_"+Tag[i_tup], "", binnum, bins);
		TH1D *h_mass_ratio = new TH1D("h_mass_ratio_"+Tag[i_tup], "", 100, -1, 1);

		// -- ntuple Setting -- //
		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
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
		cout << "\t[Total Events: " << NEvents << "]" << endl; 
		if( Sample == "Powheg" )
			nEvents.push_back( NEvents );

		Double_t norm = ( Xsec[i_tup] * lumi_all ) / (Double_t)nEvents[i_tup];
		cout << "\t[Normalization factor: " << norm << "]" << endl;

		// -- Event loop starts -- //
      // NEvents = 100000;
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

			Int_t PU = ntuple->nPileUp;
			// Double_t PUWeight = analyzer->PileUpWeightValue( PU );

			// Double_t TotWeight = norm * GenWeight * PUWeight;
			Double_t TotWeight = norm * GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
				SumWeights_Separated += GenWeight;
				SumEvents_Separated++;

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

            // acceptance flags
				bool Flag_PassAcc_postFSR = analyzer->isPassAccCondition_GenLepton(genlep_postFSR1, genlep_postFSR2);
				bool Flag_PassAcc_preFSR = analyzer->isPassAccCondition_GenLepton(genlep_preFSR1, genlep_preFSR2);

            // if neither of the pre- and post-FSR leptons pass acceptance cuts... why bother?
            // if (!Flag_PassAcc_preFSR && !Flag_PassAcc_postFSR) continue;
            if (!Flag_PassAcc_preFSR) continue;

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

				// -- Mass, Pt, Rapidity Calculation -- //
            TLorentzVector tlv_preFSR = genlep_preFSR1.Momentum + genlep_preFSR2.Momentum;
            TLorentzVector tlv_postFSR = genlep_postFSR1.Momentum + genlep_postFSR2.Momentum;
				Double_t M_preFSR = Flag_PassAcc_preFSR ? tlv_preFSR.M() : -99;
				Double_t M_postFSR = Flag_PassAcc_postFSR ? tlv_postFSR.M() : -99;
				Double_t pt_preFSR = Flag_PassAcc_preFSR ? tlv_preFSR.Pt() : -99;
				Double_t pt_postFSR = Flag_PassAcc_postFSR ? tlv_postFSR.Pt() : -99;
				Double_t rap_preFSR = Flag_PassAcc_preFSR ? tlv_preFSR.Rapidity() - rapshift : -99;
				Double_t rap_postFSR = Flag_PassAcc_postFSR ? tlv_postFSR.Rapidity() - rapshift : -99;

				h_mass_preFSR->Fill( M_preFSR, TotWeight );
				h_mass_postFSR->Fill( M_postFSR, TotWeight );

				Double_t mass_ratio = (M_preFSR - M_postFSR ) / M_preFSR;
				Double_t pt_ratio = (pt_preFSR - pt_postFSR ) / pt_preFSR;
				Double_t rap_ratio = (rap_preFSR - rap_postFSR ) / rap_preFSR;
				h_mass_ratio->Fill( mass_ratio, TotWeight );

            // mass histos
				h_mass_preFSR_tot->Fill( M_preFSR, TotWeight );
				h_mass_postFSR_tot->Fill( M_postFSR, TotWeight );
				h_mass_ratio_tot->Fill( mass_ratio, TotWeight );
				h_mass_postpreFSR_tot->Fill( M_postFSR, M_preFSR, TotWeight );

				h_mass_preFSR_tot_fine->Fill( M_preFSR, TotWeight );
				h_mass_postFSR_tot_fine->Fill( M_postFSR, TotWeight );
				h_mass_postpreFSR_tot_fine->Fill( M_postFSR, M_preFSR, TotWeight );

            // pt histos
				if (M_preFSR>60 && M_preFSR<120) h_pt_preFSR_tot->Fill( pt_preFSR, TotWeight );
				if (M_postFSR>60 && M_postFSR<120) h_pt_postFSR_tot->Fill( pt_postFSR, TotWeight );
				if (M_preFSR>60 && M_preFSR<120) h_pt_ratio_tot->Fill( pt_ratio, TotWeight );
				h_pt_postpreFSR_tot->Fill( (M_postFSR>60 && M_postFSR<120) ? pt_postFSR : -99, (M_preFSR>60 && M_preFSR<120) ? pt_preFSR : -99, TotWeight );

				if (M_preFSR>60 && M_preFSR<120) h_pt_preFSR_tot_fine->Fill( pt_preFSR, TotWeight );
				if (M_postFSR>60 && M_postFSR<120) h_pt_postFSR_tot_fine->Fill( pt_postFSR, TotWeight );
				h_pt_postpreFSR_tot_fine->Fill( (M_postFSR>60 && M_postFSR<120) ? pt_postFSR : -99, (M_preFSR>60 && M_preFSR<120) ? pt_preFSR : -99, TotWeight );

            // rap 15-60 histos
				if (M_preFSR>15 && M_preFSR<60) h_rap1560_preFSR_tot->Fill( rap_preFSR, TotWeight );
				if (M_postFSR>15 && M_postFSR<60) h_rap1560_postFSR_tot->Fill( rap_postFSR, TotWeight );
				if (M_preFSR>15 && M_preFSR<60) h_rap1560_ratio_tot->Fill( rap_ratio, TotWeight );
				h_rap1560_postpreFSR_tot->Fill( (M_postFSR>15 && M_postFSR<60) ? rap_postFSR : -99, (M_preFSR>15 && M_preFSR<60) ? rap_preFSR : -99, TotWeight );

				if (M_preFSR>15 && M_preFSR<60) h_rap1560_preFSR_tot_fine->Fill( rap_preFSR, TotWeight );
				if (M_postFSR>15 && M_postFSR<60) h_rap1560_postFSR_tot_fine->Fill( rap_postFSR, TotWeight );
				h_rap1560_postpreFSR_tot_fine->Fill( (M_postFSR>15 && M_postFSR<60) ? rap_postFSR : -99, (M_preFSR>15 && M_preFSR<60) ? rap_preFSR : -99, TotWeight );

            // rap 60-9920 histos
				if (M_preFSR>60 && M_preFSR<120) h_rap60120_preFSR_tot->Fill( rap_preFSR, TotWeight );
				if (M_postFSR>60 && M_postFSR<120) h_rap60120_postFSR_tot->Fill( rap_postFSR, TotWeight );
				if (M_preFSR>60 && M_preFSR<120) h_rap60120_ratio_tot->Fill( rap_ratio, TotWeight );
				h_rap60120_postpreFSR_tot->Fill( (M_postFSR>60 && M_postFSR<120) ? rap_postFSR : -99, (M_preFSR>60 && M_preFSR<120) ? rap_preFSR : -99, TotWeight );

				if (M_preFSR>60 && M_preFSR<120) h_rap60120_preFSR_tot_fine->Fill( rap_preFSR, TotWeight );
				if (M_postFSR>60 && M_postFSR<120) h_rap60120_postFSR_tot_fine->Fill( rap_postFSR, TotWeight );
				h_rap60120_postpreFSR_tot_fine->Fill( (M_postFSR>60 && M_postFSR<120) ? rap_postFSR : -99, (M_preFSR>60 && M_preFSR<120) ? rap_preFSR : -99, TotWeight );
			} // -- End of if( GenFlag == kTRUE ) -- //

		} //End of event iteration

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;
		cout << "\tSum of Events of Separated Events: " << SumEvents_Separated << endl;

		f->cd();
		h_mass_preFSR->Write();
		h_mass_postFSR->Write();
		h_mass_ratio->Write();

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();
	h_mass_preFSR_tot->Write();
	h_mass_postFSR_tot->Write();
   h_mass_ratio_tot->Write();
   h_mass_postpreFSR_tot->Write();
	h_mass_preFSR_tot_fine->Write();
	h_mass_postFSR_tot_fine->Write();
   h_mass_postpreFSR_tot_fine->Write();

	h_pt_preFSR_tot->Write();
	h_pt_postFSR_tot->Write();
   h_pt_ratio_tot->Write();
   h_pt_postpreFSR_tot->Write();
	h_pt_preFSR_tot_fine->Write();
	h_pt_postFSR_tot_fine->Write();
   h_pt_postpreFSR_tot_fine->Write();

	h_rap1560_preFSR_tot->Write();
	h_rap1560_postFSR_tot->Write();
   h_rap1560_ratio_tot->Write();
   h_rap1560_postpreFSR_tot->Write();
	h_rap1560_preFSR_tot_fine->Write();
	h_rap1560_postFSR_tot_fine->Write();
   h_rap1560_postpreFSR_tot_fine->Write();

	h_rap60120_preFSR_tot->Write();
	h_rap60120_postFSR_tot->Write();
   h_rap60120_ratio_tot->Write();
   h_rap60120_postpreFSR_tot->Write();
	h_rap60120_preFSR_tot_fine->Write();
	h_rap60120_postFSR_tot_fine->Write();
   h_rap60120_postpreFSR_tot_fine->Write();

   // compute the condition numbers
   TMatrixD m_mass(binnum,binnum);
   for (int i=0; i<binnum; i++)
      for (int j=0; j<binnum; j++)
         m_mass[i][j] = h_mass_postpreFSR_tot->GetBinContent(i+1,j+1);
   TDecompSVD tsvd_mass(m_mass);
   tsvd_mass.Decompose();
   TVectorD sv_mass = tsvd_mass.GetSig();
   double condnum_mass = sv_mass[0]/max(0.,sv_mass[binnum-1]);
   cout << "Condition number for mass: " << condnum_mass << endl;

   TMatrixD m_pt(ptbinnum_meas,ptbinnum_meas);
   for (int i=0; i<ptbinnum_meas; i++)
      for (int j=0; j<ptbinnum_meas; j++)
         m_pt[i][j] = h_pt_postpreFSR_tot->GetBinContent(i+1,j+1);
   TDecompSVD tsvd_pt(m_pt);
   tsvd_pt.Decompose();
   TVectorD sv_pt = tsvd_pt.GetSig();
   double condnum_pt = sv_pt[0]/max(0.,sv_pt[ptbinnum_meas-1]);
   cout << "Condition number for pt: " << condnum_pt << endl;

   TMatrixD m_rap1560(rapbinnum_1560,rapbinnum_1560);
   for (int i=0; i<rapbinnum_1560; i++)
      for (int j=0; j<rapbinnum_1560; j++)
         m_rap1560[i][j] = h_rap1560_postpreFSR_tot->GetBinContent(i+1,j+1);
   TDecompSVD tsvd_rap1560(m_rap1560);
   tsvd_rap1560.Decompose();
   TVectorD sv_rap1560 = tsvd_rap1560.GetSig();
   double condnum_rap1560 = sv_rap1560[0]/max(0.,sv_rap1560[rapbinnum_1560-1]);
   cout << "Condition number for rap1560: " << condnum_rap1560 << endl;

   TMatrixD m_rap60120(rapbinnum_60120,rapbinnum_60120);
   for (int i=0; i<rapbinnum_60120; i++)
      for (int j=0; j<rapbinnum_60120; j++)
         m_rap60120[i][j] = h_rap60120_postpreFSR_tot->GetBinContent(i+1,j+1);
   TDecompSVD tsvd_rap60120(m_rap60120);
   tsvd_rap60120.Decompose();
   TVectorD sv_rap60120 = tsvd_rap60120.GetSig();
   double condnum_rap60120 = sv_rap60120[0]/max(0.,sv_rap60120[rapbinnum_60120-1]);
   cout << "Condition number for rap60120: " << condnum_rap60120 << endl;

   // f->Close();
   // return;

   TUnfoldDensity unfold(h_mass_postpreFSR_tot,TUnfold::kHistMapOutputVert);

   // unfold.SetName("unfold");

   // define input and bias scame
   // do not use the bias, because MC peak may be at the wrong place
   // watch out for error codes returned by the SetInput method
   // errors larger or equal 10000 are fatal:
   // the data points specified as input are not sufficient to constrain the
   // unfolding process
   TFile *fdata = TFile::Open("ROOTFile_YieldHistogram.root");
   if (!fdata || !fdata->IsOpen()) {
      cout << "Error, file ROOTFile_YieldHistogram.root not found" << endl;
      return;
   }
   TH1F *histMdetData = (TH1F*) fdata->Get("h_yield_OS_MCBasedBkg1");
   if (!histMdetData) {
      cout << "Error, histo h_yield_OS_MCBasedBkg1 not found" << endl;
      return;
   }
   f->cd();

   if(unfold.SetInput(histMdetData)>=10000) {
      std::cout<<"Unfolding result may be wrong\n";
   }

   //========================================================================
   // the unfolding is done here
   //
   // scan L curve and find best point
   Int_t nScan=30;
   // use automatic L-curve scan: start with taumin=taumax=0.0
   Double_t tauMin=0.;
   Double_t tauMax=0.;
   Int_t iBest;
   TSpline *logTauX,*logTauY;
   TGraph *lCurve;

   // if required, report Info messages (for debugging the L-curve scan)
#ifdef VERBOSE_LCURVE_SCAN
   Int_t oldinfo=gErrorIgnoreLevel;
   gErrorIgnoreLevel=kInfo;
#endif
   // this method scans the parameter tau and finds the kink in the L curve
   // finally, the unfolding is done for the best choice of tau
   iBest=unfold.ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);

   // if required, switch to previous log-level
#ifdef VERBOSE_LCURVE_SCAN
   gErrorIgnoreLevel=oldinfo;
#endif

   //==========================================================================
   // create graphs with one point to visualize the best choice of tau
   //
   Double_t t[1],x[1],y[1];
   logTauX->GetKnot(iBest,t[0],x[0]);
   logTauY->GetKnot(iBest,t[0],y[0]);
   TGraph *bestLcurve=new TGraph(1,x,y);
   TGraph *bestLogTauLogChi2=new TGraph(1,t,x);

   // save the tau vs chi2
   logTauX->Write("logTauX");
   bestLogTauLogChi2->Write("bestLogTauLogChi2");
   // save the L curve
   lCurve->Write("lCurve");
   bestLcurve->Write("bestLcurve");

   // for(Int_t i_dr=0; i_dr < ndRCuts; i_dr++)
   // {
   // 	h_nPhotons[i_dr]->Write();
   // 	h_RatioE[i_dr]->Write();
   // 	h_SumE[i_dr]->Write();
   // }

   Int_t nHisto = (Int_t)GammaHisto.size();
   for(Int_t i=0; i<nHisto; i++)
      GammaHisto[i]->Write();

   unfold.Write();

	// -- Response Matrix -- //
	TCanvas *c_RespM = new TCanvas("c_RespM", "", 800, 800);
	c_RespM->cd();
	h_mass_postpreFSR_tot->SetStats(kFALSE);
	h_mass_postpreFSR_tot->Draw("COLZ");
	h_mass_postpreFSR_tot->SetMinimum(1e-3);
	h_mass_postpreFSR_tot->SetMaximum(1);
	h_mass_postpreFSR_tot->GetXaxis()->SetTitle("Invariant Mass (post-FSR)");
	h_mass_postpreFSR_tot->GetYaxis()->SetTitle("Invariant Mass (pre-FSR)");
	gPad->SetLogx();
	gPad->SetLogy();
	gPad->SetLogz();
	c_RespM->Write();

   TH1* h_Measured_TUnfold = unfold.GetInput("h_Measured_TUnfold");
   h_Measured_TUnfold->SetName("h_Measured_TUnfold");
   // DIRTY FIX
   for (int i=h_Measured_TUnfold->GetNbinsX(); i>0; i--) {
      h_Measured_TUnfold->SetBinContent(i,h_Measured_TUnfold->GetBinContent(i-1));
      h_Measured_TUnfold->SetBinError(i,h_Measured_TUnfold->GetBinError(i-1));
   }
   h_Measured_TUnfold->Write();

   TH1* h_Truth_TUnfold = unfold.GetOutput("h_Truth_TUnfold");
   h_Truth_TUnfold->SetName("h_Truth_TUnfold");
   h_Truth_TUnfold->Write();

   //==========================================================================
   // retreive results into histograms

   // get unfolded distribution
   TH1 *histMunfold=unfold.GetOutput("Unfolded");
   histMunfold->Write();

   // get unfolding result, folded back
   TH1 *histMdetFold=unfold.GetFoldedOutput("FoldedBack");
   histMdetFold->Write();

   // get error matrix (input distribution [stat] errors only)
   // TH2D *histEmatData=unfold.GetEmatrix("EmatData");

   // get total error matrix:
   //   migration matrix uncorrelated and correlated systematic errors
   //   added in quadrature to the data statistical errors
   TH2 *histEmatTotal=unfold.GetEmatrixTotal("EmatTotal");

   // create data histogram with the total errors
   TH1D *histTotalError=
      new TH1D("TotalError",";mass(gen)",binnum, bins);
   for(Int_t bin=1;bin<=binnum;bin++) {
      histTotalError->SetBinContent(bin,histMunfold->GetBinContent(bin));
      histTotalError->SetBinError
         (bin,TMath::Sqrt(histEmatTotal->GetBinContent(bin,bin)));
   }

   // get global correlation coefficients
   // for this calculation one has to specify whether the
   // underflow/overflow bins are included or not
   // default: include all bins
   // here: exclude underflow and overflow bins
   TH2 *gHistInvEMatrix;
   TH1 *histRhoi=unfold.GetRhoItotal("rho_I",
         0, // use default title
         0, // all distributions
         "*[UO]", // discard underflow and overflow bins on all axes
         kTRUE, // use original binning
         &gHistInvEMatrix // store inverse of error matrix
         );
   histRhoi->Write();

	// h_dR_E->Write();

	// for(Int_t i_status=0; i_status<nStatus; i_status++)
	// 	h_dR_Status[i_status]->Write();

	// for(Int_t i_mother=0; i_mother<nMother; i_mother++)
	// 	h_dR_Mother[i_mother]->Write();

	// h_Mother_Status->Write();

	// TCanvas *c_dR_E = new TCanvas("c_dR_E", "", 600, 600);
	// c_dR_E->cd();
	// // h_dR_E->SetStats(kFALSE);
	// h_dR_E->Draw("COLZ");
	// // h_dR_E->SetMinimum(1e-3);
	// // h_dR_E->SetMaximum(1);
	// // gPad->SetLogx();
	// // gPad->SetLogy();
	// gPad->SetLogz();
	// c_dR_E->Write();

	Double_t TotalRunTime = totaltime.CpuTime();
	cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

	TTimeStamp ts_end;
	cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;

   fdata->Close();
   f->Close();
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

// TH1F* unfold_MLE(TH1F *hin, TH2F *hresponse, TH2F *hcov) {
//    TVectorD vin(hin->GetNbinsX());
//    TVectorD vout(hin->GetNbinsX());
//    TMatrixD mr(hresponse->GetNbinsX(),hreponse->GetNbinsY());
//    TMatrixD mrinv = mr.Invert();

// }

// TH1F* fold_MLE(TH1F *hin, TH1F *hresponse) {
// }
