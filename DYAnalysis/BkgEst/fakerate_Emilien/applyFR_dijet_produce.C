#include "../interface/analysis.h"
#include "../interface/defs.h"
#include "../interface/cuts.h"

#include <TH1.h>
#include <TFile.h>

using namespace DYana;

void applyFR_dijet_produce() {
   map<SampleTag,TH1D*> hmass_OS_dijet;
   map<SampleTag,TH1D*> hmass_SS_dijet;
   map<SampleTag,TH1D*> hmass_OS_dijet_etasplit;
   map<SampleTag,TH1D*> hmass_SS_dijet_etasplit;

   vector<SampleTag> v = allSamples();
   for (vector<SampleTag>::const_iterator it = v.begin(); it!=v.end(); it++) {
      TFile *f = TFile::Open("histograms/histDijetEmi" + TString(Name(*it)) + ".root");
      if (!f->IsOpen()) {
         cout << "ERROR could not find " << f->GetName() << endl;
         return;
      }

      hmass_OS_dijet[*it] = (TH1D*) f->Get("hmass_OS_dijet");
      hmass_SS_dijet[*it] = (TH1D*) f->Get("hmass_SS_dijet");
      hmass_OS_dijet_etasplit[*it] = (TH1D*) f->Get("hmass_OS_dijet_etasplit");
      hmass_SS_dijet_etasplit[*it] = (TH1D*) f->Get("hmass_SS_dijet_etasplit");
   }


   /////////////////////////////
   // compute MC SF from OS 2iso
   /////////////////////////////

   double sf_lumi = 0.93; // from computeFR

   //////////////
   // compute FR.
   //////////////

   TH1D *hmass_OS_dijet_datasub = new TH1D("hmass_OS_dijet_datasub","",binnum,bins);
   TH1D *hmass_SS_dijet_datasub = new TH1D("hmass_SS_dijet_datasub","",binnum,bins);
   TH1D *hmass_OS_dijet_etasplit_datasub = new TH1D("hmass_OS_dijet_etasplit_datasub","",binnum,bins);
   TH1D *hmass_SS_dijet_etasplit_datasub = new TH1D("hmass_SS_dijet_etasplit_datasub","",binnum,bins);

   hmass_OS_dijet_datasub->Add(hmass_OS_dijet[Data1]);
   hmass_SS_dijet_datasub->Add(hmass_SS_dijet[Data1]);
   hmass_OS_dijet_etasplit_datasub->Add(hmass_OS_dijet_etasplit[Data1]);
   hmass_SS_dijet_etasplit_datasub->Add(hmass_SS_dijet_etasplit[Data1]);
   hmass_OS_dijet_datasub->Add(hmass_OS_dijet[Data2]);
   hmass_SS_dijet_datasub->Add(hmass_SS_dijet[Data2]);
   hmass_OS_dijet_etasplit_datasub->Add(hmass_OS_dijet_etasplit[Data2]);
   hmass_SS_dijet_etasplit_datasub->Add(hmass_SS_dijet_etasplit[Data2]);

   // MC
   for(int itag=0;itag<Data1;itag++) {
      SampleTag tag = static_cast<SampleTag>(itag);
      double lumi = lumi_all;
      if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
         lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
      }
      double norm = (Xsec(tag)*lumi*sf_lumi)/Nevts(tag);

   hmass_OS_dijet_datasub->Add(hmass_OS_dijet[tag],-norm);
   hmass_SS_dijet_datasub->Add(hmass_SS_dijet[tag],-norm);
   hmass_OS_dijet_etasplit_datasub->Add(hmass_OS_dijet_etasplit[tag],-norm);
   hmass_SS_dijet_etasplit_datasub->Add(hmass_SS_dijet_etasplit[tag],-norm);
   }

   ///////////////////////
   // plot the obtained FR
   ///////////////////////

   // write histos to output file
   TFile *fout = TFile::Open("dijet.root","RECREATE");
   hmass_OS_dijet_datasub->Write();
   hmass_SS_dijet_datasub->Write();
   hmass_OS_dijet_etasplit_datasub->Write();
   hmass_SS_dijet_etasplit_datasub->Write();
   fout->Close();
}
