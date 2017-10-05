#include "DrawControlPlotTool.h"

void DrawControlPlot(TString version="None", 
      Bool_t DrawDataDriven = kFALSE, 
      TString NormType = "Lumi", // Lumi or Zpeak
      TString MomCor = "MomCorr", // MomCorr or MomUnCorr
      TString Rew = "rewboth" ) // rewboth or norew or rewplus or rewminus
{
	DrawControlPlotTool *tool = new DrawControlPlotTool(version, DrawDataDriven, NormType, MomCor, Rew);
   tool->GenLevelMassSpectrum();
   // tool->SetupHistogramNames();
   // tool->LoopForHistograms(-1);
   // tool->LoopForHistograms(1);
}

