#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
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

TH1D* FRCheck(TH1D** numerator, TH1D** denominator);


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

    double norm_all[NSamples+2];
    double norm_fit_barrel[NSamples+2];
    double norm_fit_endcap[NSamples+2];

	 // Stack is not working yet..need to solve
	 THStack *hs_fit = new THStack("hs_fit","fit stacked");
	 THStack *hs_xsec = new THStack("hs_xsec","xsec stacked");
	 THStack *hs_data = new THStack("hs_data","data stacked");
	 
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

       norm_all[i] = (Xsec(tag)*lumi_all)/Nevts(tag);
    }
/*
    // NB: the numbers below are the output of fitTemplate.cc
    double intDY_barrel=0, intDY_endcap=0;
    for (int i=DY1050; i<=DY4001000; i++) {
       intDY_barrel+=denominator_barrel[i]->Integral();
       intDY_endcap+=denominator_endcap[i]->Integral();
    }
    for (int i=DY1050; i<=DY4001000; i++) {
       norm_fit_barrel[i] = 1.8340e+04/intDY_barrel;
    }
    norm_fit_barrel[TT] = 1.5495e+04/denominator_barrel[TT]->Integral();
    norm_fit_barrel[WMu] = 8.3640e+04/denominator_barrel[WMu]->Integral();
    norm_fit_barrel[QCD] = 1.4770e+05/denominator_barrel[QCD]->Integral();
    norm_fit_barrel[WW] = 7.1414e+02/denominator_barrel[WW]->Integral();
    norm_fit_barrel[WZ] = 1.8417e+02/denominator_barrel[WZ]->Integral();
    norm_fit_barrel[ZZ] = 1.4233e+01/denominator_barrel[ZZ]->Integral();

    for (int i=DY1050; i<=DY4001000; i++) {
       norm_fit_endcap[i] = 1.4295e+04/intDY_barrel;
    }
    norm_fit_endcap[TT] = 7.5868e+03/denominator_endcap[TT]->Integral();
    norm_fit_endcap[WMu] = 8.4252e+04/denominator_endcap[WMu]->Integral();
    norm_fit_endcap[QCD] = 1.0105e+05/denominator_endcap[QCD]->Integral();
    norm_fit_endcap[WW] = 5.6528e+02/denominator_endcap[WW]->Integral();
    norm_fit_endcap[WZ] = 1.4257e+02/denominator_endcap[WZ]->Integral();
    norm_fit_endcap[ZZ] = 1.0795e+01/denominator_endcap[ZZ]->Integral();

  */
    // NB: the numbers below are the output of fitTemplate.cc
/*
    double intDY_barrel=0, intDY_endcap=0;
    for (int i=DYMuMu1030; i<=DYMuMu30; i++) {
       intDY_barrel+=denominator_barrel[i]->Integral();
       intDY_endcap+=denominator_endcap[i]->Integral();
    }
    for (int i=DYMuMu1030; i<=DYMuMu30; i++) {
       norm_fit_barrel[i] = 6.0491e+04/intDY_barrel;
    }
    norm_fit_barrel[TT] =9.6190e+02/denominator_barrel[TT]->Integral();
    norm_fit_barrel[WpMu] = 1.1110e+05/denominator_barrel[WpMu]->Integral();
    norm_fit_barrel[QCD] = 1.1869e+06/denominator_barrel[QCD]->Integral();
    norm_fit_barrel[WW] = 3.8305e+01/denominator_barrel[WW]->Integral();
    norm_fit_barrel[WZ] = 1.7711e+01/denominator_barrel[WZ]->Integral();
    norm_fit_barrel[ZZ] = 6.7838e+00/denominator_barrel[ZZ]->Integral();

    for (int i=DYMuMu1030; i<=DYMuMu30; i++) {
       norm_fit_endcap[i] = 4.7340e+04/intDY_barrel;
    }
    norm_fit_endcap[TT] = 4.4251e+02/denominator_endcap[TT]->Integral();
    norm_fit_endcap[WpMu] = 1.0743e+05/denominator_endcap[WpMu]->Integral();
    norm_fit_endcap[QCD] = 8.7395e+05/denominator_endcap[QCD]->Integral();
    norm_fit_endcap[WW] = 2.7097e+01/denominator_endcap[WW]->Integral();
    norm_fit_endcap[WZ] = 1.2242e+01/denominator_endcap[WZ]->Integral();
    norm_fit_endcap[ZZ] = 4.6082e+00/denominator_endcap[ZZ]->Integral();
*/

    // NB: the numbers below are the output of fitTemplate.cc
    double intDY_barrel=0, intDY_endcap=0;
    for (int i=DYTauTau1030; i<=DYMuMu30_PbP; i++) {
       intDY_barrel+=denominator_barrel[i]->Integral();
       intDY_endcap+=denominator_endcap[i]->Integral();
    }
    double intWJets_barrel=0, intWJets_endcap=0;
    for (int i=WpMu; i<=WmTau; i++) {
       intWJets_barrel+=denominator_barrel[i]->Integral();
       intWJets_endcap+=denominator_endcap[i]->Integral();
    }

    for (int i=DYTauTau1030; i<=DYMuMu30_PbP; i++) {
       norm_fit_barrel[i] = 7.6047e+04/intDY_barrel;
        //norm_fit_barrel[i] = 1.0444e+05/intDY_barrel;
    }
    norm_fit_barrel[TT] =4.2011e+04/denominator_barrel[TT]->Integral();
    //norm_fit_barrel[TT] =7.7207e+02/denominator_barrel[TT]->Integral();

    for (int i=WpMu; i<=WmTau; i++) {
       norm_fit_barrel[i] = 5.8947e+04/intWJets_barrel;
       //norm_fit_barrel[i] = 1.8119e+04/intWJets_barrel;
    }

    norm_fit_barrel[QCD] = 1.1815e+06/denominator_barrel[QCD]->Integral();
    norm_fit_barrel[WW] = 5.3532e+01/denominator_barrel[WW]->Integral();
    norm_fit_barrel[WZ] = 8.1925e+00/denominator_barrel[WZ]->Integral();
    norm_fit_barrel[ZZ] = 6.0809e+00/denominator_barrel[ZZ]->Integral();

	 std::cout << "#######################" << std::endl;
	 std::cout << "norm_fit_barrel[QCD]: "<< norm_fit_barrel[QCD] << std::endl;
/*
    norm_fit_barrel[QCD] = 1.8470e+06/denominator_barrel[QCD]->Integral();
    norm_fit_barrel[WW] = 2.3510e+01/denominator_barrel[WW]->Integral();
    norm_fit_barrel[WZ] = 1.0898e+01/denominator_barrel[WZ]->Integral();
    norm_fit_barrel[ZZ] = 4.1737e+00/denominator_barrel[ZZ]->Integral();
*/
    for (int i=DYTauTau1030; i<=DYMuMu30_PbP; i++) {
       norm_fit_endcap[i] = 5.8169e+04/intDY_endcap;
//       norm_fit_endcap[i] = 7.3860e+04/intDY_endcap;
    }
    norm_fit_endcap[TT] = 1.9147e+04/denominator_endcap[TT]->Integral();
//    norm_fit_endcap[TT] = 3.0594e+02/denominator_endcap[TT]->Integral();
    for (int i=WpMu; i<=WmTau; i++) {
       norm_fit_endcap[i] = 7.9277e+04/intWJets_endcap;
//       norm_fit_endcap[i] = 3.6968e+04/intWJets_endcap;
    }
    norm_fit_endcap[QCD] = 8.7160e+05/denominator_endcap[QCD]->Integral();
    norm_fit_endcap[WW] = 4.8647e+02/denominator_endcap[WW]->Integral();
    norm_fit_endcap[WZ] = 5.3017e+02/denominator_endcap[WZ]->Integral();
    norm_fit_endcap[ZZ] = 1.9886e+02/denominator_endcap[ZZ]->Integral();

/*
    norm_fit_endcap[QCD] = 1.2412e+06/denominator_endcap[QCD]->Integral();
    norm_fit_endcap[WW] = 1.4999e+01/denominator_endcap[WW]->Integral();
    norm_fit_endcap[WZ] = 6.7946e+00/denominator_endcap[WZ]->Integral();
    norm_fit_endcap[ZZ] = 2.5479e+00/denominator_endcap[ZZ]->Integral();
*/

	  for(int i=0;i<=QCD;i++) {
       if (i==ALL) continue;
       SampleTag tag = static_cast<SampleTag>(i);
       denominator_barrel[i]->Scale(norm_fit_barrel[i]);
       denominator_pt_fit_barrel[i]->Scale(norm_fit_barrel[i]);
       denominator_pt_xsec_barrel[i]->Scale(norm_all[i]);
       numerator_pt_barrel[i]->Scale(norm_all[i]);

       denominator_endcap[i]->Scale(norm_fit_endcap[i]);
       denominator_pt_fit_endcap[i]->Scale(norm_fit_endcap[i]);
       denominator_pt_xsec_endcap[i]->Scale(norm_all[i]);
       numerator_pt_endcap[i]->Scale(norm_all[i]);

		 std::cout << "#######################" << std::endl;
		 std::cout << "scaled denominator_pt_fit_barrel[" << i << "]: " << denominator_pt_fit_barrel[i]->Integral() << std::endl;
		 std::cout << "scaled denominator_pt_fit_endcap[" << i << "]: " << denominator_pt_fit_endcap[i]->Integral() << std::endl;
	
		 std::cout << "#######################" << std::endl;

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

	if (IsData(tag)) {
		hs_data->Add(denominator_pt_xsec_barrel[i]);
	}
	else {
		hs_xsec->Add(denominator_pt_xsec_barrel[i]);
		hs_fit->Add(denominator_pt_fit_barrel[i]);
   }
/*	
	TH1D* hratio_xsec;
	hratio_xsec=(TH1D*)denominator_pt_xsec_barrel[i]->Clone();
	hratio_xsec->Sumw2();
	hratio_xsec->Divide(denominator_pt_xsec_barrel[i],denominator_pt_xsec_barrel[i],1,1,"B");

	TH1D* hratio_fit;
	hratio_fit=(TH1D*)denominator_pt_fit_barrel[i]->Clone();
	hratio_fit->Sumw2();
	hratio_fit->Divide(denominator_pt_fit_barrel[i],denominator_pt_fit_barrel[i],1,1,"B");
*/
	TH1D* hratio_xsecvsfit;
	hratio_xsecvsfit=(TH1D*)denominator_pt_fit_barrel[i]->Clone();
	hratio_xsecvsfit->Sumw2();
	hratio_xsecvsfit->Divide(denominator_pt_xsec_barrel[i],denominator_pt_fit_barrel[i],1,1,"B");

	TCanvas* chs = new TCanvas("chs","",600,800);
	chs->SetFillColor(0);

	TPad* pad1 = new TPad("pad1","",0.0,0.30,1.0,1.0,21);
	TPad* pad2 = new TPad("pad2","",0.0,0.0,1.0,0.30,21);

	pad1->SetFillColor(0);
	pad2->SetFillColor(0);

	pad1->SetTopMargin(0.05);
	pad1->SetBottomMargin(0.0);
	pad1->SetLeftMargin(0.15);
	pad1->SetRightMargin(0.05);

	pad2->SetTopMargin(0.0);
	pad2->SetBottomMargin(0.3);
	pad2->SetLeftMargin(0.15);
	pad2->SetRightMargin(0.05);

	pad1->Draw();
	pad2->Draw();

	pad1->cd();
	pad1->SetLogy(1);
	denominator_pt_xsec_barrel[i]->GetYaxis()->SetRangeUser(0.1,10*denominator_pt_xsec_barrel[i]->GetMaximum());
	denominator_pt_xsec_barrel[i]->Draw("hist");
	denominator_pt_fit_barrel[i]->Draw("same pe");

	pad2->cd();
	pad2->SetLogy(0);
	hratio_xsecvsfit->SetMarkerStyle(20);
	hratio_xsecvsfit->SetMarkerColor(kBlack);

   hratio_xsecvsfit->GetXaxis()->SetTitle("muon's p_{T} (GeV)");

   hratio_xsecvsfit->GetXaxis()->SetTitleSize(0.140);
   hratio_xsecvsfit->GetXaxis()->SetLabelSize(0.100);
   hratio_xsecvsfit->GetYaxis()->SetTitle("Fitting/Xsection");
   hratio_xsecvsfit->GetYaxis()->SetTitleSize(0.140);
   hratio_xsecvsfit->GetYaxis()->SetLabelSize(0.070);
   hratio_xsecvsfit->GetYaxis()->SetTitleOffset(0.4);
   hratio_xsecvsfit->GetXaxis()->SetTitleOffset(1.0);

	
	hratio_xsecvsfit->Draw("pe");
	chs->SaveAs(Form("comppt_%s.pdf",Name(tag)));	
}
	 TCanvas* chs1 = new TCanvas("chs1","stacked hists",600,600);
	 chs1->SetLogy(1);
	 hs_data->Draw("");
	 hs_xsec->Draw("same");
	 chs1->SaveAs("comp_data_vs_xsec.pdf");
	 hs_data->Draw("");
	 hs_fit->Draw("same");
	 chs1->SaveAs("comp_data_vs_fit.pdf");



    TH1D* FR_template_barrel = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_fit_barrel);
    TH1D* FR_template_endcap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_fit_endcap);

    TH1D* FR_xsec_barrel = (TH1D*)FRBytRatio(numerator_pt_barrel, denominator_pt_xsec_barrel);
    TH1D* FR_xsec_endcap = (TH1D*)FRBytRatio(numerator_pt_endcap, denominator_pt_xsec_endcap);

    TH1D* FR_template_barrel_swap = (TH1D*)FRByTemplate(numerator_pt_barrel, denominator_pt_xsec_barrel);
    TH1D* FR_template_endcap_swap = (TH1D*)FRByTemplate(numerator_pt_endcap, denominator_pt_xsec_endcap);

    TH1D* FR_xsec_barrel_swap = (TH1D*)FRBytRatio(numerator_pt_barrel, denominator_pt_fit_barrel);
    TH1D* FR_xsec_endcap_swap = (TH1D*)FRBytRatio(numerator_pt_endcap, denominator_pt_fit_endcap);


	 TH1D* FR_check_barrel = (TH1D*)FRCheck(numerator_pt_barrel, denominator_pt_xsec_barrel);
	 TH1D* FR_check_endcap = (TH1D*)FRCheck(numerator_pt_endcap, denominator_pt_xsec_endcap);


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
/*
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
*/
    TH2D* ptFrame = new TH2D("ptFrame","",8,47,500,105,-0.05,1.00);
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
  

    TLegend* legend2 = new TLegend(.60,.70,.90,.89);
    legend2->AddEntry(FR_template_barrel,"Template fitting","lp");
    legend2->AddEntry(FR_xsec_barrel,"Ratio method","lp");
    legend2->SetBorderSize(0);

    ptFrame->Draw();
    CMS_lumi(canv,4,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    FR_template_barrel->Draw("");
    FR_xsec_barrel->Draw("same");
    legend2->Draw("SAME");
    canv->Print("print/FR_Barrel.pdf");

    canv->Clear();
    ptFrame->Draw();
    CMS_lumi(canv,4,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    FR_template_endcap->Draw("");
    FR_xsec_endcap->Draw("same");
    legend2->Draw("SAME");
    canv->Print("print/FR_Endcap.pdf");

    FR_template_barrel_swap->SetMarkerSize(3);
    FR_template_endcap_swap->SetMarkerSize(3);
    FR_xsec_barrel_swap->SetMarkerSize(3);
    FR_xsec_endcap_swap->SetMarkerSize(3);

    FR_template_barrel_swap->SetLineColor(1);
    FR_template_barrel_swap->SetLineWidth(2);
    FR_template_barrel_swap->SetMarkerStyle(20);
    FR_template_barrel_swap->SetMarkerColor(1);

    FR_xsec_barrel_swap->SetLineColor(2);
    FR_xsec_barrel_swap->SetLineWidth(2);
    FR_xsec_barrel_swap->SetMarkerStyle(21);
    FR_xsec_barrel_swap->SetMarkerColor(2);

    FR_template_endcap_swap->SetLineColor(1);
    FR_template_endcap_swap->SetLineWidth(2);
    FR_template_endcap_swap->SetMarkerStyle(20);
    FR_template_endcap_swap->SetMarkerColor(1);

    FR_xsec_endcap_swap->SetLineColor(2);
    FR_xsec_endcap_swap->SetLineWidth(2);
    FR_xsec_endcap_swap->SetMarkerStyle(21);
    FR_xsec_endcap_swap->SetMarkerColor(2);

     ptFrame->Draw();
    CMS_lumi(canv,4,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    FR_template_barrel_swap->Draw("");
    FR_xsec_barrel_swap->Draw("same");
    legend2->Draw("SAME");
    canv->Print("print/FR_Barrel_swap.pdf");

    canv->Clear();
    ptFrame->Draw();
    CMS_lumi(canv,4,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    FR_template_endcap_swap->Draw("");
    FR_xsec_endcap_swap->Draw("same");
    legend2->Draw("SAME");
    canv->Print("print/FR_Endcap_swap.pdf");


    TFile* g = new TFile("result/fakerate.root","RECREATE");
    FR_template_barrel->Write();
    FR_template_endcap->Write();
    FR_xsec_barrel->Write();
    FR_xsec_endcap->Write();
    FR_template_barrel_swap->Write();
    FR_template_endcap_swap->Write();
    FR_xsec_barrel_swap->Write();
    FR_xsec_endcap_swap->Write();


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
    cout<<"Check Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_check_barrel->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Check Endcap"<<endl;
    for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_check_endcap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Template/Ratio Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<(FR_template_barrel->GetBinContent(i))/(FR_xsec_barrel->GetBinContent(i));
    }
    cout<<endl;
    cout<<"Template/Ratio Endcap"<<endl;
        for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<(FR_template_endcap->GetBinContent(i))/(FR_xsec_endcap->GetBinContent(i));
    }
    cout<<endl;


	 cout << "###### SWAP ############" << endl;
    cout<<"Template Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_template_barrel_swap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Template Endcap"<<endl;
    for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_template_endcap_swap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Ratio Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_xsec_barrel_swap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Ratio Endcap"<<endl;
        for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<FR_xsec_endcap_swap->GetBinContent(i);
    }
    cout<<endl;
    cout<<"Template/Ratio Barrel"<<endl;
    for(int i=1; i<ptbinnum+1; i++) {
        if(i!=1) cout<<",";
        cout<<(FR_template_barrel_swap->GetBinContent(i))/(FR_xsec_barrel_swap->GetBinContent(i));
    }
    cout<<endl;
    cout<<"Template/Ratio Endcap"<<endl;
        for(int i=1; i<ptbinnum_endcap+1; i++) {
        if(i!=1) cout<<",";
        cout<<(FR_template_endcap_swap->GetBinContent(i))/(FR_xsec_endcap_swap->GetBinContent(i));
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

    TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_template_barrel" : "FR_template_endcap";

    TH1D* num = (TH1D*)numerator[Data1]->Clone(name);
    num->Add(numerator[Data2]);
/*
    TH1D* den = (TH1D*)numerator[DY1050]->Clone(name+"_");
    den->Add(numerator[DY50100]);
    den->Add(numerator[DY100200]);
    den->Add(numerator[DY200400]);
    den->Add(numerator[DY4001000]);
*/
    TH1D* den = (TH1D*)numerator[DYTauTau1030]->Clone(name+"_");
    den->Add(numerator[DYTauTau30]);
    den->Add(numerator[DYMuMu1030]);
    den->Add(numerator[DYMuMu30]);
    den->Add(numerator[DYMuMu1030_PbP]);
    den->Add(numerator[DYMuMu30_PbP]);




    num->Multiply(numerator[QCD]);

    den->Add(numerator[TT]);
//    den->Add(numerator[WMu]);
    den->Add(numerator[WpMu]);
    den->Add(numerator[WmMu]);
    den->Add(numerator[WpTau]);
    den->Add(numerator[WmTau]);

    den->Add(numerator[QCD]);
    den->Add(numerator[WW]);
    den->Add(numerator[WZ]);
    den->Add(numerator[ZZ]);
    den->Multiply(denominator[QCD]);

    num->Divide(den);

    delete den;
    return num;
}

TH1D* FRBytRatio(TH1D** numerator, TH1D** denominator) {

    TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_xsec_barrel" : "FR_xsec_endcap";
/*
    TH1D* num = (TH1D*)denominator[DY1050]->Clone(name);
    num->Add(numerator[DY50100]);
    num->Add(numerator[DY100200]);
    num->Add(numerator[DY200400]);
    num->Add(numerator[DY4001000]);
    TH1D* den = (TH1D*)numerator[DY1050]->Clone(name+"_");
    den->Add(numerator[DY50100]);
    den->Add(numerator[DY100200]);
    den->Add(numerator[DY200400]);
    den->Add(numerator[DY4001000]);
*/

    TH1D* num = (TH1D*)denominator[DYTauTau1030]->Clone(name);
    num->Add(denominator[DYTauTau30]);
    num->Add(denominator[DYMuMu1030]);
    num->Add(denominator[DYMuMu30]);
    num->Add(denominator[DYMuMu1030_PbP]);
    num->Add(denominator[DYMuMu30_PbP]);

    TH1D* den = (TH1D*)numerator[DYTauTau1030]->Clone(name+"_");
    den->Add(numerator[DYTauTau30]);
    den->Add(numerator[DYMuMu1030]);
    den->Add(numerator[DYMuMu30]);
    den->Add(numerator[DYMuMu1030_PbP]);
    den->Add(numerator[DYMuMu30_PbP]);

    num->Add(denominator[TT]);
//    num->Add(denominator[WMu]);
	 num->Add(denominator[WpMu]);
	 num->Add(denominator[WmMu]);
	 num->Add(denominator[WpTau]);
	 num->Add(denominator[WmTau]);

    num->Add(denominator[QCD]);
    num->Add(denominator[WW]);
    num->Add(denominator[WZ]);
    num->Add(denominator[ZZ]);
    num->Multiply(numerator[QCD]);

    //TH1D* num = (TH1D*)numerator[QCD]->Clone(name);
    TH1D *num_dataAll = (TH1D*) numerator[Data1]->Clone(name+"_tmp");
    num_dataAll->Add(numerator[Data2]);
    num->Multiply(num_dataAll);

    den->Add(numerator[TT]);
//    den->Add(numerator[WMu]);
	 den->Add(numerator[WpMu]);
	 den->Add(numerator[WmMu]);
	 den->Add(numerator[WpTau]);
	 den->Add(numerator[WmTau]);

    den->Add(numerator[QCD]);
    den->Add(numerator[WW]);
    den->Add(numerator[WZ]);
    den->Add(numerator[ZZ]);
    den->Multiply(denominator[QCD]);
    TH1D *den_dataAll = (TH1D*) denominator[Data1]->Clone(name+"_tmp");
    den_dataAll->Add(denominator[Data2]);
    den->Multiply(den_dataAll);

    num->Divide(den);

    delete den;
    return num;
}

TH1D* FRCheck(TH1D** numerator, TH1D** denominator) {

    TString name = ( ((TString)(denominator[QCD]->GetName())).Contains("barrel") ) ? "FR_check_barrel" : "FR_check_endcap";

    TH1D* num = (TH1D*)denominator[DYTauTau1030]->Clone(name);
    num->Add(denominator[DYTauTau30]);
    num->Add(denominator[DYMuMu1030]);
    num->Add(denominator[DYMuMu30]);
    num->Add(denominator[DYMuMu1030_PbP]);
    num->Add(denominator[DYMuMu30_PbP]);

    num->Add(denominator[TT]);
//    num->Add(denominator[WMu]);
	 num->Add(denominator[WpMu]);
	 num->Add(denominator[WmMu]);
	 num->Add(denominator[WpTau]);
	 num->Add(denominator[WmTau]);

    num->Add(denominator[QCD]);
    num->Add(denominator[WW]);
    num->Add(denominator[WZ]);
    num->Add(denominator[ZZ]);

    TH1D *den = (TH1D*) denominator[Data1]->Clone(name+"_tmp");
    den->Add(denominator[Data2]);

    num->Divide(den);

    delete den;
    return num;
}
