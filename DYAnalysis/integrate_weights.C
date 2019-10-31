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

// -- Customized Analyzer for Drel-Yan Analysis -- //
#include "Include/DYAnalyzer.h"
#include "Include/ControlPlots.h"
#include "HIstuff/HFweight.h"
#include "Include/tnp_weight.h"

void integrate_weights(HFweight::HFside rewmode = HFweight::HFside::both)
{
	DYAnalyzer *analyzer = new DYAnalyzer( "PAL3Mu12" );

	TString BaseLocation = "/eos/cms/store/group/phys_heavyions/dileptons/echapon/pA_8p16TeV/DYtuples/";

	//Each ntuple directory & corresponding Tags
	vector<TString> ntupleDirectory; vector<TString> Tag; vector<Double_t> Xsec; vector<Double_t> nEvents; vector< DYana::SampleTag > STags;

   analyzer->SetupMCsamples_v20180814("Powheg", &ntupleDirectory, &Tag, &Xsec, &nEvents, &STags);
   // // add QCD
   // SampleTag tag=QCD;
   // ntupleDirectory.push_back(NtupleDir(tag));
   // Tag.push_back(Name(tag));
   // Xsec.push_back(DYana_v20180814::Xsec(tag));
   // nEvents.push_back(Nevts(tag));
   // DYana_v20180814::SampleTag tag_Powheg = DYana_v20180814::SampleTag::QCD;
   // STags.push_back(tag_Powheg);

   // initialise the HF reweighting tool
   HFweight hftool;

	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
      double nevents=0;
      double sumweights_zpt_mGT60=0;
      double sumweights_zpt_mLT60=0;
      double sumweights_mGT60=0;
      double sumweights_mLT60=0;
      double sumweights_hiHF=0;

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
		ntuple->TurnOnBranches_HI();
		

		Bool_t isMC = true;
		analyzer->SetupPileUpReWeighting( isMC );

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
      // NEvents = 100000;
		for(Int_t i=0; i<NEvents; i++)
      {
         ntuple->GetEvent(i);

         Bool_t GenFlag = kFALSE;
         GenFlag = analyzer->SeparateDYLLSample_isHardProcess(Tag[i_tup], ntuple);

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
            Double_t gen_Pt = (GenLeptonCollection[0].Momentum + GenLeptonCollection[1].Momentum).Pt();
            Double_t gen_Rap = (GenLeptonCollection[0].Momentum + GenLeptonCollection[1].Momentum).Rapidity();
            Double_t gen_M = (GenLeptonCollection[0].Momentum + GenLeptonCollection[1].Momentum).M();
            Bool_t Flag = (gen_Rap > rapbin_60120[0] && gen_Rap<rapbin_60120[rapbinnum_60120] );
            if (!Flag) continue; // keep only events in the rapidity range

            double zptweight = zptWeight(gen_Pt,gen_M);

            if (gen_M<60) {
               sumweights_zpt_mLT60 += zptweight;
               sumweights_mLT60 += 1.;
            } else {
               sumweights_zpt_mGT60 += zptweight;
               sumweights_mGT60 += 1.;
            }
         }

         sumweights_hiHF += hftool.weight(ntuple->hiHF,rewmode); 

         nevents += 1;

      } //End of event iteration

      cout << "For sample " << Tag[i_tup] << ": " << endl;
      cout << "HF weight: " << sumweights_hiHF << " / " << nevents << " = " << sumweights_hiHF/nevents << endl;
      if (Tag[i_tup].Contains("DYMuMu")) {
      if (sumweights_mLT60>0) cout << "Zpt, M<60: " << sumweights_zpt_mLT60 << " / " << sumweights_mLT60 << " = " << sumweights_zpt_mLT60/sumweights_mLT60 << endl;
      if (sumweights_mGT60>0) cout << "Zpt, M>60: " << sumweights_zpt_mGT60 << " / " << sumweights_mGT60 << " = " << sumweights_zpt_mGT60/sumweights_mLT60 << endl;
      }
	} //end of i_tup iteration
}
