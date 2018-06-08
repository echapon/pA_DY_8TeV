{
   // EC version
   // if (TString(gSystem->Getenv("HOSTNAME")) == "emilien-ThinkPadX1" || TString(gSystem->Getenv("SESSION_MANAGER")).Contains("emilien-ThinkPadX1")) {
   //    gSystem->SetIncludePath("-I/afs/cern.ch/user/e/echapon/workspace/private/xFitter/deps/lhapdf/include -I.");
   //    gSystem->AddLinkedLibs("/afs/cern.ch/user/e/echapon/workspace/private/xFitter/deps/lhapdf/lib/libLHAPDF.so");
   //    printf("Setup done for Emilien's computer\n");
   // }

   // CVMFS version (not working with ROOT <= 6.06, which is all I could find on lxplus...)
   // gSystem->SetIncludePath("-I/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/lhapdf/6.1.6-ikhhed/include/");
   // gSystem->AddLinkedLibs("/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/lhapdf/6.1.6-ikhhed/lib/libLHAPDF.so");
}
