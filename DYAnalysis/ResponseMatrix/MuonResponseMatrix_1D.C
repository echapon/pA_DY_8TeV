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

// -- for Rochester Muon momentum correction -- //
#include <Include/roccor.2016.v3/RoccoR.cc>

//Customized header files
#include <Include/DYAnalyzer.h>
#include <Include/UnfoldUtils.h>

using namespace DYana;
using unfold::gUnfold;

static inline void loadBar(int x, int n, int r, int w);
// TH1F* unfold_MLE(TH1F *hin, TH2F *hresponse, TH2F *hcov);
// TH1F* fold_MLE(TH1F *hin, TH1F *hresponse);
void MuonResponseMatrix_1D(Bool_t isCorrected = kFALSE, TString Sample = "Powheg", TString HLTname = "PAL3Mu12", int run=0 ) // run: 0=all, 1=pPb, 2=PbP
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

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

   analyzer->SetupMCsamples_v20180111(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

	TFile *f = new TFile("ResponseMatrix/ROOTFile_ResponseMatrix_" + Sample + "_" + isApplyMomCorr + "_" + Form("%d",run) + ".root", "RECREATE");

	TH1D *h_mass_gen = new TH1D("h_mass_gen", ";mass(gen level);Entries", binnum, bins);
	TH1D *h_mass_reco = new TH1D("h_mass_reco", ";mass(reco level);Entries", binnum, bins);
	TH1D *h_mass_resol = new TH1D("h_mass_resol", ";mass resolution ( (gen-reco)/gen) );Entries", 100,-0.2,0.2);
   TH2D *h_mass_response = new TH2D("h_mass_response", ";mass(reco level);mass(gen level)", binnum, bins, binnum, bins);

	TH1D *h_pt_gen = new TH1D("h_pt_gen", ";pt(gen level);Entries", ptbinnum_meas, ptbin_meas);
	TH1D *h_pt_reco = new TH1D("h_pt_reco", ";pt(reco level);Entries", ptbinnum_meas, ptbin_meas);
	TH1D *h_pt_resol = new TH1D("h_pt_resol", ";pt resolution ( (gen-reco)/gen) );Entries", 100, -0.4, 0.4);
   TH2D *h_pt_response = new TH2D("h_pt_response", ";pt(reco level);pt(gen level)", ptbinnum_meas, ptbin_meas, ptbinnum_meas, ptbin_meas);

	TH1D *h_rap1560_gen = new TH1D("h_rap1560_gen", ";rap1560(gen level);Entries", rapbinnum_1560, rapbin_1560);
	TH1D *h_rap1560_reco = new TH1D("h_rap1560_reco", ";rap1560(reco level);Entries", rapbinnum_1560, rapbin_1560);
	TH1D *h_rap1560_resol = new TH1D("h_rap1560_resol", ";rap1560 resolution ( (gen-reco)/gen) );Entries", 100, -0.1, 0.1);
   TH2D *h_rap1560_response = new TH2D("h_rap1560_response", ";rap1560(reco level);rap1560(gen level)", rapbinnum_1560, rapbin_1560, rapbinnum_1560, rapbin_1560);

	TH1D *h_rap60120_gen = new TH1D("h_rap60120_gen", ";rap60120(gen level);Entries", rapbinnum_60120, rapbin_60120);
	TH1D *h_rap60120_reco = new TH1D("h_rap60120_reco", ";rap60120(reco level);Entries", rapbinnum_60120, rapbin_60120);
	TH1D *h_rap60120_resol = new TH1D("h_rap60120_resol", ";rap60120 resolution ( (gen-reco)/gen) );Entries", 100, -0.1, 0.1);
   TH2D *h_rap60120_response = new TH2D("h_rap60120_response", ";rap60120(reco level);rap60120(gen level)", rapbinnum_60120, rapbin_60120, rapbinnum_60120, rapbin_60120);

	TH1D *h_phistar_gen = new TH1D("h_phistar_gen", ";phistar(gen level);Entries", phistarnum, phistarbin);
	TH1D *h_phistar_reco = new TH1D("h_phistar_reco", ";phistar(reco level);Entries", phistarnum, phistarbin);
	TH1D *h_phistar_resol = new TH1D("h_phistar_resol", ";phistar resolution ( (gen-reco)/gen) );Entries", 100, -0.02, 0.02);
   TH2D *h_phistar_response = new TH2D("h_phistar_response", ";phistar(reco level);phistar(gen level)", phistarnum, phistarbin, phistarnum, phistarbin);

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
      if (run==1 && doflip) continue;
      if (run==2 && !doflip) continue;

		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		// -- ntuple Setting -- //
		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_GenOthers();
		ntuple->TurnOnBranches_HI();

      RoccoR  rmcor("Include/roccor.2016.v3/rcdata.2016.v3"); //directory path as input for now; initialize only once, contains all variations

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
				/////////////////////////////////////
				// -- Generator level selection -- //
				/////////////////////////////////////
				vector< GenLepton > GenLeptonCollection;

				Int_t NGenLeptons = ntuple->gnpair;
				for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
				{
					GenLepton genlep;
					genlep.FillFromNtuple(ntuple, i_gen);
					if( genlep.isMuon() && genlep.fromHardProcessFinalState )
						GenLeptonCollection.push_back( genlep );
				}

				GenLepton genlep1 = GenLeptonCollection[0];
				GenLepton genlep2 = GenLeptonCollection[1];
				Double_t gen_M = (genlep1.Momentum + genlep2.Momentum).M();
				Double_t gen_Pt = (genlep1.Momentum + genlep2.Momentum).Pt();
				Double_t gen_Rap = (genlep1.Momentum + genlep2.Momentum).Rapidity() - rapshift;
				Double_t gen_Phistar = Object::phistar(genlep1, genlep2);

				Bool_t isPassAcc_GenLepton = kFALSE;
				isPassAcc_GenLepton = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

				///////////////////////////////////////////
				// -- Reconstruction level selection -- //
				///////////////////////////////////////////
				Bool_t isPassEventSelection = kFALSE;

				if( ntuple->isTriggered( analyzer->HLT ) )
				{
					// -- Collect Reconstruction level information -- //
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
                     int s=0, m=0; // nominal
                     // int s=7, m=6; // Run2016H only

                     if( Tag[i_tup].Contains("Data") )
                        // careful, need to switch back eta to the lab frame
                        qter = rmcor.kScaleDT(mu.charge, mu.Pt, analyzer->sign*mu.eta, mu.phi, s, m);
                     else{
                        double u1 = gRandom->Rndm();
                        int nl = ntuple->Muon_trackerLayers[i_reco];
                        if (!GenFlag || GenLeptonCollection.size()<2) {
                           double u2 = gRandom->Rndm();
                           qter = rmcor.kScaleAndSmearMC(mu.charge, mu.Pt, analyzer->sign*mu.eta, mu.phi, nl, u1, u2, s, m);
                        } else {
                           // gen-reco matching
                           double drmin=999; double pt_drmin=0;
                           for (unsigned int igen=0; igen<GenLeptonCollection.size(); igen++) {
                              double dr = mu.Momentum.DeltaR(GenLeptonCollection[igen].Momentum);
                              if (dr<drmin) {
                                 drmin = dr;
                                 pt_drmin = GenLeptonCollection[igen].Pt;
                              }
                           } // for igen in GenLeptonCollection (gen-reco matching)
                           if (drmin<0.1) qter = rmcor.kScaleFromGenMC(mu.charge, mu.Pt, analyzer->sign*mu.eta, mu.phi, nl, pt_drmin, u1, s, m);
                           else  {
                              double u2 = gRandom->Rndm();
                              qter = rmcor.kScaleAndSmearMC(mu.charge, mu.Pt, analyzer->sign*mu.eta, mu.phi, nl, u1, u2, s, m);
                           } // if drmin<0.03
                        } // if (!GenFlag || GenLeptonCollection.size()<2)
                     } // if Tag[i_tup] == "Data"

                     // -- Change Muon pT, eta and phi with updated(corrected) one -- //
                     mu.Momentum.SetPtEtaPhiM(qter*mu.Pt,mu.eta,mu.phi,mu.Momentum.M());
                     mu.Pt = mu.Momentum.Pt();
                     // mu.eta = mu.Momentum.Eta();
                     // mu.phi = mu.Momentum.Phi();
                  }

						MuonCollection.push_back( mu );
					}

					// -- Event Selection -- //
					vector< Muon > SelectedMuonCollection;
					isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

					if( isPassEventSelection == kTRUE )
					{
						Muon Mu1 = SelectedMuonCollection[0];
						Muon Mu2 = SelectedMuonCollection[1];
						Double_t reco_M = ( Mu1.Momentum + Mu2.Momentum ).M();
                  Double_t reco_Pt = (Mu1.Momentum + Mu2.Momentum).Pt();
                  Double_t reco_Rap = (Mu1.Momentum + Mu2.Momentum).Rapidity() - rapshift;
                  Double_t reco_Phistar = Object::phistar(Mu1, Mu2);

						if( isPassAcc_GenLepton == kTRUE )
						{
                     // fill gen histos
                     h_mass_gen->Fill( gen_M, TotWeight );
                     if (gen_M>60 && gen_M<120) {
                        h_pt_gen->Fill( gen_Pt, TotWeight );
                        h_phistar_gen->Fill( gen_Phistar, TotWeight );
                        h_rap60120_gen->Fill( gen_Rap, TotWeight );
                     } else if (gen_M>15 && gen_M<60) {
                        h_rap1560_gen->Fill( gen_Rap, TotWeight );
                     }

							h_mass_reco->Fill( reco_M, TotWeight );
							h_mass_response->Fill( reco_M, gen_M, TotWeight );
							Double_t resol = (gen_M - reco_M) / gen_M;
							h_mass_resol->Fill( resol, TotWeight );

                     if (reco_M>60 && reco_M<120) {
                        if (gen_M>60 && gen_M<120) {
                           h_pt_reco->Fill( reco_Pt, TotWeight );
                           h_pt_response->Fill( reco_Pt, gen_Pt, TotWeight );
                           resol = (gen_Pt - reco_Pt) / gen_Pt;
                           h_pt_resol->Fill( resol, TotWeight );

                           h_phistar_reco->Fill( reco_Phistar, TotWeight );
                           h_phistar_response->Fill( reco_Phistar, gen_Phistar, TotWeight );
                           resol = (gen_Phistar - reco_Phistar);
                           h_phistar_resol->Fill( resol, TotWeight );

                           h_rap60120_reco->Fill( reco_Rap, TotWeight );
                           h_rap60120_response->Fill( reco_Rap, gen_Rap, TotWeight );
                           resol = (gen_Rap - reco_Rap);
                           h_rap60120_resol->Fill( resol, TotWeight );
                        } else {
                           if (gen_M>15 && gen_M<60) {
                              h_rap1560_response->Fill( -99, gen_Rap, TotWeight );
                           } 
                           // h_pt_response->Fill( reco_Pt, -99, TotWeight );
                           // h_rap60120_response->Fill( reco_Rap, -99, TotWeight );
                           // h_phistar_response->Fill( reco_Phistar, -99, TotWeight );
                        } // gen_M range
                     } else if (reco_M>15 && reco_M<60) {
                        if (gen_M>15 && gen_M<60) {
                           h_rap1560_reco->Fill( reco_Rap, TotWeight );
                           h_rap1560_response->Fill( reco_Rap, gen_Rap, TotWeight );
                           resol = (gen_Rap - reco_Rap);
                           h_rap1560_resol->Fill( resol, TotWeight );
                        } else { 
                           if (gen_M>60 && gen_M<120) {
                              h_pt_response->Fill( -99, gen_Pt, TotWeight );
                              h_rap60120_response->Fill( -99, gen_Rap, TotWeight );
                              h_phistar_response->Fill( -99, gen_Phistar, TotWeight );
                           } 
                           // h_rap1560_response->Fill( reco_Rap, -99, TotWeight );
                        } // gen_M range
                     } else { // reco mass is neither in 15<M<60 nor 60<M<120 range
                        if (gen_M>60 && gen_M<120) {
                           h_pt_response->Fill( -99, gen_Pt, TotWeight );
                           h_rap60120_response->Fill( -99, gen_Rap, TotWeight );
                           h_phistar_response->Fill( -99, gen_Phistar, TotWeight );
                        } else if (gen_M>15 && gen_M<60)
                           h_rap1560_response->Fill( -99, gen_Rap, TotWeight );
                     } // reco_M range
						} // isPassAcc_GenLepton == kTrue
						else { // -- No gen-level event within the acceptance, but reco event exists (= "Fake" events) -- //
                     h_mass_response->Fill( reco_M, -99, TotWeight );
                     if (reco_M>60 && reco_M<120) {
                        h_pt_response->Fill( reco_Pt, -99, TotWeight );
                        h_rap60120_response->Fill( reco_Rap, -99, TotWeight );
                        h_phistar_response->Fill( reco_Phistar, -99, TotWeight );
                     } else if (reco_M>15 && reco_M<60)
                        h_rap1560_response->Fill( reco_Rap, -99, TotWeight );
                  }
					}
				} // -- if( ntuple->isTriggered( analyzer->HLT ) ) -- // 

            // if( isPassAcc_GenLepton == kTRUE && isPassEventSelection == kFALSE) { // -- gen-level event exists in gen-level, but there is no reco-level event -- //
            //    h_mass_response->Fill( -99, gen_M, TotWeight );
            //    if (gen_M>60 && gen_M<120) {
            //       h_pt_response->Fill( -99, gen_Pt, TotWeight );
            //       h_rap60120_response->Fill( -99, gen_Rap, TotWeight );
            //       h_phistar_response->Fill( -99, gen_Phistar, TotWeight );
            //    } else if (gen_M>15 && gen_M<60)
            //       h_rap1560_response->Fill( -99, gen_Rap, TotWeight );
            // }

			} // -- End of if( GenFlag == kTRUE ) -- //

		} //End of event iteration

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;
		cout << "\tSum of Events of Separated Events: " << SumEvents_Separated << endl;

		f->cd();

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();
	h_mass_gen->Write();
	h_mass_reco->Write();
	h_mass_resol->Write();

   // normalise: divide each R_ij element of the response matrix by gen_j the number of generated events in gen bin j
   for (int j=0; j<=h_mass_response->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_mass_gen->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_mass_response->GetNbinsX()+1; i++) {
            h_mass_response->SetBinContent(i,j,h_mass_response->GetBinContent(i,j)/genj);
            h_mass_response->SetBinError(i,j,h_mass_response->GetBinError(i,j)/genj);
         }
      }
   }
   h_mass_response->Write();

	h_pt_gen->Write();
	h_pt_reco->Write();
	h_pt_resol->Write();

   // normalise: divide each R_ij element of the response matrix by gen_j the number of generated events in gen bin j
   for (int j=0; j<=h_pt_response->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_pt_gen->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_pt_response->GetNbinsX()+1; i++) {
            h_pt_response->SetBinContent(i,j,h_pt_response->GetBinContent(i,j)/genj);
            h_pt_response->SetBinError(i,j,h_pt_response->GetBinError(i,j)/genj);
         }
      }
   }
   h_pt_response->Write();

	h_rap1560_gen->Write();
	h_rap1560_reco->Write();
	h_rap1560_resol->Write();

   // normalise: divide each R_ij element of the response matrix by gen_j the number of generated events in gen bin j
   for (int j=0; j<=h_rap1560_response->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_rap1560_gen->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_rap1560_response->GetNbinsX()+1; i++) {
            h_rap1560_response->SetBinContent(i,j,h_rap1560_response->GetBinContent(i,j)/genj);
            h_rap1560_response->SetBinError(i,j,h_rap1560_response->GetBinError(i,j)/genj);
         }
      }
   }
   h_rap1560_response->Write();

	h_rap60120_gen->Write();
	h_rap60120_reco->Write();
	h_rap60120_resol->Write();

   // normalise: divide each R_ij element of the response matrix by gen_j the number of generated events in gen bin j
   for (int j=0; j<=h_rap60120_response->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_rap60120_gen->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_rap60120_response->GetNbinsX()+1; i++) {
            h_rap60120_response->SetBinContent(i,j,h_rap60120_response->GetBinContent(i,j)/genj);
            h_rap60120_response->SetBinError(i,j,h_rap60120_response->GetBinError(i,j)/genj);
         }
      }
   }
   h_rap60120_response->Write();

	h_phistar_gen->Write();
	h_phistar_reco->Write();
	h_phistar_resol->Write();

   // normalise: divide each R_ij element of the response matrix by gen_j the number of generated events in gen bin j
   for (int j=0; j<=h_phistar_response->GetNbinsY()+1; j++) { // include UF and OF
      double genj = h_phistar_gen->GetBinContent(j);
      if (genj>0) {
         for (int i=0; i<=h_phistar_response->GetNbinsX()+1; i++) {
            h_phistar_response->SetBinContent(i,j,h_phistar_response->GetBinContent(i,j)/genj);
            h_phistar_response->SetBinError(i,j,h_phistar_response->GetBinError(i,j)/genj);
         }
      }
   }
   h_phistar_response->Write();

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
