#include "../interface/analysis.h"
#include "../interface/defs.h"
#include "../interface/cuts.h"

#include <TH1.h>
#include <TFile.h>

using namespace DYana;

void addToHist(TH1D *h, int i, double integral, double interror);

void computeFR(TString tag = "1mu") {// possibilities: 1mu, 2muSS, 2muSS_lochi2, 2muSS_hichi2
   // load histos: hmass_OS_2iso, h2d_1mu_HLTL3Mu12, h2d_1mu_HLTL3Mu12_barrel, h2d_1mu_HLTL3Mu12_endcap

   map<SampleTag,TH1D*> hmass_OS_2iso;
   map<SampleTag,TH2D*> h2d_1mu;
   map<SampleTag,TH2D*> h2d_1mu_barrel;
   map<SampleTag,TH2D*> h2d_1mu_endcap;

   vector<SampleTag> v = allSamples();
   v.push_back(QCD);
   for (vector<SampleTag>::const_iterator it = v.begin(); it!=v.end(); it++) {
      TFile *f = TFile::Open("histograms/histFREmi" + TString(Name(*it)) + ".root");
      if (!f->IsOpen()) {
         cout << "ERROR could not find " << f->GetName() << endl;
         return;
      }

      hmass_OS_2iso[*it] = (TH1D*) f->Get("hmass_OS_2iso");
      h2d_1mu[*it] = (TH2D*) f->Get("h2d_" + tag + "_HLTL3Mu12");
      h2d_1mu_barrel[*it] = (TH2D*) f->Get("h2d_" + tag + "_HLTL3Mu12_barrel");
      h2d_1mu_endcap[*it] = (TH2D*) f->Get("h2d_" + tag + "_HLTL3Mu12_endcap");
   }


   /////////////////////////////
   // compute MC SF from OS 2iso
   /////////////////////////////

   double sf_lumi = 1.;

   // integrate between 76 and 106
   int ibin1 = hmass_OS_2iso[Data1]->FindBin(77);
   int ibin2 = hmass_OS_2iso[Data1]->FindBin(105);

   // data
   double sumData = 0.;
   sumData += hmass_OS_2iso[Data1]->Integral(ibin1,ibin2);
   sumData += hmass_OS_2iso[Data2]->Integral(ibin1,ibin2);

   // MC
   double sumMC = 0.;
   for(int i=0;i<Data1;i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      double lumi = lumi_all;
      if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
         lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
      }
      double norm = (Xsec(tag)*lumi)/Nevts(tag);
      double toadd = norm*(hmass_OS_2iso[tag]->Integral(ibin1,ibin2));
      cout << Name(tag) << ": " << norm << ", " << toadd << endl;
      sumMC += toadd;
   }

   sf_lumi = sumData / sumMC;
   cout << sf_lumi << " = " << sumData << " / " << sumMC << endl;


   //////////////
   // compute FR.
   //////////////

   TH1D *hFR = new TH1D("hFR","",ptbinnum,ptbin);
   TH1D *hFR_barrel = new TH1D("hFR_barrel","",ptbinnum,ptbin);
   TH1D *hFR_endcap = new TH1D("hFR_endcap","",ptbinnum_endcap,ptbin_endcap);
   TH1D *hFR2 = new TH1D("hFR2","",ptbinnum,ptbin);
   TH1D *hFR2_barrel = new TH1D("hFR2_barrel","",ptbinnum,ptbin);
   TH1D *hFR2_endcap = new TH1D("hFR2_endcap","",ptbinnum_endcap,ptbin_endcap);

   TH1D *hFR_QCD = new TH1D("hFR_QCD","",ptbinnum,ptbin);
   TH1D *hFR_QCD_barrel = new TH1D("hFR_QCD_barrel","",ptbinnum,ptbin);
   TH1D *hFR_QCD_endcap = new TH1D("hFR_QCD_endcap","",ptbinnum_endcap,ptbin_endcap);
   TH1D *hFR2_QCD = new TH1D("hFR2_QCD","",ptbinnum,ptbin);
   TH1D *hFR2_QCD_barrel = new TH1D("hFR2_QCD_barrel","",ptbinnum,ptbin);
   TH1D *hFR2_QCD_endcap = new TH1D("hFR2_QCD_endcap","",ptbinnum_endcap,ptbin_endcap);

   // a. do data-MC on both iso and noniso to obtain data-driven QCD. CAREFUL are there negative bins? what to do with them?
   double imaxiso = h2d_1mu[Data1]->ProjectionY(Form("_py"),1,1)->FindBin(cuts::isomax-1e-3);
   double imaxnoniso = h2d_1mu[Data1]->ProjectionY(Form("_py"),1,1)->GetNbinsX();

   // histos needed: data_iso, data_noniso, MC_iso, MC_noniso, for all 3 eta regions, + FR histos for those regions
   TH1D *hdataiso = new TH1D("hdataiso","",ptbinnum,ptbin);
   TH1D *hdataiso_barrel = new TH1D("hdataiso_barrel","",ptbinnum,ptbin);
   TH1D *hdataiso_endcap = new TH1D("hdataiso_endcap","",ptbinnum_endcap,ptbin_endcap);
   TH1D *hdatanoniso = new TH1D("hdatanoniso","",ptbinnum,ptbin);
   TH1D *hdatanoniso_barrel = new TH1D("hdatanoniso_barrel","",ptbinnum,ptbin);
   TH1D *hdatanoniso_endcap = new TH1D("hdatanoniso_endcap","",ptbinnum_endcap,ptbin_endcap);

   TH1D *hdatasubiso = new TH1D("hdatasubiso","",ptbinnum,ptbin);
   TH1D *hdatasubiso_barrel = new TH1D("hdatasubiso_barrel","",ptbinnum,ptbin);
   TH1D *hdatasubiso_endcap = new TH1D("hdatasubiso_endcap","",ptbinnum_endcap,ptbin_endcap);
   TH1D *hdatasubnoniso = new TH1D("hdatasubnoniso","",ptbinnum,ptbin);
   TH1D *hdatasubnoniso_barrel = new TH1D("hdatasubnoniso_barrel","",ptbinnum,ptbin);
   TH1D *hdatasubnoniso_endcap = new TH1D("hdatasubnoniso_endcap","",ptbinnum_endcap,ptbin_endcap);

   TH1D *hMCiso = new TH1D("hMCiso","",ptbinnum,ptbin);
   TH1D *hMCiso_barrel = new TH1D("hMCiso_barrel","",ptbinnum,ptbin);
   TH1D *hMCiso_endcap = new TH1D("hMCiso_endcap","",ptbinnum_endcap,ptbin_endcap);
   TH1D *hMCnoniso = new TH1D("hMCnoniso","",ptbinnum,ptbin);
   TH1D *hMCnoniso_barrel = new TH1D("hMCnoniso_barrel","",ptbinnum,ptbin);
   TH1D *hMCnoniso_endcap = new TH1D("hMCnoniso_endcap","",ptbinnum_endcap,ptbin_endcap);

   TH1D *hQCDiso = new TH1D("hQCDiso","",ptbinnum,ptbin);
   TH1D *hQCDiso_barrel = new TH1D("hQCDiso_barrel","",ptbinnum,ptbin);
   TH1D *hQCDiso_endcap = new TH1D("hQCDiso_endcap","",ptbinnum_endcap,ptbin_endcap);
   TH1D *hQCDnoniso = new TH1D("hQCDnoniso","",ptbinnum,ptbin);
   TH1D *hQCDnoniso_barrel = new TH1D("hQCDnoniso_barrel","",ptbinnum,ptbin);
   TH1D *hQCDnoniso_endcap = new TH1D("hQCDnoniso_endcap","",ptbinnum_endcap,ptbin_endcap);

   double integral, interror;
   double binc,bine;

   // inclusive and barrel
   for (int i=1; i<=ptbinnum; i++) {
      // data
      integral = h2d_1mu[Data1]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hdataiso,i,integral,interror);
      integral = h2d_1mu[Data2]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hdataiso,i,integral,interror);

      integral = h2d_1mu_barrel[Data1]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hdataiso_barrel,i,integral,interror);
      integral = h2d_1mu_barrel[Data2]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hdataiso_barrel,i,integral,interror);

      integral = h2d_1mu[Data1]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hdatanoniso,i,integral,interror);
      integral = h2d_1mu[Data2]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hdatanoniso,i,integral,interror);

      integral = h2d_1mu_barrel[Data1]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hdatanoniso_barrel,i,integral,interror);
      integral = h2d_1mu_barrel[Data2]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hdatanoniso_barrel,i,integral,interror);

      // MC
      for(int itag=0;itag<Data1;itag++) {
         SampleTag tag = static_cast<SampleTag>(itag);
         double lumi = lumi_all;
         if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
            lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
         }
         double norm = (Xsec(tag)*lumi*sf_lumi)/Nevts(tag);

         integral = h2d_1mu[tag]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
         addToHist(hMCiso,i,integral*norm,interror*norm);
         integral = h2d_1mu_barrel[tag]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
         addToHist(hMCiso_barrel,i,integral*norm,interror*norm);

         integral = h2d_1mu[tag]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
         addToHist(hMCnoniso,i,integral*norm,interror*norm);
         integral = h2d_1mu_barrel[tag]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
         addToHist(hMCnoniso_barrel,i,integral*norm,interror*norm);
      }

      // QCD
      integral = h2d_1mu[QCD]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hQCDiso,i,integral,interror);
      integral = h2d_1mu_barrel[QCD]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hQCDiso_barrel,i,integral,interror);

      integral = h2d_1mu[QCD]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hQCDnoniso,i,integral,interror);
      integral = h2d_1mu_barrel[QCD]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hQCDnoniso_barrel,i,integral,interror);
   }

   // endcap
   for (int i=1; i<=ptbinnum_endcap; i++) {
      // data
      integral = h2d_1mu_endcap[Data1]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hdataiso_endcap,i,integral,interror);
      integral = h2d_1mu_endcap[Data2]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hdataiso_endcap,i,integral,interror);

      integral = h2d_1mu_endcap[Data1]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hdatanoniso_endcap,i,integral,interror);
      integral = h2d_1mu_endcap[Data2]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hdatanoniso_endcap,i,integral,interror);

      // MC
      for(int itag=0;itag<Data1;itag++) {
         SampleTag tag = static_cast<SampleTag>(itag);
         double lumi = lumi_all;
         if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
            lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
         }
         double norm = (Xsec(tag)*lumi*sf_lumi)/Nevts(tag);

         integral = h2d_1mu_endcap[tag]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
         addToHist(hMCiso_endcap,i,integral*norm,interror*norm);

         integral = h2d_1mu_endcap[tag]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
         addToHist(hMCnoniso_endcap,i,integral*norm,interror*norm);
      }

      // QCD
      integral = h2d_1mu_endcap[QCD]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(0,imaxiso,interror);
      addToHist(hQCDiso_endcap,i,integral,interror);
      integral = h2d_1mu_endcap[QCD]->ProjectionY(Form("_py_%d",i),i,i)->IntegralAndError(imaxiso+1,imaxnoniso,interror);
      addToHist(hQCDnoniso_endcap,i,integral,interror);
   }

   // data-MC
   hdatasubiso->Add(hdataiso);
   hdatasubiso_barrel->Add(hdataiso_barrel);
   hdatasubiso_endcap->Add(hdataiso_endcap);
   hdatasubnoniso->Add(hdatanoniso);
   hdatasubnoniso_barrel->Add(hdatanoniso_barrel);
   hdatasubnoniso_endcap->Add(hdatanoniso_endcap);

   hdatasubiso->Add(hMCiso,-1);
   hdatasubiso_barrel->Add(hMCiso_barrel,-1);
   hdatasubiso_endcap->Add(hMCiso_endcap,-1);
   hdatasubnoniso->Add(hMCnoniso,-1);
   hdatasubnoniso_barrel->Add(hMCnoniso_barrel,-1);
   hdatasubnoniso_endcap->Add(hMCnoniso_endcap,-1);

   // b. the FR is iso / (iso+noniso). do it for each bin in pt, in 3 eta regions: inclusive, barrel, endcap
   for (int i=1; i<=ptbin[ptbinnum]; i++) {
      double niso = hdatasubiso->GetBinContent(i);
      double nnoniso = hdatasubnoniso->GetBinContent(i);
      double dniso = hdatasubiso->GetBinError(i);
      double dnnoniso = hdatasubnoniso->GetBinError(i);
      
      if (niso>0 && nnoniso>0) {
         hFR2->SetBinContent(i, niso/(niso+nnoniso));
         hFR2->SetBinError(i, sqrt(pow(dniso * nnoniso / pow(niso+nnoniso,2),2) + pow(dnnoniso * niso / pow(niso+nnoniso,2),2)));
         hFR->SetBinContent(i, nnoniso>0 ? niso/nnoniso : 1);
         hFR->SetBinError(i, nnoniso>0&&niso>0 ? (niso / nnoniso) * sqrt(pow(dniso/niso,2)+pow(dnnoniso/nnoniso,2)) : 1);
      }

      niso = hdatasubiso_barrel->GetBinContent(i);
      nnoniso = hdatasubnoniso_barrel->GetBinContent(i);
      dniso = hdatasubiso_barrel->GetBinError(i);
      dnnoniso = hdatasubnoniso_barrel->GetBinError(i);

      if (niso>0 && nnoniso>0) {
         hFR2_barrel->SetBinContent(i, niso/(niso+nnoniso));
         hFR2_barrel->SetBinError(i, sqrt(pow(dniso * nnoniso / pow(niso+nnoniso,2),2) + pow(dnnoniso * niso / pow(niso+nnoniso,2),2)));
         hFR_barrel->SetBinContent(i, nnoniso>0 ? niso/nnoniso : 1);
         hFR_barrel->SetBinError(i, nnoniso>0&&niso>0 ? (niso / nnoniso) * sqrt(pow(dniso/niso,2)+pow(dnnoniso/nnoniso,2)) : 1);
      }

      // from QCD MC instead of data
      niso = hQCDiso->GetBinContent(i);
      nnoniso = hQCDnoniso->GetBinContent(i);
      dniso = hQCDiso->GetBinError(i);
      dnnoniso = hQCDnoniso->GetBinError(i);
      
      if (niso>0 && nnoniso>0) {
         hFR2_QCD->SetBinContent(i, niso/(niso+nnoniso));
         hFR2_QCD->SetBinError(i, sqrt(pow(dniso * nnoniso / pow(niso+nnoniso,2),2) + pow(dnnoniso * niso / pow(niso+nnoniso,2),2)));
         hFR_QCD->SetBinContent(i, nnoniso>0 ? niso/nnoniso : 1);
         hFR_QCD->SetBinError(i, nnoniso>0&&niso>0 ? (niso / nnoniso) * sqrt(pow(dniso/niso,2)+pow(dnnoniso/nnoniso,2)) : 1);
      }

      niso = hQCDiso_barrel->GetBinContent(i);
      nnoniso = hQCDnoniso_barrel->GetBinContent(i);
      dniso = hQCDiso_barrel->GetBinError(i);
      dnnoniso = hQCDnoniso_barrel->GetBinError(i);

      if (niso>0 && nnoniso>0) {
         hFR2_QCD_barrel->SetBinContent(i, niso/(niso+nnoniso));
         hFR2_QCD_barrel->SetBinError(i, sqrt(pow(dniso * nnoniso / pow(niso+nnoniso,2),2) + pow(dnnoniso * niso / pow(niso+nnoniso,2),2)));
         hFR_QCD_barrel->SetBinContent(i, nnoniso>0 ? niso/nnoniso : 1);
         hFR_QCD_barrel->SetBinError(i, nnoniso>0&&niso>0 ? (niso / nnoniso) * sqrt(pow(dniso/niso,2)+pow(dnnoniso/nnoniso,2)) : 1);
      }
   }
   for (int i=1; i<=ptbin_endcap[ptbinnum_endcap]; i++) {
      double niso = hdatasubiso_endcap->GetBinContent(i);
      double nnoniso = hdatasubnoniso_endcap->GetBinContent(i);
      double dniso = hdatasubiso_endcap->GetBinError(i);
      double dnnoniso = hdatasubnoniso_endcap->GetBinError(i);

      if (niso>0 && nnoniso>0) {
         hFR2_endcap->SetBinContent(i, niso/(niso+nnoniso));
         hFR2_endcap->SetBinError(i, sqrt(pow(dniso * nnoniso / pow(niso+nnoniso,2),2) + pow(dnnoniso * niso / pow(niso+nnoniso,2),2)));
         hFR_endcap->SetBinContent(i, nnoniso>0 ? niso/nnoniso : 1);
         hFR_endcap->SetBinError(i, nnoniso>0&&niso>0 ? (niso / nnoniso) * sqrt(pow(dniso/niso,2)+pow(dnnoniso/nnoniso,2)) : 1);
      }

      // from QCD MC instead of data
      niso = hQCDiso_endcap->GetBinContent(i);
      nnoniso = hQCDnoniso_endcap->GetBinContent(i);
      dniso = hQCDiso_endcap->GetBinError(i);
      dnnoniso = hQCDnoniso_endcap->GetBinError(i);

      if (niso>0 && nnoniso>0) {
         hFR2_QCD_endcap->SetBinContent(i, niso/(niso+nnoniso));
         hFR2_QCD_endcap->SetBinError(i, sqrt(pow(dniso * nnoniso / pow(niso+nnoniso,2),2) + pow(dnnoniso * niso / pow(niso+nnoniso,2),2)));
         hFR_QCD_endcap->SetBinContent(i, nnoniso>0 ? niso/nnoniso : 1);
         hFR_QCD_endcap->SetBinError(i, nnoniso>0&&niso>0 ? (niso / nnoniso) * sqrt(pow(dniso/niso,2)+pow(dnnoniso/nnoniso,2)) : 1);
      }
   }

   ///////////////////////
   // plot the obtained FR
   ///////////////////////

   // write histos to output file
   TFile *fout = TFile::Open("FRhistos_" + tag + ".root","RECREATE");
   hdataiso->Write();
   hdataiso_barrel->Write();
   hdataiso_endcap->Write();
   hdatanoniso->Write();
   hdatanoniso_barrel->Write();
   hdatanoniso_endcap->Write();
   hdatasubiso->Write();
   hdatasubiso_barrel->Write();
   hdatasubiso_endcap->Write();
   hdatasubnoniso->Write();
   hdatasubnoniso_barrel->Write();
   hdatasubnoniso_endcap->Write();
   hMCiso->Write();
   hMCiso_barrel->Write();
   hMCiso_endcap->Write();
   hMCnoniso->Write();
   hMCnoniso_barrel->Write();
   hMCnoniso_endcap->Write();
   hFR->Write();
   hFR_barrel->Write();
   hFR_endcap->Write();
   hFR2->Write();
   hFR2_barrel->Write();
   hFR2_endcap->Write();
   hFR_QCD->Write();
   hFR_QCD_barrel->Write();
   hFR_QCD_endcap->Write();
   hFR2_QCD->Write();
   hFR2_QCD_barrel->Write();
   hFR2_QCD_endcap->Write();
   fout->Close();
}

void addToHist(TH1D *h, int i, double integral, double interror) {
   double binc = h->GetBinContent(i);
   double bine = h->GetBinError(i);
   h->SetBinContent(i,binc+integral);
   h->SetBinError(i,sqrt(pow(bine,2)+pow(interror,2)));
}
