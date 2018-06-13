#include "TEfficiency.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TFrame.h"

#include "../Include/MyCanvas.C"
#include "../Include/tdrstyle.C"
#include "../Include/CMS_lumi.C"

void MakeAccEffGraph(TGraphAsymmErrors *g_AccEff, TGraphAsymmErrors *g_Acc, TGraphAsymmErrors *g_Eff);
void DrawAccEffDist(TString Type, TString Sample, TString variable, TGraphAsymmErrors* g_Acc, TGraphAsymmErrors* g_Eff_Corr, TGraphAsymmErrors* g_AccEff_Corr);
void PrintOutGraph(TGraphAsymmErrors* g);
void PrintTex(TGraphAsymmErrors* g_Acc, TGraphAsymmErrors* g_Eff, TGraphAsymmErrors* g_tnp, TGraphAsymmErrors* g_AccEff_Corr, TString texname);
Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B);
Double_t Error_PropagatedAtimesB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B);
// void Correction_AccEff(TH1D *h_yield_AccEff, TH1D *h_yield, TGraphAsymmErrors *g_AccEff);

void DrawAccEffPlot(TString HLTname = "PAL3Mu12", 
      TString variable = "Mass", // variable = Mass | Pt | Rap1560 | Rap60120 | Phistar
      TString MomCor = "MomCorr", // MomCorr or MomUnCorr
      TString Rew = "rewboth", // rewboth or norew or rewplus or rewminus
      TString Generator = "Powheg") 
{
	setTDRStyle();
	gROOT->SetStyle( "tdrStyle" );

	TString FileLocation = ".";// + version;

   const char* cvariable = variable.Data();

	TFile *f_input = new TFile(FileLocation + "/ROOTFile_Histogram_Acc_Eff_" + MomCor + "_" + Generator + "_" + HLTname + "_0_" + Rew + ".root");
	TString FileName = f_input->GetName();

	TString Sample = "";
	if( FileName.Contains("aMCNLO") )
		Sample = "aMCNLO";
	else if( FileName.Contains("Powheg") )
		Sample = "Powheg";
	else if( FileName.Contains("Pyquen") )
		Sample = "Pyquen";

	TEfficiency *TEff_Acc = (TEfficiency*)f_input->Get(Form("TEff_Acc_%s",cvariable));
	TGraphAsymmErrors *g_Acc = (TGraphAsymmErrors*)TEff_Acc->CreateGraph()->Clone();

	TEfficiency *TEff_Eff = (TEfficiency*)f_input->Get(Form("TEff_Eff_%s",cvariable));
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

	TCanvas *c_compare = new TCanvas("c_AccEff_"+variable+"_"+Sample, "", 800, 600);
	c_compare->cd();
   if (!variable.Contains("Rap")) {
      gPad->SetLogx();
   }
   if (variable=="Mass") {
      gPad->SetLogy();
   }

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

   if (variable=="Mass") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon Mass (post-FSR) [GeV]");
   } else if (variable=="Pt") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon Pt (post-FSR) [GeV]");
   } else if (variable=="Rap1560") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon y_{CM} (post-FSR)");
   } else if (variable=="Rap60120") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon y_{CM} (post-FSR)");
   } else if (variable=="Phistar") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon #phi^{*} (post-FSR)");
   }
	g_Acc->GetXaxis()->SetNoExponent();
	g_Acc->GetXaxis()->SetMoreLogLabels();
   // if( Sample == "Powheg" )
   //    g_Acc->GetXaxis()->SetRangeUser(50, 3000);
	g_Acc->GetYaxis()->SetTitle( "Value" );
   // g_Acc->GetYaxis()->SetRangeUser(0, 1.1);
	g_Acc->GetYaxis()->SetRangeUser(1e-3, 1.1);

   double lx1=0.55,ly1=0.2,lx2=0.99,ly2=0.35;
   if (variable=="Rap1560") {ly1+=0.2;ly2+=0.2;}
	TLegend *legend = new TLegend(lx1,ly1,lx2,ly2);
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
	latex.DrawLatex(x_201513TeV, 1.11, "#font[42]{pPb, 8.16TeV}");

   // Double_t x1 = 17; // -- CMS, preliminary x-axis value -- //
   // if( Sample == "Powheg" )
   //    x1 = 60;
   // latex.DrawLatex(x1, 1.01, "#font[62]{CMS}");
   // latex.DrawLatex(x1, 0.95, "#font[42]{#it{#scale[0.8]{Simulation}}}");

   extraText = "Simulation";
   writeExtraText = true;
   CMS_lumi( c_compare, 111, 0 );

   c_compare->Update();
   c_compare->RedrawAxis();
   c_compare->GetFrame()->Draw();
   c_compare->Modified();


	TString CanvasName = c_compare->GetName();
	c_compare->SaveAs(CanvasName+".pdf");
	c_compare->SaveAs(CanvasName+".C");


	TEfficiency *TEff_Eff_Corr_tnp = (TEfficiency*)f_input->Get(Form("TEff_Eff_%s_Corr_tnp0",cvariable));
   // TGraphAsymmErrors *g_Eff_Corr_tnp = (TGraphAsymmErrors*)TEff_Eff_Corr_tnp->CreateGraph()->Clone();
	TGraphAsymmErrors *g_Eff_Corr_tnp = (TGraphAsymmErrors*) f_input->Get(Form("g_Eff_%s_Corr_tnp_tot",cvariable));

	// TEfficiency *TEff_AccEff_Corr_tnp = (TEfficiency*)f_input->Get("TEff_AccEff_Mass_Corr_tnp");
	// TGraphAsymmErrors *g_AccEff_Corr_tnp = (TGraphAsymmErrors*)TEff_AccEff_Corr_tnp->CreateGraph()->Clone();

	TGraphAsymmErrors *g_AccEff_Corr_tnp = (TGraphAsymmErrors*)g_Acc->Clone();
	MakeAccEffGraph( g_AccEff_Corr_tnp, g_Acc, g_Eff_Corr_tnp );




	MyCanvas *myc_tnp = new MyCanvas("c_UnCorr_vs_Corr_tnp", g_Acc->GetXaxis()->GetTitle(), "Values");
	myc_tnp->LowerEdge_Y = 0.5;
	myc_tnp->UpperEdge_Y = 1.05;

	myc_tnp->LowerEdge_Ratio = 0.9;
	myc_tnp->UpperEdge_Ratio = 1.02;
	if (!variable.Contains("Rap")) myc_tnp->isLogX = kTRUE;

	myc_tnp->CanvasWithGraphRatioPlot( (TGraphAsymmErrors*)g_Eff_Corr_tnp->Clone(), (TGraphAsymmErrors*)g_Eff->Clone(),
									"Efficiency (SF Corr_tnp)", "Efficiency", "After/Before",
									kBlue+1, kGreen+1,
									"EP", "EPSAME");

	myc_tnp->c->SaveAs(Form("Eff_UnCorr_vs_Corr_tnp_%s.pdf",cvariable));



	DrawAccEffDist("Corr_tnp", Sample, variable, g_Acc, g_Eff_Corr_tnp, g_AccEff_Corr_tnp);
   // PrintOutGraph( g_Acc );
   // PrintOutGraph( g_Eff_Corr_tnp );
   // PrintOutGraph( g_AccEff_Corr_tnp );

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

   // print TeX table
   PrintTex(g_Acc, g_Eff, g_EffCorr_tnp, g_AccEff_Corr_tnp, "AccEff/tex/acceff_" + variable + ".tex");

}

void DrawAccEffDist(TString Type, TString Sample, TString variable, TGraphAsymmErrors* g_Acc, TGraphAsymmErrors* g_Eff_Corr, TGraphAsymmErrors* g_AccEff_Corr)
{
	TString CanvasName = "c_AccEff_" + variable + "_" + Type + "_" + Sample;
	TCanvas *c_compare_corr = new TCanvas(CanvasName, "", 800, 600);
	c_compare_corr->cd();
	if (!variable.Contains("Rap")) gPad->SetLogx();
   if (variable=="Mass") gPad->SetLogy();

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

   if (variable=="Mass") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon Mass (post-FSR) [GeV]");
   } else if (variable=="Pt") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon Pt (post-FSR) [GeV]");
   } else if (variable=="Rap1560") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon y_{CM} (post-FSR)");
   } else if (variable=="Rap60120") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon y_{CM} (post-FSR)");
   } else if (variable=="Phistar") {
      g_Acc->GetXaxis()->SetTitle( "Gen-Level Dimuon #phi^{*} (post-FSR)");
   }
	g_Acc->GetXaxis()->SetNoExponent();
	if (!variable.Contains("Rap")) g_Acc->GetXaxis()->SetMoreLogLabels();
   // if( Sample == "Powheg" )
   //    g_Acc->GetXaxis()->SetRangeUser(50, 3000);
	g_Acc->GetYaxis()->SetTitle( "Value" );
	g_Acc->GetYaxis()->SetRangeUser(1e-3, 1.1);

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
	latex.DrawLatex(x_201513TeV, 1.11, "#font[42]{pPb, 8.16TeV}");

   // Double_t x1 = 17; // -- CMS, preliminary x-axis value -- //
   // if( Sample == "Powheg" )
   //    x1 = 60;
   // latex.DrawLatex(x1, 1.01, "#font[62]{CMS}");
   // latex.DrawLatex(x1, 0.95, "#font[42]{#it{#scale[0.8]{Simulation}}}");

   extraText = "Simulation";
   writeExtraText = true;
   CMS_lumi( c_compare_corr, 111, 0 );

   c_compare_corr->Update();
   c_compare_corr->RedrawAxis();
   c_compare_corr->GetFrame()->Draw();
   c_compare_corr->Modified();

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
		error_AccEff = Error_PropagatedAtimesB(y_Acc, error_Acc, y_Eff, error_Eff);

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

Double_t Error_PropagatedAtimesB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
{
	Double_t ratio_A = (sigma_A) / A;
	Double_t ratio_B = (sigma_B) / B;

	Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

	return (A*B) * sqrt(errorSquare);
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

void PrintTex(TGraphAsymmErrors* g_Acc, TGraphAsymmErrors* g_Eff, TGraphAsymmErrors* g_tnp, TGraphAsymmErrors* g_AccEff_Corr, TString texname) {
   ofstream file(texname.Data());
   file.precision(3);
   file << "\\begin{tabular}{l|ccc|c}" << endl;
   file << "Bin & Acc. & Eff. (no TnP) & TnP corr. & Acc $\\times$ Eff (TnP) \\\\" << endl;
   file << "\\hline" << endl;

   for (int i=0; i<g_Acc->GetN(); i++) {
      file << g_Acc->GetX()[i] - g_Acc->GetEXlow()[i] << ", " << g_Acc->GetX()[i] + g_Acc->GetEXhigh()[i];
      file.setf(ios::fixed);
      double val = g_Acc->GetY()[i];
      double errlow = g_Acc->GetErrorYlow(i);
      double errhigh = g_Acc->GetErrorYhigh(i);
      if (fabs(errlow-errhigh)>1e-3) {
         file << " & $" << val << "_{-" << errlow << "}^{+" << errhigh << "}$ ";
      } else {
         file << " & $" << val << " \\pm " << max(errlow,errhigh) << "$ ";
      }
      val = g_Eff->GetY()[i];
      errlow = g_Eff->GetErrorYlow(i);
      errhigh = g_Eff->GetErrorYhigh(i);
      if (fabs(errlow-errhigh)>1e-3) {
         file << " & $" << val << "_{-" << errlow << "}^{+" << errhigh << "}$ ";
      } else {
         file << " & $" << val << " \\pm " << max(errlow,errhigh) << "$ ";
      }
      val = g_tnp->GetY()[i];
      errlow = g_tnp->GetErrorYlow(i);
      errhigh = g_tnp->GetErrorYhigh(i);
      if (fabs(errlow-errhigh)>1e-3) {
         file << " & $" << val << "_{-" << errlow << "}^{+" << errhigh << "}$ ";
      } else {
         file << " & $" << val << " \\pm " << max(errlow,errhigh) << "$ ";
      }
      val = g_AccEff_Corr->GetY()[i];
      errlow = g_AccEff_Corr->GetErrorYlow(i);
      errhigh = g_AccEff_Corr->GetErrorYhigh(i);
      if (fabs(errlow-errhigh)>1e-3) {
         file << " & $" << val << "_{-" << errlow << "}^{+" << errhigh << "}$ ";
      } else {
         file << " & $" << val << " \\pm " << max(errlow,errhigh) << "$ ";
      }
      file.unsetf(ios::fixed);
      file << " \\\\" << endl;
   }

   file << "\\end{tabular}" << endl;
   file.close();
   cout << "Closed " << texname << endl;
}
