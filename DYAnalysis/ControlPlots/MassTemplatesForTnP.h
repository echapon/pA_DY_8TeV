//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jan 24 14:29:34 2019 by ROOT version 6.06/08
// from TTree tr_DYMuMu30/tree DYMuMu30
// found on file: ROOTFile_Histogram_InvMass_PAL3Mu12_MC_MomCorr00_rewboth_notnprew.root
//////////////////////////////////////////////////////////

#ifndef MassTemplatesForTnP_h
#define MassTemplatesForTnP_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class MassTemplatesForTnP {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         diMass;
   Float_t         diRapidity;
   Float_t         diPt;
   Float_t         diPhistar;
   Float_t         deltaPhi;
   Float_t         minpt;
   Float_t         pt1;
   Float_t         pt2;
   Float_t         eta1;
   Float_t         eta2;
   Float_t         phi1;
   Float_t         phi2;
   Float_t         maxabseta;
   Float_t         maxrelPFiso;
   Float_t         relPFiso1;
   Float_t         relPFiso2;
   Float_t         trkiso1;
   Float_t         trkiso2;
   Int_t           isSoft1;
   Int_t           isSoft2;
   Int_t           isLoose1;
   Int_t           isLoose2;
   Int_t           isMedium1;
   Int_t           isMedium2;
   Int_t           isTight1;
   Int_t           isTight2;
   Int_t           isMyTight1;
   Int_t           isMyTight2;
   Float_t         maxabsdxy;
   Float_t         dxy1;
   Float_t         dxy2;
   Float_t         maxabsdz;
   Float_t         d01;
   Float_t         d02;
   Float_t         dsz1;
   Float_t         dsz2;
   Float_t         dz1;
   Float_t         dz2;
   Float_t         dxyBS1;
   Float_t         dxyBS2;
   Float_t         dszBS1;
   Float_t         dszBS2;
   Float_t         dzBS1;
   Float_t         dzBS2;
   Float_t         dxyVTX1;
   Float_t         dxyVTX2;
   Float_t         dszVTX1;
   Float_t         dszVTX2;
   Float_t         dzVTX1;
   Float_t         dzVTX2;
   Float_t         vtxnormchi2;
   Float_t         vtxprob;
   Float_t         hiHF;
   Float_t         pfMET;
   Float_t         pfMETtype1;
   Int_t           sign;
   Float_t         weight;
   Float_t         genpt1;
   Float_t         geneta1;
   Float_t         genphi1;
   Float_t         genpt2;
   Float_t         geneta2;
   Float_t         genphi2;

   // List of branches
   TBranch        *b_diMass;   //!
   TBranch        *b_diRapidity;   //!
   TBranch        *b_diPt;   //!
   TBranch        *b_diPhistar;   //!
   TBranch        *b_deltaPhi;   //!
   TBranch        *b_minpt;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_pt2;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_maxabseta;   //!
   TBranch        *b_maxrelPFiso;   //!
   TBranch        *b_relPFiso1;   //!
   TBranch        *b_relPFiso2;   //!
   TBranch        *b_trkiso1;   //!
   TBranch        *b_trkiso2;   //!
   TBranch        *b_isSoft1;   //!
   TBranch        *b_isSoft2;   //!
   TBranch        *b_isLoose1;   //!
   TBranch        *b_isLoose2;   //!
   TBranch        *b_isMedium1;   //!
   TBranch        *b_isMedium2;   //!
   TBranch        *b_isTight1;   //!
   TBranch        *b_isTight2;   //!
   TBranch        *b_isMyTight1;   //!
   TBranch        *b_isMyTight2;   //!
   TBranch        *b_maxabsdxy;   //!
   TBranch        *b_dxy1;   //!
   TBranch        *b_dxy2;   //!
   TBranch        *b_maxabsdz;   //!
   TBranch        *b_d01;   //!
   TBranch        *b_d02;   //!
   TBranch        *b_dsz1;   //!
   TBranch        *b_dsz2;   //!
   TBranch        *b_dz1;   //!
   TBranch        *b_dz2;   //!
   TBranch        *b_dxyBS1;   //!
   TBranch        *b_dxyBS2;   //!
   TBranch        *b_dszBS1;   //!
   TBranch        *b_dszBS2;   //!
   TBranch        *b_dzBS1;   //!
   TBranch        *b_dzBS2;   //!
   TBranch        *b_dxyVTX1;   //!
   TBranch        *b_dxyVTX2;   //!
   TBranch        *b_dszVTX1;   //!
   TBranch        *b_dszVTX2;   //!
   TBranch        *b_dzVTX1;   //!
   TBranch        *b_dzVTX2;   //!
   TBranch        *b_vtxnormchi2;   //!
   TBranch        *b_vtxprob;   //!
   TBranch        *b_hiHF;   //!
   TBranch        *b_pfMET;   //!
   TBranch        *b_pfMETtype1;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_genpt1;   //!
   TBranch        *b_geneta1;   //!
   TBranch        *b_genphi1;   //!
   TBranch        *b_genpt2;   //!
   TBranch        *b_geneta2;   //!
   TBranch        *b_genphi2;   //!

   MassTemplatesForTnP(TTree *tree=0);
   virtual ~MassTemplatesForTnP();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const char* filename="MassTemplatesForTnP.root");
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MassTemplatesForTnP_cxx
MassTemplatesForTnP::MassTemplatesForTnP(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ROOTFile_Histogram_InvMass_PAL3Mu12_MC_MomCorr00_rewboth_notnprew.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ROOTFile_Histogram_InvMass_PAL3Mu12_MC_MomCorr00_rewboth_notnprew.root");
      }
      f->GetObject("tr_DYMuMu30",tree);

   }
   Init(tree);
}

MassTemplatesForTnP::~MassTemplatesForTnP()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MassTemplatesForTnP::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MassTemplatesForTnP::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MassTemplatesForTnP::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("diMass", &diMass, &b_diMass);
   fChain->SetBranchAddress("diRapidity", &diRapidity, &b_diRapidity);
   fChain->SetBranchAddress("diPt", &diPt, &b_diPt);
   fChain->SetBranchAddress("diPhistar", &diPhistar, &b_diPhistar);
   fChain->SetBranchAddress("deltaPhi", &deltaPhi, &b_deltaPhi);
   fChain->SetBranchAddress("minpt", &minpt, &b_minpt);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt2);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("maxabseta", &maxabseta, &b_maxabseta);
   fChain->SetBranchAddress("maxrelPFiso", &maxrelPFiso, &b_maxrelPFiso);
   fChain->SetBranchAddress("relPFiso1", &relPFiso1, &b_relPFiso1);
   fChain->SetBranchAddress("relPFiso2", &relPFiso2, &b_relPFiso2);
   fChain->SetBranchAddress("trkiso1", &trkiso1, &b_trkiso1);
   fChain->SetBranchAddress("trkiso2", &trkiso2, &b_trkiso2);
   fChain->SetBranchAddress("isSoft1", &isSoft1, &b_isSoft1);
   fChain->SetBranchAddress("isSoft2", &isSoft2, &b_isSoft2);
   fChain->SetBranchAddress("isLoose1", &isLoose1, &b_isLoose1);
   fChain->SetBranchAddress("isLoose2", &isLoose2, &b_isLoose2);
   fChain->SetBranchAddress("isMedium1", &isMedium1, &b_isMedium1);
   fChain->SetBranchAddress("isMedium2", &isMedium2, &b_isMedium2);
   fChain->SetBranchAddress("isTight1", &isTight1, &b_isTight1);
   fChain->SetBranchAddress("isTight2", &isTight2, &b_isTight2);
   fChain->SetBranchAddress("isMyTight1", &isMyTight1, &b_isMyTight1);
   fChain->SetBranchAddress("isMyTight2", &isMyTight2, &b_isMyTight2);
   fChain->SetBranchAddress("maxabsdxy", &maxabsdxy, &b_maxabsdxy);
   fChain->SetBranchAddress("dxy1", &dxy1, &b_dxy1);
   fChain->SetBranchAddress("dxy2", &dxy2, &b_dxy2);
   fChain->SetBranchAddress("maxabsdz", &maxabsdz, &b_maxabsdz);
   fChain->SetBranchAddress("d01", &d01, &b_d01);
   fChain->SetBranchAddress("d02", &d02, &b_d02);
   fChain->SetBranchAddress("dsz1", &dsz1, &b_dsz1);
   fChain->SetBranchAddress("dsz2", &dsz2, &b_dsz2);
   fChain->SetBranchAddress("dz1", &dz1, &b_dz1);
   fChain->SetBranchAddress("dz2", &dz2, &b_dz2);
   fChain->SetBranchAddress("dxyBS1", &dxyBS1, &b_dxyBS1);
   fChain->SetBranchAddress("dxyBS2", &dxyBS2, &b_dxyBS2);
   fChain->SetBranchAddress("dszBS1", &dszBS1, &b_dszBS1);
   fChain->SetBranchAddress("dszBS2", &dszBS2, &b_dszBS2);
   fChain->SetBranchAddress("dzBS1", &dzBS1, &b_dzBS1);
   fChain->SetBranchAddress("dzBS2", &dzBS2, &b_dzBS2);
   fChain->SetBranchAddress("dxyVTX1", &dxyVTX1, &b_dxyVTX1);
   fChain->SetBranchAddress("dxyVTX2", &dxyVTX2, &b_dxyVTX2);
   fChain->SetBranchAddress("dszVTX1", &dszVTX1, &b_dszVTX1);
   fChain->SetBranchAddress("dszVTX2", &dszVTX2, &b_dszVTX2);
   fChain->SetBranchAddress("dzVTX1", &dzVTX1, &b_dzVTX1);
   fChain->SetBranchAddress("dzVTX2", &dzVTX2, &b_dzVTX2);
   fChain->SetBranchAddress("vtxnormchi2", &vtxnormchi2, &b_vtxnormchi2);
   fChain->SetBranchAddress("vtxprob", &vtxprob, &b_vtxprob);
   fChain->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
   fChain->SetBranchAddress("pfMET", &pfMET, &b_pfMET);
   fChain->SetBranchAddress("pfMETtype1", &pfMETtype1, &b_pfMETtype1);
   fChain->SetBranchAddress("sign", &sign, &b_sign);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("genpt1", &genpt1, &b_genpt1);
   fChain->SetBranchAddress("geneta1", &geneta1, &b_geneta1);
   fChain->SetBranchAddress("genphi1", &genphi1, &b_genphi1);
   fChain->SetBranchAddress("genpt2", &genpt2, &b_genpt2);
   fChain->SetBranchAddress("geneta2", &geneta2, &b_geneta2);
   fChain->SetBranchAddress("genphi2", &genphi2, &b_genphi2);
   Notify();
}

Bool_t MassTemplatesForTnP::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MassTemplatesForTnP::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MassTemplatesForTnP::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MassTemplatesForTnP_cxx
