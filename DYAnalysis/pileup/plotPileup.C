#include "../BkgEst/interface/defs.h"

// minimum bias cross section (in pb) from Table V of https://arxiv.org/pdf/1710.07098.pdf
const double xsec = 2120000e6;

void plotPileup() {
   TFile *f1 = TFile::Open("pileup/MyDataPileupHistogram_Pbp.root");
   TFile *f2 = TFile::Open("pileup/MyDataPileupHistogram_pPb.root");

   TH1D *h1 = (TH1D*) f1->Get("pileup");
   TH1D *h2 = (TH1D*) f2->Get("pileup");

   // scale each histo by lumi
   h1->Scale(lumi_part1*1e3/h1->Integral());
   h2->Scale(lumi_part2*1e3/h2->Integral());
   cout << h1->Integral() << " " << h2->Integral() << endl;

   // sum
   TH1D *hsum = (TH1D*) h1->Clone("hsum");
   hsum->Add(h2);
   hsum->SetTitle("");

   // graph options
   h1->SetLineColor(kRed);
   h2->SetLineColor(kBlue);
   hsum->SetLineColor(kBlack);
   hsum->SetLineWidth(2);

   // draw
   TCanvas *c1 = new TCanvas("c1","Pileup",800,600);
   hsum->Draw("L hist");
   hsum->GetXaxis()->SetRangeUser(0,1);
   hsum->GetXaxis()->SetTitle("#mu");
   hsum->GetYaxis()->SetTitle("nb^{-1} / 0.02");
   hsum->Draw("L hist");
   h1->Draw("L hist same");
   h2->Draw("L hist same");

   // legend
   TLegend *tleg = new TLegend(0.4,0.4,0.9,0.9);
   tleg->SetBorderSize(0);
   tleg->SetHeader(Form("#sigma_{MB} = %.2f b",xsec*1e-12));
   tleg->AddEntry(h1,Form("Pbp (L = %.1f nb^{-1}): <#mu> = %.3f",lumi_part1*1e3,h1->GetMean()),"L");
   tleg->AddEntry(h2,Form("pPb (L = %.1f nb^{-1}): <#mu> = %.3f",lumi_part2*1e3,h2->GetMean()),"L");
   tleg->AddEntry(hsum,Form("pPb+Pbp (L = %.1f nb^{-1}): <#mu> = %.3f",lumi_all*1e3,hsum->GetMean()),"L");
   tleg->Draw();

   c1->SaveAs("pileup/pileup.pdf");
}
