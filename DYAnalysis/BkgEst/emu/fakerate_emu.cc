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

void removeNegativeBins( TH1D* hist );

void fakerate_emu(TString suffix) // suffix can be "_pt", "_pt_barrel", "_pt_endcap", "SS_pt", "SS_pt_barrel", "SS_pt_endcap"
{

   setTDRStyle();

    TFile* file[NSamples];
    for (int i=0; i<ALL; i++) file[i] = new TFile(PathHistos(static_cast<SampleTag>(i)));

    TFile* g = new TFile(Form("result/fakerate_emu%s.root",suffix.Data()),"RECREATE");

    double norm[NSamples];

    TH1D* numerator[NSamples];
    TH1D* denominator[NSamples];

    for(int i=0;i<Data1;i++) {
        SampleTag tag = static_cast<SampleTag>(i);
        double lumi = lumi_all;
        if (IsDYMuMu(tag)) { // special case of DYMuMu which has both beam directions
           lumi = (switcheta(tag)>0) ? lumi_part1 : lumi_part2;
        }
        norm[i] = (Xsec(tag)*lumi)/Nevts(tag);

        numerator[i] = (TH1D*)file[i]->Get("numerator" + suffix)->Clone("numerator"+TString(Name(tag)));
        denominator[i] = (TH1D*)file[i]->Get("denominator" + suffix)->Clone("numerator"+TString(Name(tag)));

        numerator[i]->Scale(norm[i]);
        denominator[i]->Scale(norm[i]);

        // determine the color
        EColor icolor=kBlack;
        if (tag==TT) icolor=kRed;
        else if (IsDiboson(tag)) icolor=kGreen;
        else if (IsDY(tag)) icolor=kBlue;
        numerator[i]->SetFillColor(icolor);
        denominator[i]->SetFillColor(icolor);

        numerator[i]->SetStats(kFALSE);
        denominator[i]->SetStats(kFALSE);
    } 

    TH1D* numerator_data1 = (TH1D*)file[Data1]->Get("numerator" + suffix)->Clone("numerator_data1");
    TH1D* numerator_data2 = (TH1D*)file[Data2]->Get("numerator" + suffix)->Clone("numerator_data1");
    TH1D* numerator_data = (TH1D*)numerator_data1->Clone("numerator_data");
    numerator_data->Add(numerator_data2);

    numerator_data->SetMarkerStyle(33);
    numerator_data->SetMarkerSize(3);
    numerator_data->SetStats(kFALSE);

    TH1D* denominator_data1 = (TH1D*)file[Data1]->Get("denominator" + suffix)->Clone("denominator_data1");
    TH1D* denominator_data2 = (TH1D*)file[Data2]->Get("denominator" + suffix)->Clone("denominator_data1");
    TH1D* denominator_data = (TH1D*)denominator_data1->Clone("denominator_data");
    denominator_data->Add(denominator_data2);

    denominator_data->SetMarkerStyle(33);
    denominator_data->SetMarkerSize(3);
    denominator_data->SetStats(kFALSE);

    // DY M50 + M10to50
    TH1D* numerator_DYtautau = numerator[DYFirst];
    TH1D* denominator_DYtautau = denominator[DYFirst];
    for (int i=DYFirst+1; i<=DYLast; i++) {
       SampleTag tag = static_cast<SampleTag>(i);
       numerator_DYtautau->Add(numerator[tag]);
       denominator_DYtautau->Add(denominator[tag]);
    }

    // WW, WZ, ZZ
    TH1D* numerator_diboson = numerator[WW];
    TH1D* denominator_diboson = denominator[WW];
    numerator_diboson->Add(numerator[WZ]);
    numerator_diboson->Add(numerator[ZZ]);
    denominator_diboson->Add(denominator[WZ]);
    denominator_diboson->Add(denominator[ZZ]);


    cout<<"data in numerator: "<<numerator_data->Integral(1,43)<<endl;
    cout<<"ttbar in numerator: "<<numerator[TT]->Integral(1,43)<<endl;
    cout<<"DY in numerator: "<<numerator_DYtautau->Integral(1,43)<<endl;
    cout<<"diboson in numerator: "<<numerator_diboson->Integral(1,43)<<endl;
    cout<<"total MC in numerator: "<<numerator_DYtautau->Integral(1,43)+numerator[TT]->Integral(1,43)+numerator_diboson->Integral(1,43)<<endl;
    cout<<endl;
    cout<<"data in denominator: "<<denominator_data->Integral(1,43)<<endl;
    cout<<"ttbar in denominator: "<<denominator[TT]->Integral(1,43)<<endl;
    cout<<"DY in denominator: "<<denominator_DYtautau->Integral(1,43)<<endl;
    cout<<"diboson in denominator: "<<denominator_diboson->Integral(1,43)<<endl;
    cout<<endl;

    // remove negative bins
    removeNegativeBins( numerator_DYtautau );
    removeNegativeBins( denominator_DYtautau );

    TH1D* numerator_QCD = (TH1D*)numerator_data->Clone("numerator_QCD");
    numerator_QCD->Add(numerator_DYtautau,-1.0);
    numerator_QCD->Add(numerator[TT],-1.0);
    numerator_QCD->Add(numerator_diboson,-1.0);
    // numerator_QCD->Add(numerator_tW,-1.0);
    numerator_QCD->SetFillColor(7);

    TH1D* denominator_QCD = (TH1D*)denominator_data->Clone("denominator_QCD");
    denominator_QCD->Add(denominator_DYtautau,-1.0);
    denominator_QCD->Add(denominator[TT],-1.0);
    denominator_QCD->Add(denominator_diboson,-1.0);
    // denominator_QCD->Add(denominator_tW,-1.0);
    denominator_QCD->SetFillColor(7);


    removeNegativeBins(numerator_QCD);
    removeNegativeBins(denominator_QCD);

    TH1D *hratio = (TH1D*) numerator_QCD->Clone("ratio");
    hratio->Divide(denominator_QCD);

    TCanvas *c1 = new TCanvas();
    c1->SetLogx();
    numerator_QCD->Draw();
    hratio->GetYaxis()->SetRangeUser(0,1);
    c1->SaveAs(Form("result/fakerate_emu%s.pdf",suffix.Data()));

    TFile *output = TFile::Open(Form("result/fakerate_emu%s.root",suffix.Data()),"RECREATE");
    numerator_QCD->Write();
    denominator_QCD->Write();
    hratio->Write();
    output->Close();
}

void removeNegativeBins( TH1D* hist ) {

    for(int i=0; i<hist->GetNbinsX(); i++) {
        if(hist->GetBinContent(i+1)<0) {
            hist->SetBinContent(i+1,0);
            hist->SetBinError(i+1,0);
        }
    }   

}

void fakerate_emu() {
   fakerate_emu("_pt");
   fakerate_emu("_pt_barrel");
   fakerate_emu("_pt_endcap");
   fakerate_emu("SS_pt");
   fakerate_emu("SS_pt_barrel");
   fakerate_emu("SS_pt_endcap");
}
