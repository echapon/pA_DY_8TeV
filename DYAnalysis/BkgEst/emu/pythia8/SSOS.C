void SSOS()
{
//=========Macro generated from canvas: c1_n2/c1_n2
//=========  (Thu Mar 28 19:10:30 2019) by ROOT version 6.16/00
   TCanvas *c1_n2 = new TCanvas("c1_n2", "c1_n2",16,97,700,500);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1_n2->Range(-41.77215,-0.1604938,236.7089,1.074074);
   c1_n2->SetFillColor(0);
   c1_n2->SetBorderMode(0);
   c1_n2->SetBorderSize(0);
   c1_n2->SetTickx(1);
   c1_n2->SetTicky(1);
   c1_n2->SetLeftMargin(0.15);
   c1_n2->SetRightMargin(0.06);
   c1_n2->SetTopMargin(0.06);
   c1_n2->SetBottomMargin(0.13);
   c1_n2->SetFrameLineColor(0);
   c1_n2->SetFrameBorderMode(0);
   c1_n2->SetFrameLineColor(0);
   c1_n2->SetFrameBorderMode(0);
   
   Double_t _fx3001[10] = {
   11.75,
   24.6,
   26.9,
   29.5,
   32.6,
   35.9,
   40.2,
   46.6,
   59.9,
   134.5726};
   Double_t _fy3001[10] = {
   0.3181818,
   0.6111111,
   0.7222222,
   0.5263158,
   0.2222222,
   0.3181818,
   0.3043478,
   0.5555556,
   0.3913043,
   0.467342};
   Double_t _felx3001[10] = {
   11.75,
   1.1,
   1.2,
   1.4,
   1.7,
   1.6,
   2.7,
   3.7,
   9.6,
   65.25};
   Double_t _fely3001[10] = {
   0.1320105,
   0.2243171,
   0.252617,
   0.197233,
   0.09588122,
   0.1320105,
   0.125719,
   0.2098845,
   0.147965,
   0.2022802};
   Double_t _fehx3001[10] = {
   11.75,
   1.1,
   1.2,
   1.4,
   1.7,
   1.6,
   2.7,
   3.7,
   9.6,
   65.25};
   Double_t _fehy3001[10] = {
   0.2131938,
   0.347336,
   0.3838378,
   0.3070134,
   0.1563218,
   0.2131938,
   0.2021835,
   0.3288455,
   0.2289017,
   0.341429};
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(10,_fx3001,_fy3001,_felx3001,_fehx3001,_fely3001,_fehy3001);
   grae->SetName("");
   grae->SetTitle("");
   grae->SetFillStyle(1000);
   grae->SetMarkerStyle(20);
   
   TH1F *Graph_Graph3001 = new TH1F("Graph_Graph3001","",100,0,220);
   Graph_Graph3001->SetMinimum(0);
   Graph_Graph3001->SetMaximum(1);
   Graph_Graph3001->SetDirectory(0);
   Graph_Graph3001->SetStats(0);
   Graph_Graph3001->SetLineStyle(0);
   Graph_Graph3001->SetMarkerStyle(20);
   Graph_Graph3001->GetXaxis()->SetTitle("M_{e#mu}");
   Graph_Graph3001->GetXaxis()->SetLabelFont(42);
   Graph_Graph3001->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph3001->GetXaxis()->SetLabelSize(0.045);
   Graph_Graph3001->GetXaxis()->SetTitleSize(0.055);
   Graph_Graph3001->GetXaxis()->SetTitleOffset(1.1);
   Graph_Graph3001->GetXaxis()->SetTitleFont(42);
   Graph_Graph3001->GetYaxis()->SetTitle("SS/OS");
   Graph_Graph3001->GetYaxis()->SetLabelFont(42);
   Graph_Graph3001->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph3001->GetYaxis()->SetLabelSize(0.045);
   Graph_Graph3001->GetYaxis()->SetTitleSize(0.055);
   Graph_Graph3001->GetYaxis()->SetTitleOffset(1.4);
   Graph_Graph3001->GetYaxis()->SetTitleFont(42);
   Graph_Graph3001->GetZaxis()->SetLabelFont(42);
   Graph_Graph3001->GetZaxis()->SetLabelSize(0.045);
   Graph_Graph3001->GetZaxis()->SetTitleOffset(1);
   Graph_Graph3001->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph3001);
   
   
   TF1 *PrevFitTMP3002 = new TF1("PrevFitTMP","pol0",0,220, TF1::EAddToList::kNo);
   PrevFitTMP3002->SetFillColor(19);
   PrevFitTMP3002->SetFillStyle(0);
   PrevFitTMP3002->SetMarkerStyle(20);
   PrevFitTMP3002->SetLineWidth(3);
   PrevFitTMP3002->SetChisquare(5.31857);
   PrevFitTMP3002->SetNDF(9);
   PrevFitTMP3002->GetXaxis()->SetLabelFont(42);
   PrevFitTMP3002->GetXaxis()->SetLabelOffset(0.01);
   PrevFitTMP3002->GetXaxis()->SetLabelSize(0.045);
   PrevFitTMP3002->GetXaxis()->SetTitleSize(0.055);
   PrevFitTMP3002->GetXaxis()->SetTitleOffset(1.1);
   PrevFitTMP3002->GetXaxis()->SetTitleFont(42);
   PrevFitTMP3002->GetYaxis()->SetLabelFont(42);
   PrevFitTMP3002->GetYaxis()->SetLabelOffset(0.01);
   PrevFitTMP3002->GetYaxis()->SetLabelSize(0.045);
   PrevFitTMP3002->GetYaxis()->SetTitleSize(0.055);
   PrevFitTMP3002->GetYaxis()->SetTitleOffset(1.4);
   PrevFitTMP3002->GetYaxis()->SetTitleFont(42);
   PrevFitTMP3002->SetParameter(0,0.4181022);
   PrevFitTMP3002->SetParError(0,0.06497563);
   PrevFitTMP3002->SetParLimits(0,0,0);
   PrevFitTMP3002->SetParent(grae);
   grae->GetListOfFunctions()->Add(PrevFitTMP3002);
   
   TPaveStats *ptstats = new TPaveStats(0.62,0.835,0.98,0.995,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   TText *ptstats_LaTex = ptstats->AddText("#chi^{2} / ndf = 5.319 / 9");
   ptstats_LaTex = ptstats->AddText("Prob  = 0.8057");
   ptstats_LaTex = ptstats->AddText("p0       = 0.4181 #pm 0.06498 ");
   ptstats->SetOptStat(0);
   ptstats->SetOptFit(1111);
   ptstats->Draw();
   grae->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(grae->GetListOfFunctions());
   grae->Draw("ap");
   c1_n2->Modified();
   c1_n2->cd();
   c1_n2->SetSelected(c1_n2);
}
