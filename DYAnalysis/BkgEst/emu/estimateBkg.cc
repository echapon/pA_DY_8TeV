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

void estimateBkg() {

    // const int binnum = 43;
    // double bins[44] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185,  200, 220, 243, 273, 320, 380, 440, 510, 600, 700,  830, 1000, 1500, 3000};

    TFile* file[NSamples];
    for (int i=0; i<ALL; i++) file[i] = new TFile(PathHistos(static_cast<SampleTag>(i)));

    double norm_part1[NSamples];
    double norm_part2[NSamples];

    TH1D* emu[NSamples];
    TH1D* emuSS[NSamples];
    TH1D* dimu[NSamples];

    for(int i=0;i<Data1;i++) {
        SampleTag tag = static_cast<SampleTag>(i);
        norm_part1[i] = (Xsec(tag)*lumi_part1)/Nevts(tag);
        norm_part2[i] = (Xsec(tag)*lumi_part2)/Nevts(tag);

        emu[i] = (TH1D*)file[i]->Get("emu_mass")->Clone("emu"+TString(Name(tag)));
        emuSS[i] = (TH1D*)file[i]->Get("emuSS_mass")->Clone("emuSS"+TString(Name(tag)));
        dimu[i] = (TH1D*)file[i]->Get("dimu_mass")->Clone("dimu"+TString(Name(tag)));

        emu[i]->Scale(norm_part1[i]);
        emuSS[i]->Scale(norm_part1[i]);
        dimu[i]->Scale(norm_part2[i]);

        emu[i]->SetFillColor(i+2);
        emuSS[i]->SetFillColor(i+2);
        dimu[i]->SetFillColor(i+2);

        emu[i]->SetStats(kFALSE);
        emuSS[i]->SetStats(kFALSE);
        dimu[i]->SetStats(kFALSE);
    } 

    TH1D* emu_data1 = (TH1D*)file[Data1]->Get("emu_mass")->Clone("emu_data1");
    TH1D* emu_data2 = (TH1D*)file[Data2]->Get("emu_mass")->Clone("emu_data2");
    TH1D* emu_data = (TH1D*)emu_data1->Clone("emu_data");
    emu_data->Add(emu_data2);

    TH1D* emuSS_data1 = (TH1D*)file[Data1]->Get("emuSS_mass")->Clone("emuSS_data1");
    TH1D* emuSS_data2 = (TH1D*)file[Data2]->Get("emuSS_mass")->Clone("emuSS_data2");
    TH1D* emuSS_data = (TH1D*)emuSS_data1->Clone("emuSS_data");
    emuSS_data->Add(emuSS_data2);

    emu_data->SetMarkerStyle(33);
    emu_data->SetMarkerSize(3);
    emu_data->SetStats(kFALSE);

    emuSS_data->SetMarkerStyle(33);
    emuSS_data->SetMarkerSize(3);
    emuSS_data->SetStats(kFALSE);

    // DY M50 + M10to50
    TH1D* emu_DYtautau = emu[DY1050];
    TH1D* emuSS_DYtautau = emu[DY1050];
    TH1D* dimu_DYtautau = emu[DY1050];
    for (int i=DY50100; i<=DY4001000; i++) {
       SampleTag tag = static_cast<SampleTag>(i);
       emu_DYtautau->Add(emu[tag]);
       emuSS_DYtautau->Add(emuSS[tag]);
       dimu_DYtautau->Add(dimu[tag]);
    }

    // WW + WZ + ZZ
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

    cout<<"data in emu: "<<emu_data->Integral(1,43)<<endl;
    cout<<"ttbar in emu: "<<emu[TT]->Integral(1,43)<<endl;
    cout<<"DY in emu: "<<emu_DYtautau->Integral(1,43)<<endl;
    cout<<"diboson in emu: "<<emu_diboson->Integral(1,43)<<endl;
    // cout<<"tW in emu: "<<emu_tW->Integral(1,43)<<endl;
    // cout<<"total MC in emu: "<<emu_DYtautau->Integral(1,43)+emu[TT]->Integral(1,43)+emu_diboson->Integral(1,43)+emu_tW->Integral(1,43)<<endl;
    cout<<"total MC in emu: "<<emu_DYtautau->Integral(1,43)+emu[TT]->Integral(1,43)+emu_diboson->Integral(1,43)<<endl;
    cout<<endl;
    cout<<"data in emuSS: "<<emuSS_data->Integral(1,43)<<endl;
    cout<<"ttbar in emuSS: "<<emuSS[TT]->Integral(1,43)<<endl;
    cout<<"DY in emuSS: "<<emuSS_DYtautau->Integral(1,43)<<endl;
    cout<<"diboson in emuSS: "<<emuSS_diboson->Integral(1,43)<<endl;
    // cout<<"tW in emuSS: "<<emuSS_tW->Integral(1,43)<<endl;
    cout<<endl;
    cout<<"ttbar in dimu: "<<dimu[TT]->Integral(1,43)<<endl;
    cout<<"DY in dimu: "<<dimu_DYtautau->Integral(1,43)<<endl;
    cout<<"diboson in dimu: "<<dimu_diboson->Integral(1,43)<<endl;
    // cout<<"tW in dimu: "<<dimu_tW->Integral(1,43)<<endl;
    cout<<endl;

    // remove negative bins
    removeNegativeBins( emu_DYtautau );
    removeNegativeBins( emuSS_DYtautau );
    removeNegativeBins( dimu_DYtautau );

    TH1D* emu_sumBkg = new TH1D("emu_sumBkg","",binnum,bins);
    THStack* emu_stackBkg = new THStack("emu_stackBkg","");

    TH1D* emuSS_sumMC = new TH1D("emuSS_sumMC","",binnum,bins);
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
    TLegend* legend = new TLegend(.65,.55,.95,.89);
    legend->AddEntry(emu_data,"Data");
    legend->AddEntry(emu[TT],"ttbar","F");
    // legend->AddEntry(emu_tW,"tW+#bar{t}W","F");
    legend->AddEntry(emu_DYtautau,"DY","F");
    legend->AddEntry(emu_diboson,"VV","F");
    legend->SetBorderSize(0);  

    TH1D* emu_QCD = (TH1D*)emuSS_data->Clone();
    emu_QCD->Add(emuSS_DYtautau,-1.0);
    emu_QCD->Add(emuSS[TT],-1.0);
    emu_QCD->Add(emuSS_diboson,-1.0);
    // emu_QCD->Add(emuSS_tW,-1.0);
    emu_QCD->SetFillColor(7);

    const double RR = 0.57147108645;
    emu_QCD->Scale(1/RR);

    removeNegativeBins(emu_QCD);

    emu_sumBkg->Add(emu_QCD);
    emu_stackBkg->Add(emu_QCD);
    legend->AddEntry(emu_QCD,"QCD","F");

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

    // replace the highest mass bins with MC (becasue data-driven entries are zero)
    data_driven_ttbar->SetBinContent(43,dimu[TT]->GetBinContent(43));
    data_driven_ttbar->SetBinError(43,dimu[TT]->GetBinError(43)); 

    TH1D* ttbar_total = new TH1D("ttbar_total","",binnum,bins);
    TH1D* ttbar_systematic = new TH1D("ttbar_systematic","",binnum,bins);
    TH1D* ttbar_stat = new TH1D("ttbar_stat","",binnum,bins);

    // TH1D* tW_total = new TH1D("tW_total","",binnum,bins);
    // TH1D* tW_systematic = new TH1D("tW_systematic","",binnum,bins);
    // TH1D* tW_stat = new TH1D("tW_stat","",binnum,bins);

    TH1D* WW_total = new TH1D("WW_total","",binnum,bins);
    TH1D* WW_systematic = new TH1D("WW_systematic","",binnum,bins);
    TH1D* WW_stat = new TH1D("WW_stat","",binnum,bins);

    TH1D* DYtautau_total = new TH1D("DYtautau_total","",binnum,bins);
    TH1D* DYtautau_systematic = new TH1D("DYtautau_systematic","",binnum,bins);
    TH1D* DYtautau_stat = new TH1D("DYtautau_stat","",binnum,bins);

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

    TFile* g = new TFile("result/emu.root","RECREATE");

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
