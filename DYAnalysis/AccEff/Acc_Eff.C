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

// -- for Rochester Muon momentum correction -- //
#include "../Include/roccor.2016.v3/RoccoR.cc"

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "../Include/DYAnalyzer.h"
#include "../Include/tnp_weight.h"
#include "../BkgEst/interface/defs.h"
#include "../HIstuff/HFweight.h"

using namespace DYana;

// number of TnP variations
const int nweights = 645;

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
void Acc_Eff(Bool_t isCorrected = kFALSE, TString Sample = "Powheg", TString HLTname = "PAL3Mu12", int run=0, bool doHFrew = true, HFweight::HFside rewmode = HFweight::HFside::both, int cor_s=0, int cor_m=0, bool zptrew = true ) // run: 0=all, 1=pPb, 2=PbP
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

	TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_" + isApplyMomCorr + "_" + Sample + "_" + HLTname + "_" + Form("%d",run) + "_" + srew + srew2 + ".root", "RECREATE");

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

 	TH1D *h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", binnum, bins);
	TH1D *h_mass_AccPass = new TH1D("h_mass_AccPass", "", binnum, bins);
 	TH1D *h_mass3bins_AccTotal = new TH1D("h_mass3bins_AccTotal", "", binnum3, bins3);
	TH1D *h_mass3bins_AccPass = new TH1D("h_mass3bins_AccPass", "", binnum3, bins3);
 	TH1D *h_pt_AccTotal = new TH1D("h_pt_AccTotal", "", ptbinnum_meas, ptbin_meas);
	TH1D *h_pt_AccPass = new TH1D("h_pt_AccPass", "", ptbinnum_meas, ptbin_meas);
 	TH1D *h_phistar_AccTotal = new TH1D("h_phistar_AccTotal", "", phistarnum, phistarbin);
	TH1D *h_phistar_AccPass = new TH1D("h_phistar_AccPass", "", phistarnum, phistarbin);
 	TH1D *h_rap1560_AccTotal = new TH1D("h_rap1560_AccTotal", "", rapbinnum_1560, rapbin_1560);
	TH1D *h_rap1560_AccPass = new TH1D("h_rap1560_AccPass", "", rapbinnum_1560, rapbin_1560);
 	TH1D *h_rap60120_AccTotal = new TH1D("h_rap60120_AccTotal", "", rapbinnum_60120, rapbin_60120);
	TH1D *h_rap60120_AccPass = new TH1D("h_rap60120_AccPass", "", rapbinnum_60120, rapbin_60120);
 	TH1D *h_pt1560_AccTotal = new TH1D("h_pt1560_AccTotal", "", ptbinnum_meas_1560, ptbin_meas_1560);
	TH1D *h_pt1560_AccPass = new TH1D("h_pt1560_AccPass", "", ptbinnum_meas_1560, ptbin_meas_1560);
 	TH1D *h_phistar1560_AccTotal = new TH1D("h_phistar1560_AccTotal", "", phistarnum_1560, phistarbin_1560);
	TH1D *h_phistar1560_AccPass = new TH1D("h_phistar1560_AccPass", "", phistarnum_1560, phistarbin_1560);

   TH1D *h_mass_EffPass = new TH1D("h_mass_EffPass", "", binnum, bins);	 
   TH1D *h_mass_EffTotal = new TH1D("h_mass_EffTotal", "", binnum, bins);
   TH1D *h_mass3bins_EffPass = new TH1D("h_mass3bins_EffPass", "", binnum3, bins3);	 
   TH1D *h_mass3bins_EffTotal = new TH1D("h_mass3bins_EffTotal", "", binnum3, bins3);
   TH1D *h_pt_EffPass = new TH1D("h_pt_EffPass", "", ptbinnum_meas, ptbin_meas);	 
   TH1D *h_pt_EffTotal = new TH1D("h_pt_EffTotal", "", ptbinnum_meas, ptbin_meas);
   TH1D *h_phistar_EffPass = new TH1D("h_phistar_EffPass", "", phistarnum, phistarbin);	 
   TH1D *h_phistar_EffTotal = new TH1D("h_phistar_EffTotal", "", phistarnum, phistarbin);
   TH1D *h_rap1560_EffPass = new TH1D("h_rap1560_EffPass", "", rapbinnum_1560, rapbin_1560);	 
   TH1D *h_rap1560_EffTotal = new TH1D("h_rap1560_EffTotal", "", rapbinnum_1560, rapbin_1560);
   TH1D *h_rap60120_EffPass = new TH1D("h_rap60120_EffPass", "", rapbinnum_60120, rapbin_60120);	 
   TH1D *h_rap60120_EffTotal = new TH1D("h_rap60120_EffTotal", "", rapbinnum_60120, rapbin_60120);
   TH1D *h_pt1560_EffPass = new TH1D("h_pt1560_EffPass", "", ptbinnum_meas_1560, ptbin_meas_1560);	 
   TH1D *h_pt1560_EffTotal = new TH1D("h_pt1560_EffTotal", "", ptbinnum_meas_1560, ptbin_meas_1560);
   TH1D *h_phistar1560_EffPass = new TH1D("h_phistar1560_EffPass", "", phistarnum_1560, phistarbin_1560);	 
   TH1D *h_phistar1560_EffTotal = new TH1D("h_phistar1560_EffTotal", "", phistarnum_1560, phistarbin_1560);

	// -- After applying efficiency correction -- //
   TH1D* h_mass_EffPass_Corr_tnp[nweights];
   TH1D* h_mass3bins_EffPass_Corr_tnp[nweights];
   TH1D* h_pt_EffPass_Corr_tnp[nweights];
   TH1D* h_phistar_EffPass_Corr_tnp[nweights];
   TH1D* h_rap1560_EffPass_Corr_tnp[nweights];
   TH1D* h_rap60120_EffPass_Corr_tnp[nweights];
   TH1D* h_pt1560_EffPass_Corr_tnp[nweights];
   TH1D* h_phistar1560_EffPass_Corr_tnp[nweights];
   for (int i=0; i<nweights; i++) {
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

   // analyzer->SetupMCsamples_v20180111(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);
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
		chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/ntuple_*.root");
		
		NtupleHandle *ntuple = new NtupleHandle( chain, doflip );
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_HI();

      RoccoR  rmcor("Include/roccor.2016.v3/rcdata.2016.v3"); //directory path as input for now; initialize only once, contains all variations
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
				Double_t gen_Rap = (genlep1.Momentum + genlep2.Momentum).Rapidity()-rapshift;
				Double_t gen_Pt = (genlep1.Momentum + genlep2.Momentum).Pt();
				Double_t gen_Phistar = Object::phistar(genlep1,genlep2);

            // -- Z pt reweighting -- //
            if (zptrew) TotWeight *= zptWeight(gen_Pt);

				// -- Flags -- //
				Bool_t Flag_PassAcc = kFALSE;
				Bool_t Flag_PassAccEff = kFALSE;

				// -- Fill the mass histograms -- //
				h_mass->Fill( gen_M, TotWeight );
				h_mass_tot->Fill( gen_M, TotWeight );

				Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

            // -- Acceptance Calculation -- //
            h_mass_AccTotal->Fill( gen_M, TotWeight );
            h_mass3bins_AccTotal->Fill( gen_M, TotWeight );
            if (gen_M>60 && gen_M<120) {
               h_pt_AccTotal->Fill( gen_Pt, TotWeight );
               h_phistar_AccTotal->Fill( gen_Phistar, TotWeight );
               h_rap60120_AccTotal->Fill( gen_Rap, TotWeight );
            } else if (gen_M>15 && gen_M<60) {
               h_rap1560_AccTotal->Fill( gen_Rap, TotWeight );
               h_pt1560_AccTotal->Fill( gen_Pt, TotWeight );
               h_phistar1560_AccTotal->Fill( gen_Phistar, TotWeight );
            }
				if( Flag_PassAcc == kTRUE ) 
				{
					h_mass_AccPass->Fill( gen_M, TotWeight );
					h_mass3bins_AccPass->Fill( gen_M, TotWeight );
               if (gen_M>60 && gen_M<120) {
                  h_pt_AccPass->Fill( gen_Pt, TotWeight );
                  h_phistar_AccPass->Fill( gen_Phistar, TotWeight );
                  h_rap60120_AccPass->Fill( gen_Rap, TotWeight );
               } else if (gen_M>15 && gen_M<60) {
                  h_rap1560_AccPass->Fill( gen_Rap, TotWeight );
                  h_pt1560_AccPass->Fill( gen_Pt, TotWeight );
                  h_phistar1560_AccPass->Fill( gen_Phistar, TotWeight );
               }
				}

				Double_t TnpWeight = 1.; // -- Efficiency correction factor -- //

				// -- Calculate the efficiency among the events passing acceptance condition -- //
				if( Flag_PassAcc == kTRUE )
				{
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
                  // isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection, true);

						if( isPassEventSelection == kTRUE && 
                        SelectedMuonCollection.size()>=2 && SelectedMuonCollection[0].charge != SelectedMuonCollection[1].charge )
						{

							Flag_PassEff = kTRUE;
							Flag_PassAccEff = kTRUE;
						}

					} // -- End of if( ntuple->isTriggered( HLT ) ) -- //

					// -- Efficiency Calculation -- //
               h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );
               h_mass3bins_EffTotal->Fill( gen_M, TotWeight * PUWeight );
               if (gen_M>60 && gen_M<120) {
                  h_pt_EffTotal->Fill( gen_Pt, TotWeight * PUWeight );
                  h_phistar_EffTotal->Fill( gen_Phistar, TotWeight * PUWeight );
                  h_rap60120_EffTotal->Fill( gen_Rap, TotWeight * PUWeight );
               } else if (gen_M>15 && gen_M<60) {
                  h_rap1560_EffTotal->Fill( gen_Rap, TotWeight * PUWeight );
                  h_pt1560_EffTotal->Fill( gen_Pt, TotWeight * PUWeight );
                  h_phistar1560_EffTotal->Fill( gen_Phistar, TotWeight * PUWeight );
               }
					if( Flag_PassEff == kTRUE)
					{
                  double reco_M = (SelectedMuonCollection[0].Momentum+SelectedMuonCollection[1].Momentum).M();
                  h_mass_EffPass->Fill( gen_M, TotWeight * PUWeight );
                  h_mass3bins_EffPass->Fill( gen_M, TotWeight * PUWeight );
                  if (reco_M>60 && reco_M<120) {
                     h_pt_EffPass->Fill( gen_Pt, TotWeight * PUWeight );
                     h_phistar_EffPass->Fill( gen_Phistar, TotWeight * PUWeight );
                     h_rap60120_EffPass->Fill( gen_Rap, TotWeight * PUWeight );
                  } else if (reco_M>15 &&reco_M<60) {
                     h_rap1560_EffPass->Fill( gen_Rap, TotWeight * PUWeight );
                     h_pt1560_EffPass->Fill( gen_Pt, TotWeight * PUWeight );
                     h_phistar1560_EffPass->Fill( gen_Phistar, TotWeight * PUWeight );
                  }

                  // TnP
                  double pt1 = SelectedMuonCollection[0].Pt;
                  double pt2 = SelectedMuonCollection[1].Pt;
                  double eta1 = analyzer->sign*SelectedMuonCollection[0].eta;
                  double eta2 = analyzer->sign*SelectedMuonCollection[1].eta;
                  double aeta1 = fabs(eta1);
                  double aeta2 = fabs(eta2);

                  for (int iwt=0; iwt<nweights; iwt++) {
                     int imuid1=0,itrg1=0,iiso1=0; // nominal
                     int imuid2=0,itrg2=0,iiso2=0; // nominal
                     if (iwt<=100) {imuid1 = tnpreg(0,eta1,iwt); imuid2 = tnpreg(0,eta2,iwt);} // muID stat
                     else if (iwt<=200) {imuid1 = tnpreg(1,eta1,iwt-100); imuid2 = tnpreg(1,eta2,iwt-100);} // muID stat
                     else if (iwt<=300) {imuid1 = tnpreg(2,eta1,iwt-200); imuid2 = tnpreg(2,eta2,iwt-200);} // muID stat
                     else if (iwt<=400) {iiso1 = tnpreg(0,eta1,iwt-300); iiso2 = tnpreg(0,eta2,iwt-300);} // iso stat
                     else if (iwt<=500) {iiso1 = tnpreg(1,eta1,iwt-400); iiso2 = tnpreg(1,eta2,iwt-400);} // iso stat
                     else if (iwt<=600) {iiso1 = tnpreg(2,eta1,iwt-500); iiso2 = tnpreg(2,eta2,iwt-500);} // iso stat
                     else if (iwt<=614) {itrg1 = tnpregtrg(614-iwt,eta1,1); itrg2 = tnpregtrg(614-iwt,eta2,1);} // trg stat
                     else if (iwt<=628) {itrg1 = tnpregtrg(628-iwt,eta1,2); itrg2 = tnpregtrg(628-iwt,eta2,2);} // trg stat
                     else if (iwt<=630) {itrg1 = iwt-631;} // trg syst
                     else if (iwt<=632) {itrg2 = iwt-633;} // trg syst
                     else if (iwt<=634) {imuid1 = iwt-635;} // muID syst
                     else if (iwt<=636) {imuid2 = iwt-637;} // muID syst
                     else if (iwt<=638) {iiso1 = iwt-639;} // iso syst
                     else if (iwt<=640) {iiso2 = iwt-641;} // iso syst
                     else if (iwt==641) {imuid1 = -10;} // muid syst
                     else if (iwt==642) {imuid2 = -10;} // muid syst
                     else if (iwt==643) {iiso1 = -10;} // iso syst
                     else if (iwt==644) {iiso2 = -10;} // iso syst

                     // weights for MuID and iso
                     // L1DoubleMuOpen uses relPF iso
                     if (HLTname.Contains("L1DoubleMu")) {
                        TnpWeight = tnp_weight_muid_ppb(pt1,eta1,imuid1)*tnp_weight_iso_ppb(pt1,aeta1,iiso1)
                           *tnp_weight_muid_ppb(pt2,eta2,imuid2)*tnp_weight_iso_ppb(pt2,aeta2,iiso2);
                     } else if (HLTname.Contains("L3Mu12")) {
                        // L3Mu12 uses rel tk iso
                        TnpWeight = tnp_weight_muid_ppb(pt1,eta1,imuid1)*tnp_weight_isotk_ppb(pt1,aeta1,iiso1)
                           *tnp_weight_muid_ppb(pt2,eta2,imuid2)*tnp_weight_isotk_ppb(pt2,aeta2,iiso2);
                     } else {
                        cerr << "ERROR trigger should be L1DoubleMuOpen or L3Mu12" << endl;
                        TnpWeight = 1.;
                     }

                     // add trg... careful in the case of L3Mu12!
                     double sf_trg=1.;
                     if (HLTname.Contains("L3Mu12")) {
                        if (pt2>=15. && pt1>=15.) { // both muons could trigger
                           double eff_data = (1 - (1 - tnp_weight_L3Mu12_ppb(eta1,200)*tnp_weight_L3Mu12_ppb(eta1,itrg1)/tnp_weight_L3Mu12_ppb(eta1,0)) * (1 - tnp_weight_L3Mu12_ppb(eta2,200)*tnp_weight_L3Mu12_ppb(eta2,itrg2)/tnp_weight_L3Mu12_ppb(eta2,0)) );
                           double eff_mc = (1 - (1 - tnp_weight_L3Mu12_ppb(eta1,300)) * (1 - tnp_weight_L3Mu12_ppb(eta2,300)) );
                           sf_trg = eff_data/eff_mc;
                        } else if (pt1<15) {
                           sf_trg = tnp_weight_L3Mu12_ppb(eta2,itrg2);
                        } else if (pt2<15) {
                           sf_trg = tnp_weight_L3Mu12_ppb(eta1,itrg1);
                        }
                     } else if (HLTname.Contains("L1DoubleMuOpen")) { // L1DoubleMuOpen case
                        sf_trg = tnp_weight_L1DMOpen_ppb(eta1,itrg1) * tnp_weight_L1DMOpen_ppb(eta2,itrg2);
                     }
                     else { // L1DoubleMu0
                        sf_trg = tnp_weight_L1DM0_ppb(eta1,itrg1) * tnp_weight_L1DM0_ppb(eta2,itrg2);
                     }
                     TnpWeight = TnpWeight * sf_trg;

                     h_mass_EffPass_Corr_tnp[iwt]->Fill( gen_M, TotWeight * PUWeight * TnpWeight );
                     h_mass3bins_EffPass_Corr_tnp[iwt]->Fill( gen_M, TotWeight * PUWeight * TnpWeight );
                     if (reco_M>60 && reco_M<120) {
                        h_pt_EffPass_Corr_tnp[iwt]->Fill( gen_Pt, TotWeight * PUWeight * TnpWeight );
                        h_phistar_EffPass_Corr_tnp[iwt]->Fill( gen_Phistar, TotWeight * PUWeight * TnpWeight );
                        h_rap60120_EffPass_Corr_tnp[iwt]->Fill( gen_Rap, TotWeight * PUWeight * TnpWeight );
                     } else if (reco_M>15 && reco_M<60) {
                        h_rap1560_EffPass_Corr_tnp[iwt]->Fill( gen_Rap, TotWeight * PUWeight * TnpWeight );
                        h_pt1560_EffPass_Corr_tnp[iwt]->Fill( gen_Pt, TotWeight * PUWeight * TnpWeight );
                        h_phistar1560_EffPass_Corr_tnp[iwt]->Fill( gen_Phistar, TotWeight * PUWeight * TnpWeight );
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

	h_mass_AccTotal->Write();
	h_mass_AccPass->Write();
	h_mass3bins_AccTotal->Write();
	h_mass3bins_AccPass->Write();
	h_pt_AccTotal->Write();
	h_phistar_AccTotal->Write();
	h_pt_AccPass->Write();
	h_phistar_AccPass->Write();
	h_rap1560_AccTotal->Write();
	h_rap1560_AccPass->Write();
	h_rap60120_AccTotal->Write();
	h_rap60120_AccPass->Write();
	h_pt1560_AccTotal->Write();
	h_pt1560_AccPass->Write();
	h_phistar1560_AccTotal->Write();
	h_phistar1560_AccPass->Write();

	h_mass_EffTotal->Write();
	h_mass_EffPass->Write();
	h_mass3bins_EffTotal->Write();
	h_mass3bins_EffPass->Write();
	h_pt_EffTotal->Write();
	h_pt_EffPass->Write();
	h_phistar_EffTotal->Write();
	h_phistar_EffPass->Write();
	h_rap1560_EffTotal->Write();
	h_rap1560_EffPass->Write();
	h_rap60120_EffTotal->Write();
	h_rap60120_EffPass->Write();
   h_pt1560_EffTotal->Write();
   h_pt1560_EffPass->Write();
	h_phistar1560_EffTotal->Write();
	h_phistar1560_EffPass->Write();
	for (int i=0; i<nweights; i++) {
      h_mass_EffPass_Corr_tnp[i]->Write();
      h_mass3bins_EffPass_Corr_tnp[i]->Write();
      h_pt_EffPass_Corr_tnp[i]->Write();
      h_phistar_EffPass_Corr_tnp[i]->Write();
      h_rap1560_EffPass_Corr_tnp[i]->Write();
      h_rap60120_EffPass_Corr_tnp[i]->Write();
      h_pt1560_EffPass_Corr_tnp[i]->Write();
      h_phistar1560_EffPass_Corr_tnp[i]->Write();
   }

	TEfficiency *Acc_Mass = new TEfficiency(*h_mass_AccPass, *h_mass_AccTotal);
	Acc_Mass->SetName("TEff_Acc_Mass");
	TEfficiency *Acc_Mass3bins = new TEfficiency(*h_mass3bins_AccPass, *h_mass3bins_AccTotal);
	Acc_Mass3bins->SetName("TEff_Acc_Mass3bins");
	TEfficiency *Acc_Pt = new TEfficiency(*h_pt_AccPass, *h_pt_AccTotal);
	Acc_Pt->SetName("TEff_Acc_Pt");
	TEfficiency *Acc_Phistar = new TEfficiency(*h_phistar_AccPass, *h_phistar_AccTotal);
	Acc_Phistar->SetName("TEff_Acc_Phistar");
	TEfficiency *Acc_Rap1560 = new TEfficiency(*h_rap1560_AccPass, *h_rap1560_AccTotal);
	Acc_Rap1560->SetName("TEff_Acc_Rap1560");
	TEfficiency *Acc_Rap60120 = new TEfficiency(*h_rap60120_AccPass, *h_rap60120_AccTotal);
	Acc_Rap60120->SetName("TEff_Acc_Rap60120");
	TEfficiency *Acc_Pt1560 = new TEfficiency(*h_pt1560_AccPass, *h_pt1560_AccTotal);
	Acc_Pt1560->SetName("TEff_Acc_Pt1560");
	TEfficiency *Acc_Phistar1560 = new TEfficiency(*h_phistar1560_AccPass, *h_phistar1560_AccTotal);
	Acc_Phistar1560->SetName("TEff_Acc_Phistar1560");

	TEfficiency *Eff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_EffTotal);
	Eff_Mass->SetName("TEff_Eff_Mass");
	TEfficiency *Eff_Mass3bins = new TEfficiency(*h_mass3bins_EffPass, *h_mass3bins_EffTotal);
	Eff_Mass3bins->SetName("TEff_Eff_Mass3bins");
	TEfficiency *Eff_Pt = new TEfficiency(*h_pt_EffPass, *h_pt_EffTotal);
	Eff_Pt->SetName("TEff_Eff_Pt");
	TEfficiency *Eff_Phistar = new TEfficiency(*h_phistar_EffPass, *h_phistar_EffTotal);
	Eff_Phistar->SetName("TEff_Eff_Phistar");
	TEfficiency *Eff_Rap1560 = new TEfficiency(*h_rap1560_EffPass, *h_rap1560_EffTotal);
	Eff_Rap1560->SetName("TEff_Eff_Rap1560");
	TEfficiency *Eff_Rap60120 = new TEfficiency(*h_rap60120_EffPass, *h_rap60120_EffTotal);
	Eff_Rap60120->SetName("TEff_Eff_Rap60120");
	TEfficiency *Eff_Pt1560 = new TEfficiency(*h_pt1560_EffPass, *h_pt1560_EffTotal);
	Eff_Pt1560->SetName("TEff_Eff_Pt1560");
	TEfficiency *Eff_Phistar1560 = new TEfficiency(*h_phistar1560_EffPass, *h_phistar1560_EffTotal);
	Eff_Phistar1560->SetName("TEff_Eff_Phistar1560");

	// TEfficiency *AccEff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_AccTotal);
	// AccEff_Mass->SetName("TEff_AccEff_Mass");

   TGraphAsymmErrors *g_Eff_Mass_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_Mass3bins_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_Pt_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_Phistar_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_Rap1560_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_Rap60120_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_Pt1560_Corr_tnp_stat = NULL;
   TGraphAsymmErrors *g_Eff_Phistar1560_Corr_tnp_stat = NULL;
   TEfficiency* Eff_Mass_Corr_tnp[nweights];
   TEfficiency* Eff_Mass3bins_Corr_tnp[nweights];
   TEfficiency* Eff_Pt_Corr_tnp[nweights];
   TEfficiency* Eff_Phistar_Corr_tnp[nweights];
   TEfficiency* Eff_Rap1560_Corr_tnp[nweights];
   TEfficiency* Eff_Rap60120_Corr_tnp[nweights];
   TEfficiency* Eff_Pt1560_Corr_tnp[nweights];
   TEfficiency* Eff_Phistar1560_Corr_tnp[nweights];
   for (int i=0; i<nweights; i++) {
      // mass
      Eff_Mass_Corr_tnp[i] = new TEfficiency(*h_mass_EffPass_Corr_tnp[i], *h_mass_EffTotal);
      Eff_Mass_Corr_tnp[i]->SetName(Form("TEff_Eff_Mass_Corr_tnp%d",i));
      Eff_Mass_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Mass_Corr_tnp = new TCanvas("c_Eff_Mass_Corr_tnp", "", 800, 600);
         c_Eff_Mass_Corr_tnp->cd();
         Eff_Mass_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Mass_Corr_tnp->Write();
         g_Eff_Mass_Corr_tnp_stat = Eff_Mass_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Mass_Corr_tnp_stat->SetName("g_Eff_Mass_Corr_tnp_stat");
      }

      // mass3bins
      Eff_Mass3bins_Corr_tnp[i] = new TEfficiency(*h_mass3bins_EffPass_Corr_tnp[i], *h_mass3bins_EffTotal);
      Eff_Mass3bins_Corr_tnp[i]->SetName(Form("TEff_Eff_Mass3bins_Corr_tnp%d",i));
      Eff_Mass3bins_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Mass3bins_Corr_tnp = new TCanvas("c_Eff_Mass3bins_Corr_tnp", "", 800, 600);
         c_Eff_Mass3bins_Corr_tnp->cd();
         Eff_Mass3bins_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Mass3bins_Corr_tnp->Write();
         g_Eff_Mass3bins_Corr_tnp_stat = Eff_Mass3bins_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Mass3bins_Corr_tnp_stat->SetName("g_Eff_Mass3bins_Corr_tnp_stat");
      }

      // pt
      Eff_Pt_Corr_tnp[i] = new TEfficiency(*h_pt_EffPass_Corr_tnp[i], *h_pt_EffTotal);
      Eff_Pt_Corr_tnp[i]->SetName(Form("TEff_Eff_Pt_Corr_tnp%d",i));
      Eff_Pt_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Pt_Corr_tnp = new TCanvas("c_Eff_Pt_Corr_tnp", "", 800, 600);
         c_Eff_Pt_Corr_tnp->cd();
         Eff_Pt_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Pt_Corr_tnp->Write();
         g_Eff_Pt_Corr_tnp_stat = Eff_Pt_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Pt_Corr_tnp_stat->SetName("g_Eff_Pt_Corr_tnp_stat");
      }

      // phistar
      Eff_Phistar_Corr_tnp[i] = new TEfficiency(*h_phistar_EffPass_Corr_tnp[i], *h_phistar_EffTotal);
      Eff_Phistar_Corr_tnp[i]->SetName(Form("TEff_Eff_Phistar_Corr_tnp%d",i));
      Eff_Phistar_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Phistar_Corr_tnp = new TCanvas("c_Eff_Phistar_Corr_tnp", "", 800, 600);
         c_Eff_Phistar_Corr_tnp->cd();
         Eff_Phistar_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Phistar_Corr_tnp->Write();
         g_Eff_Phistar_Corr_tnp_stat = Eff_Phistar_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Phistar_Corr_tnp_stat->SetName("g_Eff_Phistar_Corr_tnp_stat");
      }

      // rap 15-60
      Eff_Rap1560_Corr_tnp[i] = new TEfficiency(*h_rap1560_EffPass_Corr_tnp[i], *h_rap1560_EffTotal);
      Eff_Rap1560_Corr_tnp[i]->SetName(Form("TEff_Eff_Rap1560_Corr_tnp%d",i));
      Eff_Rap1560_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Rap1560_Corr_tnp = new TCanvas("c_Eff_Rap1560_Corr_tnp", "", 800, 600);
         c_Eff_Rap1560_Corr_tnp->cd();
         Eff_Rap1560_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Rap1560_Corr_tnp->Write();
         g_Eff_Rap1560_Corr_tnp_stat = Eff_Rap1560_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Rap1560_Corr_tnp_stat->SetName("g_Eff_Rap1560_Corr_tnp_stat");
      }

      // rap 60-120
      Eff_Rap60120_Corr_tnp[i] = new TEfficiency(*h_rap60120_EffPass_Corr_tnp[i], *h_rap60120_EffTotal);
      Eff_Rap60120_Corr_tnp[i]->SetName(Form("TEff_Eff_Rap60120_Corr_tnp%d",i));
      Eff_Rap60120_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Rap60120_Corr_tnp = new TCanvas("c_Eff_Rap60120_Corr_tnp", "", 800, 600);
         c_Eff_Rap60120_Corr_tnp->cd();
         Eff_Rap60120_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Rap60120_Corr_tnp->Write();
         g_Eff_Rap60120_Corr_tnp_stat = Eff_Rap60120_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Rap60120_Corr_tnp_stat->SetName("g_Eff_Rap60120_Corr_tnp_stat");
      }

      // pt 15-60
      Eff_Pt1560_Corr_tnp[i] = new TEfficiency(*h_pt1560_EffPass_Corr_tnp[i], *h_pt1560_EffTotal);
      Eff_Pt1560_Corr_tnp[i]->SetName(Form("TEff_Eff_Pt1560_Corr_tnp%d",i));
      Eff_Pt1560_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Pt1560_Corr_tnp = new TCanvas("c_Eff_Pt1560_Corr_tnp", "", 800, 600);
         c_Eff_Pt1560_Corr_tnp->cd();
         Eff_Pt1560_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Pt1560_Corr_tnp->Write();
         g_Eff_Pt1560_Corr_tnp_stat = Eff_Pt1560_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Pt1560_Corr_tnp_stat->SetName("g_Eff_Pt1560_Corr_tnp_stat");
      }

      // phistar 15-60
      Eff_Phistar1560_Corr_tnp[i] = new TEfficiency(*h_phistar1560_EffPass_Corr_tnp[i], *h_phistar1560_EffTotal);
      Eff_Phistar1560_Corr_tnp[i]->SetName(Form("TEff_Eff_Phistar1560_Corr_tnp%d",i));
      Eff_Phistar1560_Corr_tnp[i]->Write();

      if (i==0) {
         TCanvas *c_Eff_Phistar1560_Corr_tnp = new TCanvas("c_Eff_Phistar1560_Corr_tnp", "", 800, 600);
         c_Eff_Phistar1560_Corr_tnp->cd();
         Eff_Phistar1560_Corr_tnp[i]->Draw("AP");
         gPad->Update();
         c_Eff_Phistar1560_Corr_tnp->Write();
         g_Eff_Phistar1560_Corr_tnp_stat = Eff_Phistar1560_Corr_tnp[i]->GetPaintedGraph();
         g_Eff_Phistar1560_Corr_tnp_stat->SetName("g_Eff_Phistar1560_Corr_tnp_stat");
      }
   }

   // now let's also write 2 TGRaphAsymmErrors for the efficiency including tnp corrections:
   // one with tnp uncertainties, and the other with total (stat MC + tnp syst) uncertainties
   TGraphAsymmErrors *g_Eff_Mass_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Mass_Corr_tnp_stat->Clone("g_Eff_Mass_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Mass_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Mass_Corr_tnp_stat->Clone("g_Eff_Mass_Corr_tnp_tot");
   TGraphAsymmErrors *g_Eff_Mass3bins_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Mass3bins_Corr_tnp_stat->Clone("g_Eff_Mass3bins_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Mass3bins_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Mass3bins_Corr_tnp_stat->Clone("g_Eff_Mass3bins_Corr_tnp_tot");
   TGraphAsymmErrors *g_Eff_Pt_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Pt_Corr_tnp_stat->Clone("g_Eff_Pt_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Pt_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Pt_Corr_tnp_stat->Clone("g_Eff_Pt_Corr_tnp_tot");
   TGraphAsymmErrors *g_Eff_Phistar_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Phistar_Corr_tnp_stat->Clone("g_Eff_Phistar_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Phistar_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Phistar_Corr_tnp_stat->Clone("g_Eff_Phistar_Corr_tnp_tot");
   TGraphAsymmErrors *g_Eff_Rap1560_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Rap1560_Corr_tnp_stat->Clone("g_Eff_Rap1560_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Rap1560_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Rap1560_Corr_tnp_stat->Clone("g_Eff_Rap1560_Corr_tnp_tot");
   TGraphAsymmErrors *g_Eff_Rap60120_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Rap60120_Corr_tnp_stat->Clone("g_Eff_Rap60120_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Rap60120_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Rap60120_Corr_tnp_stat->Clone("g_Eff_Rap60120_Corr_tnp_tot");
   TGraphAsymmErrors *g_Eff_Pt1560_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Pt1560_Corr_tnp_stat->Clone("g_Eff_Pt1560_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Pt1560_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Pt1560_Corr_tnp_stat->Clone("g_Eff_Pt1560_Corr_tnp_tot");
   TGraphAsymmErrors *g_Eff_Phistar1560_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Phistar1560_Corr_tnp_stat->Clone("g_Eff_Phistar1560_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Phistar1560_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Phistar1560_Corr_tnp_stat->Clone("g_Eff_Phistar1560_Corr_tnp_tot");

   // mass
   for (int ibin=0; ibin<binnum; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = sqrt(2)*0.34e-2, tnpsyst7 = sqrt(2)*0.6e-2;
      double e0 = Eff_Mass_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Mass_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Mass_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Mass_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Mass_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Mass_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Mass_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Mass_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Mass_Corr_tnp_stat->Write();
   g_Eff_Mass_Corr_tnp_syst->Write();
   g_Eff_Mass_Corr_tnp_tot->Write();

   cout << endl;

   // mass3bins
   for (int ibin=0; ibin<binnum3; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = sqrt(2)*0.34e-2, tnpsyst7 = sqrt(2)*0.6e-2;
      double e0 = Eff_Mass3bins_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Mass3bins_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Mass3bins_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Mass3bins_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Mass3bins_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Mass3bins_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Mass3bins_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Mass3bins_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Mass3bins_Corr_tnp_stat->Write();
   g_Eff_Mass3bins_Corr_tnp_syst->Write();
   g_Eff_Mass3bins_Corr_tnp_tot->Write();

   cout << endl;

   // pt
   for (int ibin=0; ibin<ptbinnum_meas; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_Pt_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Pt_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      // for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      // there is a super weird bug... dirty-fix it
      for (int ix=1; ix<=300; ix++) {
         double eix = ee[ix]->GetEfficiency(ibin+1);
         if (eix<1e-9) eix = ee[ix-1]->GetEfficiency(ibin+1);
         tnpstat2 += pow(eix-e0,2);
      }
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Pt_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Pt_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Pt_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Pt_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Pt_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Pt_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Pt_Corr_tnp_stat->Write();
   g_Eff_Pt_Corr_tnp_syst->Write();
   g_Eff_Pt_Corr_tnp_tot->Write();

   cout << endl;

   // phistar
   for (int ibin=0; ibin<phistarnum; ibin++) {
      // compute the tnp syst uncertainty
      double tnphistarot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_Phistar_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Phistar_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnphistarot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnphistarot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Phistar_Corr_tnp_syst->SetPointEYlow(ibin,tnphistarot);
      g_Eff_Phistar_Corr_tnp_syst->SetPointEYhigh(ibin,tnphistarot);
      g_Eff_Phistar_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnphistarot,2)+pow(Eff_Phistar_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Phistar_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnphistarot,2)+pow(Eff_Phistar_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Phistar_Corr_tnp_stat->Write();
   g_Eff_Phistar_Corr_tnp_syst->Write();
   g_Eff_Phistar_Corr_tnp_tot->Write();

   cout << endl;

   // rap 15-60
   for (int ibin=0; ibin<rapbinnum_1560; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_Rap1560_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Rap1560_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Rap1560_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Rap1560_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Rap1560_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Rap1560_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Rap1560_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Rap1560_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Rap1560_Corr_tnp_stat->Write();
   g_Eff_Rap1560_Corr_tnp_syst->Write();
   g_Eff_Rap1560_Corr_tnp_tot->Write();

   cout << endl;

   // rap 60-120
   for (int ibin=0; ibin<rapbinnum_60120; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_Rap60120_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Rap60120_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Rap60120_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Rap60120_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Rap60120_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Rap60120_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Rap60120_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Rap60120_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Rap60120_Corr_tnp_stat->Write();
   g_Eff_Rap60120_Corr_tnp_syst->Write();
   g_Eff_Rap60120_Corr_tnp_tot->Write();

   // pt 15-60
   for (int ibin=0; ibin<ptbinnum_meas_1560; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_Pt1560_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Pt1560_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Pt1560_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Pt1560_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Pt1560_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Pt1560_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Pt1560_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Pt1560_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Pt1560_Corr_tnp_stat->Write();
   g_Eff_Pt1560_Corr_tnp_syst->Write();
   g_Eff_Pt1560_Corr_tnp_tot->Write();

   cout << endl;

   // phistar 15-60
   for (int ibin=0; ibin<phistarnum_1560; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_Phistar1560_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Phistar1560_Corr_tnp; // shortcut

      // stat uncertainties
      // trigger
      for (int i=1; i<=14; i++) {
         tnpstat1 += pow(max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0)),2);
      }
      tnpstat1 = sqrt(tnpstat1);
      // muID
      for (int ix=1; ix<=300; ix++) tnpstat2 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat2 = sqrt(tnpstat2/100.);
      // iso
      for (int ix=301; ix<=600; ix++) tnpstat3 += pow(ee[ix]->GetEfficiency(ibin+1)-e0,2);
      tnpstat3 = sqrt(tnpstat3/100.);

      // syst uncertainties
      // trigger
      tnpsyst1 += pow(max(fabs(ee[629]->GetEfficiency(ibin+1)-e0),fabs(ee[630]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 += pow(max(fabs(ee[631]->GetEfficiency(ibin+1)-e0),fabs(ee[632]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst1 = sqrt(tnpsyst1);
      // muID
      tnpsyst2 += pow(max(fabs(ee[633]->GetEfficiency(ibin+1)-e0),fabs(ee[634]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 += pow(max(fabs(ee[635]->GetEfficiency(ibin+1)-e0),fabs(ee[636]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst2 = sqrt(tnpsyst2);
      // iso
      tnpsyst3 += pow(max(fabs(ee[637]->GetEfficiency(ibin+1)-e0),fabs(ee[638]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 += pow(max(fabs(ee[639]->GetEfficiency(ibin+1)-e0),fabs(ee[640]->GetEfficiency(ibin+1)-e0)),2);
      tnpsyst3 = sqrt(tnpsyst3);
      // muID binned
      tnpsyst4 += pow(fabs(ee[641]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 += pow(fabs(ee[642]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst4 = sqrt(tnpsyst4);
      // iso binned
      tnpsyst5 += pow(fabs(ee[643]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 += pow(fabs(ee[644]->GetEfficiency(ibin+1)-e0),2);
      tnpsyst5 = sqrt(tnpsyst5);

      // total
      tnptot = sqrt(pow(tnpstat1,2)+pow(tnpstat2,2)+pow(tnpstat3,2)
            +pow(tnpsyst1,2)+pow(tnpsyst2,2)+pow(tnpsyst3,2)+pow(tnpsyst4,2)+pow(tnpsyst5,2)+pow(tnpsyst6,2)+pow(tnpsyst7,2));
      cout << tnptot << "<-" << tnpstat1 << "," << tnpstat2 << "," << tnpstat3 << ";"
         << tnpsyst1 << "," << tnpsyst2 << "," << tnpsyst3 << ","  << tnpsyst4 << "," << tnpsyst5 << "," << tnpsyst6 << "," << tnpsyst7 << endl;

      // assign it to the graphs
      g_Eff_Phistar1560_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Phistar1560_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Phistar1560_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Phistar1560_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Phistar1560_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Phistar1560_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Phistar1560_Corr_tnp_stat->Write();
   g_Eff_Phistar1560_Corr_tnp_syst->Write();
   g_Eff_Phistar1560_Corr_tnp_tot->Write();

   cout << endl;

	Acc_Mass->Write();
	Eff_Mass->Write();
	Acc_Mass3bins->Write();
	Eff_Mass3bins->Write();
	Acc_Pt->Write();
	Eff_Pt->Write();
	Acc_Phistar->Write();
	Eff_Phistar->Write();
	Acc_Rap1560->Write();
	Eff_Rap1560->Write();
	Acc_Rap60120->Write();
	Eff_Rap60120->Write();
	Acc_Pt1560->Write();
	Eff_Pt1560->Write();
	Acc_Phistar1560->Write();
	Eff_Phistar1560->Write();

   // mass
	TCanvas *c_Acc_Mass = new TCanvas("c_Acc_Mass", "", 800, 600);
	c_Acc_Mass->cd();
	Acc_Mass->Draw("AP");
	c_Acc_Mass->Write();

	TCanvas *c_Eff_Mass = new TCanvas("c_Eff_Mass", "", 800, 600);
	c_Eff_Mass->cd();
	Eff_Mass->Draw("AP");
	c_Eff_Mass->Write();

   // mass3bins
	TCanvas *c_Acc_Mass3bins = new TCanvas("c_Acc_Mass3bins", "", 800, 600);
	c_Acc_Mass3bins->cd();
	Acc_Mass3bins->Draw("AP");
	c_Acc_Mass3bins->Write();

	TCanvas *c_Eff_Mass3bins = new TCanvas("c_Eff_Mass3bins", "", 800, 600);
	c_Eff_Mass3bins->cd();
	Eff_Mass3bins->Draw("AP");
	c_Eff_Mass3bins->Write();

   // pt
	TCanvas *c_Acc_Pt = new TCanvas("c_Acc_Pt", "", 800, 600);
	c_Acc_Pt->cd();
	Acc_Pt->Draw("AP");
	c_Acc_Pt->Write();

	TCanvas *c_Eff_Pt = new TCanvas("c_Eff_Pt", "", 800, 600);
	c_Eff_Pt->cd();
	Eff_Pt->Draw("AP");
	c_Eff_Pt->Write();

   // phistar
	TCanvas *c_Acc_Phistar = new TCanvas("c_Acc_Phistar", "", 800, 600);
	c_Acc_Phistar->cd();
	Acc_Phistar->Draw("AP");
	c_Acc_Phistar->Write();

	TCanvas *c_Eff_Phistar = new TCanvas("c_Eff_Phistar", "", 800, 600);
	c_Eff_Phistar->cd();
	Eff_Phistar->Draw("AP");
	c_Eff_Phistar->Write();

   // rap 15-60
	TCanvas *c_Acc_Rap1560 = new TCanvas("c_Acc_Rap1560", "", 800, 600);
	c_Acc_Rap1560->cd();
	Acc_Rap1560->Draw("AP");
	c_Acc_Rap1560->Write();

	TCanvas *c_Eff_Rap1560 = new TCanvas("c_Eff_Rap1560", "", 800, 600);
	c_Eff_Rap1560->cd();
	Eff_Rap1560->Draw("AP");
	c_Eff_Rap1560->Write();

   // rap 60-120
	TCanvas *c_Acc_Rap60120 = new TCanvas("c_Acc_Rap60120", "", 800, 600);
	c_Acc_Rap60120->cd();
	Acc_Rap60120->Draw("AP");
	c_Acc_Rap60120->Write();

	TCanvas *c_Eff_Rap60120 = new TCanvas("c_Eff_Rap60120", "", 800, 600);
	c_Eff_Rap60120->cd();
	Eff_Rap60120->Draw("AP");
	c_Eff_Rap60120->Write();

   // pt 15-60
	TCanvas *c_Acc_Pt1560 = new TCanvas("c_Acc_Pt1560", "", 800, 600);
	c_Acc_Pt1560->cd();
	Acc_Pt1560->Draw("AP");
	c_Acc_Pt1560->Write();

	TCanvas *c_Eff_Pt1560 = new TCanvas("c_Eff_Pt1560", "", 800, 600);
	c_Eff_Pt1560->cd();
	Eff_Pt1560->Draw("AP");
	c_Eff_Pt1560->Write();

   // phistar 15-60
	TCanvas *c_Acc_Phistar1560 = new TCanvas("c_Acc_Phistar1560", "", 800, 600);
	c_Acc_Phistar1560->cd();
	Acc_Phistar1560->Draw("AP");
	c_Acc_Phistar1560->Write();

	TCanvas *c_Eff_Phistar1560 = new TCanvas("c_Eff_Phistar1560", "", 800, 600);
	c_Eff_Phistar1560->cd();
	Eff_Phistar1560->Draw("AP");
	c_Eff_Phistar1560->Write();


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


