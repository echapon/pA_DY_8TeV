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
#include <TGraphAsymmErrors.h>
#include <TRandom3.h>

#include <vector>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "../Include/DYAnalyzer.h"
#include "../BkgEst/interface/defs.h"
#include "../HIstuff/HFweight.h"

using namespace DYana;

const Double_t isoval[10] = { 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1, 1e99};

static inline void loadBar(int x, int n, int r, int w);
void Acc_Eff(TString Sample = "Powheg", TString HLTname = "PAL3Mu12", int run=0, bool doHFrew = true, HFweight::HFside rewmode = HFweight::HFside::both, bool zptrew = true ) // run: 0=all, 1=pPb, 2=PbP
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Sample: " << Sample << endl;

	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( HLTname );

   TString srew("norew");
   if (doHFrew) {
      if (rewmode==HFweight::HFside::both) srew="rewboth";
      else if (rewmode==HFweight::HFside::plus) srew="rewplus";
      else if (rewmode==HFweight::HFside::minus) srew="rewminus";
      else if (rewmode==HFweight::HFside::Ntracks) srew="rewNtracks";
   }
   TString srew2("");
   if (!zptrew) srew2 = "_noZptrew";

	TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_isostudy_" + Sample + "_" + HLTname + "_" + Form("%d",run) + ".root", "RECREATE");

   // update HLTname to a standard name
   if (HLTname.Contains("PAL3Mu12")) HLTname = "HLT_PAL3Mu12_v*";
   else if (HLTname.Contains("L1DoubleMu0")) HLTname = "HLT_PAL1DoubleMu0_v*";

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

 	TH1D *h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", binnum, bins);
	TH1D *h_mass_AccPass = new TH1D("h_mass_AccPass", "", binnum, bins);

   TH1D *h_mass_EffTotal = new TH1D("h_mass_EffTotal", "", binnum, bins);
   TH1D ***h_mass_EffPass = new TH1D**[12];
   for (int i=0; i<12; i++) {
      h_mass_EffPass[i] = new TH1D*[10];
      for (int j=0; j<10; j++)
         h_mass_EffPass[i][j] = new TH1D(Form("h_mass_EffPass_%d_%d",i,j), "", binnum, bins);	 
   }


	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";
	// -- Each ntuple directory & corresponding Tags -- //
		// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< SampleTag > STags;

   analyzer->SetupMCsamples_v20180814(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

   // add also data and QCD
   using DYana::SampleTag;
   if (HLTname.Contains("L3Mu12")) {
            ntupleDirectory.push_back( "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20170518/170517_220343/0000/" ); 
            ntupleDirectory.push_back( "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20170518/170517_220714/0000/" ); 
   } else {
            ntupleDirectory.push_back( "PADoubleMuon/crab_PADoubleMuon_DYtuple_PAL1DoubleMu0_1stpart_2010604/180604_092521/0000/" ); 
            ntupleDirectory.push_back( "PADoubleMuon/crab_PADoubleMuon_DYtuple_PAL1DoubleMu0_2ndpart_2010604/180604_093537/0000/" ); 
   }
   Tag.push_back( "Data1" ); STags.push_back(SampleTag::Data1);
   Tag.push_back( "Data2" ); STags.push_back(SampleTag::Data2);
   ntupleDirectory.push_back( "QCDtoMu_pThat-20_PbP-EmbEPOS_8p16_Pythia8/crab_QCDtoMu_20180117/180117_132953/0000/" ); Tag.push_back( "QCD" ); STags.push_back(SampleTag::QCD);

   // initialise the HF reweighting tool
   HFweight hftool;

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
      // loop only on DYMuMu! but add QCD and data
      bool isDYMuMu = (Tag[i_tup].Contains("DYMuMu"));
      bool isData   = (Tag[i_tup].Contains("Data"));
      bool isQCD    = (Tag[i_tup].Contains("QCD"));
      if (Sample=="Powheg" && !(isDYMuMu || isQCD)) continue;
      if (Sample=="Data" && !isData) continue;

      Bool_t doflip = (switcheta(STags[i_tup])<0);
      Int_t  flipsign = doflip ? -1 : 1;
      if (run==1 && doflip) continue;
      if (run==2 && !doflip) continue;
      analyzer->sign = flipsign;

		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;
		cout << "\t" << ntupleDirectory[i_tup] << endl;
		TH1D* h_massSS = new TH1D("h_massSS_"+Tag[i_tup], "", 600, 0, 600);
		TH1D* h_massOS = new TH1D("h_massOS_"+Tag[i_tup], "", 600, 0, 600);

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain, doflip );
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		if (isDYMuMu) ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_HI();

      // set the seed
      gRandom = new TRandom3(1);

		Bool_t isNLO = 0;
		if( Sample=="Powheg" && (Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "TT" || Tag[i_tup].Contains("WE") || Tag[i_tup].Contains("WMu")) )
		{
			isNLO = 1;
			cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
		}

		Double_t SumWeights = 0;
		Double_t SumWeights_Separated = 0;

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl; 

      // // default norm
      // Double_t norm = ( Xsec[i_tup] * lumi_all ) / (Double_t)nEvents[i_tup];
      // fancy norm with pPb / PbP mix
		Double_t norm = ( Xsec[i_tup] * lumi_part2 ) / (Double_t)nEvents[i_tup];
      if (doflip)
         norm = ( Xsec[i_tup] * lumi_part1 ) / (Double_t)nEvents[i_tup];

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
         Double_t PUWeight = 1.;
         if (doHFrew && !isData) PUWeight *= hftool.weight(ntuple->hiHF,rewmode); 

			Double_t TotWeight = (isDYMuMu) ? norm * GenWeight : 1;

			Bool_t GenFlag = kFALSE;
			GenFlag = !isDYMuMu || analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

			if( GenFlag == kTRUE )
			{
            Double_t gen_M=-1;
            Bool_t Flag_PassAcc = kFALSE;

            if (isDYMuMu) {
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
               gen_M = (genlep1.Momentum + genlep2.Momentum).M();
               Double_t gen_Rap = (genlep1.Momentum + genlep2.Momentum).Rapidity()-rapshift;
               Double_t gen_Pt = (genlep1.Momentum + genlep2.Momentum).Pt();
               Double_t gen_Phistar = Object::phistar(genlep1,genlep2);

               // -- Z pt reweighting -- //
               if (zptrew && isDYMuMu) TotWeight *= zptWeight(gen_Pt);

               // -- Flags -- //
               Flag_PassAcc = kFALSE;

               // -- Fill the mass histograms -- //
               h_mass_tot->Fill( gen_M, TotWeight );

               Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

               // -- Acceptance Calculation -- //
               h_mass_AccTotal->Fill( gen_M, TotWeight );
               if( Flag_PassAcc == kTRUE ) 
               {
                  h_mass_AccPass->Fill( gen_M, TotWeight );
               }
            } // end if (isDYMuMu)
            else Flag_PassAcc = kTRUE;


				// -- Calculate the efficiency among the events passing acceptance condition -- //
				if( Flag_PassAcc == kTRUE )
				{
					Bool_t Flag_PassEff = kFALSE;
					Bool_t Flag_OS = kFALSE;
					Bool_t Flag_SS = kFALSE;

               vector< Muon > MuonCollection;
               vector< Muon > SelectedMuonCollection;
					if( ntuple->isTriggered( analyzer->HLT ) )
					{
						// -- Collect Reconstruction level information -- //
						Int_t NLeptons = ntuple->nMuon;
						for(Int_t i_reco=0; i_reco<NLeptons; i_reco++)
						{
							Muon mu;
							mu.FillFromNtuple(ntuple, i_reco);
							MuonCollection.push_back( mu );
						}

						// -- Event Selection -- //
						Bool_t isPassEventSelection = kTRUE;

						if( isPassEventSelection == kTRUE && 
                        MuonCollection.size()>=2) {
                     if (MuonCollection[0].charge != MuonCollection[1].charge) {
                        if (!isData) Flag_PassEff = kTRUE;
                        if (isData) Flag_PassEff = kTRUE;
                        Flag_OS = kTRUE;
                     } else {
                        // if (isData) Flag_PassEff = kTRUE;
                        Flag_SS = kTRUE;
                     }

                     if (!isDYMuMu) {
                        TLorentzVector dimu = MuonCollection[0].Momentum + MuonCollection[1].Momentum;
                        gen_M = dimu.M();
                     }
                  }

					} // -- End of if( ntuple->isTriggered( HLT ) ) -- //

					// -- Efficiency Calculation -- //
               // if (isDYMuMu) {
                  h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );
               // }

               if (Flag_OS) h_massOS->Fill( gen_M, TotWeight * PUWeight );
               else if (Flag_SS) h_massSS->Fill( gen_M, TotWeight * PUWeight );

					if( Flag_PassEff == kTRUE)
					{
                  for (int i=0; i<12; i++) {
                     for (int j=0; j<10; j++) {
                        bool isPassEventSelection2 = analyzer->EventSelection_generic(MuonCollection, ntuple, &SelectedMuonCollection, HLTname, i, isoval[j]);
                        if (isPassEventSelection2) h_mass_EffPass[i][j]->Fill( gen_M, TotWeight * PUWeight );
                     }
                  }
					}

				} // -- End of if( Flag_PassAcc )

			} // -- End of if( GenFlag == kTRUE )

		} //End of event iteration

		f->cd();
		h_massOS->Write();
		h_massSS->Write();

		cout << "Total Sum of Weight: " << SumWeights << endl;
		cout << "\tSum of Weights of Separated Events: " << SumWeights_Separated << endl;

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

	f->cd();
	h_mass_tot->Write();

	h_mass_AccTotal->Write();
	h_mass_AccPass->Write();
	h_mass_EffTotal->Write();
   for (int i=0; i<12; i++) 
      for (int j=0; j<10; j++)
         h_mass_EffPass[i][j]->Write();	 

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


