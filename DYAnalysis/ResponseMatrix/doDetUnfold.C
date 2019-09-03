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
#include <TVector.h>
#include <TDecompSVD.h>
#include <stdio.h>

#include <vector>

//Customized header files
#include "../Include/DYAnalyzer.h"
#include "../Include/UnfoldUtils.h"
#include "../Include/MyCanvas.C"
#include "../Include/PlotTools.h"

using namespace DYana;
using unfold::gUnfold;

void doDetUnfold( Bool_t isCorrected = kFALSE, TString Sample = "Powheg", TString HLTname = "PAL3Mu12", int run=0, var thevar = var::mass ) 
{
	TTimeStamp ts_start;
	cout << "[Start Time(local time): " << ts_start.AsString("l") << "]" << endl;

	TString isApplyMomCorr = "";
	if( isCorrected == kTRUE )
	{
		cout << "Apply Rochester Muon Momentum Correction..." << endl;
		isApplyMomCorr = "MomCorr00";
	}
	else
	{
		cout << "DO *NOT* Apply Rochester Muon Momentum Correction..." << endl;
		isApplyMomCorr = "MomUnCorr";
	}

	TStopwatch totaltime;
	totaltime.Start();

   // define the bins
   const char* thevarname = varname(thevar);
   int nbinsv = nbinsvar(thevar);
   double* binsv = binsvar(thevar);


	TFile *f = new TFile("ResponseMatrix/ROOTFile_ResponseMatrix_" + Sample + "_" + isApplyMomCorr + "_" + Form("%d",run) + ".root");
   // cout << f->GetName() << endl;
   TH2D *h_response = (TH2D*) f->Get(Form("h_%s_response",thevarname));
   TH1D *h_gen = (TH1D*) f->Get(Form("h_%s_gen",thevarname));
   TH1D *h_reco = (TH1D*) f->Get(Form("h_%s_reco",thevarname));

   // compute the condition number
   TMatrixD m(nbinsv,nbinsv);
   for (int i=0; i<nbinsv; i++)
      for (int j=0; j<nbinsv; j++)
         m[i][j] = h_response->GetBinContent(i+1,j+1);
   TDecompSVD tsvd(m);
   tsvd.Decompose();
   TVectorD sv = tsvd.GetSig();
   double condnum = sv[0]/max(0.,sv[nbinsv-1]);
   cout << "Condition number for " << thevarname << ": " << condnum << endl;
   bool doreg = (condnum>1e3);
   if (doreg) cout << "unfolding WITH regularisation" << endl;
   else cout << "unfolding WITHOUT regularisation" << endl;


   // define input and bias scame
   // do not use the bias, because MC peak may be at the wrong place
   // watch out for error codes returned by the SetInput method
   // errors larger or equal 10000 are fatal:
   // the data points specified as input are not sufficient to constrain the
   // unfolding process
   TString fdataname = "ROOTFile_YieldHistogram_" + isApplyMomCorr + "_rewboth_tnprew.root";
   // cout << fdataname << endl;
   TFile *fdata = TFile::Open(fdataname);
   if (!fdata || !fdata->IsOpen()) {
      cout << "Error, file " << fdataname.Data() << " not found" << endl;
      return;
   }
   TH1D *histMdetData = (TH1D*) fdata->Get(Form("h_%s_bkgsub_DataDrivenBkg_All1",thevarname));
   if (!histMdetData) {
      cout << "Error, histo " << Form("h_%s_bkgsub_DataDrivenBkg_All1",thevarname)<< " not found" << endl;
      return;
   }

   // output
   TFile *fout = new TFile(Form("ResponseMatrix/yields_detcor_%s_%s_%d.root",Sample.Data(),isApplyMomCorr.Data(),run),"UPDATE");
   fout->cd();

   // output
   TH1D *h_Measured_TUnfold = (TH1D*) histMdetData->Clone("h_Measured");
   TH1D *histMunfold = NULL;
   TH1D *histMdetFold = NULL;
   TH2D *histEmatTotal = NULL;

   // // scale the response matrix
   // int nn = h_response->GetNbinsX();
   // for (int i=1; i<=nn; i++)
   //    for (int j=1; j<=nn; j++)
   //       if (i!=j) h_response->SetBinContent(i,j,h_response->GetBinContent(i,j)/sqrt(h_response->GetBinContent(i,i)*h_response->GetBinContent(j,j)));
   // for (int i=1; i<=nn; i++) h_response->SetBinContent(i,i,1);

   
   if (doreg) {
   ////////////////////////////////////////////////////////////////////////////////////
   // 
   //   ____                  _            _              _     _____ _   _        __       _     _ 
   //  |  _ \ ___  __ _ _   _| | __ _ _ __(_)___  ___  __| |_  |_   _| | | |_ __  / _| ___ | | __| |
   //  | |_) / _ \/ _` | | | | |/ _` | '__| / __|/ _ \/ _` (_)   | | | | | | '_ \| |_ / _ \| |/ _` |
   //  |  _ <  __/ (_| | |_| | | (_| | |  | \__ \  __/ (_| |_    | | | |_| | | | |  _| (_) | | (_| |
   //  |_| \_\___|\__, |\__,_|_|\__,_|_|  |_|___/\___|\__,_(_)   |_|  \___/|_| |_|_|  \___/|_|\__,_|
   //             |___/                                                                             
   //
   //
   ////////////////////////////////////////////////////////////////////////////////////

   // // first a little tri to please TUnfold's treatment of the underflow and overflow bin
   // int nbinsX = h_response->GetNbinsX();
   // const double *binsX = h_response->GetXaxis()->GetXbins()->GetArray();
   // double *binsXnew = new double[nbinsX+2];
   // for (int i=0; i<nbinsX+1; i++) binsXnew[i] = binsX[i];
   // binsXnew[nbinsX+1] = binsXnew[nbinsX]+1e-10;

   // TH1D *histMdetData_new = new TH1D("histMdetData_new","histMdetData_new",nbinsX+1,binsXnew);
   // TH2D *h_response_new = new TH2D("h_response_new","h_response_new",nbinsX+1,binsXnew,nbinsX,binsX);
   // for (int i=1; i<=nbinsX; i++) {
   //    for (int j=1; j<=nbinsX; j++) {
   //       h_response_new->SetBinContent(i,j,h_response->GetBinContent(i,j));
   //       h_response_new->SetBinError(i,j,h_response->GetBinError(i,j));
   //    }
   //    h_response_new->SetBinContent(nbinsX+1,i,h_response->GetBinContent(0,i));
   //    h_response_new->SetBinError(nbinsX+1,i,h_response->GetBinError(0,i));

   //    histMdetData_new->SetBinContent(i,histMdetData->GetBinContent(i));
   //    histMdetData_new->SetBinError(i,histMdetData->GetBinError(i));
   // }
   // histMdetData_new->SetBinContent(nbinsX+1,histMdetData->GetBinContent(0));
   // histMdetData_new->SetBinError(nbinsX+1,histMdetData->GetBinError(0));

   // init the TUnfold object
   unfold::initTUnfold(h_response,TUnfold::kHistMapOutputVert, doreg ? TUnfold::kRegModeCurvature : TUnfold::kRegModeNone );

   if(gUnfold->SetInput(histMdetData)>=10000) {
      std::cout<<"Unfolding result may be wrong\n";
   }

   //========================================================================
   // the unfolding is done here
   //
   // scan L curve and find best point
   TSpline *logTauX=NULL;
   TGraph *bestLogTauLogChi2=NULL;
   TGraph *bestLcurve=NULL;
   TGraph *lCurve=NULL;

   if (doreg) {
      unfold::doUnfold(logTauX, bestLogTauLogChi2, bestLcurve, lCurve);

      // save the tau vs chi2
      logTauX->Write("logTauX");
      bestLogTauLogChi2->Write("bestLogTauLogChi2");
      // save the L curve
      lCurve->Write("lCurve");
      bestLcurve->Write("bestLcurve");
   } else {
      unfold::doUnfold();
   }

   // for(Int_t i_dr=0; i_dr < ndRCuts; i_dr++)
   // {
   // 	h_nPhotons[i_dr]->Write();
   // 	h_RatioE[i_dr]->Write();
   // 	h_SumE[i_dr]->Write();
   // }

   // gUnfold->Write();

   // -- Response Matrix -- //
   TCanvas *c_RespM = new TCanvas("c_RespM", "", 800, 800);
   c_RespM->cd();
   h_response->SetStats(kFALSE);
   h_response->Draw("COLZ");
   h_response->SetMinimum(1e-3);
   h_response->SetMaximum(1);
   h_response->GetXaxis()->SetTitle(Form("%s (reco level)", xaxistitle(thevar)));
   h_response->GetYaxis()->SetTitle(Form("%s (gen level)", xaxistitle(thevar)));
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
      gPad->SetLogx();
      gPad->SetLogy();
   }
   gPad->SetLogz();
   c_RespM->Write();

   h_Measured_TUnfold = (TH1D*) gUnfold->GetInput("h_Measured_TUnfold");
   h_Measured_TUnfold->SetName("h_Measured_TUnfold");
   // DIRTY FIX
   for (int i=h_Measured_TUnfold->GetNbinsX(); i>0; i--) {
      h_Measured_TUnfold->SetBinContent(i,h_Measured_TUnfold->GetBinContent(i-1));
      h_Measured_TUnfold->SetBinError(i,h_Measured_TUnfold->GetBinError(i-1));
   }
   h_Measured_TUnfold->Write();

   // TH1* h_Truth_TUnfold = gUnfold->GetOutput("h_Truth_TUnfold");
   // h_Truth_TUnfold->SetName("h_Truth_TUnfold");
   // h_Truth_TUnfold->Write();

   //==========================================================================
   // retreive results into histograms

   // get unfolded distribution
   histMunfold = (TH1D*) gUnfold->GetOutput("Unfolded");
   histMunfold->Write();

   // get unfolding result, folded back
   histMdetFold = (TH1D*) gUnfold->GetFoldedOutput("FoldedBack");
   histMdetFold->Write();

   // get error matrix (input distribution [stat] errors only)
   // TH2D *histEmatData=gUnfold->GetEmatrix("EmatData");

   // get total error matrix:
   //   migration matrix uncorrelated and correlated systematic errors
   //   added in quadrature to the data statistical errors
   histEmatTotal = (TH2D*) gUnfold->GetEmatrixTotal("EmatTotal");

   // create data histogram with the total errors
   TH1D *histTotalError= new TH1D("TotalError",Form(";%s(gen)",xaxistitle(thevar)),nbinsv, binsv);
   for(Int_t bin=1;bin<=nbinsv;bin++) {
      histTotalError->SetBinContent(bin,histMunfold->GetBinContent(bin));
      histTotalError->SetBinError
         (bin,TMath::Sqrt(histEmatTotal->GetBinContent(bin,bin)));
   }
   histEmatTotal->Write();
   histTotalError->Write();

   // get global correlation coefficients
   // for this calculation one has to specify whether the
   // underflow/overflow bins are included or not
   // default: include all bins
   // here: exclude underflow and overflow bins
   TH2 *gHistInvEMatrix;
   TH1D *histRhoi = (TH1D*) gUnfold->GetRhoItotal("rho_I",
         0, // use default title
         0, // all distributions
         "*[UO]", // discard underflow and overflow bins on all axes
         kTRUE, // use original binning
         &gHistInvEMatrix // store inverse of error matrix
         );
   histRhoi->Write();


   // let's make some canvas with unfolding plots as in the ROOT tuto

   TCanvas output;
   output.Divide(3,2);

   // Show the matrix which connects input and output
   // There are overflow bins at the bottom, not shown in the plot
   // These contain the background shape.
   // The overflow bins to the left and right contain
   // events which are not reconstructed. These are necessary for proper MC
   // normalisation
   output.cd(1);
   h_response->Draw("BOX");

   // draw generator-level distribution:
   //   data (red) [for real data this is not available]
   //   MC input (black) [with completely wrong peak position and shape]
   //   unfolded data (blue)
   output.cd(2);
   // divide by bin withd to get density distributions
   TH1D *histDensityGenData=new TH1D("DensityGenData",Form(";%s (gen)", xaxistitle(thevar)),
         nbinsv,binsv);
   TH1D *histDensityGenMC=new TH1D("DensityGenMC",Form("%s; (gen)", xaxistitle(thevar)),
         nbinsv,binsv);
   for(Int_t i=1;i<=nbinsv;i++) {
      histDensityGenData->SetBinContent(i,h_reco->GetBinContent(i)/
            h_reco->GetBinWidth(i));
      histDensityGenMC->SetBinContent(i,h_gen->GetBinContent(i)/
            h_gen->GetBinWidth(i));
   }
   histTotalError->SetLineColor(kBlue);
   histTotalError->Draw("E");
   histMunfold->SetLineColor(kGreen);
   histMunfold->Draw("SAME E1");
   histDensityGenData->SetLineColor(kRed);
   histDensityGenData->Draw("SAME");
   histDensityGenMC->Draw("SAME HIST");

   // show detector level distributions
   //    data (red)
   //    MC (black) [with completely wrong peak position and shape]
   //    unfolded data (blue)
   output.cd(3);
   histMdetFold->SetLineColor(kBlue);
   histMdetFold->Draw();
   h_reco->Draw("SAME HIST");

   TH1D *histInput = (TH1D*) gUnfold->GetInput("Minput",Form(";%s (det)",xaxistitle(thevar)));

   histInput->SetLineColor(kRed);
   histInput->Draw("SAME");

   // show correlation coefficients
   output.cd(4);
   histRhoi->Draw();

   if (doreg) {
      // show tau as a function of chi**2
      output.cd(5);
      logTauX->Draw();
      bestLogTauLogChi2->SetMarkerColor(kRed);
      bestLogTauLogChi2->Draw("*");

      // show the L curve
      output.cd(6);
      lCurve->Draw("AL");
      bestLcurve->SetMarkerColor(kRed);
      bestLcurve->Draw("*");
   }

   output.Write();
   } else {
      ////////////////////////////////////////////////////////////////////////////////////
      //   _   _                            _            _              _     __  __ _     _____ 
      //  | | | |_ __  _ __ ___  __ _ _   _| | __ _ _ __(_)___  ___  __| |_  |  \/  | |   | ____|
      //  | | | | '_ \| '__/ _ \/ _` | | | | |/ _` | '__| / __|/ _ \/ _` (_) | |\/| | |   |  _|  
      //  | |_| | | | | | |  __/ (_| | |_| | | (_| | |  | \__ \  __/ (_| |_  | |  | | |___| |___ 
      //   \___/|_| |_|_|  \___|\__, |\__,_|_|\__,_|_|  |_|___/\___|\__,_(_) |_|  |_|_____|_____|
      //                        |___/                                                            
      //
      ////////////////////////////////////////////////////////////////////////////////////

      histMunfold = unfold::unfold_MLE(h_Measured_TUnfold,h_response,histEmatTotal);
      histMdetFold = unfold::fold_MLE(histMunfold,h_response);

      histMunfold->Write(Form("%s_%s",histMunfold->GetName(),thevarname));
      histMunfold->Write(Form("h_%s_bkgsub_DataDrivenBkg_All1",thevarname));
      histMdetFold->Write(Form("%s_%s",histMdetFold->GetName(),thevarname));
      h_response->Write(Form("%s_%s",h_response->GetName(),thevarname));
      histEmatTotal->Write(Form("%s_%s",histEmatTotal->GetName(),thevarname));
   }

   // compare data before and after unfolding
   MyCanvas c1D(Form("ResponseMatrix/c_unfoldData_%s_beforeafter",thevarname),xaxistitle(thevar),"Entries",800,800);
   MyCanvas c_cov(Form("ResponseMatrix/c_unfoldData_%s_totCovMat",thevarname),xaxistitle(thevar),"Entries",800,800);
   MyCanvas c_cor(Form("ResponseMatrix/c_unfoldData_%s_totCorMat",thevarname),xaxistitle(thevar),"Entries",800,800);
   if (thevar==var::mass || thevar==var::pt || thevar==var::phistar || thevar==var::pt1560 || thevar==var::phistar1560) {
      c1D.SetLogx();
      c_cov.SetLogx();
      c_cov.SetLogy();
      c_cor.SetLogx();
      c_cor.SetLogy();
      fixXaxis(histMunfold);
      fixXaxis(h_Measured_TUnfold);
   }
   if (thevar==var::mass) c1D.SetRatioRange(0.7,1.3);
   else if (thevar==var::pt) c1D.SetRatioRange(0.8,1.2);
   else c1D.SetRatioRange(0.9,1.1);

   c1D.CanvasWithHistogramsRatioPlot(histMunfold,h_Measured_TUnfold,"Unfolded","Measured","Unfolded/Measured",kBlack,kRed,false,false,"EP","EPSAME");
   CMS_lumi(c1D.c, 111, 0);
   c1D.PrintCanvas();

   // better for the correlation matrix
   const Int_t Number = 3;
   Double_t Red[Number]    = { 0.00, 1.00, 1.00};
   Double_t Green[Number]  = { 0.00, 1.00, 0.00};
   Double_t Blue[Number]   = { 1.00, 1.00, 0.00};
   Double_t Length[Number] = { 0.00, 0.50, 1.00 };
   Int_t nb=50;
   TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

   c_cov.CanvasWith2DHistogram(histEmatTotal,"COLZ");
   CMS_lumi(c_cov.c, 111, 0);
   c_cov.PrintCanvas();

   // normalise the covariance matrix
   int n = histEmatTotal->GetNbinsX();
   for (int i=1; i<=n; i++)
      for (int j=1; j<=n; j++)
         if (i!=j) histEmatTotal->SetBinContent(i,j,histEmatTotal->GetBinContent(i,j)/sqrt(histEmatTotal->GetBinContent(i,i)*histEmatTotal->GetBinContent(j,j)));
   for (int i=1; i<=n; i++) histEmatTotal->SetBinContent(i,i,1);
   c_cor.SetZRange(-1,1);
   c_cor.CanvasWith2DHistogram(histEmatTotal,"COLZ");
   CMS_lumi(c_cor.c, 111, 0);
   c_cor.PrintCanvas();


   Double_t TotalRunTime = totaltime.CpuTime();
   cout << "Total RunTime: " << TotalRunTime << " seconds" << endl;

   TTimeStamp ts_end;
   cout << "[End Time(local time): " << ts_end.AsString("l") << "]" << endl;

   fdata->Close();
   fout->Close();
   f->Close();
}

// do all variations at once
void doDetUnfold_all() {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      doDetUnfold(kTRUE, "Powheg", "PAL3Mu12", 0, thevar_i);
      doDetUnfold(kFALSE, "Powheg", "PAL3Mu12", 0, thevar_i);
      doDetUnfold(kTRUE, "Pyquen", "PAL3Mu12", 0, thevar_i);
      doDetUnfold(kTRUE, "Powheg", "PAL3Mu12", 1, thevar_i);
      doDetUnfold(kTRUE, "Powheg", "PAL3Mu12", 2, thevar_i);
   }
}
