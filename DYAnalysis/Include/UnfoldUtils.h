#ifndef UnfoldUtils_h
#define UnfoldUtils_h

//TUnfold
#include "TUnfoldDensity.h"
// #define VERBOSE_LCURVE_SCAN

#include <TVector.h>
#include <TMatrix.h>

namespace unfold {
   // global TUnfold object
   TUnfoldDensity *gUnfold=NULL;
   TUnfold::ERegMode gRegMode;

   TUnfoldDensity* initTUnfold(TH2D *resp_matrix, 
         TUnfold::EHistMap themap=TUnfold::kHistMapOutputVert, 
         TUnfold::ERegMode regmode=TUnfold::kRegModeCurvature,
         TUnfold::EConstraint densmode=TUnfold::kEConstraintNone) {
      gRegMode = regmode;
      gUnfold = new TUnfoldDensity(resp_matrix,themap,regmode,densmode);
      return gUnfold;
   }

   void doUnfold(TSpline *&logTauX, TGraph *&bestLcurve, TGraph *&bestLogTauLogChi2, TGraph *&lCurve) {
      //========================================================================
      // the unfolding is done here
      //
      // scan L curve and find best point
      Int_t nScan=30;
      // use automatic L-curve scan: start with taumin=taumax=0.0
      Double_t tauMin=0.;
      Double_t tauMax=0.;
      Int_t iBest;
      TSpline *logTauY;

      // if necessary, optimise the regularisation
      if (gRegMode != TUnfold::kRegModeNone) {
         // if required, report Info messages (for debugging the L-curve scan)
#ifdef VERBOSE_LCURVE_SCAN
         Int_t oldinfo=gErrorIgnoreLevel;
         gErrorIgnoreLevel=kInfo;
#endif
         // this method scans the parameter tau and finds the kink in the L curve
         // finally, the unfolding is done for the best choice of tau
         iBest=gUnfold->ScanLcurve(nScan,tauMin,tauMax,&lCurve,&logTauX,&logTauY);

         // if required, switch to previous log-level
#ifdef VERBOSE_LCURVE_SCAN
         gErrorIgnoreLevel=oldinfo;
#endif

         //==========================================================================
         // create graphs with one point to visualize the best choice of tau
         //
         Double_t t[1],x[1],y[1];
         logTauX->GetKnot(iBest,t[0],x[0]);
         logTauY->GetKnot(iBest,t[0],y[0]);
         bestLcurve = new TGraph(1,x,y);
         bestLogTauLogChi2 = new TGraph(1,t,x);
      }
   }

   void doUnfold() {
      if (gRegMode==TUnfold::kRegModeNone) gUnfold->DoUnfold(0);
      else {
         TSpline *logTauX=NULL;
         TGraph *bestLogTauLogChi2=NULL;
         TGraph *bestLcurve=NULL;
         TGraph *lCurve=NULL;
         doUnfold(logTauX, bestLcurve, bestLogTauLogChi2, lCurve);
      }
   }

   /////////////////////////////////////////
   //                                     //
   //             MLE unfolding           //
   //                                     //
   /////////////////////////////////////////

   TH1D* unfold_MLE(TH1D *hin, TH2D *hresponse, TH2D *&hcov) {
      // cout << hin->GetNbinsX() << endl;
      // cout << hresponse->GetNbinsX() << " " << hresponse->GetNbinsY() << endl;
      // input values
      TVectorD vin(hin->GetNbinsX());
      // input covariance
      TMatrixD mcov_in(hin->GetNbinsX(),hin->GetNbinsX());
      for (int i=1; i<=hin->GetNbinsX(); i++) {
         vin[i-1] = hin->GetBinContent(i);
         mcov_in(i-1,i-1) = pow(hin->GetBinError(i),2);
      }

      TVectorD vout(hresponse->GetNbinsY());

      TMatrixD mr(hresponse->GetNbinsX(),hresponse->GetNbinsY());
      for (int i=1; i<=hresponse->GetNbinsX(); i++) {
         for (int j=1; j<=hresponse->GetNbinsY(); j++) {
            mr(i-1,j-1) = hresponse->GetBinContent(i,j);
            // cout << mr(i-1,j-1) << " ";
         }
         // cout << endl;
      }

      // helper matrix for the covariance
      TMatrixD cmat(hresponse->GetNbinsY(),hresponse->GetNbinsY());
      // cmat = (TMatrixD(TMatrixD::kTransposed,mr) * TMatrixD(TMatrixD::kTransposed,mcov_in) * mr).Invert();

      // invert the response matrix
      mr.Invert();
      for (int i=1; i<=hresponse->GetNbinsX(); i++) {
         for (int j=1; j<=hresponse->GetNbinsY(); j++) {
            // cout << mr(i-1,j-1) << " ";
         }
         // cout << endl;
      }

      // compute the unfolded result
      vout = mr * vin;
      cmat = mr * mcov_in;

      // create the output histos: distribution and covariance matrix
      TH1D *hout = new TH1D(Form("%s_unfoldedMLE",hin->GetName()),"Unfolded (MLE)",
            hresponse->GetNbinsY(),hresponse->GetYaxis()->GetXbins()->GetArray());
      hcov = new TH2D(Form("%s_unfoldedMLE_cov",hin->GetName()),"Covariance",
            hresponse->GetNbinsY(),hresponse->GetYaxis()->GetXbins()->GetArray(),
            hresponse->GetNbinsY(),hresponse->GetYaxis()->GetXbins()->GetArray());
      for (int i=1; i<=hout->GetNbinsX(); i++) {
         for (int j=1; j<=hout->GetNbinsX(); j++) {
            hcov->SetBinContent(i,j,cmat(i-1,j-1));
         }
         hout->SetBinContent(i,vout[i-1]);
         hout->SetBinError(i,sqrt(hcov->GetBinContent(i,i)));
      }

      // for (int i=0; i<hresponse->GetNbinsY(); i++) cout << vin[i] << " " << vout[i] << endl;
      // for (int i=0; i<hresponse->GetNbinsY(); i++) cout << hin->GetBinError(i+1) << " " << hout->GetBinError(i+1) << endl;

      return hout;
   }

   TH1D* fold_MLE(TH1D *hin, TH2D *hresponse) {
      // cout << hin->GetNbinsX() << endl;
      // cout << hresponse->GetNbinsX() << " " << hresponse->GetNbinsY() << endl;
      // input values
      TVectorD vin(hin->GetNbinsX());
      // input covariance
      TMatrixD mcov_in(hin->GetNbinsX(),hin->GetNbinsX());
      for (int i=1; i<=hin->GetNbinsX(); i++) {
         vin[i-1] = hin->GetBinContent(i);
         mcov_in(i-1,i-1) = pow(hin->GetBinError(i),2);
      }

      TVectorD vout(hresponse->GetNbinsX());

      TMatrixD mr(hresponse->GetNbinsX(),hresponse->GetNbinsY());
      for (int i=1; i<=hresponse->GetNbinsX(); i++)
         for (int j=1; j<=hresponse->GetNbinsY(); j++)
            mr(j-1,i-1) = hresponse->GetBinContent(i,j);

      // helper matrix for the covariance
      TMatrixD cmat(hresponse->GetNbinsX(),hresponse->GetNbinsX());
      cmat = (TMatrixD(TMatrixD::kTransposed,mr) * TMatrixD(TMatrixD::kTransposed,mcov_in) * mr).Invert();

      // compute the unfolded result
      vout = mr * vin;

      // create the output histos: distribution and covariance matrix
      TH1D *hout = new TH1D(Form("%s_foldedMLE",hin->GetName()),"Folded (MLE)",
            hresponse->GetNbinsX(),hresponse->GetXaxis()->GetXbins()->GetArray());
      TH2D *hcov = new TH2D(Form("%s_foldedMLE_cov",hin->GetName()),"Covariance",
            hresponse->GetNbinsX(),hresponse->GetXaxis()->GetXbins()->GetArray(),
            hresponse->GetNbinsX(),hresponse->GetXaxis()->GetXbins()->GetArray());
      for (int i=1; i<=hout->GetNbinsX(); i++) {
         for (int j=1; j<=hout->GetNbinsX(); j++) {
            hcov->SetBinContent(i,j,cmat(i-1,j-1));
         }
         hout->SetBinContent(i,vout[i-1]);
         hout->SetBinError(i,sqrt(hcov->GetBinContent(i,i)));
      }

      return hout;
   }

}

#endif // #ifndef UnfoldUtils_h
