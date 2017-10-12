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
#include <TEfficiency.h>
#include <stdio.h>
// #include <TGraphAsymmErrors.h>
#include <vector>
#include <TROOT.h>
#include <TLatex.h>

#include "Include/MyCanvas.C"
#include "Include/UnfoldUtils.h"

using unfold::gUnfold;

void CalculateFractionPerBin(TH2D *h_nEvents, TH1* h_Truth, TH2D *h_Response);
TH2D* Transpose( TH2D* h_2D );
void MakeCanvas_RespM( TH2D *h_RespM, TString var, int run );

void DrawClosureTest_FSR(TString var="mass", // what variable? mass, pt, rap1560, rap60120
      TString sample="Original", // which distribution to unfold? Powheg_0, Powheg_1, Powheg_2, Pyquen_0, Original (=same as used for response matrix)
      int run=0) // 0=all, 1=pPb, 2=PbP
{
	gROOT->SetBatch( kTRUE );

	TString FileLocation = "./";
	TFile *f_respM = new TFile(FileLocation + Form("/ROOTFile_FSRCorrections_DressedLepton_Powheg_%d.root",run));
	TFile *f_dist;
   if (sample=="Original") f_dist = f_respM;
   else if (sample!="Data") f_dist = new TFile(FileLocation + Form("/ROOTFile_FSRCorrections_DressedLepton_%s.root",sample.Data()));
   else f_dist = new TFile(FileLocation + "ROOTFile_YieldHistogram.root");

   TString label;
   if (var=="mass") label = "Mass [GeV]";
   else if (var=="pt") label = "p_{T} [GeV]";
   else if (var.Contains("rap")) label = "y";
   TString label2 = (sample=="Data") ? "Reconstructed " : "Gen-level ";

	///////////////////////////
	// -- Response Matrix -- //
	///////////////////////////
	TH1D *h_Truth_TUnfold = (TH1D*)f_respM->Get("h_" + var + "_preFSR");
	TH2D* h_nEvents = (TH2D*)f_respM->Get("h_" + var + "_postpreFSR_tot");
	h_nEvents->SetName("h_nEvents");
	TH2D* h_RespM = (TH2D*)h_nEvents->Clone("h_RespM");
	CalculateFractionPerBin(h_nEvents, h_Truth_TUnfold, h_RespM);
	MakeCanvas_RespM( h_RespM, var, run );

	////////////////////////
	// -- Closure Test -- //
	////////////////////////
   // init the TUnfold object
   // NB: no regularisation except for mass
   unfold::initTUnfold(h_nEvents,TUnfold::kHistMapOutputVert, var=="mass" ? TUnfold::kRegModeCurvature : TUnfold::kRegModeNone); 

   // get the histos
   TH1D *h_truth = (TH1D*) f_dist->Get("h_" + var + "_preFSR");
   TH1D *h_postFSR = (TH1D*) f_dist->Get("h_" + var + "_postFSR");
   if (sample=="Data") {
      h_postFSR = (TH1D*) f_dist->Get("h_yield_OS_MCBasedBkg1");
   }
   if(gUnfold->SetInput(h_postFSR)>=10000) {
      std::cout<<"Unfolding result may be wrong\n";
   }

   // do the unfolding
   unfold::doUnfold();

   // get the unfolded distribution, with regularisation
   TH1D* h_unfolded = (TH1D*) gUnfold->GetOutput("h_unfolded");
   // also without regularisation
   gUnfold->DoUnfold(0);
   TH1D* h_unfolded_mle = (TH1D*) gUnfold->GetOutput("h_unfolded_mle");

	MyCanvas *myc = new MyCanvas(Form("c_ClosureTest_%s_%s_%d",var.Data(),sample.Data(),run), label2+label, "Number of events");
	if (!var.Contains("rap")) myc->SetLogx();
	myc->SetLogy(0);
	if (var=="mass") myc->SetRatioRange(0.7, 1.3);
   else myc->SetRatioRange(0.91, 1.09);
	if (sample!="Data")
      myc->CanvasWithThreeHistogramsRatioPlot( h_postFSR, h_unfolded, h_truth, 
											 "post-FSR", "Unfolded", "pre-FSR", "Ratio to pre-FSR",
											 kBlue, kGreen+1, kRed);
   else
      myc->CanvasWithHistogramsRatioPlot(h_postFSR, h_unfolded, 
            "Data post FSR", "Data unfolded", "Ratio to pre-FSR",
            kBlue, kGreen+1);
	myc->PrintCanvas();

	MyCanvas *myc_mle = new MyCanvas(Form("c_ClosureTest_mle_%s_%s_%d",var.Data(),sample.Data(),run), label2+label, "Number of events");
	if (!var.Contains("rap")) myc_mle->SetLogx();
	myc_mle->SetLogy(0);
	if (var=="mass") myc_mle->SetRatioRange(0.7, 1.3);
   else myc_mle->SetRatioRange(0.91, 1.09);
   if (sample != "Data") 
      myc_mle->CanvasWithThreeHistogramsRatioPlot( h_postFSR, h_unfolded_mle, h_truth, 
											 "post-FSR", "Unfolded (MLE)", "pre-FSR", "Ratio to pre-FSR",
											 kBlue, kGreen+1, kRed);
   else
      myc_mle->CanvasWithHistogramsRatioPlot(h_postFSR, h_unfolded_mle, 
            "Data post FSR", "Data unfolded (MLE)", "Ratio to pre-FSR",
            kBlue, kGreen+1);
	myc_mle->PrintCanvas();

   // -- Check the detail number -- //
   if (sample!="Data") {
      TH1D *h_ratio = (TH1D*)(myc->h_ratio2)->Clone();
      Int_t nBin = h_ratio->GetNbinsX();
      Bool_t isPassTest = kTRUE;
      for(Int_t i=0; i<nBin; i++)
      {
         Int_t i_bin = i+1;
         Double_t ratio = h_ratio->GetBinContent(i_bin);
         if( (ratio-1) > h_ratio->GetBinError(i_bin) )
         {
            Double_t lowerEdge = h_ratio->GetBinLowEdge(i_bin);
            Double_t upperEdge = h_ratio->GetBinLowEdge(i_bin+1);
            printf("[%2dth bin] (lowerEdge, upperEdge) = (%.3lf, %.3lf) (Unfolded/Truth) = %.6lf\n", i_bin, lowerEdge, upperEdge, ratio);
            isPassTest = kFALSE;
         }
      }
      cout << "==============================================" << endl;
      if( isPassTest ) 
         cout << "[Pass the closure test]" << endl;
      else
         cout << "[Closure test is FAILED! ... Check the details!]" << endl;
      cout << "==============================================" << endl;
   }

}

void CalculateFractionPerBin(TH2D *h_nEvents, TH1* h_Truth, TH2D *h_Response)
{
	Int_t nBins = h_Truth->GetNbinsX();

	for(Int_t i_genbin=0; i_genbin <= nBins+1; i_genbin++) // -- Include under/overflow -- //
	{
		Double_t SumN_truth = h_Truth->GetBinContent(i_genbin);

		for(Int_t i_recobin=0; i_recobin <= nBins+1; i_recobin++) // -- Include under/overflow -- //
		{
			Double_t nEvent = h_nEvents->GetBinContent(i_recobin, i_genbin);

			Double_t fraction = 0;
			if( SumN_truth == 0 )
				fraction = 0;
			else
				fraction = nEvent / SumN_truth;
			
			if( fraction < 0 && fabs(fraction) < 1e-3 ) fraction = 0;

			h_Response->SetBinContent( i_recobin, i_genbin, fraction );
		}
	}
}

void MakeCanvas_RespM( TH2D *h_RespM, TString var, int run )
{
	setTDRStyle();
	gROOT->SetStyle( "tdrStyle" );

	TH2D *hT_RespM = Transpose( h_RespM );

	TCanvas *c = new TCanvas(Form("c_RespM_FSR_%s_%d",var.Data(),run), "", 800, 800);
	gPad->SetGridx();
	gPad->SetGridy();
   if (!var.Contains("rap")) {
      gPad->SetLogx();
      gPad->SetLogy();
   }
	gPad->SetLogz();

   // gPad->SetLeftMargin(0.14);
   gPad->SetRightMargin(0.14);
   // gPad->SetTopMargin(0.05);
   // gPad->SetBottomMargin(0.1);

	hT_RespM->Draw("COLZ");

	hT_RespM->GetXaxis()->SetTitle("generated mass (pre-FSR) [GeV]");
	hT_RespM->GetXaxis()->SetNoExponent();
	hT_RespM->GetXaxis()->SetMoreLogLabels();
	hT_RespM->GetYaxis()->SetTitle("generated mass (post-FSR) [GeV]");
	hT_RespM->GetYaxis()->SetNoExponent();
	hT_RespM->GetYaxis()->SetMoreLogLabels();
   // hT_RespM->GetYaxis()->SetTitleOffset(2);

   extraText = "Simulation";
   writeExtraText = true;
   CMS_lumi( c, 111 );

	c->SaveAs(".pdf");
}

TH2D* Transpose( TH2D* h_2D )
{
	TH2D* h_2D_Transposed = (TH2D*)h_2D->Clone();
	h_2D_Transposed->SetDirectory(0);

	Int_t nBin_X = h_2D->GetNbinsX();
	Int_t nBin_Y = h_2D->GetNbinsY();

	if(nBin_X != nBin_Y )
	{
		printf("(nBin_X, nBin_Y) = (%d, %d): this is not squared matrix! ... transpose for this matrix is not provided by this function\n", nBin_X, nBin_Y);
		return NULL;
	}

	for(Int_t iter_x=0; iter_x<nBin_X; iter_x++)
	{
		Int_t i_binX = iter_x+1;
		for(Int_t iter_y=0; iter_y<nBin_Y; iter_y++)
		{
			Int_t i_binY = iter_y+1;

			Double_t value_before = h_2D->GetBinContent(i_binX, i_binY);

			h_2D_Transposed->SetBinContent(i_binY, i_binX, value_before);
		}
	}

	return h_2D_Transposed;
}



