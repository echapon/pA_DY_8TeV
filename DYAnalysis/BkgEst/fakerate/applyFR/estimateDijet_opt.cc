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

//void estimateDijet_opt(int optval) {
void estimateDijet_opt() {
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

	 TString outtitle;

	 if (optval==1) {outtitle="mass";}
	 else if (optval==2) {outtitle="phistar";}
	 else if (optval==3) {outtitle="pt";}
	 else if (optval==4) {outtitle="rap60120";}
	 else if (optval==5) {outtitle="rap1560";}




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
	 std::cout << "############# lumi_all : " << lumi_all << std::endl;
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


    TH1D* wjets_template[NSamples+2]; // just for draw MC histograms
    TH1D* dijet_template[NSamples+2];
    TH1D* dijetSS_template[NSamples+2];
    TH1D* dijet_ratio[NSamples+2];
    TH1D* dijetSS_ratio[NSamples+2];
    double norm[NSamples+2];

    for (int i=0; i<ALL; i++) {
       SampleTag tag = static_cast<SampleTag>(i);
		 if (i==DYMuMu1030 || i==DYMuMu30) norm[i] = (Xsec(tag)*lumi_part2)/Nevts(tag);
		 else if (i==DYMuMu1030_PbP || i==DYMuMu30_PbP) norm[i] = (Xsec(tag)*lumi_part1)/Nevts(tag);
	    else if (i==Data1 || i==Data2) norm[i]=1.0;
       else norm[i] = (Xsec(tag)*lumi_all)/Nevts(tag);
       cout<< "norm[" << Name(static_cast<SampleTag>(i)) << "] = " << norm[i]<<endl;

       //dijet_template[i] = (TH1D*)f[i]->Get("rap1560Dijet1");
		 dijet_template[i] = (TH1D*)f[i]->Get(Form("%sDijet1",histtitle.Data()));
       dijet_template[i]->Scale(norm[i]);
       dijet_template[i]->SetStats(kFALSE);

       //dijetSS_template[i] = (TH1D*)f[i]->Get("rap1560SameDijet1");
		 dijetSS_template[i] = (TH1D*)f[i]->Get(Form("%sSameDijet1",histtitle.Data()));
       dijetSS_template[i]->Scale(norm[i]);
       dijetSS_template[i]->SetStats(kFALSE);

       //wjets_template[i] = (TH1D*)f[i]->Get("rap1560WJets1");
	    wjets_template[i] = (TH1D*)f[i]->Get(Form("%sWJets1",histtitle.Data()));
       wjets_template[i]->Scale(norm[i]);
       wjets_template[i]->SetStats(kFALSE);

       //dijet_ratio[i] = (TH1D*)f[i]->Get("rap1560Dijet2");
		 dijet_ratio[i] = (TH1D*)f[i]->Get(Form("%sDijet2",histtitle.Data()));
       dijet_ratio[i]->Scale(norm[i]);
       dijet_ratio[i]->SetStats(kFALSE);

       //dijetSS_ratio[i] = (TH1D*)f[i]->Get("rap1560SameDijet2");
	    dijetSS_ratio[i] = (TH1D*)f[i]->Get(Form("%sSameDijet2",histtitle.Data()));
       dijetSS_ratio[i]->Scale(norm[i]);
       dijetSS_ratio[i]->SetStats(kFALSE);

       // put histos in an array for easy style setting
       TH1D* h[5] = {dijet_template[i], dijetSS_template[i], wjets_template[i], dijet_ratio[i], dijetSS_ratio[i]};
       for (int j=0; j<5; j++) {
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
          for (int j=0; j<5; j++) {
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
          for (int j=0; j<5; j++) {
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
          for (int j=0; j<5; j++) {
             h[j]->SetFillColor(3);
          }

          if (tag != WW) {
             toadd = true;
             tagtoadd = WW;
          }
       }
       if (IsWjets(tag)) {
          // style 
          for (int j=0; j<5; j++) {
             h[j]->SetFillColor(4);
          }

          if (tag != WpMu) {
             toadd = true;
             tagtoadd = WpMu;
          }
       }
       if (tag==TT) {
          for (int j=0; j<5; j++) {
             h[j]->SetFillColor(6);
          }
       }

       if (toadd) {
          dijet_template[tagtoadd]->Add(dijet_template[i]);
          dijetSS_template[tagtoadd]->Add(dijetSS_template[i]);
          wjets_template[tagtoadd]->Add(wjets_template[i]);
          dijet_ratio[tagtoadd]->Add(dijet_ratio[i]);
          dijetSS_ratio[tagtoadd]->Add(dijetSS_ratio[i]);
       }
    } // sample loop

    cout<<"ttbar OS before = "<<dijet_template[TT]->Integral()<<endl;
    cout<<"ttbar SS before = "<<dijetSS_template[TT]->Integral()<<endl;

    cout<<"DY(template): "<<dijet_template[DYTauTau1030]->Integral()<<endl;
    cout<<"DY(ratio): "<<dijet_ratio[DYTauTau1030]->Integral()<<endl;

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

			dijet_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_template_%s_beforeremove.pdf",outtitle.Data()));

			dijet_template[Data1]->Add(dijet_template[DYTauTau1030],-1.0);

			dijet_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_template_%s_removedDY.pdf",outtitle.Data()));


			dijet_template[Data1]->Add(dijet_template[TT],-1.0);

			dijet_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_template_%s_removedTT.pdf",outtitle.Data()));


			dijet_template[Data1]->Add(dijet_template[WW],-1.0);

			dijet_template[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_template_%s_removedWWs.pdf",outtitle.Data()));

			dijet_template[DYTauTau1030]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_template_%s_DYs.pdf",outtitle.Data()));

			dijet_template[TT]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_template_%s_TTs.pdf",outtitle.Data()));

			dijet_template[WW]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_template_%s_WWs.pdf",outtitle.Data()));


///////////////////////////////////////////////////////

			dijet_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_ratio_%s_beforeremove.pdf",outtitle.Data()));

			dijet_ratio[Data1]->Add(dijet_ratio[DYTauTau1030],-1.0);

			dijet_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_ratio_%s_removedDY.pdf",outtitle.Data()));


			dijet_ratio[Data1]->Add(dijet_ratio[TT],-1.0);

			dijet_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_ratio_%s_removedTT.pdf",outtitle.Data()));


			dijet_ratio[Data1]->Add(dijet_ratio[WW],-1.0);

			dijet_ratio[Data1]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_ratio_%s_removedWWs.pdf",outtitle.Data()));

			dijet_ratio[DYTauTau1030]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_ratio_%s_DYs.pdf",outtitle.Data()));

			dijet_ratio[TT]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_ratio_%s_TTs.pdf",outtitle.Data()));

			dijet_ratio[WW]->Draw("HIST");
			CMS_lumi(canv,13,11);
			canv->Update();
			canv->RedrawAxis();
			canv->GetFrame()->Draw();
			if (optval==1) canv->SetLogx();
			//canv->Print("print/dijet_ratio_rap1560_noFRweight.pdf");
			canv->Print(Form("print/dijet_ratio_%s_WWs.pdf",outtitle.Data()));


/*
    dijet_template[Data1]->Add(dijet_template[DYTauTau1030],-1.0);
    dijet_template[Data1]->Add(dijet_template[TT],-1.0);
    dijet_template[Data1]->Add(dijet_template[WW],-1.0);

    dijet_ratio[Data1]->Add(dijet_ratio[DYTauTau1030],-1.0);
    dijet_ratio[Data1]->Add(dijet_ratio[TT],-1.0);
    dijet_ratio[Data1]->Add(dijet_ratio[WW],-1.0);
*/

    //dijet_template[5]->Smooth();
    TLegend* legg = new TLegend(.6,.65,.95,.89);
    legg->AddEntry(dijet_template[Data1],"Opposite sign", "F");
    legg->AddEntry(dijetSS_template[Data1],"Same sign", "P");

    dijet_template[Data1]->Draw("HIST");
    CMS_lumi(canv,13,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    if (optval==1) canv->SetLogx();
    //canv->Print("print/dijet_template_rap1560_noFRweight.pdf");
	 canv->Print(Form("print/dijet_template_%s.pdf",outtitle.Data()));
    dijetSS_template[Data1]->Draw("EPSAME");
    legg->Draw("SAME");
    //canv->Print("print/dijetBoth_template_rap1560_noFRweight.pdf");
	 canv->Print(Form("print/dijetBoth_template_%s.pdf",outtitle.Data()));
    canv->Clear();
    dijetSS_template[Data1]->SetFillColor(7);
    dijetSS_template[Data1]->Draw("HIST");
    CMS_lumi(canv,13,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    if (optval==1) canv->SetLogx();
    //canv->Print("print/dijetSS_template_rap1560_noFRweight.pdf");
	 canv->Print(Form("print/dijetSS_template_%s.pdf",outtitle.Data()));
    canv->Clear();

    dijet_ratio[Data1]->Draw("HIST");
    CMS_lumi(canv,13,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    if (optval==1) canv->SetLogx();
    //canv->Print("print/dijet_ratio_noFRweight.pdf");
	 canv->Print(Form("print/dijet_ratio_%s.pdf",outtitle.Data()));
    dijetSS_ratio[Data1]->Draw("EPSAME");
    legg->Draw("SAME");
    //canv->Print("print/dijetBoth_ratio_rap1560_noFRweight.pdf");
	 canv->Print(Form("print/dijetBoth_ratio_%s.pdf",outtitle.Data()));
    canv->Clear();
    dijetSS_ratio[Data1]->SetFillColor(7);
    dijetSS_ratio[Data1]->Draw("HIST");
    CMS_lumi(canv,13,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    if (optval==1) canv->SetLogx();
    //canv->Print("print/dijetSS_ratio_rap1560_noFRweight.pdf");
	 canv->Print(Form("print/dijetSS_ratio_%s.pdf",outtitle.Data()));
    canv->Clear();

    double error = 0;
    dijet_template[Data1]->IntegralAndError(1,45,error);
    cout<<"QCD(template) = "<<dijet_template[Data1]->Integral(1,45)<<"+-"<<error<<endl;
    error = 0;
    dijetSS_template[Data1]->IntegralAndError(1,45,error);
    cout<<"QCD(template) SS = "<<dijetSS_template[Data1]->Integral(1,45)<<"+-"<<error<<endl;
    error = 0;
    dijet_ratio[Data1]->IntegralAndError(1,45,error);
    cout<<"QCD(ratio) = "<<dijet_ratio[Data1]->Integral(1,45)<<"+-"<<error<<endl;
    error = 0;
    dijetSS_ratio[Data1]->IntegralAndError(1,45,error);
    cout<<"QCD(ratio) SS = "<<dijetSS_ratio[Data1]->Integral(1,45)<<"+-"<<error<<endl;

    //for(int i=1; i<rapbinnum_1560+1; i++) {
	 for(int i=1; i<optbinnum+1; i++) {
        if(dijet_template[Data1]->GetBinContent(i) < 0) {
          dijet_template[Data1]->SetBinContent(i,0.0);
          dijet_template[Data1]->SetBinError(i,0.0);
        }
        if(dijet_ratio[Data1]->GetBinContent(i) < 0) {
          dijet_ratio[Data1]->SetBinContent(i,0.0);
          dijet_ratio[Data1]->SetBinError(i,0.0);
        }
    }

//###    TH1D* dijet = (TH1D*)dijet_ratio[Data1]->Clone(); // !!EC: change the default from template to ratio
	 TH1D* dijet = (TH1D*)dijet_template[Data1]->Clone(); // !!EC: change the default from template to ratio
    dijet->Sumw2();
    dijet->SetName("dijet");
/*
    TH1D* dijet_total      = new TH1D("dijet_total","",rapbinnum_1560,rapbin_1560);
    TH1D* dijet_systematic = new TH1D("dijet_systematic","",rapbinnum_1560,rapbin_1560);
    TH1D* dijet_stat       = new TH1D("dijet_stat","",rapbinnum_1560,rapbin_1560);
*/

	 TH1D* dijet_total;
	 TH1D* dijet_systematic;
    TH1D* dijet_stat;

	 if (optval==1) {dijet_total = new TH1D("dijet_total","",binnum,bins);dijet_systematic = new TH1D("dijet_systematic","",binnum,bins);dijet_stat = new TH1D("dijet_stat","",binnum,bins);}
	 else if (optval==2) {dijet_total = new TH1D("dijet_total","",phistarnum,phistarbin);dijet_systematic = new TH1D("dijet_systematic","",phistarnum,phistarbin);dijet_stat = new TH1D("dijet_stat","",phistarnum,phistarbin);}
	 else if (optval==3) {dijet_total = new TH1D("dijet_total","",ptbinnum_meas,ptbin_meas);dijet_systematic = new TH1D("dijet_systematic","",ptbinnum_meas,ptbin_meas);dijet_stat = new TH1D("dijet_stat","",ptbinnum_meas,ptbin_meas);}
	 else if (optval==4) {dijet_total = new TH1D("dijet_total","",rapbinnum_60120,rapbin_60120);dijet_systematic = new TH1D("dijet_systematic","",rapbinnum_60120,rapbin_60120);dijet_stat = new TH1D("dijet_stat","",rapbinnum_60120,rapbin_60120);}
	 else if (optval==5) {dijet_total = new TH1D("dijet_total","",rapbinnum_1560,rapbin_1560);dijet_systematic = new TH1D("dijet_systematic","",rapbinnum_1560,rapbin_1560);dijet_stat = new TH1D("dijet_stat","",rapbinnum_1560,rapbin_1560);}


    //for(int i=1; i<rapbinnum_1560+1; i++) {
    for(int i=1; i<optbinnum+1; i++) {
        double systematic = fabs(dijet->GetBinContent(i) - dijet_ratio[Data1]->GetBinContent(i));
        double stat = dijet->GetBinError(i);
        double total = sqrt( systematic*systematic + stat*stat );
        if(dijet->GetBinContent(i)==0) {
          systematic = 0;
          stat = 0;
          total = 0;
        }

        dijet_systematic->SetBinContent(i,systematic);
        dijet_stat->SetBinContent(i,stat);
        dijet_total->SetBinContent(i,total);

        dijet->SetBinError(i,total);
    }
/*
    TFile* gg = new TFile("result/dijet_rap1560_noFRweight.root","RECREATE");
    dijet->Write();
    dijet_systematic->Write();
    dijet_stat->Write();
    gg->Close();
*/
    //wjets_systematic->Divide(wjets);
    dijet_systematic->Divide(dijet);
    dijet_stat->Divide(dijet);
    dijet_total->Divide(dijet);

    dijet_total->SetMarkerStyle(20);
    dijet_total->SetMarkerSize(3);
    dijet_total->SetMarkerColor(1);

    dijet_systematic->SetMarkerStyle(22);
    dijet_systematic->SetMarkerSize(3);
    dijet_systematic->SetMarkerColor(2);

    dijet_stat->SetMarkerStyle(21);
    dijet_stat->SetMarkerSize(3);
    dijet_stat->SetMarkerColor(4);

    TLegend* leg = new TLegend(.8,.15,.95,.3);
    leg->AddEntry(dijet_total,"Total","P");
    leg->AddEntry(dijet_systematic,"Sys.","P");
    leg->AddEntry(dijet_stat,"Stat.","P");

    massFrame->GetYaxis()->SetTitle("Unceratinty");
    massFrame->SetMinimum(0);
    massFrame->SetMaximum(1);
    massFrame->GetXaxis()->SetLabelSize(0.025);
    massFrame->GetXaxis()->SetTitleSize(0.05);

    massFrame->Draw();
    dijet_total->Draw("HISTPSAME");
    dijet_systematic->Draw("HISTPSAME");
    dijet_stat->Draw("HISTPSAME");
    massFrame->Draw("AXISSAME");
    leg->Draw("SAME");
    CMS_lumi(canv,13,11);
    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    if (optval==1) canv->SetLogx();
    //canv->Print("print/dijet_uncertainty_rap1560_noFRweight.pdf");
	 canv->Print(Form("print/dijet_uncertainty_%s.pdf",outtitle.Data()));
    //TFile* gg = new TFile("result/dijet_rap1560_noFRweight.root","RECREATE");
    //###TFile* gg = new TFile(Form("result/dijet_%s_noFRweight_v2.root",histtitle.Data()),"RECREATE");
	 TFile* gg = new TFile(Form("result/dijet_%s.root",outtitle.Data()),"RECREATE");
    gg->Write();
    gg->Close();
}
}
