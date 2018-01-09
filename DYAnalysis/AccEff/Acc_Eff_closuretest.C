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

#include <vector>

// -- for Rochester Muon momentum correction -- //
#include <Include/roccor.2016.v3/RoccoR.cc>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include <Include/DYAnalyzer.h>
#include <Include/tnp_weight.h>
#include <BkgEst/interface/defs.h>
#include <HIstuff/HFweight.h>

using namespace DYana;

// number of TnP variations
const int nweights = 211;

// tnp regions
int tnpreg(int ireg, float eta, int ivar) {
   if (fabs(eta)<1.2 && ireg==0) return ivar;
   else if (fabs(eta)>=1.2 && fabs(eta)<2.1 && ireg==1) return ivar;
   else if (fabs(eta)>=2.1 && fabs(eta)<2.4 && ireg==2) return ivar;
   else return 0;
}
int tnpregtrg(int ireg, float eta, int ivar) {
   if (eta>-2.4 && eta<=-2.1 && ireg==0) return ivar;
   else if (eta>-2.1 && eta<=-1.6 && ireg==1) return ivar;
   else if (eta>-1.6 && eta<=-1.2 && ireg==2) return ivar;
   else if (eta>-1.2 && eta<=-0.9 && ireg==3) return ivar;
   else if (eta>-0.9 && eta<=-0.6 && ireg==4) return ivar;
   else if (eta>-0.6 && eta<=-0.3 && ireg==5) return ivar;
   else if (eta>-0.3 && eta<=0 && ireg==6) return ivar;
   else if (eta>0 && eta<=0.3 && ireg==7) return ivar;
   else if (eta>0.3 && eta<=0.6 && ireg==8) return ivar;
   else if (eta>0.6 && eta<=0.9 && ireg==9) return ivar;
   else if (eta>0.9 && eta<=1.2 && ireg==10) return ivar;
   else if (eta>1.2 && eta<=1.6 && ireg==11) return ivar;
   else if (eta>1.6 && eta<=2.1 && ireg==12) return ivar;
   else if (eta>2.1 && eta<=2.4 && ireg==13) return ivar;
   else return 0;
}

static inline void loadBar(int x, int n, int r, int w);
void Acc_Eff_closuretest(Bool_t isCorrected = kFALSE, TString Sample = "Powheg", TString HLTname = "HLT_PAL3Mu12_v*", int run=0, bool doHFrew = true, HFweight::HFside rewmode = HFweight::HFside::both ) // run: 0=all, 1=pPb, 2=PbP

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

   TString srew("norew");
   if (doHFrew) {
      if (rewmode==HFweight::HFside::both) srew="rewboth";
      else if (rewmode==HFweight::HFside::plus) srew="rewplus";
      else if (rewmode==HFweight::HFside::minus) srew="rewminus";
      else if (rewmode==HFweight::HFside::Ntracks) srew="rewNtracks";
   }
	TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_closuretest_" + isApplyMomCorr + "_" + Sample + "_" + HLTname + "_" + Form("%d",run) + "_" + srew + ".root", "RECREATE");

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

   int nbinspt = 9;
   double ptbins[10] = {10,15,25,30,35,40,45,50,80,200};
   int nbinspt2 = 5;
   double ptbins2[6] = {10,15,25,40,80,200};

	TH1D *h_pt_EffTotal_0012 = new TH1D("h_pt_EffTotal_0012", "", nbinspt, ptbins);
	TH1D *h_pt_EffPass_0012 = new TH1D("h_pt_EffPass_0012", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffPass1_0012 = new TH1D("h_pt_EffPass1_0012", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffPass2_0012 = new TH1D("h_pt_EffPass2_0012", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffTotal_1221 = new TH1D("h_pt_EffTotal_1221", "", nbinspt, ptbins);
	TH1D *h_pt_EffPass_1221 = new TH1D("h_pt_EffPass_1221", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffPass1_1221 = new TH1D("h_pt_EffPass1_1221", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffPass2_1221 = new TH1D("h_pt_EffPass2_1221", "", nbinspt,ptbins);	 
	TH1D *h_pt_EffTotal_2124 = new TH1D("h_pt_EffTotal_2124", "", nbinspt2, ptbins2);
	TH1D *h_pt_EffPass_2124 = new TH1D("h_pt_EffPass_2124", "", nbinspt2,ptbins2);	 
	TH1D *h_pt_EffPass1_2124 = new TH1D("h_pt_EffPass1_2124", "", nbinspt2,ptbins2);	 
	TH1D *h_pt_EffPass2_2124 = new TH1D("h_pt_EffPass2_2124", "", nbinspt2,ptbins2);	 

	// -- After applying efficiency correction -- //
   TH1D* h_pt_EffPass_0012_Corr_tnp[nweights];
   TH1D* h_pt_EffPass_1221_Corr_tnp[nweights];
   TH1D* h_pt_EffPass_2124_Corr_tnp[nweights];
   for (int i=0; i<nweights; i++) {
      h_pt_EffPass_0012_Corr_tnp[i] = new TH1D(Form("h_pt_EffPass_0012_Corr_tnp%d",i), "", nbinspt,ptbins);
      h_pt_EffPass_1221_Corr_tnp[i] = new TH1D(Form("h_pt_EffPass_1221_Corr_tnp%d",i), "", nbinspt,ptbins);
      h_pt_EffPass_2124_Corr_tnp[i] = new TH1D(Form("h_pt_EffPass_2124_Corr_tnp%d",i), "", nbinspt2,ptbins2);
   }

	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";
	// -- Each ntuple directory & corresponding Tags -- //
		// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< SampleTag > STags;

   analyzer->SetupMCsamples_v20170830(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

   // initialise the HF reweighting tool
   HFweight hftool;

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
      // loop only on DYMuMu!
      if (!Tag[i_tup].Contains("DYMuMu")) continue;

      Bool_t doflip = (switcheta(STags[i_tup])<0);
      Int_t  flipsign = doflip ? -1 : 1;
      if (run==1 && doflip) continue;
      if (run==2 && !doflip) continue;
      analyzer->sign = flipsign;

		TStopwatch looptime;
		looptime.Start();

		cout << "\t<" << Tag[i_tup] << ">" << endl;
		TH1D* h_mass = new TH1D("h_mass_"+Tag[i_tup], "", 600, 0, 600);

		TChain *chain = new TChain("recoTree/DYTree");
      TString tmp = BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root";
      cout << tmp.Data() << endl;
      chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain, doflip );
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_HI();

      RoccoR  rmcor("Include/roccor.2016.v3/rcdata.2016.v3"); //directory path as input for now; initialize only once, contains all variations

		Bool_t isMC;
		Tag[i_tup] == "Data" ? isMC = kFALSE : isMC = kTRUE;

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
		Double_t norm = ( Xsec[i_tup] * lumi_part1 ) / (Double_t)nEvents[i_tup];
      if (doflip)
         norm = ( Xsec[i_tup] * lumi_part2 ) / (Double_t)nEvents[i_tup];

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
         if (doHFrew) PUWeight *= hftool.weight(ntuple->hiHF,rewmode); 

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
            // if( Flag_PassAcc == kTRUE ) 
            // {
            // }
            // else
            // {
            // } 	

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
               vector< Muon > Pass1MuonCollection;
               vector< Muon > Pass2MuonCollection;
               vector< Muon > TagMuonCollection;
               vector< int > PassIdx;
               vector< int > Pass1Idx;
               vector< int > Pass2Idx;
               vector< int > TagIdx;

               for(Int_t j=0; j<(int)MuonCollection.size(); j++)
               {
                  Muon mu = MuonCollection[j];
                  bool passeta = (fabs(mu.eta)<2.4);
                  bool passID = mu.isTightMuon();
                  bool passiso = (mu.relPFiso < 0.15);
                  bool passtrg = mu.isTrigMatched(ntuple, "HLT_PAL3Mu12_v*");
                  if(passeta) {
                     // matching 
                     int imatch = -1;
                     if (fabs(genlep1.eta-mu.eta)<0.02 && fabs(genlep1.phi-mu.phi)<0.02) imatch=0;
                     else if (fabs(genlep2.eta-mu.eta)<0.02 && fabs(genlep2.phi-mu.phi)<0.02) imatch=1;

                     if (imatch<0) continue;
                     if (mu.Pt > 15 && passID && passiso && passtrg) {
                        TagMuonCollection.push_back( mu );
                        TagIdx.push_back(imatch);
                     }
                     if (mu.Pt > 10) {
                        if (passID) {
                           Pass1MuonCollection.push_back( mu );
                           Pass1Idx.push_back(imatch);
                           if (passtrg) {
                              Pass2MuonCollection.push_back( mu );
                              Pass2Idx.push_back(imatch);
                              if (passiso) {
                                 PassMuonCollection.push_back( mu );
                                 PassIdx.push_back(imatch);
                              }
                           }
                        }
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
                  bool pass1probe=false;
                  for (unsigned int k=0; k<Pass1MuonCollection.size(); k++) {
                     if (Pass1Idx[k] == probeidx) pass1probe=true;
                  }
                  bool pass2probe=false;
                  for (unsigned int k=0; k<Pass2MuonCollection.size(); k++) {
                     if (Pass2Idx[k] == probeidx) pass2probe=true;
                  }

                  double probeeta = GenLeptonCollection[probeidx].eta;
                  double probept = GenLeptonCollection[probeidx].Pt;

                  if (fabs(probeeta)<1.2) {
                     h_pt_EffTotal_0012->Fill(probept,TotWeight);
                     if (passprobe) h_pt_EffPass_0012->Fill(probept,TotWeight);
                     if (pass1probe) h_pt_EffPass1_0012->Fill(probept,TotWeight);
                     if (pass2probe) h_pt_EffPass2_0012->Fill(probept,TotWeight);
                  } else if (fabs(probeeta)<2.1) {
                     h_pt_EffTotal_1221->Fill(probept,TotWeight);
                     if (passprobe) h_pt_EffPass_1221->Fill(probept,TotWeight);
                     if (pass1probe) h_pt_EffPass1_1221->Fill(probept,TotWeight);
                     if (pass2probe) h_pt_EffPass2_1221->Fill(probept,TotWeight);
                  } else if (fabs(probeeta)<2.4) {
                     h_pt_EffTotal_2124->Fill(probept,TotWeight);
                     if (passprobe) h_pt_EffPass_2124->Fill(probept,TotWeight);
                     if (pass1probe) h_pt_EffPass1_2124->Fill(probept,TotWeight);
                     if (pass2probe) h_pt_EffPass2_2124->Fill(probept,TotWeight);
                  }

                  // TnP
                  for (int iwt=0; iwt<nweights; iwt++) {
                     int imuid=0,itrg=0,iiso=0; // nominal
                     if (iwt<=100) imuid = iwt; // muID stat
                     else if (iwt<=200) iiso = iwt-100; // iso stat
                     else if (iwt<=202) itrg = iwt-200; // trg stat
                     else if (iwt<=204) itrg = iwt-205; // trg syst
                     else if (iwt<=206) imuid = iwt-207; // muID syst
                     else if (iwt<=208) iiso = iwt-209; // iso syst
                     else if (iwt==209) imuid = -10; // iso syst
                     else if (iwt==210) iiso = -10; // iso syst

                     // weights for MuID and iso
                     TnpWeight = tnp_weight_muid_ppb(probept,probeeta,imuid)*tnp_weight_iso_ppb(probept,probeeta,iiso)*tnp_weight_trg_ppb(probeeta,itrg);

                     if (fabs(probeeta)<1.2) {
                        if (pass2probe) h_pt_EffPass_0012_Corr_tnp[iwt]->Fill(probept,TotWeight*TnpWeight);
                     } else if (fabs(probeeta)<2.1) {
                        if (pass2probe) h_pt_EffPass_1221_Corr_tnp[iwt]->Fill(probept,TotWeight*TnpWeight);
                     } else if (fabs(probeeta)<2.4) {
                        if (pass2probe) h_pt_EffPass_2124_Corr_tnp[iwt]->Fill(probept,TotWeight*TnpWeight);
                     }
                  } // tnp weight loop
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
   h_pt_EffPass1_0012->Write();
   h_pt_EffPass2_0012->Write();
   h_pt_EffTotal_1221->Write();
   h_pt_EffPass_1221->Write();
   h_pt_EffPass1_1221->Write();
   h_pt_EffPass2_1221->Write();
   h_pt_EffTotal_2124->Write();
   h_pt_EffPass_2124->Write();
   h_pt_EffPass1_2124->Write();
   h_pt_EffPass2_2124->Write();

   // total eff
   TEfficiency *teff0012 = new TEfficiency(*h_pt_EffPass_0012,*h_pt_EffTotal_0012);
   teff0012->SetName("eff0012");
   teff0012->Write();
   TEfficiency *teff1221 = new TEfficiency(*h_pt_EffPass_1221,*h_pt_EffTotal_1221);
   teff1221->SetName("eff1221");
   teff1221->Write();
   TEfficiency *teff2124 = new TEfficiency(*h_pt_EffPass_2124,*h_pt_EffTotal_2124);
   teff2124->SetName("eff2124");
   teff2124->Write();

   // muon ID
   TEfficiency *teff10012 = new TEfficiency(*h_pt_EffPass1_0012,*h_pt_EffTotal_0012);
   teff10012->SetName("eff10012");
   teff10012->Write();
   TEfficiency *teff11221 = new TEfficiency(*h_pt_EffPass1_1221,*h_pt_EffTotal_1221);
   teff11221->SetName("eff11221");
   teff11221->Write();
   TEfficiency *teff12124 = new TEfficiency(*h_pt_EffPass1_2124,*h_pt_EffTotal_2124);
   teff12124->SetName("eff12124");
   teff12124->Write();

   // trigger
   TEfficiency *teff20012 = new TEfficiency(*h_pt_EffPass2_0012,*h_pt_EffPass1_0012);
   teff20012->SetName("eff20012");
   teff20012->Write();
   TEfficiency *teff21221 = new TEfficiency(*h_pt_EffPass2_1221,*h_pt_EffPass1_1221);
   teff21221->SetName("eff21221");
   teff21221->Write();
   TEfficiency *teff22124 = new TEfficiency(*h_pt_EffPass2_2124,*h_pt_EffPass1_2124);
   teff22124->SetName("eff22124");
   teff22124->Write();

   // iso
   TEfficiency *teff30012 = new TEfficiency(*h_pt_EffPass_0012,*h_pt_EffPass2_0012);
   teff30012->SetName("eff30012");
   teff30012->Write();
   TEfficiency *teff31221 = new TEfficiency(*h_pt_EffPass_1221,*h_pt_EffPass2_1221);
   teff31221->SetName("eff31221");
   teff31221->Write();
   TEfficiency *teff32124 = new TEfficiency(*h_pt_EffPass_2124,*h_pt_EffPass2_2124);
   teff32124->SetName("eff32124");
   teff32124->Write();

	// TEfficiency *AccEff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_AccTotal);
	// AccEff_Mass->SetName("TEff_AccEff_Mass");

   TGraphAsymmErrors *g_Eff_0012_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_1221_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_2124_Corr_tnp_stat = NULL;
   TEfficiency* Eff_0012_Corr_tnp[nweights];
   TEfficiency* Eff_1221_Corr_tnp[nweights];
   TEfficiency* Eff_2124_Corr_tnp[nweights];
   for (int i=0; i<nweights; i++) {
      Eff_0012_Corr_tnp[i] = new TEfficiency(*h_pt_EffPass_0012_Corr_tnp[i], *h_pt_EffTotal_0012);
      Eff_1221_Corr_tnp[i] = new TEfficiency(*h_pt_EffPass_1221_Corr_tnp[i], *h_pt_EffTotal_1221);
      Eff_2124_Corr_tnp[i] = new TEfficiency(*h_pt_EffPass_2124_Corr_tnp[i], *h_pt_EffTotal_2124);
      Eff_0012_Corr_tnp[i]->SetName(Form("TEff_Eff_0012_Corr_tnp%d",i));
      Eff_1221_Corr_tnp[i]->SetName(Form("TEff_Eff_1221_Corr_tnp%d",i));
      Eff_2124_Corr_tnp[i]->SetName(Form("TEff_Eff_2124_Corr_tnp%d",i));
      Eff_0012_Corr_tnp[i]->Write();
      Eff_1221_Corr_tnp[i]->Write();
      Eff_2124_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_0012_Corr_tnp = new TCanvas("c_Eff_0012_Corr_tnp", "", 800, 600);
         c_Eff_0012_Corr_tnp->cd();
         Eff_0012_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_0012_Corr_tnp->Write();
         g_Eff_0012_Corr_tnp_stat = Eff_0012_Corr_tnp[i]->GetPaintedGraph();
         TCanvas *c_Eff_1221_Corr_tnp = new TCanvas("c_Eff_1221_Corr_tnp", "", 800, 600);
         c_Eff_1221_Corr_tnp->cd();
         Eff_1221_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_1221_Corr_tnp->Write();
         g_Eff_1221_Corr_tnp_stat = Eff_1221_Corr_tnp[i]->GetPaintedGraph();
         TCanvas *c_Eff_2124_Corr_tnp = new TCanvas("c_Eff_2124_Corr_tnp", "", 800, 600);
         c_Eff_2124_Corr_tnp->cd();
         Eff_2124_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_2124_Corr_tnp->Write();
         g_Eff_2124_Corr_tnp_stat = Eff_2124_Corr_tnp[i]->GetPaintedGraph();
      }
   }

   // now let's also write 2 TGRaphAsymmErrors for the efficiency including tnp corrections:
   // one with tnp uncertainties, and the other with total (stat MC + tnp syst) uncertainties
   // ************************
   // *       0 -- 1.2       *
   // ************************
   TGraphAsymmErrors *g_Eff_0012_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_0012_Corr_tnp_stat->Clone("g_Eff_0012_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_0012_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_0012_Corr_tnp_stat->Clone("g_Eff_0012_Corr_tnp_tot");
   for (int ibin=0; ibin<nbinspt; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_0012_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_0012_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      tnpstat1 = max(fabs(ee[201]->GetEfficiency(ibin+1)-e0),fabs(ee[202]->GetEfficiency(ibin+1)-e0));
      // muID
      for (int ix=1; ix<=100; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=101; ix<=200; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 = max(fabs(ee[203]->GetEfficiency(ibin+1)-e0),fabs(ee[204]->GetEfficiency(ibin+1)-e0));
      // muID
      tnpsyst2 = max(fabs(ee[205]->GetEfficiency(ibin+1)-e0),fabs(ee[206]->GetEfficiency(ibin+1)-e0));
      // iso
      tnpsyst3 = max(fabs(ee[207]->GetEfficiency(ibin+1)-e0),fabs(ee[208]->GetEfficiency(ibin+1)-e0));
      // muID binned
      tnpsyst4 = fabs(ee[209]->GetEfficiency(ibin+1)-e0);
      // iso binned
      tnpsyst5 = fabs(ee[210]->GetEfficiency(ibin+1)-e0);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_0012_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_0012_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_0012_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_0012_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_0012_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_0012_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_0012_Corr_tnp_stat->Write();
   g_Eff_0012_Corr_tnp_syst->Write();
   g_Eff_0012_Corr_tnp_tot->Write();

   // ************************
   // *     1.2 -- 2.1       *
   // ************************
   TGraphAsymmErrors *g_Eff_1221_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_1221_Corr_tnp_stat->Clone("g_Eff_1221_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_1221_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_1221_Corr_tnp_stat->Clone("g_Eff_1221_Corr_tnp_tot");
   for (int ibin=0; ibin<nbinspt; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_1221_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_1221_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      tnpstat1 = max(fabs(ee[201]->GetEfficiency(ibin+1)-e0),fabs(ee[202]->GetEfficiency(ibin+1)-e0));
      // muID
      for (int ix=1; ix<=100; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=101; ix<=200; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 = max(fabs(ee[203]->GetEfficiency(ibin+1)-e0),fabs(ee[204]->GetEfficiency(ibin+1)-e0));
      // muID
      tnpsyst2 = max(fabs(ee[205]->GetEfficiency(ibin+1)-e0),fabs(ee[206]->GetEfficiency(ibin+1)-e0));
      // iso
      tnpsyst3 = max(fabs(ee[207]->GetEfficiency(ibin+1)-e0),fabs(ee[208]->GetEfficiency(ibin+1)-e0));
      // muID binned
      tnpsyst4 = fabs(ee[209]->GetEfficiency(ibin+1)-e0);
      // iso binned
      tnpsyst5 = fabs(ee[210]->GetEfficiency(ibin+1)-e0);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_1221_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_1221_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_1221_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_1221_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_1221_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_1221_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_1221_Corr_tnp_stat->Write();
   g_Eff_1221_Corr_tnp_syst->Write();
   g_Eff_1221_Corr_tnp_tot->Write();

   // ************************
   // *     2.1 -- 2.4       *
   // ************************
   TGraphAsymmErrors *g_Eff_2124_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_2124_Corr_tnp_stat->Clone("g_Eff_2124_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_2124_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_2124_Corr_tnp_stat->Clone("g_Eff_2124_Corr_tnp_tot");
   for (int ibin=0; ibin<nbinspt2; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_2124_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_2124_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      tnpstat1 = max(fabs(ee[201]->GetEfficiency(ibin+1)-e0),fabs(ee[202]->GetEfficiency(ibin+1)-e0));
      // muID
      for (int ix=1; ix<=100; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=101; ix<=200; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 = max(fabs(ee[203]->GetEfficiency(ibin+1)-e0),fabs(ee[204]->GetEfficiency(ibin+1)-e0));
      // muID
      tnpsyst2 = max(fabs(ee[205]->GetEfficiency(ibin+1)-e0),fabs(ee[206]->GetEfficiency(ibin+1)-e0));
      // iso
      tnpsyst3 = max(fabs(ee[207]->GetEfficiency(ibin+1)-e0),fabs(ee[208]->GetEfficiency(ibin+1)-e0));
      // muID binned
      tnpsyst4 = fabs(ee[209]->GetEfficiency(ibin+1)-e0);
      // iso binned
      tnpsyst5 = fabs(ee[210]->GetEfficiency(ibin+1)-e0);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_2124_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_2124_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_2124_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_2124_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_2124_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_2124_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_2124_Corr_tnp_stat->Write();
   g_Eff_2124_Corr_tnp_syst->Write();
   g_Eff_2124_Corr_tnp_tot->Write();

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


