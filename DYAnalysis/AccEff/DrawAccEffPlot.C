#include "Include/MyCanvas.C"

void MakeAccEffGraph(TGraphAsymmErrors *g_AccEff, TGraphAsymmErrors *g_Acc, TGraphAsymmErrors *g_Eff);
void DrawAccEffDist(TString Type, TString Sample, TGraphAsymmErrors* g_Acc, TGraphAsymmErrors* g_Eff_Corr, TGraphAsymmErrors* g_AccEff_Corr);
void PrintOutGraph(TGraphAsymmErrors* g);
Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B);
// void Correction_AccEff(TH1D *h_yield_AccEff, TH1D *h_yield, TGraphAsymmErrors *g_AccEff);

void DrawAccEffPlot(TString version = "None")
{
	setTDRStyle();
	gROOT->SetStyle( "tdrStyle" );

	TString FileLocation = ".";// + version;

	if( version == "None" )
		FileLocation = ".";

	TFile *f_input = new TFile(FileLocation + "/ROOTFile_Histogram_Acc_Eff_Powheg_HLT_PAL3Mu12_v*.root");
	TString FileName = f_input->GetName();

	TString Sample = "";
	if( FileName.Contains("aMCNLO") )
		Sample = "aMCNLO";
	else if( FileName.Contains("Powheg") )
		Sample = "Powheg";

	TEfficiency *TEff_Acc = (TEfficiency*)f_input->Get("TEff_Acc_Mass");
	TGraphAsymmErrors *g_Acc = (TGraphAsymmErrors*)TEff_Acc->CreateGraph()->Clone();

	TEfficiency *TEff_Eff = (TEfficiency*)f_input->Get("TEff_Eff_Mass");
	TGraphAsymmErrors *g_Eff = (TGraphAsymmErrors*)TEff_Eff->CreateGraph()->Clone();

	TGraphAsymmErrors *g_AccEff = (TGraphAsymmErrors*)g_Acc->Clone();
	MakeAccEffGraph( g_AccEff, g_Acc, g_Eff );

	g_Acc->SetName("g_Acc");
	g_Eff->SetName("g_Eff");
	g_AccEff->SetName("g_AccEff");
	PrintOutGraph( g_Acc );
	PrintOutGraph( g_Eff );
	PrintOutGraph( g_AccEff );

	// TEfficiency *TEff_AccEff = (TEfficiency*)f_input->Get("TEff_AccEff_Mass");
	// TGraphAsymmErrors *g_AccEff = (TGraphAsymmErrors*)TEff_AccEff->CreateGraph()->Clone();

	TCanvas *c_compare = new TCanvas("c_AccEff_"+Sample, "", 800, 600);
	c_compare->cd();
	gPad->SetLogx();
	gPad->SetLogy();

	g_Acc->Draw("AP");
	g_Eff->Draw("PSAME");
	g_AccEff->Draw("PSAME");

	g_Acc->SetLineColor(kGreen+1);
	g_Acc->SetMarkerColor(kGreen+1);
	g_Acc->SetMarkerStyle(20);
	g_Acc->SetMarkerSize(1);

	g_Eff->SetLineColor(kBlue);
	g_Eff->SetMarkerColor(kBlue);
	g_Eff->SetMarkerStyle(20);
	g_Eff->SetMarkerSize(1);

	g_AccEff->SetLineColor(kRed);
	g_AccEff->SetMarkerColor(kRed);
	g_AccEff->SetMarkerStyle(20);
	g_AccEff->SetMarkerSize(1);

	g_Acc->SetFillColorAlpha(kWhite, 0);
	g_Eff->SetFillColorAlpha(kWhite, 0);
	g_AccEff->SetFillColorAlpha(kWhite, 0);

	g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon Mass (post-FSR) [GeV]");
	g_Acc->GetXaxis()->SetNoExponent();
	g_Acc->GetXaxis()->SetMoreLogLabels();
   // if( Sample == "Powheg" )
   //    g_Acc->GetXaxis()->SetRangeUser(50, 3000);
	g_Acc->GetYaxis()->SetTitle( "Value" );
   // g_Acc->GetYaxis()->SetRangeUser(0, 1.1);
	g_Acc->GetYaxis()->SetRangeUser(1e-4, 1.1);

	TLegend *legend = new TLegend(0.55, 0.2, 0.99, 0.35);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	legend->AddEntry( g_Acc, "Acceptance" );
	legend->AddEntry( g_Eff, "Efficiency" );
	legend->AddEntry( g_AccEff, "Acceptance * Efficiency" );
	legend->Draw();


	TLatex latex;
	
	Double_t x_201513TeV = 971;
	if( Sample == "Powheg" )
		x_201513TeV = 1150;
	latex.DrawLatex(x_201513TeV, 1.11, "#font[42]{2015, 13TeV}");

	Double_t x1 = 17; // -- CMS, preliminary x-axis value -- //
	if( Sample == "Powheg" )
		x1 = 60;
	latex.DrawLatex(x1, 1.01, "#font[62]{CMS}");
	latex.DrawLatex(x1, 0.95, "#font[42]{#it{#scale[0.8]{Simulation}}}");

	TString CanvasName = c_compare->GetName();
	c_compare->SaveAs(CanvasName+".pdf");
	c_compare->SaveAs(CanvasName+".C");


	TEfficiency *TEff_Eff_Corr_tnp = (TEfficiency*)f_input->Get("TEff_Eff_Mass_Corr_tnp");
	TGraphAsymmErrors *g_Eff_Corr_tnp = (TGraphAsymmErrors*)TEff_Eff_Corr_tnp->CreateGraph()->Clone();

	// TEfficiency *TEff_AccEff_Corr_tnp = (TEfficiency*)f_input->Get("TEff_AccEff_Mass_Corr_tnp");
	// TGraphAsymmErrors *g_AccEff_Corr_tnp = (TGraphAsymmErrors*)TEff_AccEff_Corr_tnp->CreateGraph()->Clone();

	TGraphAsymmErrors *g_AccEff_Corr_tnp = (TGraphAsymmErrors*)g_Acc->Clone();
	MakeAccEffGraph( g_AccEff_Corr_tnp, g_Acc, g_Eff_Corr_tnp );




	MyCanvas *myc_tnp = new MyCanvas("c_UnCorr_vs_Corr_tnp", "Gen-Level Dimuon Mass [GeV]", "Values");
	myc_tnp->LowerEdge_Y = 0.65;
	myc_tnp->UpperEdge_Y = 1.05;

	myc_tnp->LowerEdge_Ratio = 0.9;
	myc_tnp->UpperEdge_Ratio = 1.02;
	myc_tnp->isLogX = kTRUE;

	myc_tnp->CanvasWithGraphRatioPlot( (TGraphAsymmErrors*)g_Eff_Corr_tnp->Clone(), (TGraphAsymmErrors*)g_Eff->Clone(),
									"Efficiency (SF Corr_tnp)", "Efficiency", "After/Before",
									kBlue+1, kGreen+1,
									"EP", "EPSAME");

	myc_tnp->c->SaveAs("Eff_UnCorr_vs_Corr_tnp.pdf");



	DrawAccEffDist("Corr_tnp", Sample, g_Acc, g_Eff_Corr_tnp, g_AccEff_Corr_tnp);

	TFile *f_output = new TFile("ROOTFile_AccEff.root", "RECREATE");
	f_output->cd();

	g_Acc->SetName("g_Acc");
	g_Acc->Write();

	g_Eff->SetName("g_Eff");
	g_Eff->Write();

	g_Eff_Corr_tnp->SetName("g_Eff_Corr_tnp");
	g_Eff_Corr_tnp->Write();


	TGraphAsymmErrors* g_EffCorr_tnp = (TGraphAsymmErrors*)myc_tnp->g_ratio->Clone();
	g_EffCorr_tnp->SetName("g_EffCorr_tnp");
	g_EffCorr_tnp->Write();

	
	g_AccEff->SetName("g_AccEff");
	g_AccEff->Write();

	g_AccEff_Corr_tnp->SetName("g_AccEff_Corr_tnp");
	g_AccEff_Corr_tnp->Write();


}

void DrawAccEffDist(TString Type, TString Sample, TGraphAsymmErrors* g_Acc, TGraphAsymmErrors* g_Eff_Corr, TGraphAsymmErrors* g_AccEff_Corr)
{
	TString CanvasName = "c_AccEff_" + Type + "_" + Sample;
	TCanvas *c_compare_corr = new TCanvas(CanvasName, "", 800, 600);
	c_compare_corr->cd();
	gPad->SetLogx();

	g_Acc->Draw("AP");
	g_Eff_Corr->Draw("PSAME");
	g_AccEff_Corr->Draw("PSAME");

	g_Acc->SetLineColor(kGreen+1);
	g_Acc->SetMarkerColor(kGreen+1);
	g_Acc->SetMarkerStyle(20);
	g_Acc->SetMarkerSize(1);

	g_Eff_Corr->SetLineColor(kBlue);
	g_Eff_Corr->SetMarkerColor(kBlue);
	g_Eff_Corr->SetMarkerStyle(20);
	g_Eff_Corr->SetMarkerSize(1);

	g_AccEff_Corr->SetLineColor(kRed);
	g_AccEff_Corr->SetMarkerColor(kRed);
	g_AccEff_Corr->SetMarkerStyle(20);
	g_AccEff_Corr->SetMarkerSize(1);

	g_Acc->SetFillColorAlpha(kWhite, 0);
	g_Eff_Corr->SetFillColorAlpha(kWhite, 0);
	g_AccEff_Corr->SetFillColorAlpha(kWhite, 0);

	g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon Mass (post-FSR) [GeV]");
	g_Acc->GetXaxis()->SetNoExponent();
	g_Acc->GetXaxis()->SetMoreLogLabels();
   // if( Sample == "Powheg" )
   //    g_Acc->GetXaxis()->SetRangeUser(50, 3000);
	g_Acc->GetYaxis()->SetTitle( "Value" );
	g_Acc->GetYaxis()->SetRangeUser(0, 1.1);

	TLegend *legend = new TLegend(0.55, 0.2, 0.99, 0.35);
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);

	legend->AddEntry( g_Acc, "Acceptance" );
	legend->AddEntry( g_Eff_Corr, "Efficiency (Corr)" );
	legend->AddEntry( g_AccEff_Corr, "Acceptance * Efficiency (Corr)" );
	legend->Draw();


	TLatex latex;
	
	Double_t x_201513TeV = 971;
	if( Sample == "Powheg" )
		x_201513TeV = 1150;
	latex.DrawLatex(x_201513TeV, 1.11, "#font[42]{2015, 13TeV}");

	Double_t x1 = 17; // -- CMS, preliminary x-axis value -- //
	if( Sample == "Powheg" )
		x1 = 60;
	latex.DrawLatex(x1, 1.01, "#font[62]{CMS}");
	latex.DrawLatex(x1, 0.95, "#font[42]{#it{#scale[0.8]{Simulation}}}");

	c_compare_corr->SaveAs(CanvasName+".pdf");
}

void MakeAccEffGraph(TGraphAsymmErrors *g_AccEff, TGraphAsymmErrors *g_Acc, TGraphAsymmErrors *g_Eff)
{
	g_AccEff->Set(0);
	Int_t nPoint = g_Acc->GetN();
	for(Int_t i=0; i<nPoint; i++)
	{
		Double_t x_Acc, y_Acc, error_Acc, x_Eff, y_Eff, error_Eff;
		g_Acc->GetPoint(i, x_Acc, y_Acc);
		error_Acc = g_Acc->GetErrorYhigh(i);

		if( error_Acc != g_Acc->GetErrorYlow(i) )
			printf("Acceptance has asymmetric error! ... (Low, High) = (%.5lf, %.5lf)\n", g_Acc->GetErrorYlow(i), error_Acc);
			// cout << "Acceptance has asymmetric error! ... (Low, High) = " << (Double_t)g_Acc->GetErrorYlow(i) << ", " << error_Acc << endl;

		g_Eff->GetPoint(i, x_Eff, y_Eff);
		error_Eff = g_Eff->GetErrorYhigh(i);

		if( error_Eff != g_Eff->GetErrorYlow(i) )
			printf("Efficiency has asymmetric error! ... (Low, High) = (%.5lf, %.5lf)\n", g_Eff->GetErrorYlow(i), error_Acc);

		Double_t x_AccEff, y_AccEff, error_AccEff;

		x_AccEff = x_Acc;
		y_AccEff = y_Acc * y_Eff;
		error_AccEff = Error_PropagatedAoverB(y_Acc, error_Acc, y_Eff, error_Eff);

		g_AccEff->SetPoint(i, x_AccEff, y_AccEff);
		g_AccEff->SetPointError(i, g_Acc->GetErrorXlow(i), g_Acc->GetErrorXhigh(i), error_AccEff, error_AccEff);
	}

}

Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
{
	Double_t ratio_A = (sigma_A) / A;
	Double_t ratio_B = (sigma_B) / B;

	Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

	return (A/B) * sqrt(errorSquare);
}


void PrintOutGraph(TGraphAsymmErrors* g)
{
	cout << "==========================================" << endl;

	cout << "Graph name: " << g->GetName() << endl;

	Int_t nPoints = g->GetN();
	for(Int_t i=0; i<nPoints; i++)
	{
		Double_t x, y, x_errorHigh, x_errorLow, y_errorHigh, y_errorLow;
		g->GetPoint(i, x, y);
		x_errorLow = g->GetErrorXlow(i);
		x_errorHigh = g->GetErrorXhigh(i);
		y_errorLow = g->GetErrorYlow(i);
		y_errorHigh = g->GetErrorYhigh(i);

		printf("\t[%2.d point] (x, x_errorLow, x_errorHigh, y, y_errorLow, y_errorHigh) = (%9.3lf, %9.3lf, %9.3lf, %9.5lf, %9.5lf, %9.5lf)\n", i, x, x_errorLow, x_errorHigh, y, y_errorLow, y_errorHigh);
	}
	cout << "==========================================" << endl;
}
