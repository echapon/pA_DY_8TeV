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
double tnp_weight_trg_ppb(double eta, int idx=0);
double tnp_weight_muid_ppb(double pt, double eta, int idx=0);
double tnp_weight_iso_ppb(double pt, double eta, int idx=0);
double tnp_weight_isotk_ppb(double pt, double eta, int idx=0);


///////////////////////////////////////////////////
//                 T R G    P P b                //
///////////////////////////////////////////////////
double tnp_weight_trg_ppb(double eta, int idx)
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
//                 I S O    P P b                //
///////////////////////////////////////////////////
double tnp_weight_iso_ppb(double pt, double eta, int idx) {
   double x = pt;

   if (idx == -10) {
      if (fabs(eta)<1.2) {
         // 0 < |eta| < 1.2
         if (pt<15) return 0.994131;
         else if (pt<25) return 0.960778;
         else if (pt<30) return 0.975896;
         else if (pt<35) return 0.981953;
         else if (pt<40) return 0.980849;
         else if (pt<45) return 0.994185;
         else if (pt<50) return 0.9948;
         else if (pt<80) return 0.991582;
         else if (pt<200) return 1.00307;
      } else if (fabs(eta)<2.1) {
         // 1.2 < |eta| < 2.1
         if (pt<15) return 1.01259;
         else if (pt<25) return 0.946523;
         else if (pt<30) return 0.989628;
         else if (pt<35) return 0.980356;
         else if (pt<40) return 0.990009;
         else if (pt<45) return 0.993151;
         else if (pt<50) return 0.994683;
         else if (pt<80) return 0.998054;
         else if (pt<200) return 1.00241;
      } else {
         // 2.1 < |eta| < 2.4
         if (pt<15) return 0.871449;
         else if (pt<25) return 0.966795;
         else if (pt<40) return 0.975846;
         else if (pt<80) return 0.994159;
         else if (pt<200) return 0.971635;
      }
   }

   double num = 1, den = 1;

   // nominal
   if (fabs(eta)<1.2) {
      num = 0.63744*TMath::Erf((x+9.65201)/35.75249)+0.36041;
      den = 0.83469*TMath::Erf((x+9.99864)/29.63764)+0.16014;
   } else if (fabs(eta)<2.1) {
      num = 0.39196*TMath::Erf((x+7.14115)/37.50458)+0.61002;
      den = 0.64689*TMath::Erf((x+9.77218)/28.83199)+0.34807;
   } else {
      num = 0.88862*TMath::Erf((x-0.06980)/16.72262)+0.08323;
      den = 0.86525*TMath::Erf((x+9.99908)/23.41164)+0.12609;
   }

   // 100 variations
   if (fabs(eta)<1.2) {
      if (idx == 1   ) num = 0.49389*TMath::Erf((x-0.39119)/28.61028)+0.50066;
      else if (idx == 2   ) num = 0.48921*TMath::Erf((x+4.03492)/33.43792)+0.50709;
      else if (idx == 3   ) num = 0.49209*TMath::Erf((x+0.54121)/28.31971)+0.50013;
      else if (idx == 4   ) num = 2.28911*TMath::Erf((x+10.88674)/23.81773)-1.30650;
      else if (idx == 5   ) num = 0.48685*TMath::Erf((x+6.04686)/34.89116)+0.51025;
      else if (idx == 6   ) num = 0.48169*TMath::Erf((x+9.83098)/38.74067)+0.51657;
      else if (idx == 7   ) num = 0.49323*TMath::Erf((x-0.26042)/28.56500)+0.50048;
      else if (idx == 8   ) num = 0.49046*TMath::Erf((x+1.36086)/30.65562)+0.50207;
      else if (idx == 9   ) num = 0.49584*TMath::Erf((x-1.89352)/27.30488)+0.49934;
      else if (idx == 10  ) num = 0.46929*TMath::Erf((x+16.62865)/47.80809)+0.53436;
      else if (idx == 11  ) num = 0.49240*TMath::Erf((x-0.72866)/28.31513)+0.49885;
      else if (idx == 12  ) num = 0.48905*TMath::Erf((x+4.72602)/35.85444)+0.51034;
      else if (idx == 13  ) num = 0.48519*TMath::Erf((x+6.76161)/35.97004)+0.51103;
      else if (idx == 14  ) num = 0.47899*TMath::Erf((x+12.61349)/44.88263)+0.52527;
      else if (idx == 15  ) num = 1.70588*TMath::Erf((x+14.09113)/29.66960)-0.71620;
      else if (idx == 16  ) num = 0.48028*TMath::Erf((x+10.71500)/40.77027)+0.51980;
      else if (idx == 17  ) num = 1.52971*TMath::Erf((x+11.56374)/27.83908)-0.54203;
      else if (idx == 18  ) num = 0.06312*TMath::Erf((x-32.27525)/11.66167)+0.92748;
      else if (idx == 19  ) num = 0.48587*TMath::Erf((x+6.67623)/36.20493)+0.51107;
      else if (idx == 20  ) num = 0.47116*TMath::Erf((x+16.36947)/48.53453)+0.53525;
      else if (idx == 21  ) num = 0.48478*TMath::Erf((x+7.11004)/36.70770)+0.51138;
      else if (idx == 22  ) num = 0.49016*TMath::Erf((x+1.57205)/29.71422)+0.50131;
      else if (idx == 23  ) num = 0.49369*TMath::Erf((x+0.48259)/28.81395)+0.50213;
      else if (idx == 24  ) num = 0.48574*TMath::Erf((x+3.44098)/32.17416)+0.50769;
      else if (idx == 25  ) num = 0.49348*TMath::Erf((x+0.72141)/31.42930)+0.50428;
      else if (idx == 26  ) num = 0.49257*TMath::Erf((x+0.05269)/29.59601)+0.50153;
      else if (idx == 27  ) num = 0.47701*TMath::Erf((x+12.90635)/42.59274)+0.52398;
      else if (idx == 28  ) num = 0.47394*TMath::Erf((x+14.33379)/44.16109)+0.52736;
      else if (idx == 29  ) num = 0.49071*TMath::Erf((x+3.10031)/32.90055)+0.50746;
      else if (idx == 30  ) num = 0.49522*TMath::Erf((x-4.68726)/23.01775)+0.49183;
      else if (idx == 31  ) num = 0.49260*TMath::Erf((x+2.36684)/32.91610)+0.50725;
      else if (idx == 32  ) num = 0.48861*TMath::Erf((x+5.11857)/35.09566)+0.50998;
      else if (idx == 33  ) num = 0.48985*TMath::Erf((x+4.81482)/35.84546)+0.51052;
      else if (idx == 34  ) num = 0.47764*TMath::Erf((x+12.20278)/41.06609)+0.52120;
      else if (idx == 35  ) num = 0.49238*TMath::Erf((x-0.43373)/27.56273)+0.49895;
      else if (idx == 36  ) num = 0.48157*TMath::Erf((x+10.98031)/41.20222)+0.52099;
      else if (idx == 37  ) num = 0.48425*TMath::Erf((x+7.88977)/37.32425)+0.51340;
      else if (idx == 38  ) num = 0.94422*TMath::Erf((x+7.11146)/27.76445)+0.04256;
      else if (idx == 39  ) num = 0.49595*TMath::Erf((x-3.27259)/25.21195)+0.49622;
      else if (idx == 40  ) num = 0.47865*TMath::Erf((x+12.17600)/41.56405)+0.52185;
      else if (idx == 41  ) num = 0.49041*TMath::Erf((x+2.03521)/29.73504)+0.50240;
      else if (idx == 42  ) num = 0.48789*TMath::Erf((x+5.46010)/36.42338)+0.51045;
      else if (idx == 43  ) num = 0.49357*TMath::Erf((x-0.27633)/28.40917)+0.50032;
      else if (idx == 44  ) num = 0.48263*TMath::Erf((x+9.97708)/41.38307)+0.51881;
      else if (idx == 45  ) num = 0.64055*TMath::Erf((x+6.10454)/30.26792)+0.34818;
      else if (idx == 46  ) num = 0.47890*TMath::Erf((x+12.64536)/44.13761)+0.52476;
      else if (idx == 47  ) num = 0.48994*TMath::Erf((x+1.68945)/29.02678)+0.50109;
      else if (idx == 48  ) num = 0.48940*TMath::Erf((x+2.57561)/32.00851)+0.50428;
      else if (idx == 49  ) num = 0.48640*TMath::Erf((x+6.10715)/34.75090)+0.50943;
      else if (idx == 50  ) num = 0.49443*TMath::Erf((x-4.20829)/23.40845)+0.49202;
      else if (idx == 51  ) num = 0.49204*TMath::Erf((x-1.34141)/26.29350)+0.49608;
      else if (idx == 52  ) num = 0.48525*TMath::Erf((x+3.62859)/32.18844)+0.50761;
      else if (idx == 53  ) num = 0.49051*TMath::Erf((x+2.17874)/31.56067)+0.50344;
      else if (idx == 54  ) num = 0.48672*TMath::Erf((x+3.95443)/34.67085)+0.51058;
      else if (idx == 55  ) num = 0.48473*TMath::Erf((x+7.82693)/36.95743)+0.51398;
      else if (idx == 56  ) num = 0.06606*TMath::Erf((x-31.78894)/11.56441)+0.92546;
      else if (idx == 57  ) num = 0.48661*TMath::Erf((x+6.84924)/37.43800)+0.51363;
      else if (idx == 58  ) num = 0.43356*TMath::Erf((x+19.86067)/52.90350)+0.57349;
      else if (idx == 59  ) num = 0.47570*TMath::Erf((x+14.10967)/45.53412)+0.52840;
      else if (idx == 60  ) num = 0.49154*TMath::Erf((x+0.67287)/28.21884)+0.49998;
      else if (idx == 61  ) num = 0.49419*TMath::Erf((x-3.04132)/23.53920)+0.49319;
      else if (idx == 62  ) num = 0.47755*TMath::Erf((x+13.26797)/45.13387)+0.52680;
      else if (idx == 63  ) num = 0.48829*TMath::Erf((x+3.88265)/31.94301)+0.50541;
      else if (idx == 64  ) num = 0.48378*TMath::Erf((x+6.93728)/35.17277)+0.51005;
      else if (idx == 65  ) num = 0.48547*TMath::Erf((x+7.26489)/35.97693)+0.51174;
      else if (idx == 66  ) num = 1.60370*TMath::Erf((x+13.06027)/29.32978)-0.61571;
      else if (idx == 67  ) num = 0.05808*TMath::Erf((x-29.94659)/8.07102)+0.92489;
      else if (idx == 68  ) num = 0.46376*TMath::Erf((x+18.17894)/50.69480)+0.54210;
      else if (idx == 69  ) num = 0.48241*TMath::Erf((x+8.75480)/37.07454)+0.51431;
      else if (idx == 70  ) num = 0.49495*TMath::Erf((x-2.38602)/26.73233)+0.49760;
      else if (idx == 71  ) num = 0.45871*TMath::Erf((x+18.87957)/51.00261)+0.54426;
      else if (idx == 72  ) num = 0.47640*TMath::Erf((x+13.74851)/44.46446)+0.52723;
      else if (idx == 73  ) num = 0.49077*TMath::Erf((x+0.20241)/28.51016)+0.49938;
      else if (idx == 74  ) num = 0.49522*TMath::Erf((x-2.24458)/26.34385)+0.49780;
      else if (idx == 75  ) num = 0.06171*TMath::Erf((x-32.39249)/9.21099)+0.92780;
      else if (idx == 76  ) num = 0.48409*TMath::Erf((x+9.62764)/40.43697)+0.51950;
      else if (idx == 77  ) num = 0.49341*TMath::Erf((x-0.39785)/28.71464)+0.50047;
      else if (idx == 78  ) num = 0.49218*TMath::Erf((x+1.97499)/30.96652)+0.50433;
      else if (idx == 79  ) num = 0.49438*TMath::Erf((x-2.89328)/25.01676)+0.49542;
      else if (idx == 80  ) num = 0.47146*TMath::Erf((x+15.43463)/46.10416)+0.52980;
      else if (idx == 81  ) num = 0.48733*TMath::Erf((x+5.91486)/35.21268)+0.50999;
      else if (idx == 82  ) num = 0.49244*TMath::Erf((x-0.23983)/29.27251)+0.50042;
      else if (idx == 83  ) num = 0.49280*TMath::Erf((x+0.00726)/29.74954)+0.50168;
      else if (idx == 84  ) num = 0.48624*TMath::Erf((x+7.94615)/39.77147)+0.51704;
      else if (idx == 85  ) num = 0.48260*TMath::Erf((x+8.32292)/36.52868)+0.51282;
      else if (idx == 86  ) num = 0.06137*TMath::Erf((x-31.42180)/8.98222)+0.92510;
      else if (idx == 87  ) num = 0.49500*TMath::Erf((x-4.60319)/22.85787)+0.49191;
      else if (idx == 88  ) num = 0.49493*TMath::Erf((x-1.96640)/25.61005)+0.49704;
      else if (idx == 89  ) num = 0.49007*TMath::Erf((x+2.84932)/31.77636)+0.50452;
      else if (idx == 90  ) num = 0.49017*TMath::Erf((x+3.48622)/33.86924)+0.50711;
      else if (idx == 91  ) num = 0.48958*TMath::Erf((x+3.51131)/32.34501)+0.50694;
      else if (idx == 92  ) num = 0.48953*TMath::Erf((x+2.88759)/32.56588)+0.50522;
      else if (idx == 93  ) num = 0.48959*TMath::Erf((x+2.91361)/31.91281)+0.50497;
      else if (idx == 94  ) num = 1.70430*TMath::Erf((x+10.43173)/25.61577)-0.72204;
      else if (idx == 95  ) num = 0.49044*TMath::Erf((x+2.51651)/31.07629)+0.50408;
      else if (idx == 96  ) num = 0.49427*TMath::Erf((x-2.24367)/25.71235)+0.49642;
      else if (idx == 97  ) num = 0.49505*TMath::Erf((x-2.60333)/24.81274)+0.49556;
      else if (idx == 98  ) num = 0.49480*TMath::Erf((x-2.33938)/26.15338)+0.49699;
      else if (idx == 99  ) num = 0.48606*TMath::Erf((x+7.31056)/37.65800)+0.51366;
      else if (idx == 100 ) num = 0.55144*TMath::Erf((x-1.80793)/24.83159)+0.43652;
       // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1  ) num = 0.49184*TMath::Erf((x+2.23174) / 30.38505) + 0.50443;
      else if (idx == -2  ) num = 0.48870*TMath::Erf((x+4.83105) / 35.67397) + 0.50928;
   } else if (fabs(eta)<2.1) {
      if (idx == 1   ) num = 0.55152*TMath::Erf((x+3.10429)/27.23581)+0.43739;
      else if (idx == 2   ) num = 0.55052*TMath::Erf((x+16.63164)/39.99587)+0.44885;
      else if (idx == 3   ) num = 0.05170*TMath::Erf((x-30.98145)/9.00798)+0.93688;
      else if (idx == 4   ) num = 0.54589*TMath::Erf((x+14.31725)/39.09330)+0.45041;
      else if (idx == 5   ) num = 0.55157*TMath::Erf((x+10.31638)/36.22904)+0.44468;
      else if (idx == 6   ) num = 0.54897*TMath::Erf((x+10.94397)/34.79133)+0.44210;
      else if (idx == 7   ) num = 0.55291*TMath::Erf((x+9.99481)/34.47310)+0.44446;
      else if (idx == 8   ) num = 0.51630*TMath::Erf((x+19.75729)/45.16635)+0.48466;
      else if (idx == 9   ) num = 0.55058*TMath::Erf((x+8.73195)/31.70839)+0.44021;
      else if (idx == 10  ) num = 0.55171*TMath::Erf((x+16.34990)/40.43053)+0.44958;
      else if (idx == 11  ) num = 0.54916*TMath::Erf((x+12.43683)/36.79855)+0.44408;
      else if (idx == 12  ) num = 0.55066*TMath::Erf((x+4.52820)/29.08758)+0.43911;
      else if (idx == 13  ) num = 0.54944*TMath::Erf((x+16.48810)/40.36929)+0.44778;
      else if (idx == 14  ) num = 0.20082*TMath::Erf((x-7.29568)/33.40675)+0.80233;
      else if (idx == 15  ) num = 0.55152*TMath::Erf((x+13.60923)/39.19188)+0.44714;
      else if (idx == 16  ) num = 0.53431*TMath::Erf((x+18.07346)/42.62429)+0.46547;
      else if (idx == 17  ) num = 0.06372*TMath::Erf((x-32.85473)/14.03271)+0.92892;
      else if (idx == 18  ) num = 0.55424*TMath::Erf((x-0.33743)/21.95463)+0.43101;
      else if (idx == 19  ) num = 0.05337*TMath::Erf((x-29.27274)/7.85391)+0.93232;
      else if (idx == 20  ) num = 0.55525*TMath::Erf((x+10.73528)/37.13590)+0.44791;
      else if (idx == 21  ) num = 0.54371*TMath::Erf((x+15.20397)/38.32921)+0.44947;
      else if (idx == 22  ) num = 0.55237*TMath::Erf((x+13.21527)/38.74249)+0.44721;
      else if (idx == 23  ) num = 0.55273*TMath::Erf((x+11.90367)/36.96853)+0.44656;
      else if (idx == 24  ) num = 0.47804*TMath::Erf((x+19.97414)/47.88358)+0.52682;
      else if (idx == 25  ) num = 0.17071*TMath::Erf((x-7.36692)/34.75072)+0.82560;
      else if (idx == 26  ) num = 0.06312*TMath::Erf((x-28.92553)/10.24130)+0.92831;
      else if (idx == 27  ) num = 0.07612*TMath::Erf((x-26.44101)/11.86451)+0.91400;
      else if (idx == 28  ) num = 0.55603*TMath::Erf((x+2.19788)/25.63180)+0.43836;
      else if (idx == 29  ) num = 0.46847*TMath::Erf((x+19.94756)/44.27708)+0.52768;
      else if (idx == 30  ) num = 0.54095*TMath::Erf((x+17.85334)/41.12350)+0.45813;
      else if (idx == 31  ) num = 0.55108*TMath::Erf((x+5.18029)/27.82575)+0.43605;
      else if (idx == 32  ) num = 0.54345*TMath::Erf((x+17.78535)/38.01686)+0.44461;
      else if (idx == 33  ) num = 0.07505*TMath::Erf((x-27.14997)/12.63624)+0.91670;
      else if (idx == 34  ) num = 0.54803*TMath::Erf((x+14.83459)/38.49688)+0.44561;
      else if (idx == 35  ) num = 0.51299*TMath::Erf((x+19.83392)/43.69419)+0.48449;
      else if (idx == 36  ) num = 0.55031*TMath::Erf((x+3.04372)/26.08456)+0.43539;
      else if (idx == 37  ) num = 0.55372*TMath::Erf((x+2.78485)/27.33327)+0.43903;
      else if (idx == 38  ) num = 0.55497*TMath::Erf((x+7.23955)/33.03934)+0.44464;
      else if (idx == 39  ) num = 0.04532*TMath::Erf((x-34.60991)/8.56491)+0.94824;
      else if (idx == 40  ) num = 0.55422*TMath::Erf((x+6.17833)/31.17223)+0.44259;
      else if (idx == 41  ) num = 0.55062*TMath::Erf((x+16.10334)/40.73071)+0.44840;
      else if (idx == 42  ) num = 0.55017*TMath::Erf((x+6.13932)/30.22579)+0.43908;
      else if (idx == 43  ) num = 0.46229*TMath::Erf((x+19.85943)/46.14504)+0.53594;
      else if (idx == 44  ) num = 0.55062*TMath::Erf((x+9.73921)/32.79921)+0.44111;
      else if (idx == 45  ) num = 0.05872*TMath::Erf((x-27.77680)/9.44743)+0.92961;
      else if (idx == 46  ) num = 0.54467*TMath::Erf((x+10.98217)/33.12389)+0.43765;
      else if (idx == 47  ) num = 0.38664*TMath::Erf((x+19.48087)/45.26663)+0.60457;
      else if (idx == 48  ) num = 0.49589*TMath::Erf((x+19.99850)/44.85990)+0.49838;
      else if (idx == 49  ) num = 0.55185*TMath::Erf((x+11.38019)/35.04820)+0.44409;
      else if (idx == 50  ) num = 0.55254*TMath::Erf((x+4.98204)/29.21460)+0.44041;
      else if (idx == 51  ) num = 0.55195*TMath::Erf((x+14.30305)/39.44950)+0.44814;
      else if (idx == 52  ) num = 0.48282*TMath::Erf((x+19.99999)/43.36616)+0.51553;
      else if (idx == 53  ) num = 0.55335*TMath::Erf((x+8.39022)/32.82325)+0.44267;
      else if (idx == 54  ) num = 0.50028*TMath::Erf((x+19.99954)/46.23007)+0.49419;
      else if (idx == 55  ) num = 0.55234*TMath::Erf((x+13.75378)/38.33444)+0.44701;
      else if (idx == 56  ) num = 0.55114*TMath::Erf((x+6.09976)/30.07804)+0.43974;
      else if (idx == 57  ) num = 0.39594*TMath::Erf((x+19.40482)/47.70537)+0.60253;
      else if (idx == 58  ) num = 0.55095*TMath::Erf((x+14.18260)/37.62970)+0.44623;
      else if (idx == 59  ) num = 0.55020*TMath::Erf((x+8.95221)/33.22266)+0.44149;
      else if (idx == 60  ) num = 0.44545*TMath::Erf((x+19.89845)/47.15384)+0.55586;
      else if (idx == 61  ) num = 0.55261*TMath::Erf((x+10.97535)/35.35329)+0.44451;
      else if (idx == 62  ) num = 0.54860*TMath::Erf((x+14.58664)/39.14536)+0.44549;
      else if (idx == 63  ) num = 0.54811*TMath::Erf((x+8.48501)/32.65195)+0.44009;
      else if (idx == 64  ) num = 0.54912*TMath::Erf((x+10.53250)/35.08853)+0.44258;
      else if (idx == 65  ) num = 0.54706*TMath::Erf((x+18.92434)/44.19503)+0.45134;
      else if (idx == 66  ) num = 0.52054*TMath::Erf((x+19.42537)/41.33898)+0.47255;
      else if (idx == 67  ) num = 0.55434*TMath::Erf((x+9.85391)/35.86375)+0.44642;
      else if (idx == 68  ) num = 0.06705*TMath::Erf((x-27.93865)/11.21491)+0.92082;
      else if (idx == 69  ) num = 0.54626*TMath::Erf((x+12.09210)/34.40099)+0.44017;
      else if (idx == 70  ) num = 0.54902*TMath::Erf((x+16.90579)/39.59620)+0.44795;
      else if (idx == 71  ) num = 0.55124*TMath::Erf((x+5.50110)/29.11396)+0.43894;
      else if (idx == 72  ) num = 0.54953*TMath::Erf((x+6.34597)/30.03507)+0.43848;
      else if (idx == 73  ) num = 0.38764*TMath::Erf((x+19.35293)/47.99009)+0.61370;
      else if (idx == 74  ) num = 0.06252*TMath::Erf((x-29.01138)/11.39320)+0.92853;
      else if (idx == 75  ) num = 0.54838*TMath::Erf((x+13.58281)/37.19181)+0.44387;
      else if (idx == 76  ) num = 0.47635*TMath::Erf((x+19.99897)/45.10186)+0.52231;
      else if (idx == 77  ) num = 0.55545*TMath::Erf((x+3.95072)/30.09589)+0.43923;
      else if (idx == 78  ) num = 0.55282*TMath::Erf((x+7.86075)/34.01493)+0.44427;
      else if (idx == 79  ) num = 0.55065*TMath::Erf((x+13.43688)/37.27081)+0.44572;
      else if (idx == 80  ) num = 0.55428*TMath::Erf((x+2.51576)/25.34337)+0.43683;
      else if (idx == 81  ) num = 0.05401*TMath::Erf((x-30.06576)/8.46034)+0.93611;
      else if (idx == 82  ) num = 0.55322*TMath::Erf((x+7.62814)/32.33846)+0.44280;
      else if (idx == 83  ) num = 0.05676*TMath::Erf((x-31.46306)/10.13062)+0.93355;
      else if (idx == 84  ) num = 0.45263*TMath::Erf((x+19.93523)/46.18265)+0.54793;
      else if (idx == 85  ) num = 0.34342*TMath::Erf((x+12.09173)/43.01880)+0.65582;
      else if (idx == 86  ) num = 0.05214*TMath::Erf((x-31.11655)/10.22653)+0.93940;
      else if (idx == 87  ) num = 0.07158*TMath::Erf((x-28.23539)/12.37996)+0.91826;
      else if (idx == 88  ) num = 0.55302*TMath::Erf((x+2.83959)/26.91340)+0.43751;
      else if (idx == 89  ) num = 0.55659*TMath::Erf((x-1.55175)/23.57163)+0.43282;
      else if (idx == 90  ) num = 0.47596*TMath::Erf((x+19.97678)/48.03576)+0.52652;
      else if (idx == 91  ) num = 0.54798*TMath::Erf((x+8.41785)/31.46269)+0.43898;
      else if (idx == 92  ) num = 0.54887*TMath::Erf((x+16.68743)/40.09397)+0.44749;
      else if (idx == 93  ) num = 0.49388*TMath::Erf((x+20.00000)/45.84781)+0.50470;
      else if (idx == 94  ) num = 0.55251*TMath::Erf((x+12.94214)/37.77674)+0.44697;
      else if (idx == 95  ) num = 0.55472*TMath::Erf((x+9.95921)/36.68983)+0.44768;
      else if (idx == 96  ) num = 0.55000*TMath::Erf((x+13.91292)/36.71308)+0.44428;
      else if (idx == 97  ) num = 0.49230*TMath::Erf((x+19.99432)/47.34587)+0.51233;
      else if (idx == 98  ) num = 0.54736*TMath::Erf((x+11.22862)/31.96638)+0.43848;
      else if (idx == 99  ) num = 0.05387*TMath::Erf((x-32.42992)/9.87505)+0.93994;
      else if (idx == 100 ) num = 0.55248*TMath::Erf((x+9.12957)/34.81715)+0.44471;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1  ) num = 0.55135*TMath::Erf((x+14.74471) / 39.89229) + 0.45190;
      else if (idx == -2  ) num = 0.55095*TMath::Erf((x+16.84950) / 42.32004) + 0.45059;
   } else {
      if (idx == 1   ) num = 0.55097*TMath::Erf((x-8.40759)/10.88146)+0.42528;
      else if (idx == 2   ) num = 0.52988*TMath::Erf((x-1.57857)/17.77923)+0.44143;
      else if (idx == 3   ) num = 0.52682*TMath::Erf((x-3.00855)/17.67403)+0.43207;
      else if (idx == 4   ) num = 0.54304*TMath::Erf((x-9.03014)/9.95040)+0.41627;
      else if (idx == 5   ) num = 0.54107*TMath::Erf((x-7.57650)/11.18577)+0.42101;
      else if (idx == 6   ) num = 0.54485*TMath::Erf((x-5.63141)/15.03019)+0.43543;
      else if (idx == 7   ) num = 0.53622*TMath::Erf((x-4.95828)/15.33858)+0.43114;
      else if (idx == 8   ) num = 0.52870*TMath::Erf((x-2.30478)/12.53719)+0.43294;
      else if (idx == 9   ) num = 0.55154*TMath::Erf((x-5.48768)/15.73942)+0.43458;
      else if (idx == 10  ) num = 0.52191*TMath::Erf((x-2.04946)/16.91038)+0.43202;
      else if (idx == 11  ) num = 0.53708*TMath::Erf((x-8.48960)/10.45069)+0.41390;
      else if (idx == 12  ) num = 0.55176*TMath::Erf((x-10.57027)/6.98251)+0.40995;
      else if (idx == 13  ) num = 0.50951*TMath::Erf((x+7.84431)/29.40934)+0.47900;
      else if (idx == 14  ) num = 0.52597*TMath::Erf((x-1.19990)/19.27010)+0.44095;
      else if (idx == 15  ) num = 0.52337*TMath::Erf((x+3.28817)/25.50477)+0.46203;
      else if (idx == 16  ) num = 0.53362*TMath::Erf((x-1.22919)/17.97062)+0.44611;
      else if (idx == 17  ) num = 0.49763*TMath::Erf((x+7.98701)/25.84473)+0.46272;
      else if (idx == 18  ) num = 0.51478*TMath::Erf((x+6.51044)/31.45666)+0.47746;
      else if (idx == 19  ) num = 0.52607*TMath::Erf((x-1.37565)/16.96868)+0.43871;
      else if (idx == 20  ) num = 0.51150*TMath::Erf((x+7.24979)/32.18308)+0.47933;
      else if (idx == 21  ) num = 0.51064*TMath::Erf((x+3.42796)/21.63027)+0.45011;
      else if (idx == 22  ) num = 0.51461*TMath::Erf((x+0.57277)/18.93406)+0.43780;
      else if (idx == 23  ) num = 0.54789*TMath::Erf((x-7.19732)/13.51727)+0.42953;
      else if (idx == 24  ) num = 0.52811*TMath::Erf((x-0.59537)/20.47129)+0.44592;
      else if (idx == 25  ) num = 0.52333*TMath::Erf((x-0.42994)/21.50751)+0.44339;
      else if (idx == 26  ) num = 0.52713*TMath::Erf((x+0.24924)/17.70130)+0.44679;
      else if (idx == 27  ) num = 0.53658*TMath::Erf((x-10.33539)/8.38339)+0.40354;
      else if (idx == 28  ) num = 0.52918*TMath::Erf((x-4.59360)/13.70268)+0.42561;
      else if (idx == 29  ) num = 0.55201*TMath::Erf((x-8.41272)/10.91174)+0.42632;
      else if (idx == 30  ) num = 0.54822*TMath::Erf((x-7.82811)/11.24365)+0.42541;
      else if (idx == 31  ) num = 0.51047*TMath::Erf((x+6.70101)/26.80258)+0.47143;
      else if (idx == 32  ) num = 0.53464*TMath::Erf((x-6.47141)/14.59833)+0.42368;
      else if (idx == 33  ) num = 0.52111*TMath::Erf((x+4.44791)/29.26630)+0.47157;
      else if (idx == 34  ) num = 0.55792*TMath::Erf((x-10.61856)/9.78957)+0.42367;
      else if (idx == 35  ) num = 0.53702*TMath::Erf((x-6.64458)/13.28249)+0.42350;
      else if (idx == 36  ) num = 0.54587*TMath::Erf((x-9.18976)/9.47210)+0.41699;
      else if (idx == 37  ) num = 0.55280*TMath::Erf((x-8.89211)/9.76154)+0.42303;
      else if (idx == 38  ) num = 0.53894*TMath::Erf((x-6.09783)/13.10622)+0.42664;
      else if (idx == 39  ) num = 0.54211*TMath::Erf((x-10.74010)/8.57363)+0.40863;
      else if (idx == 40  ) num = 0.55048*TMath::Erf((x-10.49247)/8.91722)+0.41731;
      else if (idx == 41  ) num = 0.06743*TMath::Erf((x-30.82222)/5.04299)+0.91267;
      else if (idx == 42  ) num = 0.54191*TMath::Erf((x-8.27036)/11.84273)+0.42063;
      else if (idx == 43  ) num = 0.52734*TMath::Erf((x-5.84251)/13.18626)+0.41859;
      else if (idx == 44  ) num = 0.53792*TMath::Erf((x-4.70820)/13.74688)+0.43253;
      else if (idx == 45  ) num = 0.52161*TMath::Erf((x+3.17615)/23.75819)+0.46102;
      else if (idx == 46  ) num = 0.49121*TMath::Erf((x+8.76061)/26.43471)+0.46203;
      else if (idx == 47  ) num = 0.54149*TMath::Erf((x-6.43397)/13.35088)+0.42787;
      else if (idx == 48  ) num = 0.53249*TMath::Erf((x-7.26649)/11.21767)+0.41573;
      else if (idx == 49  ) num = 0.51785*TMath::Erf((x-2.99448)/15.22919)+0.42328;
      else if (idx == 50  ) num = 0.51478*TMath::Erf((x+4.72541)/25.35782)+0.46406;
      else if (idx == 51  ) num = 0.53871*TMath::Erf((x-4.82738)/13.94807)+0.43278;
      else if (idx == 52  ) num = 0.54267*TMath::Erf((x-5.54719)/13.86258)+0.43262;
      else if (idx == 53  ) num = 0.55205*TMath::Erf((x-9.06352)/8.76050)+0.41999;
      else if (idx == 54  ) num = 0.53342*TMath::Erf((x-4.14031)/14.03685)+0.43010;
      else if (idx == 55  ) num = 0.55300*TMath::Erf((x-11.07026)/7.69039)+0.41418;
      else if (idx == 56  ) num = 0.55318*TMath::Erf((x-9.23961)/9.43409)+0.42162;
      else if (idx == 57  ) num = 0.54366*TMath::Erf((x-7.53499)/11.70068)+0.42354;
      else if (idx == 58  ) num = 0.54298*TMath::Erf((x-5.05616)/15.51701)+0.43174;
      else if (idx == 59  ) num = 0.53107*TMath::Erf((x-5.58314)/13.20087)+0.42279;
      else if (idx == 60  ) num = 0.53746*TMath::Erf((x-10.57703)/9.12946)+0.40656;
      else if (idx == 61  ) num = 0.53837*TMath::Erf((x-2.48747)/17.84126)+0.44472;
      else if (idx == 62  ) num = 0.55105*TMath::Erf((x-9.45248)/10.50741)+0.42278;
      else if (idx == 63  ) num = 0.54983*TMath::Erf((x-9.64739)/9.83774)+0.42053;
      else if (idx == 64  ) num = 0.52564*TMath::Erf((x+1.84304)/23.01925)+0.45567;
      else if (idx == 65  ) num = 0.53816*TMath::Erf((x-2.22629)/18.97026)+0.44700;
      else if (idx == 66  ) num = 0.54344*TMath::Erf((x-7.56062)/13.29808)+0.42614;
      else if (idx == 67  ) num = 0.54929*TMath::Erf((x-10.07739)/9.90555)+0.41926;
      else if (idx == 68  ) num = 0.53157*TMath::Erf((x-7.69272)/12.05940)+0.41459;
      else if (idx == 69  ) num = 0.49031*TMath::Erf((x+9.12414)/27.56594)+0.46416;
      else if (idx == 70  ) num = 0.54659*TMath::Erf((x-8.40827)/13.43272)+0.42666;
      else if (idx == 71  ) num = 0.55003*TMath::Erf((x-10.24919)/9.27593)+0.41873;
      else if (idx == 72  ) num = 0.52948*TMath::Erf((x-5.70831)/12.14047)+0.42023;
      else if (idx == 73  ) num = 0.51816*TMath::Erf((x+5.30278)/30.89274)+0.47569;
      else if (idx == 74  ) num = 0.54832*TMath::Erf((x-11.05116)/8.07632)+0.41130;
      else if (idx == 75  ) num = 0.55203*TMath::Erf((x-9.46452)/10.74337)+0.42368;
      else if (idx == 76  ) num = 0.54714*TMath::Erf((x-8.53749)/11.89487)+0.42437;
      else if (idx == 77  ) num = 0.38826*TMath::Erf((x+16.01666)/41.22878)+0.59748;
      else if (idx == 78  ) num = 0.49336*TMath::Erf((x+12.58917)/29.86823)+0.48622;
      else if (idx == 79  ) num = 0.55219*TMath::Erf((x-10.77604)/5.01133)+0.39695;
      else if (idx == 80  ) num = 0.54579*TMath::Erf((x-9.42652)/10.82075)+0.42012;
      else if (idx == 81  ) num = 0.54132*TMath::Erf((x-8.98214)/10.06192)+0.41517;
      else if (idx == 82  ) num = 0.49202*TMath::Erf((x+11.13353)/28.73321)+0.47692;
      else if (idx == 83  ) num = 0.54830*TMath::Erf((x-7.62073)/11.45842)+0.42633;
      else if (idx == 84  ) num = 0.50587*TMath::Erf((x-0.46539)/5.04035)+0.41908;
      else if (idx == 85  ) num = 0.51541*TMath::Erf((x+3.87754)/22.43620)+0.45746;
      else if (idx == 86  ) num = 0.53798*TMath::Erf((x-3.96443)/16.21388)+0.43682;
      else if (idx == 87  ) num = 0.51922*TMath::Erf((x+5.51432)/31.68913)+0.47855;
      else if (idx == 88  ) num = 0.51712*TMath::Erf((x+4.11750)/28.13416)+0.46583;
      else if (idx == 89  ) num = 0.52607*TMath::Erf((x+0.71079)/21.11539)+0.45057;
      else if (idx == 90  ) num = 0.50718*TMath::Erf((x+6.54814)/28.37779)+0.46679;
      else if (idx == 91  ) num = 0.51972*TMath::Erf((x-3.48371)/13.45463)+0.42071;
      else if (idx == 92  ) num = 2.67323*TMath::Erf((x+19.99208)/27.11139)-1.69304;
      else if (idx == 93  ) num = 0.53096*TMath::Erf((x-5.22470)/13.09444)+0.42413;
      else if (idx == 94  ) num = 0.52294*TMath::Erf((x+2.55368)/28.22852)+0.46167;
      else if (idx == 95  ) num = 0.53869*TMath::Erf((x-7.53625)/9.50794)+0.41824;
      else if (idx == 96  ) num = 0.52909*TMath::Erf((x-3.80024)/14.94464)+0.42965;
      else if (idx == 97  ) num = 0.54028*TMath::Erf((x-3.11592)/16.53727)+0.44263;
      else if (idx == 98  ) num = 0.52408*TMath::Erf((x-3.75409)/15.30248)+0.42569;
      else if (idx == 99  ) num = 0.53116*TMath::Erf((x-5.59678)/11.25447)+0.42197;
      else if (idx == 100 ) num = 0.54439*TMath::Erf((x-5.75150)/11.94555)+0.43170;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2)
      else if (idx == -1  ) num = 0.55349*TMath::Erf((x - 8.07217) / 8.88300) + 0.42628;
      else if (idx == -2  ) num = 0.52667*TMath::Erf((x - 0.60436) / 25.37216) + 0.44844;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;

   return num / den;
}

///////////////////////////////////////////////////
//               I S O T K    P P b              //
///////////////////////////////////////////////////
double tnp_weight_isotk_ppb(double pt, double eta, int idx) {
   double x = pt;

   if (idx == -10) {
      if (fabs(eta)<1.2) {
         // 0 < |eta| < 1.2
         if (pt<15) return 0.924174;
         else if (pt<25) return 0.988025;
         else if (pt<30) return 1.00108;
         else if (pt<35) return 0.995768;
         else if (pt<40) return 0.996912;
         else if (pt<45) return 0.998978;
         else if (pt<50) return 0.999486;
         else if (pt<80) return 0.998308;
         else if (pt<200) return 1.00106;
      } else if (fabs(eta)<2.1) {
         // 1.2 < |eta| < 2.1
         if (pt<15) return 1.01289;
         else if (pt<25) return 0.973158;
         else if (pt<30) return 0.990945;
         else if (pt<35) return 0.996513;
         else if (pt<40) return 0.996408;
         else if (pt<45) return 1.00001;
         else if (pt<50) return 0.999821;
         else if (pt<80) return 0.999545;
         else if (pt<200) return 1.00031;
      } else {
         // 2.1 < |eta| < 2.4
         if (pt<15) return 0.953001;
         else if (pt<25) return 0.987469;
         else if (pt<40) return 0.996723;
         else if (pt<80) return 1.00092;
         else if (pt<200) return 0.973697;
      }
   }

   double num = 1, den = 1;

   // nominal
   if (fabs(eta)<1.2) {
      num = 0.92206*TMath::Erf((x+5.42720)/19.63064)+0.07442;
      den = 0.90174*TMath::Erf((x+16.71404)/26.89240)+0.09728;
   } else if (fabs(eta)<2.1) {
      num = 0.88426*TMath::Erf((x+19.99534)/26.67518)+0.11042;
      den = 0.89085*TMath::Erf((x+19.70468)/26.65462)+0.10771;
   } else {
      num = 0.90288*TMath::Erf((x+8.74654)/17.91682)+0.08563;
      den = 0.90203*TMath::Erf((x+19.33607)/23.35021)+0.09595;
   }

   // 100 variations
   if (fabs(eta)<1.2) {
      if (idx == 1   ) num = 0.90798*TMath::Erf((x+19.99719)/25.56320)+0.08483;
      else if (idx == 2   ) num = 0.92213*TMath::Erf((x+3.68246)/19.04275)+0.07403;
      else if (idx == 3   ) num = 0.91875*TMath::Erf((x+9.45395)/22.72912)+0.07548;
      else if (idx == 4   ) num = 0.91981*TMath::Erf((x+9.93229)/23.45799)+0.07695;
      else if (idx == 5   ) num = 0.92406*TMath::Erf((x+0.30661)/15.91947)+0.07171;
      else if (idx == 6   ) num = 0.92058*TMath::Erf((x+4.74481)/18.77470)+0.07299;
      else if (idx == 7   ) num = 0.92318*TMath::Erf((x-1.59460)/13.85819)+0.06908;
      else if (idx == 8   ) num = 0.92286*TMath::Erf((x+3.35740)/16.89094)+0.07254;
      else if (idx == 9   ) num = 0.91838*TMath::Erf((x+11.43453)/23.95880)+0.07744;
      else if (idx == 10  ) num = 0.92214*TMath::Erf((x+6.80266)/20.37078)+0.07403;
      else if (idx == 11  ) num = 0.92189*TMath::Erf((x+4.05899)/17.67234)+0.07397;
      else if (idx == 12  ) num = 0.92124*TMath::Erf((x+6.35792)/20.34616)+0.07305;
      else if (idx == 13  ) num = 0.92268*TMath::Erf((x+1.20568)/16.64130)+0.07125;
      else if (idx == 14  ) num = 0.92222*TMath::Erf((x+6.93468)/19.88657)+0.07421;
      else if (idx == 15  ) num = 0.92080*TMath::Erf((x+7.76964)/21.11194)+0.07410;
      else if (idx == 16  ) num = 0.92111*TMath::Erf((x+7.94549)/22.03543)+0.07617;
      else if (idx == 17  ) num = 0.92105*TMath::Erf((x+8.60777)/22.93058)+0.07697;
      else if (idx == 18  ) num = 0.10673*TMath::Erf((x+12.68010)/42.15979)+0.89250;
      else if (idx == 19  ) num = 0.91777*TMath::Erf((x+13.72608)/26.58229)+0.07984;
      else if (idx == 20  ) num = 0.92036*TMath::Erf((x+9.04288)/21.72562)+0.07641;
      else if (idx == 21  ) num = 0.91126*TMath::Erf((x+19.59602)/29.91994)+0.08694;
      else if (idx == 22  ) num = 0.92420*TMath::Erf((x-2.42624)/13.40055)+0.06960;
      else if (idx == 23  ) num = 0.91664*TMath::Erf((x+14.94810)/26.15755)+0.08063;
      else if (idx == 24  ) num = 0.91846*TMath::Erf((x+13.20469)/25.14899)+0.07972;
      else if (idx == 25  ) num = 0.92194*TMath::Erf((x+5.71290)/19.36118)+0.07447;
      else if (idx == 26  ) num = 0.92255*TMath::Erf((x+7.06461)/20.67166)+0.07466;
      else if (idx == 27  ) num = 0.91956*TMath::Erf((x+11.95692)/25.51775)+0.07921;
      else if (idx == 28  ) num = 0.92321*TMath::Erf((x+1.44975)/15.95151)+0.07131;
      else if (idx == 29  ) num = 0.91762*TMath::Erf((x+12.71865)/24.84839)+0.07816;
      else if (idx == 30  ) num = 0.92389*TMath::Erf((x+1.26884)/17.05427)+0.07261;
      else if (idx == 31  ) num = 0.92206*TMath::Erf((x+7.72566)/20.84089)+0.07370;
      else if (idx == 32  ) num = 0.91534*TMath::Erf((x+14.89940)/26.67206)+0.07927;
      else if (idx == 33  ) num = 0.92346*TMath::Erf((x+0.01739)/15.44476)+0.07089;
      else if (idx == 34  ) num = 0.91565*TMath::Erf((x+15.56841)/26.96144)+0.08060;
      else if (idx == 35  ) num = 0.92260*TMath::Erf((x+3.09406)/17.02360)+0.07225;
      else if (idx == 36  ) num = 0.92166*TMath::Erf((x+4.64353)/18.88099)+0.07416;
      else if (idx == 37  ) num = 0.92299*TMath::Erf((x+3.80538)/18.88390)+0.07388;
      else if (idx == 38  ) num = 0.91857*TMath::Erf((x+11.89332)/24.06676)+0.07827;
      else if (idx == 39  ) num = 0.92278*TMath::Erf((x+3.56337)/18.40186)+0.07319;
      else if (idx == 40  ) num = 0.92177*TMath::Erf((x+7.00292)/21.46462)+0.07592;
      else if (idx == 41  ) num = 0.91957*TMath::Erf((x+11.07548)/24.94809)+0.07827;
      else if (idx == 42  ) num = 0.92182*TMath::Erf((x+6.81280)/21.37593)+0.07576;
      else if (idx == 43  ) num = 0.92409*TMath::Erf((x-2.12147)/13.67856)+0.06985;
      else if (idx == 44  ) num = 0.92134*TMath::Erf((x+5.73096)/19.33601)+0.07391;
      else if (idx == 45  ) num = 0.92103*TMath::Erf((x+4.31362)/18.39676)+0.07435;
      else if (idx == 46  ) num = 0.92077*TMath::Erf((x+8.69726)/22.15139)+0.07654;
      else if (idx == 47  ) num = 0.91526*TMath::Erf((x+17.31440)/28.97279)+0.08356;
      else if (idx == 48  ) num = 0.92054*TMath::Erf((x+9.39982)/21.46848)+0.07575;
      else if (idx == 49  ) num = 0.92106*TMath::Erf((x+5.37548)/19.70446)+0.07345;
      else if (idx == 50  ) num = 0.91885*TMath::Erf((x+11.27835)/24.14028)+0.07774;
      else if (idx == 51  ) num = 0.92186*TMath::Erf((x+5.40825)/19.64816)+0.07422;
      else if (idx == 52  ) num = 0.92248*TMath::Erf((x+7.88720)/20.83111)+0.07417;
      else if (idx == 53  ) num = 0.92389*TMath::Erf((x-1.96024)/14.18786)+0.07021;
      else if (idx == 54  ) num = 0.92328*TMath::Erf((x+2.75910)/17.54461)+0.07294;
      else if (idx == 55  ) num = 0.92014*TMath::Erf((x+10.17315)/23.39887)+0.07753;
      else if (idx == 56  ) num = 0.91992*TMath::Erf((x+8.85172)/21.39552)+0.07480;
      else if (idx == 57  ) num = 0.92102*TMath::Erf((x+4.54004)/18.82828)+0.07362;
      else if (idx == 58  ) num = 0.92114*TMath::Erf((x+5.37317)/19.69240)+0.07352;
      else if (idx == 59  ) num = 0.90839*TMath::Erf((x+19.97378)/29.97001)+0.08869;
      else if (idx == 60  ) num = 0.92304*TMath::Erf((x+1.78065)/16.51813)+0.07173;
      else if (idx == 61  ) num = 0.92346*TMath::Erf((x+0.07495)/15.27611)+0.07066;
      else if (idx == 62  ) num = 0.92292*TMath::Erf((x+2.13087)/16.76153)+0.07191;
      else if (idx == 63  ) num = 0.92357*TMath::Erf((x+0.07601)/15.65271)+0.07111;
      else if (idx == 64  ) num = 0.92210*TMath::Erf((x+3.58037)/17.32718)+0.07220;
      else if (idx == 65  ) num = 0.92453*TMath::Erf((x+0.20298)/15.72826)+0.07204;
      else if (idx == 66  ) num = 0.92400*TMath::Erf((x-1.10244)/15.01881)+0.07100;
      else if (idx == 67  ) num = 0.92221*TMath::Erf((x+6.10116)/20.66570)+0.07447;
      else if (idx == 68  ) num = 0.92251*TMath::Erf((x+4.08118)/18.86944)+0.07363;
      else if (idx == 69  ) num = 0.92425*TMath::Erf((x-3.15268)/12.94864)+0.06957;
      else if (idx == 70  ) num = 0.91483*TMath::Erf((x+16.79441)/28.08595)+0.08202;
      else if (idx == 71  ) num = 0.90768*TMath::Erf((x+19.99859)/27.11469)+0.08627;
      else if (idx == 72  ) num = 0.92334*TMath::Erf((x+0.90409)/15.36470)+0.07075;
      else if (idx == 73  ) num = 0.92207*TMath::Erf((x+3.94237)/18.81212)+0.07479;
      else if (idx == 74  ) num = 0.92096*TMath::Erf((x+7.56584)/21.87609)+0.07565;
      else if (idx == 75  ) num = 0.92169*TMath::Erf((x+6.76506)/20.02195)+0.07379;
      else if (idx == 76  ) num = 0.91870*TMath::Erf((x+11.65340)/23.76797)+0.07796;
      else if (idx == 77  ) num = 0.92389*TMath::Erf((x-0.33498)/14.63952)+0.07060;
      else if (idx == 78  ) num = 0.92215*TMath::Erf((x+3.42311)/17.93181)+0.07238;
      else if (idx == 79  ) num = 0.91685*TMath::Erf((x+15.45296)/27.52872)+0.08179;
      else if (idx == 80  ) num = 0.92047*TMath::Erf((x+4.21687)/18.44028)+0.07342;
      else if (idx == 81  ) num = 0.92065*TMath::Erf((x+5.80316)/19.28237)+0.07330;
      else if (idx == 82  ) num = 0.91725*TMath::Erf((x+13.30649)/24.50199)+0.07880;
      else if (idx == 83  ) num = 0.91773*TMath::Erf((x+14.12057)/26.31426)+0.08018;
      else if (idx == 84  ) num = 0.91991*TMath::Erf((x+9.70211)/22.79433)+0.07686;
      else if (idx == 85  ) num = 0.92279*TMath::Erf((x+1.72004)/15.44038)+0.07086;
      else if (idx == 86  ) num = 0.92173*TMath::Erf((x+1.78301)/15.92171)+0.07016;
      else if (idx == 87  ) num = 0.92373*TMath::Erf((x-1.85834)/13.51413)+0.06917;
      else if (idx == 88  ) num = 0.92291*TMath::Erf((x+2.09626)/17.07256)+0.07189;
      else if (idx == 89  ) num = 0.90775*TMath::Erf((x+19.99999)/28.13540)+0.08762;
      else if (idx == 90  ) num = 0.92031*TMath::Erf((x+5.24133)/18.38226)+0.07315;
      else if (idx == 91  ) num = 0.92511*TMath::Erf((x-3.83307)/12.09786)+0.06951;
      else if (idx == 92  ) num = 0.92227*TMath::Erf((x+5.46189)/19.59936)+0.07463;
      else if (idx == 93  ) num = 0.92349*TMath::Erf((x-0.08332)/14.95604)+0.07049;
      else if (idx == 94  ) num = 0.92180*TMath::Erf((x+5.09257)/19.91862)+0.07400;
      else if (idx == 95  ) num = 0.92316*TMath::Erf((x+0.67110)/15.96677)+0.07104;
      else if (idx == 96  ) num = 0.92261*TMath::Erf((x+1.41038)/16.13574)+0.07090;
      else if (idx == 97  ) num = 0.92168*TMath::Erf((x+7.62564)/22.82324)+0.07660;
      else if (idx == 98  ) num = 0.92179*TMath::Erf((x+0.90153)/15.44485)+0.06953;
      else if (idx == 99  ) num = 0.92074*TMath::Erf((x+8.97661)/22.13654)+0.07687;
      else if (idx == 100 ) num = 0.92316*TMath::Erf((x+1.58203)/16.74506)+0.07186;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2) FIXME missing for now
      else if (idx == -1   ) num = 0.90798*TMath::Erf((x+19.99719)/25.56320)+0.08483;
      else if (idx == -2   ) num = 0.90798*TMath::Erf((x+19.99719)/25.56320)+0.08483;
   } else if (fabs(eta)<2.1) {
      if (idx == 1   ) num = 0.88427*TMath::Erf((x+19.99875)/28.44931)+0.11179;
      else if (idx == 2   ) num = 0.88203*TMath::Erf((x+19.99941)/24.42619)+0.10870;
      else if (idx == 3   ) num = 0.88128*TMath::Erf((x+19.99951)/23.13338)+0.10815;
      else if (idx == 4   ) num = 0.11216*TMath::Erf((x+4.14954)/40.03063)+0.89115;
      else if (idx == 5   ) num = 0.88365*TMath::Erf((x+19.99975)/25.92993)+0.11001;
      else if (idx == 6   ) num = 0.88426*TMath::Erf((x+20.00000)/26.24884)+0.11055;
      else if (idx == 7   ) num = 0.88194*TMath::Erf((x+19.99959)/24.05689)+0.10866;
      else if (idx == 8   ) num = 0.88015*TMath::Erf((x+19.99792)/22.63917)+0.10711;
      else if (idx == 9   ) num = 0.88166*TMath::Erf((x+19.99559)/24.92127)+0.10823;
      else if (idx == 10  ) num = 0.88130*TMath::Erf((x+19.99960)/22.89379)+0.10817;
      else if (idx == 11  ) num = 0.88160*TMath::Erf((x+19.99886)/24.21099)+0.10828;
      else if (idx == 12  ) num = 0.88362*TMath::Erf((x+19.99902)/26.94082)+0.10991;
      else if (idx == 13  ) num = 0.88501*TMath::Erf((x+19.99312)/28.31772)+0.11203;
      else if (idx == 14  ) num = 0.88332*TMath::Erf((x+19.99995)/26.59321)+0.10958;
      else if (idx == 15  ) num = 0.88252*TMath::Erf((x+19.99849)/25.23039)+0.10899;
      else if (idx == 16  ) num = 0.88454*TMath::Erf((x+19.99954)/27.03418)+0.11080;
      else if (idx == 17  ) num = 0.88389*TMath::Erf((x+19.94500)/27.73279)+0.11068;
      else if (idx == 18  ) num = 0.88491*TMath::Erf((x+19.09237)/27.27789)+0.11127;
      else if (idx == 19  ) num = 0.88402*TMath::Erf((x+19.99995)/26.75189)+0.11019;
      else if (idx == 20  ) num = 0.88394*TMath::Erf((x+19.99993)/25.96697)+0.11030;
      else if (idx == 21  ) num = 0.88007*TMath::Erf((x+19.99953)/21.06343)+0.10727;
      else if (idx == 22  ) num = 0.84328*TMath::Erf((x+19.97715)/29.10351)+0.15377;
      else if (idx == 23  ) num = 0.88389*TMath::Erf((x+19.99100)/27.37207)+0.11037;
      else if (idx == 24  ) num = 0.88413*TMath::Erf((x+19.99760)/27.18744)+0.11050;
      else if (idx == 25  ) num = 0.88409*TMath::Erf((x+19.99958)/26.87720)+0.11034;
      else if (idx == 26  ) num = 0.88322*TMath::Erf((x+19.99606)/27.31533)+0.10995;
      else if (idx == 27  ) num = 0.88407*TMath::Erf((x+19.99505)/27.86979)+0.11109;
      else if (idx == 28  ) num = 0.88222*TMath::Erf((x+19.99980)/23.93863)+0.10893;
      else if (idx == 29  ) num = 0.88393*TMath::Erf((x+19.98718)/29.88093)+0.11239;
      else if (idx == 30  ) num = 0.25248*TMath::Erf((x+15.48607)/39.68962)+0.74991;
      else if (idx == 31  ) num = 0.88454*TMath::Erf((x+18.10194)/28.12818)+0.11320;
      else if (idx == 32  ) num = 0.50501*TMath::Erf((x+15.19094)/30.45076)+0.49063;
      else if (idx == 33  ) num = 0.88283*TMath::Erf((x+19.99985)/26.77374)+0.10924;
      else if (idx == 34  ) num = 0.88227*TMath::Erf((x+19.99513)/26.91901)+0.10890;
      else if (idx == 35  ) num = 0.88290*TMath::Erf((x+19.99940)/26.60777)+0.10921;
      else if (idx == 36  ) num = 0.88344*TMath::Erf((x+20.00000)/27.02102)+0.10980;
      else if (idx == 37  ) num = 0.88087*TMath::Erf((x+19.99808)/24.70542)+0.10746;
      else if (idx == 38  ) num = 0.88387*TMath::Erf((x+19.99860)/27.08998)+0.11023;
      else if (idx == 39  ) num = 0.88140*TMath::Erf((x+19.99654)/24.36251)+0.10808;
      else if (idx == 40  ) num = 0.88084*TMath::Erf((x+19.99931)/22.74185)+0.10778;
      else if (idx == 41  ) num = 0.88309*TMath::Erf((x+19.99999)/28.25825)+0.11071;
      else if (idx == 42  ) num = 0.88251*TMath::Erf((x+20.00000)/26.77586)+0.10896;
      else if (idx == 43  ) num = 0.88378*TMath::Erf((x+19.96824)/29.38614)+0.11228;
      else if (idx == 44  ) num = 0.88430*TMath::Erf((x+19.99734)/27.35726)+0.11080;
      else if (idx == 45  ) num = 0.88399*TMath::Erf((x+19.98897)/27.23770)+0.11037;
      else if (idx == 46  ) num = 0.88251*TMath::Erf((x+19.99983)/24.32631)+0.10914;
      else if (idx == 47  ) num = 0.88490*TMath::Erf((x+17.49497)/26.76865)+0.11101;
      else if (idx == 48  ) num = 0.88157*TMath::Erf((x+19.99953)/23.57705)+0.10837;
      else if (idx == 49  ) num = 0.88396*TMath::Erf((x+19.99960)/27.09919)+0.11030;
      else if (idx == 50  ) num = 0.88242*TMath::Erf((x+19.99715)/25.28420)+0.10891;
      else if (idx == 51  ) num = 0.87687*TMath::Erf((x+19.97897)/27.67922)+0.11700;
      else if (idx == 52  ) num = 0.24974*TMath::Erf((x+17.43919)/40.68853)+0.75068;
      else if (idx == 53  ) num = 0.88339*TMath::Erf((x+19.99789)/26.09990)+0.10972;
      else if (idx == 54  ) num = 0.88313*TMath::Erf((x+19.99994)/24.80443)+0.10968;
      else if (idx == 55  ) num = 0.55599*TMath::Erf((x+16.11058)/32.67461)+0.44605;
      else if (idx == 56  ) num = 0.88130*TMath::Erf((x+19.99551)/24.69106)+0.10796;
      else if (idx == 57  ) num = 0.88298*TMath::Erf((x+19.99956)/27.28915)+0.10981;
      else if (idx == 58  ) num = 0.04160*TMath::Erf((x-19.96721)/12.17640)+0.95429;
      else if (idx == 59  ) num = 0.88461*TMath::Erf((x+19.99937)/26.87268)+0.11080;
      else if (idx == 60  ) num = 0.88240*TMath::Erf((x+19.99853)/25.83286)+0.10883;
      else if (idx == 61  ) num = 0.88442*TMath::Erf((x+19.99226)/28.11368)+0.11156;
      else if (idx == 62  ) num = 0.88380*TMath::Erf((x+19.99985)/25.92744)+0.11018;
      else if (idx == 63  ) num = 0.88191*TMath::Erf((x+19.99567)/25.04005)+0.10849;
      else if (idx == 64  ) num = 0.88117*TMath::Erf((x+19.99817)/23.96148)+0.10793;
      else if (idx == 65  ) num = 0.88162*TMath::Erf((x+19.20118)/27.61696)+0.11133;
      else if (idx == 66  ) num = 0.88448*TMath::Erf((x+19.99359)/27.31143)+0.11091;
      else if (idx == 67  ) num = 0.88248*TMath::Erf((x+19.99993)/23.35479)+0.10930;
      else if (idx == 68  ) num = 0.88412*TMath::Erf((x+19.98497)/28.08244)+0.11103;
      else if (idx == 69  ) num = 0.88213*TMath::Erf((x+19.99962)/24.23825)+0.10876;
      else if (idx == 70  ) num = 0.88324*TMath::Erf((x+19.99960)/25.60065)+0.10964;
      else if (idx == 71  ) num = 0.88218*TMath::Erf((x+19.99995)/22.25261)+0.10914;
      else if (idx == 72  ) num = 0.88111*TMath::Erf((x+19.99971)/27.70688)+0.10828;
      else if (idx == 73  ) num = 0.88373*TMath::Erf((x+19.99935)/29.09915)+0.11207;
      else if (idx == 74  ) num = 0.88280*TMath::Erf((x+19.99977)/24.97684)+0.10934;
      else if (idx == 75  ) num = 0.88401*TMath::Erf((x+19.99999)/27.07165)+0.11034;
      else if (idx == 76  ) num = 0.88221*TMath::Erf((x+19.99697)/25.13569)+0.10868;
      else if (idx == 77  ) num = 0.88115*TMath::Erf((x+19.99906)/23.50279)+0.10797;
      else if (idx == 78  ) num = 0.88319*TMath::Erf((x+19.99986)/25.31414)+0.10975;
      else if (idx == 79  ) num = 0.88496*TMath::Erf((x+19.98849)/27.48566)+0.11140;
      else if (idx == 80  ) num = 0.88213*TMath::Erf((x+19.99626)/25.20076)+0.10859;
      else if (idx == 81  ) num = 0.88214*TMath::Erf((x+19.99946)/24.48397)+0.10877;
      else if (idx == 82  ) num = 0.88276*TMath::Erf((x+19.99792)/25.52522)+0.10920;
      else if (idx == 83  ) num = 0.88459*TMath::Erf((x+19.99101)/30.08675)+0.11314;
      else if (idx == 84  ) num = 0.88337*TMath::Erf((x+19.99805)/26.04396)+0.10973;
      else if (idx == 85  ) num = 0.88386*TMath::Erf((x+19.98145)/27.51136)+0.11042;
      else if (idx == 86  ) num = 0.88453*TMath::Erf((x+19.97708)/29.46289)+0.11288;
      else if (idx == 87  ) num = 0.88372*TMath::Erf((x+19.99999)/25.43582)+0.11017;
      else if (idx == 88  ) num = 0.88279*TMath::Erf((x+19.99977)/24.96910)+0.10939;
      else if (idx == 89  ) num = 0.87989*TMath::Erf((x+19.99687)/23.63819)+0.10674;
      else if (idx == 90  ) num = 0.88300*TMath::Erf((x+19.94068)/26.62355)+0.10923;
      else if (idx == 91  ) num = 0.88254*TMath::Erf((x+19.99990)/26.42029)+0.10886;
      else if (idx == 92  ) num = 0.88285*TMath::Erf((x+19.99698)/26.54675)+0.10911;
      else if (idx == 93  ) num = 0.88416*TMath::Erf((x+19.99134)/29.79567)+0.11304;
      else if (idx == 94  ) num = 0.88204*TMath::Erf((x+19.99858)/25.62659)+0.10849;
      else if (idx == 95  ) num = 0.88244*TMath::Erf((x+19.99875)/25.07313)+0.10897;
      else if (idx == 96  ) num = 0.88267*TMath::Erf((x+19.98262)/27.89273)+0.10992;
      else if (idx == 97  ) num = 0.88031*TMath::Erf((x+19.99896)/25.13910)+0.10695;
      else if (idx == 98  ) num = 0.88375*TMath::Erf((x+19.99885)/26.29785)+0.11003;
      else if (idx == 99  ) num = 0.87922*TMath::Erf((x+19.99221)/22.81949)+0.10614;
      else if (idx == 100 ) num = 0.88364*TMath::Erf((x+19.99558)/27.43615)+0.11042;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2) FIXME missing for now
      else if (idx == -1   ) num = 0.88427*TMath::Erf((x+19.99875)/28.44931)+0.11179;
      else if (idx == -2   ) num = 0.88427*TMath::Erf((x+19.99875)/28.44931)+0.11179;
   } else {
      if (idx == 1   ) num = 0.89721*TMath::Erf((x+14.41411)/9.38091)+0.08283;
      else if (idx == 2   ) num = 0.89620*TMath::Erf((x+0.92201)/9.33870)+0.07829;
      else if (idx == 3   ) num = 0.85807*TMath::Erf((x+19.98409)/24.45676)+0.12530;
      else if (idx == 4   ) num = 0.89817*TMath::Erf((x+19.23002)/25.63521)+0.08994;
      else if (idx == 5   ) num = 0.67478*TMath::Erf((x+13.77537)/15.35887)+0.31292;
      else if (idx == 6   ) num = 0.89980*TMath::Erf((x+18.98662)/22.28483)+0.09411;
      else if (idx == 7   ) num = 0.89803*TMath::Erf((x+3.55108)/14.12634)+0.07889;
      else if (idx == 8   ) num = 0.90141*TMath::Erf((x-2.59366)/7.77325)+0.07252;
      else if (idx == 9   ) num = 0.88941*TMath::Erf((x+19.99997)/22.70274)+0.10058;
      else if (idx == 10  ) num = 0.90723*TMath::Erf((x+1.84969)/14.95667)+0.08832;
      else if (idx == 11  ) num = 0.64707*TMath::Erf((x+13.52557)/15.62474)+0.33208;
      else if (idx == 12  ) num = 0.90099*TMath::Erf((x+19.99856)/26.62491)+0.09694;
      else if (idx == 13  ) num = 0.89784*TMath::Erf((x+9.17027)/17.60477)+0.08089;
      else if (idx == 14  ) num = 0.89537*TMath::Erf((x+11.09652)/15.44612)+0.07932;
      else if (idx == 15  ) num = 0.88915*TMath::Erf((x+5.59809)/14.54568)+0.07233;
      else if (idx == 16  ) num = 0.90624*TMath::Erf((x-2.87543)/8.32800)+0.08203;
      else if (idx == 17  ) num = 0.90023*TMath::Erf((x-0.93197)/10.80906)+0.07932;
      else if (idx == 18  ) num = 0.90681*TMath::Erf((x+0.70428)/12.33129)+0.08575;
      else if (idx == 19  ) num = 0.89735*TMath::Erf((x+19.99986)/24.42879)+0.09307;
      else if (idx == 20  ) num = 0.89557*TMath::Erf((x+7.10045)/17.24692)+0.07906;
      else if (idx == 21  ) num = 0.89513*TMath::Erf((x+6.57477)/16.05332)+0.07858;
      else if (idx == 22  ) num = 0.89487*TMath::Erf((x-0.45808)/8.75531)+0.07500;
      else if (idx == 23  ) num = 0.86934*TMath::Erf((x+19.99602)/24.36697)+0.11544;
      else if (idx == 24  ) num = 0.89961*TMath::Erf((x+17.34193)/23.03110)+0.09509;
      else if (idx == 25  ) num = 0.89596*TMath::Erf((x-4.97975)/6.03176)+0.07175;
      else if (idx == 26  ) num = 0.89931*TMath::Erf((x-4.47854)/7.00968)+0.07553;
      else if (idx == 27  ) num = 0.91056*TMath::Erf((x-4.13608)/6.43248)+0.07972;
      else if (idx == 28  ) num = 0.89028*TMath::Erf((x+19.86324)/21.79229)+0.08542;
      else if (idx == 29  ) num = 0.90444*TMath::Erf((x-7.64768)/5.15878)+0.08023;
      else if (idx == 30  ) num = 0.89738*TMath::Erf((x-4.47078)/7.26196)+0.07454;
      else if (idx == 31  ) num = 0.90134*TMath::Erf((x-5.01263)/5.55223)+0.07151;
      else if (idx == 32  ) num = 0.89885*TMath::Erf((x+13.14834)/13.88408)+0.08337;
      else if (idx == 33  ) num = 0.89519*TMath::Erf((x+17.41763)/19.83147)+0.08041;
      else if (idx == 34  ) num = 0.90304*TMath::Erf((x+0.13153)/9.78203)+0.08016;
      else if (idx == 35  ) num = 0.89772*TMath::Erf((x+19.80246)/25.70976)+0.09206;
      else if (idx == 36  ) num = 0.90106*TMath::Erf((x+2.72957)/13.68247)+0.08136;
      else if (idx == 37  ) num = 0.90688*TMath::Erf((x-3.88292)/8.47856)+0.08307;
      else if (idx == 38  ) num = 0.90448*TMath::Erf((x+9.02027)/17.68028)+0.08724;
      else if (idx == 39  ) num = 0.85192*TMath::Erf((x+19.95416)/24.77478)+0.13380;
      else if (idx == 40  ) num = 0.89664*TMath::Erf((x-1.92617)/9.11116)+0.07467;
      else if (idx == 41  ) num = 0.89482*TMath::Erf((x+11.85234)/14.15559)+0.07918;
      else if (idx == 42  ) num = 0.90417*TMath::Erf((x+12.01179)/20.81099)+0.08884;
      else if (idx == 43  ) num = 0.89897*TMath::Erf((x+9.05246)/17.70563)+0.08195;
      else if (idx == 44  ) num = 0.90131*TMath::Erf((x-1.36915)/10.05712)+0.07949;
      else if (idx == 45  ) num = 0.90382*TMath::Erf((x+2.36178)/13.70272)+0.08386;
      else if (idx == 46  ) num = 0.89669*TMath::Erf((x-6.92146)/5.32160)+0.07327;
      else if (idx == 47  ) num = 0.89402*TMath::Erf((x+19.91226)/21.93355)+0.08981;
      else if (idx == 48  ) num = 0.89694*TMath::Erf((x+3.22039)/11.98018)+0.07880;
      else if (idx == 49  ) num = 0.90247*TMath::Erf((x-2.59226)/8.82004)+0.07963;
      else if (idx == 50  ) num = 0.88953*TMath::Erf((x-6.62323)/5.08121)+0.06626;
      else if (idx == 51  ) num = 0.89359*TMath::Erf((x+0.35854)/9.82239)+0.07250;
      else if (idx == 52  ) num = 0.03561*TMath::Erf((x-16.38667)/20.05761)+0.95706;
      else if (idx == 53  ) num = 0.88836*TMath::Erf((x+19.99845)/22.77133)+0.09809;
      else if (idx == 54  ) num = 0.90181*TMath::Erf((x+11.67523)/20.52737)+0.08577;
      else if (idx == 55  ) num = 0.89653*TMath::Erf((x+18.14638)/25.15053)+0.08605;
      else if (idx == 56  ) num = 0.88039*TMath::Erf((x+19.91359)/23.72520)+0.10011;
      else if (idx == 57  ) num = 0.90714*TMath::Erf((x+10.98394)/21.29123)+0.09104;
      else if (idx == 58  ) num = 0.90365*TMath::Erf((x+8.54238)/18.05594)+0.08631;
      else if (idx == 59  ) num = 0.89561*TMath::Erf((x+9.07644)/17.71582)+0.07869;
      else if (idx == 60  ) num = 0.91178*TMath::Erf((x-5.34852)/8.15706)+0.07400;
      else if (idx == 61  ) num = 0.89905*TMath::Erf((x+14.64487)/21.02591)+0.08464;
      else if (idx == 62  ) num = 0.90382*TMath::Erf((x+8.94224)/17.75819)+0.08658;
      else if (idx == 63  ) num = 0.88285*TMath::Erf((x+19.88913)/23.46274)+0.09662;
      else if (idx == 64  ) num = 0.90259*TMath::Erf((x-0.05259)/11.60391)+0.08180;
      else if (idx == 65  ) num = 0.89750*TMath::Erf((x+19.81428)/21.88548)+0.08967;
      else if (idx == 66  ) num = 0.90299*TMath::Erf((x+13.15980)/21.59244)+0.08958;
      else if (idx == 67  ) num = 0.87559*TMath::Erf((x+19.98278)/24.22842)+0.11431;
      else if (idx == 68  ) num = 0.84704*TMath::Erf((x+14.75711)/13.15459)+0.12990;
      else if (idx == 69  ) num = 0.88866*TMath::Erf((x+19.99997)/22.54961)+0.09490;
      else if (idx == 70  ) num = 0.90558*TMath::Erf((x-2.82382)/10.56499)+0.08573;
      else if (idx == 71  ) num = 0.90430*TMath::Erf((x-0.91637)/10.15691)+0.08208;
      else if (idx == 72  ) num = 0.88129*TMath::Erf((x+19.01079)/24.79485)+0.10597;
      else if (idx == 73  ) num = 0.90469*TMath::Erf((x-4.91903)/7.23923)+0.08115;
      else if (idx == 74  ) num = 0.90407*TMath::Erf((x+4.62331)/16.58946)+0.08572;
      else if (idx == 75  ) num = 0.88814*TMath::Erf((x-7.66080)/4.68103)+0.06625;
      else if (idx == 76  ) num = 0.90136*TMath::Erf((x-6.16711)/5.53063)+0.07595;
      else if (idx == 77  ) num = 0.89806*TMath::Erf((x-5.46117)/7.04178)+0.07636;
      else if (idx == 78  ) num = 0.90165*TMath::Erf((x+4.73556)/15.68303)+0.08414;
      else if (idx == 79  ) num = 0.90253*TMath::Erf((x+4.40252)/15.34417)+0.08356;
      else if (idx == 80  ) num = 0.89910*TMath::Erf((x-6.85353)/5.11505)+0.07405;
      else if (idx == 81  ) num = 0.89894*TMath::Erf((x+0.23209)/10.64842)+0.07753;
      else if (idx == 82  ) num = 0.91269*TMath::Erf((x-5.82640)/7.90385)+0.07422;
      else if (idx == 83  ) num = 0.90350*TMath::Erf((x+7.27493)/17.36477)+0.08641;
      else if (idx == 84  ) num = 0.89429*TMath::Erf((x+6.74802)/14.68283)+0.07743;
      else if (idx == 85  ) num = 0.90212*TMath::Erf((x+0.78736)/13.77569)+0.08343;
      else if (idx == 86  ) num = 0.90122*TMath::Erf((x+0.44470)/11.89901)+0.08062;
      else if (idx == 87  ) num = 0.90500*TMath::Erf((x-7.46940)/4.42887)+0.07774;
      else if (idx == 88  ) num = 0.90667*TMath::Erf((x-3.45589)/8.62671)+0.08338;
      else if (idx == 89  ) num = 0.88888*TMath::Erf((x+19.14235)/24.12037)+0.10140;
      else if (idx == 90  ) num = 0.90564*TMath::Erf((x+3.09013)/15.27461)+0.08659;
      else if (idx == 91  ) num = 0.33500*TMath::Erf((x+6.65577)/23.02741)+0.65574;
      else if (idx == 92  ) num = 0.90089*TMath::Erf((x+6.50459)/15.51010)+0.08377;
      else if (idx == 93  ) num = 0.72629*TMath::Erf((x+13.37400)/14.55272)+0.25777;
      else if (idx == 94  ) num = 0.90709*TMath::Erf((x+2.80015)/15.38912)+0.08808;
      else if (idx == 95  ) num = 0.90771*TMath::Erf((x+7.52000)/18.62786)+0.09000;
      else if (idx == 96  ) num = 0.89229*TMath::Erf((x-2.56204)/7.93210)+0.07004;
      else if (idx == 97  ) num = 0.90720*TMath::Erf((x+7.81938)/18.50317)+0.08955;
      else if (idx == 98  ) num = 0.90319*TMath::Erf((x-1.70123)/8.52137)+0.07584;
      else if (idx == 99  ) num = 0.90271*TMath::Erf((x-2.73111)/9.69981)+0.08144;
      else if (idx == 100 ) num = 0.89978*TMath::Erf((x+19.88395)/25.94129)+0.09428;
      // ADD THE FUNCTIONS FOR SYSTS BELOW! +1 SIGMA (IDX==-1) AND -1 SIGMA (IDX==-2) FIXME missing for now
      else if (idx == -1   ) num = 0.89721*TMath::Erf((x+14.41411)/9.38091)+0.08283;
      else if (idx == -2   ) num = 0.89721*TMath::Erf((x+14.41411)/9.38091)+0.08283;
   }

   if (idx==200) den = 1.;
   if (idx==300) num = den*den;

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
