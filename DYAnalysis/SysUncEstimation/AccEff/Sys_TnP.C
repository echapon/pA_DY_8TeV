#include "../../BkgEst/interface/defs.h"
#include "../../Include/MyCanvas.C"
#include "../../Include/texUtils.h"
#include "../syst.C"

#include "TFile.h"
#include "TH1.h"
#include "TMatrixT.h"
#include "TEfficiency.h"

#include <vector>

using namespace std;
using DYana::var;

// HOW TO USE THIS FILE
// [0] .include /dir/to/lhapdf/include
// [1] gSystem->Load("/dir/to/lhapdf/lib/libLHAPDF.so");
// .L AccEff/Sys_AccEff.C+

void Sys_TnP(const char* file, var thevar) {
   TFile *f = TFile::Open(file);
   if (!f || !f->IsOpen()) return;

   TEfficiency **ee = new TEfficiency*[645];
   for (int i=0; i<645; i++) ee[i] = (TEfficiency*) f->Get(Form("TEff_Eff_%s_Corr_tnp%d",Varname(thevar),i)); 
   int nbins = ee[0]->GetTotalHistogram()->GetNbinsX();

   vector<TMatrixT<double> > mcov_trg, mcov_muid, mcov_iso, mcov_syst;

   // initialise the vectors of covariance matrices
   for (int i=0; i<14; i++) mcov_trg.push_back(TMatrixT<double>(nbins,nbins));
   for (int i=0; i<3; i++) mcov_muid.push_back(TMatrixT<double>(nbins,nbins));
   for (int i=0; i<3; i++) mcov_iso.push_back(TMatrixT<double>(nbins,nbins));
   for (int i=0; i<14; i++) mcov_syst.push_back(TMatrixT<double>(nbins,nbins));

   for (int ibin=0; ibin<nbins; ibin++) {
      // compute the tnp syst uncertainty
      double e0i = ee[0]->GetEfficiency(ibin+1);

      for (int jbin=0; jbin<nbins; jbin++) {
         double e0j = ee[0]->GetEfficiency(jbin+1);

         // stat uncertainties
         // trigger
         for (int i=1; i<=14; i++) {
            mcov_trg[i-1][ibin][jbin] = max(fabs(ee[600+i]->GetEfficiency(ibin+1)-e0i),fabs(ee[614+i]->GetEfficiency(ibin+1)-e0i))
               * max(fabs(ee[600+i]->GetEfficiency(jbin+1)-e0j),fabs(ee[614+i]->GetEfficiency(jbin+1)-e0j));
         }

         // muID
         for (int ix=1; ix<=300; ix++) mcov_muid[(ix-1)/100][ibin][jbin] += (ee[ix]->GetEfficiency(ibin+1)-e0i)
            * (ee[ix]->GetEfficiency(jbin+1)-e0j) / 99.;
         
         // iso
         for (int ix=301; ix<=600; ix++) mcov_iso[(ix-301)/100][ibin][jbin] += (ee[ix]->GetEfficiency(ibin+1)-e0i)
            * (ee[ix]->GetEfficiency(jbin+1)-e0j) / 99.;


         // syst uncertainties: no bin-to-bin correlations
         if (ibin==jbin) {
            // trigger
            mcov_syst[0][ibin][jbin] = max(fabs(ee[629]->GetEfficiency(ibin+1)-e0i),fabs(ee[630]->GetEfficiency(ibin+1)-e0i))
               * max(fabs(ee[629]->GetEfficiency(jbin+1)-e0j),fabs(ee[630]->GetEfficiency(jbin+1)-e0j));
            mcov_syst[1][ibin][jbin] = max(fabs(ee[631]->GetEfficiency(ibin+1)-e0i),fabs(ee[632]->GetEfficiency(ibin+1)-e0i))
               * max(fabs(ee[631]->GetEfficiency(jbin+1)-e0j),fabs(ee[632]->GetEfficiency(jbin+1)-e0j));

            // muID
            mcov_syst[2][ibin][jbin] = max(fabs(ee[633]->GetEfficiency(ibin+1)-e0i),fabs(ee[634]->GetEfficiency(ibin+1)-e0i))
               * max(fabs(ee[633]->GetEfficiency(jbin+1)-e0j),fabs(ee[634]->GetEfficiency(jbin+1)-e0j));
            mcov_syst[3][ibin][jbin] = max(fabs(ee[635]->GetEfficiency(ibin+1)-e0i),fabs(ee[636]->GetEfficiency(ibin+1)-e0i))
               * max(fabs(ee[635]->GetEfficiency(jbin+1)-e0j),fabs(ee[636]->GetEfficiency(jbin+1)-e0j));

            // iso
            mcov_syst[4][ibin][jbin] = max(fabs(ee[637]->GetEfficiency(ibin+1)-e0i),fabs(ee[638]->GetEfficiency(ibin+1)-e0i))
               * max(fabs(ee[637]->GetEfficiency(jbin+1)-e0j),fabs(ee[638]->GetEfficiency(jbin+1)-e0j));
            mcov_syst[5][ibin][jbin] = max(fabs(ee[639]->GetEfficiency(ibin+1)-e0i),fabs(ee[640]->GetEfficiency(ibin+1)-e0i))
               * max(fabs(ee[639]->GetEfficiency(jbin+1)-e0j),fabs(ee[640]->GetEfficiency(jbin+1)-e0j));

            // muID binned
            mcov_syst[6][ibin][jbin] = fabs(ee[641]->GetEfficiency(ibin+1)-e0i) * fabs(ee[641]->GetEfficiency(jbin+1)-e0j);
            mcov_syst[7][ibin][jbin] = fabs(ee[642]->GetEfficiency(ibin+1)-e0i) * fabs(ee[642]->GetEfficiency(jbin+1)-e0j);

            // iso binned
            mcov_syst[8][ibin][jbin] = fabs(ee[643]->GetEfficiency(ibin+1)-e0i) * fabs(ee[643]->GetEfficiency(jbin+1)-e0j);
            mcov_syst[9][ibin][jbin] = fabs(ee[644]->GetEfficiency(ibin+1)-e0i) * fabs(ee[644]->GetEfficiency(jbin+1)-e0j);

            // HF+PU
            mcov_syst[10][ibin][jbin] = pow(0.34e-2,2);
            mcov_syst[11][ibin][jbin] = pow(0.34e-2,2);

            // STA
            mcov_syst[12][ibin][jbin] = pow(0.6e-2,2);
            mcov_syst[13][ibin][jbin] = pow(0.6e-2,2);
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
   for (int i=0; i<nbins; i++) {
      double x = htot->GetBinCenter(i+1);
      double dx = htot->GetBinWidth(i+1)/2.;
      double ei = ee[0]->GetEfficiency(i+1);
      of_syst << x-dx << ", " << x+dx << ", " << sqrt(mcov_tot[i][i])/ei << endl;

      for (int j=0; j<nbins; j++) {
         if (j>0) of_cor << ", ";
         of_cor << mcor_tot[i][j];
      }
      of_cor << endl;
   
      // now print the individual components...
      of_isobinned << x-dx << ", " << x+dx << ", " << sqrt(mcov_syst[8][i][i]+mcov_syst[9][i][i])/ei << endl;
      of_isostat << x-dx << ", " << x+dx << ", " << sqrt(mcov_iso[0][i][i]+mcov_iso[1][i][i]+mcov_iso[2][i][i])/ei << endl;
      of_isosyst << x-dx << ", " << x+dx << ", " << sqrt(mcov_syst[4][i][i]+mcov_syst[5][i][i])/ei << endl;
      of_muidbinned << x-dx << ", " << x+dx << ", " << sqrt(mcov_syst[6][i][i]+mcov_syst[7][i][i])/ei << endl;
      of_muidstat << x-dx << ", " << x+dx << ", " << sqrt(mcov_muid[0][i][i]+mcov_muid[1][i][i]+mcov_muid[2][i][i])/ei << endl;
      of_muidsyst << x-dx << ", " << x+dx << ", " << sqrt(mcov_syst[2][i][i]+mcov_syst[3][i][i])/ei << endl;
      double t=0; for (int k=0; k<14; k++) t+=mcov_trg[k][i][i];
      of_trigstat << x-dx << ", " << x+dx << ", " << sqrt(t)/ei << endl;
      of_trigsyst << x-dx << ", " << x+dx << ", " << sqrt(mcov_syst[0][i][i]+mcov_syst[1][i][i])/ei << endl;
      of_STA << x-dx << ", " << x+dx << ", " << sqrt(mcov_syst[12][i][i]+mcov_syst[13][i][i]+mcov_muid[2][i][i])/ei << endl;
      of_PU << x-dx << ", " << x+dx << ", " << sqrt(mcov_syst[10][i][i]+mcov_syst[11][i][i]+mcov_muid[2][i][i])/ei << endl;
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

void Sys_TnP(const char* file) {
   for (int i=0; i<var::ALLvar; i++) {
      var thevar_i = static_cast<var>(i);
      Sys_TnP(file,thevar_i);
   }
}
