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

#include "LHAPDF/PDFSet.h"
#include "LHAPDF/PDF.h"

#include <vector>
#include <utility>
#include <iostream>

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "../Include/DYAnalyzer.h"
#include "../Include/tnp_weight.h"
#include "../BkgEst/interface/defs.h"

using namespace DYana;
using namespace LHAPDF;

const unsigned int nweights = 325;

static inline void loadBar(int x, int n, int r, int w);
void Acc_weights_genonly(TString Sample, bool doreweight=false, 
      TString initial_p="CT14nlo", TString initial_Pb="CT14nlo", 
      TString target_p="CT14nlo", TString target_Pb="CT14nlo",
      bool storetree=false,
      bool doisospin=false) 
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
	cout << "Sample: " << Sample << endl;
	TStopwatch totaltime;
	totaltime.Start();

   gRandom = new TRandom3();

   /////////////////////////////////////
   // START SETUP OF WEIGHTS AND PDFS //
   /////////////////////////////////////

   // number of gen weights (CT14: 284, EPPS16: 325
   unsigned int iwtmax = 284;
   if (Sample.Contains("EPPS16")) iwtmax = 325;
   if (Sample.Contains("MG5")) iwtmax = 0;

   // setup the PDFs we need
   PDFSet* pdfset_initial_p = new PDFSet(initial_p.Data());
   const PDF* pdf_initial_p = pdfset_initial_p->mkPDF(0);

   PDFSet* pdfset_target_p;
   if (target_p==initial_p) pdfset_target_p = pdfset_initial_p;
   else pdfset_target_p = new PDFSet(target_p.Data());
   const vector<PDF*> pdf_target_p = pdfset_target_p->mkPDFs();
   
   PDFSet* pdfset_initial_Pb;
   if (initial_Pb==initial_p) pdfset_initial_Pb = pdfset_initial_p;
   else if (initial_Pb==target_p) pdfset_initial_Pb = pdfset_target_p;
   else pdfset_initial_Pb = new PDFSet(initial_Pb.Data());
   const PDF* pdf_initial_Pb = pdfset_initial_Pb->mkPDF(0);
   
   PDFSet* pdfset_target_Pb;
   if (target_Pb==initial_p) pdfset_target_Pb = pdfset_initial_p;
   else if (target_Pb==target_p) pdfset_target_Pb = pdfset_target_p;
   else if (target_Pb==initial_Pb) pdfset_target_Pb = pdfset_initial_Pb;
   else pdfset_target_Pb = new PDFSet(target_Pb.Data());
   const vector<PDF*> pdf_target_Pb = pdfset_target_Pb->mkPDFs();

   unsigned int target_p_size = pdf_target_p.size();
   unsigned int target_Pb_size = pdf_target_Pb.size();

   if (doreweight) iwtmax = target_p_size + target_Pb_size - 1;
   if (doreweight && target_Pb.Contains("EPPS")) iwtmax = target_Pb_size;
   if (doreweight && target_p == target_Pb) iwtmax = target_p_size;
   if (doreweight && target_p.Contains("TUJU") && target_Pb.Contains("TUJU")) iwtmax = target_Pb_size;

   if (iwtmax > nweights) {
      cout << "ERROR iwtmax > nweights (" << iwtmax << " > " << nweights << "). Please increase nweights in the code." << endl;
      return;
   }

   /////////////////////////////////////
   //  END SETUP OF WEIGHTS AND PDFS  //
   /////////////////////////////////////

	DYAnalyzer *analyzer = new DYAnalyzer( "PAL3Mu12" );

   TString reweightlabel = "";
   if (doreweight) reweightlabel = "_rewt_" + target_p + "_" + target_Pb;
   if (doreweight && doisospin) reweightlabel += "_rewisospin";
	TFile *f = new TFile("ROOTFile_Histogram_Acc_weights_genonly_" + Sample + reweightlabel + ".root", "RECREATE");

 	TH1D *h_mass_tot = new TH1D("h_mass_tot", "", 10000, 0, 10000);

   // MCFM histos
   TDirectory *dMCFM = f->mkdir("MCFM","histos for comparison with MCFM");
   dMCFM->cd();
   TH1D *h_m34_1[nweights];
   TH1D *h_m34_2[nweights];
   TH1D *h_y34lM[nweights];
   TH1D *h_pt34lM[nweights];
   TH1D *h_phist34lM_1[nweights];
   TH1D *h_phist34lM_2[nweights];
   TH1D *h_y34hM[nweights];
   TH1D *h_pt34hM[nweights];
   TH1D *h_phist34hM_1[nweights];
   TH1D *h_phist34hM_2[nweights];
   TH1D *h_m34cut_1[nweights];
   TH1D *h_m34cut_2[nweights];

   f->cd();
   TH1D *h_IDp = new TH1D("h_IDp","",35,-10,25);
   TH1D *h_IDPb = new TH1D("h_IDPb","",35,-10,25);

   TH1D* h_mass_AccTotal[nweights];
   TH1D* h_mass3bins_AccTotal[nweights];
   TH1D* h_pt_AccTotal[nweights];
   TH1D* h_phistar_AccTotal[nweights];
   TH1D* h_rap1560_AccTotal[nweights];
   TH1D* h_rap60120_AccTotal[nweights];
   TH1D* h_pt1560_AccTotal[nweights];
   TH1D* h_phistar1560_AccTotal[nweights];
   TH1D* h_mass_AccTotal_pre[nweights];
   TH1D* h_mass3bins_AccTotal_pre[nweights];
   TH1D* h_pt_AccTotal_pre[nweights];
   TH1D* h_phistar_AccTotal_pre[nweights];
   TH1D* h_rap1560_AccTotal_pre[nweights];
   TH1D* h_rap60120_AccTotal_pre[nweights];
   TH1D* h_pt1560_AccTotal_pre[nweights];
   TH1D* h_phistar1560_AccTotal_pre[nweights];
   TH1D* h_mass_AccPass[nweights];
   TH1D* h_mass3bins_AccPass[nweights];
   TH1D* h_pt_AccPass[nweights];
   TH1D* h_phistar_AccPass[nweights];
   TH1D* h_rap1560_AccPass[nweights];
   TH1D* h_rap60120_AccPass[nweights];
   TH1D* h_pt1560_AccPass[nweights];
   TH1D* h_phistar1560_AccPass[nweights];
   TH1D* h_mass_AccPass_pre[nweights];
   TH1D* h_mass3bins_AccPass_pre[nweights];
   TH1D* h_pt_AccPass_pre[nweights];
   TH1D* h_phistar_AccPass_pre[nweights];
   TH1D* h_rap1560_AccPass_pre[nweights];
   TH1D* h_rap60120_AccPass_pre[nweights];
   TH1D* h_pt1560_AccPass_pre[nweights];
   TH1D* h_phistar1560_AccPass_pre[nweights];
   
   for (unsigned int i=0; i<nweights; i++) {
      dMCFM->cd();
      h_m34_1[i] = new TH1D(Form("h_m34_1_%d",i),"",60,60,120);
      h_m34_2[i] = new TH1D(Form("h_m34_2_%d",i),"",60,0,300);
      h_y34lM[i] = new TH1D(Form("h_y34lM_%d",i),"",60,-6,6);
      h_pt34lM[i] = new TH1D(Form("h_pt34lM_%d",i),"",25,0,50);
      h_phist34lM_1[i] = new TH1D(Form("h_phist34lM_1_%d",i),"",100,0,1);
      h_phist34lM_2[i] = new TH1D(Form("h_phist34lM_2_%d",i),"",60,0,3);
      h_y34hM[i] = new TH1D(Form("h_y34hM_%d",i),"",60,-6,6);
      h_pt34hM[i] = new TH1D(Form("h_pt34hM_%d",i),"",25,0,50);
      h_phist34hM_1[i] = new TH1D(Form("h_phist34hM_1_%d",i),"",100,0,1);
      h_phist34hM_2[i] = new TH1D(Form("h_phist34hM_2_%d",i),"",60,0,3);
      h_m34cut_1[i] = new TH1D(Form("h_m34cut_1_%d",i),"",60,0,600);
      h_m34cut_2[i] = new TH1D(Form("h_m34cut_2_%d",i),"",80,70,110);

      f->cd();
      h_mass_AccTotal[i] = new TH1D(Form("h_mass_AccTotal%d",i), "", binnum, bins);
      h_mass_AccTotal_pre[i] = new TH1D(Form("h_mass_AccTotal_pre%d",i), "", binnum, bins);
      h_mass_AccPass[i] = new TH1D(Form("h_mass_AccPass%d",i), "", binnum, bins);
      h_mass_AccPass_pre[i] = new TH1D(Form("h_mass_AccPass_pre%d",i), "", binnum, bins);
      h_mass3bins_AccTotal[i] = new TH1D(Form("h_mass3bins_AccTotal%d",i), "", binnum3, bins3);
      h_mass3bins_AccTotal_pre[i] = new TH1D(Form("h_mass3bins_AccTotal_pre%d",i), "", binnum3, bins3);
      h_mass3bins_AccPass[i] = new TH1D(Form("h_mass3bins_AccPass%d",i), "", binnum3, bins3);
      h_mass3bins_AccPass_pre[i] = new TH1D(Form("h_mass3bins_AccPass_pre%d",i), "", binnum3, bins3);
      h_pt_AccTotal[i] = new TH1D(Form("h_pt_AccTotal%d",i), "", ptbinnum_meas, ptbin_meas);
      h_pt_AccTotal_pre[i] = new TH1D(Form("h_pt_AccTotal_pre%d",i), "", ptbinnum_meas, ptbin_meas);
      h_pt_AccPass[i] = new TH1D(Form("h_pt_AccPass%d",i), "", ptbinnum_meas, ptbin_meas);
      h_pt_AccPass_pre[i] = new TH1D(Form("h_pt_AccPass_pre%d",i), "", ptbinnum_meas, ptbin_meas);
      h_phistar_AccTotal[i] = new TH1D(Form("h_phistar_AccTotal%d",i), "", phistarnum, phistarbin);
      h_phistar_AccTotal_pre[i] = new TH1D(Form("h_phistar_AccTotal_pre%d",i), "", phistarnum, phistarbin);
      h_phistar_AccPass[i] = new TH1D(Form("h_phistar_AccPass%d",i), "", phistarnum, phistarbin);
      h_phistar_AccPass_pre[i] = new TH1D(Form("h_phistar_AccPass_pre%d",i), "", phistarnum, phistarbin);
      h_rap1560_AccTotal[i] = new TH1D(Form("h_rap1560_AccTotal%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap1560_AccTotal_pre[i] = new TH1D(Form("h_rap1560_AccTotal_pre%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap1560_AccPass[i] = new TH1D(Form("h_rap1560_AccPass%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap1560_AccPass_pre[i] = new TH1D(Form("h_rap1560_AccPass_pre%d",i), "", rapbinnum_1560, rapbin_1560);
      h_rap60120_AccTotal[i] = new TH1D(Form("h_rap60120_AccTotal%d",i), "", rapbinnum_60120, rapbin_60120);
      h_rap60120_AccTotal_pre[i] = new TH1D(Form("h_rap60120_AccTotal_pre%d",i), "", rapbinnum_60120, rapbin_60120);
      h_rap60120_AccPass[i] = new TH1D(Form("h_rap60120_AccPass%d",i), "", rapbinnum_60120, rapbin_60120);
      h_rap60120_AccPass_pre[i] = new TH1D(Form("h_rap60120_AccPass_pre%d",i), "", rapbinnum_60120, rapbin_60120);
      h_pt1560_AccTotal[i] = new TH1D(Form("h_pt1560_AccTotal%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_pt1560_AccTotal_pre[i] = new TH1D(Form("h_pt1560_AccTotal_pre%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_pt1560_AccPass[i] = new TH1D(Form("h_pt1560_AccPass%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_pt1560_AccPass_pre[i] = new TH1D(Form("h_pt1560_AccPass_pre%d",i), "", ptbinnum_meas_1560, ptbin_meas_1560);
      h_phistar1560_AccTotal[i] = new TH1D(Form("h_phistar1560_AccTotal%d",i), "", phistarnum_1560, phistarbin_1560);
      h_phistar1560_AccTotal_pre[i] = new TH1D(Form("h_phistar1560_AccTotal_pre%d",i), "", phistarnum_1560, phistarbin_1560);
      h_phistar1560_AccPass[i] = new TH1D(Form("h_phistar1560_AccPass%d",i), "", phistarnum_1560, phistarbin_1560);
      h_phistar1560_AccPass_pre[i] = new TH1D(Form("h_phistar1560_AccPass_pre%d",i), "", phistarnum_1560, phistarbin_1560);
   }

   // let's also store stuff into a tree
   TTree *tr = new TTree("tr","tree");
   Double_t gen_M, gen_M_pre, gen_M_post;
   Double_t gen_Rap, gen_Rap_pre, gen_Rap_post;
   Double_t gen_Pt, gen_Pt_pre, gen_Pt_post;
   Double_t gen_Phistar, gen_Phistar_pre, gen_Phistar_post;
   Double_t TotWeight;
   Bool_t Flag_PassAcc = kFALSE;
   Bool_t Flag_PassAcc_pre = kFALSE;
   tr->Branch("gen_M",&gen_M,"gen_M/D");
   tr->Branch("gen_M_pre",&gen_M_pre,"gen_M_pre/D");
   tr->Branch("gen_M_post",&gen_M_post,"gen_M_post/D");
   tr->Branch("gen_Rap",&gen_Rap,"gen_Rap/D");
   tr->Branch("gen_Rap_pre",&gen_Rap_pre,"gen_Rap_pre/D");
   tr->Branch("gen_Rap_post",&gen_Rap_post,"gen_Rap_post/D");
   tr->Branch("gen_Pt",&gen_Pt,"gen_Pt/D");
   tr->Branch("gen_Pt_pre",&gen_Pt_pre,"gen_Pt_pre/D");
   tr->Branch("gen_Pt_post",&gen_Pt_post,"gen_Pt_post/D");
   tr->Branch("gen_Phistar",&gen_Phistar,"gen_Phistar/D");
   tr->Branch("gen_Phistar_pre",&gen_Phistar_pre,"gen_Phistar_pre/D");
   tr->Branch("gen_Phistar_post",&gen_Phistar_post,"gen_Phistar_post/D");
   tr->Branch("TotWeight",&TotWeight,"TotWeight/D");
   tr->Branch("Flag_PassAcc",&Flag_PassAcc,"Flag_PassAcc/O");
   tr->Branch("Flag_PassAcc_pre",&Flag_PassAcc_pre,"Flag_PassAcc_pre/O");

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
      Int_t  dorapshift = 1; 
      if (Sample.Contains("MG5")) dorapshift = 0;// NO rap shift for MG5
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
      ntuple->TurnOnBranches_GenOthers();

      vector<float>   *ttbar_w = 0;
      pair<int,int    >   *pdfID = 0;
      pair<float,float>   *pdfX = 0;
      float pdfX_first;
      float pdfX_second;
      int pdfID_first;
      int pdfID_second;
      Float_t         qScale;
      TBranch        *b_ttbar_w;   //!
      TBranch        *b_pdfID;   //!
      TBranch        *b_pdfID_first;   //!
      TBranch        *b_pdfID_second;   //!
      TBranch        *b_pdfX;   //!
      TBranch        *b_pdfX_first;   //!
      TBranch        *b_pdfX_second;   //!
      TBranch        *b_qScale;   //!
      chainGen->SetBranchAddress("ttbar_w", &ttbar_w, &b_ttbar_w);
      chainGen->SetBranchAddress("pdfID", &pdfID, &b_pdfID);
      // chainGen->SetBranchAddress("first", &pdfID_first, &b_pdfID_first);
      // chainGen->SetBranchAddress("second", &pdfID_second, &b_pdfID_second);
      chainGen->SetBranchAddress("pdfX", &pdfX, &b_pdfX);
      // chainGen->SetBranchAddress("first", &pdfX_first, &b_pdfX_first);
      // chainGen->SetBranchAddress("second", &pdfX_second, &b_pdfX_second);
      chainGen->SetBranchAddress("qScale", &qScale, &b_qScale);
      chainGen->SetBranchStatus("*",0);
      if (!doreweight) {
         chainGen->SetBranchStatus("ttbar_w",1);
      } else {
         chainGen->SetBranchStatus("pdfID",1);
         chainGen->SetBranchStatus("first",1);
         chainGen->SetBranchStatus("second",1);
         chainGen->SetBranchStatus("pdfX",1);
         chainGen->SetBranchStatus("first",1);
         chainGen->SetBranchStatus("second",1);
         chainGen->SetBranchStatus("qScale",1);
      }
      float xp=0, xPb=0;
      int   IDp=0, IDPb=0, IDPb2=0;

		Bool_t isNLO = 0;
		if( !Sample.Contains("Pyquen") && (Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "TT" || Tag[i_tup].Contains("WE") || Tag[i_tup].Contains("WMu")) )
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
      if (Sample.Contains("CT14") || Sample.Contains("MG5")) // we only have 1 beam direction for CT14 and MG5
            norm = ( Xsec[i_tup] * lumi_all ) / (Double_t)nEvents[i_tup];

		cout << "\t[Normalization factor: " << norm << "]" << endl;

      // NEvents = 10000;
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

         TotWeight = norm * GenWeight;

         Bool_t GenFlag = kFALSE;
         GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

         if( GenFlag == kTRUE )
         {
            SumWeights_Separated += GenWeight;

            // -- Collect gen-level information -- //
            vector<GenLepton> GenLeptonCollection, GenLeptonCollectionBeforeAnyFSR;
            Int_t NGenLeptons = ntuple->gnpair; 
            for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
            {
               GenLepton genlep;
               genlep.FillFromNtuple(ntuple, i_gen);
               if( genlep.isMuon() && genlep.fromHardProcessFinalState )
                  GenLeptonCollection.push_back( genlep );
               if( genlep.isMuon() && genlep.isHardProcess )
                  GenLeptonCollectionBeforeAnyFSR.push_back( genlep );

               if( GenLeptonCollection.size() ==  2 )
                  break;
            }
            GenLepton genlep1 = GenLeptonCollection[0];
            GenLepton genlep2 = GenLeptonCollection[1];
            gen_M = (genlep1.Momentum + genlep2.Momentum).M();
            gen_Rap = (genlep1.Momentum + genlep2.Momentum).Rapidity()-dorapshift*rapshift;
            gen_Pt = (genlep1.Momentum + genlep2.Momentum).Pt();
            gen_Phistar = Object::phistar(genlep1,genlep2);

            // compute pre-FSR quantities 
            vector<GenLepton> GenLeptonCollection_FinalState;
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

            GenLepton genlep_postFSR1 = GenLeptonCollection_FinalState[0];
            GenLepton genlep_postFSR2 = GenLeptonCollection_FinalState[1];
            TLorentzVector tlv_postFSR = genlep_postFSR1.Momentum + genlep_postFSR2.Momentum;
            gen_M_post = tlv_postFSR.M();
            gen_Rap_post = tlv_postFSR.Rapidity()-dorapshift*rapshift;
            gen_Pt_post = tlv_postFSR.Pt();
            gen_Phistar_post = Object::phistar(genlep_postFSR1,genlep_postFSR2);

            Double_t dRCut = 0.1;

            GenLepton genlep_preFSR1 = genlep_postFSR1; // -- Copy the values of member variables -- // 
            vector< GenOthers > GenPhotonCollection1;
            analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR1, dRCut, &genlep_preFSR1, &GenPhotonCollection1);

            GenLepton genlep_preFSR2 = genlep_postFSR2; // -- Copy the values of member variables -- // 
            vector< GenOthers > GenPhotonCollection2;
            analyzer->PostToPreFSR_byDressedLepton_AllPhotons(ntuple, &genlep_postFSR2, dRCut, &genlep_preFSR2, &GenPhotonCollection2);

            // -- Mass, Pt, Rapidity Calculation -- //
            TLorentzVector tlv_preFSR = genlep_preFSR1.Momentum + genlep_preFSR2.Momentum;
            gen_M_pre = tlv_preFSR.M();
            gen_Rap_pre = tlv_preFSR.Rapidity()-dorapshift*rapshift;
            gen_Pt_pre = tlv_preFSR.Pt();
            gen_Phistar_pre = Object::phistar(genlep_preFSR1,genlep_preFSR2);

            // quantities before any FSR, just for comparison with MCFM
            GenLepton genlep_beforeAnyFSR1 = GenLeptonCollectionBeforeAnyFSR[0];
            GenLepton genlep_beforeAnyFSR2 = GenLeptonCollectionBeforeAnyFSR[1];
            TLorentzVector genlep_beforeAnyFSR12 = genlep_beforeAnyFSR1.Momentum + genlep_beforeAnyFSR2.Momentum;
            Double_t gen_M_beforeAnyFSR = genlep_beforeAnyFSR12.M();
            Double_t gen_Rap_beforeAnyFSR = genlep_beforeAnyFSR12.Rapidity()-dorapshift*rapshift;
            Double_t gen_Pt_beforeAnyFSR = genlep_beforeAnyFSR12.Pt();
            Double_t gen_Phistar_beforeAnyFSR = Object::phistar(genlep_beforeAnyFSR1,genlep_beforeAnyFSR2);

            // -- Flags -- //
            Flag_PassAcc = kFALSE;
            Flag_PassAcc_pre = kFALSE;

            // Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep1, genlep2);
            Flag_PassAcc = analyzer->isPassAccCondition_GenLepton(genlep_postFSR1, genlep_postFSR2);
            Flag_PassAcc_pre = analyzer->isPassAccCondition_GenLepton(genlep_preFSR1, genlep_preFSR2);
            Bool_t Flag_PassTotal_pre = (gen_Rap_pre > rapbin_60120[0] && gen_Rap_pre<rapbin_60120[rapbinnum_60120] );
            Bool_t Flag_PassTotal_post = (gen_Rap_post > rapbin_60120[0] && gen_Rap_post<rapbin_60120[rapbinnum_60120] );

            // fill tree
            if (storetree) tr->Fill();

            // the gen_M, etc. quantities are not very meaningful... replace them with post-FSR
            gen_M = gen_M_post;
            gen_Rap = gen_Rap_post;
            gen_Pt = gen_Pt_post;
            gen_Phistar = gen_Phistar_post;

            // -- Fill the mass histograms -- //
            h_mass->Fill( gen_M, TotWeight );
            h_mass_tot->Fill( gen_M, TotWeight );

            if (doreweight) {
               // xp = (doflip) ? pdfX_second : pdfX_first;
               // xPb = (doflip) ? pdfX_first : pdfX_second;
               // IDp = (doflip) ? pdfID_second : pdfID_first;
               // IDPb = (doflip) ? pdfID_first : pdfID_second;
               xp = (doflip) ? pdfX->second : pdfX->first;
               xPb = (doflip) ? pdfX->first : pdfX->second;
               IDp = (doflip) ? pdfID->second : pdfID->first;
               IDPb = (doflip) ? pdfID->first : pdfID->second;
               // if (doisospin && (abs(IDPb)==1 || abs(IDPb)==2)) { 
               //    // -- do / undo isospin effect --
               //    // we give a chance for the probed quark to "change flavour"
               //    // i.e. for the u in a proton to actually be a u in a neutron (d in a proton) and vice versa
               //    double xr = gRandom->Uniform(1);
               //    if (xr > 82./208.) {
               //       if (IDPb==-2) IDPb2=-1;
               //       if (IDPb==-1) IDPb2=-2;
               //       if (IDPb==2) IDPb2=1;
               //       if (IDPb==1) IDPb2=2;
               //    }
               // } else {
                  IDPb2=IDPb;
               // }

               h_IDp->Fill(IDp, TotWeight);
               h_IDPb->Fill(IDPb, TotWeight);
            }

            // I don't like the qScale found in the tree: let's change to our own one
            // qScale = sqrt(pow(gen_M, 2)+pow(gen_Pt,2));
            // qScale = sqrt(pow(gen_M, 2)+pow(qScale,2));
            qScale = gen_M;

            // -- Acceptance Calculation -- //
            if (!doreweight && ttbar_w->size()!=iwtmax) cout << i << " -> " << ttbar_w->size() << " " << iwtmax << endl;
            for (unsigned int iwt=0; iwt<iwtmax; iwt++) {
               // int iwt=0;
               double wt = TotWeight;

               //////////////////////////
               // COMPUTE PDF WEIGHT HERE
               //////////////////////////

               if (!doreweight) { // no PDF reweighting
                  // sometimes the last weight is missing... protect against this
                  if (iwt<ttbar_w->size()) wt = ttbar_w->at(iwt)*TotWeight;
                  else wt = (1./ttbar_w->at(iwt-1))*TotWeight;
               } else {
                  int ip = 0, iPb=0;
                  if (target_Pb.Contains("EPPS") && target_p.Contains("CT14")) { // https://www.jyu.fi/science/en/physics/research/highenergy/urhic/npdfs/epps16-nuclear-pdfs
                     if (iwt < 41) {
                        ip = 0;
                        iPb = iwt;
                     } else {
                        ip = iwt-40;
                        iPb = iwt;
                     }
                  } else if (target_p.Contains("TUJU") && target_Pb.Contains("TUJU")) {
                     if (iwt < 33) {
                        ip = 0;
                        iPb = iwt;
                     } else {
                        ip = iwt-32;
                        iPb = iwt;
                     }
                  } else if (target_p == target_Pb) {
                     ip = iwt;
                     iPb = iwt;
                  } else {
                     if (iwt < target_p_size) {
                        ip = iwt;
                        iPb = 0;
                     } else {
                        ip = 0;
                        iPb = iwt - target_p_size+1;
                     }
                  }

                  double wt_num = pdf_target_p[ip]->xfxQ(IDp,xp,qScale);
                  if (!doisospin) wt_num  *= pdf_target_Pb[iPb]->xfxQ(IDPb2,xPb,qScale);
                  else {
                     if (IDPb2==1 || IDPb2==2) wt_num *= ((82./208.) * pdf_target_Pb[iPb]->xfxQ(IDPb2,xPb,qScale) + (126./208.) * pdf_target_Pb[iPb]->xfxQ(3-IDPb2,xPb,qScale));
                     else if (IDPb2==-1 || IDPb2==-2) wt_num *= ((82./208.) * pdf_target_Pb[iPb]->xfxQ(IDPb2,xPb,qScale) + (126./208.) * pdf_target_Pb[iPb]->xfxQ(-3-IDPb2,xPb,qScale));
                     else wt_num  *= pdf_target_Pb[iPb]->xfxQ(IDPb2,xPb,qScale);
                  }
                  double wt_den = pdf_initial_p->xfxQ(IDp,xp,qScale) * pdf_initial_Pb->xfxQ(IDPb,xPb,qScale);
                  double wt_pdf = (wt_den != 0) ?  wt_num / wt_den : 0;
                  if (wt_pdf < 0.03) wt_pdf = 0.03;
                  if (wt_pdf > 30) wt_pdf = 30;
                  wt = TotWeight * wt_pdf;
                  // cout << iwt << ": " << IDp << " " << IDPb << " " << xp << " " << xPb << " " << qScale << " -- " << TotWeight << ", " << pdf_target_p[ip]->xfxQ(IDp,xp,qScale)<< ", " << pdf_target_Pb[iPb]->xfxQ(IDPb,xPb,qScale)<< ", " << pdf_initial_p->xfxQ(IDp,xp,qScale)<< ", " << pdf_initial_Pb->xfxQ(IDPb,xPb,qScale)<< endl;
               }

               //////////////////////////
               // END COMPUTE PDF WEIGHT
               //////////////////////////

               // MCFM histos
               h_m34_1[iwt]->Fill(gen_M_beforeAnyFSR, wt);
               h_m34_2[iwt]->Fill(gen_M_beforeAnyFSR, wt);
               if (gen_M_beforeAnyFSR>60 && gen_M_beforeAnyFSR<120) {
                  h_y34hM[iwt]->Fill(gen_Rap_beforeAnyFSR, wt);
                  h_pt34hM[iwt]->Fill(gen_Pt_beforeAnyFSR, wt);
                  h_phist34hM_1[iwt]->Fill(gen_Phistar_beforeAnyFSR, wt);
                  h_phist34hM_2[iwt]->Fill(gen_Phistar_beforeAnyFSR, wt);
               } else if (gen_M_beforeAnyFSR>15 && gen_M_beforeAnyFSR<60) {
                  h_y34lM[iwt]->Fill(gen_Rap_beforeAnyFSR, wt);
                  h_pt34lM[iwt]->Fill(gen_Pt_beforeAnyFSR, wt);
                  h_phist34lM_1[iwt]->Fill(gen_Phistar_beforeAnyFSR, wt);
                  h_phist34lM_2[iwt]->Fill(gen_Phistar_beforeAnyFSR, wt);
               }
               if (Flag_PassTotal_pre) {
                  h_m34cut_1[iwt]->Fill(gen_M_beforeAnyFSR, wt);
                  h_m34cut_2[iwt]->Fill(gen_M_beforeAnyFSR, wt);
               }

               if (Flag_PassTotal_pre) {
                  h_mass_AccTotal[iwt]->Fill( gen_M, wt );
                  h_mass3bins_AccTotal[iwt]->Fill( gen_M, wt );
                  if (gen_M_pre>60 && gen_M_pre<120) {
                     h_pt_AccTotal[iwt]->Fill( gen_Pt, wt );
                     h_phistar_AccTotal[iwt]->Fill( gen_Phistar, wt );
                     h_rap60120_AccTotal[iwt]->Fill( gen_Rap, wt );
                  } else if (gen_M_pre>15 && gen_M_pre<60) {
                     h_rap1560_AccTotal[iwt]->Fill( gen_Rap, wt );
                     h_pt1560_AccTotal[iwt]->Fill( gen_Pt, wt );
                     h_phistar1560_AccTotal[iwt]->Fill( gen_Phistar, wt );
                  }
               }
               if( Flag_PassAcc_pre == kTRUE ) 
               {
                  h_mass_AccPass[iwt]->Fill( gen_M, wt );
                  h_mass3bins_AccPass[iwt]->Fill( gen_M, wt );
                  if (gen_M_pre>60 && gen_M_pre<120) {
                     h_pt_AccPass[iwt]->Fill( gen_Pt, wt );
                     h_phistar_AccPass[iwt]->Fill( gen_Phistar, wt );
                     h_rap60120_AccPass[iwt]->Fill( gen_Rap, wt );
                  } else if (gen_M_pre>15 && gen_M_pre<60) {
                     h_rap1560_AccPass[iwt]->Fill( gen_Rap, wt );
                     h_pt1560_AccPass[iwt]->Fill( gen_Pt, wt );
                     h_phistar1560_AccPass[iwt]->Fill( gen_Phistar, wt );
                  }
               }

               if (Flag_PassTotal_pre) {
                  h_mass_AccTotal_pre[iwt]->Fill( gen_M_pre, wt );
                  h_mass3bins_AccTotal_pre[iwt]->Fill( gen_M_pre, wt );
                  if (gen_M_pre>60 && gen_M_pre<120) {
                     h_pt_AccTotal_pre[iwt]->Fill( gen_Pt_pre, wt );
                     h_phistar_AccTotal_pre[iwt]->Fill( gen_Phistar_pre, wt );
                     h_rap60120_AccTotal_pre[iwt]->Fill( gen_Rap_pre, wt );
                  } else if (gen_M_pre>15 && gen_M_pre<60) {
                     h_rap1560_AccTotal_pre[iwt]->Fill( gen_Rap_pre, wt );
                     h_pt1560_AccTotal_pre[iwt]->Fill( gen_Pt_pre, wt );
                     h_phistar1560_AccTotal_pre[iwt]->Fill( gen_Phistar_pre, wt );
                  }
               }
               if( Flag_PassAcc_pre == kTRUE ) 
               {
                  h_mass_AccPass_pre[iwt]->Fill( gen_M_pre, wt );
                  h_mass3bins_AccPass_pre[iwt]->Fill( gen_M_pre, wt );
                  if (gen_M_pre>60 && gen_M_pre<120) {
                     h_pt_AccPass_pre[iwt]->Fill( gen_Pt_pre, wt );
                     h_phistar_AccPass_pre[iwt]->Fill( gen_Phistar_pre, wt );
                     h_rap60120_AccPass_pre[iwt]->Fill( gen_Rap_pre, wt );
                  } else if (gen_M_pre>15 && gen_M_pre<60) {
                     h_rap1560_AccPass_pre[iwt]->Fill( gen_Rap_pre, wt );
                     h_pt1560_AccPass_pre[iwt]->Fill( gen_Pt_pre, wt );
                     h_phistar1560_AccPass_pre[iwt]->Fill( gen_Phistar_pre, wt );
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
	h_IDp->Write();
	h_IDPb->Write();

   for (unsigned int i=0; i<nweights; i++) {
      // MCFM
      dMCFM->cd();
      h_m34_1[i]->Write();
      h_m34_2[i]->Write();
      h_y34lM[i]->Write();
      h_pt34lM[i]->Write();
      h_phist34lM_1[i]->Write();
      h_phist34lM_2[i]->Write();
      h_y34hM[i]->Write();
      h_pt34hM[i]->Write();
      h_phist34hM_1[i]->Write();
      h_phist34hM_2[i]->Write();
      h_m34cut_1[i]->Write();
      h_m34cut_2[i]->Write();

      f->cd();
      h_mass_AccTotal[i]->Write();
      h_mass_AccTotal_pre[i]->Write();
      h_mass_AccPass[i]->Write();
      h_mass_AccPass_pre[i]->Write();
      h_mass3bins_AccTotal[i]->Write();
      h_mass3bins_AccTotal_pre[i]->Write();
      h_mass3bins_AccPass[i]->Write();
      h_mass3bins_AccPass_pre[i]->Write();
      h_pt_AccTotal[i]->Write();
      h_phistar_AccTotal[i]->Write();
      h_pt_AccTotal_pre[i]->Write();
      h_phistar_AccTotal_pre[i]->Write();
      h_pt_AccPass[i]->Write();
      h_pt_AccPass_pre[i]->Write();
      h_phistar_AccPass[i]->Write();
      h_phistar_AccPass_pre[i]->Write();
      h_rap1560_AccTotal[i]->Write();
      h_rap1560_AccTotal_pre[i]->Write();
      h_rap1560_AccPass[i]->Write();
      h_rap1560_AccPass_pre[i]->Write();
      h_rap60120_AccTotal[i]->Write();
      h_rap60120_AccTotal_pre[i]->Write();
      h_rap60120_AccPass[i]->Write();
      h_rap60120_AccPass_pre[i]->Write();
      h_pt1560_AccTotal[i]->Write();
      h_pt1560_AccTotal_pre[i]->Write();
      h_pt1560_AccPass[i]->Write();
      h_pt1560_AccPass_pre[i]->Write();
      h_phistar1560_AccTotal[i]->Write();
      h_phistar1560_AccTotal_pre[i]->Write();
      h_phistar1560_AccPass[i]->Write();
      h_phistar1560_AccPass_pre[i]->Write();
   }

   if (storetree) tr->Write();
   f->Close();

   if (pdfset_initial_p) delete pdfset_initial_p;
   if (pdfset_target_p && pdfset_target_p != pdfset_initial_p) delete pdfset_target_p;
   if (pdfset_initial_Pb && pdfset_initial_Pb != pdfset_initial_p && pdfset_initial_Pb != pdfset_target_p) delete pdfset_initial_Pb;
   if (pdfset_target_Pb && pdfset_target_Pb != pdfset_initial_p && pdfset_target_Pb != pdfset_target_p && pdfset_target_Pb != pdfset_initial_Pb) delete pdfset_target_Pb;

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


