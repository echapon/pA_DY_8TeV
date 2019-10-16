#include "DrawControlPlotTool.h"

void DrawControlPlot(TString version="PAL3Mu12", 
      Int_t DrawDataDriven = 0,      // 0 -> no data-driven, 1 -> fakerate, 2 -> ABCD
      TString NormType = "Lumi",     // Lumi or Zpeak
      TString MomCor = "MomCorr00",    // MomCorr or MomCorrH or MomUnCorr
      TString Rew = "rewboth",       // rewboth or norew or rewplus or rewminus
      bool    doTnpRew = true,         // apply tnp weights also when filling histos or not?
      bool    doZptRew = true,         // apply Zpt rew?
      bool    PrintTex = false)
{
	DrawControlPlotTool *tool = new DrawControlPlotTool(version, DrawDataDriven, NormType, MomCor, Rew, doTnpRew, doZptRew);
   if (PrintTex) tool->SetTexOptions(true);
   tool->GenLevelMassSpectrum();
   tool->SetupHistogramNames();
   tool->LoopForHistograms(-1);
   // tool->LoopForHistograms(15);
}

