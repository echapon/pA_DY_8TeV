#include "../../BkgEst/interface/defs.h"
#include "../../Include/MyCanvas.C"
#include "../../Include/texUtils.h"
#include "../syst.C"

#include "TFile.h"
#include "TH1.h"
#include "TMatrixT.h"
#include "TEfficiency.h"

#include <vector>
#include <iostream>

using namespace std;
using DYana::var;

// HOW TO USE THIS FILE
// [0] .include /dir/to/lhapdf/include
// [1] gSystem->Load("/dir/to/lhapdf/lib/libLHAPDF.so");
// .L AccEff/Sys_AccEff.C+

void Sys_TnP(const char* file, var thevar, bool absxsec=true) {// if absxsec=false, do effs for 1/N dN/dX instead of dN/dX
   TFile *f = TFile::Open(file);
   if (!f || !f->IsOpen()) return;

   bool isAllMass = isAllMassVar(thevar);

   TEfficiency **ee = new TEfficiency*[645];
   TEfficiency **ee2 = new TEfficiency*[645];

   TString thevarname, theVarname, thevarname2, theVarname2;
   if (!isAllMass) {
      thevarname = varname(thevar);
      theVarname = Varname(thevar);
   } else {
      thevarname = varname1560(thevar);
      theVarname = Varname1560(thevar);
      thevarname2 = varname60120(thevar);
      theVarname2 = Varname60120(thevar);
   }

   if (absxsec) {
      for (int i=0; i<645; i++) {
         ee[i] = (TEfficiency*) f->Get(Form("TEff_Eff_%s_Corr_tnp%d",theVarname.Data(),i)); 
         if (isAllMass) ee2[i] = (TEfficiency*) f->Get(Form("TEff_Eff_%s_Corr_tnp%d",theVarname2.Data(),i)); 
      }
   } else {
      TH1D *hpass, *hpass2, *htot, *htot2;
      htot = (TH1D*) f->Get(Form("h_%s_EffTotal",thevarname.Data()));
      if (isAllMass) htot2 = (TH1D*) f->Get(Form("h_%s_EffTotal",thevarname2.Data()));
      // htot->Scale(1./htot->Integral());

      for (int i=0; i<645; i++) {
         // cout << i << endl;
         hpass = (TH1D*) f->Get(Form("h_%s_EffPass_Corr_tnp%d",thevarname.Data(),i));
         hpass->Scale(1./hpass->Integral());
         ee[i] = new TEfficiency(*hpass,*htot);
         if (isAllMass) {
            hpass2 = (TH1D*) f->Get(Form("h_%s_EffPass_Corr_tnp%d",thevarname2.Data(),i));
            hpass2->Scale(1./hpass2->Integral());
            ee2[i] = new TEfficiency(*hpass2,*htot2);
         }
      }
   }


   int nbins = nbinsvar(thevar);

   vector<TMatrixT<double> > mcov_trg, mcov_muid, mcov_iso, mcov_syst;

   // initialise the vectors of covariance matrices
   for (int i=0; i<14; i++) mcov_trg.push_back(TMatrixT<double>(nbins,nbins));
   for (int i=0; i<3; i++) mcov_muid.push_back(TMatrixT<double>(nbins,nbins));
   for (int i=0; i<3; i++) mcov_iso.push_back(TMatrixT<double>(nbins,nbins));
   for (int i=0; i<14; i++) mcov_syst.push_back(TMatrixT<double>(nbins,nbins));

   for (int ibin=0; ibin<nbins; ibin++) {
      // compute the tnp syst uncertainty
      double ei[645];
      for (int isyst=0; isyst<645; isyst++) {
         if (!isAllMass) ei[isyst] = ee[isyst]->GetEfficiency(ibin+1);
         else {
            int ibin1 = DYana::idx1560(thevar,ibin);
            int ibin2 = DYana::idx60120(thevar,ibin);
            if (ibin1>=0) ei[isyst] = ee[isyst]->GetEfficiency(ibin1+1);
            else ei[isyst] = ee2[isyst]->GetEfficiency(ibin2+1);
         }
      }

      for (int jbin=0; jbin<nbins; jbin++) {
         double ej[645];
         for (int isyst=0; isyst<645; isyst++) {
            if (!isAllMass) ej[isyst] = ee[isyst]->GetEfficiency(jbin+1);
            else {
               int jbin1 = DYana::idx1560(thevar,jbin);
               int jbin2 = DYana::idx60120(thevar,jbin);
               if (jbin1>=0) ej[isyst] = ee[isyst]->GetEfficiency(jbin1+1);
               else ej[isyst] = ee2[isyst]->GetEfficiency(jbin2+1);
            }
         }

         // stat uncertainties
         // trigger
         for (int i=1; i<=14; i++) {
            mcov_trg[i-1][ibin][jbin] = max(fabs(ei[600+i]-ei[0]),fabs(ei[614+i]-ei[0]))
               * max(fabs(ej[600+i]-ej[0]),fabs(ej[614+i]-ej[0]));
         }

         // muID
         // there is a super weird bug... dirty-fix it
         for (int ix=1; ix<=300; ix++) {
            double eixi = ei[ix];
            if (eixi<1e-9) eixi = ei[ix-1];
            double eixj = ej[ix];
            if (eixj<1e-9) eixj = ej[ix-1];
            mcov_muid[(ix-1)/100][ibin][jbin] += (eixi-ei[0]) * (eixj-ej[0]) / 99.;
         }
         
         // iso
         for (int ix=301; ix<=600; ix++) mcov_iso[(ix-301)/100][ibin][jbin] += (ei[ix]-ei[0])
            * (ej[ix]-ej[0]) / 99.;


         // syst uncertainties: no bin-to-bin correlations
         if (ibin==jbin) {
            // trigger
            mcov_syst[0][ibin][jbin] = max(fabs(ei[629]-ei[0]),fabs(ei[630]-ei[0]))
               * max(fabs(ej[629]-ej[0]),fabs(ej[630]-ej[0]));
            mcov_syst[1][ibin][jbin] = max(fabs(ei[631]-ei[0]),fabs(ei[632]-ei[0]))
               * max(fabs(ej[631]-ej[0]),fabs(ej[632]-ej[0]));

            // muID
            mcov_syst[2][ibin][jbin] = max(fabs(ei[633]-ei[0]),fabs(ei[634]-ei[0]))
               * max(fabs(ej[633]-ej[0]),fabs(ej[634]-ej[0]));
            mcov_syst[3][ibin][jbin] = max(fabs(ei[635]-ei[0]),fabs(ei[636]-ei[0]))
               * max(fabs(ej[635]-ej[0]),fabs(ej[636]-ej[0]));

            // iso
            mcov_syst[4][ibin][jbin] = max(fabs(ei[637]-ei[0]),fabs(ei[638]-ei[0]))
               * max(fabs(ej[637]-ej[0]),fabs(ej[638]-ej[0]));
            mcov_syst[5][ibin][jbin] = max(fabs(ei[639]-ei[0]),fabs(ei[640]-ei[0]))
               * max(fabs(ej[639]-ej[0]),fabs(ej[640]-ej[0]));

            // muID binned
            mcov_syst[6][ibin][jbin] = fabs(ei[641]-ei[0]) * fabs(ej[641]-ej[0]);
            mcov_syst[7][ibin][jbin] = fabs(ei[642]-ei[0]) * fabs(ej[642]-ej[0]);

            // iso binned
            mcov_syst[8][ibin][jbin] = fabs(ei[643]-ei[0]) * fabs(ej[643]-ej[0]);
            mcov_syst[9][ibin][jbin] = fabs(ei[644]-ei[0]) * fabs(ej[644]-ej[0]);

            // HF+PU
            mcov_syst[10][ibin][jbin] = absxsec ? pow(0.34e-2,2) : 0;
            mcov_syst[11][ibin][jbin] = absxsec ? pow(0.34e-2,2) : 0;

            // STA
            mcov_syst[12][ibin][jbin] = absxsec ? pow(0.6e-2,2) : 0;
            mcov_syst[13][ibin][jbin] = absxsec ? pow(0.6e-2,2) : 0;
         } // if ibin==jbin
      } // jbin loop for tnp uncertainty
   } // ibin loop for tnp uncertainty

   // now let's all combine all these individual covariance matrices into the total one
   TMatrixT<double> mcov_tot(nbins,nbins);
   for (unsigned int i=0; i<mcov_trg.size(); i++) mcov_tot += mcov_trg[i];
   for (unsigned int i=0; i<mcov_muid.size(); i++) mcov_tot += mcov_muid[i];
   for (unsigned int i=0; i<mcov_iso.size(); i++) mcov_tot += mcov_iso[i];
   for (unsigned int i=0; i<mcov_syst.size(); i++) mcov_tot += mcov_syst[i];

   // now we turn this into a correlation matrix
   TMatrixT<double> mcor_tot = mcov_tot;
   for (int j=0; j<nbins; j++) {
      for (int k=0; k<nbins; k++) {
         if (j==k) continue;
         mcor_tot[j][k] *= 1./sqrt(fabs(mcor_tot[j][j]*mcor_tot[k][k]));
      }
   }
   for (int j=0; j<nbins; j++) mcor_tot[j][j] = 1;

   for (int i=0; i<nbins; i++) {
      for (int j=0; j<nbins; j++) {
         cout << mcor_tot[i][j] << " ";
      }
      cout << endl;
   }

   // print the csv (also for the individual systs)... and the correlation matrix
   ofstream of_syst(Form("csv/tnp_tot_%s.csv",varname(thevar)));
   ofstream of_isobinned(Form("csv/tnp_isobinned_%s.csv",varname(thevar)));
   ofstream of_isostat(Form("csv/tnp_isostat_%s.csv",varname(thevar)));
   ofstream of_isosyst(Form("csv/tnp_isosyst_%s.csv",varname(thevar)));
   ofstream of_muidbinned(Form("csv/tnp_muidbinned_%s.csv",varname(thevar)));
   ofstream of_muidstat(Form("csv/tnp_muidstat_%s.csv",varname(thevar)));
   ofstream of_muidsyst(Form("csv/tnp_muidsyst_%s.csv",varname(thevar)));
   ofstream of_trigstat(Form("csv/tnp_trigstat_%s.csv",varname(thevar)));
   ofstream of_trigsyst(Form("csv/tnp_trigsyst_%s.csv",varname(thevar)));
   ofstream of_STA(Form("csv/tnp_STA_%s.csv",varname(thevar)));
   ofstream of_PU(Form("csv/tnp_PU_%s.csv",varname(thevar)));
   ofstream of_cor(Form("cor/tnp_tot_%s.csv",varname(thevar)));

   of_syst << "TnP (total)" << endl;
   of_isobinned << "TnP (iso binned)" << endl;
   of_isostat << "TnP (iso stat)" << endl;
   of_isosyst << "TnP (iso syst)" << endl;
   of_muidbinned << "TnP (muid binned)" << endl;
   of_muidstat << "TnP (muid stat)" << endl;
   of_muidsyst << "TnP (muid syst)" << endl;
   of_trigstat << "TnP (trig stat)" << endl;
   of_trigsyst << "TnP (trig syst)" << endl;
   of_STA << "TnP (STA)" << endl;
   of_PU << "TnP (PU)" << endl;
   of_cor << "TnP (total)" << endl;

   const TH1 *htot = ee[0]->GetTotalHistogram();
   const TH1 *htot2 = (isAllMass) ? ee2[0]->GetTotalHistogram() : NULL;
   double* binedges = binsvar(thevar);
   for (int i=0; i<nbins; i++) {
      int ibin1=i, ibin2=-1;
      if (isAllMass) {
         ibin1 = DYana::idx1560(thevar,i);
         ibin2 = DYana::idx60120(thevar,i);
      }
      double x1 = binedges[i];
      double x2 = binedges[i+1];
      double ei = ibin1>=0 ? ee[0]->GetEfficiency(ibin1+1) : ee2[0]->GetEfficiency(ibin2+1);
      of_syst << x1 << ", " << x2 << ", " << sqrt(mcov_tot[i][i])/ei << endl;

      for (int j=0; j<nbins; j++) {
         if (j>0) of_cor << ", ";
         of_cor << mcor_tot[i][j];
      }
      of_cor << endl;
   
      // now print the individual components...
      of_isobinned << x1 << ", " << x2 << ", " << sqrt(mcov_syst[8][i][i]+mcov_syst[9][i][i])/ei << endl;
      of_isostat << x1 << ", " << x2 << ", " << sqrt(mcov_iso[0][i][i]+mcov_iso[1][i][i]+mcov_iso[2][i][i])/ei << endl;
      of_isosyst << x1 << ", " << x2 << ", " << sqrt(mcov_syst[4][i][i]+mcov_syst[5][i][i])/ei << endl;
      of_muidbinned << x1 << ", " << x2 << ", " << sqrt(mcov_syst[6][i][i]+mcov_syst[7][i][i])/ei << endl;
      of_muidstat << x1 << ", " << x2 << ", " << sqrt(mcov_muid[0][i][i]+mcov_muid[1][i][i]+mcov_muid[2][i][i])/ei << endl;
      of_muidsyst << x1 << ", " << x2 << ", " << sqrt(mcov_syst[2][i][i]+mcov_syst[3][i][i])/ei << endl;
      double t=0; for (int k=0; k<14; k++) t+=mcov_trg[k][i][i];
      of_trigstat << x1 << ", " << x2 << ", " << sqrt(t)/ei << endl;
      of_trigsyst << x1 << ", " << x2 << ", " << sqrt(mcov_syst[0][i][i]+mcov_syst[1][i][i])/ei << endl;
      of_STA << x1 << ", " << x2 << ", " << sqrt(mcov_syst[12][i][i]+mcov_syst[13][i][i]+mcov_muid[2][i][i])/ei << endl;
      of_PU << x1 << ", " << x2 << ", " << sqrt(mcov_syst[10][i][i]+mcov_syst[11][i][i]+mcov_muid[2][i][i])/ei << endl;
   }
   of_syst.close();
   of_isobinned.close();
   of_isostat.close();
   of_isosyst.close();
   of_muidbinned.close();
   of_muidstat.close();
   of_muidsyst.close();
   of_trigstat.close();
   of_trigsyst.close();
   of_STA.close();
   of_PU.close();
   of_cor.close();

   f->Close();
}

void Sys_TnP(const char* file, bool absxsec=true) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_TnP(file,thevar_i,absxsec);
   }
   Sys_TnP(file,var::ptall,absxsec);
   Sys_TnP(file,var::rapall,absxsec);
   Sys_TnP(file,var::phistarall,absxsec);
}
