#include "TTree.h"
#include "TH1.h"

void optimal_bins(TH1F* hpt, int nmin);
void optimal_bins(TTree *tr, TString cut, int nmin) {
   tr->Draw("pt>>hpt(60,0,30)",cut);
   TH1F *hpt = (TH1F*) gDirectory->Get("hpt");
   optimal_bins(hpt, nmin);
}

void optimal_bins(TH1F* hpt, int nmin)
{
   int nentries = hpt->Integral();
   int nbins = nentries/nmin;
   double *binedges = new double[nbins+1];
   double *bincontent = new double[nbins];
   double xmax = hpt->GetBinLowEdge(hpt->GetNbinsX()+1);
   cout << "Total events: " << nentries << ", " << nmin << " events per bin." << endl;
   int cnt=0;
   int tot=0;
   int ibin=0;
   bool isfirst=true;
   for (int i=1; i<hpt->GetNbinsX()+1; i++)
   {
      cnt += hpt->GetBinContent(i);
      tot += hpt->GetBinContent(i);
      if (isfirst&&cnt>0)
      {
         isfirst=false;
         binedges[0] = hpt->GetBinLowEdge(i);
      }
      if (cnt>= nmin&&(nentries-tot)>=nmin)
      {
         bincontent[ibin] = cnt;
         cnt=0;
         ibin++;
         binedges[ibin] = hpt->GetBinLowEdge(i+1);
         // cout << ibin << " " << binedges[ibin] << " " << bincontent[ibin] << endl;
      }
   }

   if (ibin<nbins)
   {
      bincontent[ibin] = cnt;
      cnt=0;
      ibin++;
      binedges[ibin] = xmax;
      // cout << ibin << "..." << endl;
   }

   for (ibin=0; ibin<nbins; ibin++)
   {
      cout << ibin << ": [" << binedges[ibin] << "," << binedges[ibin+1] << "]: " << bincontent[ibin] << endl;
      if (binedges[ibin+1]==xmax) break;
   }

   int nbins2=1;
   cout << 0 << ", ";
   for (int ibin=0; ibin<nbins; ibin++)
   {
      if (binedges[ibin]==xmax) break;
      cout << binedges[ibin] << ", ";
      nbins2++;
   }
   cout << xmax << endl;
   cout << "-> " << nbins2 << " bins." << endl;
}
