#ifndef UnfoldUtils_h
#define UnfoldUtils_h

//TUnfold
#include "TUnfoldDensity.h"
// #define VERBOSE_LCURVE_SCAN

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
}

#endif // #ifndef UnfoldUtils_h
