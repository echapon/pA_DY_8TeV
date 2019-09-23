{
   // EC version
   // if (TString(gSystem->Getenv("HOSTNAME")) == "emilien-ThinkPadX1" || TString(gSystem->Getenv("SESSION_MANAGER")).Contains("emilien-ThinkPadX1")) {
   //    gSystem->SetIncludePath("-I/afs/cern.ch/user/e/echapon/workspace/private/xFitter/deps/lhapdf/include -I.");
   //    gSystem->AddLinkedLibs("/afs/cern.ch/user/e/echapon/workspace/private/xFitter/deps/lhapdf/lib/libLHAPDF.so");
   //    printf("Setup done for Emilien's computer\n");
   // }
   
   // CMSSW_10_3_0 version
   if (TString(gSystem->Getenv("CMSSW_RELEASE_BASE")).Contains("CMSSW_10")) {
      gSystem->SetIncludePath("-I/cvmfs/cms.cern.ch/slc6_amd64_gcc700/external/lhapdf/6.2.1-ogkkac2/include -I.");
      gSystem->AddLinkedLibs("/cvmfs/cms.cern.ch/slc6_amd64_gcc700/external/lhapdf/6.2.1-ogkkac2/lib/libLHAPDF.so");
      printf("Setup done for CMSSW 10\n");
   }

   // CVMFS version (not working with ROOT <= 6.06, which is all I could find on lxplus...)
   // gSystem->SetIncludePath("-I/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/lhapdf/6.1.6-ikhhed/include/");
   // gSystem->AddLinkedLibs("/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/lhapdf/6.1.6-ikhhed/lib/libLHAPDF.so");

   // setup LHAPDF for lxplus
   if (TString(gSystem->Getenv("HOSTNAME")).Contains("lxplus")) {
      gROOT->ProcessLine(".include /afs/cern.ch/work/e/echapon/private/xFitter/deps/lhapdf/include/");
      gSystem->Load("/afs/cern.ch/work/e/echapon/private/xFitter/deps/lhapdf/lib/libLHAPDF.so");
   }
}
