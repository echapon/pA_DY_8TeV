#include "DrawControlPlotTool.h"

void DrawControlPlot(TString version="None", Bool_t DrawDataDriven = kFALSE, TString NormType = "Lumi" )
{
	DrawControlPlotTool *tool = new DrawControlPlotTool(version, DrawDataDriven, NormType);
	// tool->GenLevelMassSpectrum();
	tool->SetupHistogramNames();
	tool->LoopForHistograms(1);
}

