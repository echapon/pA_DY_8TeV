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
#include <TROOT.h>
#include <TApplication.h>
#include <vector>
#include <TMath.h>

// -- for Rochester Muon momentum correction -- //
#include "Include/roccor.2016.v3/RoccoR.cc"

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "Include/DYAnalyzer.h"
#include "Include/ControlPlots.h"
#include "../HIstuff/HFweight.h"
#include "Include/tnp_weight.h"

static inline void loadBar(int x, int n, int r, int w);
void MuonPlots(Bool_t isCorrected = kTRUE, 
      TString Type = "MC", 
      TString HLTname = "PAL3Mu12", 
      bool doHFrew = false, 
      HFweight::HFside rewmode = HFweight::HFside::both, 
      bool doTnPrew = false,
      int cor_s=0, int cor_m=0, bool zptrew = true, bool filltree = false)
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;
   bool doData = Type.Contains("Data");
	cout << "Type: " << Type << endl;
	TString isApplyMomCorr = "";
	if( isCorrected == kTRUE )
	{
		cout << "Apply Roochester Muon Momentum Correction..." << endl;
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

   TString srew("noHFrew");
   // if data, do not HF reweight
   if (doData) doHFrew = false;
   if (doHFrew) {
      if (rewmode==HFweight::HFside::both) srew="rewboth";
      else if (rewmode==HFweight::HFside::Ntracks) srew="rewNtracks";
      else if (rewmode==HFweight::HFside::plus) srew="rewplus";
      else if (rewmode==HFweight::HFside::minus) srew="rewminus";
   }
   TString stnprew = doTnPrew ? "tnprew" : "notnprew";
   TString srew2("");
   if (!zptrew) srew2 = "_noZptrew";

	TFile *f = new TFile("ROOTFile_Histogram_InvMass_" + HLTname + "_" + Type + "_" + isApplyMomCorr + "_" + srew + "_" + stnprew + srew2 + ".root", "RECREATE");

   // TString BaseLocation = gSystem->Getenv("KP_DATA_PATH");
   // TString BaseLocation = "/afs/cern.ch/work/e/echapon/public/DY_pA_2016/trees_20170518/";
	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";

	//Each ntuple directory & corresponding Tags
	vector<TString> ntupleDirectory; vector<TString> Tag; vector<Double_t> Xsec; vector<Double_t> nEvents; vector< DYana::SampleTag > STags;

   // if( !doData )
   // {
		analyzer->SetupMCsamples_v20180814(Type, &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);
      // add QCD
      SampleTag tag=QCD;
      ntupleDirectory.push_back(NtupleDir(tag));
      Tag.push_back(Name(tag));
      Xsec.push_back(DYana_v20180814::Xsec(tag));
      nEvents.push_back(Nevts(tag));
      DYana_v20180814::SampleTag tag_Powheg = DYana_v20180814::SampleTag::QCD;
      STags.push_back(tag_Powheg);
   // }
   // else
   // {
   //    using DYana::SampleTag;
   //    if (HLTname.Contains("L3Mu12")) {
   //          ntupleDirectory.push_back( "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_1stpart_20170518/170517_220343/0000/" ); 
   //          ntupleDirectory.push_back( "PASingleMuon/crab_PASingleMuon_DYtuple_PAL3Mu12_2ndpart_20170518/170517_220714/0000/" ); 
   //    } else {
   //          ntupleDirectory.push_back( "PADoubleMuon/crab_PADoubleMuon_DYtuple_PAL1DoubleMu0_1stpart_2010604/180604_092521/0000/" ); 
   //          ntupleDirectory.push_back( "PADoubleMuon/crab_PADoubleMuon_DYtuple_PAL1DoubleMu0_2ndpart_2010604/180604_093537/0000/" ); 
   //    }
   //    Tag.push_back( "Data1" ); STags.push_back(SampleTag::Data1);
   //    Tag.push_back( "Data2" ); STags.push_back(SampleTag::Data2);
   // }

   // initialise the HF reweighting tool
   HFweight hftool;

	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
      if (!doData && IsData(STags[i_tup])) continue;
      if (doData && !IsData(STags[i_tup])) continue;

      // if (!(Tag[i_tup].Contains("DYMuMu30"))) continue;
		TStopwatch looptime;
		looptime.Start();

      Bool_t doflip = (switcheta(STags[i_tup])<0);
      analyzer->sign = (doflip) ? -1 : 1;

		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TChain *chain = new TChain("recoTree/DYTree");
		chain->Add(BaseLocation + "/" + ntupleDirectory[i_tup]+"/ntuple_*.root");
      // if( Tag[i_tup].Contains("Data") && doData )
      // {
      //    // -- Run2015D -- // 
      //    chain->Add(BaseLocation+"/80X/v20170519/*.root");
      // }
		NtupleHandle *ntuple = new NtupleHandle( chain, doflip );
		ntuple->TurnOnBranches_GenLepton();
		ntuple->TurnOnBranches_GenOthers();
		ntuple->TurnOnBranches_Muon();
		ntuple->TurnOnBranches_HLT();
		ntuple->TurnOnBranches_HI();
		ntuple->TurnOnBranches_MET();
		
      RoccoR  rmcor("Include/roccor.2016.v3/rcdata.2016.v3"); //directory path as input for now; initialize only once, contains all variations

		Bool_t isMC;
		Tag[i_tup].Contains("Data") ? isMC = kFALSE : isMC = kTRUE;
		analyzer->SetupPileUpReWeighting( isMC );

		ControlPlots *Plots = new ControlPlots( Tag[i_tup], analyzer );
		ControlPlots_MET *Plots_MET = new ControlPlots_MET( Tag[i_tup], analyzer, ntuple );

		TH1D *h_PU = new TH1D("h_PU_"+Tag[i_tup], "", 50, 0, 50);
		TH1D *h_nVertices_before = new TH1D("h_nVertices_before_"+Tag[i_tup], "", 10, 0, 10);
		TH1D *h_nVertices_after = new TH1D("h_nVertices_after_"+Tag[i_tup], "", 10, 0, 10);

      // HI stuff
		TH1D *h_hiHF = new TH1D("h_hiHF_"+Tag[i_tup], "", 110, 0, 550);
		TH1D *h_hiHFplus = new TH1D("h_hiHFplus_"+Tag[i_tup], "", 110, 0, 550);
		TH1D *h_hiHFminus = new TH1D("h_hiHFminus_"+Tag[i_tup], "", 100, 0, 200);
		TH1D *h_hiHFplusEta4 = new TH1D("h_hiHFplusEta4_"+Tag[i_tup], "", 100, 0, 200);
		TH1D *h_hiHFminusEta4 = new TH1D("h_hiHFminusEta4_"+Tag[i_tup], "", 100, 0, 100);
		TH1D *h_hiHFhit = new TH1D("h_hiHFhit_"+Tag[i_tup], "", 180, 0, 9000);
		TH1D *h_hiHFhitPlus = new TH1D("h_hiHFhitPlus_"+Tag[i_tup], "", 120, 0, 6000);
		TH1D *h_hiHFhitMinus = new TH1D("h_hiHFhitMinus_"+Tag[i_tup], "", 120, 0, 6000);
		TH1D *h_hiET = new TH1D("h_hiET_"+Tag[i_tup], "", 150, 0, 300);
		TH1D *h_hiEE = new TH1D("h_hiEE_"+Tag[i_tup], "", 100, 0, 200);
		TH1D *h_hiEB = new TH1D("h_hiEB_"+Tag[i_tup], "", 100, 0, 200);
		TH1D *h_hiEEplus = new TH1D("h_hiEEplus_"+Tag[i_tup], "", 100, 0, 200);
		TH1D *h_hiEEminus = new TH1D("h_hiEEminus_"+Tag[i_tup], "", 100, 0, 200);
		TH1D *h_hiNpix = new TH1D("h_hiNpix_"+Tag[i_tup], "", 200, 0, 2000);
		TH1D *h_hiNtracks = new TH1D("h_hiNtracks_"+Tag[i_tup], "", 150, 0, 300);
		TH1D *h_hiNtracks_M1560 = new TH1D("h_hiNtracks_M1560_"+Tag[i_tup], "", 150, 0, 300);
		TH1D *h_hiNtracks_M60120 = new TH1D("h_hiNtracks_M60120_"+Tag[i_tup], "", 150, 0, 300);
		TH1D *h_hiNtracksPtCut = new TH1D("h_hiNtracksPtCut_"+Tag[i_tup], "", 150, 0, 300);

      // TTree
      f->cd();
      TTree *tr = new TTree("tr_" + Tag[i_tup],"tree " + Tag[i_tup]);
      float diMass; tr->Branch("diMass",&diMass,"diMass/F");
      float diRapidity; tr->Branch("diRapidity",&diRapidity,"diRapidity/F");
      float diPt; tr->Branch("diPt",&diPt,"diPt/F");
      float diPhistar; tr->Branch("diPhistar",&diPhistar,"diPhistar/F");
      float deltaPhi; tr->Branch("deltaPhi",&deltaPhi,"deltaPhi/F");
      float minpt; tr->Branch("minpt",&minpt,"minpt/F");
      float pt1; tr->Branch("pt1",&pt1,"pt1/F");
      float pt2; tr->Branch("pt2",&pt2,"pt2/F");
      float eta1; tr->Branch("eta1",&eta1,"eta1/F");
      float eta2; tr->Branch("eta2",&eta2,"eta2/F");
      float phi1; tr->Branch("phi1",&phi1,"phi1/F");
      float phi2; tr->Branch("phi2",&phi2,"phi2/F");
      float maxabseta; tr->Branch("maxabseta",&maxabseta,"maxabseta/F");
      float maxrelPFiso; tr->Branch("maxrelPFiso",&maxrelPFiso,"maxrelPFiso/F");
      float relPFiso1; tr->Branch("relPFiso1",&relPFiso1,"relPFiso1/F");
      float relPFiso2; tr->Branch("relPFiso2",&relPFiso2,"relPFiso2/F");
      float trkiso1; tr->Branch("trkiso1",&trkiso1,"trkiso1/F");
      float trkiso2; tr->Branch("trkiso2",&trkiso2,"trkiso2/F");
      int   isSoft1; tr->Branch("isSoft1",&isSoft1,"isSoft1/I");
      int   isSoft2; tr->Branch("isSoft2",&isSoft2,"isSoft2/I");
      int   isLoose1; tr->Branch("isLoose1",&isLoose1,"isLoose1/I");
      int   isLoose2; tr->Branch("isLoose2",&isLoose2,"isLoose2/I");
      int   isMedium1; tr->Branch("isMedium1",&isMedium1,"isMedium1/I");
      int   isMedium2; tr->Branch("isMedium2",&isMedium2,"isMedium2/I");
      int   isTight1; tr->Branch("isTight1",&isTight1,"isTight1/I");
      int   isTight2; tr->Branch("isTight2",&isTight2,"isTight2/I");
      int   isMyTight1; tr->Branch("isMyTight1",&isMyTight1,"isMyTight1/I");
      int   isMyTight2; tr->Branch("isMyTight2",&isMyTight2,"isMyTight2/I");
      float maxabsdxy; tr->Branch("maxabsdxy",&maxabsdxy,"maxabsdxy/F");
      float dxy1; tr->Branch("dxy1",&dxy1,"dxy1/F");
      float dxy2; tr->Branch("dxy2",&dxy2,"dxy2/F");
      float maxabsdz; tr->Branch("maxabsdz",&maxabsdz,"maxabsdz/F");
      float d01; tr->Branch("d01",&d01,"d01/F");
      float d02; tr->Branch("d02",&d02,"d02/F");
      float dsz1; tr->Branch("dsz1",&dsz1,"dsz1/F");
      float dsz2; tr->Branch("dsz2",&dsz2,"dsz2/F");
      float dz1; tr->Branch("dz1",&dz1,"dz1/F");
      float dz2; tr->Branch("dz2",&dz2,"dz2/F");
      float dxyBS1; tr->Branch("dxyBS1",&dxyBS1,"dxyBS1/F");
      float dxyBS2; tr->Branch("dxyBS2",&dxyBS2,"dxyBS2/F");
      float dszBS1; tr->Branch("dszBS1",&dszBS1,"dszBS1/F");
      float dszBS2; tr->Branch("dszBS2",&dszBS2,"dszBS2/F");
      float dzBS1; tr->Branch("dzBS1",&dzBS1,"dzBS1/F");
      float dzBS2; tr->Branch("dzBS2",&dzBS2,"dzBS2/F");
      float dxyVTX1; tr->Branch("dxyVTX1",&dxyVTX1,"dxyVTX1/F");
      float dxyVTX2; tr->Branch("dxyVTX2",&dxyVTX2,"dxyVTX2/F");
      float dszVTX1; tr->Branch("dszVTX1",&dszVTX1,"dszVTX1/F");
      float dszVTX2; tr->Branch("dszVTX2",&dszVTX2,"dszVTX2/F");
      float dzVTX1; tr->Branch("dzVTX1",&dzVTX1,"dzVTX1/F");
      float dzVTX2; tr->Branch("dzVTX2",&dzVTX2,"dzVTX2/F");
      float vtxnormchi2; tr->Branch("vtxnormchi2",&vtxnormchi2,"vtxnormchi2/F");
      float vtxprob; tr->Branch("vtxprob",&vtxprob,"vtxprob/F");
      float hiHF; tr->Branch("hiHF",&hiHF,"hiHF/F");
      float hiNtracks; tr->Branch("hiNtracks",&hiNtracks,"hiNtracks/F");
      float pfMET; tr->Branch("pfMET",&pfMET,"pfMET/F");
      float pfMETtype1; tr->Branch("pfMETtype1",&pfMETtype1,"pfMETtype1/F");
      int sign; tr->Branch("sign",&sign,"sign/I");
      float weight; tr->Branch("weight",&weight,"weight/F");
      // gen stuff (only muons)
      float genpt1, geneta1, genphi1;
      float genpt2, geneta2, genphi2;
      if (isMC) {
         tr->Branch("genpt1",&genpt1,"genpt1/F");
         tr->Branch("geneta1",&geneta1,"geneta1/F");
         tr->Branch("genphi1",&genphi1,"genphi1/F");
         tr->Branch("genpt2",&genpt2,"genpt2/F");
         tr->Branch("geneta2",&geneta2,"geneta2/F");
         tr->Branch("genphi2",&genphi2,"genphi2/F");
      }

		Bool_t isNLO = 0;
      if( Type=="Powheg" && (Tag[i_tup].Contains("DYMuMu") || Tag[i_tup].Contains("DYTauTau") || Tag[i_tup] == "WJets") )
      {
         isNLO = 1;
         cout << "\t" << Tag[i_tup] << ": generated with NLO mode - Weights are applied" << endl;
      }

		Double_t SumWeight = 0;
		Double_t SumWeight_Separated = 0;

		Double_t Norm = doData ? 1 : (lumi_all * Xsec[i_tup]) / nEvents[i_tup];
      if (IsDYMuMu(STags[i_tup])) {
         // combine pPb and PbP for DYMuMu
         if (doflip)
            Norm = ( Xsec[i_tup] * lumi_part1 ) / (Double_t)nEvents[i_tup];
         else 
            Norm = ( Xsec[i_tup] * lumi_part2 ) / (Double_t)nEvents[i_tup];
      }

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
      // NEvents = 100000;
		for(Int_t i=0; i<NEvents; i++)
      {
         loadBar(i+1, NEvents, 100, 100);

         ntuple->GetEvent(i);

         /////////////////////////////
         // -- Bring the weights -- //
         /////////////////////////////
         // -- Positive/Negative Gen-weights -- //
         Double_t GenWeight;
         if( isNLO == 1 )
            ntuple->GENEvt_weight < 0 ? GenWeight = -1 : GenWeight = 1;
         else
            GenWeight = 1;

         SumWeight += GenWeight;

         // -- Pileup-Reweighting -- //
         Double_t PUWeight = 1.; //analyzer->PileUpWeightValue( ntuple->nPileUp );
         if (doHFrew) PUWeight *= hftool.weight(ntuple->hiHF,rewmode); 

         // -- Tag and probe weights -- //
         Double_t TnpWeight = 1.;

         Bool_t GenFlag = kFALSE;
         GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

         if( GenFlag )
            SumWeight_Separated += GenWeight;

         // -- Fill the histogram for gen-level information (signal sample) -- //
         vector<GenLepton> GenLeptonCollection;
         if( GenFlag && Tag[i_tup].Contains("DYMuMu") )
         {
            Int_t NGenLeptons = ntuple->gnpair;
            for(Int_t i_gen=0; i_gen<NGenLeptons; i_gen++)
            {
               GenLepton genlep;
               genlep.FillFromNtuple(ntuple, i_gen);
               if( genlep.isMuon() && genlep.isHardProcess )
               {
                  GenLeptonCollection.push_back( genlep );

                  if( GenLeptonCollection.size() ==  2 )
                     break;
               }
            }

            // -- Z pt reweighting -- //
            if (zptrew) {
               Double_t gen_Pt = (GenLeptonCollection[0].Momentum + GenLeptonCollection[1].Momentum).Pt();
               Double_t gen_M = (GenLeptonCollection[0].Momentum + GenLeptonCollection[1].Momentum).M();
               GenWeight *= zptWeight(gen_Pt,gen_M);
            }

            Plots->FillHistograms_GenDoubleMu(ntuple, GenLeptonCollection[0], GenLeptonCollection[1], GenWeight);
            genpt1 = GenLeptonCollection[0].Pt;
            geneta1 = GenLeptonCollection[0].eta;
            genphi1 = GenLeptonCollection[0].phi;
            genpt2 = GenLeptonCollection[1].Pt;
            geneta2 = GenLeptonCollection[1].eta;
            genphi2 = GenLeptonCollection[1].phi;
         } else {
            genpt1=-1; geneta1=999; genphi1=999;
            genpt2=-2; geneta2=999; genphi2=999;
         }

         if( ntuple->isTriggered( analyzer->HLT )  && GenFlag) 
         {
            //Collect Reconstruction level information
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
                        double drmin=0.2; double pt_drmin=0;
                        for (unsigned int igen=0; igen<GenLeptonCollection.size(); igen++) {
                           double dr = mu.Momentum.DeltaR(GenLeptonCollection[igen].Momentum);
                           if (dr<drmin) {
                              drmin = dr;
                              pt_drmin = GenLeptonCollection[igen].Pt;
                           }
                        } // for igen in GenLeptonCollection (gen-reco matching)
                        if (drmin<0.1) 
                           qter = rmcor.kScaleFromGenMC(mu.charge, mu.Pt, analyzer->sign*mu.eta, mu.phi, nl, pt_drmin, u1, s, m);
                        else  {
                           double u2 = gRandom->Rndm();
                           qter = rmcor.kScaleAndSmearMC(mu.charge, mu.Pt, analyzer->sign*mu.eta, mu.phi, nl, u1, u2, s, m);
                        } // if drmin<0.1
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
            vector< Muon > SelectedMuonCollection_noiso;
            bool isPassEventSelection_noiso = !filltree || analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection_noiso,true,1e99,false);
            vector< Muon > SelectedMuonCollection;
            Bool_t isPassEventSelection = kFALSE;
            if (isPassEventSelection_noiso) isPassEventSelection = analyzer->EventSelection(MuonCollection, ntuple, &SelectedMuonCollection); 

            Muon mu1;
            Muon mu2;
            TLorentzVector dimu; 

            if( isPassEventSelection == kTRUE )
            {
               mu1 = SelectedMuonCollection[0];
               mu2 = SelectedMuonCollection[1];
               pt1 = mu1.Pt;
               pt2 = mu2.Pt;
               eta1 = mu1.eta;
               eta2 = mu2.eta;
               double aeta1 = fabs(eta1);
               double aeta2 = fabs(eta2);

               // TnP
               if (doTnPrew) {
                  if (HLTname.Contains("L1DoubleMu")) {
                     TnpWeight = tnp_weight_muid_ppb(pt1,eta1,0)*tnp_weight_iso_ppb(pt1,aeta1,0)
                        *tnp_weight_muid_ppb(pt2,eta2,0)*tnp_weight_iso_ppb(pt2,aeta2,0);
                  } else if (HLTname.Contains("L3Mu12")) {
                     // L3Mu12 uses rel tk iso
                     // TnpWeight = tnp_weight_muid_ppb(pt1,eta1,0)*tnp_weight_isotk_ppb(pt1,aeta1,0)
                     //    *tnp_weight_muid_ppb(pt2,eta2,0)*tnp_weight_isotk_ppb(pt2,aeta2,0);
                     TnpWeight = tnp_weight_muid_ppb(pt1,eta1,0)*tnp_weight_isotk_ppb(pt1,aeta1,0)
                        *tnp_weight_muid_ppb(pt2,eta2,0)*tnp_weight_isotk_ppb(pt2,aeta2,0);
                  } else {
                     cerr << "ERROR trigger should be L1DoubleMuOpen or L3Mu12" << endl;
                     TnpWeight = 1.;
                  }
                  // add trg... careful!
                  double sf_trg=1.;
                  if (HLTname.Contains("L3Mu12")) {
                     if (pt2>=15. && pt1>=15.) { // both muons could trigger
                        double eff_data = (1 - (1 - tnp_weight_L3Mu12_ppb(eta1,200)*tnp_weight_L3Mu12_ppb(eta1,0)/tnp_weight_L3Mu12_ppb(eta1,0)) * (1 - tnp_weight_L3Mu12_ppb(eta2,200)*tnp_weight_L3Mu12_ppb(eta2,0)/tnp_weight_L3Mu12_ppb(eta2,0)) );
                        double eff_mc = (1 - (1 - tnp_weight_L3Mu12_ppb(eta1,300)) * (1 - tnp_weight_L3Mu12_ppb(eta2,300)) );
                        sf_trg = eff_data/eff_mc;
                     } else if (pt1<15) {
                        sf_trg = tnp_weight_L3Mu12_ppb(eta2,0);
                     } else if (pt2<15) {
                        sf_trg = tnp_weight_L3Mu12_ppb(eta1,0);
                     }
                  } else if (HLTname.Contains("L1DoubleMuOpen")) { // L1DoubleMuOpen case
                     sf_trg = tnp_weight_L1DMOpen_ppb(eta1,0) * tnp_weight_L1DMOpen_ppb(eta2,0);
                  }
                  else { // L1DoubleMu0
                     sf_trg = tnp_weight_L1DM0_ppb(eta1,0) * tnp_weight_L1DM0_ppb(eta2,0);
                  }
                  TnpWeight = TnpWeight * sf_trg;
               }

               double TotWeight = (isMC) ? GenWeight*PUWeight*TnpWeight : 1.;
               Plots->FillHistograms_DoubleMu(ntuple, mu1, mu2, TotWeight);
               Plots_MET->FillHistograms_MET(TotWeight);

               Int_t PU = ntuple->nPileUp;
               h_PU->Fill( PU, PUWeight*TnpWeight );

               Int_t nVertices = ntuple->nVertices;
               h_nVertices_before->Fill(nVertices, isMC ? GenWeight*TnpWeight : 1.);
               h_nVertices_after->Fill(nVertices, TotWeight);

               h_hiHF->Fill(ntuple->hiHF,TotWeight);
               h_hiHFplus->Fill(ntuple->hiHFplus,TotWeight);
               h_hiHFminus->Fill(ntuple->hiHFminus,TotWeight);
               h_hiHFplusEta4->Fill(ntuple->hiHFplusEta4,TotWeight);
               h_hiHFminusEta4->Fill(ntuple->hiHFminusEta4,TotWeight);
               h_hiHFhit->Fill(ntuple->hiHFhit,TotWeight);
               h_hiHFhitPlus->Fill(ntuple->hiHFhitPlus,TotWeight);
               h_hiHFhitMinus->Fill(ntuple->hiHFhitMinus,TotWeight);
               h_hiET->Fill(ntuple->hiET,TotWeight);
               h_hiEE->Fill(ntuple->hiEE,TotWeight);
               h_hiEB->Fill(ntuple->hiEB,TotWeight);
               h_hiEEplus->Fill(ntuple->hiEEplus,TotWeight);
               h_hiEEminus->Fill(ntuple->hiEEminus,TotWeight);
               h_hiNpix->Fill(ntuple->hiNpix,TotWeight);
               h_hiNtracks->Fill(ntuple->hiNtracks,TotWeight);
               h_hiNtracksPtCut->Fill(ntuple->hiNtracksPtCut,TotWeight);
               TLorentzVector dimu = mu1.Momentum+mu2.Momentum; 
               double mass = dimu.M();
               if (mass>=15 && mass<60) h_hiNtracks_M1560->Fill(ntuple->hiNtracks,TotWeight);
               else if (mass>=60 && mass<120) h_hiNtracks_M60120->Fill(ntuple->hiNtracks,TotWeight);
            }
            if (filltree && isPassEventSelection_noiso) {
               hiHF = ntuple->hiHF;
               hiNtracks = ntuple->hiNtracks;
               pfMET = ntuple->pfMET_pT;
               pfMETtype1 = ntuple->pfMET_Type1_pT;
               mu1 = SelectedMuonCollection_noiso[0];
               mu2 = SelectedMuonCollection_noiso[1];
               pt1 = mu1.Pt;
               pt2 = mu2.Pt;
               eta1 = mu1.eta;
               eta2 = mu2.eta;
               phi1 = mu1.phi;
               phi2 = mu2.phi;
               dimu = mu1.Momentum+mu2.Momentum; 
               // variables for the tree
               diMass = dimu.M(); diRapidity = dimu.Rapidity(); diPt = dimu.Pt();
               diPhistar = Object::phistar(mu1,mu2);
               deltaPhi = mu1.Momentum.DeltaPhi(mu2.Momentum);
               minpt = min(pt1,pt2);
               maxabseta = max(fabs(eta1),fabs(eta2));
               maxrelPFiso = max(mu1.relPFiso,mu2.relPFiso);
               relPFiso1 = mu1.relPFiso;
               relPFiso2 = mu2.relPFiso;
               trkiso1 = mu1.trkiso;
               trkiso2 = mu2.trkiso;
               isLoose1 = mu1.isLoose;
               isLoose2 = mu2.isLoose;
               isMedium1 = mu1.isMedium;
               isMedium2 = mu2.isMedium;
               isTight1 = mu1.isTight;
               isTight2 = mu2.isTight;
               isSoft1 = mu1.isSoft;
               isSoft2 = mu2.isSoft;
               isMyTight1 = mu1.isTightMuon();
               isMyTight2 = mu2.isTightMuon();
               maxabsdxy = max(fabs(mu1.dxyVTX),fabs(mu2.dxyVTX));
               dxy1 = mu1.dxy;
               dxy2 = mu2.dxy;
               d01 = mu1.d0;
               d02 = mu2.d0;
               dsz1 = mu1.dsz;
               dsz2 = mu2.dsz;
               dz1 = mu1.dz;
               dz2 = mu2.dz;
               dxyBS1 = mu1.dxyBS;
               dxyBS2 = mu2.dxyBS;
               dszBS1 = mu1.dszBS;
               dszBS2 = mu2.dszBS;
               dzBS1 = mu1.dzBS;
               dzBS2 = mu2.dzBS;
               dxyVTX1 = mu1.dxyVTX;
               dxyVTX2 = mu2.dxyVTX;
               dszVTX1 = mu1.dszVTX;
               dszVTX2 = mu2.dszVTX;
               dzVTX1 = mu1.dzVTX;
               dzVTX2 = mu2.dzVTX;
               maxabsdz = max(fabs(mu1.dzVTX),fabs(mu2.dzVTX));
               double vtxprobD = -999;
               double vtxnormchi2D = 999;
               analyzer->DimuonVertexProbNormChi2(ntuple, mu1.Inner_pT, mu2.Inner_pT, &vtxprobD, &vtxnormchi2D);
               vtxprob = vtxprobD;
               vtxnormchi2 = vtxnormchi2D;
               weight = GenWeight*PUWeight*TnpWeight*Norm;
               sign = abs(mu1.charge+mu2.charge);
               tr->Fill();
            }
         } //End of if( isTriggered )

      } //End of event iteration

		Plots->WriteHistograms( f );
		Plots_MET->WriteHistograms( f );
		h_PU->Write();
		h_nVertices_before->Write();
		h_nVertices_after->Write();
      h_hiHF->Write();
      h_hiHFplus->Write();
      h_hiHFminus->Write();
      h_hiHFplusEta4->Write();
      h_hiHFminusEta4->Write();
      h_hiHFhit->Write();
      h_hiHFhitPlus->Write();
      h_hiHFhitMinus->Write();
      h_hiET->Write();
      h_hiEE->Write();
      h_hiEB->Write();
      h_hiEEplus->Write();
      h_hiEEminus->Write();
      h_hiNpix->Write();
      h_hiNtracks->Write();
      h_hiNtracks_M1560->Write();
      h_hiNtracks_M60120->Write();
      h_hiNtracksPtCut->Write();
      if (filltree) tr->Write();

		if(isNLO == 1)
		{
			printf("\tTotal sum of weights: %.1lf\n", SumWeight);
			printf("\tSum of weights of Seperated events: %.1lf\n", SumWeight_Separated);
		}

		Double_t LoopRunTime = looptime.CpuTime();
		cout << "\tLoop RunTime(" << Tag[i_tup] << "): " << LoopRunTime << " seconds\n" << endl;
	} //end of i_tup iteration

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

