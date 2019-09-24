#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TPaveText.h>

#include "../../interface/analysis.h"
#include "../../interface/defs.h"
#include "../../interface/tdrstyle.C"
#include "../../interface/CMS_lumi.C"



using namespace DYana;
using namespace std;

void DrawPlot(SampleTag index, int cate,TString rmk, TH1D* num,TH1D* den);

void DoCompHist(SampleTag index) {
	std::cout << "Open files" << std::endl;
	TFile* fin = new TFile(PathFRHistos(index),"READ");
	std::cout << PathFRHistos(index) << std::endl;

	TH1D* numerator_pt;
	TH1D* numerator_pt_barrel;
	TH1D* numerator_pt_endcap;
	TH1D* numerator_eta;
	TH1D* numerator_iso;
	TH1D* numerator_iso_barrel;
	TH1D* numerator_iso_endcap;

	numerator_pt = (TH1D*)fin->Get("numerator_pt")->Clone("numerator_pt"+TString(Name(index)));
	numerator_pt_barrel = (TH1D*)fin->Get("numerator_pt_barrel")->Clone("numerator_pt_barrel"+TString(Name(index)));
	numerator_pt_endcap = (TH1D*)fin->Get("numerator_pt_endcap")->Clone("numerator_pt_endcap"+TString(Name(index)));
	numerator_eta = (TH1D*)fin->Get("numerator_eta")->Clone("numerator_eta"+TString(Name(index)));
	numerator_iso = (TH1D*)fin->Get("numerator")->Clone("numerator_iso"+TString(Name(index)));
	numerator_iso_barrel = (TH1D*)fin->Get("numerator_barrel")->Clone("numerator_iso_barrel"+TString(Name(index)));
	numerator_iso_endcap = (TH1D*)fin->Get("numerator_endcap")->Clone("numerator_iso_endcap"+TString(Name(index)));

	numerator_pt->Sumw2();
	numerator_pt_barrel->Sumw2();
	numerator_pt_endcap->Sumw2();
	numerator_eta->Sumw2();
	numerator_iso->Sumw2();
	numerator_iso_barrel->Sumw2();
	numerator_iso_endcap->Sumw2();

	TH1D* denominator_pt;
	TH1D* denominator_pt_barrel;
	TH1D* denominator_pt_endcap;
	TH1D* denominator_eta;
	TH1D* denominator_iso;
	TH1D* denominator_iso_barrel;
	TH1D* denominator_iso_endcap;

	denominator_pt = (TH1D*)fin->Get("denominator_pt")->Clone("denominator_pt"+TString(Name(index)));
	denominator_pt_barrel = (TH1D*)fin->Get("denominator_pt_barrel")->Clone("denominator_pt_barrel"+TString(Name(index)));
	denominator_pt_endcap = (TH1D*)fin->Get("denominator_pt_endcap")->Clone("denominator_pt_endcap"+TString(Name(index)));
	denominator_eta = (TH1D*)fin->Get("denominator_eta")->Clone("denominator_eta"+TString(Name(index)));
	denominator_iso = (TH1D*)fin->Get("denominator")->Clone("denominator_iso"+TString(Name(index)));
	denominator_iso_barrel = (TH1D*)fin->Get("denominator_barrel")->Clone("denominator_iso_barrel"+TString(Name(index)));
	denominator_iso_endcap = (TH1D*)fin->Get("denominator_endcap")->Clone("denominator_iso_endcap"+TString(Name(index)));

	denominator_pt->Sumw2();
	denominator_pt_barrel->Sumw2();
	denominator_pt_endcap->Sumw2();
	denominator_eta->Sumw2();
	denominator_iso->Sumw2();
	denominator_iso_barrel->Sumw2();
	denominator_iso_endcap->Sumw2();

	DrawPlot(index,1,"pt",numerator_pt,denominator_pt);
	DrawPlot(index,1,"pt_barrel",numerator_pt_barrel,denominator_pt_barrel);
	DrawPlot(index,1,"pt_endcap",numerator_pt_endcap,denominator_pt_endcap);
	DrawPlot(index,3,"eta",numerator_eta,denominator_eta);
	DrawPlot(index,2,"iso",numerator_iso,denominator_iso);
	DrawPlot(index,2,"iso_barrel",numerator_iso_barrel,denominator_iso_barrel);
	DrawPlot(index,2,"iso_endcap",numerator_iso_endcap,denominator_iso_endcap);

}
void DrawPlot(SampleTag index,int cate, TString rmk, TH1D* num,TH1D* den) {
	TH1D* hratio;
	hratio = (TH1D*)num->Clone();
	hratio->Sumw2();

	hratio->Divide(num,den,1,1,"B");
	TCanvas* c1 = new TCanvas("c1","",600,800);
	c1->SetFillColor(0);
	c1->SetGrid(1);
	gStyle->SetOptStat(0);

	TPad* pad1 = new TPad("pad1","",0.0,0.30,1.0,1.0,21);//xlow, ylow, xup, yup, color
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
	//pad1->SetLogx(1);
	 
	den->SetFillColor(8);
	num->SetMarkerStyle(20);
	num->SetMarkerColor(kBlack);
	num->SetLineColor(kBlack);

	den->GetYaxis()->SetTitle("Entries");
	den->GetYaxis()->SetTitleSize(0.080);
	den->GetYaxis()->SetLabelSize(0.050);
	den->GetYaxis()->SetTitleOffset(0.8);
	den->SetMinimum(0.5);
	den->SetMaximum(100.0*den->GetMaximum());


	den->Draw("hist");
	num->Draw("same pe");

	TLegend* leg = new TLegend(0.50,0.70,0.90,0.90);
	leg->SetFillColor(kWhite);
	leg->SetLineWidth(0);
	leg->AddEntry(num,"Numerator   (tight cut)", "EP");
	leg->AddEntry(den,"Denominator (loose cut)", "F");
	leg->Draw();

	TPaveText *varname = new TPaveText(0.18,0.80,0.35,0.90,"brNDC");
	varname->SetBorderSize(0);
	varname->SetFillColor(0);
	varname->SetFillStyle(0);
	varname->SetTextSize(0.05);
	varname->SetTextFont(42);
	varname->SetTextAlign(12);

	varname->AddText(Form("%s",Name(index)));
	varname->Draw();

	pad2->cd();
	//pad2->SetLogx(1);
	hratio->SetMarkerStyle(20);
	hratio->SetMarkerColor(kBlack);

	if (cate==1) hratio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	else if (cate==2) hratio->GetXaxis()->SetTitle("PFIso/p_{T}");
	else if (cate==3) hratio->GetXaxis()->SetTitle("#eta");

	hratio->GetXaxis()->SetTitleSize(0.140);
	hratio->GetXaxis()->SetLabelSize(0.100);
	hratio->GetYaxis()->SetTitle("Tight/Loose");
	hratio->GetYaxis()->SetTitleSize(0.140);
	hratio->GetYaxis()->SetLabelSize(0.070);
	hratio->GetYaxis()->SetTitleOffset(0.4);
	hratio->GetXaxis()->SetTitleOffset(1.0);

	hratio->Draw("pe");
	c1->SaveAs(Form("print/Dist_%s_%s.pdf",Name(index),rmk.Data()));
}



void CompHist () {
	std::cout << "##### START Chain #####" << std::endl;

	for (int i=0; i<=QCD; i++) {
//	for (int i=0; i<=0; i++) {
		SampleTag tag = static_cast<SampleTag>(i);
		if (tag==ALL) continue;
		std::cout << Name(tag) << std::endl;
		DoCompHist(tag);
	}

}

