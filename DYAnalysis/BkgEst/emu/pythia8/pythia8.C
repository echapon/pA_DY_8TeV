/// \file
/// \ingroup tutorial_pythia
/// pythia8 basic example
///
/// to run, do:
///
/// ~~~{.cpp}
///  root > .x pythia8.C
/// ~~~
///
/// Note that before executing this script, for some Pythia8 builds:
///
///  - the env variable PYTHIA8 must point to the pythia8100 (or newer) directory
///  - the env variable PYTHIA8DATA must be defined and it must point to $PYTHIA8/xmldoc
///
/// \macro_code
///
/// \author Andreas Morsch

#include "TSystem.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TPythia8.h"
#include "TParticle.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

void pythia8(Int_t seed = 42, Int_t nev  = 10000, Int_t ndeb = 1)
{
// Load libraries
   gSystem->Load("libEG");
   gSystem->Load("libEGPythia8");
// Tree
   TFile *f = TFile::Open(Form("tree_BBar_emu_s%d_nev%d.root",seed,nev),"RECREATE");
   TTree *tr = new TTree("tree","tree");
   float pt_e,eta_e,phi_e; int charge_e;
   float pt_mu,eta_mu,phi_mu; int charge_mu;
   float mass_emu, pt_emu, rap_emu;
   tr->Branch("pt_e",&pt_e,"pt_e/F");
   tr->Branch("eta_e",&eta_e,"eta_e/F");
   tr->Branch("phi_e",&phi_e,"phi_e/F");
   tr->Branch("charge_e",&charge_e,"charge_e/I");
   tr->Branch("pt_mu",&pt_mu,"pt_mu/F");
   tr->Branch("eta_mu",&eta_mu,"eta_mu/F");
   tr->Branch("phi_mu",&phi_mu,"phi_mu/F");
   tr->Branch("charge_mu",&charge_mu,"charge_mu/I");
   tr->Branch("mass_emu",&mass_emu,"mass_emu/F");
   tr->Branch("pt_emu",&pt_emu,"pt_emu/F");
   tr->Branch("rap_emu",&rap_emu,"rap_emu/F");


// Array of particles
   TClonesArray* particles = new TClonesArray("TParticle", 1000);
// Create pythia8 object
   TPythia8* pythia8 = new TPythia8();

#if PYTHIA_VERSION_INTEGER == 8235
   // Pythia 8.235 is known to cause crashes:
   printf("ABORTING PYTHIA8 TUTORIAL!\n");
   printf("The version of Pythia you use is known to case crashes due to memory errors.\n");
   printf("They have been reported to the authors; the Pythia versions 8.1... are known to work.\n");
   return;
#endif

// Configure
   pythia8->ReadString("HardQCD:gg2bbbar    = on");
   pythia8->ReadString("HardQCD:qqbar2bbbar = on");
   pythia8->ReadString("HardQCD:hardbbbar   = on");
   pythia8->ReadString("PhaseSpace:pTHatMin = 10.");
   pythia8->ReadString("Random:setSeed = on");
   // use a reproducible seed: always the same results for the tutorial.
   pythia8->ReadString(Form("Random:seed = %d", seed));


// Initialize

   pythia8->Initialize(2212 /* p */, 2212 /* p */, 14000. /* TeV */);
   int nout = 0;

// Event loop
   for (Int_t iev = 0; iev < nev; iev++) {
      pythia8->GenerateEvent();
      if (iev < ndeb) pythia8->EventListing();
      pythia8->ImportParticles(particles,"All");
      Int_t np = particles->GetEntriesFast();

      pt_e = -1; pt_mu = -1;

// Particle loop
      for (Int_t ip = 0; ip < np; ip++) {
         TParticle* part = (TParticle*) particles->At(ip);
         if (part->Pt()<10) continue;
         Int_t ist = part->GetStatusCode();
         // Positive codes are final particles.
         if (ist <= 0) continue;
         Int_t pdg = part->GetPdgCode();
         // only look for electrons or muons
         if (abs(pdg) != 11 && abs(pdg) != 13) continue;
         // is there a B meson up the history? (TODO)
         Float_t charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();

         if (abs(pdg)==11) {
            pt_e = part->Pt();
            eta_e = part->Eta();
            phi_e = part->Phi();
            charge_e = charge;
         }
         if (abs(pdg)==13) {
            pt_mu = part->Pt();
            eta_mu = part->Eta();
            phi_mu = part->Phi();
            charge_mu = charge;
         }
      }

      if (pt_e<0 || pt_mu<0) continue;
      TLorentzVector tlve, tlvmu;
      tlve.SetPtEtaPhiM(pt_e,eta_e,phi_e,0.5e-3);
      tlvmu.SetPtEtaPhiM(pt_mu,eta_mu,phi_mu,106e-3);
      TLorentzVector tlvemu = tlve+tlvmu;
      mass_emu = tlvemu.M();
      pt_emu = tlvemu.Pt();
      rap_emu = tlvemu.Rapidity();

      tr->Fill();
      nout++;
   }

   cout << "Wrote " << nout << " after " << nev << " trials" << endl;

   pythia8->PrintStatistics();

   f->Write();
   f->Close();
 }
