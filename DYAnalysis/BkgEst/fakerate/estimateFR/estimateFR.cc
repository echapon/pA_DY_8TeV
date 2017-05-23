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

void setDataHist(TH1D* hist);
void setMCHist(TH1D* hist, const int& color);
void setFRHist(TH1D* hist);
TH1D* FRByTemplate(TH1D** numerator, TH1D** denominator);
TH1D* FRBytRatio(TH1D** numerator, TH1D** denominator);

void estimateFR() {


    TFile* file[NSamples+2];
    for (int i=0; i<ALL; i++) file[i] = new TFile(PathFRHistos(static_cast<SampleTag>(i)));
    file[QCD] = new TFile(PathFRHistos(QCD));

    TH1D* denominator_pt_fit_barrel[NSamples+2];
    TH1D* denominator_pt_fit_endcap[NSamples+2];
    TH1D* denominator_pt_xsec_barrel[NSamples+2];
    TH1D* denominator_pt_xsec_endcap[NSamples+2];

    TH1D* numerator_pt_barrel[NSamples+2];
    TH1D* numerator_pt_endcap[NSamples+2];

    TH1D* denominator_barrel[NSamples+2];
    TH1D* denominator_endcap[NSamples+2];

    double norm_part1[NSamples+2];
    double norm_part2[NSamples+2];
    double norm_fit_barrel[NSamples+2];
    double norm_fit_endcap[NSamples+2];

    for(int i=0;i<=QCD;i++) {
       if (i==ALL) continue;
       SampleTag tag = static_cast<SampleTag>(i);
       denominator_pt_fit_barrel[i] = (TH1D*)file[i]->Get("denominator_pt_barrel")->Clone("denominator_pt_fit_barrel"+TString(Name(tag)));
       denominator_pt_xsec_barrel[i] = (TH1D*)file[i]->Get("denominator_pt_barrel")->Clone("denominator_pt_xsec_barrel"+TString(Name(tag)));
       denominator_barrel[i] = (TH1D*)file[i]->Get("denominator_barrel")->Clone("denominator_barrel"+TString(Name(tag)));
       numerator_pt_barrel[i] = (TH1D*)file[i]->Get("numerator_pt_barrel")->Clone("numerator_pt_barrel"+TString(Name(tag)));
       denominator_pt_fit_endcap[i] = (TH1D*)file[i]->Get("denominator_pt_endcap")->Clone("denominator_pt_fit_endcap"+TString(Name(tag)));
       denominator_pt_xsec_endcap[i] = (TH1D*)file[i]->Get("denominator_pt_endcap")->Clone("denominator_pt_xsec_endcap"+TString(Name(tag)));
       denominator_endcap[i] = (TH1D*)file[i]->Get("denominator_endcap")->Clone("denominator_endcap"+TString(Name(tag)));
       numerator_pt_endcap[i] = (TH1D*)file[i]->Get("numerator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(tag)));

       if(IsData(tag)) {
          setDataHist( denominator_pt_fit_barrel[i] );
          setDataHist( denominator_pt_xsec_barrel[i] );
          setDataHist( denominator_barrel[i] );
          setDataHist( numerator_pt_barrel[i] );
          setDataHist( denominator_pt_fit_endcap[i] );
          setDataHist( denominator_pt_xsec_endcap[i] );
          setDataHist( denominator_endcap[i] );
          setDataHist( numerator_pt_endcap[i] );
       }
       else {
          setMCHist( denominator_pt_fit_barrel[i], i );
          setMCHist( denominator_pt_xsec_barrel[i], i );
          setMCHist( denominator_barrel[i], i );
          setMCHist( numerator_pt_barrel[i], i );
          setMCHist( denominator_pt_fit_endcap[i], i );
          setMCHist( denominator_pt_xsec_endcap[i], i );
          setMCHist( denominator_endcap[i], i );
          setMCHist( numerator_pt_endcap[i], i );
       }

       norm_part1[i] = (Xsec(tag)*lumi_part1)/Nevts(tag);
       norm_part2[i] = (Xsec(tag)*lumi_part2)/Nevts(tag);
    }

    // EC: what is this??
    // norm_fit_barrel[0] = 6.0827e+05/denominator_barrel[0]->Integral();
    // norm_fit_barrel[1] = 1.3244e+05/denominator_barrel[1]->Integral();
    // norm_fit_barrel[2] = 1.8218e+06/denominator_barrel[2]->Integral();
    // norm_fit_barrel[5] = 2.7482e+06/denominator_barrel[5]->Integral();
    // norm_fit_barrel[11] = 4.4531e+03/denominator_barrel[11]->Integral();
    // norm_fit_barrel[12] = 1.4433e+03/denominator_barrel[12]->Integral();
    // norm_fit_barrel[13] = 6.3316e+02/denominator_barrel[13]->Integral();

    // norm_fit_endcap[0] = 4.8779e+05/denominator_endcap[0]->Integral();
    // norm_fit_endcap[1] = 5.8684e+04/denominator_endcap[1]->Integral();
    // norm_fit_endcap[2] = 1.4858e+06/denominator_endcap[2]->Integral();
    // norm_fit_endcap[5] = 1.3886e+06/denominator_endcap[5]->Integral();
    // norm_fit_endcap[11] = 3.4160e+03/denominator_endcap[11]->Integral();
    // norm_fit_endcap[12] = 1.0673e+03/denominator_endcap[12]->Integral();
    // norm_fit_endcap[13] = 4.7170e+02/denominator_endcap[13]->Integral();

    for(int i=0;i<=QCD;i++) {
       if (i==ALL) continue;
       SampleTag tag = static_cast<SampleTag>(i);
       // denominator_barrel[i]->Scale(norm_fit_barrel[i]);
       // denominator_pt_fit_barrel[i]->Scale(norm_fit_barrel[i]);
       denominator_pt_xsec_barrel[i]->Scale(Xsec(tag));
       numerator_pt_barrel[i]->Scale(Xsec(tag));

       // denominator_endcap[i]->Scale(norm_fit_endcap[i]);
       // denominator_pt_fit_endcap[i]->Scale(norm_fit_endcap[i]);
       denominator_pt_xsec_endcap[i]->Scale(Xsec(tag));
       numerator_pt_endcap[i]->Scale(Xsec(tag));
    }

    cout << __LINE__ << endl;
    TH1D* FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
    cout << __LINE__ << endl;
    TH1D* FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    TH1D* FR_xsec_barrel = (TH1D*)FRBytRatio(numerator_pt_barrel, denominator_pt_xsec_barrel);
    cout << __LINE__ << endl;
    TH1D* FR_xsec_endcap = (TH1D*)FRBytRatio(numerator_pt_endcap, denominator_pt_xsec_endcap);
    cout << __LINE__ << endl;

    int W = 1200;
    int H = 1200;

    int H_ref = 1200;
    int W_ref = 1200;

    // references for T, B, L, R
    float T = 0.08*H_ref;
    float B = 0.12*H_ref;
    float L = 0.12*W_ref;
    float R = 0.04*W_ref;

    lumi_13TeV = "2759 pb^{-1}";
    writeExtraText = true;
    extraText = "Preliminary";
    drawLogo = false;
    tdrGrid(true);
    lumiTextSize = 0.5;
    cmsTextSize = 0.75;

    TH1D* ptFrame = new TH1D("ptFrame","",8,47,500);
    ptFrame->SetStats(kFALSE);
    ptFrame->GetXaxis()->SetTitle("p_{T}[GeV]");
    ptFrame->GetYaxis()->SetTitle("Fake Rate");

    ptFrame->SetMinimum(0);
    ptFrame->SetMaximum(1.0); 
    ptFrame->GetXaxis()->SetTitleOffset(1);
    ptFrame->GetYaxis()->SetTitleOffset(1.1);
    ptFrame->GetXaxis()->SetTitleSize(0.05);
    ptFrame->GetYaxis()->SetTitleSize(0.05);  
    ptFrame->GetXaxis()->SetLabelSize(0.035);
    ptFrame->GetYaxis()->SetLabelSize(0.035); 
    ptFrame->GetXaxis()->SetMoreLogLabels(); 

    TCanvas* canv = new TCanvas("canv","",1200,1200);
    canv->SetFillColor(0);
    canv->SetLeftMargin( L/W );
    canv->SetRightMargin( R/W );
    canv->SetTopMargin( T/H );
    canv->SetBottomMargin( B/H );

    FR_template_barrel->SetMarkerSize(3);
    FR_template_endcap->SetMarkerSize(3);
    FR_xsec_barrel->SetMarkerSize(3);
    FR_xsec_endcap->SetMarkerSize(3);

    FR_template_barrel->SetLineColor(1);
    FR_template_barrel->SetLineWidth(2);
    FR_template_barrel->SetMarkerStyle(20);
    FR_template_barrel->SetMarkerColor(1);

    FR_xsec_barrel->SetLineColor(2);
    FR_xsec_barrel->SetLineWidth(2);
    FR_xsec_barrel->SetMarkerStyle(21);
    FR_xsec_barrel->SetMarkerColor(2);

    FR_template_endcap->SetLineColor(1);
    FR_template_endcap->SetLineWidth(2);
    FR_template_endcap->SetMarkerStyle(20);
    FR_template_endcap->SetMarkerColor(1);

    FR_xsec_endcap->SetLineColor(2);
    FR_xsec_endcap->SetLineWidth(2);
    FR_xsec_endcap->SetMarkerStyle(21);
    FR_xsec_endcap->SetMarkerColor(2);

    canv->cd();
    canv->SetLogx();
  

    TLegend* legend2 = new TLegend(.45,.65,.75,.89);
    legend2->AddEntry(FR_template_barrel,"Template fitting");
    legend2->AddEntry(FR_xsec_barrel,"Ratio method");
    legend2->SetBorderSize(0);

    ptFrame->Draw();
    CMS_lumi(canv,4,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    FR_template_barrel->Draw("EPSAME");
    FR_xsec_barrel->Draw("EPSAME");
    legend2->Draw("SAME");
    canv->Print("print/FR_Barrel.pdf");

    canv->Clear();
    ptFrame->Draw();
    CMS_lumi(canv,4,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    FR_template_endcap->Draw("EPSAME");
    FR_xsec_endcap->Draw("EPSAME");
    legend2->Draw("SAME");
    canv->Print("print/FR_Endcap.pdf");

    TFile* g = new TFile("result/fakerate.root","RECREATE");
    FR_template_barrel->Write();
    FR_template_endcap->Write();
    FR_xsec_barrel->Write();
    FR_xsec_endcap->Write();
    g->Close();

    cout<<"Template Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_template_barrel->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Template Endcap"<<endl;
    for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_template_endcap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Ratio Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_xsec_barrel->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Ratio Endcap"<<endl;
        for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_xsec_endcap->GetBinContent(i);
    }
    cout<<endl;

}

void setDataHist(TH1D* hist) {
    hist->SetLineWidth(2);
    hist->SetMarkerStyle(33);
    hist->SetMarkerSize(3);
    hist->SetStats(kFALSE);
    hist->Sumw2();
}

void setMCHist(TH1D* hist, const int& color) {
    hist->SetFillColor(color+2);
    hist->SetStats(kFALSE);
    hist->Sumw2();
}

TH1D* FRByTemplate(TH1D** numerator, TH1D** denominator) {

    cout << __LINE__ << endl;
    TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_template_barrel" : "FR_template_endcap";
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    TH1D* num = (TH1D*)numerator[Data1]->Clone(name);
    cout << __LINE__ << endl;
    num->Add(numerator[Data2]);
    cout << __LINE__ << endl;
    TH1D* den = (TH1D*)numerator[DY1050]->Clone(name+"_");
    cout << __LINE__ << endl;
    den->Add(numerator[DY50100]);
    cout << __LINE__ << endl;
    den->Add(numerator[DY100200]);
    cout << __LINE__ << endl;
    den->Add(numerator[DY200400]);
    cout << __LINE__ << endl;
    den->Add(numerator[DY4001000]);
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    num->Multiply(numerator[QCD]);
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    den->Add(numerator[TT]);
    cout << __LINE__ << endl;
    den->Add(numerator[WMu]);
    cout << __LINE__ << endl;
    den->Add(numerator[QCD]);
    cout << __LINE__ << endl;
    den->Add(numerator[WW]);
    cout << __LINE__ << endl;
    den->Add(numerator[WZ]);
    cout << __LINE__ << endl;
    den->Add(numerator[ZZ]);
    cout << __LINE__ << endl;
    den->Multiply(denominator[QCD]);
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    num->Divide(den);
    cout << __LINE__ << endl;

    delete den;
    return num;
}

TH1D* FRBytRatio(TH1D** numerator, TH1D** denominator) {

    cout << __LINE__ << endl;
    TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_xsec_barrel" : "FR_xsec_endcap";
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    TH1D* num = (TH1D*)denominator[DY1050]->Clone(name);
    cout << __LINE__ << endl;
    num->Add(numerator[DY50100]);
    cout << __LINE__ << endl;
    num->Add(numerator[DY100200]);
    cout << __LINE__ << endl;
    num->Add(numerator[DY200400]);
    cout << __LINE__ << endl;
    num->Add(numerator[DY4001000]);
    cout << __LINE__ << endl;
    TH1D* den = (TH1D*)numerator[DY1050]->Clone(name+"_");
    cout << __LINE__ << endl;
    den->Add(numerator[DY50100]);
    cout << __LINE__ << endl;
    den->Add(numerator[DY100200]);
    cout << __LINE__ << endl;
    den->Add(numerator[DY200400]);
    cout << __LINE__ << endl;
    den->Add(numerator[DY4001000]);
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    num->Add(denominator[TT]);
    cout << __LINE__ << endl;
    num->Add(denominator[WMu]);
    cout << __LINE__ << endl;
    cout << num->GetNbinsX() << " " << denominator[QCD]->GetName() << endl;
    num->Add(denominator[QCD]);
    cout << __LINE__ << endl;
    cout << num->GetNbinsX() << " " << denominator[WW]->GetName() << endl;
    num->Add(denominator[WW]);
    cout << __LINE__ << endl;
    num->Add(denominator[WZ]);
    cout << __LINE__ << endl;
    num->Add(denominator[ZZ]);
    cout << __LINE__ << endl;
    num->Multiply(numerator[QCD]);
    cout << __LINE__ << endl;
    TH1D *num_dataAll = (TH1D*) numerator[Data1]->Clone(name+"_tmp");
    cout << __LINE__ << endl;
    num_dataAll->Add(numerator[Data2]);
    cout << __LINE__ << endl;
    num->Multiply(num_dataAll);
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    den->Add(numerator[TT]);
    cout << __LINE__ << endl;
    den->Add(numerator[WMu]);
    cout << __LINE__ << endl;
    cout << den->GetNbinsX() << " " << numerator[QCD]->GetName() << endl;
    den->Add(numerator[QCD]);
    cout << __LINE__ << endl;
    cout << den->GetNbinsX() << " " << numerator[WW]->GetName() << endl;
    den->Add(numerator[WW]);
    cout << __LINE__ << endl;
    den->Add(numerator[WZ]);
    cout << __LINE__ << endl;
    den->Add(numerator[ZZ]);
    cout << __LINE__ << endl;
    den->Multiply(denominator[QCD]);
    cout << __LINE__ << endl;
    TH1D *den_dataAll = (TH1D*) denominator[Data1]->Clone(name+"_tmp");
    cout << __LINE__ << endl;
    den_dataAll->Add(denominator[Data2]);
    cout << __LINE__ << endl;
    den->Multiply(den_dataAll);
    cout << __LINE__ << endl;

    cout << __LINE__ << endl;
    num->Divide(den);

    delete den;
    return num;
}
