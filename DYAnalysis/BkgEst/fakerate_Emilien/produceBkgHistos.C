#include "../interface/defs.h"

using namespace RooFit;
using namespace DYana;

void produceBkgHistos() {
	Double_t Lumi = lumi_all;

   //Get ROOT Files
   TFile* f[NSamples+2];
   for (int i=0; i<ALL; i++) f[i] = new TFile(PathFRHistosEmi(static_cast<SampleTag>(i)));
   f[QCD] = new TFile(PathFRHistos2(QCD));

	//Get Histograms
   TH1D *hm_OS_2iso[NSamples+2];
   TH1D *hm_OS_1iso[NSamples+2];
   TH1D *hm_OS_0iso[NSamples+2];
   TH1D *hm_SS_2iso[NSamples+2];
   TH1D *hm_SS_1iso[NSamples+2];
   TH1D *hm_SS_0iso[NSamples+2];

   double norm[NSamples+2];
   for (int i=0; i<ALL; i++) {
      SampleTag tag = static_cast<SampleTag>(i);
      norm[i] = (Xsec(tag)*lumi_all)/Nevts(tag);
      cout<< "norm[" << i << "] = " << norm[i]<<endl;

      hm_OS_2iso[i] = (TH1D*)f[i]->Get("hmass_OS_2iso");
      hm_OS_2iso[i]->Scale(norm[i]);
      hm_OS_1iso[i] = (TH1D*)f[i]->Get("hmass_OS_1iso");
      hm_OS_1iso[i]->Scale(norm[i]);
      hm_OS_0iso[i] = (TH1D*)f[i]->Get("hmass_OS_0iso");
      hm_OS_0iso[i]->Scale(norm[i]);
      hm_SS_2iso[i] = (TH1D*)f[i]->Get("hmass_SS_2iso");
      hm_SS_2iso[i]->Scale(norm[i]);
      hm_SS_1iso[i] = (TH1D*)f[i]->Get("hmass_SS_1iso");
      hm_SS_1iso[i]->Scale(norm[i]);
      hm_SS_0iso[i] = (TH1D*)f[i]->Get("hmass_SS_0iso");
      hm_SS_0iso[i]->Scale(norm[i]);

       // add histos together
       bool toadd = false;
       SampleTag tagtoadd;
       if (IsDY(tag)) {
          if (tag != DYFirst) {
             toadd = true;
             tagtoadd = DYFirst;
          }
       }
       if (IsData(tag)) {
          if (tag != DataFirst) {
             toadd = true;
             tagtoadd = DataFirst;
          }
       }
       if (IsDiboson(tag)) {
          if (tag != VVFirst) {
             toadd = true;
             tagtoadd = VVFirst;
          }
       }
       if (IsWjets(tag)) {
          if (tag != WFirst) {
             toadd = true;
             tagtoadd = WFirst;
          }
       }

       if (toadd) {
          hm_OS_2iso[tagtoadd]->Add(hm_OS_2iso[i]);
          hm_OS_1iso[tagtoadd]->Add(hm_OS_1iso[i]);
          hm_OS_0iso[tagtoadd]->Add(hm_OS_0iso[i]);
          hm_SS_2iso[tagtoadd]->Add(hm_SS_2iso[i]);
          hm_SS_1iso[tagtoadd]->Add(hm_SS_1iso[i]);
          hm_SS_0iso[tagtoadd]->Add(hm_SS_0iso[i]);
       }
   } // sample loop

   // construct the bkg-sub histos
   TH1D *hm_SS_2iso_BS = (TH1D*) hm_SS_2iso[DataFirst]->Clone("hm_SS_2iso_BS");
   hm_SS_2iso_BS->Add(hm_SS_2iso[DYFirst],-1);
   hm_SS_2iso_BS->Add(hm_SS_2iso[VVFirst],-1);
   hm_SS_2iso_BS->Add(hm_SS_2iso[TT],-1);
   TH1D *hm_SS_01iso_BS = (TH1D*) hm_SS_0iso[DataFirst]->Clone("hm_SS_01iso_BS");
   // hm_SS_01iso_BS->Add(hm_SS_1iso[DataFirst]);
   THStack *hs_SS_01iso = new THStack("hs_SS_01iso","");
   hm_SS_0iso[DYFirst]->SetFillColor(2);
   hs_SS_01iso->Add(hm_SS_0iso[DYFirst]);
   hm_SS_01iso_BS->Add(hm_SS_0iso[DYFirst],-1);
   hm_SS_0iso[VVFirst]->SetFillColor(3);
   hs_SS_01iso->Add(hm_SS_0iso[VVFirst]);
   hm_SS_01iso_BS->Add(hm_SS_0iso[VVFirst],-1);
   hm_SS_0iso[TT]->SetFillColor(6);
   hs_SS_01iso->Add(hm_SS_0iso[TT]);
   hm_SS_01iso_BS->Add(hm_SS_0iso[TT],-1);
   // hm_SS_1iso[DYFirst]->SetFillColor(2);
   // hs_SS_01iso->Add(hm_SS_1iso[DYFirst]);
   // hm_SS_01iso_BS->Add(hm_SS_1iso[DYFirst],-1);
   // hm_SS_1iso[VVFirst]->SetFillColor(3);
   // hs_SS_01iso->Add(hm_SS_1iso[VVFirst]);
   // hm_SS_01iso_BS->Add(hm_SS_1iso[VVFirst],-1);
   // hm_SS_1iso[TT]->SetFillColor(6);
   // hs_SS_01iso->Add(hm_SS_1iso[TT]);
   // hm_SS_01iso_BS->Add(hm_SS_1iso[TT],-1);

   TH1D *hm_OS_01iso_BS = (TH1D*) hm_OS_0iso[DataFirst]->Clone("hm_OS_01iso_BS");
   // hm_OS_01iso_BS->Add(hm_OS_1iso[DataFirst]);
   THStack *hs_OS_01iso = new THStack("hs_OS_01iso","");
   hm_OS_0iso[DYFirst]->SetFillColor(2);
   hs_OS_01iso->Add(hm_OS_0iso[DYFirst]);
   hm_OS_01iso_BS->Add(hm_OS_0iso[DYFirst],-1);
   hm_OS_0iso[VVFirst]->SetFillColor(3);
   hs_OS_01iso->Add(hm_OS_0iso[VVFirst]);
   hm_OS_01iso_BS->Add(hm_OS_0iso[VVFirst],-1);
   hm_OS_0iso[TT]->SetFillColor(6);
   hs_OS_01iso->Add(hm_OS_0iso[TT]);
   hm_OS_01iso_BS->Add(hm_OS_0iso[TT],-1);
   // hm_OS_1iso[DYFirst]->SetFillColor(2);
   // hs_OS_01iso->Add(hm_OS_1iso[DYFirst]);
   // hm_OS_01iso_BS->Add(hm_OS_1iso[DYFirst],-1);
   // hm_OS_1iso[VVFirst]->SetFillColor(3);
   // hs_OS_01iso->Add(hm_OS_1iso[VVFirst]);
   // hm_OS_01iso_BS->Add(hm_OS_1iso[VVFirst],-1);
   // hm_OS_1iso[TT]->SetFillColor(6);
   // hs_OS_01iso->Add(hm_OS_1iso[TT]);
   // hm_OS_01iso_BS->Add(hm_OS_1iso[TT],-1);

   TH1D *hfake = (TH1D*) hm_SS_2iso_BS->Clone("hfake");
   hfake->Multiply(hm_OS_01iso_BS);
   hfake->Divide(hm_SS_01iso_BS);

   TFile *fout = TFile::Open("fakerate.root","RECREATE");
   hfake->Write();
   hm_SS_2iso_BS->Write();
   hm_SS_01iso_BS->Write();
   hs_SS_01iso->Write();
   hm_OS_01iso_BS->Write();
   hs_OS_01iso->Write();
   fout->Write();
   fout->Close();
}
