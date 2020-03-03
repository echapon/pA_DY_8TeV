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
#include "../Include/roccor.2016.v3/RoccoR.cc"

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "../Include/DYAnalyzer.h"
#include "../Include/tnp_weight.h"
#include "../BkgEst/interface/defs.h"
#include "../HIstuff/HFweight.h"

using namespace DYana;

// number of gen weights
const int nweights = 325;

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
void Acc_Eff_weights(Bool_t isCorrected = kFALSE, TString Sample = "Powheg", TString HLTname = "PAL3Mu12", int run=0, bool doHFrew = true, HFweight::HFside rewmode = HFweight::HFside::both, int cor_s=0, int cor_m=0, bool zptrew = true ) // run: 0=all, 1=pPb, 2=PbP
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Sample: " << Sample << endl;

	TString isApplyMomCorr = "";
	if( isCorrected == kTRUE )
	{
		cout << "Apply Roochester Muon Momentum Correction..." << endl;
      cout << "Using s = " << cor_s << ", m = " << cor_m << endl;
		isApplyMomCorr = Form("MomCorr%d%d",cor_s,cor_m);
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
   TString srew2("");
   if (!zptrew) srew2 = "_noZptrew";

	TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_weights_" + isApplyMomCorr + "_" + Sample + "_" + HLTname + "_" + Form("%d",run) + "_" + srew + srew2 + ".root", "RECREATE");

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

   TH1D* h_mass_AccTotal[nweights];
   TH1D* h_mass3bins_AccTotal[nweights];
   TH1D* h_pt_AccTotal[nweights];
   TH1D* h_phistar_AccTotal[nweights];
   TH1D* h_rap1560_AccTotal[nweights];
   TH1D* h_rap60120_AccTotal[nweights];
   TH1D* h_pt1560_AccTotal[nweights];
   TH1D* h_phistar1560_AccTotal[nweights];
   TH1D* h_mass_AccPass[nweights];
   TH1D* h_mass3bins_AccPass[nweights];
   TH1D* h_pt_AccPass[nweights];
   TH1D* h_phistar_AccPass[nweights];
   TH1D* h_rap1560_AccPass[nweights];
   TH1D* h_rap60120_AccPass[nweights];
   TH1D* h_pt1560_AccPass[nweights];
   TH1D* h_phistar1560_AccPass[nweights];
   
   TH1D* h_mass_EffTotal[nweights];
   TH1D* h_mass3bins_EffTotal[nweights];
   TH1D* h_pt_EffTotal[nweights];
   TH1D* h_phistar_EffTotal[nweights];
   TH1D* h_rap1560_EffTotal[nweights];
   TH1D* h_rap60120_EffTotal[nweights];
   TH1D* h_pt1560_EffTotal[nweights];
   TH1D* h_phistar1560_EffTotal[nweights];
   TH1D* h_mass_EffPass[nweights];
   TH1D* h_mass3bins_EffPass[nweights];
   TH1D* h_pt_EffPass[nweights];
   TH1D* h_phistar_EffPass[nweights];
   TH1D* h_rap1560_EffPass[nweights];
   TH1D* h_rap60120_EffPass[nweights];
   TH1D* h_pt1560_EffPass[nweights];
   TH1D* h_phistar1560_EffPass[nweights];

   TH1D* h_mass_EffPass_Corr_tnp[nweights];
   TH1D* h_mass3bins_EffPass_Corr_tnp[nweights];
   TH1D* h_pt_EffPass_Corr_tnp[nweights];
   TH1D* h_phistar_EffPass_Corr_tnp[nweights];
   TH1D* h_rap1560_EffPass_Corr_tnp[nweights];
   TH1D* h_rap60120_EffPass_Corr_tnp[nweights];
   TH1D* h_pt1560_EffPass_Corr_tnp[nweights];
   TH1D* h_phistar1560_EffPass_Corr_tnp[nweights];
   for (int i=0; i<nweights; i++) {
      h_mass_AccTotal[i] = new TH1D(Form("h_mass_AccTotal%d",i), "", binnum, bins);
      h_mass_AccPass[i] = new TH1D(Form("h_mass_AccPass%d",i), "", binnum, bins);
      h_mass3bins_AccTotal[i] = new TH1D(Form("h_mass3bins_AccTotal%d",i), "", binnum3, bins3);
      h_mass3bins_AccPass[i] = new TH1D(Form("h_mass3bins_AccPass%d",i), "", binnum3, bins3);
      h_pt_AccTotal[i] = new TH1D(Form("h_pt_AccTotal%d",i), "", ptbinnum_meas, ptbin_meas);
      h_pt_AccPass[i] = new TH1D(Form("h_pt_AccPass%d",i), "", ptbinnum_meas, ptbin_meas);
      h_phistar_AccTotal[i] = new TH1D(Form("h_phistar_AccTotal%d",i), "", phistarnum, phistarbin);
      h_phistar_AccPass[i] = new TH1D(Form("h_phistar_AccPass%d",i), "", phistarnum, phistarbin);
      h_rap1560_AccTotal[i] = new TH1D(Form("h_rap1560_AccTotal%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap1560_AccPass[i] = new TH1D(Form("h_rap1560_AccPass%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap60120_AccTotal[i] = new TH1D(Form("h_rap60120_AccTotal%d",i), "", rapbinnum_60120, rapbin_60120);
      h_rap60120_AccPass[i] = new TH1D(Form("h_rap60120_AccPass%d",i), "", rapbinnum_60120, rapbin_60120);
      h_pt1560_AccTotal[i] = new TH1D(Form("h_pt1560_AccTotal%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_pt1560_AccPass[i] = new TH1D(Form("h_pt1560_AccPass%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_phistar1560_AccTotal[i] = new TH1D(Form("h_phistar1560_AccTotal%d",i), "", phistarnum_1560, phistarbin_1560);
      h_phistar1560_AccPass[i] = new TH1D(Form("h_phistar1560_AccPass%d",i), "", phistarnum_1560, phistarbin_1560);

      h_mass_EffPass[i] = new TH1D(Form("h_mass_EffPass%d",i), "", binnum, bins);	 
      h_mass_EffTotal[i] = new TH1D(Form("h_mass_EffTotal%d",i), "", binnum, bins);
      h_mass3bins_EffPass[i] = new TH1D(Form("h_mass3bins_EffPass%d",i), "", binnum3, bins3);	 
      h_mass3bins_EffTotal[i] = new TH1D(Form("h_mass3bins_EffTotal%d",i), "", binnum3, bins3);
      h_pt_EffPass[i] = new TH1D(Form("h_pt_EffPass%d",i), "", ptbinnum_meas, ptbin_meas);	 
      h_pt_EffTotal[i] = new TH1D(Form("h_pt_EffTotal%d",i), "", ptbinnum_meas, ptbin_meas);
      h_phistar_EffPass[i] = new TH1D(Form("h_phistar_EffPass%d",i), "", phistarnum, phistarbin);	 
      h_phistar_EffTotal[i] = new TH1D(Form("h_phistar_EffTotal%d",i), "", phistarnum, phistarbin);
      h_rap1560_EffPass[i] = new TH1D(Form("h_rap1560_EffPass%d",i), "", rapbinnum_1560, rapbin_1560);	 
      h_rap1560_EffTotal[i] = new TH1D(Form("h_rap1560_EffTotal%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap60120_EffPass[i] = new TH1D(Form("h_rap60120_EffPass%d",i), "", rapbinnum_60120, rapbin_60120);	 
      h_rap60120_EffTotal[i] = new TH1D(Form("h_rap60120_EffTotal%d",i), "", rapbinnum_60120, rapbin_60120);
      h_pt1560_EffPass[i] = new TH1D(Form("h_pt1560_EffPass%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);	 
      h_pt1560_EffTotal[i] = new TH1D(Form("h_pt1560_EffTotal%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_phistar1560_EffPass[i] = new TH1D(Form("h_phistar1560_EffPass%d",i), "", phistarnum_1560, phistarbin_1560);	 
      h_phistar1560_EffTotal[i] = new TH1D(Form("h_phistar1560_EffTotal%d",i), "", phistarnum_1560, phistarbin_1560);

      h_mass_EffPass_Corr_tnp[i] = new TH1D(Form("h_mass_EffPass_Corr_tnp%d",i), "", binnum, bins);
      h_mass3bins_EffPass_Corr_tnp[i] = new TH1D(Form("h_mass3bins_EffPass_Corr_tnp%d",i), "", binnum3, bins3);
      h_pt_EffPass_Corr_tnp[i] = new TH1D(Form("h_pt_EffPass_Corr_tnp%d",i), "", ptbinnum_meas, ptbin_meas);
      h_phistar_EffPass_Corr_tnp[i] = new TH1D(Form("h_phistar_EffPass_Corr_tnp%d",i), "", phistarnum, phistarbin);
      h_rap1560_EffPass_Corr_tnp[i] = new TH1D(Form("h_rap1560_EffPass_Corr_tnp%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap60120_EffPass_Corr_tnp[i] = new TH1D(Form("h_rap60120_EffPass_Corr_tnp%d",i), "", rapbinnum_60120, rapbin_60120);
      h_pt1560_EffPass_Corr_tnp[i] = new TH1D(Form("h_pt1560_EffPass_Corr_tnp%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_phistar1560_EffPass_Corr_tnp[i] = new TH1D(Form("h_phistar1560_EffPass_Corr_tnp%d",i), "", phistarnum_1560, phistarbin_1560);
   }

	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";
	// -- Each ntuple directory & corresponding Tags -- //
		// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< SampleTag > STags;

   analyzer->SetupMCsamples_v20180814(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

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
		cout << "\t" << ntupleDirectory[i_tup] << endl;
		TH1D* h_mass = new TH1D("h_mass_"+Tag[i_tup], "", 600, 0, 600);

		TChain *chain = new TChain("recoTree/DYTree");
		TChain *chainGen = new TChain("hiEvtAna/HiTree");
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		chainGen->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain, doflip );
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_GenOthers();
		ntuple->TurnOnBranches_HI();

      vector<float>   *ttbar_w = 0;
      TBranch        *b_ttbar_w;   //!
      chainGen->SetBranchAddress("ttbar_w", &ttbar_w, &b_ttbar_w);
      chainGen->SetBranchStatus("*",0);
      chainGen->SetBranchStatus("ttbar_w",1);

      RoccoR  rmcor("Include/roccor.2016.v3/rcdata.2016.v3"); //directory path as input for now; initialize only once, contains all variations

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
         chainGen->GetEntry(i);

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
            Double_t gen_Rap = (genlep1.Momentum + genlep2.Momentum).Rapidity()-rapshift;
            Double_t gen_Pt = (genlep1.Momentum + genlep2.Momentum).Pt();
            Double_t gen_Phistar = Object::phistar(genlep1,genlep2);

            // -- Z pt reweighting -- //
            if (zptrew) TotWeight *= zptWeight(gen_Pt,gen_M);

            // -- Flags -- //
            Bool_t Flag_PassAcc = kFALSE;
            Bool_t Flag_PassAccEff = kFALSE;

            // -- Fill the mass histograms -- //
            h_mass->Fill( gen_M, TotWeight );
            h_mass_tot->Fill( gen_M, TotWeight );

            // compute pre-FSR quantities
            Double_t dRCut = 0.1;

            GenLepton genlep_postFSR1 = GenLeptonCollection[0];
            GenLepton genlep_preFSR1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
            vector< GenOthers > GenPhotonCollection1;
            analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_preFSR1, &GenPhotonCollection1);

            GenLepton genlep_postFSR2 = GenLeptonCollection[1];
            GenLepton genlep_preFSR2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
            vector< GenOthers > GenPhotonCollection2;
            analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_preFSR2, &GenPhotonCollection2);

            // Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);
            // we want the acceptanc correction to be PRE-FSR
            Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep_preFSR1, genlep_preFSR2);
            Double_t gen_M_preFSR = (genlep_preFSR1.Momentum + genlep_preFSR2.Momentum).M();
            Double_t gen_Rap_preFSR = (genlep_preFSR1.Momentum + genlep_preFSR2.Momentum).Rapidity()-rapshift;
               
            Bool_t Flag_PassTotal_preFSR = (gen_Rap_preFSR > rapbin_60120[0] && gen_Rap_preFSR<rapbin_60120[rapbinnum_60120] );

            // -- Acceptance Calculation -- //
            if (ttbar_w->size()!=nweights) cout << i << " -> " << ttbar_w->size() << " " << nweights << endl;
            for (unsigned int iwt=0; iwt<nweights; iwt++) {
               double wt = 1;
               // sometimes the last weight is missing... protect against this
               if (iwt<ttbar_w->size()) wt = ttbar_w->at(iwt)*TotWeight;
               else wt = (1./ttbar_w->at(iwt-1))*TotWeight;

               if (Flag_PassTotal_preFSR) {
                  h_mass_AccTotal[iwt]->Fill( gen_M, wt );
                  h_mass3bins_AccTotal[iwt]->Fill( gen_M, wt );
                  if (gen_M_preFSR>60 && gen_M_preFSR<120) {
                     h_pt_AccTotal[iwt]->Fill( gen_Pt, wt );
                     h_phistar_AccTotal[iwt]->Fill( gen_Phistar, wt );
                     h_rap60120_AccTotal[iwt]->Fill( gen_Rap, wt );
                  } else if (gen_M_preFSR>15 && gen_M_preFSR<60) {
                     h_rap1560_AccTotal[iwt]->Fill( gen_Rap, wt );
                     h_pt1560_AccTotal[iwt]->Fill( gen_Pt, wt );
                     h_phistar1560_AccTotal[iwt]->Fill( gen_Phistar, wt );
                  }
               }
               if( Flag_PassAcc == kTRUE ) 
               {
                  h_mass_AccPass[iwt]->Fill( gen_M, wt );
                  h_mass3bins_AccPass[iwt]->Fill( gen_M, wt );
                  if (gen_M_preFSR>60 && gen_M_preFSR<120) {
                     h_pt_AccPass[iwt]->Fill( gen_Pt, wt );
                     h_phistar_AccPass[iwt]->Fill( gen_Phistar, wt );
                     h_rap60120_AccPass[iwt]->Fill( gen_Rap, wt );
                  } else if (gen_M_preFSR>15 && gen_M_preFSR<60) {
                     h_rap1560_AccPass[iwt]->Fill( gen_Rap, wt );
                     h_pt1560_AccPass[iwt]->Fill( gen_Pt, wt );
                     h_phistar1560_AccPass[iwt]->Fill( gen_Phistar, wt );
                  }
               }
            }

            Double_t TnpWeight = 1.; // -- Efficiency correction factor -- //

            // -- Calculate the efficiency. It's OK if events do not pass the acceptance condition in the numerator -- //
            Bool_t Flag_PassEff = kFALSE;

            vector< Muon > SelectedMuonCollection;
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
                     int s=cor_s, m=cor_m;

                     if( Tag[i_tup] == "Data" )
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
               Bool_t isPassEventSelection = kFALSE;
               isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

               if( isPassEventSelection == kTRUE && 
                     SelectedMuonCollection.size()>=2 && SelectedMuonCollection[0].charge != SelectedMuonCollection[1].charge )
               {

                  Flag_PassEff = kTRUE;
                  Flag_PassAccEff = Flag_PassAcc;
               }

            } // -- End of if( ntuple->isTriggered( HLT ) ) -- //

            // -- Efficiency Calculation -- //
            if (Flag_PassAcc) {
               for (unsigned int iwt=0; iwt<nweights; iwt++) {
                  double wt = 1;
                  // sometimes the last weight is missing... protect against this
                  if (iwt<ttbar_w->size()) wt = ttbar_w->at(iwt)*TotWeight;
                  else wt = (1./ttbar_w->at(iwt-1))*TotWeight;

                  h_mass_EffTotal[iwt]->Fill( gen_M, wt * PUWeight );
                  h_mass3bins_EffTotal[iwt]->Fill( gen_M, wt * PUWeight );
                  if (gen_M_preFSR>60 && gen_M_preFSR<120) {
                     h_pt_EffTotal[iwt]->Fill( gen_Pt, wt * PUWeight );
                     h_phistar_EffTotal[iwt]->Fill( gen_Phistar, wt * PUWeight );
                     h_rap60120_EffTotal[iwt]->Fill( gen_Rap, wt * PUWeight );
                  } else if (gen_M_preFSR>15 && gen_M_preFSR<60) {
                     h_rap1560_EffTotal[iwt]->Fill( gen_Rap, wt * PUWeight );
                     h_pt1560_EffTotal[iwt]->Fill( gen_Pt, wt * PUWeight );
                     h_phistar1560_EffTotal[iwt]->Fill( gen_Phistar, wt * PUWeight );
                  }
               }
            }
            if( Flag_PassEff == kTRUE)
            {
               double reco_M = (SelectedMuonCollection[0].Momentum+SelectedMuonCollection[1].Momentum).M();
               for (unsigned int iwt=0; iwt<nweights; iwt++) {
                  double wt = 1;
                  // sometimes the last weight is missing... protect against this
                  if (iwt<ttbar_w->size()) wt = ttbar_w->at(iwt)*TotWeight;
                  else wt = (1./ttbar_w->at(iwt-1))*TotWeight;

                  h_mass_EffPass[iwt]->Fill( gen_M, wt * PUWeight );
                  h_mass3bins_EffPass[iwt]->Fill( gen_M, wt * PUWeight );
                  if (reco_M>60 && reco_M<120) {
                     h_pt_EffPass[iwt]->Fill( gen_Pt, wt * PUWeight );
                     h_phistar_EffPass[iwt]->Fill( gen_Phistar, wt * PUWeight );
                     h_rap60120_EffPass[iwt]->Fill( gen_Rap, wt * PUWeight );
                  } else if (reco_M>15 && reco_M<60) {
                     h_rap1560_EffPass[iwt]->Fill( gen_Rap, wt * PUWeight );
                     h_pt1560_EffPass[iwt]->Fill( gen_Pt, wt * PUWeight );
                     h_phistar1560_EffPass[iwt]->Fill( gen_Phistar, wt * PUWeight );
                  }
               }

               // TnP
               double pt1 = SelectedMuonCollection[0].Pt;
               double pt2 = SelectedMuonCollection[1].Pt;
               double eta1 = analyzer->sign*SelectedMuonCollection[0].eta;
               double eta2 = analyzer->sign*SelectedMuonCollection[1].eta;
               double aeta1 = fabs(eta1);
               double aeta2 = fabs(eta2);

               // weights for MuID and iso
               // L1DoubleMuOpen uses relPF iso
               if (HLTname.Contains("L1DoubleMu")) {
                  TnpWeight = tnp_weight_muid_ppb(pt1,eta1,0)*tnp_weight_iso_ppb(pt1,aeta1,0)
                     *tnp_weight_muid_ppb(pt2,eta2,0)*tnp_weight_iso_ppb(pt2,aeta2,0);
               } else if (HLTname.Contains("L3Mu12")) {
                  // L3Mu12 uses rel tk iso
                  TnpWeight = tnp_weight_muid_ppb(pt1,eta1,0)*tnp_weight_isotk_ppb(pt1,aeta1,0)
                     *tnp_weight_muid_ppb(pt2,eta2,0)*tnp_weight_isotk_ppb(pt2,aeta2,0);
               } else {
                  cerr << "ERROR trigger should be L1DoubleMuOpen or L3Mu12" << endl;
                  TnpWeight = 1.;
               }

               // add trg... careful!
               double sf_trg;
               if (pt2>=15. && pt1>=15.) { // both muons could trigger
                  double eff_data = (1 - (1 - tnp_weight_L3Mu12_ppb(eta1,200)*tnp_weight_L3Mu12_ppb(eta1,0)/tnp_weight_L3Mu12_ppb(eta1,0)) * (1 - tnp_weight_L3Mu12_ppb(eta2,200)*tnp_weight_L3Mu12_ppb(eta2,0)/tnp_weight_L3Mu12_ppb(eta2,0)) );
                  double eff_mc = (1 - (1 - tnp_weight_L3Mu12_ppb(eta1,300)) * (1 - tnp_weight_L3Mu12_ppb(eta2,300)) );
                  sf_trg = eff_data/eff_mc;
               } else if (pt1<15) {
                  sf_trg = tnp_weight_L3Mu12_ppb(eta2,0);
               } else if (pt2<15) {
                  sf_trg = tnp_weight_L3Mu12_ppb(eta1,0);
               }
               TnpWeight = TnpWeight * sf_trg;

               for (unsigned int iwt=0; iwt<nweights; iwt++) {
                  double wt = 1;
                  // sometimes the last weight is missing... protect against this
                  if (iwt<ttbar_w->size()) wt = ttbar_w->at(iwt)*TotWeight;
                  else wt = (1./ttbar_w->at(iwt-1))*TotWeight;

                  h_mass_EffPass_Corr_tnp[iwt]->Fill( gen_M, wt * PUWeight * TnpWeight );
                  h_mass3bins_EffPass_Corr_tnp[iwt]->Fill( gen_M, wt * PUWeight * TnpWeight );
                  if (reco_M>60 && reco_M<120) {
                     h_pt_EffPass_Corr_tnp[iwt]->Fill( gen_Pt, wt * PUWeight * TnpWeight );
                     h_phistar_EffPass_Corr_tnp[iwt]->Fill( gen_Phistar, wt * PUWeight * TnpWeight );
                     h_rap60120_EffPass_Corr_tnp[iwt]->Fill( gen_Rap, wt * PUWeight * TnpWeight );
                  } else if (reco_M>15 && reco_M<60) {
                     h_rap1560_EffPass_Corr_tnp[iwt]->Fill( gen_Rap, wt * PUWeight * TnpWeight );
                     h_pt1560_EffPass_Corr_tnp[iwt]->Fill( gen_Pt, wt * PUWeight * TnpWeight );
                     h_phistar1560_EffPass_Corr_tnp[iwt]->Fill( gen_Phistar, wt * PUWeight * TnpWeight );
                  }
               } 
            }


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

   for (int i=0; i<nweights; i++) {
      // cancel the effect of HF reweighting
      h_mass_EffPass[i]->Multiply(h_mass_AccPass[i]); h_mass_EffPass[i]->Divide(h_mass_EffTotal[i]);
      h_mass3bins_EffPass[i]->Multiply(h_mass3bins_AccPass[i]); h_mass3bins_EffPass[i]->Divide(h_mass3bins_EffTotal[i]);
      h_pt_EffPass[i]->Multiply(h_pt_AccPass[i]); h_pt_EffPass[i]->Divide(h_pt_EffTotal[i]);
      h_phistar_EffPass[i]->Multiply(h_phistar_AccPass[i]); h_phistar_EffPass[i]->Divide(h_phistar_EffTotal[i]);
      h_rap1560_EffPass[i]->Multiply(h_rap1560_AccPass[i]); h_rap1560_EffPass[i]->Divide(h_rap1560_EffTotal[i]);
      h_rap60120_EffPass[i]->Multiply(h_rap60120_AccPass[i]); h_rap60120_EffPass[i]->Divide(h_rap60120_EffTotal[i]);
      h_pt1560_EffPass[i]->Multiply(h_pt1560_AccPass[i]); h_pt1560_EffPass[i]->Divide(h_pt1560_EffTotal[i]);
      h_phistar1560_EffPass[i]->Multiply(h_phistar1560_AccPass[i]); h_phistar1560_EffPass[i]->Divide(h_phistar1560_EffTotal[i]);

      h_mass_EffPass_Corr_tnp[i]->Multiply(h_mass_AccPass[i]); h_mass_EffPass_Corr_tnp[i]->Divide(h_mass_EffTotal[i]);
      h_mass3bins_EffPass_Corr_tnp[i]->Multiply(h_mass3bins_AccPass[i]); h_mass3bins_EffPass_Corr_tnp[i]->Divide(h_mass3bins_EffTotal[i]);
      h_pt_EffPass_Corr_tnp[i]->Multiply(h_pt_AccPass[i]); h_pt_EffPass_Corr_tnp[i]->Divide(h_pt_EffTotal[i]);
      h_phistar_EffPass_Corr_tnp[i]->Multiply(h_phistar_AccPass[i]); h_phistar_EffPass_Corr_tnp[i]->Divide(h_phistar_EffTotal[i]);
      h_rap1560_EffPass_Corr_tnp[i]->Multiply(h_rap1560_AccPass[i]); h_rap1560_EffPass_Corr_tnp[i]->Divide(h_rap1560_EffTotal[i]);
      h_rap60120_EffPass_Corr_tnp[i]->Multiply(h_rap60120_AccPass[i]); h_rap60120_EffPass_Corr_tnp[i]->Divide(h_rap60120_EffTotal[i]);
      h_pt1560_EffPass_Corr_tnp[i]->Multiply(h_pt1560_AccPass[i]); h_pt1560_EffPass_Corr_tnp[i]->Divide(h_pt1560_EffTotal[i]);
      h_phistar1560_EffPass_Corr_tnp[i]->Multiply(h_phistar1560_AccPass[i]); h_phistar1560_EffPass_Corr_tnp[i]->Divide(h_phistar1560_EffTotal[i]);

      h_mass_AccTotal[i]->Write();
      h_mass_AccPass[i]->Write();
      h_mass3bins_AccTotal[i]->Write();
      h_mass3bins_AccPass[i]->Write();
      h_pt_AccTotal[i]->Write();
      h_phistar_AccTotal[i]->Write();
      h_pt_AccPass[i]->Write();
      h_phistar_AccPass[i]->Write();
      h_rap1560_AccTotal[i]->Write();
      h_rap1560_AccPass[i]->Write();
      h_rap60120_AccTotal[i]->Write();
      h_rap60120_AccPass[i]->Write();
      h_pt1560_AccTotal[i]->Write();
      h_pt1560_AccPass[i]->Write();
      h_phistar1560_AccTotal[i]->Write();
      h_phistar1560_AccPass[i]->Write();

      // substitute AccPass to EffTotal
      h_mass_EffTotal[i] = (TH1D*) h_mass_AccPass[i]->Clone(Form("h_mass_EffTotal%d",i));
      h_mass3bins_EffTotal[i] = (TH1D*) h_mass3bins_AccPass[i]->Clone(Form("h_mass3bins_EffTotal%d",i));
      h_pt_EffTotal[i] = (TH1D*) h_pt_AccPass[i]->Clone(Form("h_pt_EffTotal%d",i));
      h_phistar_EffTotal[i] = (TH1D*) h_phistar_AccPass[i]->Clone(Form("h_phistar_EffTotal%d",i));
      h_rap1560_EffTotal[i] = (TH1D*) h_rap1560_AccPass[i]->Clone(Form("h_rap1560_EffTotal%d",i));
      h_rap60120_EffTotal[i] = (TH1D*) h_rap60120_AccPass[i]->Clone(Form("h_rap60120_EffTotal%d",i));
      h_pt1560_EffTotal[i] = (TH1D*) h_pt1560_AccPass[i]->Clone(Form("h_pt1560_EffTotal%d",i));
      h_phistar1560_EffTotal[i] = (TH1D*) h_phistar1560_AccPass[i]->Clone(Form("h_phistar1560_EffTotal%d",i));

      h_mass_EffTotal[i]->Write();
      h_mass3bins_EffTotal[i]->Write();
      h_pt_EffTotal[i]->Write();
      h_phistar_EffTotal[i]->Write();
      h_rap1560_EffTotal[i]->Write();
      h_pt1560_EffTotal[i]->Write();
      h_phistar1560_EffTotal[i]->Write();
      h_rap60120_EffTotal[i]->Write();

      h_mass_EffPass[i]->Write();
      h_mass3bins_EffPass[i]->Write();
      h_pt_EffPass[i]->Write();
      h_phistar_EffPass[i]->Write();
      h_rap1560_EffPass[i]->Write();
      h_pt1560_EffPass[i]->Write();
      h_phistar1560_EffPass[i]->Write();
      h_rap60120_EffPass[i]->Write();

      h_mass_EffPass_Corr_tnp[i]->Write();
      h_mass3bins_EffPass_Corr_tnp[i]->Write();
      h_pt_EffPass_Corr_tnp[i]->Write();
      h_phistar_EffPass_Corr_tnp[i]->Write();
      h_rap1560_EffPass_Corr_tnp[i]->Write();
      h_rap60120_EffPass_Corr_tnp[i]->Write();
      h_pt1560_EffPass_Corr_tnp[i]->Write();
      h_phistar1560_EffPass_Corr_tnp[i]->Write();
   }



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


