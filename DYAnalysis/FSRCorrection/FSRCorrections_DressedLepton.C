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
#include "../Include/DYAnalyzer.h"
#include "../Include/UnfoldUtils.h"

using namespace DYana;
using unfold::gUnfold;

static inline void loadBar(int x, int n, int r, int w);
// TH1F* unfold_MLE(TH1F *hin, TH2F *hresponse, TH2F *hcov);
// TH1F* fold_MLE(TH1F *hin, TH1F *hresponse);
void FSRCorrections_DressedLepton( TString Sample = "Powheg", TString HLTname = "PAL3Mu12", int run=0, bool correctforacc = true ) // run: 0=all, 1=pPb, 2=PbP
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

   analyzer->SetupMCsamples_v20180814(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

   TString accstr = correctforacc ? "" : "_noacc";

	TFile *f = new TFile("FSRCorrection/ROOTFile_FSRCorrections_DressedLepton_" + Sample + "_" + Form("%d",run) + accstr + ".root", "RECREATE");

	TH1D *h_mass_preFSR_tot = new TH1D("h_mass_preFSR", "", binnum, bins);
	TH1D *h_mass_preFSR_tot_fine = new TH1D("h_mass_preFSR_fine", "", 585,15,600);
	TH1D *h_mass_postFSR_tot = new TH1D("h_mass_postFSR", "", binnum, bins);
	TH1D *h_mass_postFSR_tot_fine = new TH1D("h_mass_postFSR_fine", "", 585,15,600);
	TH1D *h_mass_bare_tot_fine = new TH1D("h_mass_bare_fine", "", 585,15,600);
	TH1D *h_mass_ratio_tot = new TH1D("h_mass_ratio", "", 100, -1, 1);
   TH2D *h_mass_postpreFSR_tot = new TH2D("h_mass_postpreFSR_tot", ";mass(post-FSR);mass(pre-FSR)", binnum, bins, binnum, bins);
   TH2D *h_mass_postpreFSR_tot_fine = new TH2D("h_mass_postpreFSR_tot_fine", ";mass(post-FSR);mass(pre-FSR)", 585,15,600,585,15,600);

	TH1D *h_mass3bins_preFSR_tot = new TH1D("h_mass3bins_preFSR", "", binnum3, bins3);
	TH1D *h_mass3bins_preFSR_tot_fine = new TH1D("h_mass3bins_preFSR_fine", "", 585,15,600);
	TH1D *h_mass3bins_postFSR_tot = new TH1D("h_mass3bins_postFSR", "", binnum3, bins3);
	TH1D *h_mass3bins_postFSR_tot_fine = new TH1D("h_mass3bins_postFSR_fine", "", 585,15,600);
	TH1D *h_mass3bins_bare_tot_fine = new TH1D("h_mass3bins_bare_fine", "", 585,15,600);
	TH1D *h_mass3bins_ratio_tot = new TH1D("h_mass3bins_ratio", "", 100, -1, 1);
   TH2D *h_mass3bins_postpreFSR_tot = new TH2D("h_mass3bins_postpreFSR_tot", ";mass3bins(post-FSR);mass3bins(pre-FSR)", binnum3, bins3, binnum3, bins3);
   TH2D *h_mass3bins_postpreFSR_tot_fine = new TH2D("h_mass3bins_postpreFSR_tot_fine", ";mass3bins(post-FSR);mass3bins(pre-FSR)", 585,15,600,585,15,600);

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

	TH1D *h_phistar_preFSR_tot = new TH1D("h_phistar_preFSR", "", phistarnum, phistarbin);
	TH1D *h_phistar_preFSR_tot_fine = new TH1D("h_phistar_preFSR_fine", "", 500,-3,2);
	TH1D *h_phistar_postFSR_tot = new TH1D("h_phistar_postFSR", "", phistarnum, phistarbin);
	TH1D *h_phistar_postFSR_tot_fine = new TH1D("h_phistar_postFSR_fine", "", 500,-3,2);
	TH1D *h_phistar_ratio_tot = new TH1D("h_phistar_ratio", "", 100, -1, 1);
   TH2D *h_phistar_postpreFSR_tot = new TH2D("h_phistar_postpreFSR_tot", ";phistar(post-FSR);phistar(pre-FSR)", phistarnum, phistarbin, phistarnum, phistarbin);
   TH2D *h_phistar_postpreFSR_tot_fine = new TH2D("h_phistar_postpreFSR_tot_fine", ";phistar(post-FSR);phistar(pre-FSR)", 500,-3,2,500,-3,2);

	TH1D *h_pt1560_preFSR_tot = new TH1D("h_pt1560_preFSR", "", ptbinnum_meas_1560, ptbin_meas_1560);
	TH1D *h_pt1560_preFSR_tot_fine = new TH1D("h_pt1560_preFSR_fine", "", 200,0,200);
	TH1D *h_pt1560_postFSR_tot = new TH1D("h_pt1560_postFSR", "", ptbinnum_meas_1560, ptbin_meas_1560);
	TH1D *h_pt1560_postFSR_tot_fine = new TH1D("h_pt1560_postFSR_fine", "", 200,0,200);
	TH1D *h_pt1560_ratio_tot = new TH1D("h_pt1560_ratio", "", 100, -1, 1);
   TH2D *h_pt1560_postpreFSR_tot = new TH2D("h_pt1560_postpreFSR_tot", ";pt1560(post-FSR);pt1560(pre-FSR)", ptbinnum_meas_1560, ptbin_meas_1560, ptbinnum_meas_1560, ptbin_meas_1560);
   TH2D *h_pt1560_postpreFSR_tot_fine = new TH2D("h_pt1560_postpreFSR_tot_fine", ";pt1560(post-FSR);pt1560(pre-FSR)", 200,0,200,200,0,200);

	TH1D *h_phistar1560_preFSR_tot = new TH1D("h_phistar1560_preFSR", "", phistarnum_1560, phistarbin_1560);
	TH1D *h_phistar1560_preFSR_tot_fine = new TH1D("h_phistar1560_preFSR_fine", "", 500,-3,2);
	TH1D *h_phistar1560_postFSR_tot = new TH1D("h_phistar1560_postFSR", "", phistarnum_1560, phistarbin_1560);
	TH1D *h_phistar1560_postFSR_tot_fine = new TH1D("h_phistar1560_postFSR_fine", "", 500,-3,2);
	TH1D *h_phistar1560_ratio_tot = new TH1D("h_phistar1560_ratio", "", 100, -1, 1);
   TH2D *h_phistar1560_postpreFSR_tot = new TH2D("h_phistar1560_postpreFSR_tot", ";phistar1560(post-FSR);phistar1560(pre-FSR)", phistarnum_1560, phistarbin_1560, phistarnum_1560, phistarbin_1560);
   TH2D *h_phistar1560_postpreFSR_tot_fine = new TH2D("h_phistar1560_postpreFSR_tot_fine", ";phistar1560(post-FSR);phistar1560(pre-FSR)", 500,-3,2,500,-3,2);

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

      Bool_t doflip = (switcheta(STags[i_tup])<0);
      Int_t  flipsign = doflip ? -1 : 1;
      analyzer->sign = flipsign;
      if (run==1 && doflip) continue;
      if (run==2 && !doflip) continue;

		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TH1D *h_mass_preFSR = new TH1D("h_mass_preFSR_"+Tag[i_tup], "", binnum, bins);
		TH1D *h_mass_postFSR = new TH1D("h_mass_postFSR_"+Tag[i_tup], "", binnum, bins);
		TH1D *h_mass_ratio = new TH1D("h_mass_ratio_"+Tag[i_tup], "", 100, -1, 1);

		// -- ntuple Setting -- //
		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain, doflip );
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_GenOthers();

		// analyzer->SetupPileUpReWeighting( Tag[i_tup] );

		Bool_t isNLO = 0;
		if( Sample=="Powheg" && (Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets") )
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

		Double_t norm = ( Xsec[i_tup] * lumi_part2 ) / (Double_t)nEvents[i_tup];
      if (doflip)
         norm = ( Xsec[i_tup] * lumi_part1 ) / (Double_t)nEvents[i_tup];
      if (! Sample.Contains("Powheg")) // only Powheg has the 2 beam directions
         norm = ( Xsec[i_tup] * lumi_all ) / (Double_t)nEvents[i_tup];

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

				// -- Collect ``bare'' gen-level information -- //
				vector<GenLepton> GenLeptonCollection;
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( genlep.isMuon() && genlep.isHardProcess )
						GenLeptonCollection.push_back( genlep );

						if( GenLeptonCollection.size() ==  2 )
							break;
				}
				GenLepton genlep1 = GenLeptonCollection[0];
				GenLepton genlep2 = GenLeptonCollection[1];
				Double_t gen_M = (genlep1.Momentum + genlep2.Momentum).M();
				Double_t gen_Pt = (genlep1.Momentum + genlep2.Momentum).Pt();

				Double_t dRCut = 0.1;

				GenLepton genlep_postFSR1 = GenLeptonCollection_FinalState[0];
				GenLepton genlep_preFSR1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
				vector< GenOthers > GenPhotonCollection1;
				analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_preFSR1, &GenPhotonCollection1);

				GenLepton genlep_postFSR2 = GenLeptonCollection_FinalState[1];
				GenLepton genlep_preFSR2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
				vector< GenOthers > GenPhotonCollection2;
				analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_preFSR2, &GenPhotonCollection2);

				// -- Mass, Pt, Rapidity Calculation -- //
            TLorentzVector tlv_preFSR = genlep_preFSR1.Momentum + genlep_preFSR2.Momentum;
            TLorentzVector tlv_postFSR = genlep_postFSR1.Momentum + genlep_postFSR2.Momentum;
				Double_t M_preFSR = tlv_preFSR.M();
				Double_t M_postFSR = tlv_postFSR.M();
				Double_t pt_preFSR = tlv_preFSR.Pt();
				Double_t pt_postFSR = tlv_postFSR.Pt();
				Double_t rap_preFSR = tlv_preFSR.Rapidity() - rapshift;
				Double_t rap_postFSR = tlv_postFSR.Rapidity() - rapshift;
				Double_t phistar_preFSR = Object::phistar(genlep_preFSR1, genlep_preFSR2);
				Double_t phistar_postFSR = Object::phistar(genlep_postFSR1, genlep_postFSR2);

            // acceptance flags
				bool Flag_PassAcc_postFSR = kFALSE;
            if (correctforacc) {
               Flag_PassAcc_postFSR = (rap_postFSR > rapbin_60120[0] && rap_postFSR<rapbin_60120[rapbinnum_60120] );
            } else {
               Flag_PassAcc_postFSR = analyzer->isPassAccCondition_GenLepton(genlep_postFSR1, genlep_postFSR2);
            }
				bool Flag_PassAcc_preFSR = kFALSE;
            if (correctforacc) {
               Flag_PassAcc_preFSR = (rap_preFSR > rapbin_60120[0] && rap_preFSR<rapbin_60120[rapbinnum_60120] );
            } else {
               Flag_PassAcc_preFSR = analyzer->isPassAccCondition_GenLepton(genlep_preFSR1, genlep_preFSR2);
            }

            // if neither of the pre- and post-FSR leptons pass acceptance cuts... why bother?
            // if (!Flag_PassAcc_preFSR && !Flag_PassAcc_postFSR) continue;
            if (!Flag_PassAcc_preFSR) continue;

            // -- Z pt reweighting -- //
            TotWeight *= zptWeight(gen_Pt);

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

				h_mass_preFSR->Fill( M_preFSR, TotWeight );
				h_mass_postFSR->Fill( M_postFSR, TotWeight );

				Double_t mass_ratio = (M_preFSR - M_postFSR ) / M_preFSR;
				Double_t pt_ratio = (pt_preFSR - pt_postFSR ) / pt_preFSR;
				Double_t rap_ratio = (rap_preFSR - rap_postFSR ) / rap_preFSR;
				Double_t phistar_ratio = (phistar_preFSR - phistar_postFSR ) / phistar_preFSR;
				h_mass_ratio->Fill( mass_ratio, TotWeight );

            if( Flag_PassAcc_preFSR == kTRUE )
            {
               // fill pre-FSR histos
               h_mass_preFSR_tot->Fill( M_preFSR, TotWeight );
               h_mass_preFSR_tot_fine->Fill( M_preFSR, TotWeight );
               h_mass3bins_preFSR_tot->Fill( M_preFSR, TotWeight );
               h_mass3bins_preFSR_tot_fine->Fill( M_preFSR, TotWeight );
               if (M_preFSR>60 && M_preFSR<120) {
                  h_pt_preFSR_tot->Fill( pt_preFSR, TotWeight );
                  h_pt_preFSR_tot_fine->Fill( pt_preFSR, TotWeight );
                  h_phistar_preFSR_tot->Fill( phistar_preFSR, TotWeight );
                  h_phistar_preFSR_tot_fine->Fill( phistar_preFSR, TotWeight );
                  h_rap60120_preFSR_tot->Fill( rap_preFSR, TotWeight );
                  h_rap60120_preFSR_tot_fine->Fill( rap_preFSR, TotWeight );
               } else if (M_preFSR>15 && M_preFSR<60) {
                  h_rap1560_preFSR_tot->Fill( rap_preFSR, TotWeight );
                  h_pt1560_preFSR_tot->Fill( pt_preFSR, TotWeight );
                  h_phistar1560_preFSR_tot->Fill( phistar_preFSR, TotWeight );
                  h_rap1560_preFSR_tot_fine->Fill( rap_preFSR, TotWeight );
                  h_pt1560_preFSR_tot_fine->Fill( pt_preFSR, TotWeight );
                  h_phistar1560_preFSR_tot_fine->Fill( phistar_preFSR, TotWeight );
               }

               // fill post-FSR histos and response matrices
               h_mass_postFSR_tot->Fill( M_postFSR, TotWeight );
               h_mass_postpreFSR_tot->Fill( M_postFSR, M_preFSR, TotWeight );
               h_mass_postFSR_tot_fine->Fill( M_postFSR, TotWeight );
               h_mass_postpreFSR_tot_fine->Fill( M_postFSR, M_preFSR, TotWeight );
               Double_t resol = (M_preFSR - M_postFSR) / M_preFSR;
               h_mass_ratio_tot->Fill( resol, TotWeight );

               h_mass3bins_postFSR_tot->Fill( M_postFSR, TotWeight );
               h_mass3bins_postpreFSR_tot->Fill( M_postFSR, M_preFSR, TotWeight );
               h_mass3bins_ratio_tot->Fill( resol, TotWeight );
               h_mass3bins_postFSR_tot_fine->Fill( M_postFSR, TotWeight );
               h_mass3bins_postpreFSR_tot_fine->Fill( M_postFSR, M_preFSR, TotWeight );

               if (M_preFSR>60 && M_preFSR<120) {
                  h_pt_postFSR_tot->Fill( pt_postFSR, TotWeight );
                  h_pt_postpreFSR_tot->Fill( pt_postFSR, pt_preFSR, TotWeight );
                  h_pt_postFSR_tot_fine->Fill( pt_postFSR, TotWeight );
                  h_pt_postpreFSR_tot_fine->Fill( pt_postFSR, pt_preFSR, TotWeight );
                  resol = (pt_preFSR - pt_postFSR) / pt_preFSR;
                  h_pt_ratio_tot->Fill( resol, TotWeight );

                  h_phistar_postFSR_tot->Fill( phistar_postFSR, TotWeight );
                  h_phistar_postpreFSR_tot->Fill( phistar_postFSR, phistar_preFSR, TotWeight );
                  h_phistar_postFSR_tot_fine->Fill( phistar_postFSR, TotWeight );
                  h_phistar_postpreFSR_tot_fine->Fill( phistar_postFSR, phistar_preFSR, TotWeight );
                  resol = (phistar_preFSR - phistar_postFSR);
                  h_phistar_ratio_tot->Fill( resol, TotWeight );

                  h_rap60120_postFSR_tot->Fill( rap_postFSR, TotWeight );
                  h_rap60120_postpreFSR_tot->Fill( rap_postFSR, rap_preFSR, TotWeight );
                  h_rap60120_postFSR_tot_fine->Fill( rap_postFSR, TotWeight );
                  h_rap60120_postpreFSR_tot_fine->Fill( rap_postFSR, rap_preFSR, TotWeight );
                  resol = (rap_preFSR - rap_postFSR);
                  h_rap60120_ratio_tot->Fill( resol, TotWeight );
               } else if (M_preFSR>15 && M_preFSR<60) {
                  h_rap1560_postFSR_tot->Fill( rap_postFSR, TotWeight );
                  h_rap1560_postpreFSR_tot->Fill( rap_postFSR, rap_preFSR, TotWeight );
                  h_rap1560_postFSR_tot_fine->Fill( rap_postFSR, TotWeight );
                  h_rap1560_postpreFSR_tot_fine->Fill( rap_postFSR, rap_preFSR, TotWeight );
                  resol = (rap_preFSR - rap_postFSR);
                  h_rap1560_ratio_tot->Fill( resol, TotWeight );

                  h_pt1560_postFSR_tot->Fill( pt_postFSR, TotWeight );
                  h_pt1560_postpreFSR_tot->Fill( pt_postFSR, pt_preFSR, TotWeight );
                  h_pt1560_postFSR_tot_fine->Fill( pt_postFSR, TotWeight );
                  h_pt1560_postpreFSR_tot_fine->Fill( pt_postFSR, pt_preFSR, TotWeight );
                  resol = (pt_preFSR - pt_postFSR);
                  h_pt1560_ratio_tot->Fill( resol, TotWeight );

                  h_phistar1560_postFSR_tot->Fill( phistar_postFSR, TotWeight );
                  h_phistar1560_postpreFSR_tot->Fill( phistar_postFSR, phistar_preFSR, TotWeight );
                  h_phistar1560_postFSR_tot_fine->Fill( phistar_postFSR, TotWeight );
                  h_phistar1560_postpreFSR_tot_fine->Fill( phistar_postFSR, phistar_preFSR, TotWeight );
                  resol = (phistar_preFSR - phistar_postFSR);
                  h_phistar1560_ratio_tot->Fill( resol, TotWeight );
               } // M_preFSR range
            } // isPassAcc_preFSR == kTrue
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

   // normalise: divide each R_ij element of the response matrix by gen_j the number of pre-FSR events in pre-FSR bin j
   for (int j=0; j<=h_mass_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_mass_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_mass_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_mass_postpreFSR_tot->SetBinContent(i,j,h_mass_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_mass_postpreFSR_tot->SetBinError(i,j,h_mass_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }
   for (int j=0; j<=h_mass3bins_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_mass3bins_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_mass3bins_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_mass3bins_postpreFSR_tot->SetBinContent(i,j,h_mass3bins_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_mass3bins_postpreFSR_tot->SetBinError(i,j,h_mass3bins_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }
   for (int j=0; j<=h_pt_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_pt_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_pt_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_pt_postpreFSR_tot->SetBinContent(i,j,h_pt_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_pt_postpreFSR_tot->SetBinError(i,j,h_pt_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }
   for (int j=0; j<=h_rap1560_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_rap1560_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_rap1560_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_rap1560_postpreFSR_tot->SetBinContent(i,j,h_rap1560_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_rap1560_postpreFSR_tot->SetBinError(i,j,h_rap1560_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }
   for (int j=0; j<=h_rap60120_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_rap60120_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_rap60120_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_rap60120_postpreFSR_tot->SetBinContent(i,j,h_rap60120_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_rap60120_postpreFSR_tot->SetBinError(i,j,h_rap60120_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }
   for (int j=0; j<=h_phistar_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_phistar_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_phistar_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_phistar_postpreFSR_tot->SetBinContent(i,j,h_phistar_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_phistar_postpreFSR_tot->SetBinError(i,j,h_phistar_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }
   for (int j=0; j<=h_pt1560_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_pt1560_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_pt1560_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_pt1560_postpreFSR_tot->SetBinContent(i,j,h_pt1560_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_pt1560_postpreFSR_tot->SetBinError(i,j,h_pt1560_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }
   for (int j=0; j<=h_phistar1560_postpreFSR_tot->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_phistar1560_preFSR_tot->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_phistar1560_postpreFSR_tot->GetNbinsX()+1; i++) {
            h_phistar1560_postpreFSR_tot->SetBinContent(i,j,h_phistar1560_postpreFSR_tot->GetBinContent(i,j)/genj);
            h_phistar1560_postpreFSR_tot->SetBinError(i,j,h_phistar1560_postpreFSR_tot->GetBinError(i,j)/genj);
         }
      }
   }

	f->cd();
	h_mass_preFSR_tot->Write();
	h_mass_postFSR_tot->Write();
   h_mass_ratio_tot->Write();
   h_mass_postpreFSR_tot->Write();
	h_mass_preFSR_tot_fine->Write();
	h_mass_postFSR_tot_fine->Write();
	h_mass_bare_tot_fine->Write();
   h_mass_postpreFSR_tot_fine->Write();

	h_mass3bins_preFSR_tot->Write();
	h_mass3bins_postFSR_tot->Write();
   h_mass3bins_ratio_tot->Write();
   h_mass3bins_postpreFSR_tot->Write();
	h_mass3bins_preFSR_tot_fine->Write();
	h_mass3bins_postFSR_tot_fine->Write();
	h_mass3bins_bare_tot_fine->Write();
   h_mass3bins_postpreFSR_tot_fine->Write();

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

	h_phistar_preFSR_tot->Write();
	h_phistar_postFSR_tot->Write();
   h_phistar_ratio_tot->Write();
   h_phistar_postpreFSR_tot->Write();
	h_phistar_preFSR_tot_fine->Write();
	h_phistar_postFSR_tot_fine->Write();
   h_phistar_postpreFSR_tot_fine->Write();

	h_pt1560_preFSR_tot->Write();
	h_pt1560_postFSR_tot->Write();
   h_pt1560_ratio_tot->Write();
   h_pt1560_postpreFSR_tot->Write();
	h_pt1560_preFSR_tot_fine->Write();
	h_pt1560_postFSR_tot_fine->Write();
   h_pt1560_postpreFSR_tot_fine->Write();

	h_phistar1560_preFSR_tot->Write();
	h_phistar1560_postFSR_tot->Write();
   h_phistar1560_ratio_tot->Write();
   h_phistar1560_postpreFSR_tot->Write();
	h_phistar1560_preFSR_tot_fine->Write();
	h_phistar1560_postFSR_tot_fine->Write();
   h_phistar1560_postpreFSR_tot_fine->Write();


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
