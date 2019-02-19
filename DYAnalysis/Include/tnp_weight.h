#ifndef tnp_weight_h
#define tnp_weight_h

#include "TMath.h"

// IN THIS FILE YOU WILL FIND:
// ++++++++++++++
//
// - Iso, MuID: (tnp_weight_(iso|muid)_ppb)
//   * idx = 0:  nominal
//   * idx = 1..100: toy variations, stat. only
//   * idx = -1: syst variation, "new_MAX", +1 sigma
//   * idx = -2: syst variation, "new_MAX", -1 sigma
//   * idx = -10: binned
// - Trigger: (tnp_weight_trg_ppb)
//   * idx = 0: nominal
//   * idx = -1: syst variation,  +1 sigma
//   * idx = -2: syst variation,  -1 sigma
//   * idx = +1: stat variation,  +1 sigma
//   * idx = +2: stat variation,  -1 sigma
//
// For all:
//   * idx = +200: tnp efficiency for data
//   * idx = +300: tnp efficiency for MC

// THE INDIVIDUAL SFs
// ++++++++++++++++++
double tnp_weight_L3Mu12_ppb(double eta, int idx=0);
double tnp_weight_L1DM0_ppb(double eta, int idx=0);
double tnp_weight_L1DMOpen_ppb(double eta, int idx=0);
double tnp_weight_muid_ppb(double pt, double eta, int idx=0);
double tnp_weight_iso_ppb(double pt, double eta, int idx=0);
double tnp_weight_isotk_ppb(double pt, double eta, int idx=0);


///////////////////////////////////////////////////
//           L 3 M u 1 2    P P b                //
///////////////////////////////////////////////////
double tnp_weight_L3Mu12_ppb(double eta, int idx)
{
   double x = eta;
   double syst = 1.1e-2;

   double num=1,den=1;
   
   // MC
   if (x>-2.4&&x<=-2.1) den = 0.908255;
   if (x>-2.1&&x<=-1.6) den = 0.914033;
   if (x>-1.6&&x<=-1.2) den = 0.926589;
   if (x>-1.2&&x<=-0.9) den = 0.98233;
   if (x>-0.9&&x<=-0.6) den = 0.959976;
   if (x>-0.6&&x<=-0.3) den = 0.98021;
   if (x>-0.3&&x<=0) den = 0.962761;
   if (x>0&&x<=0.3) den = 0.960885;
   if (x>0.3&&x<=0.6) den = 0.979578;
   if (x>0.6&&x<=0.9) den = 0.960998;
   if (x>0.9&&x<=1.2) den = 0.983907;
   if (x>1.2&&x<=1.6) den = 0.929089;
   if (x>1.6&&x<=2.1) den = 0.935263;
   if (x>2.1&&x<=2.4) den = 0.910981;


   // data
   if (idx<=0 || idx>10) { // nominal
      if (x>-2.4&&x<=-2.1) num = 0.912108;
      if (x>-2.1&&x<=-1.6) num = 0.911221;
      if (x>-1.6&&x<=-1.2) num = 0.981533;
      if (x>-1.2&&x<=-0.9) num = 0.96613;
      if (x>-0.9&&x<=-0.6) num = 0.949597;
      if (x>-0.6&&x<=-0.3) num = 0.969409;
      if (x>-0.3&&x<=0) num = 0.945339;
      if (x>0&&x<=0.3) num = 0.940214;
      if (x>0.3&&x<=0.6) num = 0.971724;
      if (x>0.6&&x<=0.9) num = 0.948503;
      if (x>0.9&&x<=1.2) num = 0.961593;
      if (x>1.2&&x<=1.6) num = 0.96347;
      if (x>1.6&&x<=2.1) num = 0.934085;
      if (x>2.1&&x<=2.4) num = 0.900878;
   } else if (idx==1) { // stat up
      if (x>-2.4&&x<=-2.1) num = 0.919225;
      if (x>-2.1&&x<=-1.6) num = 0.917041;
      if (x>-1.6&&x<=-1.2) num = 0.984238;
      if (x>-1.2&&x<=-0.9) num = 0.970189;
      if (x>-0.9&&x<=-0.6) num = 0.953795;
      if (x>-0.6&&x<=-0.3) num = 0.972969;
      if (x>-0.3&&x<=0) num = 0.947511;
      if (x>0&&x<=0.3) num = 0.945271;
      if (x>0.3&&x<=0.6) num = 0.974013;
      if (x>0.6&&x<=0.9) num = 0.953942;
      if (x>0.9&&x<=1.2) num = 0.965556;
      if (x>1.2&&x<=1.6) num = 0.966879;
      if (x>1.6&&x<=2.1) num = 0.939012;
      if (x>2.1&&x<=2.4) num = 0.908827;
   } else if (idx==2) { // stat down
      if (x>-2.4&&x<=-2.1) num = 0.904625;
      if (x>-2.1&&x<=-1.6) num = 0.903859;
      if (x>-1.6&&x<=-1.2) num = 0.974557;
      if (x>-1.2&&x<=-0.9) num = 0.962079;
      if (x>-0.9&&x<=-0.6) num = 0.945176;
      if (x>-0.6&&x<=-0.3) num = 0.965603;
      if (x>-0.3&&x<=0) num = 0.945339;
      if (x>0&&x<=0.3) num = 0.93499;
      if (x>0.3&&x<=0.6) num = 0.968431;
      if (x>0.6&&x<=0.9) num = 0.943928;
      if (x>0.9&&x<=1.2) num = 0.957373;
      if (x>1.2&&x<=1.6) num = 0.959845;
      if (x>1.6&&x<=2.1) num = 0.928945;
      if (x>2.1&&x<=2.4) num = 0.892385;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;


   double syst_factor = 1.;
   if (idx==-1) syst_factor = 1 + syst;
   if (idx==-2) syst_factor = 1 - syst;
   return (num / den)*syst_factor;
}

///////////////////////////////////////////////////
//     L 1 D o u b l e M u 0    P P b            //
///////////////////////////////////////////////////
double tnp_weight_L1DM0_ppb(double eta, int idx)
{
   double x = eta;
   double syst = 1.1e-2; // difference between L1DoubleMu0 and L1DoubleMuOpen scale factors

   double num=1,den=1;
   
   // MC
   if (x>-2.4&&x<=-2.1) den = 0.982857;
   if (x>-2.1&&x<=-1.6) den = 0.973484;
   if (x>-1.6&&x<=-1.2) den = 0.972134;
   if (x>-1.2&&x<=-0.9) den = 0.987525;
   if (x>-0.9&&x<=-0.6) den = 0.973313;
   if (x>-0.6&&x<=-0.3) den = 0.990441;
   if (x>-0.3&&x<=0)    den = 0.973092;
   if (x>0&&x<=0.3)     den = 0.971528;
   if (x>0.3&&x<=0.6)   den = 0.990836;
   if (x>0.6&&x<=0.9)   den = 0.976367;
   if (x>0.9&&x<=1.2)   den = 0.990444;
   if (x>1.2&&x<=1.6)   den = 0.975738;
   if (x>1.6&&x<=2.1)   den = 0.985286;
   if (x>2.1&&x<=2.4)   den = 0.986199;

   // data
   if (idx<=2 || idx>10) { // nominal
      if (x>-2.4&&x<=-2.1) num =0.991539;
      if (x>-2.1&&x<=-1.6) num =0.969555;
      if (x>-1.6&&x<=-1.2) num =0.977558;
      if (x>-1.2&&x<=-0.9) num =0.992806;
      if (x>-0.9&&x<=-0.6) num =0.96072 ;
      if (x>-0.6&&x<=-0.3) num =0.978676;
      if (x>-0.3&&x<=0)    num =0.952624;
      if (x>0&&x<=0.3)     num =0.946901;
      if (x>0.3&&x<=0.6)   num =0.982872;
      if (x>0.6&&x<=0.9)   num =0.961925;
      if (x>0.9&&x<=1.2)   num =0.98804 ;
      if (x>1.2&&x<=1.6)   num =0.977894;
      if (x>1.6&&x<=2.1)   num =0.984519;
      if (x>2.1&&x<=2.4)   num =0.994097;
   }
   if (idx==1) { // stat up
      if (x>-2.4&&x<=-2.1) num += 0.00225041;
      if (x>-2.1&&x<=-1.6) num += 0.00343651;
      if (x>-1.6&&x<=-1.2) num += 0.00275357;
      if (x>-1.2&&x<=-0.9) num += 0.00164803;
      if (x>-0.9&&x<=-0.6) num += 0.00396919;
      if (x>-0.6&&x<=-0.3) num += 0.00303944;
      if (x>-0.3&&x<=0)    num += 0.0043784 ;
      if (x>0&&x<=0.3)     num += 0.00491259;
      if (x>0.3&&x<=0.6)   num += 0.00246509;
      if (x>0.6&&x<=0.9)   num += 0.00425373;
      if (x>0.9&&x<=1.2)   num += 0.00231363;
      if (x>1.2&&x<=1.6)   num += 0.00272991;
      if (x>1.6&&x<=2.1)   num += 0.00220958;
      if (x>2.1&&x<=2.4)   num += 0.00206436;
   } else if (idx==2) { // stat down
      if (x>-2.4&&x<=-2.1) num -= 0.00270934;
      if (x>-2.1&&x<=-1.6) num -= 0.00366383;
      if (x>-1.6&&x<=-1.2) num -= 0.00297891;
      if (x>-1.2&&x<=-0.9) num -= 0.00194414;
      if (x>-0.9&&x<=-0.6) num -= 0.00424359;
      if (x>-0.6&&x<=-0.3) num -= 0.00330551;
      if (x>-0.3&&x<=0)    num -= 0.00464207;
      if (x>0&&x<=0.3)     num -= 0.00519082;
      if (x>0.3&&x<=0.6)   num -= 0.00272528;
      if (x>0.6&&x<=0.9)   num -= 0.00454474;
      if (x>0.9&&x<=1.2)   num -= 0.00263606;
      if (x>1.2&&x<=1.6)   num -= 0.00297563;
      if (x>1.6&&x<=2.1)   num -= 0.00243847;
      if (x>2.1&&x<=2.4)   num -= 0.00251122;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;


   double syst_factor = 1.;
   if (idx==-1) syst_factor = 1 + syst;
   if (idx==-2) syst_factor = 1 - syst;
   return (num / den)*syst_factor;
}

///////////////////////////////////////////////////
//  L 1 D o u b l e M u O p e n    P P b         //
///////////////////////////////////////////////////
double tnp_weight_L1DMOpen_ppb(double eta, int idx)
{
   double x = eta;
   double syst = 1.1e-2; // difference between L1DoubleMu0 and L1DoubleMuOpen scale factors

   double num=1,den=1;
   
   // MC
   if (x>-2.4&&x<=-2.1) den =0.99031 ;
   if (x>-2.1&&x<=-1.6) den =0.992792;
   if (x>-1.6&&x<=-1.2) den =0.983185;
   if (x>-1.2&&x<=-0.9) den =0.996178;
   if (x>-0.9&&x<=-0.6) den =0.979449;
   if (x>-0.6&&x<=-0.3) den =0.990441;
   if (x>-0.3&&x<=0)    den =0.973092;
   if (x>0&&x<=0.3)     den =0.971528;
   if (x>0.3&&x<=0.6)   den =0.990836;
   if (x>0.6&&x<=0.9)   den =0.983241;
   if (x>0.9&&x<=1.2)   den =0.997454;
   if (x>1.2&&x<=1.6)   den =0.982754;
   if (x>1.6&&x<=2.1)   den =0.993091;
   if (x>2.1&&x<=2.4)   den =0.992475;
  
   // data
   if (idx<=2 || idx>10) { // nominal
      if (x>-2.4&&x<=-2.1) num = 0.99516 ;
      if (x>-2.1&&x<=-1.6) num = 0.994228;
      if (x>-1.6&&x<=-1.2) num = 0.977558;
      if (x>-1.2&&x<=-0.9) num = 0.997302;
      if (x>-0.9&&x<=-0.6) num = 0.970336;
      if (x>-0.6&&x<=-0.3) num = 0.978676;
      if (x>-0.3&&x<=0)    num = 0.952624;
      if (x>0&&x<=0.3)     num = 0.946901;
      if (x>0.3&&x<=0.6)   num = 0.982872;
      if (x>0.6&&x<=0.9)   num = 0.971548;
      if (x>0.9&&x<=1.2)   num = 0.998564;
      if (x>1.2&&x<=1.6)   num = 0.984637;
      if (x>1.6&&x<=2.1)   num = 0.997191;
      if (x>2.1&&x<=2.4)   num = 0.998567;
   }
   if (idx==1) { // stat up
      if (x>-2.4&&x<=-2.1) num += 0.0016834  ;
      if (x>-2.1&&x<=-1.6) num += 0.00161528 ;
      if (x>-1.6&&x<=-1.2) num += 0.00275357 ;
      if (x>-1.2&&x<=-0.9) num += 0.000956706;
      if (x>-0.9&&x<=-0.6) num += 0.00344249 ;
      if (x>-0.6&&x<=-0.3) num += 0.00303944 ;
      if (x>-0.3&&x<=0)    num += 0.0043784  ;
      if (x>0&&x<=0.3)     num += 0.00491259 ;
      if (x>0.3&&x<=0.6)   num += 0.00246509 ;
      if (x>0.6&&x<=0.9)   num += 0.00366432 ;
      if (x>0.9&&x<=1.2)   num += 0.000678155;
      if (x>1.2&&x<=1.6)   num += 0.00246445 ;
      if (x>1.6&&x<=2.1)   num += 0.00087856 ;
      if (x>2.1&&x<=2.4)   num += 0.000790046;
   } else if (idx==2) { // stat down
      if (x>-2.4&&x<=-2.1) num -= 0.00214072 ;
      if (x>-2.1&&x<=-1.6) num -= 0.00188805 ;
      if (x>-1.6&&x<=-1.2) num -= 0.00297891 ;
      if (x>-1.2&&x<=-0.9) num -= 0.00125364 ;
      if (x>-0.9&&x<=-0.6) num -= 0.003725   ;
      if (x>-0.6&&x<=-0.3) num -= 0.00330551 ;
      if (x>-0.3&&x<=0)    num -= 0.00464207 ;
      if (x>0&&x<=0.3)     num -= 0.00519082 ;
      if (x>0.3&&x<=0.6)   num -= 0.00272528 ;
      if (x>0.6&&x<=0.9)   num -= 0.00394902 ;
      if (x>0.9&&x<=1.2)   num -= 0.000992391;
      if (x>1.2&&x<=1.6)   num -= 0.00250703 ;
      if (x>1.6&&x<=2.1)   num -= 0.00111354 ;
      if (x>2.1&&x<=2.4)   num -= 0.00126329 ;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;


   double syst_factor = 1.;
   if (idx==-1) syst_factor = 1 + syst;
   if (idx==-2) syst_factor = 1 - syst;
   return (num / den)*syst_factor;
}

///////////////////////////////////////////////////
//             I S O T K    P P b                //
///////////////////////////////////////////////////
double tnp_weight_isotk_ppb(double pt, double eta, int idx) {
   double x = pt;

   if (idx == -10) {
      if (eta<0) {
      // 0 < |eta| < 2.4
         if (pt<12) return 0.899875;
         else if (pt<15) return 1.00203;
         else if (pt<30) return 0.988601;
         else if (pt<40) return 0.996251;
         else if (pt<50) return 0.999622;
         else if (pt<60) return 0.999224;
         else if (pt<70) return 0.998399;
         else if (pt<80) return 0.998121;
         else if (pt<100) return 1.00064;
         else return 0.994654;
      } else if (fabs(eta)<1.2) {
         // 0 < |eta| < 1.2
         if (pt<12) return 0.797468;
         else if (pt<15) return 0.97672;
         else if (pt<25) return 0.988025;
         else if (pt<30) return 1.00108;
         else if (pt<35) return 0.995768;
         else if (pt<40) return 0.996912;
         else if (pt<45) return 0.998978;
         else if (pt<50) return 0.999486;
         else if (pt<80) return 0.998308;
         else return 1.00106;
      } else if (fabs(eta)<2.1) {
         // 1.2 < |eta| < 2.1
         if (pt<12) return 0.832326;
         else if (pt<15) return 1.03105;
         else if (pt<25) return 0.973158;
         else if (pt<30) return 0.990945;
         else if (pt<35) return 0.996513;
         else if (pt<40) return 0.996408;
         else if (pt<45) return 1.00001;
         else if (pt<50) return 0.999821;
         else if (pt<80) return 0.999545;
         else return 1.00031;
      } else {
         // 2.1 < |eta| < 2.4
         if (pt<12) return 0.973428;
         else if (pt<15) return 0.976332;
         else if (pt<25) return 0.987469;
         else if (pt<40) return 0.996723;
         else if (pt<80) return 1.00092;
         else return 0.973697;
      }
   }


   double num = 1, den = 1;
   double den_template = 1; // for syst

   // nominal
   if (eta<0) {
      num = 0.94114*TMath::Erf((x+15.73192)/26.01468)+0.05704;
      den = 0.55296*TMath::Erf((x+9.96082)/22.50927)+0.44538;
      den_template = 0.54889*TMath::Erf((x+13.38925)/24.30749)+0.44911;
   } else if (fabs(eta)<1.2) {
      num = 1.07071*TMath::Erf((x+10.65585)/23.81678)-0.07241;
      den = 0.55394*TMath::Erf((x+5.60927)/20.43398)+0.44286;
      den_template = 0.55375*TMath::Erf((x+6.09326)/19.87543)+0.44373;
   } else if (fabs(eta)<2.1) {
      num = 1.41583*TMath::Erf((x+19.36896)/27.22747)-0.41668;
      den = 0.55335*TMath::Erf((x+6.78270)/18.88898)+0.44334;
      den_template = 0.55354*TMath::Erf((x+6.97575)/18.62340)+0.44417;
   } else {
      num = 0.61923*TMath::Erf((x+20.44974)/30.12043)+0.38085;
      den = 0.55361*TMath::Erf((x+11.17798)/19.84385)+0.44441;
      den_template = 0.55303*TMath::Erf((x+12.16760)/20.18979)+0.44509;
   }

   // 100 variations
   if (eta<0) {
      if (idx == 1   ) num = 0.37989*TMath::Erf((x+10.59475)/29.98341)+0.61633;
      else if (idx == 2   ) num = 0.94008*TMath::Erf((x+24.33931)/30.46569)+0.05603;
      else if (idx == 3   ) num = 1.59435*TMath::Erf((x+17.54774)/25.28373)-0.59630;
      else if (idx == 4   ) num = 0.94132*TMath::Erf((x+18.00545)/28.68355)+0.05724;
      else if (idx == 5   ) num = 1.37238*TMath::Erf((x+21.75730)/27.49049)-0.37505;
      else if (idx == 6   ) num = 1.52796*TMath::Erf((x+22.35371)/28.31532)-0.52928;
      else if (idx == 7   ) num = 0.94015*TMath::Erf((x+24.80040)/33.17943)+0.05608;
      else if (idx == 8   ) num = 1.70568*TMath::Erf((x+28.48101)/31.97963)-0.70751;
      else if (idx == 9   ) num = 1.65039*TMath::Erf((x+21.43335)/27.74606)-0.65457;
      else if (idx == 10  ) num = 1.74866*TMath::Erf((x+22.50134)/27.73189)-0.75111;
      else if (idx == 11  ) num = 0.94128*TMath::Erf((x+12.38695)/23.51048)+0.05716;
      else if (idx == 12  ) num = 0.94059*TMath::Erf((x+14.86325)/25.21063)+0.05649;
      else if (idx == 13  ) num = 0.94078*TMath::Erf((x+17.07817)/26.71057)+0.05668;
      else if (idx == 14  ) num = 0.94090*TMath::Erf((x+23.95538)/30.23182)+0.05682;
      else if (idx == 15  ) num = 0.94116*TMath::Erf((x+19.37241)/29.65654)+0.05708;
      else if (idx == 16  ) num = 0.94098*TMath::Erf((x+23.65989)/31.75993)+0.05691;
      else if (idx == 17  ) num = 0.94130*TMath::Erf((x+18.85691)/29.40099)+0.05722;
      else if (idx == 18  ) num = 0.94044*TMath::Erf((x+18.59179)/26.54932)+0.05634;
      else if (idx == 19  ) num = 0.94112*TMath::Erf((x+19.72728)/29.42496)+0.05705;
      else if (idx == 20  ) num = 0.94012*TMath::Erf((x+14.73718)/26.05507)+0.05601;
      else if (idx == 21  ) num = 0.94065*TMath::Erf((x+15.94593)/26.18241)+0.05656;
      else if (idx == 22  ) num = 0.94060*TMath::Erf((x+22.30689)/31.88791)+0.05653;
      else if (idx == 23  ) num = 0.94060*TMath::Erf((x+18.11846)/27.16788)+0.05650;
      else if (idx == 24  ) num = 1.93273*TMath::Erf((x+27.79349)/30.56860)-0.93459;
      else if (idx == 25  ) num = 0.94082*TMath::Erf((x+14.30348)/24.92349)+0.05671;
      else if (idx == 26  ) num = 0.94050*TMath::Erf((x+14.22666)/23.51640)+0.05640;
      else if (idx == 27  ) num = 0.94125*TMath::Erf((x+16.47957)/27.70111)+0.05716;
      else if (idx == 28  ) num = 0.94094*TMath::Erf((x+21.01462)/28.84180)+0.05686;
      else if (idx == 29  ) num = 0.94020*TMath::Erf((x+18.08383)/27.38920)+0.05611;
      else if (idx == 30  ) num = 1.70563*TMath::Erf((x+28.88044)/31.65658)-0.70841;
      else if (idx == 31  ) num = 0.94149*TMath::Erf((x+24.37762)/30.08884)+0.05741;
      else if (idx == 32  ) num = 0.94032*TMath::Erf((x+14.80792)/27.25833)+0.05622;
      else if (idx == 33  ) num = 0.93993*TMath::Erf((x+23.89121)/32.75886)+0.05585;
      else if (idx == 34  ) num = 1.62436*TMath::Erf((x+18.31533)/24.51477)-0.62560;
      else if (idx == 35  ) num = 0.94141*TMath::Erf((x+19.27117)/29.63030)+0.05734;
      else if (idx == 36  ) num = 0.94088*TMath::Erf((x+11.20100)/22.70046)+0.05676;
      else if (idx == 37  ) num = 1.90798*TMath::Erf((x+26.65514)/32.10272)-0.91102;
      else if (idx == 38  ) num = 0.94106*TMath::Erf((x+16.54297)/28.44310)+0.05696;
      else if (idx == 39  ) num = 1.43460*TMath::Erf((x+21.23387)/27.06066)-0.43615;
      else if (idx == 40  ) num = 1.76710*TMath::Erf((x+23.08973)/28.77545)-0.76970;
      else if (idx == 41  ) num = 0.34877*TMath::Erf((x+5.80859)/25.92602)+0.64921;
      else if (idx == 42  ) num = 0.94073*TMath::Erf((x+13.06165)/24.59529)+0.05662;
      else if (idx == 43  ) num = 0.94129*TMath::Erf((x+19.76949)/28.78747)+0.05721;
      else if (idx == 44  ) num = 0.94125*TMath::Erf((x+20.66595)/28.53801)+0.05717;
      else if (idx == 45  ) num = 0.94024*TMath::Erf((x+15.03700)/24.18672)+0.05613;
      else if (idx == 46  ) num = 0.94086*TMath::Erf((x+8.11335)/19.97074)+0.05673;
      else if (idx == 47  ) num = 1.51588*TMath::Erf((x+25.52542)/30.40586)-0.51719;
      else if (idx == 48  ) num = 1.69885*TMath::Erf((x+21.98757)/28.27713)-0.70170;
      else if (idx == 49  ) num = 0.94054*TMath::Erf((x+19.93593)/29.34151)+0.05645;
      else if (idx == 50  ) num = 1.68454*TMath::Erf((x+21.88983)/28.05480)-0.68584;
      else if (idx == 51  ) num = 0.94010*TMath::Erf((x+22.19465)/31.81395)+0.05603;
      else if (idx == 52  ) num = 0.94087*TMath::Erf((x+18.02915)/26.67598)+0.05678;
      else if (idx == 53  ) num = 0.94146*TMath::Erf((x+14.34407)/24.94129)+0.05736;
      else if (idx == 54  ) num = 1.62726*TMath::Erf((x+20.59295)/27.02469)-0.62840;
      else if (idx == 55  ) num = 0.94050*TMath::Erf((x+13.52714)/24.33792)+0.05639;
      else if (idx == 56  ) num = 0.93976*TMath::Erf((x+20.40168)/31.31410)+0.05568;
      else if (idx == 57  ) num = 0.94014*TMath::Erf((x+21.70164)/31.91282)+0.05606;
      else if (idx == 58  ) num = 1.77507*TMath::Erf((x+30.88982)/35.50181)-0.77721;
      else if (idx == 59  ) num = 0.46294*TMath::Erf((x+9.24492)/24.95459)+0.53348;
      else if (idx == 60  ) num = 0.94127*TMath::Erf((x+16.03131)/27.21049)+0.05718;
      else if (idx == 61  ) num = 0.93981*TMath::Erf((x+17.51575)/30.31970)+0.05573;
      else if (idx == 62  ) num = 0.94136*TMath::Erf((x+15.78496)/25.66361)+0.05726;
      else if (idx == 63  ) num = 0.94094*TMath::Erf((x+19.65735)/30.06447)+0.05686;
      else if (idx == 64  ) num = 0.48521*TMath::Erf((x+18.26141)/32.89736)+0.51289;
      else if (idx == 65  ) num = 1.68969*TMath::Erf((x+21.78560)/27.36325)-0.69208;
      else if (idx == 66  ) num = 1.44912*TMath::Erf((x+19.64525)/26.72554)-0.45127;
      else if (idx == 67  ) num = 1.69155*TMath::Erf((x+22.92216)/27.95179)-0.69365;
      else if (idx == 68  ) num = 0.33949*TMath::Erf((x+2.84432)/25.20801)+0.65821;
      else if (idx == 69  ) num = 0.94128*TMath::Erf((x+23.13499)/31.50401)+0.05723;
      else if (idx == 70  ) num = 1.73048*TMath::Erf((x+20.71897)/27.57399)-0.73165;
      else if (idx == 71  ) num = 1.89956*TMath::Erf((x+26.26061)/29.96950)-0.90162;
      else if (idx == 72  ) num = 1.54382*TMath::Erf((x+24.86487)/30.26677)-0.54472;
      else if (idx == 73  ) num = 1.68277*TMath::Erf((x+23.53359)/28.79453)-0.68611;
      else if (idx == 74  ) num = 0.14648*TMath::Erf((x-8.21342)/21.65051)+0.84896;
      else if (idx == 75  ) num = 0.94107*TMath::Erf((x+20.55384)/28.63451)+0.05698;
      else if (idx == 76  ) num = 1.60519*TMath::Erf((x+20.70684)/26.98729)-0.60948;
      else if (idx == 77  ) num = 0.94170*TMath::Erf((x+24.04755)/30.85580)+0.05763;
      else if (idx == 78  ) num = 1.79433*TMath::Erf((x+24.66297)/28.70715)-0.79595;
      else if (idx == 79  ) num = 0.94047*TMath::Erf((x+11.06613)/21.77162)+0.05636;
      else if (idx == 80  ) num = 1.53371*TMath::Erf((x+33.52217)/36.46866)-0.53609;
      else if (idx == 81  ) num = 1.62735*TMath::Erf((x+22.69801)/27.41089)-0.63093;
      else if (idx == 82  ) num = 0.94088*TMath::Erf((x+22.59615)/29.27524)+0.05681;
      else if (idx == 83  ) num = 1.67209*TMath::Erf((x+26.03019)/28.95765)-0.67425;
      else if (idx == 84  ) num = 0.94038*TMath::Erf((x+15.96739)/28.01957)+0.05628;
      else if (idx == 85  ) num = 1.71168*TMath::Erf((x+24.22183)/29.06285)-0.71488;
      else if (idx == 86  ) num = 0.94183*TMath::Erf((x+21.78438)/30.10353)+0.05776;
      else if (idx == 87  ) num = 1.79471*TMath::Erf((x+21.62298)/28.20806)-0.79574;
      else if (idx == 88  ) num = 0.94063*TMath::Erf((x+14.51994)/24.56142)+0.05652;
      else if (idx == 89  ) num = 0.94100*TMath::Erf((x+15.15749)/26.32133)+0.05690;
      else if (idx == 90  ) num = 1.25241*TMath::Erf((x+18.19431)/27.12279)-0.25456;
      else if (idx == 91  ) num = 1.44397*TMath::Erf((x+29.17284)/33.27819)-0.44756;
      else if (idx == 92  ) num = 0.94071*TMath::Erf((x+15.98918)/25.85122)+0.05661;
      else if (idx == 93  ) num = 0.94010*TMath::Erf((x+14.81826)/26.03188)+0.05599;
      else if (idx == 94  ) num = 0.94097*TMath::Erf((x+19.20736)/29.58564)+0.05690;
      else if (idx == 95  ) num = 0.93986*TMath::Erf((x+10.74912)/22.54525)+0.05573;
      else if (idx == 96  ) num = 0.94151*TMath::Erf((x+13.25569)/23.48682)+0.05740;
      else if (idx == 97  ) num = 0.94153*TMath::Erf((x+17.27073)/28.12527)+0.05744;
      else if (idx == 98  ) num = 1.69092*TMath::Erf((x+20.85395)/26.90380)-0.69455;
      else if (idx == 99  ) num = 0.94100*TMath::Erf((x+17.60982)/27.28841)+0.05691;
      else if (idx == 100 ) num = 0.94071*TMath::Erf((x+16.37840)/25.90793)+0.05661;
      else if (idx == -1 || idx == -2) num = 0.32294*TMath::Erf((x+2.62892)/22.42486)+0.67596;
   } else if (fabs(eta)<1.2) {
      if (idx == 1   ) num = 0.31847*TMath::Erf((x-2.04389)/21.46501)+0.68145;
      else if (idx == 2   ) num = 1.68981*TMath::Erf((x+9.29136)/21.47165)-0.69084;
      else if (idx == 3   ) num = 1.83398*TMath::Erf((x+17.62472)/27.23028)-0.83587;
      else if (idx == 4   ) num = 1.07137*TMath::Erf((x+8.56983)/23.39742)-0.07175;
      else if (idx == 5   ) num = 1.67094*TMath::Erf((x+20.17574)/28.33410)-0.67280;
      else if (idx == 6   ) num = 1.06967*TMath::Erf((x+8.97559)/21.74527)-0.07345;
      else if (idx == 7   ) num = 1.07129*TMath::Erf((x+6.40955)/21.71597)-0.07185;
      else if (idx == 8   ) num = 1.74846*TMath::Erf((x+15.52814)/25.52893)-0.75003;
      else if (idx == 9   ) num = 1.07119*TMath::Erf((x+7.87422)/22.95510)-0.07194;
      else if (idx == 10  ) num = 1.07156*TMath::Erf((x+16.58709)/27.41873)-0.07154;
      else if (idx == 11  ) num = 1.07077*TMath::Erf((x+6.50696)/22.23903)-0.07237;
      else if (idx == 12  ) num = 0.56726*TMath::Erf((x+3.49781)/23.19383)+0.43293;
      else if (idx == 13  ) num = 1.79101*TMath::Erf((x+18.01118)/27.93834)-0.79194;
      else if (idx == 14  ) num = 1.07101*TMath::Erf((x+8.54432)/22.77295)-0.07211;
      else if (idx == 15  ) num = 1.07118*TMath::Erf((x+7.91892)/22.81570)-0.07195;
      else if (idx == 16  ) num = 1.07146*TMath::Erf((x+6.86667)/22.46208)-0.07167;
      else if (idx == 17  ) num = 1.86067*TMath::Erf((x+17.02545)/26.85530)-0.86127;
      else if (idx == 18  ) num = 1.90653*TMath::Erf((x+14.17576)/24.69606)-0.90737;
      else if (idx == 19  ) num = 1.07115*TMath::Erf((x+19.52283)/29.69164)-0.07194;
      else if (idx == 20  ) num = 1.07136*TMath::Erf((x+5.04038)/20.92857)-0.07178;
      else if (idx == 21  ) num = 1.72582*TMath::Erf((x+21.10435)/28.95792)-0.72670;
      else if (idx == 22  ) num = 1.92060*TMath::Erf((x+21.57730)/29.19668)-0.92167;
      else if (idx == 23  ) num = 1.07065*TMath::Erf((x+7.02293)/21.30039)-0.07248;
      else if (idx == 24  ) num = 1.88096*TMath::Erf((x+17.82610)/25.95412)-0.88280;
      else if (idx == 25  ) num = 1.07137*TMath::Erf((x+9.52759)/23.75936)-0.07175;
      else if (idx == 26  ) num = 1.07041*TMath::Erf((x+17.91335)/27.81462)-0.07269;
      else if (idx == 27  ) num = 1.07082*TMath::Erf((x+5.96890)/21.16834)-0.07231;
      else if (idx == 28  ) num = 1.07094*TMath::Erf((x+13.91073)/25.54908)-0.07217;
      else if (idx == 29  ) num = 1.07061*TMath::Erf((x+11.00858)/25.05870)-0.07251;
      else if (idx == 30  ) num = 1.07146*TMath::Erf((x+16.41833)/28.37711)-0.07162;
      else if (idx == 31  ) num = 0.47856*TMath::Erf((x+6.16372)/24.83601)+0.52084;
      else if (idx == 32  ) num = 1.07067*TMath::Erf((x+13.15663)/26.35925)-0.07243;
      else if (idx == 33  ) num = 1.07155*TMath::Erf((x+10.24287)/24.31861)-0.07157;
      else if (idx == 34  ) num = 1.07169*TMath::Erf((x+14.69770)/27.94667)-0.07140;
      else if (idx == 35  ) num = 1.07058*TMath::Erf((x+13.97489)/25.97912)-0.07252;
      else if (idx == 36  ) num = 0.57741*TMath::Erf((x+0.12420)/19.79757)+0.42170;
      else if (idx == 37  ) num = 1.07117*TMath::Erf((x+6.01876)/21.87805)-0.07197;
      else if (idx == 38  ) num = 1.07161*TMath::Erf((x+14.12915)/27.01000)-0.07147;
      else if (idx == 39  ) num = 1.65024*TMath::Erf((x+13.40782)/24.76153)-0.65144;
      else if (idx == 40  ) num = 1.07057*TMath::Erf((x+11.47073)/24.59892)-0.07254;
      else if (idx == 41  ) num = 1.07135*TMath::Erf((x+12.38641)/25.25026)-0.07176;
      else if (idx == 42  ) num = 1.07168*TMath::Erf((x+20.97892)/31.22818)-0.07139;
      else if (idx == 43  ) num = 1.07172*TMath::Erf((x+7.64668)/22.42344)-0.07141;
      else if (idx == 44  ) num = 1.07002*TMath::Erf((x+15.14829)/26.33203)-0.07308;
      else if (idx == 45  ) num = 1.07080*TMath::Erf((x+16.66263)/27.20092)-0.07229;
      else if (idx == 46  ) num = 1.07124*TMath::Erf((x+10.93952)/24.39255)-0.07188;
      else if (idx == 47  ) num = 1.07167*TMath::Erf((x+15.65537)/26.76994)-0.07143;
      else if (idx == 48  ) num = 1.06975*TMath::Erf((x+4.10964)/19.17744)-0.07339;
      else if (idx == 49  ) num = 1.07122*TMath::Erf((x+12.90051)/27.05003)-0.07189;
      else if (idx == 50  ) num = 1.47721*TMath::Erf((x+17.17424)/28.12523)-0.47794;
      else if (idx == 51  ) num = 0.59867*TMath::Erf((x+2.42348)/21.21224)+0.40130;
      else if (idx == 52  ) num = 1.80430*TMath::Erf((x+12.90543)/23.49643)-0.80621;
      else if (idx == 53  ) num = 1.07128*TMath::Erf((x+19.73405)/29.96698)-0.07178;
      else if (idx == 54  ) num = 1.87785*TMath::Erf((x+20.09440)/28.72749)-0.87787;
      else if (idx == 55  ) num = 1.80696*TMath::Erf((x+13.77046)/23.67068)-0.80874;
      else if (idx == 56  ) num = 1.48176*TMath::Erf((x+20.28471)/28.75653)-0.48231;
      else if (idx == 57  ) num = 1.07077*TMath::Erf((x+12.70656)/24.94139)-0.07233;
      else if (idx == 58  ) num = 1.87316*TMath::Erf((x+9.04491)/21.43467)-0.87421;
      else if (idx == 59  ) num = 1.07117*TMath::Erf((x+24.91608)/32.40179)-0.07190;
      else if (idx == 60  ) num = 1.43889*TMath::Erf((x+11.13415)/24.02655)-0.43989;
      else if (idx == 61  ) num = 1.07103*TMath::Erf((x+22.10928)/31.05570)-0.07205;
      else if (idx == 62  ) num = 0.29492*TMath::Erf((x-10.00181)/16.12413)+0.70372;
      else if (idx == 63  ) num = 1.87689*TMath::Erf((x+16.43716)/26.81907)-0.87835;
      else if (idx == 64  ) num = 1.07122*TMath::Erf((x+21.28186)/30.70982)-0.07183;
      else if (idx == 65  ) num = 1.68738*TMath::Erf((x+13.49043)/24.37883)-0.68965;
      else if (idx == 66  ) num = 1.99634*TMath::Erf((x+18.33211)/26.61084)-0.99818;
      else if (idx == 67  ) num = 1.07152*TMath::Erf((x+6.23343)/22.03072)-0.07162;
      else if (idx == 68  ) num = 1.07112*TMath::Erf((x+9.30885)/23.83494)-0.07201;
      else if (idx == 69  ) num = 1.07039*TMath::Erf((x+10.66794)/24.20009)-0.07273;
      else if (idx == 70  ) num = 1.44337*TMath::Erf((x+14.88134)/25.00965)-0.44513;
      else if (idx == 71  ) num = 2.02978*TMath::Erf((x+15.92893)/25.70836)-1.02984;
      else if (idx == 72  ) num = 1.07100*TMath::Erf((x+18.17580)/29.35890)-0.07209;
      else if (idx == 73  ) num = 1.07081*TMath::Erf((x+12.69754)/25.26948)-0.07230;
      else if (idx == 74  ) num = 1.07089*TMath::Erf((x+18.43952)/28.67310)-0.07218;
      else if (idx == 75  ) num = 1.69900*TMath::Erf((x+21.74788)/28.52636)-0.70056;
      else if (idx == 76  ) num = 1.22908*TMath::Erf((x+18.42936)/29.84729)-0.22874;
      else if (idx == 77  ) num = 1.79090*TMath::Erf((x+17.33328)/26.57345)-0.79254;
      else if (idx == 78  ) num = 1.07112*TMath::Erf((x+7.30089)/21.83941)-0.07200;
      else if (idx == 79  ) num = 1.07073*TMath::Erf((x+10.77880)/24.40508)-0.07238;
      else if (idx == 80  ) num = 1.07107*TMath::Erf((x+18.84382)/29.44363)-0.07203;
      else if (idx == 81  ) num = 1.86394*TMath::Erf((x+13.01368)/24.13321)-0.86432;
      else if (idx == 82  ) num = 1.96243*TMath::Erf((x+12.42912)/23.07677)-0.96398;
      else if (idx == 83  ) num = 1.07188*TMath::Erf((x+13.90716)/25.93250)-0.07121;
      else if (idx == 84  ) num = 1.07184*TMath::Erf((x+13.03838)/25.85826)-0.07125;
      else if (idx == 85  ) num = 1.07074*TMath::Erf((x+17.58849)/27.08986)-0.07235;
      else if (idx == 86  ) num = 1.07073*TMath::Erf((x+6.76731)/20.39920)-0.07240;
      else if (idx == 87  ) num = 1.75383*TMath::Erf((x+14.61999)/24.86081)-0.75556;
      else if (idx == 88  ) num = 1.07100*TMath::Erf((x+10.35664)/24.20426)-0.07212;
      else if (idx == 89  ) num = 0.56330*TMath::Erf((x+7.38318)/24.80575)+0.43676;
      else if (idx == 90  ) num = 1.07097*TMath::Erf((x+8.89612)/23.17183)-0.07215;
      else if (idx == 91  ) num = 1.62281*TMath::Erf((x+11.85694)/24.03410)-0.62371;
      else if (idx == 92  ) num = 1.35581*TMath::Erf((x+19.23059)/28.53992)-0.35698;
      else if (idx == 93  ) num = 1.07121*TMath::Erf((x+10.53549)/24.17389)-0.07190;
      else if (idx == 94  ) num = 0.44294*TMath::Erf((x-2.87963)/18.76174)+0.55548;
      else if (idx == 95  ) num = 1.07038*TMath::Erf((x+9.25266)/22.28688)-0.07275;
      else if (idx == 96  ) num = 1.07159*TMath::Erf((x+11.58078)/25.60725)-0.07152;
      else if (idx == 97  ) num = 1.07117*TMath::Erf((x+15.50772)/27.22787)-0.07192;
      else if (idx == 98  ) num = 1.06973*TMath::Erf((x+13.04500)/24.29877)-0.07338;
      else if (idx == 99  ) num = 0.58751*TMath::Erf((x+1.13169)/21.39713)+0.41140;
      else if (idx == 100 ) num = 1.07077*TMath::Erf((x+11.69211)/23.55225)-0.07235;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1 || idx == -2) num = 0.35297*TMath::Erf((x-0.29199)/20.53027)+0.64531;
   } else if (fabs(eta)<2.1) {
      if (idx == 1   ) num = 1.41467*TMath::Erf((x+25.15405)/30.74056)-0.41781;
      else if (idx == 2   ) num = 2.05873*TMath::Erf((x+29.77577)/31.28382)-1.06073;
      else if (idx == 3   ) num = 1.41583*TMath::Erf((x+18.40864)/26.93731)-0.41668;
      else if (idx == 4   ) num = 2.12744*TMath::Erf((x+21.21537)/26.61429)-1.12984;
      else if (idx == 5   ) num = 2.17779*TMath::Erf((x+20.81129)/25.90202)-1.18006;
      else if (idx == 6   ) num = 2.19673*TMath::Erf((x+27.88701)/30.23629)-1.19723;
      else if (idx == 7   ) num = 2.11665*TMath::Erf((x+18.02000)/23.73520)-1.11911;
      else if (idx == 8   ) num = 1.99855*TMath::Erf((x+26.68196)/29.71550)-0.99897;
      else if (idx == 9   ) num = 1.41684*TMath::Erf((x+37.45495)/39.53561)-0.41554;
      else if (idx == 10  ) num = 1.41629*TMath::Erf((x+25.16271)/31.11885)-0.41618;
      else if (idx == 11  ) num = 1.41525*TMath::Erf((x+17.84949)/26.71882)-0.41726;
      else if (idx == 12  ) num = 1.41453*TMath::Erf((x+15.41095)/24.14749)-0.41799;
      else if (idx == 13  ) num = 1.41489*TMath::Erf((x+15.99674)/24.13202)-0.41763;
      else if (idx == 14  ) num = 2.06019*TMath::Erf((x+20.07700)/25.82256)-1.06330;
      else if (idx == 15  ) num = 2.09987*TMath::Erf((x+27.08320)/30.78020)-1.10020;
      else if (idx == 16  ) num = 2.18409*TMath::Erf((x+24.95548)/29.42797)-1.18555;
      else if (idx == 17  ) num = 2.08324*TMath::Erf((x+38.44819)/37.93762)-1.08240;
      else if (idx == 18  ) num = 2.21144*TMath::Erf((x+13.37380)/21.34947)-1.21368;
      else if (idx == 19  ) num = 2.12140*TMath::Erf((x+32.23741)/32.88550)-1.12334;
      else if (idx == 20  ) num = 1.41609*TMath::Erf((x+17.61205)/26.64818)-0.41643;
      else if (idx == 21  ) num = 2.10790*TMath::Erf((x+16.79807)/24.05128)-1.11095;
      else if (idx == 22  ) num = 2.16662*TMath::Erf((x+26.87596)/30.63222)-1.16805;
      else if (idx == 23  ) num = 2.12294*TMath::Erf((x+25.55220)/29.71260)-1.12433;
      else if (idx == 24  ) num = 1.77524*TMath::Erf((x+22.15964)/27.66507)-0.77706;
      else if (idx == 25  ) num = 2.15442*TMath::Erf((x+24.30299)/27.90562)-1.15735;
      else if (idx == 26  ) num = 2.13378*TMath::Erf((x+23.42521)/27.72509)-1.13664;
      else if (idx == 27  ) num = 2.01640*TMath::Erf((x+16.29500)/23.59037)-1.01859;
      else if (idx == 28  ) num = 2.13103*TMath::Erf((x+14.91009)/22.93386)-1.13431;
      else if (idx == 29  ) num = 1.41425*TMath::Erf((x+15.58822)/23.72926)-0.41827;
      else if (idx == 30  ) num = 1.41559*TMath::Erf((x+23.37820)/30.11743)-0.41690;
      else if (idx == 31  ) num = 2.15477*TMath::Erf((x+22.39093)/26.69799)-1.15607;
      else if (idx == 32  ) num = 1.41501*TMath::Erf((x+11.67592)/22.89951)-0.41753;
      else if (idx == 33  ) num = 2.20275*TMath::Erf((x+22.71969)/27.51081)-1.20395;
      else if (idx == 34  ) num = 2.18689*TMath::Erf((x+29.39049)/32.29752)-1.18787;
      else if (idx == 35  ) num = 1.41542*TMath::Erf((x+14.86884)/24.32173)-0.41710;
      else if (idx == 36  ) num = 2.24168*TMath::Erf((x+22.11725)/27.36971)-1.24399;
      else if (idx == 37  ) num = 1.41554*TMath::Erf((x+17.75502)/26.16510)-0.41698;
      else if (idx == 38  ) num = 1.41481*TMath::Erf((x+20.45617)/26.61249)-0.41769;
      else if (idx == 39  ) num = 2.13566*TMath::Erf((x+35.61398)/35.91721)-1.13576;
      else if (idx == 40  ) num = 1.86157*TMath::Erf((x+33.20463)/34.30551)-0.86155;
      else if (idx == 41  ) num = 1.41572*TMath::Erf((x+20.19017)/28.23919)-0.41678;
      else if (idx == 42  ) num = 1.41617*TMath::Erf((x+25.21071)/30.93284)-0.41630;
      else if (idx == 43  ) num = 2.25592*TMath::Erf((x+18.28055)/25.01975)-1.25637;
      else if (idx == 44  ) num = 2.22373*TMath::Erf((x+24.43010)/29.23466)-1.22516;
      else if (idx == 45  ) num = 2.27263*TMath::Erf((x+23.63880)/28.05251)-1.27374;
      else if (idx == 46  ) num = 1.41543*TMath::Erf((x+14.68365)/23.99612)-0.41710;
      else if (idx == 47  ) num = 1.41577*TMath::Erf((x+24.55398)/31.10808)-0.41670;
      else if (idx == 48  ) num = 1.41518*TMath::Erf((x+18.49498)/26.77458)-0.41732;
      else if (idx == 49  ) num = 2.20874*TMath::Erf((x+15.24823)/22.52150)-1.21233;
      else if (idx == 50  ) num = 2.14191*TMath::Erf((x+28.72250)/30.92911)-1.14318;
      else if (idx == 51  ) num = 2.10254*TMath::Erf((x+20.80654)/25.98162)-1.10543;
      else if (idx == 52  ) num = 1.41496*TMath::Erf((x+13.76097)/22.78794)-0.41756;
      else if (idx == 53  ) num = 1.61798*TMath::Erf((x+49.97522)/46.72917)-0.61370;
      else if (idx == 54  ) num = 1.93874*TMath::Erf((x+20.04097)/23.77391)-0.94136;
      else if (idx == 55  ) num = 2.12791*TMath::Erf((x+13.68730)/22.08593)-1.13127;
      else if (idx == 56  ) num = 1.41556*TMath::Erf((x+14.18082)/23.39035)-0.41697;
      else if (idx == 57  ) num = 1.41534*TMath::Erf((x+19.53767)/27.54390)-0.41716;
      else if (idx == 58  ) num = 1.41459*TMath::Erf((x+19.76984)/26.51305)-0.41791;
      else if (idx == 59  ) num = 2.19808*TMath::Erf((x+30.22290)/31.68562)-1.20025;
      else if (idx == 60  ) num = 2.01197*TMath::Erf((x+20.33808)/26.16110)-1.01414;
      else if (idx == 61  ) num = 2.09434*TMath::Erf((x+18.49891)/25.23370)-1.09554;
      else if (idx == 62  ) num = 1.41436*TMath::Erf((x+18.30284)/25.13160)-0.41814;
      else if (idx == 63  ) num = 1.41574*TMath::Erf((x+22.96140)/29.52188)-0.41675;
      else if (idx == 64  ) num = 1.41524*TMath::Erf((x+16.59674)/24.93643)-0.41727;
      else if (idx == 65  ) num = 2.21887*TMath::Erf((x+12.07719)/20.08689)-1.22148;
      else if (idx == 66  ) num = 2.26944*TMath::Erf((x+11.32475)/20.28944)-1.27329;
      else if (idx == 67  ) num = 2.18858*TMath::Erf((x+25.74562)/29.03264)-1.19074;
      else if (idx == 68  ) num = 2.13328*TMath::Erf((x+26.33751)/29.40702)-1.13521;
      else if (idx == 69  ) num = 2.23777*TMath::Erf((x+21.82587)/27.07417)-1.23964;
      else if (idx == 70  ) num = 2.38998*TMath::Erf((x+21.78635)/27.29218)-1.39157;
      else if (idx == 71  ) num = 2.22088*TMath::Erf((x+19.60245)/24.95999)-1.22314;
      else if (idx == 72  ) num = 1.41543*TMath::Erf((x+13.85501)/23.64391)-0.41710;
      else if (idx == 73  ) num = 2.14629*TMath::Erf((x+29.57234)/30.99043)-1.14847;
      else if (idx == 74  ) num = 2.14856*TMath::Erf((x+30.11000)/32.31988)-1.15006;
      else if (idx == 75  ) num = 1.41570*TMath::Erf((x+21.82198)/28.96577)-0.41680;
      else if (idx == 76  ) num = 2.20561*TMath::Erf((x+24.34325)/27.58960)-1.20826;
      else if (idx == 77  ) num = 1.41548*TMath::Erf((x+28.89846)/32.21986)-0.41700;
      else if (idx == 78  ) num = 2.18708*TMath::Erf((x+24.38935)/28.28265)-1.18780;
      else if (idx == 79  ) num = 2.13737*TMath::Erf((x+22.18729)/26.17573)-1.14126;
      else if (idx == 80  ) num = 2.17724*TMath::Erf((x+21.19073)/26.55657)-1.17786;
      else if (idx == 81  ) num = 1.41576*TMath::Erf((x+30.67589)/33.78933)-0.41671;
      else if (idx == 82  ) num = 1.41485*TMath::Erf((x+19.68553)/26.85289)-0.41765;
      else if (idx == 83  ) num = 1.41623*TMath::Erf((x+33.25832)/36.40746)-0.41621;
      else if (idx == 84  ) num = 2.11504*TMath::Erf((x+24.62444)/29.20136)-1.11551;
      else if (idx == 85  ) num = 1.41623*TMath::Erf((x+21.80484)/27.88040)-0.41627;
      else if (idx == 86  ) num = 1.41643*TMath::Erf((x+28.20930)/32.68967)-0.41603;
      else if (idx == 87  ) num = 1.41611*TMath::Erf((x+18.87426)/27.32932)-0.41640;
      else if (idx == 88  ) num = 1.41591*TMath::Erf((x+17.42063)/26.69684)-0.41661;
      else if (idx == 89  ) num = 2.17524*TMath::Erf((x+21.52705)/27.00506)-1.17637;
      else if (idx == 90  ) num = 1.41618*TMath::Erf((x+20.45636)/28.00278)-0.41632;
      else if (idx == 91  ) num = 2.12692*TMath::Erf((x+25.28303)/29.25771)-1.12920;
      else if (idx == 92  ) num = 1.41533*TMath::Erf((x+17.42283)/25.13643)-0.41718;
      else if (idx == 93  ) num = 2.12816*TMath::Erf((x+27.08619)/30.46369)-1.13006;
      else if (idx == 94  ) num = 1.41469*TMath::Erf((x+17.32450)/25.27382)-0.41782;
      else if (idx == 95  ) num = 2.26414*TMath::Erf((x+16.13904)/23.25960)-1.26578;
      else if (idx == 96  ) num = 2.08983*TMath::Erf((x+29.67603)/31.71115)-1.09113;
      else if (idx == 97  ) num = 1.41536*TMath::Erf((x+20.06576)/27.49716)-0.41714;
      else if (idx == 98  ) num = 1.41631*TMath::Erf((x+20.44752)/27.79557)-0.41620;
      else if (idx == 99  ) num = 2.16187*TMath::Erf((x+15.22778)/22.23192)-1.16569;
      else if (idx == 100 ) num = 2.00139*TMath::Erf((x+31.06785)/33.60168)-1.00177;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1 || idx == -2) num = 0.53764*TMath::Erf((x+5.90601)/20.02128)+0.46236;
   } else {
      if (idx == 1   ) num = 0.16166*TMath::Erf((x+2.28203)/20.83571)+0.83434;
      else if (idx == 2   ) num = 0.25652*TMath::Erf((x+17.75042)/30.25816)+0.73989;
      else if (idx == 3   ) num = 0.16808*TMath::Erf((x-7.80363)/16.64177)+0.83102;
      else if (idx == 4   ) num = 0.03045*TMath::Erf((x-24.23896)/10.16521)+0.96645;
      else if (idx == 5   ) num = 0.17371*TMath::Erf((x-2.62702)/19.27493)+0.82241;
      else if (idx == 6   ) num = 0.12807*TMath::Erf((x-4.30511)/18.72813)+0.87000;
      else if (idx == 7   ) num = 0.30726*TMath::Erf((x-3.23470)/17.94260)+0.69142;
      else if (idx == 8   ) num = 0.10033*TMath::Erf((x-17.46687)/12.36281)+0.89617;
      else if (idx == 9   ) num = 0.02175*TMath::Erf((x-30.79439)/5.00004)+0.97657;
      else if (idx == 10  ) num = 1.59015*TMath::Erf((x+43.69340)/39.16272)-0.59091;
      else if (idx == 11  ) num = 1.29075*TMath::Erf((x+18.06936)/25.83433)-0.29118;
      else if (idx == 12  ) num = 0.61815*TMath::Erf((x+3.40529)/16.06791)+0.37928;
      else if (idx == 13  ) num = 0.61717*TMath::Erf((x+13.31507)/25.64181)+0.37845;
      else if (idx == 14  ) num = 0.19866*TMath::Erf((x+6.43817)/23.55413)+0.79895;
      else if (idx == 15  ) num = 0.05536*TMath::Erf((x-12.27746)/19.13998)+0.94331;
      else if (idx == 16  ) num = 0.27440*TMath::Erf((x-2.63241)/17.25799)+0.72140;
      else if (idx == 17  ) num = 0.07139*TMath::Erf((x-14.74496)/14.66403)+0.92720;
      else if (idx == 18  ) num = 0.25398*TMath::Erf((x+4.91819)/25.44079)+0.74571;
      else if (idx == 19  ) num = 1.36545*TMath::Erf((x+24.67877)/28.22442)-0.36638;
      else if (idx == 20  ) num = 0.11320*TMath::Erf((x-5.63609)/20.49492)+0.88584;
      else if (idx == 21  ) num = 0.02461*TMath::Erf((x-30.10226)/14.05638)+0.96939;
      else if (idx == 22  ) num = 0.22127*TMath::Erf((x+1.60001)/22.05875)+0.77716;
      else if (idx == 23  ) num = 0.61848*TMath::Erf((x+7.98706)/20.42550)+0.37988;
      else if (idx == 24  ) num = 1.32354*TMath::Erf((x+23.02198)/27.02408)-0.32408;
      else if (idx == 25  ) num = 0.61776*TMath::Erf((x+6.55194)/21.18910)+0.37881;
      else if (idx == 26  ) num = 1.48233*TMath::Erf((x+37.26128)/35.22342)-0.48410;
      else if (idx == 27  ) num = 1.34895*TMath::Erf((x+15.83662)/23.63034)-0.35024;
      else if (idx == 28  ) num = 0.61823*TMath::Erf((x+7.33832)/20.21959)+0.37951;
      else if (idx == 29  ) num = 1.34966*TMath::Erf((x+13.85725)/22.14959)-0.35405;
      else if (idx == 30  ) num = 0.16885*TMath::Erf((x-4.14354)/17.10051)+0.82862;
      else if (idx == 31  ) num = 1.41327*TMath::Erf((x+19.70779)/25.72498)-0.41728;
      else if (idx == 32  ) num = 1.29087*TMath::Erf((x+14.61114)/22.04933)-0.29141;
      else if (idx == 33  ) num = 1.22662*TMath::Erf((x+29.03953)/30.45484)-0.22822;
      else if (idx == 34  ) num = 1.18434*TMath::Erf((x+16.82725)/24.38150)-0.18646;
      else if (idx == 35  ) num = 0.05216*TMath::Erf((x-21.09923)/10.48513)+0.94732;
      else if (idx == 36  ) num = 1.07476*TMath::Erf((x+19.02057)/25.49576)-0.07631;
      else if (idx == 37  ) num = 1.32636*TMath::Erf((x+17.06582)/24.27775)-0.32852;
      else if (idx == 38  ) num = 0.29837*TMath::Erf((x-0.17233)/19.31086)+0.69797;
      else if (idx == 39  ) num = 0.09272*TMath::Erf((x-3.01198)/24.07051)+0.90601;
      else if (idx == 40  ) num = 0.33718*TMath::Erf((x+4.24304)/21.33898)+0.66282;
      else if (idx == 41  ) num = 0.03414*TMath::Erf((x-22.65553)/11.68517)+0.96505;
      else if (idx == 42  ) num = 0.08238*TMath::Erf((x-11.33628)/17.59945)+0.91621;
      else if (idx == 43  ) num = 0.06985*TMath::Erf((x-18.28082)/13.06876)+0.92841;
      else if (idx == 44  ) num = 0.10712*TMath::Erf((x-7.61605)/20.11904)+0.89279;
      else if (idx == 45  ) num = 0.10712*TMath::Erf((x-7.61605)/20.11904)+0.89279;
      // else if (idx == 45  ) num = 0.53263*TMath::Erf((x-50.00000)/50.00000)+1.22640;
      else if (idx == 46  ) num = 1.08623*TMath::Erf((x+18.91129)/25.56354)-0.08627;
      else if (idx == 47  ) num = 0.61738*TMath::Erf((x+2.84681)/18.40465)+0.37786;
      else if (idx == 48  ) num = 0.02295*TMath::Erf((x-31.47247)/5.00000)+0.97591;
      else if (idx == 49  ) num = 0.01967*TMath::Erf((x-30.37335)/5.00000)+0.97623;
      else if (idx == 50  ) num = 0.97054*TMath::Erf((x+17.57442)/26.68699)+0.02894;
      else if (idx == 51  ) num = 0.61747*TMath::Erf((x+6.76825)/19.98476)+0.37856;
      else if (idx == 52  ) num = 0.19353*TMath::Erf((x-0.62386)/19.38863)+0.80437;
      else if (idx == 53  ) num = 0.10473*TMath::Erf((x-14.43075)/13.53575)+0.89399;
      else if (idx == 54  ) num = 0.02953*TMath::Erf((x-23.48674)/8.37529)+0.96726;
      else if (idx == 55  ) num = 0.16686*TMath::Erf((x-0.48211)/24.01919)+0.83195;
      else if (idx == 56  ) num = 0.39890*TMath::Erf((x+12.40050)/27.58163)+0.59808;
      else if (idx == 57  ) num = 0.07684*TMath::Erf((x-8.56368)/20.14222)+0.92086;
      else if (idx == 58  ) num = 0.03452*TMath::Erf((x-23.86514)/8.20173)+0.96462;
      else if (idx == 59  ) num = 0.07251*TMath::Erf((x-8.28552)/23.12116)+0.92305;
      else if (idx == 60  ) num = 0.02497*TMath::Erf((x-29.72792)/5.00000)+0.97352;
      else if (idx == 61  ) num = 1.42432*TMath::Erf((x+10.47240)/19.29263)-0.42706;
      else if (idx == 62  ) num = 0.61871*TMath::Erf((x+8.66942)/21.64536)+0.38010;
      else if (idx == 63  ) num = 0.18563*TMath::Erf((x+6.55976)/24.97479)+0.81422;
      else if (idx == 64  ) num = 0.03479*TMath::Erf((x-19.70083)/12.29988)+0.96101;
      else if (idx == 65  ) num = 0.61754*TMath::Erf((x+8.45293)/19.68537)+0.37870;
      else if (idx == 66  ) num = 0.15615*TMath::Erf((x-3.90570)/20.50199)+0.83934;
      else if (idx == 67  ) num = 0.01694*TMath::Erf((x-30.16404)/5.00004)+0.97980;
      else if (idx == 68  ) num = 0.04848*TMath::Erf((x-21.31771)/7.99701)+0.94810;
      else if (idx == 69  ) num = 0.02506*TMath::Erf((x-29.17738)/5.00000)+0.97409;
      else if (idx == 70  ) num = 1.00481*TMath::Erf((x+24.82713)/29.70995)-0.00781;
      else if (idx == 71  ) num = 0.61777*TMath::Erf((x+5.80954)/17.96780)+0.37893;
      else if (idx == 72  ) num = 0.16199*TMath::Erf((x-9.34155)/15.14716)+0.83393;
      else if (idx == 73  ) num = 1.19197*TMath::Erf((x+43.49437)/40.21606)-0.19365;
      else if (idx == 74  ) num = 0.12043*TMath::Erf((x+8.50433)/29.67195)+0.87520;
      else if (idx == 75  ) num = 0.03636*TMath::Erf((x-21.76429)/6.23450)+0.95864;
      else if (idx == 76  ) num = 1.70030*TMath::Erf((x+24.10258)/28.88419)-0.70127;
      else if (idx == 77  ) num = 0.27027*TMath::Erf((x-5.61800)/16.62969)+0.72841;
      else if (idx == 78  ) num = 0.03032*TMath::Erf((x-19.17034)/15.12005)+0.96627;
      else if (idx == 79  ) num = 0.61802*TMath::Erf((x+9.34149)/20.13981)+0.37932;
      else if (idx == 80  ) num = 1.37490*TMath::Erf((x+23.91490)/30.16779)-0.37443;
      else if (idx == 81  ) num = 0.23924*TMath::Erf((x-6.33108)/16.15960)+0.75873;
      else if (idx == 82  ) num = 0.10849*TMath::Erf((x-13.62503)/15.06765)+0.89011;
      else if (idx == 83  ) num = 1.40047*TMath::Erf((x+15.38132)/22.70852)-0.40351;
      else if (idx == 84  ) num = 1.48427*TMath::Erf((x+32.00819)/32.86476)-0.48472;
      else if (idx == 85  ) num = 1.41241*TMath::Erf((x+19.00379)/26.05688)-0.41485;
      else if (idx == 86  ) num = 1.26866*TMath::Erf((x+16.84176)/24.42223)-0.27007;
      else if (idx == 87  ) num = 1.36573*TMath::Erf((x+17.31610)/24.47413)-0.36590;
      else if (idx == 88  ) num = 0.61703*TMath::Erf((x+10.28451)/20.62689)+0.37806;
      else if (idx == 89  ) num = 1.20146*TMath::Erf((x+18.32046)/25.17788)-0.20389;
      else if (idx == 90  ) num = 0.61761*TMath::Erf((x+2.84335)/15.02451)+0.37851;
      else if (idx == 91  ) num = 1.24173*TMath::Erf((x+12.86365)/23.42062)-0.24350;
      else if (idx == 92  ) num = 1.30337*TMath::Erf((x+25.55312)/30.06211)-0.30566;
      else if (idx == 93  ) num = 0.08527*TMath::Erf((x-20.07623)/10.12749)+0.91162;
      else if (idx == 94  ) num = 1.00835*TMath::Erf((x+32.98816)/33.83878)-0.01064;
      else if (idx == 95  ) num = 1.35389*TMath::Erf((x+5.73875)/17.98349)-0.35730;
      else if (idx == 96  ) num = 1.33800*TMath::Erf((x+16.44290)/23.29724)-0.33949;
      else if (idx == 97  ) num = 1.35252*TMath::Erf((x+18.61823)/26.03634)-0.35390;
      else if (idx == 98  ) num = 0.25323*TMath::Erf((x+1.24818)/20.88480)+0.74635;
      else if (idx == 99  ) num = 1.51834*TMath::Erf((x+33.49696)/33.24380)-0.51951;
      else if (idx == 100 ) num = 0.61802*TMath::Erf((x+3.51451)/16.67335)+0.37908;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1 || idx == -2) num = 0.90579*TMath::Erf((x+23.72352)/29.82791)+0.09435;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;

   if (idx==-1) return num / den_template;
   if (idx==-2) return 2*tnp_weight_isotk_ppb(pt,eta,0) - (num / den_template);

   return num / den;
}

///////////////////////////////////////////////////
//                 I S O     P P b               //
///////////////////////////////////////////////////
double tnp_weight_iso_ppb(double pt, double eta, int idx) {
   double x = pt;

   if (idx == -10) {
      if (eta<0) {
         if (pt<12) return 0.912047;
         else if (pt<15) return 0.993451;
         else if (pt<20) return 0.919913;
         else if (pt<25) return 0.963868;
         else if (pt<30) return 0.969365;
         else if (pt<35) return 0.97824;
         else if (pt<40) return 0.982915;
         else if (pt<45) return 0.99278;
         else if (pt<50) return 0.993982;
         else if (pt<60) return 0.992292;
         else if (pt<70) return 0.995669;
         else if (pt<80) return 0.992766;
         else if (pt<100) return 0.999952;
         else return 0.998071;
      } else if (fabs(eta)<1.2) {
         // 0 < |eta| < 1.2
         if (pt<12) return 0.719202;
         else if (pt<15) return 0.92157;
         else if (pt<20) return 0.920744;
         else if (pt<25) return 0.973847;
         else if (pt<30) return 0.966376;
         else if (pt<35) return 0.97827;
         else if (pt<40) return 0.978048;
         else if (pt<45) return 0.993797;
         else if (pt<50) return 0.994795;
         else if (pt<60) return 0.987962;
         else if (pt<70) return 0.995603;
         else if (pt<80) return 0.996229;
         else if (pt<100) return 0.995042;
         else return 1.00572;
      } else if (fabs(eta)<2.1) {
         // 1.2 < |eta| < 2.1
         if (pt<12) return 0.946958;
         else if (pt<15) return 1.0376;
         else if (pt<20) return 0.914804;
         else if (pt<25) return 0.962558;
         else if (pt<30) return 0.969656;
         else if (pt<35) return 0.977249;
         else if (pt<40) return 0.993305;
         else if (pt<45) return 0.990594;
         else if (pt<50) return 0.995777;
         else if (pt<60) return 0.998647;
         else if (pt<70) return 0.998415;
         else if (pt<80) return 0.984835;
         else if (pt<100) return 1.00602;
         else return 1.00334;
      } else {
         // 2.1 < |eta| < 2.4
         if (pt<12) return 1.02393;
         else if (pt<15) return 0.970897;
         else if (pt<25) return 0.957169;
         else if (pt<40) return 0.983059;
         else if (pt<80) return 0.994825;
         else return 0.977672;
      }
   }

   double num = 1, den = 1;

   // nominal
   if (eta<0) {
      num = 0.28382*TMath::Erf((x-9.54002)/25.01660)+0.70597;
      den = 0.81009*TMath::Erf((x+14.69508)/33.90306)+0.18385;
   } else if (fabs(eta)<1.2) {
      num = 1.77612*TMath::Erf((x+19.08317)/34.99496)-0.78366;
      den = 0.81477*TMath::Erf((x+8.12792)/29.50211)+0.17742;
   } else if (fabs(eta)<2.1) {
      num = 0.18346*TMath::Erf((x-16.06473)/21.36595)+0.80830;
      den = 0.81190*TMath::Erf((x+12.25376)/29.86254)+0.18040;
   } else {
      num = 0.09843*TMath::Erf((x-21.10068)/17.36898)+0.88579;
      den = 0.81118*TMath::Erf((x+15.73925)/29.86169)+0.18118;
   }

   // 100 variations
   if (eta<0) {
      if (idx == 1   ) num = 0.14541*TMath::Erf((x-21.55646)/18.41176)+0.84460;
      else if (idx == 2   ) num = 0.18016*TMath::Erf((x-19.03202)/18.29338)+0.80525;
      else if (idx == 3   ) num = 0.45948*TMath::Erf((x-1.46612)/28.99659)+0.53157;
      else if (idx == 4   ) num = 0.22762*TMath::Erf((x-13.98164)/21.72317)+0.75712;
      else if (idx == 5   ) num = 0.44437*TMath::Erf((x-2.57114)/27.40636)+0.54506;
      else if (idx == 6   ) num = 0.39352*TMath::Erf((x-5.17292)/25.85766)+0.59474;
      else if (idx == 7   ) num = 1.61942*TMath::Erf((x+30.54563)/43.17846)-0.62625;
      else if (idx == 8   ) num = 0.15129*TMath::Erf((x-20.50106)/19.50748)+0.83498;
      else if (idx == 9   ) num = 0.80295*TMath::Erf((x+9.59949)/33.03408)+0.19106;
      else if (idx == 10  ) num = 0.25612*TMath::Erf((x-10.86719)/25.38507)+0.73164;
      else if (idx == 11  ) num = 1.70498*TMath::Erf((x+28.04487)/40.61566)-0.71294;
      else if (idx == 12  ) num = 0.16737*TMath::Erf((x-18.99887)/19.30480)+0.81913;
      else if (idx == 13  ) num = 0.66747*TMath::Erf((x+4.61936)/30.15998)+0.32304;
      else if (idx == 14  ) num = 0.14877*TMath::Erf((x-21.88052)/14.80133)+0.83642;
      else if (idx == 15  ) num = 0.29905*TMath::Erf((x-10.43086)/22.51465)+0.68712;
      else if (idx == 16  ) num = 0.42786*TMath::Erf((x-1.29435)/29.06866)+0.56165;
      else if (idx == 17  ) num = 0.19944*TMath::Erf((x-15.42216)/22.42035)+0.79019;
      else if (idx == 18  ) num = 0.14636*TMath::Erf((x-21.39383)/18.32799)+0.84202;
      else if (idx == 19  ) num = 0.15957*TMath::Erf((x-20.77211)/19.32059)+0.83067;
      else if (idx == 20  ) num = 0.22357*TMath::Erf((x-14.53639)/22.40536)+0.76663;
      else if (idx == 21  ) num = 0.24928*TMath::Erf((x-13.04108)/23.96771)+0.74121;
      else if (idx == 22  ) num = 0.15678*TMath::Erf((x-19.98520)/21.21124)+0.83434;
      else if (idx == 23  ) num = 0.18854*TMath::Erf((x-17.58823)/19.33462)+0.79632;
      else if (idx == 24  ) num = 0.16922*TMath::Erf((x-19.15236)/18.74427)+0.81839;
      else if (idx == 25  ) num = 0.28617*TMath::Erf((x-10.34982)/24.20523)+0.70236;
      else if (idx == 26  ) num = 0.25856*TMath::Erf((x-12.24448)/23.75835)+0.73096;
      else if (idx == 27  ) num = 0.20049*TMath::Erf((x-16.12506)/20.85728)+0.78798;
      else if (idx == 28  ) num = 1.39406*TMath::Erf((x+20.71716)/36.42020)-0.40426;
      else if (idx == 29  ) num = 0.27556*TMath::Erf((x-11.59785)/22.95866)+0.71307;
      else if (idx == 30  ) num = 0.17297*TMath::Erf((x-19.18036)/18.40274)+0.81429;
      else if (idx == 31  ) num = 0.20093*TMath::Erf((x-17.30437)/19.74008)+0.78813;
      else if (idx == 32  ) num = 0.72255*TMath::Erf((x+10.75110)/35.49277)+0.27029;
      else if (idx == 33  ) num = 0.23814*TMath::Erf((x-11.21056)/25.97291)+0.75342;
      else if (idx == 34  ) num = 0.18659*TMath::Erf((x-15.83672)/21.11006)+0.79912;
      else if (idx == 35  ) num = 0.38505*TMath::Erf((x-5.00669)/26.41084)+0.60340;
      else if (idx == 36  ) num = 0.26378*TMath::Erf((x-12.22766)/23.66270)+0.72788;
      else if (idx == 37  ) num = 0.34685*TMath::Erf((x-5.36742)/28.49628)+0.64591;
      else if (idx == 38  ) num = 0.35737*TMath::Erf((x-3.14689)/29.71584)+0.63378;
      else if (idx == 39  ) num = 0.18476*TMath::Erf((x-16.34446)/23.60495)+0.80642;
      else if (idx == 40  ) num = 0.14817*TMath::Erf((x-21.57105)/17.96592)+0.84004;
      else if (idx == 41  ) num = 0.36432*TMath::Erf((x-5.01011)/27.33327)+0.62514;
      else if (idx == 42  ) num = 0.27308*TMath::Erf((x-10.47406)/25.85647)+0.71905;
      else if (idx == 43  ) num = 0.20969*TMath::Erf((x-15.56398)/21.84747)+0.77645;
      else if (idx == 44  ) num = 0.26632*TMath::Erf((x-11.04768)/24.35181)+0.72331;
      else if (idx == 45  ) num = 0.17649*TMath::Erf((x-18.97639)/19.74155)+0.80938;
      else if (idx == 46  ) num = 0.12452*TMath::Erf((x-24.24730)/17.45154)+0.86467;
      else if (idx == 47  ) num = 1.48320*TMath::Erf((x+23.70488)/38.26786)-0.49068;
      else if (idx == 48  ) num = 0.83913*TMath::Erf((x+13.41196)/36.32411)+0.15456;
      else if (idx == 49  ) num = 0.24899*TMath::Erf((x-12.38262)/23.40254)+0.74049;
      else if (idx == 50  ) num = 0.36043*TMath::Erf((x-5.19325)/28.12752)+0.63157;
      else if (idx == 51  ) num = 0.42169*TMath::Erf((x-3.92679)/26.25538)+0.56590;
      else if (idx == 52  ) num = 0.71704*TMath::Erf((x+9.30255)/34.81211)+0.27608;
      else if (idx == 53  ) num = 0.29488*TMath::Erf((x-9.02111)/25.19228)+0.69326;
      else if (idx == 54  ) num = 0.16144*TMath::Erf((x-19.74633)/19.37556)+0.82741;
      else if (idx == 55  ) num = 0.23431*TMath::Erf((x-10.75311)/26.60064)+0.75538;
      else if (idx == 56  ) num = 0.19338*TMath::Erf((x-18.01416)/19.24573)+0.79320;
      else if (idx == 57  ) num = 0.25831*TMath::Erf((x-13.56195)/21.29294)+0.73064;
      else if (idx == 58  ) num = 0.26658*TMath::Erf((x-11.37320)/24.85496)+0.72493;
      else if (idx == 59  ) num = 0.13751*TMath::Erf((x-22.65792)/16.74203)+0.84938;
      else if (idx == 60  ) num = 0.22209*TMath::Erf((x-14.08624)/21.37985)+0.76509;
      else if (idx == 61  ) num = 0.15260*TMath::Erf((x-19.64860)/20.52382)+0.83609;
      else if (idx == 62  ) num = 0.26385*TMath::Erf((x-12.55610)/21.83425)+0.72333;
      else if (idx == 63  ) num = 0.19597*TMath::Erf((x-17.67749)/18.52659)+0.78853;
      else if (idx == 64  ) num = 0.15822*TMath::Erf((x-20.42988)/18.50990)+0.83151;
      else if (idx == 65  ) num = 0.36022*TMath::Erf((x-3.06312)/30.13268)+0.63128;
      else if (idx == 66  ) num = 0.18023*TMath::Erf((x-17.71936)/20.91291)+0.80691;
      else if (idx == 67  ) num = 0.23530*TMath::Erf((x-14.09815)/22.58591)+0.75392;
      else if (idx == 68  ) num = 0.11049*TMath::Erf((x-25.17636)/14.88154)+0.87731;
      else if (idx == 69  ) num = 0.28856*TMath::Erf((x-10.23023)/24.10193)+0.70185;
      else if (idx == 70  ) num = 0.25158*TMath::Erf((x-12.37756)/24.14159)+0.73962;
      else if (idx == 71  ) num = 0.29231*TMath::Erf((x-9.06565)/25.93218)+0.69757;
      else if (idx == 72  ) num = 1.63386*TMath::Erf((x+26.62669)/39.12635)-0.64335;
      else if (idx == 73  ) num = 0.40740*TMath::Erf((x-3.44636)/26.88253)+0.58107;
      else if (idx == 74  ) num = 0.60499*TMath::Erf((x+4.69072)/31.03044)+0.38685;
      else if (idx == 75  ) num = 0.15776*TMath::Erf((x-19.30373)/20.65819)+0.83191;
      else if (idx == 76  ) num = 0.22782*TMath::Erf((x-12.48185)/24.93939)+0.76289;
      else if (idx == 77  ) num = 0.59488*TMath::Erf((x+4.71244)/31.32739)+0.39636;
      else if (idx == 78  ) num = 0.12809*TMath::Erf((x-22.94765)/16.62538)+0.85550;
      else if (idx == 79  ) num = 0.22413*TMath::Erf((x-13.83282)/22.11084)+0.76381;
      else if (idx == 80  ) num = 0.21379*TMath::Erf((x-15.69904)/20.83484)+0.77367;
      else if (idx == 81  ) num = 0.24271*TMath::Erf((x-12.15332)/23.77466)+0.74544;
      else if (idx == 82  ) num = 0.58150*TMath::Erf((x+4.68386)/31.62280)+0.40967;
      else if (idx == 83  ) num = 0.94194*TMath::Erf((x+11.62785)/33.17436)+0.04623;
      else if (idx == 84  ) num = 0.55193*TMath::Erf((x+0.24761)/27.72150)+0.43511;
      else if (idx == 85  ) num = 0.20971*TMath::Erf((x-16.48535)/21.17222)+0.77935;
      else if (idx == 86  ) num = 0.22862*TMath::Erf((x-13.79538)/23.47281)+0.76398;
      else if (idx == 87  ) num = 1.48918*TMath::Erf((x+21.13857)/36.86808)-0.49851;
      else if (idx == 88  ) num = 0.21121*TMath::Erf((x-15.52563)/21.82299)+0.77665;
      else if (idx == 89  ) num = 0.23875*TMath::Erf((x-12.12129)/24.79063)+0.75056;
      else if (idx == 90  ) num = 0.16764*TMath::Erf((x-18.34706)/21.41457)+0.82190;
      else if (idx == 91  ) num = 0.46266*TMath::Erf((x-1.89562)/27.77123)+0.52747;
      else if (idx == 92  ) num = 0.28714*TMath::Erf((x-7.32114)/28.41588)+0.70275;
      else if (idx == 93  ) num = 0.14273*TMath::Erf((x-21.10298)/19.16928)+0.84489;
      else if (idx == 94  ) num = 0.16084*TMath::Erf((x-19.71910)/18.68927)+0.82822;
      else if (idx == 95  ) num = 0.13271*TMath::Erf((x-23.42652)/17.62833)+0.85565;
      else if (idx == 96  ) num = 0.24647*TMath::Erf((x-13.40650)/21.78765)+0.74096;
      else if (idx == 97  ) num = 0.24301*TMath::Erf((x-13.64357)/22.50112)+0.74588;
      else if (idx == 98  ) num = 0.13301*TMath::Erf((x-23.71850)/16.70738)+0.85467;
      else if (idx == 99  ) num = 0.31715*TMath::Erf((x-7.00357)/28.24773)+0.67639;
      else if (idx == 100 ) num = 0.29882*TMath::Erf((x-12.40678)/20.42987)+0.68830;
      else if (idx == -1 || idx == -2) num = 0.77463*TMath::Erf((x+9.33600)/34.18691)+0.21880;
   } else if (fabs(eta)<1.2) {
      if (idx == 1   ) num = 2.26242*TMath::Erf((x+24.14104)/36.70030)-1.27116;
      else if (idx == 2   ) num = 0.39612*TMath::Erf((x-6.56788)/26.73571)+0.59422;
      else if (idx == 3   ) num = 2.78269*TMath::Erf((x+29.73016)/39.16871)-1.79245;
      else if (idx == 4   ) num = 1.77340*TMath::Erf((x+14.84083)/31.20312)-0.78667;
      else if (idx == 5   ) num = 1.77388*TMath::Erf((x+16.10939)/32.88930)-0.78616;
      else if (idx == 6   ) num = 1.77298*TMath::Erf((x+15.05899)/31.93905)-0.78710;
      else if (idx == 7   ) num = 0.49398*TMath::Erf((x-5.91938)/24.11693)+0.49242;
      else if (idx == 8   ) num = 1.77442*TMath::Erf((x+20.15494)/35.36772)-0.78525;
      else if (idx == 9   ) num = 1.77442*TMath::Erf((x+23.92060)/38.09911)-0.78493;
      else if (idx == 10  ) num = 1.77839*TMath::Erf((x+22.50398)/38.20994)-0.78103;
      else if (idx == 11  ) num = 2.83888*TMath::Erf((x+21.28634)/33.66463)-1.85006;
      else if (idx == 12  ) num = 1.77436*TMath::Erf((x+15.50155)/31.78044)-0.78567;
      else if (idx == 13  ) num = 1.77203*TMath::Erf((x+14.66284)/30.56273)-0.78804;
      else if (idx == 14  ) num = 2.69644*TMath::Erf((x+24.22142)/35.69077)-1.70829;
      else if (idx == 15  ) num = 0.19141*TMath::Erf((x-19.80228)/17.82634)+0.79393;
      else if (idx == 16  ) num = 1.77361*TMath::Erf((x+20.69019)/35.73832)-0.78599;
      else if (idx == 17  ) num = 2.69466*TMath::Erf((x+24.60361)/35.47958)-1.70430;
      else if (idx == 18  ) num = 2.64754*TMath::Erf((x+19.80129)/32.74410)-1.65694;
      else if (idx == 19  ) num = 2.75597*TMath::Erf((x+29.18206)/39.14339)-1.76325;
      else if (idx == 20  ) num = 1.77553*TMath::Erf((x+19.13931)/35.03508)-0.78425;
      else if (idx == 21  ) num = 2.56819*TMath::Erf((x+20.33780)/32.36336)-1.58243;
      else if (idx == 22  ) num = 0.17313*TMath::Erf((x-21.22848)/17.43989)+0.81565;
      else if (idx == 23  ) num = 1.77532*TMath::Erf((x+19.93095)/35.47009)-0.78438;
      else if (idx == 24  ) num = 0.56750*TMath::Erf((x-4.73689)/23.52037)+0.41555;
      else if (idx == 25  ) num = 2.81341*TMath::Erf((x+20.42777)/32.58330)-1.82607;
      else if (idx == 26  ) num = 1.77440*TMath::Erf((x+21.17660)/36.10635)-0.78520;
      else if (idx == 27  ) num = 2.60321*TMath::Erf((x+24.72642)/36.38877)-1.61427;
      else if (idx == 28  ) num = 0.37469*TMath::Erf((x-11.44322)/20.35042)+0.60851;
      else if (idx == 29  ) num = 1.77511*TMath::Erf((x+22.36432)/37.18154)-0.78435;
      else if (idx == 30  ) num = 2.65673*TMath::Erf((x+30.03603)/39.90602)-1.66362;
      else if (idx == 31  ) num = 0.43099*TMath::Erf((x-4.79419)/28.80877)+0.56305;
      else if (idx == 32  ) num = 2.48978*TMath::Erf((x+35.75392)/44.37971)-1.49476;
      else if (idx == 33  ) num = 1.77597*TMath::Erf((x+20.47948)/36.00327)-0.78368;
      else if (idx == 34  ) num = 0.63862*TMath::Erf((x-2.60946)/24.15635)+0.34484;
      else if (idx == 35  ) num = 2.55831*TMath::Erf((x+24.95497)/35.92487)-1.57045;
      else if (idx == 36  ) num = 0.31864*TMath::Erf((x-14.45623)/19.02013)+0.66826;
      else if (idx == 37  ) num = 0.48779*TMath::Erf((x-3.02458)/28.23691)+0.50231;
      else if (idx == 38  ) num = 2.56640*TMath::Erf((x+26.84777)/37.78364)-1.57814;
      else if (idx == 39  ) num = 2.62516*TMath::Erf((x+32.19358)/41.39383)-1.63239;
      else if (idx == 40  ) num = 1.77420*TMath::Erf((x+12.26197)/29.75827)-0.78619;
      else if (idx == 41  ) num = 1.77663*TMath::Erf((x+20.17673)/36.21616)-0.78302;
      else if (idx == 42  ) num = 0.27383*TMath::Erf((x-15.30882)/20.75131)+0.71268;
      else if (idx == 43  ) num = 2.64126*TMath::Erf((x+29.23668)/39.22850)-1.65016;
      else if (idx == 44  ) num = 1.77507*TMath::Erf((x+13.90830)/31.20543)-0.78513;
      else if (idx == 45  ) num = 2.65342*TMath::Erf((x+27.98929)/38.24620)-1.66263;
      else if (idx == 46  ) num = 0.47180*TMath::Erf((x-5.68641)/24.50727)+0.51507;
      else if (idx == 47  ) num = 0.64592*TMath::Erf((x-3.44943)/25.12202)+0.34052;
      else if (idx == 48  ) num = 0.18882*TMath::Erf((x-22.25020)/16.05534)+0.79798;
      else if (idx == 49  ) num = 2.72234*TMath::Erf((x+27.88170)/38.44349)-1.73010;
      else if (idx == 50  ) num = 1.77285*TMath::Erf((x+17.83476)/32.78528)-0.78703;
      else if (idx == 51  ) num = 1.77640*TMath::Erf((x+20.40372)/36.31944)-0.78324;
      else if (idx == 52  ) num = 1.77486*TMath::Erf((x+19.21873)/34.75947)-0.78489;
      else if (idx == 53  ) num = 2.68124*TMath::Erf((x+31.14037)/40.39683)-1.68726;
      else if (idx == 54  ) num = 0.55953*TMath::Erf((x-2.29040)/27.92057)+0.43154;
      else if (idx == 55  ) num = 2.70739*TMath::Erf((x+24.08032)/35.70239)-1.71457;
      else if (idx == 56  ) num = 2.66752*TMath::Erf((x+21.49074)/33.89081)-1.67776;
      else if (idx == 57  ) num = 0.31434*TMath::Erf((x-13.14865)/21.04364)+0.67314;
      else if (idx == 58  ) num = 2.78996*TMath::Erf((x+27.27104)/36.95523)-1.80175;
      else if (idx == 59  ) num = 1.77390*TMath::Erf((x+20.83260)/35.89987)-0.78573;
      else if (idx == 60  ) num = 0.63206*TMath::Erf((x-2.39598)/25.42494)+0.35644;
      else if (idx == 61  ) num = 2.55290*TMath::Erf((x+29.09462)/38.99916)-1.56402;
      else if (idx == 62  ) num = 2.56900*TMath::Erf((x+31.25893)/40.46081)-1.57684;
      else if (idx == 63  ) num = 1.77665*TMath::Erf((x+20.34709)/35.79849)-0.78305;
      else if (idx == 64  ) num = 0.17088*TMath::Erf((x-23.46761)/15.30450)+0.81878;
      else if (idx == 65  ) num = 1.77520*TMath::Erf((x+22.97527)/37.75808)-0.78418;
      else if (idx == 66  ) num = 0.35533*TMath::Erf((x-9.85499)/24.53720)+0.63293;
      else if (idx == 67  ) num = 1.77438*TMath::Erf((x+17.33822)/33.27796)-0.78552;
      else if (idx == 68  ) num = 2.66473*TMath::Erf((x+21.97521)/33.76567)-1.67537;
      else if (idx == 69  ) num = 2.71088*TMath::Erf((x+31.87157)/40.95862)-1.71758;
      else if (idx == 70  ) num = 2.76338*TMath::Erf((x+28.83533)/38.60385)-1.77361;
      else if (idx == 71  ) num = 2.78962*TMath::Erf((x+24.65434)/35.22975)-1.80007;
      else if (idx == 72  ) num = 1.77341*TMath::Erf((x+19.90529)/34.85218)-0.78630;
      else if (idx == 73  ) num = 1.77502*TMath::Erf((x+24.01072)/38.26460)-0.78431;
      else if (idx == 74  ) num = 2.56722*TMath::Erf((x+24.44985)/35.13284)-1.58315;
      else if (idx == 75  ) num = 2.75534*TMath::Erf((x+32.14003)/40.27544)-1.76442;
      else if (idx == 76  ) num = 2.75236*TMath::Erf((x+21.11003)/32.86419)-1.76748;
      else if (idx == 77  ) num = 2.67672*TMath::Erf((x+24.05728)/35.51869)-1.68463;
      else if (idx == 78  ) num = 1.77314*TMath::Erf((x+17.29635)/32.77250)-0.78675;
      else if (idx == 79  ) num = 2.74969*TMath::Erf((x+23.51268)/34.49441)-1.76196;
      else if (idx == 80  ) num = 2.67019*TMath::Erf((x+24.86481)/35.99282)-1.67982;
      else if (idx == 81  ) num = 1.77374*TMath::Erf((x+19.34131)/34.61905)-0.78601;
      else if (idx == 82  ) num = 2.69988*TMath::Erf((x+28.97278)/38.95978)-1.70904;
      else if (idx == 83  ) num = 0.57852*TMath::Erf((x-4.23223)/25.18110)+0.40594;
      else if (idx == 84  ) num = 2.77438*TMath::Erf((x+24.60370)/36.18402)-1.78351;
      else if (idx == 85  ) num = 2.78848*TMath::Erf((x+24.80583)/35.88517)-1.79656;
      else if (idx == 86  ) num = 2.60346*TMath::Erf((x+29.83386)/39.18991)-1.61285;
      else if (idx == 87  ) num = 2.90782*TMath::Erf((x+30.37698)/39.95912)-1.91165;
      else if (idx == 88  ) num = 1.77403*TMath::Erf((x+16.30211)/32.68684)-0.78595;
      else if (idx == 89  ) num = 1.77406*TMath::Erf((x+18.67617)/33.80158)-0.78574;
      else if (idx == 90  ) num = 2.58604*TMath::Erf((x+24.65532)/36.91045)-1.59298;
      else if (idx == 91  ) num = 1.77568*TMath::Erf((x+23.33105)/38.56288)-0.78363;
      else if (idx == 92  ) num = 0.43433*TMath::Erf((x-8.82239)/21.60671)+0.55211;
      else if (idx == 93  ) num = 0.54582*TMath::Erf((x-1.50868)/28.32384)+0.44303;
      else if (idx == 94  ) num = 1.77481*TMath::Erf((x+19.68543)/34.91189)-0.78493;
      else if (idx == 95  ) num = 2.07948*TMath::Erf((x+25.11930)/37.99808)-1.08712;
      else if (idx == 96  ) num = 1.77701*TMath::Erf((x+28.44668)/41.56411)-0.78190;
      else if (idx == 97  ) num = 0.76876*TMath::Erf((x+6.13243)/31.43197)+0.22151;
      else if (idx == 98  ) num = 0.43308*TMath::Erf((x-8.68974)/23.33452)+0.55474;
      else if (idx == 99  ) num = 2.16070*TMath::Erf((x+14.99328)/29.53756)-1.17609;
      else if (idx == 100 ) num = 2.55592*TMath::Erf((x+19.39600)/32.03372)-1.57130;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1 || idx == -2) num = 1.78971*TMath::Erf((x+16.17029)/33.15068)-0.80060;
   } else if (fabs(eta)<2.1) {
      if (idx == 1   ) num = 0.22503*TMath::Erf((x-13.12795)/23.34890)+0.76580;
      else if (idx == 2   ) num = 0.24845*TMath::Erf((x-11.82518)/23.54526)+0.74156;
      else if (idx == 3   ) num = 0.32175*TMath::Erf((x-4.05520)/27.91180)+0.67023;
      else if (idx == 4   ) num = 0.13348*TMath::Erf((x-22.69087)/15.91990)+0.85686;
      else if (idx == 5   ) num = 0.26900*TMath::Erf((x-10.09353)/22.30544)+0.71781;
      else if (idx == 6   ) num = 0.08673*TMath::Erf((x-27.77485)/14.73774)+0.90563;
      else if (idx == 7   ) num = 0.15138*TMath::Erf((x-20.16127)/17.92086)+0.84148;
      else if (idx == 8   ) num = 0.42998*TMath::Erf((x-0.93539)/27.07512)+0.56379;
      else if (idx == 9   ) num = 0.21158*TMath::Erf((x-15.20249)/18.73748)+0.77277;
      else if (idx == 10  ) num = 0.12352*TMath::Erf((x-23.42056)/17.17045)+0.86853;
      else if (idx == 11  ) num = 0.14710*TMath::Erf((x-21.96492)/16.46367)+0.84456;
      else if (idx == 12  ) num = 0.12688*TMath::Erf((x-20.73976)/18.96228)+0.86453;
      else if (idx == 13  ) num = 0.12075*TMath::Erf((x-24.33036)/15.64379)+0.87043;
      else if (idx == 14  ) num = 0.16054*TMath::Erf((x-17.71974)/20.05010)+0.83233;
      else if (idx == 15  ) num = 0.09478*TMath::Erf((x-25.23917)/15.34918)+0.89524;
      else if (idx == 16  ) num = 0.21194*TMath::Erf((x-14.00869)/22.47334)+0.78103;
      else if (idx == 17  ) num = 0.12600*TMath::Erf((x-21.78480)/17.19377)+0.86170;
      else if (idx == 18  ) num = 0.19130*TMath::Erf((x-14.39244)/22.24158)+0.79821;
      else if (idx == 19  ) num = 0.17319*TMath::Erf((x-17.22365)/18.39547)+0.81635;
      else if (idx == 20  ) num = 0.20828*TMath::Erf((x-14.34709)/19.40087)+0.77566;
      else if (idx == 21  ) num = 0.10558*TMath::Erf((x-23.76424)/17.18825)+0.88363;
      else if (idx == 22  ) num = 0.07314*TMath::Erf((x-26.29771)/12.30971)+0.90923;
      else if (idx == 23  ) num = 0.12677*TMath::Erf((x-19.93314)/18.39581)+0.86231;
      else if (idx == 24  ) num = 0.09195*TMath::Erf((x-26.08973)/12.52034)+0.89063;
      else if (idx == 25  ) num = 0.15763*TMath::Erf((x-17.58881)/19.79759)+0.83015;
      else if (idx == 26  ) num = 0.23023*TMath::Erf((x-12.10264)/24.14765)+0.76094;
      else if (idx == 27  ) num = 0.12685*TMath::Erf((x-22.36643)/16.81946)+0.86117;
      else if (idx == 28  ) num = 0.54599*TMath::Erf((x+0.68303)/26.81423)+0.44095;
      else if (idx == 29  ) num = 0.11733*TMath::Erf((x-23.71453)/15.33388)+0.87156;
      else if (idx == 30  ) num = 0.18658*TMath::Erf((x-17.74702)/17.83519)+0.80306;
      else if (idx == 31  ) num = 0.17243*TMath::Erf((x-17.22387)/20.41216)+0.81946;
      else if (idx == 32  ) num = 0.12958*TMath::Erf((x-22.87332)/15.79333)+0.86222;
      else if (idx == 33  ) num = 0.12018*TMath::Erf((x-23.77550)/15.56211)+0.86971;
      else if (idx == 34  ) num = 0.43454*TMath::Erf((x+0.92352)/29.88408)+0.55645;
      else if (idx == 35  ) num = 0.08192*TMath::Erf((x-28.67478)/12.64801)+0.90814;
      else if (idx == 36  ) num = 0.48747*TMath::Erf((x-2.21112)/26.29120)+0.50810;
      else if (idx == 37  ) num = 0.39592*TMath::Erf((x-4.13163)/24.86411)+0.59396;
      else if (idx == 38  ) num = 1.60913*TMath::Erf((x+24.72101)/37.48753)-0.61793;
      else if (idx == 39  ) num = 0.22256*TMath::Erf((x-15.18561)/20.38992)+0.76622;
      else if (idx == 40  ) num = 0.13986*TMath::Erf((x-20.50340)/16.36611)+0.84524;
      else if (idx == 41  ) num = 0.23933*TMath::Erf((x-9.75744)/26.50820)+0.75847;
      else if (idx == 42  ) num = 0.12883*TMath::Erf((x-22.37389)/17.36666)+0.86524;
      else if (idx == 43  ) num = 0.14301*TMath::Erf((x-20.75886)/16.51517)+0.84552;
      else if (idx == 44  ) num = 0.44011*TMath::Erf((x-1.28885)/27.24413)+0.55016;
      else if (idx == 45  ) num = 0.11256*TMath::Erf((x-23.92851)/18.89258)+0.88021;
      else if (idx == 46  ) num = 0.29880*TMath::Erf((x-8.80300)/24.88932)+0.69182;
      else if (idx == 47  ) num = 0.26400*TMath::Erf((x-10.13472)/25.51715)+0.72731;
      else if (idx == 48  ) num = 0.15689*TMath::Erf((x-20.11354)/20.83395)+0.83823;
      else if (idx == 49  ) num = 0.15433*TMath::Erf((x-20.17535)/16.98568)+0.83368;
      else if (idx == 50  ) num = 0.09657*TMath::Erf((x-24.73727)/15.01728)+0.88853;
      else if (idx == 51  ) num = 0.12681*TMath::Erf((x-22.41250)/17.23782)+0.85848;
      else if (idx == 52  ) num = 0.21093*TMath::Erf((x-15.51277)/18.92149)+0.77667;
      else if (idx == 53  ) num = 0.16763*TMath::Erf((x-18.05721)/20.38850)+0.82460;
      else if (idx == 54  ) num = 0.42269*TMath::Erf((x-4.15539)/25.83915)+0.56955;
      else if (idx == 55  ) num = 0.16584*TMath::Erf((x-16.50071)/21.57278)+0.82402;
      else if (idx == 56  ) num = 0.27200*TMath::Erf((x-10.79674)/24.10510)+0.72031;
      else if (idx == 57  ) num = 0.12318*TMath::Erf((x-22.83762)/18.31534)+0.86970;
      else if (idx == 58  ) num = 0.33205*TMath::Erf((x-6.09779)/23.83705)+0.65887;
      else if (idx == 59  ) num = 0.09213*TMath::Erf((x-27.83341)/13.21197)+0.89888;
      else if (idx == 60  ) num = 0.12593*TMath::Erf((x-23.57091)/14.96216)+0.86053;
      else if (idx == 61  ) num = 0.16649*TMath::Erf((x-18.75248)/18.02217)+0.82185;
      else if (idx == 62  ) num = 0.25660*TMath::Erf((x-11.02800)/21.92508)+0.73022;
      else if (idx == 63  ) num = 0.23763*TMath::Erf((x-13.90106)/20.91595)+0.75315;
      else if (idx == 64  ) num = 0.13662*TMath::Erf((x-22.27745)/15.66375)+0.85429;
      else if (idx == 65  ) num = 0.20407*TMath::Erf((x-16.16714)/19.54420)+0.78492;
      else if (idx == 66  ) num = 0.89793*TMath::Erf((x+11.57524)/32.86046)+0.09535;
      else if (idx == 67  ) num = 0.35276*TMath::Erf((x-2.93222)/27.11971)+0.63702;
      else if (idx == 68  ) num = 0.09548*TMath::Erf((x-25.15927)/13.45556)+0.89282;
      else if (idx == 69  ) num = 0.10125*TMath::Erf((x-24.58338)/15.02849)+0.88538;
      else if (idx == 70  ) num = 0.16501*TMath::Erf((x-17.91384)/21.22839)+0.82983;
      else if (idx == 71  ) num = 0.08955*TMath::Erf((x-27.77159)/12.93546)+0.89695;
      else if (idx == 72  ) num = 0.19978*TMath::Erf((x-16.37550)/18.47343)+0.78646;
      else if (idx == 73  ) num = 0.27751*TMath::Erf((x-10.59685)/22.20881)+0.71230;
      else if (idx == 74  ) num = 0.18805*TMath::Erf((x-16.74730)/20.29939)+0.80642;
      else if (idx == 75  ) num = 0.21716*TMath::Erf((x-12.86510)/21.33632)+0.77056;
      else if (idx == 76  ) num = 0.17230*TMath::Erf((x-19.38947)/19.76519)+0.81978;
      else if (idx == 77  ) num = 0.11522*TMath::Erf((x-22.33285)/16.43933)+0.87257;
      else if (idx == 78  ) num = 0.14354*TMath::Erf((x-20.67668)/17.65836)+0.84269;
      else if (idx == 79  ) num = 0.13930*TMath::Erf((x-21.66679)/17.94784)+0.85107;
      else if (idx == 80  ) num = 0.39949*TMath::Erf((x-2.67012)/26.69960)+0.59088;
      else if (idx == 81  ) num = 0.19218*TMath::Erf((x-18.42384)/18.30712)+0.79887;
      else if (idx == 82  ) num = 0.99772*TMath::Erf((x+10.30007)/28.00629)-0.01343;
      else if (idx == 83  ) num = 0.11951*TMath::Erf((x-23.34235)/14.13474)+0.86361;
      else if (idx == 84  ) num = 0.42343*TMath::Erf((x-4.32356)/26.39638)+0.57158;
      else if (idx == 85  ) num = 0.15764*TMath::Erf((x-19.32273)/19.32258)+0.83207;
      else if (idx == 86  ) num = 0.36027*TMath::Erf((x-4.17961)/26.33444)+0.63327;
      else if (idx == 87  ) num = 0.17070*TMath::Erf((x-18.62637)/19.40238)+0.81994;
      else if (idx == 88  ) num = 0.32008*TMath::Erf((x-8.94998)/24.61948)+0.67394;
      else if (idx == 89  ) num = 0.11309*TMath::Erf((x-22.06489)/18.09033)+0.87538;
      else if (idx == 90  ) num = 0.10872*TMath::Erf((x-23.30612)/15.14226)+0.87584;
      else if (idx == 91  ) num = 0.46036*TMath::Erf((x+0.49337)/28.51590)+0.53308;
      else if (idx == 92  ) num = 0.18741*TMath::Erf((x-14.29817)/23.78779)+0.80679;
      else if (idx == 93  ) num = 0.13325*TMath::Erf((x-20.79728)/15.70993)+0.85247;
      else if (idx == 94  ) num = 0.37651*TMath::Erf((x-2.03573)/28.45198)+0.61726;
      else if (idx == 95  ) num = 0.30222*TMath::Erf((x-8.75564)/24.34993)+0.68966;
      else if (idx == 96  ) num = 0.69107*TMath::Erf((x+11.91460)/34.08074)+0.29804;
      else if (idx == 97  ) num = 0.14125*TMath::Erf((x-21.18642)/16.79883)+0.84726;
      else if (idx == 98  ) num = 0.12112*TMath::Erf((x-21.29301)/15.53264)+0.86165;
      else if (idx == 99  ) num = 0.17613*TMath::Erf((x-16.54956)/17.80761)+0.80454;
      else if (idx == 100 ) num = 0.18881*TMath::Erf((x-16.69549)/19.25720)+0.80050;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1 || idx == -2) num = 0.19648*TMath::Erf((x-16.92807)/20.03593)+0.79412;
   } else {
      if (idx == 1   ) num = 0.05221*TMath::Erf((x-32.47681)/5.00108)+0.93776;
      else if (idx == 2   ) num = 0.07157*TMath::Erf((x-27.43713)/12.97153)+0.91627;
      else if (idx == 3   ) num = 0.05209*TMath::Erf((x-27.76108)/18.76034)+0.91862;
      else if (idx == 4   ) num = 0.12699*TMath::Erf((x-15.72533)/18.82372)+0.85041;
      else if (idx == 5   ) num = 0.05400*TMath::Erf((x-31.99766)/13.83533)+0.91041;
      else if (idx == 6   ) num = 0.11873*TMath::Erf((x-19.46617)/20.46212)+0.85750;
      else if (idx == 7   ) num = 0.10931*TMath::Erf((x-13.21755)/26.55515)+0.85952;
      else if (idx == 8   ) num = 0.08121*TMath::Erf((x-26.05225)/13.18373)+0.90487;
      else if (idx == 9   ) num = 0.17791*TMath::Erf((x-14.83355)/22.99859)+0.78956;
      else if (idx == 10  ) num = 0.10735*TMath::Erf((x-20.73327)/19.87336)+0.86245;
      else if (idx == 11  ) num = 2.14735*TMath::Erf((x+49.99926)/49.56013)-1.15637;
      else if (idx == 12  ) num = 0.10388*TMath::Erf((x-22.48689)/20.88580)+0.84579;
      else if (idx == 13  ) num = 0.09822*TMath::Erf((x-22.58377)/13.78641)+0.89969;
      else if (idx == 14  ) num = 0.13861*TMath::Erf((x-14.32504)/20.68709)+0.83781;
      else if (idx == 15  ) num = 0.05181*TMath::Erf((x-31.08386)/5.00000)+0.92944;
      else if (idx == 16  ) num = 0.08295*TMath::Erf((x-23.86181)/18.61158)+0.88735;
      else if (idx == 17  ) num = 0.06502*TMath::Erf((x-27.08828)/16.34836)+0.91489;
      else if (idx == 18  ) num = 0.29937*TMath::Erf((x-6.49590)/22.99194)+0.69114;
      else if (idx == 19  ) num = 0.13875*TMath::Erf((x-20.22729)/21.73030)+0.82290;
      else if (idx == 20  ) num = 0.49530*TMath::Erf((x+4.57613)/27.94302)+0.49561;
      else if (idx == 21  ) num = 1.76198*TMath::Erf((x+26.68937)/34.05303)-0.77187;
      else if (idx == 22  ) num = 0.12124*TMath::Erf((x-17.43308)/20.41759)+0.85652;
      else if (idx == 23  ) num = 0.03384*TMath::Erf((x-35.13866)/13.22240)+0.93370;
      else if (idx == 24  ) num = 0.11385*TMath::Erf((x-20.12017)/16.96524)+0.87133;
      else if (idx == 25  ) num = 0.09849*TMath::Erf((x-21.27217)/16.84459)+0.88167;
      else if (idx == 26  ) num = 0.06145*TMath::Erf((x-29.81705)/7.47211)+0.92728;
      else if (idx == 27  ) num = 0.12541*TMath::Erf((x-19.98925)/12.04192)+0.85882;
      else if (idx == 28  ) num = 0.14911*TMath::Erf((x-16.02779)/17.42233)+0.83354;
      else if (idx == 29  ) num = 0.27303*TMath::Erf((x-4.74538)/27.71391)+0.72252;
      else if (idx == 30  ) num = 0.07936*TMath::Erf((x-24.75472)/9.72145)+0.90102;
      else if (idx == 31  ) num = 1.62671*TMath::Erf((x+39.62360)/45.68711)-0.63201;
      else if (idx == 32  ) num = 0.08453*TMath::Erf((x-26.62980)/7.42346)+0.89757;
      else if (idx == 33  ) num = 0.08009*TMath::Erf((x-25.94199)/10.86733)+0.90164;
      else if (idx == 34  ) num = 0.10040*TMath::Erf((x-23.82021)/12.90773)+0.88042;
      else if (idx == 35  ) num = 0.17573*TMath::Erf((x-11.98010)/21.28874)+0.81188;
      else if (idx == 36  ) num = 0.05346*TMath::Erf((x-31.43747)/5.00001)+0.93304;
      else if (idx == 37  ) num = 0.13858*TMath::Erf((x-12.05571)/25.52759)+0.85040;
      else if (idx == 38  ) num = 0.26821*TMath::Erf((x-6.82403)/24.63553)+0.70875;
      else if (idx == 39  ) num = 0.52682*TMath::Erf((x+2.84624)/24.77180)+0.44912;
      else if (idx == 40  ) num = 0.07343*TMath::Erf((x-26.61110)/11.48385)+0.91042;
      else if (idx == 41  ) num = 0.13536*TMath::Erf((x-17.32724)/19.80506)+0.85325;
      else if (idx == 42  ) num = 0.09826*TMath::Erf((x-23.39617)/12.85827)+0.88811;
      else if (idx == 43  ) num = 0.14530*TMath::Erf((x-16.86110)/17.69935)+0.83686;
      else if (idx == 44  ) num = 0.07853*TMath::Erf((x-28.91420)/8.88509)+0.90595;
      else if (idx == 45  ) num = 0.10391*TMath::Erf((x-24.67349)/14.36711)+0.88178;
      else if (idx == 46  ) num = 0.33818*TMath::Erf((x+1.62990)/30.09893)+0.64991;
      else if (idx == 47  ) num = 1.42521*TMath::Erf((x+31.57450)/41.08725)-0.42759;
      else if (idx == 48  ) num = 0.07269*TMath::Erf((x-27.71790)/12.58845)+0.91561;
      else if (idx == 49  ) num = 1.86888*TMath::Erf((x+36.12297)/40.37203)-0.89147;
      else if (idx == 50  ) num = 0.05841*TMath::Erf((x-30.43305)/17.15607)+0.90903;
      else if (idx == 51  ) num = 0.18026*TMath::Erf((x-11.72122)/26.58325)+0.81191;
      else if (idx == 52  ) num = 0.18048*TMath::Erf((x-10.32377)/24.51537)+0.78585;
      else if (idx == 53  ) num = 0.03996*TMath::Erf((x-34.19946)/11.74938)+0.92748;
      else if (idx == 54  ) num = 0.07329*TMath::Erf((x-24.74897)/10.86276)+0.90740;
      else if (idx == 55  ) num = 2.19705*TMath::Erf((x+42.15083)/44.24287)-1.20698;
      else if (idx == 56  ) num = 0.05109*TMath::Erf((x-26.78352)/13.97099)+0.94045;
      else if (idx == 57  ) num = 0.10093*TMath::Erf((x-25.45303)/12.83269)+0.89174;
      else if (idx == 58  ) num = 0.58849*TMath::Erf((x+4.41087)/29.01852)+0.38768;
      else if (idx == 59  ) num = 0.19083*TMath::Erf((x-12.17529)/25.02490)+0.77227;
      else if (idx == 60  ) num = 0.26389*TMath::Erf((x-5.57803)/25.46922)+0.71350;
      else if (idx == 61  ) num = 0.26212*TMath::Erf((x-2.38687)/26.98561)+0.71550;
      else if (idx == 62  ) num = 0.05578*TMath::Erf((x-30.19668)/7.36412)+0.93526;
      else if (idx == 63  ) num = 0.06610*TMath::Erf((x-23.77292)/19.76951)+0.90314;
      else if (idx == 64  ) num = 0.12933*TMath::Erf((x-18.50124)/21.71825)+0.85148;
      else if (idx == 65  ) num = 0.06059*TMath::Erf((x-27.59447)/10.52918)+0.93119;
      else if (idx == 66  ) num = 0.14327*TMath::Erf((x-20.17146)/10.44734)+0.83707;
      else if (idx == 67  ) num = 0.43055*TMath::Erf((x+3.16500)/27.73481)+0.55555;
      else if (idx == 68  ) num = 1.70500*TMath::Erf((x+21.62022)/30.35031)-0.72239;
      else if (idx == 69  ) num = 0.10982*TMath::Erf((x-23.38330)/13.73446)+0.88978;
      else if (idx == 70  ) num = 0.14535*TMath::Erf((x-20.70857)/11.93437)+0.84145;
      else if (idx == 71  ) num = 0.12246*TMath::Erf((x-18.10747)/21.34999)+0.86463;
      else if (idx == 72  ) num = 0.05648*TMath::Erf((x-26.77392)/17.65498)+0.91079;
      else if (idx == 73  ) num = 0.04578*TMath::Erf((x-35.18971)/11.89993)+0.92797;
      else if (idx == 74  ) num = 0.08758*TMath::Erf((x-25.74438)/9.98128)+0.88983;
      else if (idx == 75  ) num = 0.05228*TMath::Erf((x-30.91626)/14.15080)+0.91451;
      else if (idx == 76  ) num = 1.57869*TMath::Erf((x+27.75876)/36.67965)-0.59978;
      else if (idx == 77  ) num = 0.07812*TMath::Erf((x-23.64030)/17.57520)+0.90343;
      else if (idx == 78  ) num = 0.27381*TMath::Erf((x-6.16724)/25.86286)+0.71709;
      else if (idx == 79  ) num = 2.01882*TMath::Erf((x+33.72864)/38.73656)-1.02779;
      else if (idx == 80  ) num = 0.05041*TMath::Erf((x-29.66574)/13.85959)+0.92557;
      else if (idx == 81  ) num = 0.04448*TMath::Erf((x-31.62321)/5.00004)+0.93779;
      else if (idx == 82  ) num = 0.13261*TMath::Erf((x-19.83326)/17.51401)+0.85333;
      else if (idx == 83  ) num = 1.39943*TMath::Erf((x+33.60449)/40.92322)-0.40998;
      else if (idx == 84  ) num = 0.09516*TMath::Erf((x-21.40837)/18.74325)+0.87886;
      else if (idx == 85  ) num = 0.44325*TMath::Erf((x+3.84256)/27.63554)+0.54043;
      else if (idx == 86  ) num = 0.10540*TMath::Erf((x-22.36777)/9.71767)+0.87432;
      else if (idx == 87  ) num = 0.17645*TMath::Erf((x-10.53795)/24.00368)+0.79888;
      else if (idx == 88  ) num = 0.68834*TMath::Erf((x+17.87302)/37.29539)+0.29598;
      else if (idx == 89  ) num = 0.08801*TMath::Erf((x-23.32973)/17.39835)+0.89816;
      else if (idx == 90  ) num = 0.07536*TMath::Erf((x-28.09391)/12.17393)+0.91603;
      else if (idx == 91  ) num = 0.11108*TMath::Erf((x-21.83060)/9.57688)+0.86359;
      else if (idx == 92  ) num = 0.14869*TMath::Erf((x-13.93774)/19.78508)+0.83129;
      else if (idx == 93  ) num = 0.07213*TMath::Erf((x-26.70539)/14.42410)+0.91561;
      else if (idx == 94  ) num = 0.05652*TMath::Erf((x-31.57486)/5.00000)+0.92499;
      else if (idx == 95  ) num = 0.71207*TMath::Erf((x+13.53905)/33.54610)+0.27484;
      else if (idx == 96  ) num = 0.15094*TMath::Erf((x-14.63659)/21.20991)+0.83536;
      else if (idx == 97  ) num = 0.04552*TMath::Erf((x-35.64488)/12.89832)+0.92920;
      else if (idx == 98  ) num = 0.15766*TMath::Erf((x-17.36590)/18.52519)+0.83654;
      else if (idx == 99  ) num = 0.05616*TMath::Erf((x-32.57253)/14.23573)+0.90977;
      else if (idx == 100 ) num = 0.15347*TMath::Erf((x-21.20729)/10.86360)+0.84327;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1 || idx == -2) num = 0.10505*TMath::Erf((x-21.94033)/14.87376)+0.88088;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;

   if (idx==-2) return 2*tnp_weight_iso_ppb(pt,eta,0) - (num / den);

   return num / den;
}

///////////////////////////////////////////////////
//               M U I D      P P b              //
///////////////////////////////////////////////////
double tnp_weight_muid_ppb(double pt, double eta, int idx) {
   double x = pt;

   if (idx == -10) {
      if (fabs(eta)<1.2) {
         // 0 < |eta| < 1.2
         if (pt<15) return 0.981114;
         else if (pt<25) return 1.00118;
         else if (pt<30) return 0.99319;
         else if (pt<35) return 0.994709;
         else if (pt<40) return 0.991174;
         else if (pt<45) return 0.993454;
         else if (pt<50) return 0.987039;
         else if (pt<80) return 0.992419;
         else if (pt<200) return 0.971874;
      } else if (fabs(eta)<2.1) {
         // 1.2 < |eta| < 2.1
         if (pt<15) return 1.0036;
         else if (pt<25) return 0.998054;
         else if (pt<30) return 0.995244;
         else if (pt<35) return 0.992706;
         else if (pt<40) return 0.999179;
         else if (pt<45) return 0.999337;
         else if (pt<50) return 0.995187;
         else if (pt<80) return 1.00077;
         else if (pt<200) return 1.00301;
      } else {
         // 2.1 < |eta| < 2.4
         if (pt<15) return 1.00886;
         else if (pt<25) return 1.00277;
         else if (pt<40) return 0.9927;
         else if (pt<80) return 0.985097;
         else if (pt<200) return 0.974808;
      }
   }

   double num = 1, den = 1;

   // nominal
   if (fabs(eta)<1.2) {
      num = 0.98711-0.00022*x;
      den = 0.98864-0.00002*x;
   } else if (fabs(eta)<2.1) {
      num = 0.99296 + 0.00001*x;
      den = 0.99633-0.00004*x;
   } else {
      num = 1.00072-0.00052*x;
      den = 0.99254-0.00017*x;
   }

   // 100 variations 
   if (fabs(eta)<1.2) {
      if (idx == 1   ) num = 0.98216-0.00012*x;
      else if (idx == 2   ) num = 0.98444-0.00015*x;
      else if (idx == 3   ) num = 0.99079-0.00033*x;
      else if (idx == 4   ) num = 0.97209+0.00010*x;
      else if (idx == 5   ) num = 0.99083-0.00029*x;
      else if (idx == 6   ) num = 0.99184-0.00029*x;
      else if (idx == 7   ) num = 0.98340-0.00016*x;
      else if (idx == 8   ) num = 0.98541-0.00026*x;
      else if (idx == 9   ) num = 0.97642+0.00005*x;
      else if (idx == 10  ) num = 0.99088-0.00026*x;
      else if (idx == 11  ) num = 0.97731-0.00003*x;
      else if (idx == 12  ) num = 0.98927-0.00032*x;
      else if (idx == 13  ) num = 0.99025-0.00031*x;
      else if (idx == 14  ) num = 0.99398-0.00040*x;
      else if (idx == 15  ) num = 0.98014-0.00017*x;
      else if (idx == 16  ) num = 0.99707-0.00046*x;
      else if (idx == 17  ) num = 0.97607-0.00002*x;
      else if (idx == 18  ) num = 0.99917-0.00049*x;
      else if (idx == 19  ) num = 0.98614-0.00018*x;
      else if (idx == 20  ) num = 0.99061-0.00024*x;
      else if (idx == 21  ) num = 0.98719-0.00022*x;
      else if (idx == 22  ) num = 0.98611-0.00021*x;
      else if (idx == 23  ) num = 0.98541-0.00014*x;
      else if (idx == 24  ) num = 0.98480-0.00015*x;
      else if (idx == 25  ) num = 0.97988-0.00009*x;
      else if (idx == 26  ) num = 0.97945-0.00006*x;
      else if (idx == 27  ) num = 0.99505-0.00038*x;
      else if (idx == 28  ) num = 0.99933-0.00049*x;
      else if (idx == 29  ) num = 0.98003-0.00000*x;
      else if (idx == 30  ) num = 0.97219+0.00009*x;
      else if (idx == 31  ) num = 0.98237-0.00010*x;
      else if (idx == 32  ) num = 0.98512-0.00015*x;
      else if (idx == 33  ) num = 0.98142-0.00007*x;
      else if (idx == 34  ) num = 0.99604-0.00039*x;
      else if (idx == 35  ) num = 0.98885-0.00031*x;
      else if (idx == 36  ) num = 0.99388-0.00034*x;
      else if (idx == 37  ) num = 0.99168-0.00033*x;
      else if (idx == 38  ) num = 0.97524+0.00004*x;
      else if (idx == 39  ) num = 0.98027-0.00011*x;
      else if (idx == 40  ) num = 0.99009-0.00022*x;
      else if (idx == 41  ) num = 0.98929-0.00026*x;
      else if (idx == 42  ) num = 0.98244-0.00015*x;
      else if (idx == 43  ) num = 0.98258-0.00011*x;
      else if (idx == 44  ) num = 0.98014-0.00002*x;
      else if (idx == 45  ) num = 0.98656-0.00026*x;
      else if (idx == 46  ) num = 0.98013+0.00004*x;
      else if (idx == 47  ) num = 0.98895-0.00027*x;
      else if (idx == 48  ) num = 0.98689-0.00028*x;
      else if (idx == 49  ) num = 0.98938-0.00026*x;
      else if (idx == 50  ) num = 0.97786-0.00007*x;
      else if (idx == 51  ) num = 0.97637+0.00003*x;
      else if (idx == 52  ) num = 0.99635-0.00050*x;
      else if (idx == 53  ) num = 0.98092-0.00011*x;
      else if (idx == 54  ) num = 0.98849-0.00032*x;
      else if (idx == 55  ) num = 0.98958-0.00022*x;
      else if (idx == 56  ) num = 0.99087-0.00023*x;
      else if (idx == 57  ) num = 0.99112-0.00035*x;
      else if (idx == 58  ) num = 0.99542-0.00035*x;
      else if (idx == 59  ) num = 0.99380-0.00035*x;
      else if (idx == 60  ) num = 0.98629-0.00019*x;
      else if (idx == 61  ) num = 0.97954-0.00005*x;
      else if (idx == 62  ) num = 0.99353-0.00037*x;
      else if (idx == 63  ) num = 0.99171-0.00033*x;
      else if (idx == 64  ) num = 0.99339-0.00038*x;
      else if (idx == 65  ) num = 0.98256-0.00003*x;
      else if (idx == 66  ) num = 0.98016-0.00020*x;
      else if (idx == 67  ) num = 0.99609-0.00037*x;
      else if (idx == 68  ) num = 0.99209-0.00030*x;
      else if (idx == 69  ) num = 1.00039-0.00055*x;
      else if (idx == 70  ) num = 0.98237-0.00018*x;
      else if (idx == 71  ) num = 0.99425-0.00040*x;
      else if (idx == 72  ) num = 0.98840-0.00017*x;
      else if (idx == 73  ) num = 0.99310-0.00048*x;
      else if (idx == 74  ) num = 0.98728-0.00028*x;
      else if (idx == 75  ) num = 0.99716-0.00041*x;
      else if (idx == 76  ) num = 0.99419-0.00036*x;
      else if (idx == 77  ) num = 0.98171-0.00012*x;
      else if (idx == 78  ) num = 0.98239-0.00006*x;
      else if (idx == 79  ) num = 0.98080-0.00012*x;
      else if (idx == 80  ) num = 0.99226-0.00032*x;
      else if (idx == 81  ) num = 0.98672-0.00018*x;
      else if (idx == 82  ) num = 0.99198-0.00048*x;
      else if (idx == 83  ) num = 0.98282-0.00018*x;
      else if (idx == 84  ) num = 0.98647-0.00020*x;
      else if (idx == 85  ) num = 0.99965-0.00056*x;
      else if (idx == 86  ) num = 1.00148-0.00056*x;
      else if (idx == 87  ) num = 0.97968-0.00012*x;
      else if (idx == 88  ) num = 0.98417-0.00014*x;
      else if (idx == 89  ) num = 0.98236-0.00010*x;
      else if (idx == 90  ) num = 0.98229-0.00015*x;
      else if (idx == 91  ) num = 0.98788-0.00021*x;
      else if (idx == 92  ) num = 0.98183-0.00011*x;
      else if (idx == 93  ) num = 0.98130-0.00005*x;
      else if (idx == 94  ) num = 0.97616-0.00006*x;
      else if (idx == 95  ) num = 0.98670-0.00024*x;
      else if (idx == 96  ) num = 0.98200-0.00012*x;
      else if (idx == 97  ) num = 0.98162-0.00010*x;
      else if (idx == 98  ) num = 0.97650+0.00003*x;
      else if (idx == 99  ) num = 0.99554-0.00045*x;
      else if (idx == 100 ) num = 0.97934-0.00010*x;
      // // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1  ) num = 0.99313-0.00032*x;
      else if (idx == -2  ) num = 0.98101-0.00013*x;
   } else if (fabs(eta)<2.1) {
      if (idx == 1   ) num = 0.99035+0.00003*x;
      else if (idx == 2   ) num = 0.99224+0.00003*x;
      else if (idx == 3   ) num = 0.99557-0.00009*x;
      else if (idx == 4   ) num = 0.99262-0.00003*x;
      else if (idx == 5   ) num = 0.98790+0.00006*x;
      else if (idx == 6   ) num = 0.99470-0.00010*x;
      else if (idx == 7   ) num = 0.99488-0.00004*x;
      else if (idx == 8   ) num = 0.99265+0.00000*x;
      else if (idx == 9   ) num = 0.99252-0.00001*x;
      else if (idx == 10  ) num = 0.99290+0.00003*x;
      else if (idx == 11  ) num = 0.99594-0.00013*x;
      else if (idx == 12  ) num = 0.99790-0.00019*x;
      else if (idx == 13  ) num = 0.99244+0.00000*x;
      else if (idx == 14  ) num = 0.99034+0.00006*x;
      else if (idx == 15  ) num = 0.99067+0.00003*x;
      else if (idx == 16  ) num = 0.99296-0.00000*x;
      else if (idx == 17  ) num = 0.99447-0.00012*x;
      else if (idx == 18  ) num = 0.99822-0.00015*x;
      else if (idx == 19  ) num = 0.99034+0.00004*x;
      else if (idx == 20  ) num = 0.98905+0.00009*x;
      else if (idx == 21  ) num = 0.99728-0.00013*x;
      else if (idx == 22  ) num = 0.98986+0.00006*x;
      else if (idx == 23  ) num = 0.99346-0.00001*x;
      else if (idx == 24  ) num = 0.99091+0.00005*x;
      else if (idx == 25  ) num = 0.99422-0.00007*x;
      else if (idx == 26  ) num = 0.99192+0.00004*x;
      else if (idx == 27  ) num = 0.99295+0.00000*x;
      else if (idx == 28  ) num = 0.99333+0.00003*x;
      else if (idx == 29  ) num = 0.99803-0.00013*x;
      else if (idx == 30  ) num = 0.99531-0.00004*x;
      else if (idx == 31  ) num = 0.99849-0.00018*x;
      else if (idx == 32  ) num = 1.00133-0.00023*x;
      else if (idx == 33  ) num = 0.99100+0.00004*x;
      else if (idx == 34  ) num = 0.99552-0.00012*x;
      else if (idx == 35  ) num = 0.99358-0.00003*x;
      else if (idx == 36  ) num = 0.99728-0.00016*x;
      else if (idx == 37  ) num = 0.99082+0.00004*x;
      else if (idx == 38  ) num = 0.98951+0.00009*x;
      else if (idx == 39  ) num = 0.99028+0.00005*x;
      else if (idx == 40  ) num = 0.99038+0.00006*x;
      else if (idx == 41  ) num = 0.99066+0.00004*x;
      else if (idx == 42  ) num = 0.99353-0.00007*x;
      else if (idx == 43  ) num = 0.99252-0.00002*x;
      else if (idx == 44  ) num = 0.99347-0.00003*x;
      else if (idx == 45  ) num = 0.99327+0.00001*x;
      else if (idx == 46  ) num = 1.00178-0.00033*x;
      else if (idx == 47  ) num = 0.99974-0.00021*x;
      else if (idx == 48  ) num = 0.99517-0.00011*x;
      else if (idx == 49  ) num = 0.99390-0.00002*x;
      else if (idx == 50  ) num = 0.99083+0.00003*x;
      else if (idx == 51  ) num = 0.99079+0.00004*x;
      else if (idx == 52  ) num = 0.99611-0.00004*x;
      else if (idx == 53  ) num = 0.99067+0.00006*x;
      else if (idx == 54  ) num = 0.99370-0.00011*x;
      else if (idx == 55  ) num = 0.99157+0.00004*x;
      else if (idx == 56  ) num = 0.99434-0.00006*x;
      else if (idx == 57  ) num = 0.99419-0.00006*x;
      else if (idx == 58  ) num = 0.99454-0.00002*x;
      else if (idx == 59  ) num = 0.99427-0.00008*x;
      else if (idx == 60  ) num = 0.99417-0.00002*x;
      else if (idx == 61  ) num = 0.99198+0.00003*x;
      else if (idx == 62  ) num = 0.99311-0.00007*x;
      else if (idx == 63  ) num = 0.99782-0.00020*x;
      else if (idx == 64  ) num = 0.99705-0.00017*x;
      else if (idx == 65  ) num = 0.99078+0.00001*x;
      else if (idx == 66  ) num = 0.99604-0.00009*x;
      else if (idx == 67  ) num = 0.98999+0.00007*x;
      else if (idx == 68  ) num = 0.99561-0.00009*x;
      else if (idx == 69  ) num = 1.00041-0.00026*x;
      else if (idx == 70  ) num = 0.99434-0.00004*x;
      else if (idx == 71  ) num = 0.99150+0.00000*x;
      else if (idx == 72  ) num = 0.99620-0.00015*x;
      else if (idx == 73  ) num = 0.99408-0.00001*x;
      else if (idx == 74  ) num = 0.99139+0.00004*x;
      else if (idx == 75  ) num = 0.99662-0.00013*x;
      else if (idx == 76  ) num = 0.99270+0.00001*x;
      else if (idx == 77  ) num = 0.99150-0.00001*x;
      else if (idx == 78  ) num = 0.99159-0.00001*x;
      else if (idx == 79  ) num = 0.99518-0.00006*x;
      else if (idx == 80  ) num = 0.99239+0.00003*x;
      else if (idx == 81  ) num = 0.99147+0.00004*x;
      else if (idx == 82  ) num = 0.99207+0.00002*x;
      else if (idx == 83  ) num = 0.99147+0.00001*x;
      else if (idx == 84  ) num = 0.99450-0.00002*x;
      else if (idx == 85  ) num = 0.99665-0.00010*x;
      else if (idx == 86  ) num = 0.99218+0.00003*x;
      else if (idx == 87  ) num = 0.99033+0.00005*x;
      else if (idx == 88  ) num = 0.99448-0.00005*x;
      else if (idx == 89  ) num = 0.98874+0.00005*x;
      else if (idx == 90  ) num = 0.99019+0.00005*x;
      else if (idx == 91  ) num = 0.99692-0.00016*x;
      else if (idx == 92  ) num = 0.99135+0.00002*x;
      else if (idx == 93  ) num = 0.99239-0.00001*x;
      else if (idx == 94  ) num = 0.99123+0.00005*x;
      else if (idx == 95  ) num = 0.98927+0.00008*x;
      else if (idx == 96  ) num = 0.99349-0.00001*x;
      else if (idx == 97  ) num = 0.99073+0.00005*x;
      else if (idx == 98  ) num = 1.00333-0.00029*x;
      else if (idx == 99  ) num = 0.99071+0.00006*x;
      else if (idx == 100 ) num = 0.99103+0.00002*x;
      // // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1  ) num = 0.99234+0.00003*x;
      else if (idx == -2  ) num = 0.99346-0.00001*x;
   } else {
      if (idx == 1   ) num = 0.98883-0.00012*x;
      else if (idx == 2   ) num = 0.99973-0.00052*x;
      else if (idx == 3   ) num = 1.00163-0.00073*x;
      else if (idx == 4   ) num = 1.00401-0.00077*x;
      else if (idx == 5   ) num = 1.00779-0.00083*x;
      else if (idx == 6   ) num = 0.98365+0.00006*x;
      else if (idx == 7   ) num = 0.99932-0.00061*x;
      else if (idx == 8   ) num = 1.01102-0.00087*x;
      else if (idx == 9   ) num = 0.98429+0.00004*x;
      else if (idx == 10  ) num = 1.00947-0.00100*x;
      else if (idx == 11  ) num = 1.01235-0.00110*x;
      else if (idx == 12  ) num = 1.00550-0.00077*x;
      else if (idx == 13  ) num = 0.98864-0.00009*x;
      else if (idx == 14  ) num = 1.00527-0.00073*x;
      else if (idx == 15  ) num = 0.99135-0.00022*x;
      else if (idx == 16  ) num = 0.99817-0.00033*x;
      else if (idx == 17  ) num = 1.00889-0.00088*x;
      else if (idx == 18  ) num = 0.98720-0.00007*x;
      else if (idx == 19  ) num = 1.00314-0.00068*x;
      else if (idx == 20  ) num = 0.98765-0.00013*x;
      else if (idx == 21  ) num = 1.01230-0.00097*x;
      else if (idx == 22  ) num = 1.01536-0.00112*x;
      else if (idx == 23  ) num = 0.98338+0.00002*x;
      else if (idx == 24  ) num = 0.99702-0.00047*x;
      else if (idx == 25  ) num = 1.00064-0.00067*x;
      else if (idx == 26  ) num = 0.99694-0.00036*x;
      else if (idx == 27  ) num = 1.01886-0.00139*x;
      else if (idx == 28  ) num = 1.01914-0.00120*x;
      else if (idx == 29  ) num = 0.99698-0.00036*x;
      else if (idx == 30  ) num = 0.99037-0.00022*x;
      else if (idx == 31  ) num = 0.99771-0.00039*x;
      else if (idx == 32  ) num = 1.00317-0.00080*x;
      else if (idx == 33  ) num = 0.98440+0.00000*x;
      else if (idx == 34  ) num = 0.98109+0.00010*x;
      else if (idx == 35  ) num = 1.00229-0.00073*x;
      else if (idx == 36  ) num = 1.00581-0.00077*x;
      else if (idx == 37  ) num = 0.99311-0.00028*x;
      else if (idx == 38  ) num = 0.99696-0.00050*x;
      else if (idx == 39  ) num = 1.00555-0.00097*x;
      else if (idx == 40  ) num = 0.99395-0.00044*x;
      else if (idx == 41  ) num = 0.98796-0.00028*x;
      else if (idx == 42  ) num = 0.99429-0.00050*x;
      else if (idx == 43  ) num = 1.01477-0.00122*x;
      else if (idx == 44  ) num = 0.99855-0.00045*x;
      else if (idx == 45  ) num = 0.99397-0.00029*x;
      else if (idx == 46  ) num = 1.01511-0.00114*x;
      else if (idx == 47  ) num = 0.99798-0.00052*x;
      else if (idx == 48  ) num = 1.01710-0.00126*x;
      else if (idx == 49  ) num = 1.02298-0.00145*x;
      else if (idx == 50  ) num = 0.99830-0.00046*x;
      else if (idx == 51  ) num = 0.99691-0.00042*x;
      else if (idx == 52  ) num = 0.99923-0.00046*x;
      else if (idx == 53  ) num = 0.99528-0.00035*x;
      else if (idx == 54  ) num = 1.00914-0.00083*x;
      else if (idx == 55  ) num = 0.99466-0.00044*x;
      else if (idx == 56  ) num = 0.98395+0.00001*x;
      else if (idx == 57  ) num = 0.99536-0.00043*x;
      else if (idx == 58  ) num = 0.99864-0.00046*x;
      else if (idx == 59  ) num = 1.01598-0.00112*x;
      else if (idx == 60  ) num = 1.01881-0.00140*x;
      else if (idx == 61  ) num = 0.99198-0.00019*x;
      else if (idx == 62  ) num = 0.98994-0.00025*x;
      else if (idx == 63  ) num = 0.99481-0.00039*x;
      else if (idx == 64  ) num = 0.99666-0.00038*x;
      else if (idx == 65  ) num = 0.99272-0.00021*x;
      else if (idx == 66  ) num = 0.99375-0.00047*x;
      else if (idx == 67  ) num = 0.99298-0.00042*x;
      else if (idx == 68  ) num = 1.01118-0.00117*x;
      else if (idx == 69  ) num = 1.00950-0.00099*x;
      else if (idx == 70  ) num = 0.98714-0.00025*x;
      else if (idx == 71  ) num = 0.99450-0.00048*x;
      else if (idx == 72  ) num = 1.01686-0.00117*x;
      else if (idx == 73  ) num = 0.98565-0.00004*x;
      else if (idx == 74  ) num = 1.00325-0.00075*x;
      else if (idx == 75  ) num = 0.98219-0.00003*x;
      else if (idx == 76  ) num = 0.99635-0.00046*x;
      else if (idx == 77  ) num = 0.99561-0.00032*x;
      else if (idx == 78  ) num = 0.99953-0.00042*x;
      else if (idx == 79  ) num = 1.02005-0.00124*x;
      else if (idx == 80  ) num = 1.00194-0.00067*x;
      else if (idx == 81  ) num = 1.01322-0.00107*x;
      else if (idx == 82  ) num = 1.00293-0.00065*x;
      else if (idx == 83  ) num = 0.99452-0.00029*x;
      else if (idx == 84  ) num = 1.01578-0.00125*x;
      else if (idx == 85  ) num = 1.00544-0.00065*x;
      else if (idx == 86  ) num = 0.99637-0.00039*x;
      else if (idx == 87  ) num = 0.98428+0.00001*x;
      else if (idx == 88  ) num = 0.99187-0.00031*x;
      else if (idx == 89  ) num = 0.99737-0.00042*x;
      else if (idx == 90  ) num = 1.00048-0.00060*x;
      else if (idx == 91  ) num = 1.02340-0.00147*x;
      else if (idx == 92  ) num = 0.99232-0.00027*x;
      else if (idx == 93  ) num = 1.00800-0.00094*x;
      else if (idx == 94  ) num = 0.98921-0.00029*x;
      else if (idx == 95  ) num = 1.00956-0.00084*x;
      else if (idx == 96  ) num = 1.00888-0.00091*x;
      else if (idx == 97  ) num = 0.99272-0.00018*x;
      else if (idx == 98  ) num = 1.01214-0.00111*x;
      else if (idx == 99  ) num = 1.01610-0.00107*x;
      else if (idx == 100 ) num = 0.99436-0.00021*x;
      // // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1  ) num = 1.00129-0.00053*x;
      else if (idx == -2  ) num = 1.00014-0.00052*x;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;

   return num / den;
}

#endif //#ifndef tnp_weight_h
