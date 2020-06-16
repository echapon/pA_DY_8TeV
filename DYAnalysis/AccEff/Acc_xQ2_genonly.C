#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
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
#include <utility>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "../Include/DYAnalyzer.h"
#include "../BkgEst/interface/defs.h"

using namespace DYana;

static inline void loadBar(int x, int n, int r, int w);
void Acc_xQ2_genonly(TString Sample) 
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Sample: " << Sample << endl;
	TStopwatch totaltime;
	totaltime.Start();

	DYAnalyzer *analyzer = new DYAnalyzer( "PAL3Mu12" );

	TFile *f = new TFile("ROOTFile_Histogram_Acc_xQ2_genonly_" + Sample + ".root", "RECREATE");

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

   TH2D *h_mass_AccTotal = new TH2D("h_mass_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass)), binnum, bins,50,-5,0);
   TH2D *h_mass_AccPass = new TH2D("h_mass_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass)), binnum, bins,50,-5,0);
   TH2D *h_mass3bins_AccTotal = new TH2D("h_mass3bins_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,50,-5,0);
   TH2D *h_mass3bins_AccPass = new TH2D("h_mass3bins_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,50,-5,0);
   TH2D *h_pt_AccTotal = new TH2D("h_pt_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,50,-5,0);
   TH2D *h_pt_AccPass = new TH2D("h_pt_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,50,-5,0);
   TH2D *h_phistar_AccTotal = new TH2D("h_phistar_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,50,-5,0);
   TH2D *h_phistar_AccPass = new TH2D("h_phistar_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,50,-5,0);
   TH2D *h_rap1560_AccTotal = new TH2D("h_rap1560_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,50,-5,0);
   TH2D *h_rap1560_AccPass = new TH2D("h_rap1560_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,50,-5,0);
   TH2D *h_rap60120_AccTotal = new TH2D("h_rap60120_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,50,-5,0);
   TH2D *h_rap60120_AccPass = new TH2D("h_rap60120_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,50,-5,0);

   TProfile *p_mass_AccTotal = new TProfile("p_mass_AccTotal", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::mass)), binnum, bins,1e-6,1,"s");
   TProfile *p_mass_AccPass = new TProfile("p_mass_AccPass", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::mass)), binnum, bins,1e-6,1,"s");
   TProfile *p_mass3bins_AccTotal = new TProfile("p_mass3bins_AccTotal", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,1e-6,1,"s");
   TProfile *p_mass3bins_AccPass = new TProfile("p_mass3bins_AccPass", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,1e-6,1,"s");
   TProfile *p_pt_AccTotal = new TProfile("p_pt_AccTotal", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,1e-6,1,"s");
   TProfile *p_pt_AccPass = new TProfile("p_pt_AccPass", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,1e-6,1,"s");
   TProfile *p_phistar_AccTotal = new TProfile("p_phistar_AccTotal", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,1e-6,1,"s");
   TProfile *p_phistar_AccPass = new TProfile("p_phistar_AccPass", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,1e-6,1,"s");
   TProfile *p_rap1560_AccTotal = new TProfile("p_rap1560_AccTotal", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,1e-6,1,"s");
   TProfile *p_rap1560_AccPass = new TProfile("p_rap1560_AccPass", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,1e-6,1,"s");
   TProfile *p_rap60120_AccTotal = new TProfile("p_rap60120_AccTotal", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,1e-6,1,"s");
   TProfile *p_rap60120_AccPass = new TProfile("p_rap60120_AccPass", Form(";%s;x_{Pb}",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,1e-6,1,"s");
   TProfile *plog_mass_AccTotal = new TProfile("plog_mass_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass)), binnum, bins,-10,0,"s");
   TProfile *plog_mass_AccPass = new TProfile("plog_mass_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass)), binnum, bins,-10,0,"s");
   TProfile *plog_mass3bins_AccTotal = new TProfile("plog_mass3bins_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,-10,0,"s");
   TProfile *plog_mass3bins_AccPass = new TProfile("plog_mass3bins_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,-10,0,"s");
   TProfile *plog_pt_AccTotal = new TProfile("plog_pt_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,-10,0,"s");
   TProfile *plog_pt_AccPass = new TProfile("plog_pt_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,-10,0,"s");
   TProfile *plog_phistar_AccTotal = new TProfile("plog_phistar_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,-10,0,"s");
   TProfile *plog_phistar_AccPass = new TProfile("plog_phistar_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,-10,0,"s");
   TProfile *plog_rap1560_AccTotal = new TProfile("plog_rap1560_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,-10,0,"s");
   TProfile *plog_rap1560_AccPass = new TProfile("plog_rap1560_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,-10,0,"s");
   TProfile *plog_rap60120_AccTotal = new TProfile("plog_rap60120_AccTotal", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,-10,0,"s");
   TProfile *plog_rap60120_AccPass = new TProfile("plog_rap60120_AccPass", Form(";%s;log_{10}(x_{Pb})",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,-10,0,"s");

   TH2D *hQ_mass_AccTotal = new TH2D("hQ_mass_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass)), binnum, bins,200,0,1000);
   TH2D *hQ_mass_AccPass = new TH2D("hQ_mass_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass)), binnum, bins,200,0,1000);
   TH2D *hQ_mass3bins_AccTotal = new TH2D("hQ_mass3bins_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,200,0,1000);
   TH2D *hQ_mass3bins_AccPass = new TH2D("hQ_mass3bins_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,200,0,1000);
   TH2D *hQ_pt_AccTotal = new TH2D("hQ_pt_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,200,0,1000);
   TH2D *hQ_pt_AccPass = new TH2D("hQ_pt_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,200,0,1000);
   TH2D *hQ_phistar_AccTotal = new TH2D("hQ_phistar_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,200,0,1000);
   TH2D *hQ_phistar_AccPass = new TH2D("hQ_phistar_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,200,0,1000);
   TH2D *hQ_rap1560_AccTotal = new TH2D("hQ_rap1560_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,200,0,1000);
   TH2D *hQ_rap1560_AccPass = new TH2D("hQ_rap1560_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,200,0,1000);
   TH2D *hQ_rap60120_AccTotal = new TH2D("hQ_rap60120_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,200,0,1000);
   TH2D *hQ_rap60120_AccPass = new TH2D("hQ_rap60120_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,200,0,1000);

   TProfile *pQ_mass_AccTotal = new TProfile("pQ_mass_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass)), binnum, bins,0,8000,"s");
   TProfile *pQ_mass_AccPass = new TProfile("pQ_mass_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass)), binnum, bins,0,8000,"s");
   TProfile *pQ_mass3bins_AccTotal = new TProfile("pQ_mass3bins_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,0,8000,"s");
   TProfile *pQ_mass3bins_AccPass = new TProfile("pQ_mass3bins_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::mass3bins)), binnum3, bins3,0,8000,"s");
   TProfile *pQ_pt_AccTotal = new TProfile("pQ_pt_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,0,8000,"s");
   TProfile *pQ_pt_AccPass = new TProfile("pQ_pt_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::pt)), ptbinnum_meas, ptbin_meas,0,8000,"s");
   TProfile *pQ_phistar_AccTotal = new TProfile("pQ_phistar_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,0,8000,"s");
   TProfile *pQ_phistar_AccPass = new TProfile("pQ_phistar_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::phistar)), phistarnum, phistarbin,0,8000,"s");
   TProfile *pQ_rap1560_AccTotal = new TProfile("pQ_rap1560_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,0,8000,"s");
   TProfile *pQ_rap1560_AccPass = new TProfile("pQ_rap1560_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap1560)), rapbinnum_1560, rapbin_1560,0,8000,"s");
   TProfile *pQ_rap60120_AccTotal = new TProfile("pQ_rap60120_AccTotal", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,0,8000,"s");
   TProfile *pQ_rap60120_AccPass = new TProfile("pQ_rap60120_AccPass", Form(";%s;Q [GeV]",DYana::xaxistitle(DYana::var::rap60120)), rapbinnum_60120, rapbin_60120,0,8000,"s");

	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";
	// -- Each ntuple directory & corresponding Tags -- //
		// -- GenWeights are already taken into account in nEvents -- //
	vector< TString > ntupleDirectory; vector< TString > Tag; vector< Double_t > Xsec; vector< Double_t > nEvents; vector< SampleTag > STags;

   analyzer->SetupMCsamples_v20180814(Sample, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);

	// -- Loop for all samples -- //
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
      // loop only on DYMuMu!
      if (!Tag[i_tup].Contains("DYMuMu")) continue;

      Bool_t doflip = (switcheta(STags[i_tup])<0);
      Int_t  flipsign = doflip ? -1 : 1;
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
		ntuple->TurnOnBranches_GenLepton();
      // ntuple->TurnOnBranches_GenOthers();

      pair<float,float>   *pdfX = 0;
      float pdfX_first;
      float pdfX_second;
      Float_t         qScale;
      TBranch        *b_pdfX;   //!
      TBranch        *b_pdfX_first;   //!
      TBranch        *b_pdfX_second;   //!
      TBranch        *b_qScale;   //!
      chainGen->SetBranchAddress("pdfX", &pdfX, &b_pdfX);
      chainGen->SetBranchAddress("first", &pdfX_first, &b_pdfX_first);
      chainGen->SetBranchAddress("second", &pdfX_second, &b_pdfX_second);
      chainGen->SetBranchAddress("qScale", &qScale, &b_qScale);
      chainGen->SetBranchStatus("*",0);
      chainGen->SetBranchStatus("pdfX",1);
      chainGen->SetBranchStatus("first",1);
      chainGen->SetBranchStatus("second",1);
      chainGen->SetBranchStatus("qScale",1);
      float xPb,log_xPb;

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
      if (Sample.Contains("CT14")) // we only have 1 beam direction for CT14
            norm = ( Xsec[i_tup] * lumi_all ) / (Double_t)nEvents[i_tup];

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

         xPb = (doflip) ? pdfX->first : pdfX->second;
         log_xPb = log(xPb)/log(10);

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

				// -- Flags -- //
				Bool_t Flag_PassAcc = kFALSE;

				// -- Fill the mass histograms -- //
				h_mass->Fill( gen_M, TotWeight );
				h_mass_tot->Fill( gen_M, TotWeight );

				Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);

            // -- Acceptance Calculation -- //

            h_mass_AccTotal->Fill( gen_M, log_xPb, TotWeight );
            plog_mass_AccTotal->Fill( gen_M, log_xPb, TotWeight );
            p_mass_AccTotal->Fill( gen_M, xPb, TotWeight );
            h_mass3bins_AccTotal->Fill( gen_M, log_xPb, TotWeight );
            plog_mass3bins_AccTotal->Fill( gen_M, log_xPb, TotWeight );
            p_mass3bins_AccTotal->Fill( gen_M, xPb, TotWeight );

            hQ_mass_AccTotal->Fill( gen_M, qScale, TotWeight );
            pQ_mass_AccTotal->Fill( gen_M, qScale, TotWeight );
            hQ_mass3bins_AccTotal->Fill( gen_M, qScale, TotWeight );
            pQ_mass3bins_AccTotal->Fill( gen_M, qScale, TotWeight );
            if (gen_M>60 && gen_M<120) {
               h_pt_AccTotal->Fill( gen_Pt, log_xPb, TotWeight );
               h_phistar_AccTotal->Fill( gen_Phistar, log_xPb, TotWeight );
               h_rap60120_AccTotal->Fill( gen_Rap, log_xPb, TotWeight );
               plog_pt_AccTotal->Fill( gen_Pt, log_xPb, TotWeight );
               plog_phistar_AccTotal->Fill( gen_Phistar, log_xPb, TotWeight );
               plog_rap60120_AccTotal->Fill( gen_Rap, log_xPb, TotWeight );
               p_pt_AccTotal->Fill( gen_Pt, xPb, TotWeight );
               p_phistar_AccTotal->Fill( gen_Phistar, xPb, TotWeight );
               p_rap60120_AccTotal->Fill( gen_Rap, xPb, TotWeight );

               hQ_pt_AccTotal->Fill( gen_Pt, qScale, TotWeight );
               hQ_phistar_AccTotal->Fill( gen_Phistar, qScale, TotWeight );
               hQ_rap60120_AccTotal->Fill( gen_Rap, qScale, TotWeight );
               pQ_pt_AccTotal->Fill( gen_Pt, qScale, TotWeight );
               pQ_phistar_AccTotal->Fill( gen_Phistar, qScale, TotWeight );
               pQ_rap60120_AccTotal->Fill( gen_Rap, qScale, TotWeight );
            } else if (gen_M>15 && gen_M<60) {
               h_rap1560_AccTotal->Fill( gen_Rap, log_xPb, TotWeight );
               plog_rap1560_AccTotal->Fill( gen_Rap, log_xPb, TotWeight );
               p_rap1560_AccTotal->Fill( gen_Rap, xPb, TotWeight );

               hQ_rap1560_AccTotal->Fill( gen_Rap, qScale, TotWeight );
               pQ_rap1560_AccTotal->Fill( gen_Rap, qScale, TotWeight );
            }
            if( Flag_PassAcc == kTRUE ) 
            {
               h_mass_AccPass->Fill( gen_M, log_xPb, TotWeight );
               plog_mass_AccPass->Fill( gen_M, log_xPb, TotWeight );
               p_mass_AccPass->Fill( gen_M, xPb, TotWeight );
               h_mass3bins_AccPass->Fill( gen_M, log_xPb, TotWeight );
               plog_mass3bins_AccPass->Fill( gen_M, log_xPb, TotWeight );
               p_mass3bins_AccPass->Fill( gen_M, xPb, TotWeight );

               hQ_mass_AccPass->Fill( gen_M, qScale, TotWeight );
               pQ_mass_AccPass->Fill( gen_M, qScale, TotWeight );
               hQ_mass3bins_AccPass->Fill( gen_M, qScale, TotWeight );
               pQ_mass3bins_AccPass->Fill( gen_M, qScale, TotWeight );
               if (gen_M>60 && gen_M<120) {
                  h_pt_AccPass->Fill( gen_Pt, log_xPb, TotWeight );
                  h_phistar_AccPass->Fill( gen_Phistar, log_xPb, TotWeight );
                  h_rap60120_AccPass->Fill( gen_Rap, log_xPb, TotWeight );
                  plog_pt_AccPass->Fill( gen_Pt, log_xPb, TotWeight );
                  plog_phistar_AccPass->Fill( gen_Phistar, log_xPb, TotWeight );
                  plog_rap60120_AccPass->Fill( gen_Rap, log_xPb, TotWeight );
                  p_pt_AccPass->Fill( gen_Pt, xPb, TotWeight );
                  p_phistar_AccPass->Fill( gen_Phistar, xPb, TotWeight );
                  p_rap60120_AccPass->Fill( gen_Rap, xPb, TotWeight );

                  hQ_pt_AccPass->Fill( gen_Pt, qScale, TotWeight );
                  hQ_phistar_AccPass->Fill( gen_Phistar, qScale, TotWeight );
                  hQ_rap60120_AccPass->Fill( gen_Rap, qScale, TotWeight );
                  pQ_pt_AccPass->Fill( gen_Pt, qScale, TotWeight );
                  pQ_phistar_AccPass->Fill( gen_Phistar, qScale, TotWeight );
                  pQ_rap60120_AccPass->Fill( gen_Rap, qScale, TotWeight );
               } else if (gen_M>15 && gen_M<60) {
                  h_rap1560_AccPass->Fill( gen_Rap, log_xPb, TotWeight );
                  plog_rap1560_AccPass->Fill( gen_Rap, log_xPb, TotWeight );
                  p_rap1560_AccPass->Fill( gen_Rap, xPb, TotWeight );

                  hQ_rap1560_AccPass->Fill( gen_Rap, qScale, TotWeight );
                  pQ_rap1560_AccPass->Fill( gen_Rap, qScale, TotWeight );
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

   plog_mass_AccTotal->Write();
   plog_mass_AccPass->Write();
   plog_mass3bins_AccTotal->Write();
   plog_mass3bins_AccPass->Write();
   plog_pt_AccTotal->Write();
   plog_phistar_AccTotal->Write();
   plog_pt_AccPass->Write();
   plog_phistar_AccPass->Write();
   plog_rap1560_AccTotal->Write();
   plog_rap1560_AccPass->Write();
   plog_rap60120_AccTotal->Write();
   plog_rap60120_AccPass->Write();

   p_mass_AccTotal->Write();
   p_mass_AccPass->Write();
   p_mass3bins_AccTotal->Write();
   p_mass3bins_AccPass->Write();
   p_pt_AccTotal->Write();
   p_phistar_AccTotal->Write();
   p_pt_AccPass->Write();
   p_phistar_AccPass->Write();
   p_rap1560_AccTotal->Write();
   p_rap1560_AccPass->Write();
   p_rap60120_AccTotal->Write();
   p_rap60120_AccPass->Write();

   hQ_mass_AccTotal->Write();
   hQ_mass_AccPass->Write();
   hQ_mass3bins_AccTotal->Write();
   hQ_mass3bins_AccPass->Write();
   hQ_pt_AccTotal->Write();
   hQ_phistar_AccTotal->Write();
   hQ_pt_AccPass->Write();
   hQ_phistar_AccPass->Write();
   hQ_rap1560_AccTotal->Write();
   hQ_rap1560_AccPass->Write();
   hQ_rap60120_AccTotal->Write();
   hQ_rap60120_AccPass->Write();

   pQ_mass_AccTotal->Write();
   pQ_mass_AccPass->Write();
   pQ_mass3bins_AccTotal->Write();
   pQ_mass3bins_AccPass->Write();
   pQ_pt_AccTotal->Write();
   pQ_phistar_AccTotal->Write();
   pQ_pt_AccPass->Write();
   pQ_phistar_AccPass->Write();
   pQ_rap1560_AccTotal->Write();
   pQ_rap1560_AccPass->Write();
   pQ_rap60120_AccTotal->Write();
   pQ_rap60120_AccPass->Write();


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


