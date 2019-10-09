#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <THStack.h>
#include <TMath.h>
#include <TText.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TColor.h>
#include <TLatex.h>
#include <TEfficiency.h>
#include <TFrame.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include "../../interface/tdrstyle.C"
#include "../../interface/CMS_lumi.C"
#include "../../interface/defs.h"
using namespace std;
using namespace DYana;

const double lumi_sf = 1.;//0.92;

// const double sf_fit_1[3] = {5.1352e-01, 2.0523e+00, 9.1145e-01}; // MC, QCD, Wjets; from fit_wjets.cc(1)
// const double sf_fit_2[3] = {5.1506e-01, 1.8630e+00, 5.1743e-01}; // MC, QCD, Wjets; from fit_wjets.cc(2)
// const double sf_fit_1[3] = {1.1698e+00,1.0671e+00,1.8833e+00}; // MC, QCD, Wjets; from fit_wjets.cc(1,"")
// const double sf_fit_2[3] = {1.1820e+00,9.9705e-01,2.7503e-01}; // MC, QCD, Wjets; from fit_wjets.cc(2,"")

// // values from 2019 09/18
// const double sf_fit_1[3] = {1.1464e+00,9.6762e-01,8.4521e-01}; // MC, QCD, Wjets; from fit_wjets.cc(1,"massMET")
// const double sf_fit_2[3] = {1.1503e+00,9.7400e-01,1.0749e+00}; // MC, QCD, Wjets; from fit_wjets.cc(2,"massMET")

// values from 2019 10/09
const double sf_fit_1[3] = {1.1619e+00,9.7863e-01,1.3484e+00}; // MC, QCD, Wjets; from fit_wjets.cc(1,"massMET")
const double sf_fit_2[3] = {1.1484e+00,9.7525e-01,1.0788e+00}; // MC, QCD, Wjets; from fit_wjets.cc(2,"massMET")


void estimateWjets();

void estimateWjets(var thevar) {
    setTDRStyle();
    // tdrGrid(true);
    lumiTextSize = 0.5;
    cmsTextSize = 0.75;
    gStyle->SetTitleOffset(0.9,"Y");

    int W = 1200;
    int H = 1200;

    int H_ref = 1200;
    int W_ref = 1200;

    // references for T, B, L, R
    float T = 0.08*H_ref;
    float B = 0.12*H_ref;
    float L = 0.12*W_ref;
    float R = 0.04*W_ref;

    // UPDATED IN 2017
    lumiTextSize = 0.5;
    writeExtraText = true;
    extraText = "Preliminary";
    drawLogo = false;

    TH1D* massFrame = new TH1D("massFrame","",nbinsvar(thevar),binsvar(thevar));
    massFrame->SetMinimum(0.001);
    massFrame->SetMaximum(1000000);
    massFrame->SetStats(kFALSE);
    massFrame->GetXaxis()->SetTitle(xaxistitle(thevar));
    //massFrame->GetXaxis()->CenterTitle(kTRUE);
    //massFrame->GetYaxis()->CenterTitle(kTRUE);
    massFrame->GetYaxis()->SetTitleOffset(0.9);
    massFrame->GetYaxis()->SetTitle("Number of events");
    massFrame->GetXaxis()->SetTitleSize(0);
    massFrame->GetYaxis()->SetTitleSize(0.05);
    massFrame->GetXaxis()->SetLabelSize(0);
    //massFrame->GetYaxis()->SetLabelSize(0.025); 
    massFrame->GetXaxis()->SetMoreLogLabels();

    TFile* f[NSamples+2];
//    for (int i=0; i<ALL; i++) f[i] = new TFile(PathFRHistos2(static_cast<SampleTag>(i)));
//    f[QCD] = new TFile(PathFRHistos2(QCD));
    for (int i=0; i<ALL; i++) f[i] = new TFile(Form("histograms/fake%s.root",(TString*)(Name(static_cast<SampleTag>(i)))));
    f[QCD] = new TFile("histograms/fakeQCD.root");


    TH1D* wjets_template[NSamples+2];
    TH1D* wjetsSS_template[NSamples+2];
    TH1D* wjets_ratio[NSamples+2];
    TH1D* wjetsSS_ratio[NSamples+2];
    TH1D* dijet_template[NSamples+2];
    TH1D* dijetSS_template[NSamples+2];
    TH1D* dijet_ratio[NSamples+2];
    TH1D* dijetSS_ratio[NSamples+2];
    TH1D* DYsel[NSamples+2];
    TH1D* DYselSS[NSamples+2];

    double norm[NSamples+2];

    TString histtag;
    if (thevar == mass) histtag = "hist";
    else if (thevar == mass3bins) histtag = "hist3bins";
    else if (thevar == rap60120) histtag = "rap";
    else if (thevar == pt) histtag = "Zpt";
    else if (thevar == rap1560) histtag = "lowMrap";
    else if (thevar == phistar) histtag = "Zphistar";
    else if (thevar == pt1560) histtag = "Zpt1560";
    else histtag = "Zphistar1560";

    TString thevarname(varname(thevar));
    int varNbins = nbinsvar(thevar);
    double * varbins = binsvar(thevar);

    for (int i=0; i<ALL; i++) {
       SampleTag tag = static_cast<SampleTag>(i);
       if (tag==QCD) continue;
       double lumi = lumi_all;
       if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
          lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
       }
       norm[i] = IsData(tag) ? 1. : (Xsec(tag)*lumi*lumi_sf)/Nevts(tag);
       cout<< "norm[" << Name(static_cast<SampleTag>(i)) << "] = " << norm[i]<<endl;

       wjets_template[i] = (TH1D*)f[i]->Get("WJets/" + histtag + "WJets1");
       wjets_template[i]->Scale(norm[i]);
       wjets_template[i]->SetStats(kFALSE);

       wjetsSS_template[i] = (TH1D*)f[i]->Get("WJets/" + histtag + "SameWJets1");
       wjetsSS_template[i]->Scale(norm[i]);
       wjetsSS_template[i]->SetStats(kFALSE);


       wjets_ratio[i] = (TH1D*)f[i]->Get("WJets/" + histtag + "WJets2");
       wjets_ratio[i]->Scale(norm[i]);
       wjets_ratio[i]->SetStats(kFALSE);

       wjetsSS_ratio[i] = (TH1D*)f[i]->Get("WJets/" + histtag + "SameWJets2");
       wjetsSS_ratio[i]->Scale(norm[i]);
       wjetsSS_ratio[i]->SetStats(kFALSE);

       dijet_template[i] = (TH1D*)f[i]->Get("Dijet/" + histtag + "Dijet1");
       dijet_template[i]->Scale(norm[i]);
       dijet_template[i]->SetStats(kFALSE);

       dijetSS_template[i] = (TH1D*)f[i]->Get("Dijet/" + histtag + "SameDijet1");
       dijetSS_template[i]->Scale(norm[i]);
       dijetSS_template[i]->SetStats(kFALSE);


       dijet_ratio[i] = (TH1D*)f[i]->Get("Dijet/" + histtag + "Dijet2");
       dijet_ratio[i]->Scale(norm[i]);
       dijet_ratio[i]->SetStats(kFALSE);

       dijetSS_ratio[i] = (TH1D*)f[i]->Get("Dijet/" + histtag + "SameDijet2");
       dijetSS_ratio[i]->Scale(norm[i]);
       dijetSS_ratio[i]->SetStats(kFALSE);

       DYsel[i] = (TH1D*)f[i]->Get("DYsel/" + histtag + "DYsel");
       DYsel[i]->Scale(norm[i]);
       DYsel[i]->SetStats(kFALSE);

       DYselSS[i] = (TH1D*)f[i]->Get("DYsel/" + histtag + "SameDYsel");
       DYselSS[i]->Scale(norm[i]);
       DYselSS[i]->SetStats(kFALSE);

       // put histos in an array for easy style setting
       TH1D* h[10] = {wjets_template[i], wjetsSS_template[i], wjets_ratio[i], wjetsSS_ratio[i],
          dijet_template[i], dijetSS_template[i], dijet_ratio[i], dijetSS_ratio[i],
          DYsel[i], DYselSS[i]};
       for (int j=0; j<10; j++) {
          h[j]->GetXaxis()->SetTitle(xaxistitle(thevar));
          h[j]->GetYaxis()->SetTitleOffset(0.9);
          h[j]->GetYaxis()->SetTitle("Number of events");
          //h[j]->GetXaxis()->SetTitleSize(0.032);
          //h[j]->GetYaxis()->SetTitleSize(0.032);
          h[j]->GetXaxis()->SetLabelSize(0.025);
          h[j]->GetYaxis()->SetLabelSize(0.025);
          h[j]->GetXaxis()->SetMoreLogLabels();

          // remove negative bins
          for(int k=1; k<varNbins+3; k++) {
             if(h[j]->GetBinContent(i) < 0) {
                h[j]->SetBinContent(k,0.0);
                h[j]->SetBinError(k,0.0);
             }
          }
       } // j loop on histos

       // add histos together
       bool toadd = false;
       SampleTag tagtoadd;
       if (IsDY(tag)) {
          // style 
          for (int j=0; j<10; j++) {
             h[j]->SetFillColor(2);
             // h[j]->SetmarkerColor(2);
             // h[j]->SetmarkerStyle(22);
          }

          if (tag != DYFirst) {
             toadd = true;
             tagtoadd = DYFirst;
          }
       }
       if (IsData(tag)) {
          // style 
          for (int j=0; j<8; j++) {
             h[j]->SetLineColor(1);
             h[j]->SetMarkerColor(1);
             h[j]->SetMarkerStyle(22);
          }

          if (tag != Data1) {
             toadd = true;
             tagtoadd = Data1;
          }
       }
       if (IsDiboson(tag)) {
          // style 
          for (int j=0; j<10; j++) {
             h[j]->SetFillColor(3);
          }

          if (tag != WW) {
             toadd = true;
             tagtoadd = WW;
          }
       }
       if (IsWjets(tag)) {
          // style 
          for (int j=0; j<10; j++) {
             h[j]->SetFillColor(4);
          }

          if (tag != WFirst) {
             toadd = true;
             tagtoadd = WFirst;
          }
       }
       if (tag==TT) {
          for (int j=0; j<10; j++) {
             h[j]->SetFillColor(6);
          }
       }

       if (toadd) {
          wjets_template[tagtoadd]->Add(wjets_template[i]);
          wjetsSS_template[tagtoadd]->Add(wjetsSS_template[i]);
          wjets_ratio[tagtoadd]->Add(wjets_ratio[i]);
          wjetsSS_ratio[tagtoadd]->Add(wjetsSS_ratio[i]);
          dijet_template[tagtoadd]->Add(dijet_template[i]);
          dijetSS_template[tagtoadd]->Add(dijetSS_template[i]);
          dijet_ratio[tagtoadd]->Add(dijet_ratio[i]);
          dijetSS_ratio[tagtoadd]->Add(dijetSS_ratio[i]);
          DYsel[tagtoadd]->Add(DYsel[i]);
          DYselSS[tagtoadd]->Add(DYselSS[i]);
       }
    } // sample loop

    TCanvas* canv = new TCanvas("canv","",1200,1200);
    canv->SetFillColor(0);
    canv->SetLeftMargin( L/W );
    canv->SetRightMargin( R/W );
    canv->SetTopMargin( T/H );
    canv->SetBottomMargin( B/H );

    /////////////////////////////////////
    // method 1: subtract MC from data //
    /////////////////////////////////////
    wjets_template[Data1]->Add(wjets_template[DYFirst],-sf_fit_1[0]);
    wjets_template[Data1]->Add(wjets_template[TT],-sf_fit_1[0]);
    wjets_template[Data1]->Add(wjets_template[WW],-sf_fit_1[0]);
    wjetsSS_template[Data1]->Add(wjetsSS_template[DYFirst],-sf_fit_1[0]);
    wjetsSS_template[Data1]->Add(wjetsSS_template[TT],-sf_fit_1[0]);
    wjetsSS_template[Data1]->Add(wjetsSS_template[WW],-sf_fit_1[0]);

    wjets_ratio[Data1]->Add(wjets_ratio[DYFirst],-sf_fit_2[0]);
    wjets_ratio[Data1]->Add(wjets_ratio[TT],-sf_fit_2[0]);
    wjets_ratio[Data1]->Add(wjets_ratio[WW],-sf_fit_2[0]);
    wjetsSS_ratio[Data1]->Add(wjetsSS_ratio[DYFirst],-sf_fit_2[0]);
    wjetsSS_ratio[Data1]->Add(wjetsSS_ratio[TT],-sf_fit_2[0]);
    wjetsSS_ratio[Data1]->Add(wjetsSS_ratio[WW],-sf_fit_2[0]);

    // and subtract QCD
    TH1D *h_QCD_template = dijet_template[DataFirst];
    TH1D *h_DYJets_Dijet_template = dijet_template[DYFirst];
    TH1D *h_ttbar_Dijet_template = dijet_template[TT];
    h_QCD_template->Add(h_DYJets_Dijet_template,-1.0);
    h_QCD_template->Add(h_ttbar_Dijet_template,-1.0);
    wjets_template[Data1]->Add(h_QCD_template,-2.*sf_fit_1[1]);
    TH1D *h_QCDSS_template = dijetSS_template[DataFirst];
    TH1D *h_DYJets_DijetSS_template = dijetSS_template[DYFirst];
    TH1D *h_ttbar_DijetSS_template = dijetSS_template[TT];
    h_QCDSS_template->Add(h_DYJets_DijetSS_template,-1.0);
    h_QCDSS_template->Add(h_ttbar_DijetSS_template,-1.0);
    wjetsSS_template[Data1]->Add(h_QCDSS_template,-2.*sf_fit_1[1]);

    TH1D *h_QCD_ratio = dijet_ratio[DataFirst];
    TH1D *h_DYJets_Dijet_ratio = dijet_ratio[DYFirst];
    TH1D *h_ttbar_Dijet_ratio = dijet_ratio[TT];
    h_QCD_ratio->Add(h_DYJets_Dijet_ratio,-1.0);
    h_QCD_ratio->Add(h_ttbar_Dijet_ratio,-1.0);
    wjets_ratio[Data1]->Add(h_QCD_ratio,-2.*sf_fit_2[1]);
    TH1D *h_QCDSS_ratio = dijetSS_ratio[DataFirst];
    TH1D *h_DYJets_DijetSS_ratio = dijetSS_ratio[DYFirst];
    TH1D *h_ttbar_DijetSS_ratio = dijetSS_ratio[TT];
    h_QCDSS_ratio->Add(h_DYJets_DijetSS_ratio,-1.0);
    h_QCDSS_ratio->Add(h_ttbar_DijetSS_ratio,-1.0);
    wjetsSS_ratio[Data1]->Add(h_QCDSS_ratio,-2.*sf_fit_2[1]);

    // remove negative bins
    for(int k=1; k<varNbins+3; k++) {
       if(wjets_template[Data1]->GetBinContent(k) < 0) {
          wjets_template[Data1]->SetBinContent(k,0.0);
          wjets_template[Data1]->SetBinError(k,0.0);
       }
       if(wjets_ratio[Data1]->GetBinContent(k) < 0) {
          wjets_ratio[Data1]->SetBinContent(k,0.0);
          wjets_ratio[Data1]->SetBinError(k,0.0);
       }
       if(wjetsSS_template[Data1]->GetBinContent(k) < 0) {
          wjetsSS_template[Data1]->SetBinContent(k,0.0);
          wjetsSS_template[Data1]->SetBinError(k,0.0);
       }
       if(wjetsSS_ratio[Data1]->GetBinContent(k) < 0) {
          wjetsSS_ratio[Data1]->SetBinContent(k,0.0);
          wjetsSS_ratio[Data1]->SetBinError(k,0.0);
       }
    }

    /////////////////////////
    // method 2: scaled MC //
    /////////////////////////

    DYsel[WFirst]->Scale(sf_fit_2[1]);
    DYselSS[WFirst]->Scale(sf_fit_2[1]);

    for(int i=1; i<varNbins+1; i++) {
        if(wjets_template[Data1]->GetBinContent(i) < 0) {
          wjets_template[Data1]->SetBinContent(i,0.0);
          wjets_template[Data1]->SetBinError(i,0.0);
        }
        if(wjets_ratio[Data1]->GetBinContent(i) < 0) {
          wjets_ratio[Data1]->SetBinContent(i,0.0);
          wjets_ratio[Data1]->SetBinError(i,0.0);
        }
    }

    // wjets estimations can be very jumpy, especially vs pt or phistar. Let's remove fluctuations with smoothing.
    TH1D *wjets_template_nosmooth = (TH1D*) wjets_template[Data1]->Clone("wjets_template_nosmooth");
    TH1D *wjets_ratio_nosmooth = (TH1D*) wjets_ratio[Data1]->Clone("wjets_ratio_nosmooth");
    TH1D *wjetsSS_template_nosmooth = (TH1D*) wjetsSS_template[Data1]->Clone("wjetsSS_template_nosmooth");
    TH1D *wjetsSS_ratio_nosmooth = (TH1D*) wjetsSS_ratio[Data1]->Clone("wjetsSS_ratio_nosmooth");
    if (thevar==var::pt || thevar==var::pt1560 || thevar==var::phistar || thevar==var::phistar1560) {
       wjets_template[Data1]->Smooth();
       wjets_ratio[Data1]->Smooth();
       wjetsSS_template[Data1]->Smooth();
       wjetsSS_ratio[Data1]->Smooth();

       wjets_template_nosmooth->SetLineStyle(2);
       wjets_ratio_nosmooth->SetLineStyle(2);
       wjetsSS_template_nosmooth->SetLineStyle(2);
       wjetsSS_ratio_nosmooth->SetLineStyle(2);
    }
    
    TLegend* legg = new TLegend(.6,.65,.95,.89);
    legg->AddEntry(wjets_template[Data1],"Nominal (2#mu OS high #chi^{2})", "P");
    if (thevar==var::pt || thevar==var::pt1560 || thevar==var::phistar || thevar==var::phistar1560) legg->AddEntry(wjets_template_nosmooth,"Nominal, no smooth", "L");
    legg->AddEntry(wjets_ratio[Data1],"Alt. (2#mu SS)", "L");
    legg->AddEntry(DYsel[WFirst],"Scaled MC", "F");

    wjets_template[Data1]->Draw("EP");
    CMS_lumi(canv,111,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    if (thevar != rap1560 && thevar != rap60120) canv->SetLogx();
    wjets_ratio[Data1]->Draw("HIST SAME");
    DYsel[WFirst]->Draw("hist same");
    wjets_template[Data1]->Draw("EP same");
    wjets_ratio[Data1]->Draw("HIST SAME");
    if (thevar==var::pt || thevar==var::pt1560 || thevar==var::phistar || thevar==var::phistar1560) wjets_template_nosmooth->Draw("HIST SAME");
    legg->Draw("SAME");
    canv->Print("print/wjets_" + thevarname + ".pdf");
    canv->Clear();

    wjetsSS_template[Data1]->Draw("EP");
    CMS_lumi(canv,111,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    if (thevar != rap1560 && thevar != rap60120) canv->SetLogx();
    wjetsSS_ratio[Data1]->Draw("HIST SAME");
    DYselSS[WFirst]->Draw("hist same");
    wjetsSS_template[Data1]->Draw("EP same");
    wjetsSS_ratio[Data1]->Draw("HIST SAME");
    if (thevar==var::pt || thevar==var::pt1560 || thevar==var::phistar || thevar==var::phistar1560) wjetsSS_template_nosmooth->Draw("HIST SAME");
    legg->Draw("SAME");
    canv->Print("print/wjetsSS_" + thevarname + ".pdf");
    canv->Clear();

    double error = 0;
    wjets_template[Data1]->IntegralAndError(1,45,error);
    cout<<"WJets(template) = "<<wjets_template[Data1]->Integral(1,45)<<"+-"<<error<<endl;
    error = 0;
    wjetsSS_template[Data1]->IntegralAndError(1,45,error);
    cout<<"WJets(template) SS = "<<wjetsSS_template[Data1]->Integral(1,45)<<"+-"<<error<<endl;
    error = 0;
    wjets_ratio[Data1]->IntegralAndError(1,45,error);
    cout<<"WJets(ratio) = "<<wjets_ratio[Data1]->Integral(1,45)<<"+-"<<error<<endl;
    error = 0;
    wjetsSS_ratio[Data1]->IntegralAndError(1,45,error);
    cout<<"WJets(ratio) SS = "<<wjetsSS_ratio[Data1]->Integral(1,45)<<"+-"<<error<<endl;

    TH1D* wjets = (TH1D*)wjets_template[Data1]->Clone();
    wjets->Sumw2();
    wjets->SetName("wjets");

    TH1D* wjets_total      = new TH1D("wjets_total","",varNbins,varbins);
    TH1D* wjets_systematic = new TH1D("wjets_systematic","",varNbins,varbins);
    TH1D* wjets_stat       = new TH1D("wjets_stat","",varNbins,varbins);

    for(int i=1; i<varNbins+1; i++) {

        double systematic = fabs(wjets->GetBinContent(i) - wjets_ratio[Data1]->GetBinContent(i));
        double stat = wjets->GetBinError(i);
        double total = sqrt( systematic*systematic + stat*stat );
        if(wjets->GetBinContent(i)==0) {
          systematic = 0;
          stat = 0;
          total = 0;
        }

        wjets_systematic->SetBinContent(i,systematic);
        wjets_stat->SetBinContent(i,stat);
        wjets_total->SetBinContent(i,total);

        wjets->SetBinError(i,total);
    }

    TFile* gg = new TFile("result/wjets_" + thevarname + ".root","RECREATE");
    wjets->Write();
    wjets_systematic->Write();
    wjets_stat->Write();
    wjets_ratio[DataFirst]->Write("wjets_ratio");
    wjets_template[DataFirst]->Write("wjets_template");
    wjetsSS_ratio[DataFirst]->Write("wjetsSS_ratio");
    wjetsSS_template[DataFirst]->Write("wjetsSS_template");
    DYsel[WFirst]->Write("wjetsMC");
    DYselSS[WFirst]->Write("wjetsSSMC");
    gg->Close();

    //wjets_systematic->Divide(wjets);
    wjets_systematic->Divide(wjets);
    wjets_stat->Divide(wjets);
    wjets_total->Divide(wjets);

    wjets_total->SetMarkerStyle(20);
    wjets_total->SetMarkerSize(3);
    wjets_total->SetMarkerColor(1);

    wjets_systematic->SetMarkerStyle(22);
    wjets_systematic->SetMarkerSize(3);
    wjets_systematic->SetMarkerColor(2);

    wjets_stat->SetMarkerStyle(21);
    wjets_stat->SetMarkerSize(3);
    wjets_stat->SetMarkerColor(4);

    TLegend* leg = new TLegend(.8,.15,.95,.3);
    leg->AddEntry(wjets_total,"Total","P");
    leg->AddEntry(wjets_systematic,"Sys.","P");
    leg->AddEntry(wjets_stat,"Stat.","P");

    massFrame->GetYaxis()->SetTitle("Unceratinty");
    massFrame->SetMinimum(0);
    massFrame->SetMaximum(1);
    massFrame->GetXaxis()->SetLabelSize(0.025);
    massFrame->GetXaxis()->SetTitleSize(0.05);

    massFrame->Draw();
    wjets_total->Draw("HISTPSAME");
    wjets_systematic->Draw("HISTPSAME");
    wjets_stat->Draw("HISTPSAME");
    massFrame->Draw("AXISSAME");
    leg->Draw("SAME");
    CMS_lumi(canv,111,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    canv->SetLogx();
    canv->Print("print/wjets_" + thevarname + "_uncertainty.pdf");
}

void estimateWjets() {
   estimateWjets(mass);
   estimateWjets(mass3bins);
   estimateWjets(rap60120);
   estimateWjets(pt);
   estimateWjets(phistar);
   estimateWjets(rap1560);
   estimateWjets(pt1560);
   estimateWjets(phistar1560);
}
