#include <iostream>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFrame.h>

#include "tdrstyle.C"
#include "CMS_lumi.C"

using namespace std;

void CompXsec () {
		  TFile* fin = TFile::Open("xsec_nom.root");

		  TGraphAsymmErrors* gaeres_rap1560 = (TGraphAsymmErrors*)fin->Get("gres_rap1560");
		  TGraphAsymmErrors* gaeres_rap60120 = (TGraphAsymmErrors*)fin->Get("gres_rap60120");
		  const int rfb_rapbinnum_1560 = 5;
		  double rfb_rapbin_1560[rfb_rapbinnum_1560+1] = {0.0,0.4,0.8,1.2,1.6,1.93};
		  const int rfb_rapbinnum_60120 = 10;
		  double rfb_rapbin_60120[rfb_rapbinnum_60120+1] = {0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,1.93};

		  TH1D* grfbp_rap1560 = new TH1D("grfbp_rap1560",";|y_{CM}| (15<M<60 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_1560,rfb_rapbin_1560);
		  TH1D* grfbp_rap60120 = new TH1D("grfbp_rap60120",";|y_{CM}| (60<M<120 (GeV/c^{2});Forward/Backward (Xsec)",rfb_rapbinnum_60120,rfb_rapbin_60120);

		  TH1D* f_rap1560_temp = new TH1D("f_rap1560_temp",";|y_{CM}| (15<M<60 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_1560,rfb_rapbin_1560);
		  TH1D* b_rap1560_temp = new TH1D("b_rap1560_temp",";|y_{CM}| (15<M<60 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_1560,rfb_rapbin_1560);

		  f_rap1560_temp->Sumw2();
		  b_rap1560_temp->Sumw2();

		  TH1D* f_rap60120_temp = new TH1D("f_rap60120_temp",";|y_{CM}| (60<M<120 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_60120,rfb_rapbin_60120);
		  TH1D* b_rap60120_temp = new TH1D("b_rap60120_temp",";|y_{CM}| (60<M<120 (GeV/c^{2}));Forward/Backward (Xsec)",rfb_rapbinnum_60120,rfb_rapbin_60120);

		  f_rap60120_temp->Sumw2();
		  b_rap60120_temp->Sumw2();


		  grfbp_rap1560->Sumw2();
		  grfbp_rap60120->Sumw2();

		  for (int i=2;i<2+rfb_rapbinnum_1560;i++) {
					 std::cout << gaeres_rap1560->GetY()[13-i] << " / " << gaeres_rap1560->GetY()[i] << std::endl;
					 if (gaeres_rap1560->GetY()[13-i]<=0) {f_rap1560_temp->SetBinContent(7-i,0.0);f_rap1560_temp->SetBinError(7-i,0.0);}
					 else {f_rap1560_temp->SetBinContent(7-i,gaeres_rap1560->GetY()[13-i]);f_rap1560_temp->SetBinError(7-i,gaeres_rap1560->GetErrorY(13-i));}
					 if (gaeres_rap1560->GetY()[i]<=0) {b_rap1560_temp->SetBinContent(7-i,0.0);b_rap1560_temp->SetBinError(7-i,0.0);}
					 else {b_rap1560_temp->SetBinContent(7-i,gaeres_rap1560->GetY()[i]);b_rap1560_temp->SetBinError(7-i,gaeres_rap1560->GetErrorY(i));}
		  }
		  /*
			  for (int i=0;i<2+rfb_rapbinnum_1560;i++) {
			  std::cout << f_rap1560_temp->GetBinContent(i) << " / " << b_rap1560_temp->GetBinContent(i) << std::endl;
			  }
			*/
		  grfbp_rap1560->Divide(f_rap1560_temp,b_rap1560_temp,1,1);

		  std::cout << "------------------------" << std::endl;

		  for (int i=4;i<4+rfb_rapbinnum_60120;i++) {
					 std::cout << gaeres_rap60120->GetY()[27-i] << " / " << gaeres_rap60120->GetY()[i] << std::endl;
					 if (gaeres_rap60120->GetY()[27-i]<=0) {f_rap60120_temp->SetBinContent(14-i,0.0);f_rap60120_temp->SetBinError(14-i,0.0);}
					 else {f_rap60120_temp->SetBinContent(14-i,gaeres_rap60120->GetY()[27-i]);f_rap60120_temp->SetBinError(14-i,gaeres_rap60120->GetErrorY(27-i));}
					 if (gaeres_rap60120->GetY()[i]<=0) {b_rap60120_temp->SetBinContent(14-i,0.0);b_rap60120_temp->SetBinError(14-i,0.0);}
					 else {b_rap60120_temp->SetBinContent(14-i,gaeres_rap60120->GetY()[i]);b_rap60120_temp->SetBinError(14-i,gaeres_rap60120->GetErrorY(i));}
		  }
		  grfbp_rap60120->Divide(f_rap60120_temp,b_rap60120_temp,1,1);


		  /*
			  for (int i=0;i<14;i++) std::cout << i << "-" << gaeres_rap1560->GetY()[i] << std::endl;

			  for (int i=2;i<2+rfb_rapbinnum_1560;i++) {
			  std::cout << gaeres_rap1560->GetY()[13-i] << " / " << gaeres_rap1560->GetY()[i] << std::endl;
			  if (gaeres_rap1560->GetY()[13-i]<=0 || gaeres_rap1560->GetY()[i]<=0) {grfbp_rap1560->SetBinContent(7-i,0.0);grfbp_rap1560->SetBinError(7-i,0.0);}
			  else {
			  grfbp_rap1560->SetBinContent(7-i,gaeres_rap1560->GetY()[13-i]/gaeres_rap1560->GetY()[i]);
			  grfbp_rap1560->SetBinError(7-i,gaeres_rap1560->GetErrorY(13-i)/gaeres_rap1560->GetErrorY(i));
			  }
			  }

			  std::cout << std::endl;

			  for (int i=4;i<4+rfb_rapbinnum_60120;i++) {
			  std::cout << gaeres_rap60120->GetY()[27-i] << " / " << gaeres_rap60120->GetY()[i] << std::endl;
			  if (gaeres_rap60120->GetY()[27-i]<=0 || gaeres_rap60120->GetY()[i]<=0) {grfbp_rap60120->SetBinContent(14-i,0.0);grfbp_rap60120->SetBinError(14-i,0.0);}
			  else {
			  grfbp_rap60120->SetBinContent(14-i,gaeres_rap60120->GetY()[27-i]/gaeres_rap60120->GetY()[i]);
			  grfbp_rap60120->SetBinError(14-i,gaeres_rap60120->GetErrorY(27-i)/gaeres_rap60120->GetErrorY(i));
			  }
			  }
			*/
		  // no : old, v2 : with B, v3 : without B

		  TH2D* hcanvas = new TH2D("hcanvas","",20,0.0,2.0,15,0.5,2.0);
		  setTDRStyle();
		  tdrGrid(true);


		  TCanvas* c1 = new TCanvas("c1","",600,600);
		  gStyle->SetOptStat(0);

		  hcanvas->GetXaxis()->SetTitle("|y_{CM}| (15<M<60 (GeV/c^{2}))");
		  hcanvas->GetYaxis()->SetTitle("Forward/Backward (Xsec)");


		  hcanvas->Draw("");
		  grfbp_rap1560->Draw("same pe");
		  CMS_lumi(c1,4,11);


		  c1->Update();
		  c1->RedrawAxis();
		  c1->GetFrame()->Draw();	
		  c1->SaveAs("grfbp_rap1560_v3.pdf");

		  hcanvas->GetXaxis()->SetTitle("|y_{CM}| (60<M<120 (GeV/c^{2}))");
		  hcanvas->GetYaxis()->SetTitle("Forward/Backward (Xsec)");


		  hcanvas->Draw("");
		  grfbp_rap60120->Draw("same pe");
		  CMS_lumi(c1,4,11);
		  c1->Update();
		  c1->RedrawAxis();
		  c1->GetFrame()->Draw();	
		  c1->SaveAs("grfbp_rap60120_v3.pdf");

		  ///////////////////////////////////////////////////////////////////

		  //		  double fff[fff]={-2.8,-2.4,-2.0,-1.6,-1.2,-0.8,-0.4,0.0,
		  c1->Close();
}

