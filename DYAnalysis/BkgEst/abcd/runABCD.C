// #include "Math/IFunction.h"
// #include "Math/IParamFunction.h"
//  
// class MyFuncA;
// class MyFuncB;
// class MyFuncC;
// class MyFuncD;
// class MyFuncE;
// class MyFuncF;

void runABCD(TString trigger="PAL3Mu12") { 
   TFile *fdata = TFile::Open("histos_ABCD_Data_" + trigger + ".root");
   TFile *fmc = TFile::Open("histos_ABCD_Powheg_" + trigger + ".root");
   TFile *fout = TFile::Open("output_ABCD_" + trigger + ".root","RECREATE");
   TDirectory *tdir_inputs = fout->mkdir("inputs");

   TDirectory *tdir_Data1 = (TDirectory*) fdata->Get("Data1");

   // loop on histograms
   TIter next(tdir_Data1->GetListOfKeys()); TObject *obj;
   while ((obj = next())) {
      obj = ((TKey*) obj)->ReadObj();
      if (TString(obj->ClassName()) == "TH1D") {
         TH1D *hist = dynamic_cast<TH1D*>(obj);
         TString hname = hist->GetName();
         tdir_inputs->cd();

         // data histo
         TH1D *hdata = (TH1D*) hist->Clone(hname + "_Data");
         hdata->Add((TH1D*) fdata->Get("Data2/" + hname));

         // MC histo
         TH1D *hMC = (TH1D*) ((TH1D*) fmc->Get("TT/" + hname))->Clone(hname + "_MC");
         TH1D *htmp = (TH1D*) fmc->Get("TW/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("WW/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("WZ/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("ZZ/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("DYMuMu1030/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("DYMuMu30/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("DYMuMu1030_PbP/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("DYTauTau1030/" + hname);
         if (htmp) hMC->Add(htmp);
         htmp = (TH1D*) fmc->Get("DYTauTau30/" + hname);
         if (htmp) hMC->Add(htmp);

         // subtract MC from data
         TH1D *hdatasub = (TH1D*) hdata->Clone(hname + "_DataMinusMC");
         hdatasub->Add(hMC,-1);
      } // object is histo
   } // loop on objects

   // now do the ABCD method
   TString varnames[5] = {"Mass","Rap1560","Rap60120","Pt","Phistar"};

   for (int niso=0; niso<=3; niso++) {
      TString methtag;
      if (niso==2) methtag="1and2isoTo2iso";
      else if (niso==1) methtag="1isoTo2iso";
      else if (niso==0) methtag="0isoTo2iso";
      else methtag="0isoTo1iso";

      TDirectory *tdir_outputs = fout->mkdir("outputs_" + methtag);
      tdir_outputs->cd();

      for (int i=0; i<5; i++) {
         TH1D *h_0iso_SS = (TH1D*) tdir_inputs->Get("h_" + varnames[i] + "_0iso_SS_DataMinusMC");
         TH1D *h_0iso_OS = (TH1D*) tdir_inputs->Get("h_" + varnames[i] + "_0iso_OS_DataMinusMC");
         TH1D *h_1iso_SS = (TH1D*) tdir_inputs->Get("h_" + varnames[i] + "_1iso_SS_DataMinusMC");
         TH1D *h_1iso_OS = (TH1D*) tdir_inputs->Get("h_" + varnames[i] + "_1iso_OS_DataMinusMC");
         TH1D *h_2iso_SS = (TH1D*) tdir_inputs->Get("h_" + varnames[i] + "_2iso_SS_DataMinusMC");
         TH1D *h_2iso_OS = (TH1D*) tdir_inputs->Get("h_" + varnames[i] + "_2iso_OS_DataMinusMC");

         TH1D *houtput = (TH1D*) h_0iso_SS->Clone("hbkg_OS_2iso_" + varnames[i]);
         if (niso==3) houtput->SetName("hbkg_OS_1iso_" + varnames[i]);

         for (int i=1; i<=houtput->GetNbinsX(); i++) {
            double A, B, C, D, dA, dB, dC, dD;
            if (niso==2) {
               A = h_2iso_SS->GetBinContent(i);
               dA = h_2iso_SS->GetBinError(i);
               C = h_1iso_SS->GetBinContent(i) + h_0iso_SS->GetBinContent(i);
               dC = sqrt(pow(h_1iso_SS->GetBinError(i),2) + pow(h_0iso_SS->GetBinError(i),2));
               D = h_1iso_OS->GetBinContent(i) + h_0iso_OS->GetBinContent(i);
               dD = sqrt(pow(h_1iso_OS->GetBinError(i),2) + pow(h_0iso_OS->GetBinError(i),2));
            } else if (niso==1) {
               A = h_2iso_SS->GetBinContent(i);
               dA = h_2iso_SS->GetBinError(i);
               C = h_1iso_SS->GetBinContent(i) ;
               dC = h_1iso_SS->GetBinError(i);
               D = h_1iso_OS->GetBinContent(i);
               dD = h_1iso_OS->GetBinError(i);
            } else if (niso==0) {
               A = h_2iso_SS->GetBinContent(i);
               dA = h_2iso_SS->GetBinError(i);
               C = h_0iso_SS->GetBinContent(i) ;
               dC = h_0iso_SS->GetBinError(i);
               D = h_0iso_OS->GetBinContent(i);
               dD = h_0iso_OS->GetBinError(i);
            } else {
               A = h_1iso_SS->GetBinContent(i);
               dA = h_1iso_SS->GetBinError(i);
               C = h_0iso_SS->GetBinContent(i) ;
               dC = h_0iso_SS->GetBinError(i);
               D = h_0iso_OS->GetBinContent(i);
               dD = h_0iso_OS->GetBinError(i);
            }

            B = (C>0) ? (A*D)/C : 0;
            dB = (A>0 && C>0 && D>0) ? B*sqrt(pow(dA/A,2)+pow(dD/D,2)+pow(dC/C,2)) : 0;

            houtput->SetBinContent(i,B);
            houtput->SetBinError(i,dB);
         } // bin loop
      } // variable loop
   } // niso loop

   fout->Write();
   fout->Close();
}

// class MyFuncA: 
//    public ROOT::Math::IParametricGradFunctionMultiDim{
//       private:
//          const double* pars;

//       public:
//          double DoEvalPar(const double* x, const double* p) const{
//             return x[0] + x[1] - p[0];
//          }
//          unsigned int NDim() const{
//             return 6;
//          }
//          ROOT::Math::IParametricGradFunctionMultiDim* Clone() const{
//             return new MyFuncA();
//          }
//          const double* Parameters() const {
//             return pars;
//          }
//          void SetParameters(const double* p){
//             pars = p;
//          }
//          unsigned int NPar() const{
//             return 6;
//          }
//          double DoParameterDerivative(const double* x, const double* p, unsigned int ipar) const{
//             if ( ipar == 0 || ipar == 1 )
//                return 1;
//             else
//                return 0;
//          }
//    };
// class MyFuncB: 
//    public ROOT::Math::IParametricGradFunctionMultiDim{
//       private:
//          const double* pars;

//       public:
//          double DoEvalPar(const double* x, const double* p) const{
//             return x[0]*x[4] + x[1]*x[5] - p[1];
//          }
//          unsigned int NDim() const{
//             return 6;
//          }
//          ROOT::Math::IParametricGradFunctionMultiDim* Clone() const{
//             return new MyFuncA();
//          }
//          const double* Parameters() const {
//             return pars;
//          }
//          void SetParameters(const double* p){
//             pars = p;
//          }
//          unsigned int NPar() const{
//             return 6;
//          }
//          double DoParameterDerivative(const double* x, const double* p, unsigned int ipar) const{
//             if ( ipar == 0 )
//                return x[4];
//             else if ( ipar == 1 )
//                return x[5];
//             else if ( ipar == 4 )
//                return x[0];
//             else if ( ipar == 5 )
//                return x[1];
//             else
//                return 0;
//          }
//    };
// class MyFuncC: 
//    public ROOT::Math::IParametricGradFunctionMultiDim{
//       private:
//          const double* pars;

//       public:
//          double DoEvalPar(const double* x, const double* p) const{
//             return x[0]*x[2] + x[1]*x[3] - p[2];
//          }
//          unsigned int NDim() const{
//             return 6;
//          }
//          ROOT::Math::IParametricGradFunctionMultiDim* Clone() const{
//             return new MyFuncA();
//          }
//          const double* Parameters() const {
//             return pars;
//          }
//          void SetParameters(const double* p){
//             pars = p;
//          }
//          unsigned int NPar() const{
//             return 6;
//          }
//          double DoParameterDerivative(const double* x, const double* p, unsigned int ipar) const{
//             if ( ipar == 0 )
//                return x[2];
//             else if ( ipar == 1 )
//                return x[3];
//             else if ( ipar == 2 )
//                return x[0];
//             else if ( ipar == 3 )
//                return x[1];
//             else
//                return 0;
//          }
//    };
// class MyFuncD: 
//    public ROOT::Math::IParametricGradFunctionMultiDim{
//       private:
//          const double* pars;

//       public:
//          double DoEvalPar(const double* x, const double* p) const{
//             return x[0]*x[2]*x[4] + x[1]*x[3]*x[5] - p[3];
//          }
//          unsigned int NDim() const{
//             return 6;
//          }
//          ROOT::Math::IParametricGradFunctionMultiDim* Clone() const{
//             return new MyFuncA();
//          }
//          const double* Parameters() const {
//             return pars;
//          }
//          void SetParameters(const double* p){
//             pars = p;
//          }
//          unsigned int NPar() const{
//             return 6;
//          }
//          double DoParameterDerivative(const double* x, const double* p, unsigned int ipar) const{
//             if ( ipar == 0 )
//                return x[2]*x[4];
//             else if ( ipar == 1 )
//                return x[3]*x[5];
//             else if ( ipar == 2 )
//                return x[0]*x[4];
//             else if ( ipar == 3 )
//                return x[1]*x[5];
//             else if ( ipar == 4 )
//                return x[0]*x[2];
//             else if ( ipar == 5 )
//                return x[1]*x[3];
//             else
//                return 0;
//          }
//    };
// class MyFuncE: 
//    public ROOT::Math::IParametricGradFunctionMultiDim{
//       private:
//          const double* pars;

//       public:
//          double DoEvalPar(const double* x, const double* p) const{
//             return x[0]*x[2]*x[2] + x[1]*x[3]*x[3] - p[4];
//          }
//          unsigned int NDim() const{
//             return 6;
//          }
//          ROOT::Math::IParametricGradFunctionMultiDim* Clone() const{
//             return new MyFuncA();
//          }
//          const double* Parameters() const {
//             return pars;
//          }
//          void SetParameters(const double* p){
//             pars = p;
//          }
//          unsigned int NPar() const{
//             return 6;
//          }
//          double DoParameterDerivative(const double* x, const double* p, unsigned int ipar) const{
//             if ( ipar == 0 )
//                return x[2]*x[2];
//             else if ( ipar == 1 )
//                return x[3]*x[3];
//             else if ( ipar == 2 )
//                return 2*x[0]*x[2];
//             else if ( ipar == 3 )
//                return 2*x[1]*x[3];
//             else
//                return 0;
//          }
//    };
// class MyFuncF: 
//    public ROOT::Math::IParametricGradFunctionMultiDim{
//       private:
//          const double* pars;

//       public:
//          double DoEvalPar(const double* x, const double* p) const{
//             return x[0]*x[2]*x[2]*x[4] + x[1]*x[3]*x[3]*x[5] - p[5];
//          }
//          unsigned int NDim() const{
//             return 6;
//          }
//          ROOT::Math::IParametricGradFunctionMultiDim* Clone() const{
//             return new MyFuncA();
//          }
//          const double* Parameters() const {
//             return pars;
//          }
//          void SetParameters(const double* p){
//             pars = p;
//          }
//          unsigned int NPar() const{
//             return 6;
//          }
//          double DoParameterDerivative(const double* x, const double* p, unsigned int ipar) const{
//             if ( ipar == 0 )
//                return x[2]*x[2]*x[4];
//             else if ( ipar == 1 )
//                return x[3]*x[3]*x[5];
//             else if ( ipar == 2 )
//                return 2*x[0]*x[2]*x[4];
//             else if ( ipar == 3 )
//                return 2*x[1]*x[3]*x[5];
//             else if ( ipar == 4 )
//                return x[0]*x[2]*x[2];
//             else if ( ipar == 5 )
//                return x[1]*x[3]*x[3];
//             else
//                return 0;
//          }
//    };
