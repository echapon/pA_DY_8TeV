//CompRpPb.C
#include <iostream>
#include <TGraphAsymmErrors.h>
#include <TEfficiency.h>
#include <TH1.h>
#include <TCanvas.h>
#include "../BkgEst/interface/defs.h"
#include "../Include/MyCanvas.C"

#define ARRAY_MAX 50;



TH1D *g2h(TGraphAsymmErrors *g) {
			std::cout << "g2h ----------------" << std::endl;
		  const int nbins = g->GetN();
		  double bins[nbins+1];

		  for (int i=0; i<nbins; i++) bins[i] = g->GetX()[i]-g->GetEXlow()[i];
		  bins[nbins] = g->GetX()[nbins-1]+g->GetEXhigh()[nbins-1];

		  TH1D *hans = new TH1D("hans","",nbins,bins);
		  for (int i=0; i<nbins; i++) {
					 hans->SetBinContent(i+1,g->GetY()[i]);
					 hans->SetBinError(i+1,0.5*(g->GetEYlow()[i]+g->GetEYhigh()[i]));

					 std::cout << i+1 << " - " << hans->GetBinContent(i+1) << " , " << hans->GetBinError(i+1) << std::endl;
		  }
		  return hans;
}

TH1D *g2hcor(TGraphAsymmErrors *g, TEfficiency *e) {
		  const int nbins = g->GetN();
		  double bins[nbins+1];

		  for (int i=0; i<nbins; i++) bins[i] = g->GetX()[i]-g->GetEXlow()[i];
		  bins[nbins] = g->GetX()[nbins-1]+g->GetEXhigh()[nbins-1];

		  TH1D *hans = new TH1D("hans","",nbins,bins);
		  for (int i=0; i<nbins; i++) {
					 hans->SetBinContent(i+1,g->GetY()[i]*e->GetEfficiency(i+1));
					 hans->SetBinError(i+1,0.5*(g->GetEYlow()[i]+g->GetEYhigh()[i]));

					 std::cout << i+1 << " - " << hans->GetBinContent(i+1) << " , " << hans->GetBinError(i+1) << std::endl;
		  }
		  return hans;
}


TH1D *array2h(int tempsize,double tempbin[],double tempval[],double temperr[]) {
std::cout << "######## array2h ----------------" << std::endl;
		  int temph2binnum = tempsize/sizeof(double);
		  std::cout << "####" << temph2binnum << std::endl;
		  TH1D *temph = new TH1D("temph","",temph2binnum-1,tempbin);	
		  for (int i=0;i<temph2binnum-1;i++) {
					 std::cout << i+1 << " - " << tempval[i] << " , " << temperr[i] << std::endl;
					 temph->SetBinContent(i+1,tempval[i]);
					 temph->SetBinError(i+1,temperr[i]);
		  }
		  return temph;
}

TH1D *array2h_yabs(int tempsize,double tempbin[],double tempval[],double temperr[]) {

		  int temph2binnum2 = tempsize/sizeof(double);
		  std::cout << "####" << temph2binnum2 << std::endl;

		  double tempbinyall[2*temph2binnum2-1];
		  double tempvalyall[2*temph2binnum2-2];
		  double temperryall[2*temph2binnum2-2];
		  /*
			  01234
			  ABCD

			  432101234 4 35 26 17 08 t-2 t
			  DCBAABCD 34 25 16 07 t-2 t-1 / 
			*/

		  tempbinyall[temph2binnum2-1]=tempbin[0];
		  std::cout << "tempbinyall[" << (temph2binnum2)-1 << "]=" << tempbinyall[(temph2binnum2)-1] << std::endl;

		  for (int i=0;i<temph2binnum2-1;i++) {
					 std::cout << "-----" << tempbin[i] << " " << tempval[i] << " " << temperr[i] << std::endl;
					 tempbinyall[(temph2binnum2-2)-i]=-1*tempbin[i+1];
					 tempvalyall[temph2binnum2-2-i]=tempval[i]/2;
					 temperryall[temph2binnum2-2-i]=temperr[i]/2;
					 tempbinyall[(temph2binnum2)+i]=tempbin[i+1];
					 tempvalyall[temph2binnum2-1+i]=tempval[i]/2;
					 temperryall[temph2binnum2-1+i]=temperr[i]/2;

					 std::cout << "tempvalyall[" << (temph2binnum2-2)-i << "]=" << tempvalyall[(temph2binnum2-2)-i] << std::endl;
					 std::cout << "tempvalyall[" << (temph2binnum2-1)+i << "]=" << tempvalyall[(temph2binnum2-1)+i] << std::endl;
					 std::cout << "tempbinyall[" << (temph2binnum2-2)-i << "]=" << tempbinyall[(temph2binnum2-2)-i] << std::endl;
					 std::cout << "tempbinyall[" << (temph2binnum2)+i << "]=" << tempbinyall[(temph2binnum2)+i] << std::endl;


		  }

		  int temph2binnum = sizeof(tempbinyall)/sizeof(double);
		  std::cout << "####" << temph2binnum << std::endl;

		  for (int i=0;i<temph2binnum;i++)
		  {
					 std::cout << i << "-" << tempbinyall[i] << " ";
		  }
		  std::cout << std::endl;

		  TH1D *temph = new TH1D("temph","",temph2binnum-1,tempbinyall);	
		  //			TH1D *temph = new TH1D("temph","",temph2binnum2-1,tempbin);  
		  std::cout << tempvalyall[0] << std::endl;
		  if (tempvalyall[0]<=0.1) {std::cout << "ERROR " << tempvalyall[0] << std::endl;return temph;}


		  for (int i=0;i<temph2binnum-1;i++) {
					 std::cout << i+1 << " - " << tempvalyall[i] << " , " << temperryall[i] << std::endl;
					 temph->SetBinContent(i+1,tempvalyall[i]);
					 temph->SetBinError(i+1,temperryall[i]);
		  }

		  return temph;
}

void mergehist(int tempbinnum,double tempbin[],TH1D* tempbefh,TH1D* temphist, double wt) {

		  double tempval=0.0;
		  double temperr=0.0;
		  double tempbinwidth=0;
		  int inij=0;

		  for (int i=0;i<tempbinnum;i++) {//pPb
					 int tempinij=0;
					 std::cout << "----- inij : " << inij << " >=? " << tempbefh->GetNbinsX() << std::endl; 
					 if (inij>=tempbefh->GetNbinsX()) continue;
					 std::cout << "tempbin[" << i+1 << "] : " << tempbin[i+1] << " >? " << "tempbefh->GetBinLowEdge(" << inij+1 << " ) :  " << tempbefh->GetBinLowEdge(inij+1) << std::endl;
					 //for (int j=inij;tempbin[i+1]>tempbefh->GetBinLowEdge(j+1);j++) {//pp
//					 for (int j=inij;j<tempbefh->GetNbinsX();j++) {
//							   if (tempbin[i+1]<=tempbefh->GetBinLowEdge(j+1)) continue;
					 for (int j=inij;tempbin[i+1]>tempbefh->GetBinLowEdge(j+1)+0.0000001;j++) {
								std::cout << "j : " << j << " - " << tempbin[i+1] << " >? " << tempbefh->GetBinLowEdge(j+1) << std::endl;
								std::cout << j+1 << " ------> " << tempbefh->GetBinContent(j+1) << " " << tempbefh->GetBinWidth(j+1) << " " << tempbefh->GetBinError(j+1) << std::endl;	
								tempval+=tempbefh->GetBinContent(j+1)*tempbefh->GetBinWidth(j+1);
								temperr+=tempbefh->GetBinError(j+1)*tempbefh->GetBinWidth(j+1);	
								tempbinwidth+=tempbefh->GetBinWidth(j+1);
								std::cout << "tempbinwidth : " << tempbinwidth << std::endl;
								tempinij=j;
								std::cout << "tempinij : " << tempinij << std::endl;
					 }
					std::cout << "************* tempinij : " << tempinij << std::endl;
					 if (tempbinwidth!=0.0 && tempbin[i+1]<=tempbefh->GetBinLowEdge(tempinij+1)+0.0000001) {
								std::cout << "ERROR ------------------- " << std::endl;	
								tempval+=tempbefh->GetBinContent(tempinij+1)*tempbefh->GetBinWidth(tempinij+1);
								temperr-=tempbefh->GetBinError(tempinij+1)*tempbefh->GetBinWidth(tempinij+1);	
								tempbinwidth-=tempbefh->GetBinWidth(tempinij+1);
								std::cout << tempinij+1 << " ------> " << tempbefh->GetBinContent(tempinij+1) << " " << tempbefh->GetBinWidth(tempinij+1) << " " << tempbefh->GetBinError(tempinij+1) << std::endl;	
									std::cout << "tempbinwidth : " << tempbinwidth << std::endl;
	
								tempinij-=1;	
									std::cout << "tempinij : " << tempinij << std::endl;
								std::cout << "ERROR correction ---------------------- " << std::endl;
					}
					 std::cout << tempval << " / " << tempbinwidth << " * " << wt << " = " << "tempval/tempbinwidth*wt : " << tempval/tempbinwidth*wt << std::endl;
					 temphist->SetBinContent(i+1,tempval/tempbinwidth*wt);
					 temphist->SetBinError(i+1,temperr/tempbinwidth*wt);
					 if (tempbinwidth==0.0) {std::cout << "$$$$$$$$ tempbinwidth is ZERO" << std::endl;temphist->SetBinContent(i+1,0.0);temphist->SetBinError(i+1,0.0);}
					 std::cout << i+1 << " ---" << temphist->GetBinContent(i+1) << " " << temphist->GetBinWidth(i+1) << " " << temphist->GetBinError(i+1) << std::endl;	

					 if (tempbinwidth==0.0) inij=tempinij; else inij=tempinij+1;
					 tempval=0.0;
					 temperr=0.0;
					 tempbinwidth=0;
		  }
		  return;
}


//void CompRpPb(const char* infile="results/xsec_nom.root") {
//void CompRpPb(const char* infile="results/xsec_nom_detcor_FSRcor.root") {
void CompRpPb(const char* infile="results/xsec_nom_detcor_FSR.root") {


		  TFile* finacc = TFile::Open("../ROOTFile_Histogram_Acc_Eff_MomCorr00_Powheg_PAL3Mu12_0_rewboth.root");
		  TEfficiency *TEff_Acc_Mass = (TEfficiency*)finacc->Get("TEff_Acc_Mass");
		  TEfficiency *TEff_Acc_Pt = (TEfficiency*)finacc->Get("TEff_Acc_Pt");
		  TEfficiency *TEff_Acc_Phistar = (TEfficiency*)finacc->Get("TEff_Acc_Phistar");
		  TEfficiency *TEff_Acc_Rap1560 = (TEfficiency*)finacc->Get("TEff_Acc_Rap1560");
		  TEfficiency *TEff_Acc_Rap60120 = (TEfficiency*)finacc->Get("TEff_Acc_Rap60120");

//////////////////////////////////		  



		  TFile* fin = TFile::Open(infile);

		  TGraphAsymmErrors* gaeres_mass = (TGraphAsymmErrors*)fin->Get("gres_mass");
		  TGraphAsymmErrors* gaeres_pt = (TGraphAsymmErrors*)fin->Get("gres_pt");
		  TGraphAsymmErrors* gaeres_rap1560 = (TGraphAsymmErrors*)fin->Get("gres_rap1560");
		  TGraphAsymmErrors* gaeres_rap60120 = (TGraphAsymmErrors*)fin->Get("gres_rap60120");
		  TGraphAsymmErrors* gaeres_phistar = (TGraphAsymmErrors*)fin->Get("gres_phistar");

		  // HIN-15-002, Z boson analysis in pPb collision(http://hepdata.cedar.ac.uk/view/ins1410832), 5.02 TeV - PLB 759 (2016) 36
		  double ZpPb_cross_pt_bin[14]={0.0, 2.5, 5.0, 7.5, 10.0, 12.5, 15.0, 20.0, 30.0, 40.0, 50.0, 70.0, 100.0, 150.0};
		  double ZpPb_cross_pt_val[13]={2.948,4.706,3.812,3.273,2.376,1.780,1.211,0.7593,0.3810,0.2450,0.09426,0.03765, 0.007902};
		  double ZpPb_cross_pt_staterr[13]={0.175,0.197,0.185,0.177,0.154,0.139,0.070,0.0393,0.0282,0.0239,0.00994,0.00515,0.001721};
		  double ZpPb_cross_pt_systerr[13]={0.101,0.116,0.107,0.112,0.080,0.062,0.035,0.0215,0.0139,0.0104,0.00465,0.00205,0.000560};

		  double ZpPb_cross_ycm_bin[13]={-2.8,-2.4,-2.0,-1.6,-1.2,-0.8,-0.4,0.0,0.4,0.8,1.2,1.6,2.0};
		  double ZpPb_cross_ycm_val[12]={4.004,10.68,18.29,19.44,19.61,21.03,19.42,20.14,19.39,15.31,8.984,1.850};
		  double ZpPb_cross_ycm_staterr[12]={0.465,0.77,0.97,0.99,1.02,0.99,0.96,1.01,1.00,0.91,0.710,0.342};
		  double ZpPb_cross_ycm_systerr[12]={0.097,0.20,0.31,0.39,0.54,0.46,0.49,0.56,0.46,0.41,0.218,0.052};

		  double ZpPb_rfb_absycm_bin[6]={0.0,0.4,0.8,1.2,1.6,2.0};
		  double ZpPb_rfb_absycm_val[5]={1.035,0.9222,0.7786,0.4628,0.1016};
		  double ZpPb_rfb_absycm_staterr[5]={0.073,0.0646,0.0610,0.0436,0.0195};
		  double ZpPb_rfb_absycm_systerr[5]={0.040,0.0300,0.0308,0.0146,0.0033};

		  //### SMP-14-003 : Drell-Yan 8 TeV (http://hepdata.cedar.ac.uk/view/ins1332509)
		  //mass - unit : pb
/*		  double DYpp_cross_mass_bin[]={15.0,20.0,25.0,30.0,35.0,40.0,45.0,50.0,55.0,60.0,64.0,68.0,72.0,76.0,81.0,86.0,91.0,96.0,101.0,106.0,110.0,115.0,120.0,126.0,133.0,141.0,150.0,160.0,171.0,185.0,200.0,220.0,243.0,273.0,320.0,380.0,440.0,510.0,600.0,1000.0,1500.0,2000.0};
		  double DYpp_cross_mass_val[]={184.5,75.0,35.9,19.72,11.81,7.32,4.97,3.591,2.750,2.342,2.177,2.243,2.623,3.889,9.31,85.7,103.1,8.71,2.961,1.552,0.983,0.649,0.4453,0.3101,0.2135,0.1501,0.1061,0.0738,0.0536,0.0351,0.02404,0.01532,0.00909,0.00491,0.00221, 0.001034, 0.000476, 0.000221, 0.0000471, 0.00000309, 0.00000029};
		  double DYpp_cross_mass_err[]={8.8,3.2,1.5,0.81,0.39,0.22,0.14,0.094,0.068,0.056,0.049,0.049,0.055,0.075,0.20,1.4,1.6,0.17,0.053,0.029,0.019,0.014,0.0100,0.0075,0.0056,0.0043,0.0033,0.0025,0.0019,0.0014,0.00098,0.00067,0.00043,0.00025,0.00012,0.000071,0.000038,0.000021,0.0000048,0.00000084,0.00000015};	
*/
		  double DYpp_cross_mass_bin[]={15.0,20.0,25.0,30.0,35.0,40.0,45.0,50.0,55.0,60.0,64.0,68.0,72.0,76.0,81.0,86.0,91.0,96.0,101.0,106.0,110.0,115.0,120.0,126.0,133.0,141.0,150.0,160.0,171.0,185.0,200.0,220.0,243.0,273.0,320.0,380.0,440.0,510.0,600.0};
		  double DYpp_cross_mass_val[]={184.5,75.0,35.9,19.72,11.81,7.32,4.97,3.591,2.750,2.342,2.177,2.243,2.623,3.889,9.31,85.7,103.1,8.71,2.961,1.552,0.983,0.649,0.4453,0.3101,0.2135,0.1501,0.1061,0.0738,0.0536,0.0351,0.02404,0.01532,0.00909,0.00491,0.00221, 0.001034, 0.000476, 0.000221};
		  double DYpp_cross_mass_err[]={8.8,3.2,1.5,0.81,0.39,0.22,0.14,0.094,0.068,0.056,0.049,0.049,0.055,0.075,0.20,1.4,1.6,0.17,0.053,0.029,0.019,0.014,0.0100,0.0075,0.0056,0.0043,0.0033,0.0025,0.0019,0.0014,0.00098,0.00067,0.00043,0.00025,0.00012,0.000071,0.000038,0.000021};	

		  //yrap, Mass : 60-120 unit : pb-1
		  double DYpp_cross_rap60120_bin[]={0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4};
		  double DYpp_cross_rap60120_val[]={339.0,338.0,336.2,335.10,334.63,331.38,329.38,325.00,320.23,312.08,300.68,287.81,273.00,256.66,236.71,216.40,192.85,167.84,142.33,116.38,88.29,62.37,36.93,12.37};
		  double DYpp_cross_rap60120_err[]={1.2,1.2,1.1,0.96,0.93,0.93,0.91,0.86,0.83,0.85,0.81,0.77,0.74,0.74,0.72,0.71,0.73,0.68,0.61,0.54,0.52,0.41,0.30,0.18};

			  //### SMP-14-012 : Z boson 8 TeV (https://www.hepdata.net/record/77221)
		  //mass - unit : pb
		  double DYpp_cross_pt60120_bin[]={0.0,2.5,5.0,7.5,10.0,12.5,15.0,17.5,20.0,30.0,40.0,50.0,70.0,90.0,110.0,150.0,190.0,250.0,600.0};
		  double DYpp_cross_pt60120_val[]={184.5,75.0,35.9,19.72,11.81,7.32,4.97,3.591,2.750,2.342,2.177,2.243,2.623,3.889,9.31,85.7,103.1,8.71,2.961,1.552,0.983,0.649,0.4453,0.3101,0.2135,0.1501,0.1061,0.0738,0.0536,0.0351,0.02404,0.01532,0.00909,0.00491,0.00221, 0.001034, 0.000476, 0.000221, 0.0000471, 0.00000309, 0.00000029};
		  double DYpp_cross_pt60120_err[]={8.8,3.2,1.5,0.81,0.39,0.22,0.14,0.094,0.068,0.056,0.049,0.049,0.055,0.075,0.20,1.4,1.6,0.17,0.053,0.029,0.019,0.014,0.0100,0.0075,0.0056,0.0043,0.0033,0.0025,0.0019,0.0014,0.00098,0.00067,0.00043,0.00025,0.00012,0.000071,0.000038,0.000021,0.0000048,0.00000084,0.00000015};	

			  //mass - unit : nb
	
		  double DYpp_cross_pt60120_val2[]={3.34*0.44,5.53*0.44,5.19*0.44,3.86*0.44,3.55*0.44,2.41*0.44,1.72*0.44,1.17*0.44,0.651*0.44,0.402*0.44,0.216*0.44,0.0889*0.44,0.0410*0.44,0.0165*0.44,0.00765*0.44,0.000898*0.44,0.000444*0.44};
		  double DYpp_cross_pt60120_err2[]={0.21*0.44,0.26*0.44,0.25*0.44,0.23*0.44,0.22*0.44,0.19*0.44,0.17*0.44,0.15*0.44,0.05*0.44,0.036*0.44,0.029*0.44,0.014*0.44,0.0096*0.44,0.0066*0.44,0.0028*0.44,0.00189*0.44,0.000627*0.44,0.000196*0.44};

			double DYpp_cross_pt60120_inclval = 0.44;
	  
		  // SMP-13-013 : here are 81.0-101.0, need to check mass range 60-120? 2D, http://hepdata.cedar.ac.uk/view/ins1359450
		  // pb/GeV
		  double cross_absyrap_ptbin[]={0.0,20.0,40.0,60.0,80.0,100.0,120.0,140.0,170.0,200.0,1000.0};
		  double cross_absyrap_0p0_0p4_ptval[]={9.865,2.917,1.074,0.4605,0.2226,0.1098,0.06184,0.03019,0.01355,0.0006181};
		  double cross_absyrap_0p0_0p4_pterr[]={0.281,0.083,0.032,0.0138,0.0071,0.0036,0.00214,0.00108,0.00056,0.0000236};
		  double cross_absyrap_0p4_0p8_ptval[]={9.862,2.863,1.047,0.4461,0.2145,0.1100,0.05813,0.02985,0.01376,0.0006122};
		  double cross_absyrap_0p4_0p8_pterr[]={0.281,0.082,0.031,0.0135,0.0068,0.0036,0.00203,0.00107,0.00056,0.0000230};
		  double cross_absyrap_0p8_1p2_ptval[]={9.205,2.588,0.9355,0.4163,0.1990,0.1031,0.05406,0.02845,0.01364,0.0005521};
		  double cross_absyrap_0p8_1p2_pterr[]={0.263,0.074,0.0280,0.0127,0.0064,0.0034,0.00193,0.00104,0.00059,0.0000221};
		  double cross_absyrap_1p2_1p6_ptval[]={6.893,1.933,0.7195,0.3207,0.1611,0.08462,0.04757,0.02413,0.01130,0.0004657};
		  double cross_absyrap_1p2_1p6_pterr[]={0.197,0.056,0.0221,0.0101,0.0053,0.00293,0.00176,0.00093,0.00053,0.0000197};
		  double cross_absyrap_1p6_2p0_ptval[]={3.709,1.021,0.3816,0.1766,0.09049,0.04740,0.02838,0.01413,0.007001,0.0002605};
		  double cross_absyrap_1p6_2p0_pterr[]={0.106,0.030,0.0122,0.0059,0.00328,0.00186,0.00121,0.00064,0.000400,0.0000130};

		  // SMP-17-002 : use AN-14-107	v6, Table 39, Muon channel unit in pb * 10^3
		  double DYpp_cross_phistar_bin[]={0.000,0.004,0.008,0.012,0.016,0.020,0.024,0.029,0.034,0.039,0.045,0.051,0.057,0.064,0.072,0.081,0.091,0.102,0.114,0.128,0.145,0.165,0.189,0.219,0.258,0.312,0.391,0.524,0.695,0.918,1.153,1.496,1.947,2.522,3.277};
		  double DYpp_cross_phistar_val[]={4.437,4.400,4.306,4.183,4.045,3.884,3.697,3.470,3.249,3.033,2.788,2.563,2.362,2.143,1.922,1.702,1.505,1.324,1.151,0.983,0.823,0.675,0.539,0.4151,0.3023,0.2031,0.1179,0.0617,0.0309,0.01607,0.00826,0.00397,0.00191,0.000992};
		  double DYpp_cross_phistar_staterr[]={0.010,0.010,0.010,0.010,0.009,0.009,0.008,0.008,0.007,0.007,0.006,0.006,0.005,0.005,0.004,0.004,0.003,0.003,0.003,0.02,0.02,0.01,0.01,0.009,0.007,0.004,0.003,0.002,0.001,0.0007,0.00004,0.00003,0.00002,0.000001};
		  double DYpp_cross_phistar_systerr[]={0.067,0.066,0.065,0.063,0.062,0.059,0.056,0.052,0.049,0.046,0.042,0.038,0.036,0.033,0.029,0.026,0.023,0.020,0.017,0.015,0.012,0.010,0.008,0.0063,0.0045,0.0031,0.0018,0.0009,0.0005,0.00025,0.00013,0.00007,0.00003,0.000019};
		  double DYpp_cross_phistar_lumierr[]={0.067,0.066,0.065,0.063,0.062,0.059,0.056,0.052,0.049,0.046,0.042,0.038,0.036,0.033,0.029,0.026,0.023,0.020,0.017,0.015,0.012,0.010,0.008,0.0063,0.0045,0.0031,0.0018,0.0009,0.0005,0.00025,0.00013,0.00007,0.00003,0.000019};

		  /*
		  //###################
		  // mass
		  const int binnum = 13;
		  double bins[14] = {15,20,30,40,50,60,76,86,96,106,120,150,200,600};
		  // pt
		  const int ptbinnum_endcap = 10;
		  double ptbin_endcap[ptbinnum_endcap+1] = {15,20,25,30,35,40,50,70,100,200,500};
		  const int ptbinnum = 13;
		  double ptbin[ptbinnum+1] = {15,17,20,22,25,30,35,40,45,50,70,100,200,500};
		  const int etabinnum = 2;
		  double etabin[etabinnum+1] = {0,1.2,2.4};
		  // pt for measurement 60-120
		  const int ptbinnum_meas = 21;
		  double ptbin_meas[ptbinnum_meas+1] = {0,2,4,6,8,10,12,14,16,18,20,25,30,40,45,50,60,70,85,100,125,200};
		  // rapidity binning
		  const int rapbinnum_1560 = 12;
		  double rapbin_1560[rapbinnum_1560+1] = {-2.87,-2.4,-1.93,-1.6,-1.2,-0.8,-0.4,0,0.4,0.8,1.2,1.6,1.93};
		  const int rapbinnum_60120 = 24;
		  double rapbin_60120[rapbinnum_60120+1] = {-2.87,-2.6,-2.4,-2.2,-1.93,-1.8,-1.6,-1.4,-1.2,-1.,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.,1.2,1.4,1.6,1.8,1.93};
		  // phi* binning
		  const int phistarnum = 10;
		  double phistarbin[phistarnum+1] = {0, 0.01, 0.03, 0.05, 0.07, 0.1, 0.15, 0.2, 0.3, 0.5, 3.};
		  //####################		
			*/

		  //const int DYComp_cross_mass_binnum=14;
		  //double DYComp_cross_mass_bin[DYComp_cross_mass_binnum+1]={15.0,20.0,30.0,40.0,50.0,60.0,76.0,86.0,96.0,106.0,120.0,150.0,200.0,600.0,2000.0};
		  const int DYComp_cross_mass_binnum=13;
		  double DYComp_cross_mass_bin[DYComp_cross_mass_binnum+1]={15.0,20.0,30.0,40.0,50.0,60.0,76.0,86.0,96.0,106.0,120.0,150.0,200.0,600.0};
		  //const int DYComp_cross_pt_binnum=7;
		  //double DYComp_cross_pt_bin[DYComp_cross_pt_binnum+1] = {0.0,10.0,20.0,30.0,40.0,50.0,70.0,150.0};
		  const int DYComp_cross_pt_binnum=12;
		  double DYComp_cross_pt_bin[DYComp_cross_pt_binnum+1] = {0.0,2.5,5.0,7.5,10.0,12.5,15.0,20.0,30.0,40.0,50.0,70.0,150.0};


		  //const int DYComp_cross_2rap60120_binnum=11;
		  //double DYComp_cross_2rap60120_bin[DYComp_cross_2rap60120_binnum+1] = {-2.87,-2.0,-1.6,-1.2,-0.8,-0.4,0.0,0.4,0.8,1.2,1.6,2.0};
		  const int DYComp_cross_2rap60120_binnum=9;
		  double DYComp_cross_2rap60120_bin[DYComp_cross_2rap60120_binnum+1] = {-2.0,-1.6,-1.2,-0.8,-0.4,0.0,0.4,0.8,1.2,1.6};


		  //###const int DYComp_cross_rap60120_binnum=9;
		  //###double DYComp_cross_rap60120_bin[DYComp_cross_rap60120_binnum+1] = {-2.4,-1.2,-0.8,-0.4,0.0,0.4,0.8,1.2,1.6,2.0};
		  //const int DYComp_cross_rap60120_binnum=22;
		  //double DYComp_cross_rap60120_bin[DYComp_cross_rap60120_binnum+1] = {-2.87,-2.4,-2.2,-1.8,-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0};
		  const int DYComp_cross_rap60120_binnum=20;
		  double DYComp_cross_rap60120_bin[DYComp_cross_rap60120_binnum+1] = {-2.4,-2.2,-1.8,-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8};



		  //###const int DYComp_cross_rap60120_binnum=21;
		  //###double DYComp_cross_rap60120_bin[DYComp_cross_rap60120_binnum+1] = {-2.4,-2.2,-1.8,-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0};
		  //###const int DYComp_cross_rap60120_binnum=21;
		  //###double DYComp_cross_rap60120_bin[DYComp_cross_rap60120_binnum+1] = {-2.4,-2.2,-1.8,-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0};
		  const int DYComp_cross_phistar_binnum=17;
		  double DYComp_cross_phistar_bin[DYComp_cross_phistar_binnum+1] = {0.000,0.008,0.016,0.024,0.034,0.045,0.057,0.072,0.091,0.114,0.145,0.189,0.258,0.391,0.695,1.153,1.947,3.277};

//##################################################################
/*		  // HIN-15-002, Z boson analysis in pPb collision(http://hepdata.cedar.ac.uk/view/ins1410832), 5.02 TeV
		  double ZpPb_cross_ycm_bin[13]={-2.8,-2.4,-2.0,-1.6,-1.2,-0.8,-0.4,0.0,0.4,0.8,1.2,1.6,2.0};
		  double ZpPb_rfb_absycm_bin[6]={0.0,0.4,0.8,1.2,1.6,2.0};

		  //yrap, Mass : 60-120 unit : pb-1
		  double DYpp_cross_rap60120_bin[]={0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4};
		  
		  // SMP-13-013 : here are 81.0-101.0, need to check mass range 60-120? 2D, http://hepdata.cedar.ac.uk/view/ins1359450
		  // pb/GeV
		  double cross_absyrap_ptbin[]={0.0,20.0,40.0,60.0,80.0,100.0,120.0,140.0,170.0,200.0,1000.0};
*/
//############################################################################

		  //		  const int DYComp_cross_phistar_binnum=10;
		  //		  double DYComp_cross_phistar_bin[DYComp_cross_phistar_binnum+1] = {0.0,0.01,0.03,0.05,0.07,0.1,0.15,0.2,0.3,0.5,3.0};


		  /*
			  double ptbin[]={0.0,10.0,20.0,30.0,40.0,50.0,70.0,100.0,200.0};
			  double cross_ybin[13]={-2.8,-2.4,-2.0,-1.6,-1.2,-0.8,-0.4,0.0,0.4,0.8,1.2,1.6,2.0};
			  double rfb_absycmbin[6]={0.0,0.4,0.8,1.2,1.6,2.0};
			*/	

		  // 1. pPb vs pp (whole mass)

		  TH1D *temph1 = g2h(gaeres_mass);
		  //TH1D *temph1 = g2hcor(gaeres_mass, TEff_Acc_Mass);


		  TH1D *temph2 = array2h(sizeof(DYpp_cross_mass_bin),DYpp_cross_mass_bin,DYpp_cross_mass_val,DYpp_cross_mass_err);

		  TH1D* h1DYpPb_cross_mass_rebin=new TH1D("h1DYpPb_cross_mass_rebin","",DYComp_cross_mass_binnum,DYComp_cross_mass_bin);
		  TH1D* h2DYpPb_cross_mass_rebin=new TH1D("h2DYpPb_cross_mass_rebin","",DYComp_cross_mass_binnum,DYComp_cross_mass_bin);

		  mergehist(DYComp_cross_mass_binnum,DYComp_cross_mass_bin,temph2,h2DYpPb_cross_mass_rebin,208.0*0.001);//pp*208
		  mergehist(DYComp_cross_mass_binnum,DYComp_cross_mass_bin,temph1,h1DYpPb_cross_mass_rebin,1.0);

		  for (int i=0;i<DYComp_cross_mass_binnum;i++) {
					 std::cout << i+1 << " - " << h1DYpPb_cross_mass_rebin->GetBinContent(i+1) << " , " << h1DYpPb_cross_mass_rebin->GetBinError(i+1) << std::endl;
					 std::cout << i+1 << " - " << h2DYpPb_cross_mass_rebin->GetBinContent(i+1) << " , " << h2DYpPb_cross_mass_rebin->GetBinError(i+1) << std::endl;
		  }

		  TH1D *hCompDYpPb_cross_mass_rebin=new TH1D("hCompDYpPb_cross_mass_rebin","",DYComp_cross_mass_binnum,DYComp_cross_mass_bin);
		  hCompDYpPb_cross_mass_rebin->Sumw2();
		  hCompDYpPb_cross_mass_rebin->Divide(h1DYpPb_cross_mass_rebin,h2DYpPb_cross_mass_rebin,1,1,"");

		  std::vector<TH1D*> vh;
		  std::vector<TString> vhistnames;
		  std::vector<TH1D*> vrh;
		  std::vector<TString> vrhistnames;
		  std::vector<bool> vrisFillbool;



		  temph2->Scale(208.0*0.001);

		  MyCanvas c11("CompRpPb_cross_mass_oribin_detcor_FSRcor","M (GeV)","d#sigma/dM (nb/GeV)",800,800);
/*		  vh.push_back(temph1);
		  vh.push_back(temph2);
		  vhistnames.push_back("#splitline{DY in pPb@8.16 TeV}{#splitline{-2.87 < |y_{CM}| < 1.93}{p_{T, leading lepton} > 15 GeV, p_{T, trailing lepton} > 10 GeV}}");
		  vhistnames.push_back("#splitline{208#timesDY in pp@8 TeV, EPJC 75 (2015) 147}{|#eta| < 2.4, p_{T, leading lepton} > 20 GeV, p_{T, trailing lepton} > 10 GeV}");
*/
		  vh.push_back(temph1);
		  vh.push_back(temph1);
//		  vh.push_back(temph1);
		  vh.push_back(temph2);
//		  vh.push_back(temph2);
//		  vh.push_back(temph2);
		  vhistnames.push_back("DY in pPb@8.16 TeV");
        vhistnames.push_back("-2.87 < |y_{CM}| < 1.93");
//		  vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
//		  vhistnames.push_back("p_{T, leading lepton} > 15 GeV, p_{T, trailing lepton} > 10 GeV");
		  vhistnames.push_back("208#timesDY in pp@8 TeV, EPJC 75 (2015) 147");
//		  vhistnames.push_back("|#eta| < 2.4");
//		  vhistnames.push_back("p_{T, leading lepton} > 20 GeV, p_{T, trailing lepton} > 10 GeV");
		  //c11.isLogX=kTRUE;
		  c11.LowerEdge_X=10.0;
		  c11.UpperEdge_X=600.0;
		  //c11.isSetNoExpo_MoreLogLabels_X=kFALSE;
		  c11.isLogY=kTRUE;
		  c11.TitleX="M (GeV)";
		  c11.TitleY="d#sigma/dM (nb/GeV)";
		  c11.LowerEdge_Y=0.00003;
		  c11.UpperEdge_Y=500.0;
		  c11.Legend_x1=0.35;//0.50
		  c11.Legend_y1=0.64;//0.80
		  c11.Legend_x2=0.55;//0.95
		  c11.Legend_y2=0.81;
		  c11.LegendTextSize=0.03;
		  c11.LegendFontStyle=42;

		  c11.CanvasWithMultipleHistograms(vh, vhistnames, "PE");
		  c11.PrintCanvas();

		  std::cout << "############### c11 is done" << std::endl;

		  MyCanvas c12("CompRpPb_cross_mass_rebin_detcor_FSRcor","M (GeV)","d#sigma/dM (nb/GeV)",800,1000);
 		  vrh.push_back(temph1);
		  vrh.push_back(temph1);
//		  vrh.push_back(temph1);
		  vrh.push_back(temph2);
//		  vrh.push_back(temph2);
//		  vrh.push_back(temph2);
		  vrhistnames.push_back("DY in pPb@8.16 TeV, -2.87 < |y_{CM}| < 1.93");
		  vrhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
//		  vrhistnames.push_back("p_{T, leading lepton} > 15 GeV, p_{T, trailing lepton} > 10 GeV");
		  vrhistnames.push_back("208#timesDY in pp@8 TeV, EPJC 75 (2015) 147");
//		  vrhistnames.push_back("|#eta| < 2.4");
//		  vrhistnames.push_back("p_{T, leading lepton} > 20 GeV, p_{T, trailing lepton} > 10 GeV");
			vrisFillbool.push_back(kFALSE);
			vrisFillbool.push_back(kFALSE);
 
 	  //c12.isLogX=kTRUE;
		  c12.LowerEdge_X=10.0;
		  c12.UpperEdge_X=600.0;
		  //c12.isSetNoExpo_MoreLogLabels_X=kFALSE;
		  c12.isLogY=kTRUE;
	     c12.isRatioPadAttached=kTRUE;
		  c12.TitleX="M (GeV)";
		  c12.TitleY="d#sigma/dM (nb/GeV)";
		  c12.LowerEdge_Y=0.00003;
		  c12.UpperEdge_Y=500.0;
		  c12.Legend_x1=0.35;//0.50
		  c12.Legend_y1=0.62;//0.80
		  c12.Legend_x2=0.55;//0.95
		  c12.Legend_y2=0.81;//0.90
		  c12.LegendTextSize=0.033;
		  c12.LegendFontStyle=42;

		  c12.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_mass_rebin, h2DYpPb_cross_mass_rebin,	"#splitline{DY in pPb@8.16 TeV}{-2.87 < |y_{CM}| < 1.93}", "208#timesDY in pp@8 TeV, EPJC 75 (2015) 147", "#frac{pPb(DY)}{208#timespp(DY)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
//		  c12.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_mass_rebin, h2DYpPb_cross_mass_rebin,	"#splitline{DY in pPb@8.16 TeV}{-2.87 < |y_{CM}| < 1.93}", "#splitline{208#timesDY in pp@8 TeV, EPJC 75 (2015) 147}{|#eta| < 2.4}", "#frac{pPb(DY)}{208#timespp(DY)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
//		  c12.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_mass_rebin, h2DYpPb_cross_mass_rebin,	"#splitline{DY in pPb@8.16 TeV}{#splitline{-2.87 < |y_{CM}| < 1.93}{|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV}}", "208#timesDY in pp@8 TeV, EPJC 75 (2015) 147", "#frac{pPb(DY)}{208#timespp(DY)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");


		  //c12.CanvasWithHistogramsRatioPlotv2(vrh, vrhistnames, vrisFillbool, "PE");
	
		  c12.PrintCanvas();

		  temph1->Clear();
		  temph2->Clear();

		  vh.clear();
		  vhistnames.clear();
		  std::cout << "********** CHECK CLEAR" << std::endl;
		  vh.empty();
		  vhistnames.empty();

		  // 2. pPb DY vs pp DY (rap60120)

		  temph1 = g2h(gaeres_rap60120);
		  //temph1 = g2hcor(gaeres_rap60120, TEff_Acc_Rap60120);
		  temph2 = array2h_yabs(sizeof(DYpp_cross_rap60120_bin),DYpp_cross_rap60120_bin,DYpp_cross_rap60120_val,DYpp_cross_rap60120_err);

		  TH1D* h1DYpPb_cross_rap60120_rebin=new TH1D("h1DYpPb_cross_rap60120_rebin","",DYComp_cross_rap60120_binnum,DYComp_cross_rap60120_bin);
		  TH1D* h2DYpPb_cross_rap60120_rebin=new TH1D("h2DYpPb_cross_rap60120_rebin","",DYComp_cross_rap60120_binnum,DYComp_cross_rap60120_bin);

		  mergehist(DYComp_cross_rap60120_binnum,DYComp_cross_rap60120_bin,temph2,h2DYpPb_cross_rap60120_rebin,208.0*0.001);//pp*208
		  std::cout << "1111111111111111111111111" << std::endl;

		  mergehist(DYComp_cross_rap60120_binnum,DYComp_cross_rap60120_bin,temph1,h1DYpPb_cross_rap60120_rebin,1.0);

		  for (int i=0;i<DYComp_cross_rap60120_binnum;i++) {
					 std::cout << i+1 << " - " << h1DYpPb_cross_rap60120_rebin->GetBinContent(i+1) << " , " << h1DYpPb_cross_rap60120_rebin->GetBinError(i+1) << std::endl;
					 std::cout << i+1 << " - " << h2DYpPb_cross_rap60120_rebin->GetBinContent(i+1) << " , " << h2DYpPb_cross_rap60120_rebin->GetBinError(i+1) << std::endl;
		  }

		  TH1D *hCompDYpPb_cross_rap60120_rebin=new TH1D("hCompDYpPb_cross_rap60120_rebin","",DYComp_cross_rap60120_binnum,DYComp_cross_rap60120_bin);
		  hCompDYpPb_cross_rap60120_rebin->Sumw2();
		  hCompDYpPb_cross_rap60120_rebin->Divide(h1DYpPb_cross_rap60120_rebin,h2DYpPb_cross_rap60120_rebin,1,1,"");

//		  std::vector<TH1D*> vh2;
//		  std::vector<TString> vhistnames2;

		  temph2->Scale(208.0*0.001);

		  MyCanvas c21("CompRpPb_cross_rap60120_oribin_detcor_FSRcor","y_{CM}","d#sigma/dy_{CM} (nb)",800,800);
		  vh.push_back(temph1);
//		  vh.push_back(temph1);
		  vh.push_back(temph2);
		  vh.push_back(temph2);
        //vh.push_back(temph2);
		  vhistnames.push_back("60 < M < 120 in pPb@8.16 TeV");
//        vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
		  vhistnames.push_back("208#timesZ in pp@8 TeV, EPJC 75 (2015) 147");
		  //vhistnames.push_back("p_{T}^{leading} > 20 GeV, p_{T}^{trailing} > 10 GeV");
		  //vhistnames.push_back("|#eta^{#mu}| < 2.1");
		  vhistnames.push_back("|#eta^{#mu}| < 2.1, p_{T}^{#mu} > 20 (10) GeV");

	     c21.isLogX=kFALSE;
        c21.isLogY=kTRUE;
		  c21.LowerEdge_Y=0.7;//4.0
		  c21.UpperEdge_Y=800.0;//200.0
		  c21.Legend_x1=0.35;
		  c21.Legend_y1=0.64;
		  c21.Legend_x2=0.55;
		  c21.Legend_y2=0.81;
        c21.LegendTextSize=0.03;
		  c21.LegendFontStyle=42;
	     c21.CanvasWithMultipleHistograms(vh, vhistnames, "PE");
	     c21.PrintCanvas();

		  MyCanvas c22("CompRpPb_cross_rap60120_rebin_detcor_FSRcor","y_{CM}","d#sigma/dy_{CM} (nb)",800,1000);
	     c22.isLogX=kFALSE;
        c22.isLogY=kTRUE;
		  c22.isRatioPadAttached=kTRUE;
		  c22.TitleX="y_{CM}";
		  c22.TitleY="d#sigma/dy_{CM} (nb)";
        c22.RatioStandard=1.0;
		  c22.LowerEdge_Ratio=0.0;
		  c22.UpperEdge_Ratio=2.0;
		  c22.LowerEdge_Y=0.7;//4.0
		  c22.UpperEdge_Y=800.0;//200.0
		  c22.Legend_x1=0.35;
		  c22.Legend_y1=0.62;
		  c22.Legend_x2=0.55;
		  c22.Legend_y2=0.81;
		  c22.LegendTextSize=0.033;
		  c22.LegendFontStyle=42;

		  c22.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_rap60120_rebin, h2DYpPb_cross_rap60120_rebin, "60 < M < 120 in pPb@8.16 TeV", "#splitline{208#timesZ in pp@8 TeV, EPJC 75 (2015) 147}{|#eta^{#mu}| < 2.1, p_{T}^{#mu} > 20 (10) GeV}", "#frac{pPb(DY)}{208#timespp(DY)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
//		  c22.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_rap60120_rebin, h2DYpPb_cross_rap60120_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV}", "#splitline{208#timesZ in pp@8 TeV, EPJC 75 (2015) 147}{|#eta^{#mu}| < 2.1, p_{T}^{#mu} > 20 (10) GeV}", "#frac{pPb(DY)}{208#timespp(DY)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");


		  c22.PrintCanvas();

		  temph1->Clear();
		  temph2->Clear();

		  vh.clear();
		  vhistnames.clear();
		  std::cout << "********** CHECK CLEAR" << std::endl;
		  vh.empty();
		  vhistnames.empty();


		  // 3. pPb DY vs pPb Z (pt)

		  TCanvas* c1= new TCanvas("c1","",800,800);

		  temph1 = g2h(gaeres_pt);//
		  //temph1 = g2hcor(gaeres_pt, TEff_Acc_Pt);
		  temph2 = array2h(sizeof(ZpPb_cross_pt_bin),ZpPb_cross_pt_bin,ZpPb_cross_pt_val,ZpPb_cross_pt_staterr);//pb-1

		  TH1D* h1DYpPb_cross_pt_rebin=new TH1D("h1DYpPb_cross_pt_rebin","",DYComp_cross_pt_binnum,DYComp_cross_pt_bin);
		  TH1D* h2DYpPb_cross_pt_rebin=new TH1D("h2DYpPb_cross_pt_rebin","",DYComp_cross_pt_binnum,DYComp_cross_pt_bin);

		  mergehist(DYComp_cross_pt_binnum,DYComp_cross_pt_bin,temph2,h2DYpPb_cross_pt_rebin,1.0);
		  mergehist(DYComp_cross_pt_binnum,DYComp_cross_pt_bin,temph1,h1DYpPb_cross_pt_rebin,1.0);

		  std::cout << "####################" << std::endl;
		  //h1DYpPb_cross_mass_rebin = (TH1D*)temph1->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  //h2DYpPb_cross_mass_rebin = (TH1D*)temph2->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  for (int i=0;i<DYComp_cross_pt_binnum;i++) {
					 std::cout << i+1 << " - " << h1DYpPb_cross_pt_rebin->GetBinContent(i+1) << " , " << h1DYpPb_cross_pt_rebin->GetBinError(i+1) << std::endl;
					 std::cout << i+1 << " - " << h2DYpPb_cross_pt_rebin->GetBinContent(i+1) << " , " << h2DYpPb_cross_pt_rebin->GetBinError(i+1) << std::endl;
		  }


		  //		  TH1D *hCompDYpPb_cross_pt_rebin=(TH1D*)temph1->Clone();
		  TH1D *hCompDYpPb_cross_pt_rebin= new TH1D("hCompDYpPb_cross_pt_rebin","",DYComp_cross_pt_binnum,DYComp_cross_pt_bin);
		  hCompDYpPb_cross_pt_rebin->Sumw2();
		  hCompDYpPb_cross_pt_rebin->Divide(h1DYpPb_cross_pt_rebin,h2DYpPb_cross_pt_rebin,1,1,"");

		  temph2->Scale(1.0);

		  MyCanvas c31("CompRpPb_cross_pt_oribin_detcor_FSRcor","p_{T} (GeV)","d#sigma/dp_{T} (nb/GeV)",800,800);
		  vh.push_back(temph1);
		  vh.push_back(temph1);
		  //vh.push_back(temph1);
		  vh.push_back(temph2);
        //vh.push_back(temph2);
		  vhistnames.push_back("60 < M < 120 in pPb@8.16 TeV");
        vhistnames.push_back("-2.87 < |y_{CM}| < 1.93");
		  //vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
		  vhistnames.push_back("Z in pPb@5.02 TeV, PLB 759 (2016) 36");
        //vhistnames.push_back("p_{T}^{l} > 20 GeV, |#eta_{lab}^{l}| < 2.4");
	     //c31.isLogX=kTRUE;
        c31.isLogY=kTRUE;
		  c31.LowerEdge_Y=0.005;
		  c31.UpperEdge_Y=90.0;
		  c31.Legend_x1=0.35;
		  c31.Legend_y1=0.64;
		  c31.Legend_x2=0.55;
		  c31.Legend_y2=0.81;
        c31.LegendTextSize=0.03;
		  c31.LegendFontStyle=42;

	     c31.CanvasWithMultipleHistograms(vh, vhistnames, "PE");
	     c31.PrintCanvas();

		  MyCanvas c32("CompRpPb_cross_pt_rebin_detcor_FSRcor","p_{T} (GeV)","d#sigma/dp_{T} (nb/GeV)",800,1000);
	     //c32.isLogX=kTRUE;
        c32.isLogY=kTRUE;
		  c32.isRatioPadAttached=kTRUE;
        c32.RatioStandard=3.0;
		  c32.LowerEdge_Ratio=-0.45;
		  c32.UpperEdge_Ratio=2.55;
		  c32.LowerEdge_Y=0.005;
		  c32.UpperEdge_Y=90.0;
		  c32.Legend_x1=0.35;
		  c32.Legend_y1=0.62;
		  c32.Legend_x2=0.55;
		  c32.Legend_y2=0.81;
		  c32.LegendTextSize=0.033;
		  c32.LegendFontStyle=42;

		  c32.TitleX="p_{T} (GeV)";
		  c32.TitleY="d#sigma/dp_{T} (nb/GeV)";
		  c32.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_pt_rebin, h2DYpPb_cross_pt_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{-2.87 < |y_{CM}| < 1.93}", "Z in pPb@5.02 TeV, PLB 759 (2016) 36", "#frac{pPb(DY)}{pPb(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
		  //c32.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_pt_rebin, h2DYpPb_cross_pt_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{#splitline{-2.87 < |y_{CM}| < 1.93}{|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV}}", "Z in pPb@5.02 TeV, PLB 759 (2016) 36", "#frac{pPb(DY)}{pPb(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");


		  c32.PrintCanvas();

		  temph1->Clear();
		  temph2->Clear();

		  vh.clear();
		  vhistnames.clear();
		  std::cout << "********** CHECK CLEAR" << std::endl;
		  vh.empty();
		  vhistnames.empty();

		  // 4. pPb DY vs pp DY (phistar)

		  temph1 = g2h(gaeres_phistar);//
		  //temph1 = g2hcor(gaeres_phistar, TEff_Acc_Phistar);
		  temph2 = array2h(sizeof(DYpp_cross_phistar_bin),DYpp_cross_phistar_bin,DYpp_cross_phistar_val,DYpp_cross_phistar_staterr);//pb-1

		  temph2->Scale(208.0);

		  MyCanvas c41("CompRpPb_cross_phistar_oribin_detcor_FSRcor","#phi*","d#sigma/d#phi* (nb)",800,800);
		  vh.push_back(temph1);
		  vh.push_back(temph1);
		  //vh.push_back(temph1);
		  vh.push_back(temph2);
		  vh.push_back(temph2);
		  vhistnames.push_back("60 < M < 120 in pPb@8.16 TeV");
        vhistnames.push_back("-2.87 < |y_{CM}| < 1.93");
        //vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
		  vhistnames.push_back("208#timesZ in pp@8 TeV, JHEP 03 (2018) 172");
		  vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 30 (20) GeV");
	     //c41.isLogX=kTRUE;
        c41.isLogY=kTRUE;
		  c41.LowerEdge_Y=0.2;
		  c41.UpperEdge_Y=5000.0;
		  c41.Legend_x1=0.35;
		  c41.Legend_y1=0.62;
		  c41.Legend_x2=0.55;
		  c41.Legend_y2=0.81;
        c41.LegendTextSize=0.03;
        c41.LegendFontStyle=42;
	     c41.CanvasWithMultipleHistograms(vh, vhistnames, "PE");
	     c41.PrintCanvas();


			std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		  for (int i=0;i<temph1->GetNbinsX();i++) {
					 std::cout << i+1 << " - " << temph1->GetBinLowEdge(i+1) << " - " << temph1->GetBinContent(i+1) << " , " << temph1->GetBinError(i+1) << std::endl;
		  }

		  TH1D* h1DYpPb_cross_phistar_rebin=new TH1D("h1DYpPb_cross_phistar_rebin","",DYComp_cross_phistar_binnum,DYComp_cross_phistar_bin);
		  TH1D* h2DYpPb_cross_phistar_rebin=new TH1D("h2DYpPb_cross_phistar_rebin","",DYComp_cross_phistar_binnum,DYComp_cross_phistar_bin);

//		  mergehist(DYComp_cross_phistar_binnum,DYComp_cross_phistar_bin,temph2,h2DYpPb_cross_phistar_rebin,208);
		  mergehist(DYComp_cross_phistar_binnum,DYComp_cross_phistar_bin,temph2,h2DYpPb_cross_phistar_rebin,1.0);
		  mergehist(DYComp_cross_phistar_binnum,DYComp_cross_phistar_bin,temph1,h1DYpPb_cross_phistar_rebin,1.0);

		  std::cout << "####################" << std::endl;
		  //h1DYpPb_cross_mass_rebin = (TH1D*)temph1->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  //h2DYpPb_cross_mass_rebin = (TH1D*)temph2->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  for (int i=0;i<DYComp_cross_phistar_binnum;i++) {
					 std::cout << i+1 << " - " << h1DYpPb_cross_phistar_rebin->GetBinLowEdge(i+1) << " - " << h1DYpPb_cross_phistar_rebin->GetBinContent(i+1) << " , " << h1DYpPb_cross_phistar_rebin->GetBinError(i+1) << std::endl;
					 std::cout << i+1 << " - " << h1DYpPb_cross_phistar_rebin->GetBinLowEdge(i+1) << " - " << h2DYpPb_cross_phistar_rebin->GetBinContent(i+1) << " , " << h2DYpPb_cross_phistar_rebin->GetBinError(i+1) << std::endl;
		  }

		  //		  TH1D *hCompDYpPb_cross_pt_rebin=(TH1D*)temph1->Clone();
		  TH1D *hCompDYpPb_cross_phistar_rebin= new TH1D("hCompDYpPb_cross_phistar_rebin","",DYComp_cross_phistar_binnum,DYComp_cross_phistar_bin);
		  hCompDYpPb_cross_phistar_rebin->Sumw2();
		  hCompDYpPb_cross_phistar_rebin->Divide(h1DYpPb_cross_phistar_rebin,h2DYpPb_cross_phistar_rebin,1,1,"");

		  MyCanvas c42("CompRpPb_cross_phistar_rebin_detcor_FSRcor","#phi*","d#sigma/d#phi* (nb)",800,1000);
	     //c42.isLogX=kTRUE;
        c42.isLogY=kTRUE;
		  c42.isRatioPadAttached=kTRUE;
		  c42.TitleX="#phi*";
		  c42.TitleY="d#sigma/d#phi* (nb)";
        c42.RatioStandard=3.5;
		  c42.LowerEdge_Ratio=3.0;
		  c42.UpperEdge_Ratio=0.5;
		  c42.LowerEdge_Y=9.0;
		  c42.UpperEdge_Y=90.0;
		  c42.LowerEdge_Y=0.2;
		  c42.UpperEdge_Y=5000.0;
		  c42.Legend_x1=0.35;
		  c42.Legend_y1=0.57;
		  c42.Legend_x2=0.55;
		  c42.Legend_y2=0.81;
		  c42.LegendTextSize=0.033;
        c42.LegendFontStyle=42;
		  //c42.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_phistar_rebin, h2DYpPb_cross_phistar_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{#splitline{-2.87 < |y_{CM}| < 1.93}{|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV}}", "#splitline{208#timesZ in pp@8 TeV, JHEP 03 (2018) 172}{|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 30 (20) GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
		  c42.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_phistar_rebin, h2DYpPb_cross_phistar_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{-2.87 < |y_{CM}| < 1.93}", "#splitline{208#timesZ in pp@8 TeV, JHEP 03 (2018) 172}{|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 30 (20) GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");


		  c42.PrintCanvas();

		  temph1->Clear();
		  temph2->Clear();

		  vh.clear();
		  vhistnames.clear();
		  std::cout << "********** CHECK CLEAR" << std::endl;
		  vh.empty();
		  vhistnames.empty();
/////////////
		  // for ptbin
		  TH1D *temph2_absyrap_0p0_0p4 = array2h(sizeof(cross_absyrap_ptbin),cross_absyrap_ptbin,cross_absyrap_0p0_0p4_ptval,cross_absyrap_0p0_0p4_pterr);
		  TH1D *temph2_absyrap_0p4_0p8 = array2h(sizeof(cross_absyrap_ptbin),cross_absyrap_ptbin,cross_absyrap_0p4_0p8_ptval,cross_absyrap_0p4_0p8_pterr);
		  TH1D *temph2_absyrap_0p8_1p2 = array2h(sizeof(cross_absyrap_ptbin),cross_absyrap_ptbin,cross_absyrap_0p8_1p2_ptval,cross_absyrap_0p8_1p2_pterr);
		  TH1D *temph2_absyrap_1p2_1p6 = array2h(sizeof(cross_absyrap_ptbin),cross_absyrap_ptbin,cross_absyrap_1p2_1p6_ptval,cross_absyrap_1p2_1p6_pterr);
		  TH1D *temph2_absyrap_1p6_2p0 = array2h(sizeof(cross_absyrap_ptbin),cross_absyrap_ptbin,cross_absyrap_1p6_2p0_ptval,cross_absyrap_1p6_2p0_pterr);

		  TH1D *temph2_ptinteg = (TH1D*)temph2_absyrap_0p0_0p4->Clone();
		  temph2_ptinteg->Add(temph2_absyrap_0p4_0p8);
		  temph2_ptinteg->Add(temph2_absyrap_0p8_1p2);
		  temph2_ptinteg->Add(temph2_absyrap_1p2_1p6);
		  temph2_ptinteg->Add(temph2_absyrap_1p6_2p0);
		  temph2_ptinteg->Scale(1/4.0);

//		  TH1D *temph2_absyrapinteg = new TH1D("temph2_absyrapinteg","",5,0.0,2.0);
		  TH1D *temph2_absyrapinteg = new TH1D("temph2_absyrapinteg","",10,-2.0,2.0);


			double tpval1=0.0;
			double tperr1=0.0;
			double tpval2=0.0;
			double tperr2=0.0;
			double tpval3=0.0;
			double tperr3=0.0;
			double tpval4=0.0;
			double tperr4=0.0;
			double tpval5=0.0;
			double tperr5=0.0;


				for (int j=0;j<9;j++) {		
					double tpbin = cross_absyrap_ptbin[j+1]-cross_absyrap_ptbin[j];
					std::cout << cross_absyrap_ptbin[j+1] << " - " << cross_absyrap_ptbin[j] << std::endl;
					tpval1+=cross_absyrap_0p0_0p4_ptval[j]*tpbin;
					tperr1+=cross_absyrap_0p0_0p4_pterr[j]*tpbin;
					tpval2+=cross_absyrap_0p4_0p8_ptval[j]*tpbin;
					tperr2+=cross_absyrap_0p4_0p8_pterr[j]*tpbin;
					tpval3+=cross_absyrap_0p8_1p2_ptval[j]*tpbin;
					tperr3+=cross_absyrap_0p8_1p2_pterr[j]*tpbin;
					tpval4+=cross_absyrap_1p2_1p6_ptval[j]*tpbin;
					tperr4+=cross_absyrap_1p2_1p6_pterr[j]*tpbin;
					tpval5+=cross_absyrap_1p6_2p0_ptval[j]*tpbin;
					tperr5+=cross_absyrap_1p6_2p0_pterr[j]*tpbin;
			}

				temph2_absyrapinteg->SetBinContent(5,tpval1/200.0);
				temph2_absyrapinteg->SetBinContent(4,tpval2/200.0);
				temph2_absyrapinteg->SetBinContent(3,tpval3/200.0);
				temph2_absyrapinteg->SetBinContent(2,tpval4/200.0);
				temph2_absyrapinteg->SetBinContent(1,tpval5/200.0);
				temph2_absyrapinteg->SetBinError(5,tperr1/200.0);
				temph2_absyrapinteg->SetBinError(4,tperr2/200.0);
				temph2_absyrapinteg->SetBinError(3,tperr3/200.0);
				temph2_absyrapinteg->SetBinError(2,tperr4/200.0);
				temph2_absyrapinteg->SetBinError(1,tperr5/200.0);
				temph2_absyrapinteg->SetBinContent(6,tpval1/200.0);
				temph2_absyrapinteg->SetBinContent(7,tpval2/200.0);
				temph2_absyrapinteg->SetBinContent(8,tpval3/200.0);
				temph2_absyrapinteg->SetBinContent(9,tpval4/200.0);
				temph2_absyrapinteg->SetBinContent(10,tpval5/200.0);
				temph2_absyrapinteg->SetBinError(6,tperr1/200.0);
				temph2_absyrapinteg->SetBinError(7,tperr2/200.0);
				temph2_absyrapinteg->SetBinError(8,tperr3/200.0);
				temph2_absyrapinteg->SetBinError(9,tperr4/200.0);
				temph2_absyrapinteg->SetBinError(10,tperr5/200.0);







		   	//temph2_absyrapinteg->SetBinContent(i+1,(cross_absyrap_0p0_0p4_ptval[i]+cross_absyrap_0p4_0p8_ptval[i]+cross_absyrap_0p8_1p2_ptval[i]+cross_absyrap_1p2_1p6_ptval[i]+cross_absyrap_1p6_2p0_ptval[i])*0.8/4.0);
		   	//temph2_absyrapinteg->SetBinError(i+1,(cross_absyrap_0p0_0p4_pterr[i]+cross_absyrap_0p4_0p8_pterr[i]+cross_absyrap_0p8_1p2_pterr[i]+cross_absyrap_1p2_1p6_pterr[i]+cross_absyrap_1p6_2p0_pterr[i])*0.8/4.0);
		  

		  // 5. pPb DY vs pp DY (pt, 60<M<120)

		  temph1 = g2h(gaeres_pt);//
		  //temph1 = g2hcor(gaeres_pt, TEff_Acc_Pt);

		  //temph2 = array2h(sizeof(ZpPb_cross_pt_bin),ZpPb_cross_pt_bin,ZpPb_cross_pt_val,ZpPb_cross_pt_staterr);//pb-1
	
		  temph2 = (TH1D*)temph2_ptinteg;

		  //const int DYComp_cross_pt2_binnum=7;
		  //double DYComp_cross_pt2_bin[DYComp_cross_pt2_binnum+1]={0,20,40,60,70,100,200,1000};
		  const int DYComp_cross_pt2_binnum=3;
		  double DYComp_cross_pt2_bin[DYComp_cross_pt2_binnum+1]={0,20,40,200};


		  TH1D* h1DYpPb_cross_pt2_rebin=new TH1D("h1DYpPb_cross_pt2_rebin","",DYComp_cross_pt2_binnum,DYComp_cross_pt2_bin);
		  TH1D* h2DYpPb_cross_pt2_rebin=new TH1D("h2DYpPb_cross_pt2_rebin","",DYComp_cross_pt2_binnum,DYComp_cross_pt2_bin);

		  mergehist(DYComp_cross_pt2_binnum,DYComp_cross_pt2_bin,temph2,h2DYpPb_cross_pt2_rebin,208*0.001);
		  std::cout << "!!!!!!!!!!!!!!!!!@@@@@@@@@@@@@@@!!!!!!!!!!!!!!!!!!!" << std::endl;

		  mergehist(DYComp_cross_pt2_binnum,DYComp_cross_pt2_bin,temph1,h1DYpPb_cross_pt2_rebin,1.0);

		  std::cout << "####################" << std::endl;
		  //h1DYpPb_cross_mass_rebin = (TH1D*)temph1->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  //h2DYpPb_cross_mass_rebin = (TH1D*)temph2->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  for (int i=0;i<DYComp_cross_pt2_binnum;i++) {
					 std::cout << i+1 << " - " << h1DYpPb_cross_pt2_rebin->GetBinContent(i+1) << " , " << h1DYpPb_cross_pt2_rebin->GetBinError(i+1) << std::endl;
					 std::cout << i+1 << " - " << h2DYpPb_cross_pt2_rebin->GetBinContent(i+1) << " , " << h2DYpPb_cross_pt2_rebin->GetBinError(i+1) << std::endl;
		  }


		  //		  TH1D *hCompDYpPb_cross_pt_rebin=(TH1D*)temph1->Clone();
		  TH1D *hCompDYpPb_cross_pt2_rebin= new TH1D("hCompDYpPb_cross_pt2_rebin","",DYComp_cross_pt2_binnum,DYComp_cross_pt2_bin);
		  hCompDYpPb_cross_pt2_rebin->Sumw2();
		  hCompDYpPb_cross_pt2_rebin->Divide(h1DYpPb_cross_pt2_rebin,h2DYpPb_cross_pt2_rebin,1,1,"");

		  temph2->Scale(208*0.001);

		  MyCanvas c51("CompRpPb_cross_pt2_oribin_detcor_FSRcor","p_{T} (GeV)","d#sigma/dp_{T} (nb/GeV)",800,800);
		  vh.push_back(temph1);
		  vh.push_back(temph1);
		  //vh.push_back(temph1);
		  vh.push_back(temph2);
		  vh.push_back(temph2);
		  //vh.push_back(temph2);
		  vhistnames.push_back("60 < M < 120 in pPb@8.16 TeV");
		  vhistnames.push_back("-2.87 < |y_{CM}| < 1.93");
		  //vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
		  vhistnames.push_back("208#timesZ in pp@8 TeV, PLB 749 (2015) 187");
        vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 25 (10) GeV");
		  //vhistnames.push_back("p_{T}^{leading} > 25 GeV, |#eta^{leading}| <2.1");
        //vhistnames.push_back("p_{T}^{second} > 10 GeV, |#eta^{second}| <2.4");


	
	     //c51.isLogX=kTRUE;
        c51.isLogY=kTRUE;
		  c51.LowerEdge_Y=0.0005;
		  c51.UpperEdge_Y=80.0;
		  c51.Legend_x1=0.35;
		  c51.Legend_y1=0.62;
		  c51.Legend_x2=0.55;
		  c51.Legend_y2=0.81;
		  c51.LegendTextSize=0.03;
        c51.LegendFontStyle=42;
	     c51.CanvasWithMultipleHistograms(vh, vhistnames, "PE");
	     c51.PrintCanvas();

		  MyCanvas c52("CompRpPb_cross_pt2_rebin_detcor_FSRcor","p_{T} (GeV)","d#sigma/dp_{T} (nb/GeV)",800,1000);
	     //c52.isLogX=kTRUE;
        c52.isLogY=kTRUE;
		  c52.isRatioPadAttached=kTRUE;
        c52.RatioStandard=3.0;
		  c52.LowerEdge_Ratio=-0.7;
		  c52.UpperEdge_Ratio=2.2;
		  c52.TitleX="p_{T} (GeV)";
		  c52.TitleY="d#sigma/dp_{T} (nb/GeV)";
		  c52.LowerEdge_X=0.5;
		  c52.UpperEdge_X=200.0;
		  c52.LowerEdge_Y=0.0005;
		  c52.UpperEdge_Y=80.0;
		  c52.Legend_x1=0.35;
		  c52.Legend_y1=0.57;
		  c52.Legend_x2=0.55;
		  c52.Legend_y2=0.81;
        c52.LegendTextSize=0.033;
		  c52.LegendFontStyle=42;

		  c52.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_pt2_rebin, h2DYpPb_cross_pt2_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{-2.87 < |y_{CM}| < 1.93}", "#splitline{208#timesZ in pp@8 TeV, PLB 749 (2015) 187}{|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 25 (10) GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
//		  c52.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_pt2_rebin, h2DYpPb_cross_pt2_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{#splitline{-2.87 < |y_{CM}| < 1.93}{|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV}}", "#splitline{208#timesZ in pp@8 TeV, PLB 749 (2015) 187}{|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 25 (10) GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");


		  c52.PrintCanvas();


		  temph1->Clear();
		  temph2->Clear();

		  vh.clear();
		  vhistnames.clear();
		  std::cout << "********** CHECK CLEAR" << std::endl;
		  vh.empty();
		  vhistnames.empty();

		  // 6. pPb DY vs pp DY (rap60120)

		  temph1 = g2h(gaeres_rap60120);
		  //temph1 = g2hcor(gaeres_rap60120, TEff_Acc_Rap60120);
		  //temph2 = array2h_yabs(sizeof(DYpp_cross_rap60120_bin),DYpp_cross_rap60120_bin,DYpp_cross_rap60120_val,DYpp_cross_rap60120_err);
		  temph2=(TH1D*)temph2_absyrapinteg;	

		  TH1D* h1DYpPb_cross_2rap60120_rebin=new TH1D("h1DYpPb_cross_2rap60120_rebin","",DYComp_cross_2rap60120_binnum,DYComp_cross_2rap60120_bin);
		  TH1D* h2DYpPb_cross_2rap60120_rebin=new TH1D("h2DYpPb_cross_2rap60120_rebin","",DYComp_cross_2rap60120_binnum,DYComp_cross_2rap60120_bin);

		  mergehist(DYComp_cross_2rap60120_binnum,DYComp_cross_2rap60120_bin,temph2,h2DYpPb_cross_2rap60120_rebin,208.0*0.001);//pp*208
		  mergehist(DYComp_cross_2rap60120_binnum,DYComp_cross_2rap60120_bin,temph1,h1DYpPb_cross_2rap60120_rebin,1.0);

		  for (int i=0;i<DYComp_cross_2rap60120_binnum;i++) {
					 std::cout << i+1 << " - " << h1DYpPb_cross_2rap60120_rebin->GetBinContent(i+1) << " , " << h1DYpPb_cross_2rap60120_rebin->GetBinError(i+1) << std::endl;
					 std::cout << i+1 << " - " << h2DYpPb_cross_2rap60120_rebin->GetBinContent(i+1) << " , " << h2DYpPb_cross_2rap60120_rebin->GetBinError(i+1) << std::endl;
		  }

		  TH1D *hCompDYpPb_cross_2rap60120_rebin=new TH1D("hCompDYpPb_cross_2rap60120_rebin","",DYComp_cross_2rap60120_binnum,DYComp_cross_2rap60120_bin);
		  hCompDYpPb_cross_2rap60120_rebin->Sumw2();
		  hCompDYpPb_cross_2rap60120_rebin->Divide(h1DYpPb_cross_2rap60120_rebin,h2DYpPb_cross_2rap60120_rebin,1,1,"");

//		  std::vector<TH1D*> vh2;
//		  std::vector<TString> vhistnames2;

		  temph2->Scale(208.0*0.001);

		  MyCanvas c61("CompRpPb_cross_2rap60120_oribin_detcor_FSRcor","y_{CM}","d#sigma/dy_{CM} (nb)",800,800);
		  vh.push_back(temph1);
		  //vh.push_back(temph1);
		  vh.push_back(temph2);
		  vh.push_back(temph2);

		  vhistnames.push_back("60 < M < 120 in pPb@8.16 TeV");
		  //vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
		  vhistnames.push_back("208#timesZ in pp@8 TeV, PLB 749 (2015) 187");
        vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 25 (10) GeV");
	
	     c61.isLogX=kFALSE;
        c61.isLogY=kTRUE;
		  c61.LowerEdge_Y=0.08;
		  c61.UpperEdge_Y=2000.0;
		  c61.Legend_x1=0.35;
		  c61.Legend_y1=0.64;
		  c61.Legend_x2=0.55;
		  c61.Legend_y2=0.81;
		  c61.LegendTextSize=0.03;
        c61.LegendFontStyle=42;

	     c61.CanvasWithMultipleHistograms(vh, vhistnames, "PE");
	     c61.PrintCanvas();

		  MyCanvas c62("CompRpPb_cross_2rap60120_rebin_detcor_FSRcor","y_{CM}","d#sigma/dy_{CM} (nb)",800,1000);
	     c62.isLogX=kFALSE;
        c62.isLogY=kTRUE;
		  c62.isRatioPadAttached=kTRUE;
		  c62.TitleX="y_{CM}";
		  c62.TitleY="d#sigma/dy_{CM} (nb)";
        c62.RatioStandard=200.0;
		  c62.LowerEdge_Ratio=-100.0;
		  c62.UpperEdge_Ratio=100.0;
		  c62.LowerEdge_Y=0.08;
		  c62.UpperEdge_Y=2000.0;
		  c62.Legend_x1=0.35;
		  c62.Legend_y1=0.62;
		  c62.Legend_x2=0.55;
		  c62.Legend_y2=0.81;
        c62.LegendTextSize=0.033;
        c62.LegendFontStyle=42;

		  c62.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_2rap60120_rebin, h2DYpPb_cross_2rap60120_rebin, "60 < M < 120 in pPb@8.16 TeV", "#splitline{208#timesZ in pp@8 TeV, PLB 749 (2015) 187}{|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 25 (10) GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
//		  c62.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_2rap60120_rebin, h2DYpPb_cross_2rap60120_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV}", "#splitline{208#timesZ in pp@8 TeV, PLB 749 (2015) 187}{|#eta_{lab}^{#mu}| < 2.1 (2.4), p_{T}^{#mu} > 25 (10) GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
	
		  c62.PrintCanvas();
		  std::cout << "################# CHECK CLEAR" << std::endl;
	
		  temph1->Clear();
		  temph2->Clear();

		  vh.clear();
		  vhistnames.clear();
		  std::cout << "**********#################### CHECK CLEAR" << std::endl;
		  vh.empty();
			vhistnames.empty();

		  // 7. pPb DY vs pp DY (pt, 60<M<120)-SMP-14-012

		  temph1 = g2h(gaeres_pt);//nb
		  temph2 = array2h(sizeof(DYpp_cross_pt60120_bin),DYpp_cross_pt60120_bin,DYpp_cross_pt60120_val2,DYpp_cross_pt60120_err2);//nb


		  const int DYComp_cross_ptv2_binnum=15;
		  double DYComp_cross_ptv2_bin[DYComp_cross_ptv2_binnum+1]={0.0,2.5,5.0,7.5,10.0,12.5,15.0,17.5,20.0,30.0,40.0,50.0,70.0,90.0,110.0,150.0};
	
		  TH1D* h1DYpPb_cross_ptv2_rebin=new TH1D("h1DYpPb_cross_ptv2_rebin","",DYComp_cross_ptv2_binnum,DYComp_cross_ptv2_bin);
		  TH1D* h2DYpPb_cross_ptv2_rebin=new TH1D("h2DYpPb_cross_ptv2_rebin","",DYComp_cross_ptv2_binnum,DYComp_cross_ptv2_bin);

		  mergehist(DYComp_cross_ptv2_binnum,DYComp_cross_ptv2_bin,temph2,h2DYpPb_cross_ptv2_rebin,208*0.01);
		  std::cout << "!!!!!!!!!!!!!!!!!@@@@@@@@@@@@@@@!!!!!!!!!!!!!!!!!!!" << std::endl;

		  mergehist(DYComp_cross_ptv2_binnum,DYComp_cross_ptv2_bin,temph1,h1DYpPb_cross_ptv2_rebin,1.0);

		  std::cout << "####################" << std::endl;
		  //h1DYpPb_cross_mass_rebin = (TH1D*)temph1->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  //h2DYpPb_cross_mass_rebin = (TH1D*)temph2->Rebin(DYComp_cross_mass_binnum,"",DYComp_cross_mass_bin);
		  for (int i=0;i<DYComp_cross_ptv2_binnum;i++) {
					 std::cout << i+1 << " - " << h1DYpPb_cross_ptv2_rebin->GetBinContent(i+1) << " , " << h1DYpPb_cross_ptv2_rebin->GetBinError(i+1) << std::endl;
					 std::cout << i+1 << " - " << h2DYpPb_cross_ptv2_rebin->GetBinContent(i+1) << " , " << h2DYpPb_cross_ptv2_rebin->GetBinError(i+1) << std::endl;
		  }


		  //		  TH1D *hCompDYpPb_cross_pt_rebin=(TH1D*)temph1->Clone();
		  TH1D *hCompDYpPb_cross_ptv2_rebin= new TH1D("hCompDYpPb_cross_ptv2_rebin","",DYComp_cross_ptv2_binnum,DYComp_cross_ptv2_bin);
		  hCompDYpPb_cross_ptv2_rebin->Sumw2();
		  hCompDYpPb_cross_ptv2_rebin->Divide(h1DYpPb_cross_ptv2_rebin,h2DYpPb_cross_ptv2_rebin,1,1,"");

		  temph2->Scale(208*0.01);

		  MyCanvas c71("CompRpPb_cross_ptv2_oribin_detcor_FSRcor","p_{T} (GeV)","d#sigma/dp_{T} (nb/GeV)",800,800);
		  vh.push_back(temph1);
		  vh.push_back(temph1);
		  //vh.push_back(temph1);
		  vh.push_back(temph2);
		  vh.push_back(temph2);

		  vhistnames.push_back("60 < M < 120 in pPb@8.16 TeV");
        vhistnames.push_back("-2.87 < |y_{CM}| < 1.93");
		  //vhistnames.push_back("|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV");
		  vhistnames.push_back("208#timesZ in pp@8 TeV, JHEP 1702 (2017) 096");
        vhistnames.push_back("|#eta^{#mu}| < 2.1, p_{T}^{#mu} > 20 GeV");


     //c71.isLogX=kTRUE;
        c71.isLogY=kTRUE;
		  c71.LowerEdge_Y=0.00005;
		  c71.UpperEdge_Y=800.0;
		  c71.Legend_x1=0.35;
		  c71.Legend_y1=0.62;
		  c71.Legend_x2=0.55;
		  c71.Legend_y2=0.81;
		  c71.LegendTextSize=0.03;
		  c71.LegendFontStyle=42;
	     c71.CanvasWithMultipleHistograms(vh, vhistnames, "PE");
	     c71.PrintCanvas();

		  MyCanvas c72("CompRpPb_cross_ptv2_rebin_detcor_FSRcor","p_{T} (GeV)","d#sigma/dp_{T} (nb/GeV)",800,1000);
	     //c72.isLogX=kTRUE;
        c72.isLogY=kTRUE;
		  c72.isRatioPadAttached=kTRUE;
        c72.RatioStandard=3.00;
		  c72.LowerEdge_Ratio=-1.00;
		  c72.UpperEdge_Ratio=6.50;
		  c72.TitleX="p_{T} (GeV)";
		  c72.TitleY="d#sigma/dp_{T} (nb/GeV)";
		  c72.LowerEdge_X=0.0;
		  c72.UpperEdge_X=150.0;
		  c72.LowerEdge_Y=0.00005;
		  c72.UpperEdge_Y=800.0;
		  c72.Legend_x1=0.35;
		  c72.Legend_y1=0.57;
		  c72.Legend_x2=0.55;
		  c72.Legend_y2=0.81;
		  c72.LegendTextSize=0.033;
		  c72.LegendFontStyle=42;

		  c72.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_ptv2_rebin, h2DYpPb_cross_ptv2_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{-2.87 < |y_{CM}| < 1.93}", "#splitline{208#timesZ in pp@8 TeV, JHEP 1702 (2017) 096}{|#eta^{#mu}| < 2.1, p_{T}^{#mu} > 20 GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");
//		  c72.CanvasWithHistogramsRatioPlot(h1DYpPb_cross_ptv2_rebin, h2DYpPb_cross_ptv2_rebin, "#splitline{60 < M < 120 in pPb@8.16 TeV}{#splitline{-2.87 < |y_{CM}| < 1.93}{|#eta_{lab}^{#mu}| < 2.4, p_{T}^{#mu} > 15 (10) GeV}}", "#splitline{208#timesZ in pp@8 TeV, JHEP 1702 (2017) 096}{|#eta^{#mu}| < 2.1, p_{T}^{#mu} > 20 GeV}", "#frac{pPb(DY)}{208#timespp(Z)}", kBlack, kRed, kFALSE, kFALSE, "EP", "EPSAME");


		  c72.PrintCanvas();


		  temph1->Clear();
		  temph2->Clear();

		  vh.clear();
		  vhistnames.clear();
		  std::cout << "********** CHECK CLEAR" << std::endl;
		  vh.empty();
		  vhistnames.empty();
///////////////

}
