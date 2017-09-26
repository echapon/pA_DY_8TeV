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
#include <Include/tnp_weight.h>
#include <BkgEst/interface/defs.h>
#include <HIstuff/HFweight.h>

using namespace DYana;

static inline void loadBar(int x, int n, int r, int w);
void Acc_Eff(Bool_t isCorrected = kFALSE, TString Sample = "Powheg", TString HLTname = "HLT_PAL3Mu12_v*", int run=0, bool doHFrew = true, HFweight::HFside rewmode = HFweight::HFside::both )
// run: 0=all, 1=pPb, 2=PbP
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
   }
	TFile *f = new TFile("ROOTFile_Histogram_Acc_Eff_" + Sample + "_" + HLTname + "_" + Form("%d",run) + "_" + srew + ".root", "RECREATE");

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

 	TH1D *h_mass_AccTotal = new TH1D("h_mass_AccTotal", "", binnum, bins);
	TH1D *h_mass_AccPass = new TH1D("h_mass_AccPass", "", binnum, bins);

   TH1D *h_mass_EffPass = new TH1D("h_mass_EffPass", "", binnum, bins);	 
   TH1D *h_mass_EffTotal = new TH1D("h_mass_EffTotal", "", binnum, bins);

	// -- After applying efficiency correction -- //
   const int nweights = 211;
   TH1D* h_mass_EffPass_Corr_tnp[nweights];
   for (int i=0; i<nweights; i++) {
      h_mass_EffPass_Corr_tnp[i] = new TH1D(Form("h_mass_EffPass_Corr_tnp%d",i), "", binnum, bins);
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

      // rochcor2015 *rmcor = new rochcor2015();

		Bool_t isNLO = 0;
		if( Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "TT" || Tag[i_tup].Contains("WE") || Tag[i_tup].Contains("WMu") )
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
         if (doHFrew) PUWeight = hftool.weight(ntuple->hiHF,rewmode); 

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

				// -- Flags -- //
				Bool_t Flag_PassAcc = kFALSE;
				Bool_t Flag_PassAccEff = kFALSE;

				// -- Fill the mass histograms -- //
				h_mass->Fill( gen_M, TotWeight );
				h_mass_tot->Fill( gen_M, TotWeight );

				Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

				// -- Acceptance Calculation -- //
				if( Flag_PassAcc == kTRUE ) 
				{
					h_mass_AccTotal->Fill( gen_M, TotWeight );
					h_mass_AccPass->Fill( gen_M, TotWeight );
				}
				else
				{
					h_mass_AccTotal->Fill( gen_M, TotWeight );
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
						Bool_t isPassEventSelection = kFALSE;
						isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection);

						if( isPassEventSelection == kTRUE )
						{

							Flag_PassEff = kTRUE;
							Flag_PassAccEff = kTRUE;
						}

					} // -- End of if( ntuple->isTriggered( HLT ) ) -- //

					// -- Efficiency Calculation -- //
					if( Flag_PassEff == kTRUE)
					{
						h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );
						h_mass_EffPass->Fill( gen_M, TotWeight * PUWeight );

                  // TnP
                  double pt1 = SelectedMuonCollection[0].Pt;
                  double pt2 = SelectedMuonCollection[1].Pt;
                  double eta1 = SelectedMuonCollection[0].eta;
                  double eta2 = SelectedMuonCollection[1].eta;

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
                     TnpWeight = tnp_weight_muid_ppb(pt1,eta1,imuid)*tnp_weight_iso_ppb(pt1,eta1,iiso)
                        *tnp_weight_muid_ppb(pt2,eta2,imuid)*tnp_weight_iso_ppb(pt2,eta1,iiso);
                     // add trg... careful!
                     double sf_trg;
                     if (pt2>=15. && pt1>=15.) { // both muons could trigger
                        double eff_data = (1 - (1 - tnp_weight_trg_ppb(eta1,200)*tnp_weight_trg_ppb(eta1,itrg)/tnp_weight_trg_ppb(eta1,0)) * (1 - tnp_weight_trg_ppb(eta2,200)*tnp_weight_trg_ppb(eta2,itrg)/tnp_weight_trg_ppb(eta2,0)) );
                        double eff_mc = (1 - (1 - tnp_weight_trg_ppb(eta1,300)) * (1 - tnp_weight_trg_ppb(eta2,300)) );
                        sf_trg = eff_data/eff_mc;
                     } else if (pt1<15) {
                        sf_trg = tnp_weight_trg_ppb(eta2,itrg);
                     } else if (pt2<15) {
                        sf_trg = tnp_weight_trg_ppb(eta1,itrg);
                     }
                     TnpWeight = TnpWeight * sf_trg;

                     h_mass_EffPass_Corr_tnp[iwt]->Fill( gen_M, TotWeight  * PUWeight * TnpWeight );
                  } // tnp weight loop
					}
					else
						h_mass_EffTotal->Fill( gen_M, TotWeight * PUWeight );

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
	h_mass_EffTotal->Write();
	h_mass_EffPass->Write();
	for (int i=0; i<nweights; i++) h_mass_EffPass_Corr_tnp[i]->Write();

	TEfficiency *Acc_Mass = new TEfficiency(*h_mass_AccPass, *h_mass_AccTotal);
	Acc_Mass->SetName("TEff_Acc_Mass");

	TEfficiency *Eff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_EffTotal);
	Eff_Mass->SetName("TEff_Eff_Mass");

	// TEfficiency *AccEff_Mass = new TEfficiency(*h_mass_EffPass, *h_mass_AccTotal);
	// AccEff_Mass->SetName("TEff_AccEff_Mass");

   TGraphAsymmErrors *g_Eff_Mass_Corr_tnp_stat = NULL;
   TEfficiency* Eff_Mass_Corr_tnp[nweights];
   for (int i=0; i<nweights; i++) {
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
   }

   // now let's also write 2 TGRaphAsymmErrors for the efficiency including tnp corrections:
   // one with tnp uncertainties, and the other with total (stat MC + tnp syst) uncertainties
   TGraphAsymmErrors *g_Eff_Mass_Corr_tnp_syst = (TGraphAsymmErrors*) g_Eff_Mass_Corr_tnp_stat->Clone("g_Eff_Mass_Corr_tnp_syst");
   TGraphAsymmErrors *g_Eff_Mass_Corr_tnp_tot = (TGraphAsymmErrors*) g_Eff_Mass_Corr_tnp_stat->Clone("g_Eff_Mass_Corr_tnp_tot");
   for (int ibin=0; ibin<binnum; ibin++) {
      // compute the tnp syst uncertainty
      double tnptot = 0;
      double tnpstat1 = 0, tnpstat2 = 0, tnpstat3 = 0;
      double tnpsyst1 = 0, tnpsyst2 = 0, tnpsyst3 = 0, tnpsyst4 = 0, tnpsyst5 = 0, tnpsyst6 = 0.34e-2, tnpsyst7 = 0.6e-2;
      double e0 = Eff_Mass_Corr_tnp[0]->GetEfficiency(ibin+1);
      TEfficiency** ee = Eff_Mass_Corr_tnp; // shortcut

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
      g_Eff_Mass_Corr_tnp_syst->SetPointEYlow(ibin,tnptot);
      g_Eff_Mass_Corr_tnp_syst->SetPointEYhigh(ibin,tnptot);
      g_Eff_Mass_Corr_tnp_tot->SetPointEYlow(ibin,sqrt(pow(tnptot,2)+pow(Eff_Mass_Corr_tnp[0]->GetEfficiencyErrorLow(ibin+1),2)));
      g_Eff_Mass_Corr_tnp_tot->SetPointEYhigh(ibin,sqrt(pow(tnptot,2)+pow(Eff_Mass_Corr_tnp[0]->GetEfficiencyErrorUp(ibin+1),2)));
   } // ibin loop for tnp uncertainty
   g_Eff_Mass_Corr_tnp_stat->Write();
   g_Eff_Mass_Corr_tnp_syst->Write();
   g_Eff_Mass_Corr_tnp_tot->Write();

	Acc_Mass->Write();
	Eff_Mass->Write();
	// AccEff_Mass->Write();

	TCanvas *c_Acc_Mass = new TCanvas("c_Acc_Mass", "", 800, 600);
	c_Acc_Mass->cd();
	Acc_Mass->Draw("AP");
	c_Acc_Mass->Write();

	TCanvas *c_Eff_Mass = new TCanvas("c_Eff_Mass", "", 800, 600);
	c_Eff_Mass->cd();
	Eff_Mass->Draw("AP");
	c_Eff_Mass->Write();

	// TCanvas *c_AccEff_Mass = new TCanvas("c_AccEff_Mass", "", 800, 600);
	// c_AccEff_Mass->cd();
	// AccEff_Mass->Draw("AP");
	// c_AccEff_Mass->Write();


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


