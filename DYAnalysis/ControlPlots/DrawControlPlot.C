#include "DrawControlPlotTool.h"

void DrawControlPlot(TString version="None", 
      Bool_t DrawDataDriven = kFALSE, 
      TString NormType = "Lumi",     // Lumi or Zpeak
      TString MomCor = "MomCorr",    // MomCorr or MomUnCorr
      TString Rew = "rewboth",       // rewboth or norew or rewplus or rewminus
      bool    doTnpRew = true,         // apply tnp weights also when filling histos or not?
      bool    PrintTex = false)
{
	DrawControlPlotTool *tool = new DrawControlPlotTool(version, DrawDataDriven, NormType, MomCor, Rew, doTnpRew);
   if (PrintTex) tool->SetTexOptions(true);
   tool->GenLevelMassSpectrum();
   tool->SetupHistogramNames();
   // tool->LoopForHistograms(-1);
   tool->LoopForHistograms(1);
}

