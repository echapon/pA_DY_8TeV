void SSOS(TString name) {
   TFile *f = TFile::Open(name);
   TTree* tr_QCD = (TTree*) f->Get("tr_QCD");
   double bins[11] = {0,23.5,25.7,28.1,30.9,34.3,37.5,42.9,50.3,69.5,200};
   double bin[11] = {0,23.5,25.7,28.1,30.9,34.3,37.5,42.9,50.3,69.5,200};
   TH1D *hnum = new TH1D("hnum",";M;SS/OS",10,bin);
   TH1D *hden = new TH1D("hden",";M;SS/OS",10,bin);
   // tr_QCD->Draw("diMass>>+hnum","diMass>10&&(sign!=0)&&trkiso1<0.2&&trkiso2<0.2&&pt1>15&&pt2>10&&vtxnormchi2<20&&isTight1==1&&isTight2==1");
   // tr_QCD->Draw("diMass>>+hden","diMass>10&&(sign==0)&&trkiso1<0.2&&trkiso2<0.2&&pt1>15&&pt2>10&&vtxnormchi2<20&&isTight1==1&&isTight2==1");
   // tr_QCD->Draw("diMass>>+hnum","diMass>10&&(sign!=0)&&pt1>15&&pt2>10&&vtxnormchi2<20&&isTight1==1&&isTight2==1");
   // tr_QCD->Draw("diMass>>+hden","diMass>10&&(sign==0)&&pt1>15&&pt2>10&&vtxnormchi2<20&&isTight1==1&&isTight2==1");
   tr_QCD->Draw("diMass>>+hnum","diMass>10&&(sign!=0)&&pt1>15&&pt2>10");
   tr_QCD->Draw("diMass>>+hden","diMass>10&&(sign==0)&&pt1>15&&pt2>10");
   TGraphAsymmErrors *g = new TGraphAsymmErrors();
   g->Divide(hnum,hden,"pois");
   g->Draw("AP");
}
