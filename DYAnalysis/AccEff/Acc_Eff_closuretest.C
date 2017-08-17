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

#include <vector>

// -- for Rochester Muon momentum correction -- //
// #include </home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/RoccoR.cc>
// #include </home/kplee/CommonCodes/DrellYanAnalysis/RochesterMomCorr_76X/rochcor2015.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>
#include <BkgEst/interface/defs.h>
#include <HIstuff/HFweight.h>

using namespace DYana;

static inline void loadBar(int x, int n, int r, int w);
void Acc_Eff_closuretest(Bool_t isCorrected = kFALSE, TString Sample = "Powheg", TString HLTname = "HLT_PAL3Mu12_v*" )
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Sample: " << Sample << endl;

	TString isApplyMomCorr = "";
	if( isCorrected == kTRUE )
	{
		cout << "Apply Roochester Muon Momentum Correction..." << endl;
		cout << "NOT IMPLEMENTED YET" << endl;
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

	TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_closuretest_" + Sample + "_" + HLTname + ".root", "RECREATE");

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

   int nbinspt = 9;
   double ptbins[10] = {10,15,25,30,35,40,45,50,80,200};
   int nbinspt2 = 5;
   double ptbins2[6] = {10,15,25,40,80,200};

	TH1D *h_pt_EffTotal_0012 = new TH1D("h_mass_EffTotal_0012", "", nbinspt, ptbins);
	TH1D *h_pt_EffPass_0012 = new TH1D("h_mass_EffPass_0012", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffTotal_1221 = new TH1D("h_mass_EffTotal_1221", "", nbinspt, ptbins);
	TH1D *h_pt_EffPass_1221 = new TH1D("h_mass_EffPass_1221", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffTotal_2124 = new TH1D("h_mass_EffTotal_2124", "", nbinspt, ptbins);
	TH1D *h_pt_EffPass_2124 = new TH1D("h_mass_EffPass_2124", "", nbinspt,ptbins);	 

	// -- After applying efficiency correction -- //
	TH1D *h_mass_EffPass_Corr_tnp = new TH1D("h_mass_EffPass_Corr_tnp", "", binnum, bins);

	TString BaseLocation = "/eos/cms/store/group/cmst3/user/echapon/pA_8p16TeV/DYtuples/";
	// -- Each ntuple directory & corresponding Tags -- //
		// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents;

   analyzer->SetupMCsamples_v20170519(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents);

   // initialise the HF reweighting tool
   HFweight hftool;

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
      // loop only on DYMuMu!
      if (!Tag[i_tup].Contains("DYMuMu")) continue;

		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;
		TH1D* h_mass = new TH1D("h_mass_"+Tag[i_tup], "", 600, 0, 600);

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain );
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_HI();

      // rochcor2015 *rmcor = new rochcor2015();

		Bool_t isMC;
		Tag[i_tup] == "Data" ? isMC = kFALSE : isMC = kTRUE;

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "TT" )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Double_t SumWeights = 0;
		Double_t SumWeights_Separated = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl; 

		Double_t norm = ( Xsec[i_tup] * lumi_all ) / (Double_t)nEvents[i_tup];
		cout << "\t[Normalization factor: " << norm << "]" << endl;

		// NEvents = 1000;
		// -- Event loop starts -- //
		for(Int_t i=0; i<NEvents; i++)
		{
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
         // ADD HF weight !!
         Double_t PUWeight = hftool.weight(ntuple->hiHF,HFweight::HFside::both); 

			Double_t TotWeight = norm * GenWeight;

			Bool_t GenFlag = kFALSE;
			GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
				SumWeights_Separated += GenWeight;

				// -- Collect gen-level information -- //
				vector<GenLepton> GenLeptonCollection;
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

            // add mass cut
            if (gen_M<60 || gen_M>120) continue;

				// -- Flags -- //
				Bool_t Flag_PassAcc = kFALSE;
				Bool_t Flag_PassAccEff = kFALSE;

				// -- Fill the mass histograms -- //
				h_mass->Fill( gen_M, TotWeight );
				h_mass_tot->Fill( gen_M, TotWeight );

            // check the acceptance at gen level
				Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

				// -- Acceptance Calculation -- //
				if( Flag_PassAcc == kTRUE ) 
				{
				}
				else
				{
				} 	

				Double_t TnpWeight = 1.; // -- Efficiency correction factor -- //

				// -- Calculate the efficiency among the events passing acceptance condition -- //
				if( Flag_PassAcc == kTRUE )
				{
					Bool_t Flag_PassEff = kFALSE;

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
                     // not implemented yet // FIXME
                     // float qter = 1.0;
                     // 
                     // if( Tag[i_tup] == "Data" )
                     //    rmcor->momcor_data(mu.Momentum, mu.charge, 0, qter);
                     // else
                     //    rmcor->momcor_mc(mu.Momentum, mu.charge, mu.trackerLayers, qter);

                     // // -- Change Muon pT, eta and phi with updated(corrected) one -- //
                     // mu.Pt = mu.Momentum.Pt();
                     // mu.eta = mu.Momentum.Eta();
                     // mu.phi = mu.Momentum.Phi();
                  }

                  MuonCollection.push_back( mu );
               }

               // -- Event Selection -- //
               vector< Muon > PassMuonCollection;
               vector< Muon > TagMuonCollection;
               vector< int > PassIdx;
               vector< int > TagIdx;

               for(Int_t j=0; j<(int)MuonCollection.size(); j++)
               {
                  Muon mu = MuonCollection[j];
                  if( mu.isHighPtMuon_minus_dzVTX() && mu.relPFiso < 0.15 && mu.isTrigMatched(ntuple, "HLT_PAL3Mu12_v*") && fabs(mu.eta)<2.4) {
                     // matching 
                     int imatch = -1;
                     if (fabs(genlep1.eta-mu.eta)<0.02 && fabs(genlep1.phi-mu.phi)<0.02) imatch=0;
                     else if (fabs(genlep2.eta-mu.eta)<0.02 && fabs(genlep2.phi-mu.phi)<0.02) imatch=1;

                     if (imatch<0) continue;
                     if (mu.Pt > 15) {
                        TagMuonCollection.push_back( mu );
                        TagIdx.push_back(imatch);
                     }
                     if (mu.Pt > 10) {
                        PassMuonCollection.push_back( mu );
                        PassIdx.push_back(imatch);
                     }
                  }
               }

               // now fill histos
               bool used[2] = {false,false}; // have we used genlepton i?
               for (unsigned int j=0; j<TagMuonCollection.size(); j++) {
                  if (used[TagIdx[j]]) continue;
                  used[TagIdx[j]] = true;
                  int probeidx = 1-TagIdx[j];

                  bool passprobe=false;
                  for (unsigned int k=0; k<PassMuonCollection.size(); k++) {
                     if (PassIdx[k] == probeidx) passprobe=true;
                  }

                  double probeeta = GenLeptonCollection[probeidx].eta;
                  double probept = GenLeptonCollection[probeidx].Pt;

                  if (fabs(probeeta)<1.2) {
                     h_pt_EffTotal_0012->Fill(probept,TotWeight);
                     if (passprobe) h_pt_EffPass_0012->Fill(probept,TotWeight);
                  } else if (fabs(probeeta)<2.1) {
                     h_pt_EffTotal_1221->Fill(probept,TotWeight);
                     if (passprobe) h_pt_EffPass_1221->Fill(probept,TotWeight);
                  } else if (fabs(probeeta)<2.4) {
                     h_pt_EffTotal_2124->Fill(probept,TotWeight);
                     if (passprobe) h_pt_EffPass_2124->Fill(probept,TotWeight);
                  }
               }

				} // -- End of if( Flag_PassAcc )


				// // -- Acceptance * Efficiency Calculation -- //
				// if( Flag_PassAccEff == kTRUE)
				// {
				// 	AccEff_Mass->FillWeighted(1, TotWeight, gen_M);
				// 	h_mass_AccEffPass_Corr->Fill( gen_M, TotWeight * Eff_SF );
				// }
				// else 
				// 	AccEff_Mass->FillWeighted(0, TotWeight, gen_M);

			} // -- End of if( GenFlag == kTRUE )

		} //End of event iteration

		f->cd();
		h_mass->Write();

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();
	h_mass_tot->Write();
   h_pt_EffTotal_0012->Write();
   h_pt_EffPass_0012->Write();
   h_pt_EffTotal_1221->Write();
   h_pt_EffPass_1221->Write();
   h_pt_EffTotal_2124->Write();
   h_pt_EffPass_2124->Write();

   TEfficiency *teff0012 = new TEfficiency(*h_pt_EffPass_0012,*h_pt_EffTotal_0012);
   teff0012->SetName("eff0012");
   teff0012->Write();
   TEfficiency *teff1221 = new TEfficiency(*h_pt_EffPass_1221,*h_pt_EffTotal_1221);
   teff1221->SetName("eff1221");
   teff1221->Write();
   TEfficiency *teff2124 = new TEfficiency(*h_pt_EffPass_2124,*h_pt_EffTotal_2124);
   teff2124->SetName("eff2124");
   teff2124->Write();
   f->Close();


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


