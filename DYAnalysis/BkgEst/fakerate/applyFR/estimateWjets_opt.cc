#include <TStyle.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <THStack.h>
#include <TMath.h>
#include <TText.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TColor.h>
#include <TLatex.h>
#include <TEfficiency.h>
#include <TFrame.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include "../../interface/tdrstyle.C"
#include "../../interface/CMS_lumi.C"
#include "../../interface/defs.h"
using namespace std;
using namespace DYana;

//void estimateWjets_opt(int optval) {
void estimateWjets_opt(){
	for (int optval=1;optval<6;optval++) {

		TString opttitle;
		TString histtitle;

		if (optval==1) {opttitle="Mass (GeV)";histtitle="hist";}
		else if (optval==2) {opttitle="#phi^{*} in 60<M<120 (GeV)";histtitle="Zphistar";}
		else if (optval==3) {opttitle="p_{T} (GeV) in 60<M<120 (GeV)";histtitle="Zpt";}
		else if (optval==4) {opttitle="y_{CM} in 60<M<120 (GeV)";histtitle="rap60120";}
		else if (optval==5) {opttitle="y_{CM} in 15<M<60 (GeV)";histtitle="rap1560";}

		double minx, maxx;
		int optbinnum;

		if (optval==1) {minx=15.0;maxx=3000.0;optbinnum=binnum;}
		else if (optval==2) {minx=0.0;maxx=3.0;optbinnum=phistarnum;}
		else if (optval==3) {minx=0.0;maxx=200.0;optbinnum=ptbinnum_meas;}
		else if (optval==4) {minx=-3.0;maxx=2.0;optbinnum=rapbinnum_60120;}
		else if (optval==5) {minx=-3.0;maxx=2.0;optbinnum=rapbinnum_1560;}




		int W = 1200;
		int H = 1200;

		int H_ref = 1200;
		int W_ref = 1200;

		// references for T, B, L, R
		float T = 0.08*H_ref;
		float B = 0.12*H_ref;
		float L = 0.12*W_ref;
		float R = 0.04*W_ref;

		// UPDATED IN 2017
		lumi_13TeV = Form("%.2f pb^{-1}",lumi_all);
		// lumi_13TeV = "2833 pb^{-1}";
		lumiTextSize = 0.5;
		writeExtraText = true;
		extraText = "Preliminary";
		drawLogo = false;

		// int binnum = 43;
		// double bins[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,  200, 220, 243, 273, 320, 380, 440, 510, 600, 700,  830, 1000, 1500, 3000};

		//TH1D* massFrame = new TH1D("massFrame","",38,15,3000);
		TH1D* massFrame = new TH1D("massFrame","",(maxx-minx)*100,minx,maxx);


		massFrame->SetMinimum(0.001);
		massFrame->SetMaximum(1000000);
		massFrame->SetStats(kFALSE);
		//    massFrame->GetXaxis()->SetTitle("Mass[GeV]");
		massFrame->GetXaxis()->SetTitle(Form("%s",opttitle.Data()));
		//massFrame->GetXaxis()->CenterTitle(kTRUE);
		//massFrame->GetYaxis()->CenterTitle(kTRUE);
		massFrame->GetYaxis()->SetTitleOffset(1);
		massFrame->GetYaxis()->SetTitle("Number of events");
		massFrame->GetXaxis()->SetTitleSize(0);
		massFrame->GetYaxis()->SetTitleSize(0.05);
		massFrame->GetXaxis()->SetLabelSize(0);
		//massFrame->GetYaxis()->SetLabelSize(0.025); 
		massFrame->GetXaxis()->SetMoreLogLabels();

		TFile* f[NSamples+2];
		//    for (int i=0; i<ALL; i++) f[i] = new TFile(PathFRHistos2(static_cast<SampleTag>(i)));
		//    f[QCD] = new TFile(PathFRHistos2(QCD));
		for (int i=0; i<ALL; i++) f[i] = new TFile(Form("histograms/fake%s_noFRweight.root",(TString*)(Name(static_cast<SampleTag>(i)))));
		f[QCD] = new TFile("histograms/fakeQCD_noFRweight.root");


		TH1D* wjets_template[NSamples+2];
		TH1D* wjetsSS_template[NSamples+2];
		TH1D* wjets_ratio[NSamples+2];
		TH1D* wjetsSS_ratio[NSamples+2];
		double norm[NSamples+2];

		for (int i=0; i<ALL; i++) {
			SampleTag tag = static_cast<SampleTag>(i);
			norm[i] = (Xsec(tag)*lumi_all)/Nevts(tag);
			cout<< "norm[" << Name(static_cast<SampleTag>(i)) << "] = " << norm[i]<<endl;

			//wjets_template[i] = (TH1D*)f[i]->Get("rap1560Dijet1");
			wjets_template[i] = (TH1D*)f[i]->Get(Form("%sWJets1",histtitle.Data()));
			wjets_template[i]->Scale(norm[i]);
			wjets_template[i]->SetStats(kFALSE);

			//wjetsSS_template[i] = (TH1D*)f[i]->Get("rap1560SameDijet1");
			wjetsSS_template[i] = (TH1D*)f[i]->Get(Form("%sSameWJets1",histtitle.Data()));
			wjetsSS_template[i]->Scale(norm[i]);
			wjetsSS_template[i]->SetStats(kFALSE);

			//wjets_ratio[i] = (TH1D*)f[i]->Get("rap1560Dijet2");
			wjets_ratio[i] = (TH1D*)f[i]->Get(Form("%sWJets2",histtitle.Data()));
			wjets_ratio[i]->Scale(norm[i]);
			wjets_ratio[i]->SetStats(kFALSE);

			//wjetsSS_ratio[i] = (TH1D*)f[i]->Get("rap1560SameDijet2");
			wjetsSS_ratio[i] = (TH1D*)f[i]->Get(Form("%sSameWJets2",histtitle.Data()));
			wjetsSS_ratio[i]->Scale(norm[i]);
			wjetsSS_ratio[i]->SetStats(kFALSE);

			// put histos in an array for easy style setting
			TH1D* h[4] = {wjets_template[i], wjetsSS_template[i], wjets_ratio[i], wjetsSS_ratio[i]};
			for (int j=0; j<4; j++) {
				//h[j]->GetXaxis()->SetTitle("Mass[GeV]");
				h[j]->GetXaxis()->SetTitle(Form("%s",opttitle.Data()));


				h[j]->GetYaxis()->SetTitleOffset(1.5);
				h[j]->GetYaxis()->SetTitle("Number of events");
				//h[j]->GetXaxis()->SetTitleSize(0.032);
				//h[j]->GetYaxis()->SetTitleSize(0.032);
				h[j]->GetXaxis()->SetLabelSize(0.025);
				h[j]->GetYaxis()->SetLabelSize(0.025);
				h[j]->GetXaxis()->SetMoreLogLabels();

				// remove negative bins
				//for(int k=1; k<rapbinnum_1560+3; k++) {
				for(int k=1; k<optbinnum+3; k++) {
					if(h[j]->GetBinContent(i) < 0) {
						h[j]->SetBinContent(k,0.0);
						h[j]->SetBinError(k,0.0);
					}
				}
			} // j loop on histos

			// add histos together
			bool toadd = false;
			SampleTag tagtoadd;
			if (IsDY(tag)) {
				// style 
				for (int j=0; j<4; j++) {
					h[j]->SetFillColor(2);
					// h[j]->SetmarkerColor(2);
					// h[j]->SetmarkerStyle(22);
				}

				if (tag != DYTauTau1030) {
					toadd = true;
					tagtoadd = DYTauTau1030;
				}
			}
			if (IsData(tag)) {
				// style 
				for (int j=0; j<4; j++) {
					h[j]->SetLineColor(1);
					h[j]->SetMarkerColor(1);
					h[j]->SetMarkerStyle(22);
				}

				if (tag != Data1) {
					toadd = true;
					tagtoadd = Data1;
				}
			}
			if (IsDiboson(tag)) {
				// style 
				for (int j=0; j<4; j++) {
					h[j]->SetFillColor(3);
				}

				if (tag != WW) {
					toadd = true;
					tagtoadd = WW;
				}
			}
			if (IsWjets(tag)) {
				// style 
				for (int j=0; j<4; j++) {
					h[j]->SetFillColor(4);
				}

				if (tag != WpMu) {
					toadd = true;
					tagtoadd = WpMu;
				}
			}
			if (tag==TT) {
				for (int j=0; j<4; j++) {
					h[j]->SetFillColor(6);
				}
			}

			if (toadd) {
				wjets_template[tagtoadd]->Add(wjets_template[i]);
				wjetsSS_template[tagtoadd]->Add(wjetsSS_template[i]);
				wjets_ratio[tagtoadd]->Add(wjets_ratio[i]);
				wjetsSS_ratio[tagtoadd]->Add(wjetsSS_ratio[i]);
			}
			} // sample loop

			cout<<"ttbar OS before = "<<wjets_template[TT]->Integral()<<endl;
			cout<<"ttbar SS before = "<<wjetsSS_template[TT]->Integral()<<endl;

			cout<<"DY(template): "<<wjets_template[DYTauTau1030]->Integral()<<endl;
			cout<<"DY(ratio): "<<wjets_ratio[DYTauTau1030]->Integral()<<endl;

			setTDRStyle();
			tdrGrid(true);

			lumiTextSize = 0.6;
			cmsTextSize = 1.0;

			setTDRStyle();
			tdrGrid(true);
			lumiTextSize = 0.5;
			cmsTextSize = 0.75;

			TCanvas* canv = new TCanvas("canv","",1200,1200);
			canv->SetFillColor(0);
			canv->SetLeftMargin( L/W );
			canv->SetRightMargin( R/W );
			canv->SetTopMargin( T/H );
			canv->SetBottomMargin( B/H );

			/////
			// subtract MC from data here
			wjets_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_template_%s_noFRweight_v2_beforeremove.pdf",histtitle.Data()));

			wjets_template[Data1]->Add(wjets_template[DYTauTau1030],-1.0);

			wjets_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_template_%s_noFRweight_v2_removedDY.pdf",histtitle.Data()));


			wjets_template[Data1]->Add(wjets_template[TT],-1.0);

			wjets_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_template_%s_noFRweight_v2_removedTT.pdf",histtitle.Data()));


			wjets_template[Data1]->Add(wjets_template[WW],-1.0);

			wjets_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_template_%s_noFRweight_v2_removedWWs.pdf",histtitle.Data()));

			wjets_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_ratio_%s_noFRweight_v2_beforeremove.pdf",histtitle.Data()));

			wjets_ratio[Data1]->Add(wjets_ratio[DYTauTau1030],-1.0);

			wjets_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_ratio_%s_noFRweight_v2_removedDY.pdf",histtitle.Data()));


			wjets_ratio[Data1]->Add(wjets_ratio[TT],-1.0);

			wjets_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_ratio_%s_noFRweight_v2_removedTT.pdf",histtitle.Data()));


			wjets_ratio[Data1]->Add(wjets_ratio[WW],-1.0);

			wjets_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_ratio_%s_noFRweight_v2_removedWWs.pdf",histtitle.Data()));

/*
			wjets_ratio[Data1]->Add(wjets_ratio[DYTauTau1030],-1.0);
			wjets_ratio[Data1]->Add(wjets_ratio[TT],-1.0);
			wjets_ratio[Data1]->Add(wjets_ratio[WW],-1.0);
*/

			//wjets_template[5]->Smooth();
			TLegend* legg = new TLegend(.6,.65,.95,.89);
			legg->AddEntry(wjets_template[Data1],"Opposite sign", "F");
			legg->AddEntry(wjetsSS_template[Data1],"Same sign", "P");

			wjets_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_template_%s_noFRweight_v2.pdf",histtitle.Data()));
			wjetsSS_template[Data1]->Draw("EPSAME");
			legg->Draw("SAME");
			//canv->Print("print/wjetsBoth_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjetsBoth_template_%s_noFRweight_v2.pdf",histtitle.Data()));
			canv->Clear();
			wjetsSS_template[Data1]->SetFillColor(7);
			wjetsSS_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjetsSS_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjetsSS_template_%s_noFRweight_v2.pdf",histtitle.Data()));
			canv->Clear();

			wjets_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_ratio_noFRweight.pdf");
			canv->Print(Form("print/wjets_ratio_%s_noFRweight_v2.pdf",histtitle.Data()));
			wjetsSS_ratio[Data1]->Draw("EPSAME");
			legg->Draw("SAME");
			//canv->Print("print/wjetsBoth_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjetsBoth_ratio_%s_noFRweight_v2.pdf",histtitle.Data()));
			canv->Clear();
			wjetsSS_ratio[Data1]->SetFillColor(7);
			wjetsSS_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjetsSS_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjetsSS_ratio_%s_noFRweight_v2.pdf",histtitle.Data()));
			canv->Clear();

			double error = 0;
			wjets_template[Data1]->IntegralAndError(1,45,error);
			cout<<"QCD(template) = "<<wjets_template[Data1]->Integral(1,45)<<"+-"<<error<<endl;
			error = 0;
			wjetsSS_template[Data1]->IntegralAndError(1,45,error);
			cout<<"QCD(template) SS = "<<wjetsSS_template[Data1]->Integral(1,45)<<"+-"<<error<<endl;
			error = 0;
			wjets_ratio[Data1]->IntegralAndError(1,45,error);
			cout<<"QCD(ratio) = "<<wjets_ratio[Data1]->Integral(1,45)<<"+-"<<error<<endl;
			error = 0;
			wjetsSS_ratio[Data1]->IntegralAndError(1,45,error);
			cout<<"QCD(ratio) SS = "<<wjetsSS_ratio[Data1]->Integral(1,45)<<"+-"<<error<<endl;

			//for(int i=1; i<rapbinnum_1560+1; i++) {
			for(int i=1; i<optbinnum+1; i++) {
				if(wjets_template[Data1]->GetBinContent(i) < 0) {
					wjets_template[Data1]->SetBinContent(i,0.0);
					wjets_template[Data1]->SetBinError(i,0.0);
				}
				if(wjets_ratio[Data1]->GetBinContent(i) < 0) {
					wjets_ratio[Data1]->SetBinContent(i,0.0);
					wjets_ratio[Data1]->SetBinError(i,0.0);
				}
			}

			//###    TH1D* wjets = (TH1D*)wjets_ratio[Data1]->Clone(); // !!EC: change the default from template to ratio
			TH1D* wjets = (TH1D*)wjets_template[Data1]->Clone(); // !!EC: change the default from template to ratio
			wjets->Sumw2();
			wjets->SetName("wjets");
			/*
				TH1D* wjets_total      = new TH1D("wjets_total","",rapbinnum_1560,rapbin_1560);
				TH1D* wjets_systematic = new TH1D("wjets_systematic","",rapbinnum_1560,rapbin_1560);
				TH1D* wjets_stat       = new TH1D("wjets_stat","",rapbinnum_1560,rapbin_1560);
				*/

			TH1D* wjets_total;
			TH1D* wjets_systematic;
			TH1D* wjets_stat;

			if (optval==1) {wjets_total = new TH1D("wjets_total","",binnum,bins);wjets_systematic = new TH1D("wjets_systematic","",binnum,bins);wjets_stat = new TH1D("wjets_stat","",binnum,bins);}
			else if (optval==2) {wjets_total = new TH1D("wjets_total","",phistarnum,phistarbin);wjets_systematic = new TH1D("wjets_systematic","",phistarnum,phistarbin);wjets_stat = new TH1D("wjets_stat","",phistarnum,phistarbin);}
			else if (optval==3) {wjets_total = new TH1D("wjets_total","",ptbinnum_meas,ptbin_meas);wjets_systematic = new TH1D("wjets_systematic","",ptbinnum_meas,ptbin_meas);wjets_stat = new TH1D("wjets_stat","",ptbinnum_meas,ptbin_meas);}
			else if (optval==4) {wjets_total = new TH1D("wjets_total","",rapbinnum_60120,rapbin_60120);wjets_systematic = new TH1D("wjets_systematic","",rapbinnum_60120,rapbin_60120);wjets_stat = new TH1D("wjets_stat","",rapbinnum_60120,rapbin_60120);}
			else if (optval==5) {wjets_total = new TH1D("wjets_total","",rapbinnum_1560,rapbin_1560);wjets_systematic = new TH1D("wjets_systematic","",rapbinnum_1560,rapbin_1560);wjets_stat = new TH1D("wjets_stat","",rapbinnum_1560,rapbin_1560);}


			//for(int i=1; i<rapbinnum_1560+1; i++) {
			for(int i=1; i<optbinnum+1; i++) {
				double systematic = fabs(wjets->GetBinContent(i) - wjets_ratio[Data1]->GetBinContent(i));
				double stat = wjets->GetBinError(i);
				double total = sqrt( systematic*systematic + stat*stat );
				if(wjets->GetBinContent(i)==0) {
					systematic = 0;
					stat = 0;
					total = 0;
				}

				wjets_systematic->SetBinContent(i,systematic);
				wjets_stat->SetBinContent(i,stat);
				wjets_total->SetBinContent(i,total);

				wjets->SetBinError(i,total);
			}
			/*
				TFile* gg = new TFile("result/wjets_rap1560_noFRweight.root","RECREATE");
				wjets->Write();
				wjets_systematic->Write();
				wjets_stat->Write();
				gg->Close();
				*/
			//wjets_systematic->Divide(wjets);
			wjets_systematic->Divide(wjets);
			wjets_stat->Divide(wjets);
			wjets_total->Divide(wjets);

			wjets_total->SetMarkerStyle(20);
			wjets_total->SetMarkerSize(3);
			wjets_total->SetMarkerColor(1);

			wjets_systematic->SetMarkerStyle(22);
			wjets_systematic->SetMarkerSize(3);
			wjets_systematic->SetMarkerColor(2);

			wjets_stat->SetMarkerStyle(21);
			wjets_stat->SetMarkerSize(3);
			wjets_stat->SetMarkerColor(4);

			TLegend* leg = new TLegend(.8,.15,.95,.3);
			leg->AddEntry(wjets_total,"Total","P");
			leg->AddEntry(wjets_systematic,"Sys.","P");
			leg->AddEntry(wjets_stat,"Stat.","P");

			massFrame->GetYaxis()->SetTitle("Unceratinty");
			massFrame->SetMinimum(0);
			massFrame->SetMaximum(1);
			massFrame->GetXaxis()->SetLabelSize(0.025);
			massFrame->GetXaxis()->SetTitleSize(0.05);

			massFrame->Draw();
			wjets_total->Draw("HISTPSAME");
			wjets_systematic->Draw("HISTPSAME");
			wjets_stat->Draw("HISTPSAME");
			massFrame->Draw("AXISSAME");
			leg->Draw("SAME");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/wjets_uncertainty_rap1560_noFRweight.pdf");
			canv->Print(Form("print/wjets_uncertainty_%s_noFRweight_v2.pdf",histtitle.Data()));
			//TFile* gg = new TFile("result/wjets_rap1560_noFRweight.root","RECREATE");
			TFile* gg = new TFile(Form("result/wjets_%s_noFRweight_v2.root",histtitle.Data()),"RECREATE");
			gg->Write();
			gg->Close();
		}
		}
