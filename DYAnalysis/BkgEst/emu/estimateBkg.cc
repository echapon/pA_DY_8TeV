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

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include "../interface/tdrstyle.C"
#include "../interface/CMS_lumi.C"
#include "../interface/defs.h"

using namespace std;
using namespace DYana;

void fillSystematics( TH1D* data_driven, TH1D* stat, TH1D* systematic, TH1D* total );
void removeNegativeBins( TH1D* hist );

void estimateBkg(const char* var="mass", // var = mass | mass3bins | pt | phistar | rap1560 | rap60120 | pt1560 | phistar1560
      int syst_tt=0,                     // 0=nominal, 1/-1 = scale ttbar up/down for syst
      int syst_RR=0)                     // 0=nominal, 1=variation
{

   setTDRStyle();

    // const int binnum = 43;
    // double bins[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,  200, 220, 243, 273, 320, 380, 440, 510, 600, 700,  830, 1000, 1500, 3000};

    // the bins depend on the variable
    TString tvar(var);
    int varbinnum = nbinsvar(tvar);
    double *varbins = binsvar(tvar);

    TString tt_str("");
    if (syst_tt==1) tt_str="_ttup";
    if (syst_tt==-1) tt_str="_ttdown";
    TString RR_str("");
    if (syst_RR==1) RR_str="_RRvar";

    TFile* file[NSamples];
    for (int i=0; i<ALL; i++) file[i] = new TFile(PathHistos(static_cast<SampleTag>(i)));

    TFile* g = new TFile(Form("result/emu_%s%s%s.root",var,tt_str.Data(),RR_str.Data()),"RECREATE");

    double norm[NSamples];

    TH1D* emu[NSamples];
    TH1D* emuSS[NSamples];
    TH1D* dimu[NSamples];

    for(int i=0;i<Data1;i++) {
        SampleTag tag = static_cast<SampleTag>(i);
        double lumi = lumi_all;
        if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
           lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
        }
        norm[i] = (Xsec(tag)*lumi)/Nevts(tag);
        // cout << i << " " << Name(tag) << " " << Xsec(tag) << " " << Nevts(tag) << endl;
        // special case of ttbar: scale it up or down for syst, by 18% (total uncertainty from HIN-17-002)
        if (tag==TT) {
           if (syst_tt==1) norm[i] *= 1.18;
           if (syst_tt==-1) norm[i] /= 1.18;
        }

        emu[i] = (TH1D*)file[i]->Get(Form("emu_%s",var))->Clone("emu"+TString(Name(tag)));
        emuSS[i] = (TH1D*)file[i]->Get(Form("emuSS_%s",var))->Clone("emuSS"+TString(Name(tag)));
        dimu[i] = (TH1D*)file[i]->Get(Form("dimu_%s",var))->Clone("dimu"+TString(Name(tag)));

        emu[i]->Scale(norm[i]);
        emuSS[i]->Scale(norm[i]);
        dimu[i]->Scale(norm[i]);

        // determine the color
        EColor icolor=kBlack;
        if (tag==TT) icolor=kRed;
        else if (IsDiboson(tag)) icolor=kGreen;
        else if (IsDY(tag)) icolor=kBlue;
        emu[i]->SetFillColor(icolor);
        emuSS[i]->SetFillColor(icolor);
        dimu[i]->SetFillColor(icolor);

        emu[i]->SetStats(kFALSE);
        emuSS[i]->SetStats(kFALSE);
        dimu[i]->SetStats(kFALSE);
    } 

    TH1D* emu_data1 = (TH1D*)file[Data1]->Get(Form("emu_%s",var))->Clone("emu_data1");
    TH1D* emu_data2 = (TH1D*)file[Data2]->Get(Form("emu_%s",var))->Clone("emu_data2");
    TH1D* emu_data = (TH1D*)emu_data1->Clone("emu_data");
    emu_data->Add(emu_data2);

    TH1D* emuSS_data1 = (TH1D*)file[Data1]->Get(Form("emuSS_%s",var))->Clone("emuSS_data1");
    TH1D* emuSS_data2 = (TH1D*)file[Data2]->Get(Form("emuSS_%s",var))->Clone("emuSS_data2");
    TH1D* emuSS_data = (TH1D*)emuSS_data1->Clone("emuSS_data");
    emuSS_data->Add(emuSS_data2);

    emu_data->SetMarkerStyle(33);
    emu_data->SetMarkerSize(3);
    emu_data->SetStats(kFALSE);

    // emuSS_data->SetMarkerStyle(33);
    // emuSS_data->SetMarkerSize(3);
    // emuSS_data->SetStats(kFALSE);

    // DY M50 + M10to50
    TH1D* emu_DYtautau = emu[DYFirst];
    TH1D* emuSS_DYtautau = emuSS[DYFirst];
    TH1D* dimu_DYtautau = dimu[DYFirst];
    for (int i=DYFirst+1; i<=DYLast; i++) {
       SampleTag tag = static_cast<SampleTag>(i);
       emu_DYtautau->Add(emu[tag]);
       emuSS_DYtautau->Add(emuSS[tag]);
       dimu_DYtautau->Add(dimu[tag]);
    }

    // WW, WZ, ZZ
    TH1D* emu_diboson = emu[WW];
    TH1D* emuSS_diboson = emuSS[WW];
    TH1D* dimu_diboson = dimu[WW];
    emu_diboson->Add(emu[WZ]);
    emu_diboson->Add(emu[ZZ]);
    emuSS_diboson->Add(emuSS[WZ]);
    emuSS_diboson->Add(emuSS[ZZ]);
    dimu_diboson->Add(dimu[WZ]);
    dimu_diboson->Add(dimu[ZZ]);

    // tW + antitW
    // emu_tW->Add(emu_antitW);
    // emuSS_tW->Add(emuSS_antitW);
    // dimu_tW->Add(dimu_antitW);

    cout<<"data in emu: "<<emu_data->Integral()<<endl;
    cout<<"ttbar in emu: "<<emu[TT]->Integral()<<endl;
    cout<<"DY in emu: "<<emu_DYtautau->Integral()<<endl;
    cout<<"diboson in emu: "<<emu_diboson->Integral()<<endl;
    // cout<<"tW in emu: "<<emu_tW->Integral()<<endl;
    // cout<<"total MC in emu: "<<emu_DYtautau->Integral()+emu[TT]->Integral()+emu_diboson->Integral()+emu_tW->Integral()<<endl;
    cout<<"total MC in emu: "<<emu_DYtautau->Integral()+emu[TT]->Integral()+emu_diboson->Integral()<<endl;
    cout<<endl;
    cout<<"data in emuSS: "<<emuSS_data->Integral()<<endl;
    cout<<"ttbar in emuSS: "<<emuSS[TT]->Integral()<<endl;
    cout<<"DY in emuSS: "<<emuSS_DYtautau->Integral()<<endl;
    cout<<"diboson in emuSS: "<<emuSS_diboson->Integral()<<endl;
    // cout<<"tW in emuSS: "<<emuSS_tW->Integral()<<endl;
    cout<<endl;
    cout<<"ttbar in dimu: "<<dimu[TT]->Integral()<<endl;
    cout<<"DY in dimu: "<<dimu_DYtautau->Integral()<<endl;
    cout<<"diboson in dimu: "<<dimu_diboson->Integral()<<endl;
    // cout<<"tW in dimu: "<<dimu_tW->Integral()<<endl;
    cout<<endl;

    // remove negative bins
    removeNegativeBins( emu_DYtautau );
    removeNegativeBins( emuSS_DYtautau );
    removeNegativeBins( dimu_DYtautau );

    TH1D* emu_sumBkg = new TH1D("emu_sumBkg","",varbinnum,varbins);
    THStack* emu_stackBkg = new THStack("emu_stackBkg","");

    TH1D* emuSS_sumMC = new TH1D("emuSS_sumMC","",varbinnum,varbins);
    THStack* emuSS_stackMC = new THStack("emuSS_stackMC","");

    emu_sumBkg->Add(emu_DYtautau);	
    emu_sumBkg->Add(emu[TT]);	
    emu_sumBkg->Add(emu_diboson);	
    // emu_sumBkg->Add(emu_tW);	

    emu_stackBkg->Add(emu_diboson);
    emu_stackBkg->Add(emu_DYtautau);
    // emu_stackBkg->Add(emu_tW);
    emu_stackBkg->Add(emu[TT]);

    emuSS_sumMC->Add(emuSS_DYtautau);  
    emuSS_sumMC->Add(emuSS[TT]); 
    emuSS_sumMC->Add(emuSS_diboson); 
    // emuSS_sumMC->Add(emuSS_tW);  

    emuSS_stackMC->Add(emuSS_diboson);
    emuSS_stackMC->Add(emuSS_DYtautau);
    // emuSS_stackMC->Add(emuSS_tW);
    emuSS_stackMC->Add(emuSS[TT]);

    // Legend
    double xl=.65,yl=.55,xh=.85,yh=.89;

    if (tvar.Contains("phistar") || tvar.Contains("pt") || tvar.Contains("rap")) {xl -= 0.45; xh -= 0.45;}
    if (tvar == "pt1560") {yl -= 0.4; yh -= 0.4;}
    TLegend* legend = new TLegend(xl,yl,xh,yh);
    legend->AddEntry(emu_data,"Data");
    legend->AddEntry(emu[TT],"ttbar","F");
    // legend->AddEntry(emu_tW,"tW+#bar{t}W","F");
    legend->AddEntry(emu_DYtautau,"DY","F");
    legend->AddEntry(emu_diboson,"VV","F");
    legend->SetBorderSize(0);  

    TH1D* emu_QCD = (TH1D*)emuSS_data->Clone("emu_QCD");
    emu_QCD->Add(emuSS_DYtautau,-1.0);
    emu_QCD->Add(emuSS[TT],-1.0);
    emu_QCD->Add(emuSS_diboson,-1.0);
    // emu_QCD->Add(emuSS_tW,-1.0);
    emu_QCD->SetFillColor(7);

    double RR = 0.607; // from the formula, with the latest value of chibar
    if (syst_RR==1) RR = 0.418; // from MC, with cuts
    if (syst_RR==2) RR = 0.5365; // from MC, no cuts

    emu_QCD->Scale(1/RR);

    removeNegativeBins(emu_QCD);

    emu_sumBkg->Add(emu_QCD);
    emu_stackBkg->Add(emu_QCD);
    legend->AddEntry(emu_QCD,"QCD","F");

    TCanvas *c1 = new TCanvas();
    if (!tvar.Contains("rap")) c1->SetLogx();
    emu_data->GetYaxis()->SetTitle("Entries");
    if (tvar.Contains("mass")) emu_data->GetXaxis()->SetTitle("M_{e#mu} [GeV/c^{2}]");
    else if (tvar.Contains("pt")) emu_data->GetXaxis()->SetTitle("p_{T,e#mu} [GeV/c]");
    else if (tvar.Contains("phistar")) emu_data->GetXaxis()->SetTitle("#phi^{*}_{e#mu}");
    else emu_data->GetXaxis()->SetTitle("y_{e#mu}");
    emu_data->Draw();
    emu_data->GetYaxis()->SetRangeUser(0,1.5*emu_data->GetBinContent(emu_data->GetMaximumBin()));
    emu_stackBkg->Draw("hist same");
    emu_data->Draw("same");
    legend->Draw();
    c1->RedrawAxis();
    c1->SaveAs(Form("result/plot_%s.root",var));
    c1->SaveAs(Form("result/plot_%s.pdf",var));

    TH1D* emu_ratio = (TH1D*)emu_data->Clone("emu_ratio");
    emu_ratio->Divide(emu_data,emu_sumBkg,1.0,1.0,"B");

    TH1D* data_driven_ttbar = (TH1D*)emu_ratio->Clone("data_driven_ttbar");
    // TH1D* data_driven_tW = (TH1D*)emu_ratio->Clone("data_driven_tW");
    TH1D* data_driven_WW = (TH1D*)emu_ratio->Clone("data_driven_WW");
    TH1D* data_driven_DYtautau = (TH1D*)emu_ratio->Clone("data_driven_DYtautau");

    removeNegativeBins(data_driven_ttbar);
    // removeNegativeBins(data_driven_tW);
    removeNegativeBins(data_driven_WW);
    removeNegativeBins(data_driven_DYtautau);

    data_driven_ttbar->Multiply(dimu[TT]);
    data_driven_DYtautau->Multiply(dimu_DYtautau);
    data_driven_WW->Multiply(dimu[WW]);
    // data_driven_tW->Multiply(dimu_tW);

    // // replace the highest mass bins with MC (becasue data-driven entries are zero)
    // data_driven_ttbar->SetBinContent(43,dimu[TT]->GetBinContent(43));
    // data_driven_ttbar->SetBinError(43,dimu[TT]->GetBinError(43)); 

    TH1D* ttbar_total = new TH1D("ttbar_total","",varbinnum,varbins);
    TH1D* ttbar_systematic = new TH1D("ttbar_systematic","",varbinnum,varbins);
    TH1D* ttbar_stat = new TH1D("ttbar_stat","",varbinnum,varbins);

    // TH1D* tW_total = new TH1D("tW_total","",varbinnum,varbins);
    // TH1D* tW_systematic = new TH1D("tW_systematic","",varbinnum,varbins);
    // TH1D* tW_stat = new TH1D("tW_stat","",varbinnum,varbins);

    TH1D* WW_total = new TH1D("WW_total","",varbinnum,varbins);
    TH1D* WW_systematic = new TH1D("WW_systematic","",varbinnum,varbins);
    TH1D* WW_stat = new TH1D("WW_stat","",varbinnum,varbins);

    TH1D* DYtautau_total = new TH1D("DYtautau_total","",varbinnum,varbins);
    TH1D* DYtautau_systematic = new TH1D("DYtautau_systematic","",varbinnum,varbins);
    TH1D* DYtautau_stat = new TH1D("DYtautau_stat","",varbinnum,varbins);

    ttbar_systematic->Add(dimu[TT]);
    ttbar_systematic->Add(data_driven_ttbar,-1.0);

    DYtautau_systematic->Add(dimu_DYtautau);
    DYtautau_systematic->Add(data_driven_DYtautau,-1.0);

    // tW_systematic->Add(dimu_tW);
    // tW_systematic->Add(data_driven_tW,-1.0);

    WW_systematic->Add(dimu[WW]);
    WW_systematic->Add(data_driven_WW,-1.0);

    fillSystematics( data_driven_ttbar, ttbar_stat, ttbar_systematic, ttbar_total );
    fillSystematics( data_driven_DYtautau, DYtautau_stat, DYtautau_systematic, DYtautau_total );
    // fillSystematics( data_driven_tW, tW_stat, tW_systematic, tW_total );
    fillSystematics( data_driven_WW, WW_stat, WW_systematic, WW_total );

    data_driven_ttbar->SetName("ttbar");  
    data_driven_DYtautau->SetName("DYtautau");
    // data_driven_tW->SetName("tW");
    data_driven_WW->SetName("WW");

    dimu[TT]->SetName("ttbar_MC");  
    dimu_DYtautau->SetName("DYtautau_MC");
    // dimu_tW->SetName("tW_MC");
    dimu[WW]->SetName("WW_MC");


    data_driven_ttbar->Write();
    data_driven_DYtautau->Write();
    // data_driven_tW->Write();
    data_driven_WW->Write();

    dimu[TT]->Write();
    dimu_DYtautau->Write();
    // dimu_tW->Write();
    dimu[WW]->Write();

    ttbar_systematic->Write();
    DYtautau_systematic->Write();
    // tW_systematic->Write();
    WW_systematic->Write();

    ttbar_stat->Write();
    DYtautau_stat->Write();
    // tW_stat->Write();
    WW_stat->Write();

    g->Write();
    g->Close();

}

void fillSystematics( TH1D* data_driven, TH1D* stat, TH1D* systematic, TH1D* total ) {

    double binSystematic = 0;
    double binStat = 0;
    double binTotal = 0;

    for(int i=0; i<data_driven->GetNbinsX(); i++) {

        if(data_driven->GetBinContent(i+1)!=0) {  
            binStat = data_driven->GetBinError(i+1);
            binSystematic = fabs(systematic->GetBinContent(i+1));
            binTotal = sqrt(binSystematic*binSystematic + binStat*binStat);
        }
        else{
            binSystematic = 0;
            binStat = 0;
            binTotal = 0;
        }

        systematic->SetBinContent(i+1,binSystematic);
        stat->SetBinContent(i+1,binStat);    
        total->SetBinContent(i+1,binTotal);

        data_driven->SetBinError(i+1,binTotal);

    }

}

void removeNegativeBins( TH1D* hist ) {

    for(int i=0; i<hist->GetNbinsX(); i++) {
        if(hist->GetBinContent(i+1)<0) {
            hist->SetBinContent(i+1,0);
            hist->SetBinError(i+1,0);
        }
    }   

}
